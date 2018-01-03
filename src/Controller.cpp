#include "Controller.h"

void Controller::print_inPortReq(){
	if(inPort.valid()){
		cout << "*********Controller" << ControllerId << " print inPort Req************" << endl;
		Req tmp = inPort.get_req();
		tmp.print();
	}
}
void Controller::init(MasterPort* masterPort){
	cout << "Controller" << ControllerId << " init begin..." << endl;
	inPort.add_masterPort(masterPort);
	if(!inPort.init_check()){
		cout << "ERROR - Controller" << ControllerId << "'s inPort init_check() return false in init()" << endl;
		exit(0);
	}
	cout << "Controller" << ControllerId << " init ok!" << endl << endl;
}

