#include "StatusMachine.h"

using namespace std;

BankStatusMachine::BankStatusMachine(int Id_)
	:bankId(Id_), 
	 curRow(0), 
	 curStatus(idle){}

CmdStatus BankStatusMachine::get_cmdStatus(const Req &req){
	
	CmdStatus s;

	switch(curStatus){
		case idle:
			s.needAct = true;
			if(actCnt == 0){
				s.gntAct = true;
				return s;
			}
			else{
				s.gntAct = false;
				return s;
			}
			break;
		case act:
			if(req.row == curRow){
				if(req.write){
					s.needWr = true;
					s.gntWr = (wrCnt == 0);
					return s;
				}
				else{
					s.needRd = true;
					s.gntRd = (rdCnt == 0);
					return s;
				}
			}
			else{
				s.needPre = true;
				s.gntPre = (preCnt == 0);
				return s;
			}
			break;
		default:
			cout << "ERROR - error status type "<< curStatus << " in get_cmdStatus" << endl;
			exit(0);
	}
	cout << "ERROR - no return CmdStatus before switch end in get_cmdStatus" << endl;
	exit(0);
	
	return s;
}

bool BankStatusMachine::send_pre(const Req& req){

	bool correct = true;

	if(preCnt != 0){
		cout << "Error - send pre request but preCnt!=0 in send_pre" << endl;
		correct = false;
	}
	preCnt	= max(preCnt, t_p2p_sameba);
	actCnt	= max(actCnt, t_p2a_sameba);
	rdCnt	= rdCnt;
	wrCnt	= wrCnt;

	switch(curStatus){
		case idle:
			cout << "Warning - send useless pre to idle bank in send_pre" << endl;
			curStatus = idle;
			return correct;
		case act:
			curStatus = idle;
			return correct;
		default:
			cout << "ERROR - error status type "<< curStatus << " in send_pre" << endl;
			exit(0);
	}

	cout << "ERROR - no return correct signal before switch end in send_pre" << endl;
	exit(0);
	return correct;
}

bool BankStatusMachine::send_act(const Req& req){

	bool correct = true;

	if(actCnt != 0){
		cout << "Error - send act request but actCnt!=0 in send_act" << endl;
		correct = false;
	}
	preCnt	= max(preCnt, t_a2p_sameba);
	actCnt	= max(actCnt, t_a2a_sameba);
	rdCnt	= max(rdCnt, t_a2r_sameba);
	wrCnt	= max(wrCnt, t_a2w_sameba);

	switch(curStatus){
		case idle:
			curStatus = act;
			curRow = req.row;
			return correct;
		case act:
			cout << "Warning - send useless act to act bank in send_act" << endl;
			if(curRow != req.row){
				cout << "Error - send act to act bank and row is different int send_act" << endl;
				correct = false;
			}
			curRow = req.row;
			curStatus = act;
			return correct;
		default:
			cout << "ERROR - error status type "<< curStatus << " in send_act" << endl;
			exit(0);
	}

	cout << "ERROR - no return correct signal before switch end in send_act" << endl;
	exit(0);
	return correct;
}

bool BankStatusMachine::send_rd(const Req& req){

	bool correct = true;

	if(rdCnt != 0){
		cout << "Error - send rd request but rdCnt!=0 in send_rd" << endl;
		correct = false;
	}
	if(curRow != req.row){
		cout << "Error - send rd request but curRow!=req.row in send_rd" << endl;
		correct = false;
	}

	preCnt	= max(preCnt, t_r2p_sameba;
	actCnt	= actCnt;
	rdCnt	= max(rdCnt, t_r2r_sameba);
	wrCnt	= max(wrCnt, t_r2w_sameba);
	curRow	= req.row;

	switch(curStatus){
		case idle:
			cout << "Error - send rd request in idle status in send_rd" << endl;
			correct = false
			curStatus = act;
			return correct;
		case act:
			curStatus = act;
			return correct;
		default:
			cout << "ERROR - error status type "<< curStatus << " in send_rd" << endl;
			exit(0);
	}

	cout << "ERROR - no return correct signal before switch end in send_rd" << endl;
	exit(0);
	return correct;
}

bool BankStatusMachine::send_wr(const Req& req){

	bool correct = true;

	if(wrCnt != 0){
		cout << "Error - send wr request but wrCnt!=0 in send_wr" << endl;
		correct = false;
	}
	if(curRow != req.row){
		cout << "Error - send wr request but curRow!=req.row in send_wr" << endl;
		correct = false;
	}

	preCnt	= max(preCnt, t_w2p_sameba;
	actCnt	= actCnt;
	rdCnt	= max(rdCnt, t_w2r_sameba);
	wrCnt	= max(wrCnt, t_w2w_sameba);
	curRow	= req.row;

	switch(curStatus){
		case idle:
			cout << "Error - send wr request in idle status in send_wr" << endl;
			correct = false
			curStatus = act;
			return correct;
		case act:
			curStatus = act;
			return correct;
		default:
			cout << "ERROR - error status type "<< curStatus << " in send_wr" << endl;
			exit(0);
	}

	cout << "ERROR - no return correct signal before switch end in send_wr" << endl;
	exit(0);
	return correct;
}













