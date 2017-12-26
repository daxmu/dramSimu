#include<iostream>
//#include<stdio.h>
//#include<stdlib.h>
#include<getopt.h>
#include<string>

#include "define.h"
#include "Debug.h"
#include "TraceInput.h"

using namespace std;

//global var

bool init(int, char**, TraceInput &); //init the simulator for trace file, ini, etc
void run_step(TraceInput &);

int main(int argc, char **argv){

	TraceInput traceInput;
	init(argc, argv, traceInput);
	run_step(traceInput);

	cout << "The simulator passed " << endl;
	return 0;
}



bool init(int argc, char **argv, TraceInput &traceInput){
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
	
	traceInput.init(traceFileName);
	return paramCorrect;
}

void run_step(TraceInput &trace){
	trace.get_req();	
	trace.get_req();	
	trace.get_req();	
}
