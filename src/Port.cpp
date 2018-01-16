#include "Port.h"
void Port::add_linkPort(Port* port_){
	if(linkPort != NULL){
		cout << "ERROR - add more than one Port to " << portName << endl;
		exit(0);
	}
	linkPort = port_;
}

bool Port::init_check(){
	if(linkPort == NULL){
		cout << "Warning - " << portName << " linkPort is NULL in init_check" << endl;
		return false;
	}
	return true;
}

bool Port::valid(){
	return !reqBuffer.is_empty();
}

bool Port::ready(){
	return !reqBuffer.is_full();
}

const Req& Port::get_head(){
	return reqBuffer.get_head();
}

bool Port::receive_req(Req req){
	if(!ready()){
		cout << "ERROR - " << portName << " is not ready() and asked receive_req" << endl;
		return false;
	}
	reqBuffer.write(req);
	return true;
}

bool MasterPort::send_req(){
	if(valid()){
		if(linkPort->ready()){
			DEBUG(portName + " send_req success ");
			Req req = reqBuffer.read();
			//req.print();
			linkPort->receive_req(req);
			return true;
		}
		return false;
	}
	return false;
}

void MasterPort::add_slavePort(Port* port_){
	add_linkPort(port_);
}

Req SlavePort::get_req(){
	if(valid())
		return reqBuffer.read();
	else{
		cout << "ERROR - " << portName << " is not valid and asked get_req()" << endl;
		exit(0);
	}
}
void SlavePort::add_masterPort(Port* port_){
	add_linkPort(port_);
}


