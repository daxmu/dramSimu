#ifndef _PORT_H_
#define _PORT_H_

#include <iostream>
#include <deque>
#include <vector>
#include <string>

#include "define.h"
#include "Req.h"
#include "baseStruct.h"

using namespace std;

class Port{
protected:
	string portName;
	FIFO<Req> reqBuffer;
	int BufferLength;
	Port *linkPort;
public:
	Port(string name = "Unset", int l=1): portName(name), reqBuffer(l), BufferLength(l), linkPort(NULL){}
	const string& get_name();
	void add_linkPort(Port*);
	bool init_check();
	bool valid();
	bool ready();
	bool receive_req(Req);
};

class MasterPort: public Port{
public:
	MasterPort(string name = "Unset", int l=1):Port(name,l){}
	bool send_req();
	void add_slavePort(Port*);
};

class SlavePort: public Port{
public:
	SlavePort(string name = "Unset", int l=1):Port(name,l){}
	Req get_req();
	void add_masterPort(Port*);
};

inline void Port::add_linkPort(Port* port_){
	if(linkPort != NULL){
		cout << "ERROR - add more than one Port to " << portName << endl;
		exit(0);
	}
	linkPort = port_;
}

inline bool Port::init_check(){
	if(linkPort == NULL){
		cout << "Warning - " << portName << " linkPort is NULL in init_check" << endl;
		return false;
	}
	return true;
}

inline bool Port::valid(){
	return !reqBuffer.is_empty();
}

inline bool Port::ready(){
	return !reqBuffer.is_full();
}

inline bool Port::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - " << portName << " is not ready() and asked receive_req" << endl;
		return false;
	}
	reqBuffer.write(req);
	return true;
}

inline bool MasterPort::send_req(){
	if(valid()){
		if(linkPort->ready()){
			Req req = reqBuffer.read();
			linkPort->receive_req(req);
			return true;
		}
		return false;
	}
	return false;
}

inline void MasterPort::add_slavePort(Port* port_){
	add_linkPort(port_);
}

inline Req SlavePort::get_req(){
	if(valid())
		return reqBuffer.read();
	else{
		cout << "ERROR - " << portName << " is not valid and asked get_req()" << endl;
		exit(0);
	}
}
inline void SlavePort::add_masterPort(Port* port_){
	add_linkPort(port_);
}

#endif
