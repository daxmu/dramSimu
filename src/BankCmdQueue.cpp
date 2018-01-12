#include "BankCmdQueue.h"

using namespace std;

BankCmdQueue::BankCmdQueue(int bcqId_, int qLen_)
	:bcqId(bcqId_),
	 qLen(qLen_){}

bool BankCmdQueue::valid(){
	return !reqList.empty();
}

bool BankCmdQueue::ready(){
	return reqList.size() < qLen;
}

bool BankCmdQueue::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - BankCmdQueue is not ready and asked receive_req " << endl;
		exit(0);
		return false;
	}
	add_req(req);
	return true;
}

#ifdef INSERT_STRATEGY_1
void BankCmdQueue::add_req(Req req){
	bool tmp = false;
	for(auto i = reqList.begin(); i != reqList.end(); i++){
		if(req.row == (*i).row){
			for(auto j = i; j != reqList.end(); j++){
				if(req.row != (*j).row){
					reqList.insert(j,req);
					tmp = true;
					break;
				}
			}
			break;
		}
	}
	if(tmp == false)
		reqList.push_back(req);
}
#endif
#ifdef INSERT_STRATEGY_2
void BankCmdQueue::add_req(Req req){
	reqList.push_back(req);
}
#endif

const Req& BankCmdQueue::get_head() {
	return reqList.front();
}

void BankCmdQueue::pop_front(){
	reqList.pop_front();
}

void BankCmdQueue::reorder(std::size_t row){
	if(reqList.empty())
		return;
	list<Req> tmp;
	for(auto i = reqList.begin(); i != reqList.end(); i++){
		if((*i).row == row){
			tmp.push_back(*i);
			i = reqList.erase(i);
			i--;
		}
	}
	reqList.splice(reqList.begin(), tmp, tmp.begin(), tmp.end());
}
size_t BankCmdQueue::get_reqNum(){
	return reqList.size();
}

void BankCmdQueue::run_step(){}
void BankCmdQueue::update(){}


CsCmdQueue::CsCmdQueue(int ccqId_, int bankNum_, int qLen_)
	:ccqId(ccqId_),
	 bankNum(bankNum_),
	 qLen(qLen_),
	 rdCounter(false),
	 wrCounter(false),
	 preCounter(false),
	 actCounter(false),
	 tempValid(false)
	 {
	 	for(std::size_t i = 0; i < bankNum; i++){
			BankCmdQueue tmp(i, qLen);
			bcq.push_back(tmp);
		}
	 }

bool CsCmdQueue::valid(){
	bool val = false;
	for(auto i = bcq.begin(); i < bcq.end(); i++){
		val = val || (*i).valid();
	}
	return val;
}

bool CsCmdQueue::ready(){
	return !tempValid;
}

bool CsCmdQueue::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - CsCmdQueue is not ready but asked to receive req " << endl;
		exit(0);
		return false;
	}
	tempReq = req;
	tempValid = true;
	return true;
}

bool CsCmdQueue::accept_req(){
	if(!tempValid)
		return false;
	if(tempReq.bank >= bankNum){
		cout << "ERROR - req bank >= bankNum in CsCmdQueue accept_req()" << endl;
		exit(0);
	}
	if(bcq[tempReq.bank].ready()){
		bcq[tempReq.bank].receive_req(tempReq);
		tempValid = false;
		return true;
	}
	return false;
}

void CsCmdQueue::send_pre(const Req& req){
	if(bcq[req.bank].get_head() != req){
		cout << "ERROR - bcq[req.bank].get_head() != req" << endl;
		exit(0);
	}
	preCounter++;
}

void CsCmdQueue::send_act(const Req& req){
	if(bcq[req.bank].get_head() != req){
		cout << "ERROR - bcq[req.bank].get_head() != req" << endl;
		exit(0);
	}
	actCounter++;
}

void CsCmdQueue::send_rd(const Req& req){
	if(bcq[req.bank].get_head() != req){
		cout << "ERROR - bcq[req.bank].get_head() != req" << endl;
		exit(0);
	}
	bcq[req.bank].pop_front();
	rdCounter++;
}

void CsCmdQueue::send_wr(const Req& req){
	if(bcq[req.bank].get_head() != req){
		cout << "ERROR - bcq[req.bank].get_head() != req" << endl;
		exit(0);
	}
	bcq[req.bank].pop_front();
	wrCounter++;
}

size_t CsCmdQueue::get_reqNum(){
	size_t sum = 0;
	for(auto i = bcq.begin(); i < bcq.end(); i++){
		sum += (*i).get_reqNum();
	}
	return sum;
}

void CsCmdQueue::run_step(){
	for(auto i = bcq.begin(); i < bcq.end(); i++)
		(*i).run_step();
}

void CsCmdQueue::update(){
	for(auto i = bcq.begin(); i < bcq.end(); i++)
		(*i).update();
}

