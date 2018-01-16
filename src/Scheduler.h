#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include<deque>

#include "Req.h"
#include "define.h"

class Scheduler{
private:
	std::size_t qLen;
	std::deque<Req> cmdq;
	std::deque<Req> issuedq;

	bool hit(const Req& a, const Req& b);
	bool conflict(const Req& a, const Req& b);
	void insert_conf(deque<Req>::iterator begin, deque<Req>::iterator end, const Req& req);
public:
	Scheduler(std::size_t qLen_);
	bool valid();
	bool ready();
	const Req& get_head();
	void pop_front();
	//void insert_conf(deque<Req>::iterator begin, deque<Req>::iterator end);
	void receive_req(Req);
	void print_status();
	void print_summary(std::size_t cycles);
	void run_step();
	void update();
};

#endif
