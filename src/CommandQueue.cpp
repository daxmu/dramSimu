#include "CommandQueue.h"

using namespace std;

CommandQueue::CommandQueue(int csNum_, int qLen_, int bankNum_)
	:csNum(csNum_), 
	qLen(qLen_),
	cmdq(qLen){
		for(std::size_t i = 0; i < csNum; i++){
			CsStatusMachine tmp(i, bankNum_);
			csm.push_back(tmp);
		}
	}

bool CommandQueue::valid(){
	return !cmdq.is_empty();
}
bool CommandQueue::ready(){
	return !cmdq.is_full();
}

bool CommandQueue::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - CommandQueue is not ready and asked receive_req" << endl;
		exit(0);
		return false;
	}
	cmdq.write(req);
	return true;
}

bool CommandQueue::check_status(const CmdStatus &status){
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

void CommandQueue::send_pre(const Req& req){
	cout << "    CommandQueue send pre to cs " << req.cs << ", bank " << req.bank << " ";
	if(csm[req.cs].send_pre(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
}
void CommandQueue::send_act(const Req& req){
	cout << "    CommandQueue send act to cs " << req.cs << ", bank " << req.bank << " ";
	if(csm[req.cs].send_act(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
}
void CommandQueue::send_rd(const Req& req){
	cout << "    CommandQueue send rd to cs " << req.cs << ", bank " << req.bank << " ";
	if(csm[req.cs].send_rd(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	cmdq.read();
}
void CommandQueue::send_wr(const Req& req){
	cout << "    CommandQueue send wr to cs " << req.cs << ", bank " << req.bank << " ";
	if(csm[req.cs].send_wr(req))
		cout << "Success!" << endl;
	else
		cout << "Failed!" << endl;
	cmdq.read();
}

void CommandQueue::print_status(){
	cout << "CommandQueue print_status***" << endl;
	cmdq.print_status();
}

void CommandQueue::run_step(){
	print_status();

	if(!valid()){
		//cout << "    CommandQueue not valid " << endl;
		for(auto i = csm.begin(); i < csm.end(); i++){
			(*i).run_step();
		}
		return;
	}
	const Req &tmp = cmdq.get_head();
	
	if(tmp.cs >= csNum){
		cout << "ERROR - req.cs >= csNum in CommandQueue run_step()" << endl;
		exit(0);
	}
	CmdStatus tmpStatus = csm[tmp.cs].get_cmdStatus(tmp);

	if(!check_status(tmpStatus)){
		cout << "ERROR - check_status failed in CommandQueue run_step()" << endl;
		exit(0);
	}

	if(tmpStatus.needPre && tmpStatus.gntPre)
		send_pre(tmp);
	if(tmpStatus.needAct && tmpStatus.gntAct)
		send_act(tmp);
	if(tmpStatus.needRd && tmpStatus.gntRd)
		send_rd(tmp);
	if(tmpStatus.needWr && tmpStatus.gntWr)
		send_wr(tmp);
	
	for(auto i = csm.begin(); i < csm.end(); i++){
		(*i).run_step();
	}

}

void CommandQueue::update(){}


