#ifndef _STATUSMACHINE_H_
#define _STATUSMACHINE_H_

#include <iostream>

#include "define.h"
#include "Debug.h"
#include "Req.h"

struct CmdStatus{
	bool needPre;
	bool needAct;
	bool needRd;
	bool needWr;
	bool gntPre;
	bool gntAct;
	bool gntRd;
	bool gntWr;
	CmdStatus():needPre(0),needAct(0),needRd(0),needWr(0),gntPre(0),gntAct(0),gntRd(0),gntWr(0){}
};

class BankStatusMachine{
private:
	enum Status {idle, act};
	std::size_t bankId;
	std::size_t curRow;
	Status curStatus;
	std::size_t preCnt;
	std::size_t actCnt;
	std::size_t rdCnt;
	std::size_t wrCnt;
	
public:
	BankStatusMachine(int);
	CmdStatus get_cmdStatus(const Req&);
	bool send_pre(const Req&);
	bool send_act(const Req&);
	bool send_rd(const Req&);
	bool send_wr(const Req&);
	void run_step();
	void update();

}

#endif
