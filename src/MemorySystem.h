#ifndef _MEMORYSYSTEM_H_
#define _MEMORYSYSTEM_H_

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"

using namespace std;

class MemorySystem{
private:
	SlavePort inPort;
public:
	MemorySystem():inPort("MemorySystem_inPort", 1){}
	void print_inPortReq(){
		if(inPort.valid()){
			Req tmp = inPort.get_req();
			tmp.print();
		}
	}
	void init(MasterPort* masterPort){
		cout << "MemorySystem init begin..." << endl;
		inPort.add_masterPort(masterPort);
		inPort.init_check();
		cout << "MemorySystem init ok!" << endl << endl;
	}
	SlavePort* get_inPort(){
		return &inPort;
	}
	void run_step(){
		print_inPortReq();
	}
};

#endif
