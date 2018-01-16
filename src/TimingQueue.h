#ifndef _TIMINGQUEUE_H_
#define _TIMINGQUEUE_H_

#include<deque>

#include "Req.h"
#include "StatusMachine.h"

class TimingQueue{
private:
	std::size_t csNum;
	std::size_t qLen;
	std::size_t rdCounter;
	std::size_t wrCounter;
	std::size_t reqCounter;
	std::size_t preCounter;
	std::size_t actCounter;
	MultiCsStatusMachine mcsm;
	std::deque<Req> cmdq;

	void pop_req();
	void send_pre(const Req&);
	void send_act(const Req&);
	void send_rd(const Req&);
	void send_wr(const Req&);
	bool is_sameba(const Req&, const Req&);
public:
	TimingQueue(int csNum_, int qLen_, int bankNum_);
	bool valid();
	bool ready();
	bool receive_req(Req req);
	bool check_status(const CmdStatus&);
	bool refresh_control();
	void print_status();
	void print_summary(std::size_t cycles);
	void print_summary();
	void run_step();
	void update();

};


#endif
