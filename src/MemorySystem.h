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
#include "MultiPort.h"
#include "Controller.h"

using namespace std;

class MemorySystem{
private:
	int channelNum;
	MultiPort routePort;
	vector<Controller> channels;
public:
	MemorySystem(int channelNum_ = 1, int routeBit_ = 0):
	channelNum(channelNum_), routePort("MemorySystem_routePort", 1, routeBit_, channelNum){
		for(int i = 0; i < channelNum; i++){
			Controller tmp(i);
			channels.push_back(tmp);
		}
	}
	SlavePort* get_inPort(){
		return routePort.get_inPort();
	}

	void init(MasterPort*);
	void run_step();
	void update();

};
#endif


