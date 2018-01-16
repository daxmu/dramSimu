#include "LSMC.h"

using namespace std;

Scheduler::Scheduler(std::size_t qLen_)
	:qLen(qLen_){}

bool Scheduler::valid(){
	return !cmdq.empty();
}

bool Scheduler::ready(){
	return cmdq.size() < qLen;
}

bool Scheduler::hit(const Req& a, const Req& b){
	return((a.cs == b.cs) && (a.bank == b.bank) && (a.row == b.row));
}
bool Scheduler::conflict(const Req& a, const Req& b){
	return((a.cs == b.cs) && (a.bank == b.bank) && (a.row != b.row));
}

const Req& Scheduler::get_head(){
	if(!valid()){
		cout << "ERROR - Scheduler is not valid but asked to get_head" << endl;
		exit(0);
	}
	return cmdq.front();
}
void Scheduler::pop_front(){
	if(!valid()){
		cout << "ERROR - Scheduler is not valid but asked to get_head" << endl;
		exit(0);
	}
	issuedq.push_back(cmdq.front());
	if(issuedq.size() >= 16)
		issuedq.pop_front();
	cmdq.pop_front();
}
void Scheduler::print_status(){
	cout << "Scheduler print status---" << endl;
	for(auto i = cmdq.rbegin(); i < cmdq.rend(); i++){
		cout << (*i).cs << " " << (*i).bank << " " << (*i).row << endl;
	}
}

void inline Scheduler::insert_conf(deque<Req>::iterator begin, deque<Req>::iterator end, const Req& req){
	if(end - begin > 1){
		for(auto i = begin; i < end - 1; i++){
			if(conflict(*i, *(i+1))){
				cmdq.insert(i+1, req);
				cout << "Scheduler result---" << endl;
				print_status();
				return;
			}
		}
	}
	else{
		cmdq.insert(begin + 1, req);
		return;
	}
	
	if(end - begin > 2){
		for(auto i = begin; i < end - 2; i++){
			if(conflict(*i, *(i+2))){
				cmdq.insert(i+1, req);
				return;
			}
		}
	}
//	else{
//		cmdq.insert(begin + 1, req);
//		return;
//	}
//
//	if(end - begin > 3){
//		for(auto i = begin; i < end - 3; i++){
//			if(conflict(*i, *(i+3))){
//				cmdq.insert(i+1, req);
//				return;
//			}
//		}
//	}
//	else{
//		cmdq.insert(begin + 1, req);
//		return;
//	}

	cmdq.insert(begin + 1, req);
}

#ifdef INSERT_STRATEGY_1
void Scheduler::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - Scheduler is not ready and asked to receive req in receive_req" << endl;
		exit(0);
	}

	print_status();
	cout << "receive req---" << endl;
	cout << req.cs << " " << req.bank << " " << req.row << endl;

	if(cmdq.empty()){
		cmdq.push_back(req);
		return;
	}

// find last hit and insert to it
//	for(auto i = cmdq.end() - 1; i >= cmdq.begin(); i--){
//		if(hit(*i, req)){
//			cmdq.insert(i+1, req);
//			return;
//		}
//	}
//	cmdq.push_back(req);

	bool find = false;
	auto last_hit = cmdq.end() - 1;
	for(;last_hit >= cmdq.begin(); last_hit--){
		if(hit(*last_hit, req)){
			find = true;
			break;
		}
	}
	if(!find){
		for(auto i = issuedq.rbegin(); i < issuedq.rend(); i++){
			if(conflict(*i, req)){
				cmdq.push_back(req);
				return;
			}
		}
	}
	if(!find)
		last_hit = cmdq.begin();

	find = false;
	auto first_conf = last_hit;
	for(; first_conf < cmdq.end(); first_conf++){
		if(conflict(*first_conf, req)){
			find = true;
			break;
		}
	}

	if(!find)
		insert_conf(last_hit, cmdq.end(), req);
	else{
		if(first_conf - last_hit <= 3){
			cmdq.insert(last_hit + 1, req);
			return;
		}
		else
			insert_conf(last_hit, first_conf - 3, req);
	}
	

//
//	print_status();
}
#endif
#ifdef INSERT_STRATEGY_2
void Scheduler::receive_req(Req req){
	cmdq.push_back(req);
}
#endif


void Scheduler::run_step(){}
void Scheduler::update(){}
