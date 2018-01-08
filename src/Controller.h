#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"
#include "CommandQueue.h"

using namespace std;

class Controller{
private:
	int ControllerId;
	SlavePort inPort;
	CommandQueue cmdq;
public:
	Controller(int id, int csNum_, int qLen_, int bankNum_);

	void print_inPortReq(const Req&);
	void init(MasterPort*);
	SlavePort* get_inPort(){
		return &inPort;
	}
	void map_addr(Req&);

	void print_summary(std::size_t cycles);
	void run_step();
	void update();
};


#endif
