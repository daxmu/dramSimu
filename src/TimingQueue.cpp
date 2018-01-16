#include "TimingQueue.h"

using namespace std;

TimingQueue::TimingQueue(int csNum_, int qLen_, int bankNum_)
	:csNum(csNum_), 
	qLen(qLen_),
	rdCounter(0),
	wrCounter(0),
	reqCounter(0),
	preCounter(0),
	actCounter(0),
	mcsm(csNum, bankNum_)
	{}

bool TimingQueue::valid(){
	return !cmdq.empty();
}
bool TimingQueue::ready(){
	return cmdq.size() < qLen;
}

bool TimingQueue::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - TimingQueue is not ready and asked receive_req" << endl;
		exit(0);
		return false;
	}
	cmdq.push_back(req);
	return true;
}

bool TimingQueue::check_status(const CmdStatus &status){
	if(((int)status.needPre + 
	    (int)status.needAct + 
	    (int)status.needRd + 
	    (int)status.needWr) >= 2)
		return false;
	if(((int)status.gntPre + 
	    (int)status.gntAct + 
	    (int)status.gntRd + 
	    (int)status.gntWr) >= 2)
		return false;
	return true;
}

void TimingQueue::pop_req(){
	if(cmdq.front().length == 1)
		cmdq.pop_front();
	else
		cmdq.front().length--;
	reqCounter++;
	cout << "    TimingQueue solved req " << reqCounter << endl;
}

void TimingQueue::send_pre(const Req& req){
	cout << "    TimingQueue send pre to cs " << req.cs << ", bank " << req.bank << " ";
	if(mcsm.send_pre(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	preCounter++;
}
void TimingQueue::send_act(const Req& req){
	cout << "    TimingQueue send act to cs " << req.cs << ", bank " << req.bank << " ";
	if(mcsm.send_act(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	actCounter++;
}
void TimingQueue::send_rd(const Req& req){
	cout << "    TimingQueue send rd to cs " << req.cs << ", bank " << req.bank << " ";
	if(mcsm.send_rd(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	pop_req();
	rdCounter++;
}
void TimingQueue::send_wr(const Req& req){
	cout << "    TimingQueue send wr to cs " << req.cs << ", bank " << req.bank << " ";
	if(mcsm.send_wr(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	pop_req();
	wrCounter++;
}

bool TimingQueue::refresh_control(){
	bool is_send = false;
	for(std::size_t i = 0; i < csNum; i++){
		if(mcsm.need_refresh(i)){
			mcsm.send_ref(i);
			is_send = true;
			cout << "    TimingQueue send ref to CS " << i << endl;
			break;
		}
	}
	return is_send;
}

void TimingQueue::print_status(){
	cout << "TimingQueue print_status***" << endl;
}

void TimingQueue::print_summary(){
	cout << "    rd request num: " << rdCounter << endl 
		 << "    wr request num: " << wrCounter << endl
		 << "    rw summary: " << reqCounter << endl 
		 << endl 
		 << "    pre command num: " << preCounter << endl
		 << "    act command num: " << actCounter << endl;
}

void TimingQueue::print_summary(std::size_t cycles){
	print_summary();
	cout << "    the bus occupied rate = " << ((double)reqCounter*4/cycles) << endl;
}

bool TimingQueue::is_sameba(const Req& a, const Req& b){
	return ((a.cs == b.cs) && (a.bank == b.bank));
}
void TimingQueue::run_step(){
	
	bool is_send_ref;
	is_send_ref = refresh_control();

	if(!valid() || is_send_ref){
		//cout << "    TimingQueue not valid " << endl;
		return;
	}

	for(auto i = cmdq.begin(); i < cmdq.end(); i++){
		const Req &tmp = (*i);
		if(tmp.cs >= csNum){
			cout << "ERROR - req.cs >= csNum in TimingQueue run_step()" << endl;
			exit(0);
		}
		CmdStatus tmpStatus = mcsm.get_cmdStatus(tmp);

		if(!check_status(tmpStatus)){
			cout << "ERROR - check_status failed in TimingQueue run_step()" << endl;
			exit(0);
		}

		bool skip = false;
		for(auto j = cmdq.begin(); j < i; j++){
			if(is_sameba(*i, *j)){
				skip = true;
				break;
			}
		}
		if(skip)
			continue;

		if(tmpStatus.needPre && tmpStatus.gntPre){
			send_pre(tmp);
			break;
		}
		if(tmpStatus.needAct && tmpStatus.gntAct){
			send_act(tmp);
			break;
		}
		if(tmpStatus.needRd && tmpStatus.gntRd && (i == cmdq.begin())){
			send_rd(tmp);
			break;
		}
		if(tmpStatus.needWr && tmpStatus.gntWr && (i == cmdq.begin())){
			send_wr(tmp);
			break;
		}

	}

	mcsm.run_step();
}

void TimingQueue::update(){
	mcsm.update();
}




