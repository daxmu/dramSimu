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

//#ifdef LOONGSON_MC
#include "LSMC.h"
//#else
//#include "Controller.h"
//#endif

class MemorySystem{
private:
	int channelNum;
	MultiPort routePort;
	std::vector<Controller> channels;
public:
	MemorySystem(int channelNum_ = 1, int routeBit_ = 0):
	channelNum(channelNum_), routePort("MemorySystem_routePort", 1, routeBit_, channelNum){
		for(int i = 0; i < channelNum; i++){
			Controller tmp(i, CS_NUM, COMMAND_QUEUE_LENGTH, BANK_NUM);
			channels.push_back(tmp);
		}
	}
	SlavePort* get_inPort(){
		return routePort.get_inPort();
	}

	void init(MasterPort*);
	void print_summary(std::size_t cycles);
	void run_step();
	void update();

};
#endif


