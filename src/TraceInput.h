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
	TraceInput():outPort("TraceInput_outPort", 1){}

	void init(const string, SlavePort*);
	MasterPort* get_outPort();
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


#endif
