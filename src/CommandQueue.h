#ifndef _COMMANDQUEUE_H
#define _COMMANDQUEUE_H

#include <iostream>
#include <vector>

#include "StatusMachine.h"
#include "Req.h"
#include "baseStruct.h"

class CommandQueue{
private:
	std::size_t csNum;
	std::size_t qLen;
	std::size_t rdCounter;
	std::size_t wrCounter;
	std::size_t reqCounter;
	std::size_t preCounter;
	std::size_t actCounter;
	std::vector<CsStatusMachine> csm;
	FIFO<Req> cmdq;

	void pop_req();
	void send_pre(const Req&);
	void send_act(const Req&);
	void send_rd(const Req&);
	void send_wr(const Req&);
public:
	CommandQueue(int csNum_, int qLen_, int bankNum_);
	bool valid();
	bool ready();
	bool receive_req(Req req);
	bool check_status(const CmdStatus&);
	void refresh_control();
	void print_status();
	void print_summary(std::size_t cycles);
	void print_summary();
	void run_step();
	void update();

};

#endif
