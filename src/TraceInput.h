#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include "define.h"
#include "Debug.h"
#include "Req.h"
using namespace std;

class TraceInput{
private:
	string traceFileName;
	ifstream traceFile;
	bool fileValid;
	int lineNum;
public:
	//TraceInput(string tfn_):traceFileName(tfn){
	//	traceFile.open(traceFileName);
	//	if(!traceFile.is_open())
	//		cout << "ERROR - Could not open the trace file, please check" << endl;
	//}
	TraceInput() = default;

	void init(const string);
	void debug_print() const;
	void print() const;
	string getTraceLine();
	Req get_req();
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

void TraceInput::init(const string tfn_){
	traceFileName = tfn_;
	traceFile.open(traceFileName);
	if(!traceFile.is_open()){
		cout << "ERROR - Could not open the trace file, please check" << endl;
		exit(0);
	}
	fileValid = true;
	lineNum = 0;
	cout << "The file " << traceFileName << " is open" << endl;
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
