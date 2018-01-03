#include "MultiPort.h"

void MultiPort::add_masterPort(Port* port_){
	inPort.add_linkPort(port_);
}

void MultiPort::add_slavePort(vector<Port*> port_v){
	if(port_v.size() != outPort.size()){
		cout << "ERROR - add incorrect number slavePort to " << multiPortName << " in add_slavePort" << endl;
		cout << port_v.size() << " " << outPort.size() << " " << outPortNum << endl;
		exit(0);
	}

	auto j = port_v.begin();
	for(auto i = outPort.begin(); i < outPort.end(); i++){
		(*i).add_slavePort(*j);
		j++;
	}
}

void MultiPort::init(Port* master, vector<Port*> slave_v){
	cout << "    " << multiPortName << " init" << endl;
	add_masterPort(master);
	add_slavePort(slave_v);
	if(!init_check()){
		cout << "ERROR - " << multiPortName << " init_check() failed in init()" << endl;
		exit(0);
	}
}

bool MultiPort::init_check(){
	if(!inPort.init_check())
		return false;
	for(auto i = outPort.begin(); i < outPort.end(); i++){
		if(!(*i).init_check())
			return false;
	}

	// if the outPortNum is not 2^n, and we use the addr to splict the request to different slaves,
	// we will let the extra slaves get not request forever or let some request has no slaves to send
	if(outPortNum&(outPortNum-1))
		cout << "Warning - " << multiPortName << "'s outPortNum is not 2^n, may have some invalid modules" << endl;
	
	cout << "    " << multiPortName << " init check ok" << endl;

	return true;

}

SlavePort* MultiPort::get_inPort(){
	return &inPort;
}

vector<MasterPort*> MultiPort::get_outPort(){
	vector<MasterPort*> port_v;
	for(auto i = outPort.begin(); i < outPort.end(); i++){
		port_v.push_back(&(*i));
	}
	return port_v;
}

void MultiPort::run_step(){
	if(!tempValid){
		if(inPort.valid()){
			temp = inPort.get_req();
			tempValid = true;
		}
	}

	if(tempValid){
		Addr addr = temp.addr;
		int routeId = (addr >> routeBit) & (routeBitNum-1);
		if(outPort[routeId].ready()){
			outPort[routeId].receive_req(temp);
			tempValid = false;
		}
	}
}

void MultiPort::update(){
	for(auto i = outPort.begin(); i < outPort.end(); i++)
		(*i).send_req();
}

