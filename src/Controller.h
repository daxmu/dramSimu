#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>
#include <vector>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"
#include "BankCmdQueue.h"

class Controller{
private:
	int ControllerId;
	SlavePort inPort;
	std::size_t csNum;
	std::size_t bankNum;
	std::vector<CsCmdQueue> wrCcq;
	std::vector<CsCmdQueue> rdCcq;
	MultiCsStatusMachine mcsm;
	Req tempReq;
	bool tempValid;
	bool wrPrior;
	std::size_t wrCnt;
	std::size_t rdCnt;

	void send_wrpre(const Req&);
	void send_wract(const Req&);
	void send_rdpre(const Req&);
	void send_rdact(const Req&);
	void send_rd(const Req&);
	void send_wr(const Req&);
	bool check_status(const CmdStatus &status);
	bool refresh_control();
	bool arbitrate_and_send();
	void reorder();
	std::size_t get_wrreqNum();
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
