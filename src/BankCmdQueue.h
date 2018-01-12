#ifndef _BANKCMDQUEUE_H_
#define _BANKCMDQUEUE_H_

#include <iostream>
#include <list>
#include <vector>

#include "Req.h"
#include "StatusMachine.h"

class BankCmdQueue{
private:
	std::size_t bcqId;
	std::size_t qLen;
	std::list<Req> reqList;

	void add_req(Req req);
public:
	BankCmdQueue(int bcqId_, int qLen_);
	bool valid();
	bool ready();
	bool receive_req(Req req);
	const Req& get_head();
	void pop_front();
	void reorder(std::size_t row);
	std::size_t get_reqNum();

	void run_step();
	void update();
};

class CsCmdQueue{
public:
	std::size_t ccqId;
	std::size_t bankNum;
	std::size_t qLen;

	std::size_t rdCounter;
	std::size_t wrCounter;
	std::size_t preCounter;
	std::size_t actCounter;
	
	Req tempReq;
	bool tempValid;

	//CsStatusMachine csm;
	std::vector<BankCmdQueue> bcq;

	CsCmdQueue(int ccqId_, int bankNum_, int qLen_);
	bool valid();
	bool ready();
	bool receive_req(Req req);
	bool accept_req();

	void send_pre(const Req&);
	void send_act(const Req&);
	void send_rd(const Req&);
	void send_wr(const Req&);

	std::size_t get_reqNum();

	void run_step();
	void update();
};


#endif
