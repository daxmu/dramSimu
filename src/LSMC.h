#ifndef _LSMC_H_
#define _LSMC_H_
	
#include <iostream>
#include <vector>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"
#include "Scheduler.h"
#include "TimingQueue.h"


class Controller{
private:
	int ControllerId;
	SlavePort inPort;
	std::size_t csNum;
	std::size_t bankNum;
	Scheduler sch;
	TimingQueue tq;
public:
	Controller(int id, std::size_t csNum_, std::size_t qLen_, std::size_t bankNum_);
	void init(MasterPort*);
	SlavePort* get_inPort();
	void map_addr(Req&);
	void print_summary(std::size_t cycles);
	void run_step();
	void update();
};

#endif
