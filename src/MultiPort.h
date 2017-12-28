#ifndef _MULTIPORT_H_
#define _MULTIPORT_H_

#include "Port.h"
#include <sstream>

class MultiPort{
private:
	Req temp;
	bool tempValid;
	SlavePort inPort;
	vector<MasterPort> outPort; 
	int outPortNum;
	int routeBit;
public:
	MultiPort(string name = "Unset", int l = 1, int routeBit_ = 0, int outPortNum_ = 2):
	inPort(name + "_Multi_inPort", l){
		for(int i = 0, i < outPortNum; i++){
			stringstream stream;
			string slaveId;
			stream << i;
			stream >> slaveId;

			MasterPort tmp(name + "_Multi_outPort_" + slaveId, 1);
			outPort.push_back(tmp);
		}
	}
	
}

#endif
