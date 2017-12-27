#ifndef _PORT_H_
#define _PORT_H_

#include <iostream>
#include <deque>
#include <vector>

#include"define.h"
#include "Req.h"
#include "baseStruct.h"

using namespace std;

class SlavePort;

class MasterPort{
private:
	FIFO<Req> reqBuffer;
	int BufferLength;
	vector<SlavePort*> slave; 
	const int routeBit;
	const uint8_t slaveNum;
public:
	MasterPort(int l, int routeBit_ = 0, int slaveNum_ = 1): reqBuffer(l), BufferLength(l), routeBit(routeBit_), slaveNum(slaveNum_){}
	bool init_check();
	void add_slavePort(SlavePort*);
	void add_slavePort(vector<SlavePort*>);
	bool send_req();
	bool ready();
	bool receive_req(Req);
};

class SlavePort{
private:
	FIFO<Req> reqBuffer;
	int BufferLength;
	vector<MasterPort*> master;
	const int routeBit;
	const uint8_t masterNum;
public:
	SlavePort(int l, int routeBit_ = 0, int masterNum_ = 1): reqBuffer(l), BufferLength(l), routeBit(routeBit_), masterNum(masterNum_){}
	bool init_check();
	void add_masterPort(MasterPort*);
	void add_masterPort(vector<MasterPort*>);
	void ret_req();
	bool ready();
	bool receive_req(Req);
	bool valid();
	Req	 get_req();
};

bool MasterPort::init_check(){
	if(slave.size() == slaveNum){
		cout << "MasterPort init ok" << endl;	
		return true;
	}
	cout << "ERROR - MasterPort has wrong number of SlavePort with slaveNum" << endl;
	cout << "slave.size(): " << slave.size() << "  slaveNum: " << slaveNum << endl;
	return false;
}

void MasterPort::add_slavePort(SlavePort* s){
	if(slave.size() >= slaveNum){
		cout << "ERROR - Add more slaves than slaveNum" << endl;
		exit(0);
		return;
	}
	slave.push_back(s);
}

void MasterPort::add_slavePort(vector<SlavePort*> s){
	for(auto i = s.begin(); i < s.end(); i++)
		add_slavePort(*i);
}

// when send a req, we get the req from the reqBuffer, and route it to it's slave, then verify the
// slave can receive it or not. if it can receive, pop the req and send it to the slave
bool MasterPort::send_req(){
	if(!reqBuffer.is_empty()){
		const Req &tmp = reqBuffer.get_head();
		Addr addr = tmp.addr;
		int num = (addr >> routeBit) & (slaveNum-1);
		if(slave[num]->ready()){
			Req req = reqBuffer.read();
			slave[num]->receive_req(req);
			return true;
		}
		return false;
	}
	return false;
}

bool MasterPort::ready(){
	return !reqBuffer.is_full();
}

bool MasterPort::receive_req(Req a){
	if(reqBuffer.is_full()){
		cout << "ERROR - master reqBuffer is full in receive_req " << endl;
		return false;
	}
	reqBuffer.write(a);
	return true;
}











bool SlavePort::init_check(){
	if(master.size() == masterNum){
		cout << "MasterPort init ok" << endl;	
		return true;
	}
	cout << "ERROR - MasterPort has wrong number of MasterPort with masterNum" << endl;
	cout << "master.size(): " << master.size() << "  masterNum: " << masterNum << endl;
	return false;
}

void SlavePort::add_masterPort(MasterPort* m){
	if(master.size() >= masterNum){
		cout << "ERROR - Add more masters than masterNum" << endl;
		exit(0);
		return;
	}
	master.push_back(m);
}
void SlavePort::add_masterPort(vector<MasterPort*> m){
	for(auto i = m.begin(); i < m.end(); i++)
		add_masterPort(*i);
}

bool SlavePort::ready(){
	return !reqBuffer.is_full();
}
bool SlavePort::valid(){
	return !reqBuffer.is_empty();
}

bool SlavePort::receive_req(Req a){
	if(reqBuffer.is_full()){
		cout << "ERROR - slave reqBuffer is full in receive_req " << endl;
		return false;
	}
	reqBuffer.write(a);
	return true;
}

void SlavePort::ret_req(){}

Req SlavePort::get_req(){
	if(!valid()){
		cout << "ERROR - slave reqBuffer is empty in get_req" << endl;
		exit(0);
	}
	return reqBuffer.read();
}

#endif
