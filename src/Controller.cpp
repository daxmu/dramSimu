#include "Controller.h"

Controller::Controller(int id, int csNum_, int qLen_, int bankNum_)
	:ControllerId(id),
	 inPort("Controller_inPort", 1),
	 cmdq(csNum_, qLen_, bankNum_){}

void Controller::print_inPortReq(const Req& req){
	cout << "*********Controller" << ControllerId << " print inPort Req************" << endl;
	req.print();
}
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
	
void Controller::print_summary(std::size_t cycles){
	cout << "Controller " << ControllerId << " print summary:" << endl;
	cmdq.print_summary(cycles);
	cout << endl;
}

void Controller::run_step(){
	cout << "Controller " << ControllerId << " run_step()..." << endl;
	if(inPort.valid()){
		if(cmdq.ready()){
			Req req = inPort.get_req();
			map_addr(req);
			cmdq.receive_req(req);
		}
	}
	
	cmdq.run_step();
}

void Controller::update(){
	cmdq.update();
}


