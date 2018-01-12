#ifndef _STATUSMACHINE_H_
#define _STATUSMACHINE_H_

#include <iostream>
#include <vector>
#include <deque>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "baseStruct.h"

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
	bool send_ref();
	std::size_t get_actRow();
	void run_step();
	void update();

};

class CsStatusMachine{
private:
	enum Status {idle, preref, ref};
	std::size_t csId;
	std::size_t bankNum;
	std::size_t fawCnt;
	std::size_t rrdCnt;
	std::size_t preCnt;
	std::size_t actCnt;
	std::size_t rdCnt;
	std::size_t wrCnt;
	std::size_t refCnt;
	std::size_t rfcCnt;
	std::deque<std::size_t> actQueue;
	std::vector<BankStatusMachine> bsm;
	Status curStatus;
	
	void act_push(std::size_t time = 0);
	void act_add();

public:
	CsStatusMachine(int csId_, int bankNum_);
	CmdStatus get_cmdStatus(const Req&);
	bool send_pre(const Req&);
	bool send_act(const Req&);
	bool send_rd(const Req&);
	bool send_wr(const Req&);
	bool need_refresh();
	bool send_ref();
	std::size_t get_actRow(int bankId);
	void run_step();
	void update();
};

class MultiCsStatusMachine{
private:
	std::size_t csNum;
	std::size_t bankNum;
	std::vector<CsStatusMachine> csm;

	std::size_t rdCnt;
	std::size_t wrCnt;

public:
	MultiCsStatusMachine(int csNum_, int bankNum_);
	CmdStatus get_cmdStatus(const Req&);
	bool send_pre(const Req&);
	bool send_act(const Req&);
	bool send_rd(const Req&);
	bool send_wr(const Req&);
	bool need_refresh(std::size_t csId);
	bool send_ref(std::size_t csId);
	std::size_t get_actRow(int csId, int bankId);
	void run_step();
	void update();
};

#endif
