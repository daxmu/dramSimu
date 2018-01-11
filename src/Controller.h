#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iostream>
#include <vector>

#include "define.h"
#include "Debug.h"
#include "Req.h"
#include "Port.h"
#include "BankCmdQueue.h"

using namespace std;

class Controller{
private:
	int ControllerId;
	SlavePort inPort;
	std::size_t csNum;
	std::vector<CsCmdQueue> ccq;
	MultiCsStatusMachine mcsm;
	Req tempReq;
	bool tempValid;

	void send_pre(const Req&);
	void send_act(const Req&);
	void send_rd(const Req&);
	void send_wr(const Req&);
	bool check_status(const CmdStatus &status);
	bool refresh_control();
	bool arbitrate_and_send();
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
