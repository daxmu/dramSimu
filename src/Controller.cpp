#include "Controller.h"

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
	
	
void Controller::run_step(){
	if(inPort.valid()){
		Req req = inPort.get_req();
		print_inPortReq(req);
		map_addr(req);
		req.print();
	}
}
