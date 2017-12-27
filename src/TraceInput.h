#ifndef _TRACEINPUT_H_
#define _TRACEINPUT_H_

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "baseStruct.h"
#include "Port.h"
using namespace std;

class TraceInput{
private:
	string traceFileName;
	ifstream traceFile;
	bool fileValid;
	int lineNum;
	MasterPort outPort;
public:
	//TraceInput(string tfn_):traceFileName(tfn){
	//	traceFile.open(traceFileName);
	//	if(!traceFile.is_open())
	//		cout << "ERROR - Could not open the trace file, please check" << endl;
	//}
	//TraceInput() = default;
	TraceInput():outPort(2, 0, 1){}

	void init(const string, vector<SlavePort*>);
	vector<MasterPort*> get_outPort();
	void debug_print() const;
	void print() const;
	string getTraceLine();
	Req get_req();
	void run_step();
	void update();
};


inline void splitStr(const string &origin, string &first, string &second){
	uint32_t spaceIndex = origin.find_first_of(" ", 0);
	first = origin.substr(0, spaceIndex);
	uint32_t nextIndex = origin.find_first_not_of(" ", spaceIndex);
	if(nextIndex >= origin.length())
		second = "";
	else
		second = origin.substr(nextIndex);
	return;
}

void TraceInput::init(const string tfn_, vector<SlavePort*> slavePort){
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

vector<MasterPort*> TraceInput::get_outPort(){
	return vector<MasterPort*>(1, &outPort);
}

inline void TraceInput::debug_print() const{
	DEBUG(traceFileName);
}

inline void TraceInput::print() const{
	cout << traceFileName << endl;
}

inline string TraceInput::getTraceLine() {
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
	Req req;
	
	splitStr(line, addrStr, tmpStr);
	splitStr(tmpStr, cmdStr, tmpStr);

	istringstream tmp(addrStr.substr(2));
	tmp >> hex >> addr;
	req.addr = addr;
	req.valid = 1;

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
		outPort.receive_req(req);
	}
}

void TraceInput::update(){
	outPort.send_req();
}


#endif
