#ifndef _PORT_H_
#define _PORT_H_

#include <iostream>
#include <deque>
#include <vector>
#include <string>

#include "define.h"
#include "Debug.h"
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

#endif
