#include "TraceInput.h"

void TraceInput::init(const string tfn_, SlavePort *slavePort){
	cout << "TraceInput init begin... " << endl;

	traceFileName = tfn_;
	traceFile.open(traceFileName);
	if(!traceFile.is_open()){
		cout << "ERROR - Could not open the trace file, please check" << endl;
		exit(0);
	}
	fileValid = true;
	lineNum = 0;
	cout << "The file " << traceFileName << " is open" << endl;
	
	outPort.add_slavePort(slavePort);
	outPort.init_check();
	cout << "TraceInput init ok!" << endl << endl;
}

MasterPort* TraceInput::get_outPort(){
	return &outPort;
}

void TraceInput::debug_print() const{
	DEBUG(traceFileName);
}

void TraceInput::print() const{
	cout << traceFileName << endl;
}

string TraceInput::getTraceLine() {
	string line;
	getline(traceFile, line);
	return line;
}

Req TraceInput::get_req(){

	if(!fileValid || traceFile.eof()){
		fileValid = false;
		return Req{0};
	}

	string line;
	getline(traceFile, line);
	if(line.size() == 0){
		DEBUG("Warning - Read a empty line in trace file");
		return Req{0};
	}

	string addrStr, cmdStr, tmpStr;
	Addr addr;
	Req req{0};
	
	splitStr(line, addrStr, tmpStr);
	splitStr(tmpStr, cmdStr, tmpStr);

	istringstream tmp(addrStr.substr(2));
	tmp >> hex >> addr;
	req.addr = addr;
	req.valid = 1;
	req.length = REQ_LENGTH;

	if((cmdStr == "P_MEM_WR") ||
	   (cmdStr == "BOFF")){
	   		req.write = 1;
	} else if
	  ((cmdStr == "P_FETCH")	||
	   (cmdStr == "P_MEM_RD") 	||
       (cmdStr == "P_LOCK_RD")	||
       (cmdStr == "P_LOCK_WR")){
		req.write = 0;
	} else{
		DEBUG("Warning - Not valid request type ");
		req.valid = 0;
	}

	//DEBUG("The addStr is " + addrStr);
	//DEBUG("The cmdStr is " + cmdStr);
	//DEBUG("The lineStr is " + line);
	
	return req;
}

void TraceInput::run_step(){
	if(outPort.ready()){
		Req req = get_req();
		//req.print();
		if(req.valid)
			outPort.receive_req(req);
	}
}

void TraceInput::update(){
	outPort.send_req();
}

