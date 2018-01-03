#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"

using namespace std;

class Controller{
private:
	int ControllerId;
	SlavePort inPort;
public:
	Controller(int id = 0):ControllerId(id), inPort("Controller_inPort", 1){}

	void print_inPortReq();
	void init(MasterPort*);

	SlavePort* get_inPort(){
		return &inPort;
	}
	void run_step(){
		print_inPortReq();
	}
	void update(){}
};


#endif
