#ifndef _MULTIPORT_H_
#define _MULTIPORT_H_

#include "Port.h"
#include <sstream>
#include <vector>

class MultiPort{
private:
	string multiPortName;
	SlavePort inPort;
	int routeBit;
	int routeBitNum;
	int outPortNum;
	vector<MasterPort> outPort; 

	Req temp;
	bool tempValid;
public:
	MultiPort(string name = "Unset", int l = 1, int routeBit_ = 0, int outPortNum_ = 2):
	multiPortName(name), inPort(multiPortName + "_Multi_inPort", l), routeBit(routeBit_), outPortNum(outPortNum_), tempValid(false){
		for(int i = 0; i < outPortNum; i++){
			stringstream stream;
			string slaveId;
			stream << i;
			stream >> slaveId;

			MasterPort tmp(name + "_Multi_outPort_" + slaveId, 1);
			outPort.push_back(tmp);
		}
		int num = 1, tmp = outPortNum;
		while(tmp != 1){
			tmp = tmp >> 1;
			num = num << 1;
		}
		routeBitNum = num;
	}
	void add_masterPort(Port*);
	void add_slavePort(vector<Port*>);
	void init(Port*, vector<Port*>);
	bool init_check();
	
	SlavePort* get_inPort();
	vector<MasterPort*> get_outPort();

	void run_step();
	void update();
};




#endif
