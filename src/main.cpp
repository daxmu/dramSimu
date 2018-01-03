#include<iostream>
#include<getopt.h>
#include<string>

#include "define.h"
#include "Debug.h"
#include "TraceInput.h"
#include "MemorySystem.h"

using namespace std;

//global var

bool init(int, char**, TraceInput &, MemorySystem &); //init the simulator for trace file, ini, etc
void run_step(TraceInput &, MemorySystem &);

int main(int argc, char **argv){

	TraceInput traceInput;
	MemorySystem memSystem(CHANNEL_NUM, CHANNEL_ROUTEBIT);

	init(argc, argv, traceInput, memSystem);

	for(int i=0; i<100; i++){	
		cout << "----------run " << i << " clock--------------" << endl;
		run_step(traceInput, memSystem);
		cout << "-----------------------------------" << endl << endl;
	}

	cout << "The simulator passed " << endl;
	return 0;
}



bool init(int argc, char **argv, TraceInput &traceInput, MemorySystem &ms){
	
	cout << "DramSimu init begin..." << endl;
	cout << "Load the param..." << endl << endl;

	int opt;
	char optstring[100] = "t:";
	bool paramCorrect = true;
	string traceFileName;

	while((opt = getopt(argc, argv, optstring)) != -1){
		switch(opt){
		case 't':
			traceFileName = optarg;
			break;
		default:
			paramCorrect = false;
			cout << "ERROR - Not valid input, please check the param " << endl;
			exit(0);
			break;
		}
	}
	
	traceInput.init(traceFileName, ms.get_inPort());
	ms.init(traceInput.get_outPort()); 
	return paramCorrect;
}

void run_step(TraceInput &traceInput, MemorySystem &memSystem){
	traceInput.run_step();
	memSystem.run_step();

	traceInput.update();
	memSystem.update();
}
