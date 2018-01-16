#include "LSMC.h"

using namespace std;

Controller::Controller(int id, size_t csNum_, size_t qLen_, size_t bankNum_)
	:ControllerId(id),
	 inPort("Controller_inPort", 1),
	 csNum(csNum_),
	 bankNum(bankNum_),
	 sch(qLen_),
	 tq(csNum, TIMING_QUEUE_LENGTH, bankNum){}

void Controller::init(MasterPort* masterPort){
	cout << "    Controller" << ControllerId << " init begin..." << endl;
	inPort.add_masterPort(masterPort);
	if(!inPort.init_check()){
		cout << "ERROR - Controller" << ControllerId 
			 << "'s inPort init_check() return false in init()" << endl;
		exit(0);
	}
	cout << "    Controller" << ControllerId << " init ok!" << endl;
}

SlavePort* Controller::get_inPort(){
	return &inPort;
}

void Controller::map_addr(Req& req){
	req.cs = get_corBits(req.addr, CS_BIT_NUM, CS_BIT_POS);
	req.cid = get_corBits(req.addr, CID_BIT_NUM, CID_BIT_POS);
	req.bg = get_corBits(req.addr, BG_BIT_NUM, BG_BIT_POS);
	req.bank = get_corBits(req.addr, BANK_BIT_NUM, BANK_BIT_POS);

	req.col = get_corBits(req.addr, COL_LOWBIT_NUM, COL_LOWBIT_POS) | 
			  (get_corBits(req.addr, COL_HIGHBIT_NUM, COL_HIGHBIT_POS) << COL_LOWBIT_NUM);
	req.row = get_corBits(req.addr, ROW_LOWBIT_NUM, ROW_LOWBIT_POS) | 
			  (get_corBits(req.addr, ROW_HIGHBIT_NUM, ROW_HIGHBIT_POS) << ROW_LOWBIT_NUM);
}
	
void Controller::print_summary(size_t cycles){
	cout << "Controller " << ControllerId << " print summary:" << endl;
	tq.print_summary(cycles);	
}

void Controller::run_step(){
	cout << "Controller " << ControllerId << " run_step()..." << endl;
	if(inPort.valid()){
		if(sch.ready()){
			Req tmp = inPort.get_req();
			map_addr(tmp);
			sch.receive_req(tmp);
		}
	}

	sch.run_step();

	if(sch.valid()){
		if(tq.ready()){
			Req tmp = sch.get_head();
			tq.receive_req(tmp);
			sch.pop_front();
		}
	}

	tq.run_step();
}

void Controller::update(){
	sch.update();
	tq.update();
}
