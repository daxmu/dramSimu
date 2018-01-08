#include<iostream>
#include<getopt.h>
#include<string>
#include<time.h>

#include "define.h"
#include "Debug.h"
#include "TraceInput.h"
#include "MemorySystem.h"

using namespace std;

//global var

bool init(int, char**, TraceInput &, MemorySystem &, int &); //init the simulator for trace file, ini, etc
void run_step(TraceInput &, MemorySystem &);

int main(int argc, char **argv){

	int Cycles = 100;
	TraceInput traceInput;
	MemorySystem memSystem(CHANNEL_NUM, CHANNEL_ROUTEBIT);

	print_config_message();

	init(argc, argv, traceInput, memSystem, Cycles);

	for(int i=0; i<Cycles; i++){	
		cout << "----------run " << i << " clock--------------" << endl;
		run_step(traceInput, memSystem);
		cout << "-----------------------------------" << endl << endl;
	}

	cout << "The simulator passed " << endl;
	return 0;
}



bool init(int argc, char **argv, TraceInput &traceInput, MemorySystem &ms, int &cyc){
	
	cout << "DramSimu init begin..." << endl;
	cout << "Load the param..." << endl << endl;

	int opt;
	char optstring[100] = "t:c:";
	bool paramCorrect = true;
	string traceFileName;

	while((opt = getopt(argc, argv, optstring)) != -1){
		switch(opt){
		case 't':
			traceFileName = optarg;
			break;
		case 'c':
			cyc = atoi(optarg);
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
	cout << "run_step()&&&" << endl;
	traceInput.run_step();
	memSystem.run_step();

	cout << endl << "update()&&&" << endl;
	traceInput.update();
	memSystem.update();
}
