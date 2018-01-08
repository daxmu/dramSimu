#include "StatusMachine.h"

using namespace std;

BankStatusMachine::BankStatusMachine(int Id_)
	:bankId(Id_), 
	 curRow(0), 
	 curStatus(idle),
	 preCnt(0),
	 actCnt(0),
	 rdCnt(0),
	 wrCnt(0)
	 {}

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
	preCnt	= max_(preCnt, t_p2p_sameba);
	actCnt	= max_(actCnt, t_p2a_sameba);
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
	preCnt	= max_(preCnt, t_a2p_sameba);
	actCnt	= max_(actCnt, t_a2a_sameba);
	rdCnt	= max_(rdCnt, t_a2r_sameba);
	wrCnt	= max_(wrCnt, t_a2w_sameba);

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

	preCnt	= max_(preCnt, t_r2p_sameba);
	actCnt	= actCnt;
	rdCnt	= max_(rdCnt, t_r2r_sameba);
	wrCnt	= max_(wrCnt, t_r2w_sameba);
	curRow	= req.row;

	switch(curStatus){
		case idle:
			cout << "Error - send rd request in idle status in send_rd" << endl;
			correct = false;
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

	preCnt	= max_(preCnt, t_w2p_sameba);
	actCnt	= actCnt;
	rdCnt	= max_(rdCnt, t_w2r_sameba);
	wrCnt	= max_(wrCnt, t_w2w_sameba);
	curRow	= req.row;

	switch(curStatus){
		case idle:
			cout << "Error - send wr request in idle status in send_wr" << endl;
			correct = false;
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

bool BankStatusMachine::send_ref(){
	curStatus = idle;
	return true;
}

void BankStatusMachine::run_step(){}

void BankStatusMachine::update(){
	if(preCnt!=0)
		preCnt--;
	if(actCnt!=0)
		actCnt--;
	if(rdCnt!=0)
		rdCnt--;
	if(wrCnt!=0)
		wrCnt--;
}


CsStatusMachine::CsStatusMachine(int csId_, int bankNum_)
	:csId(csId_),
	 bankNum(bankNum_),
	 fawCnt(0),
	 rrdCnt(0),
	 refCnt(tREF),
	 rfcCnt(0),
	 curStatus(idle)
	 {
	 	for(unsigned int i = 0; i < bankNum; i++){
			BankStatusMachine tmp(i);
			bsm.push_back(tmp);	
		}
	 }

void CsStatusMachine::act_push(size_t time){
	if(actQueue.size() == 3){
		actQueue.pop_front();
		actQueue.push_back(time);
	}
	else{
		actQueue.push_back(time);
	}
	if(actQueue.size() > 3){
		cout << "ERROR - actQueue's length > 3 in act_push" << endl;
		exit(0);
	}
	if(actQueue.size() == 3){
		if(tFAW < actQueue.front()){
			cout << "ERROR - tFAW < actQueue.front() in act_push" << endl;
			exit(0);
		}
		fawCnt = tFAW - actQueue.front();
	}
	else
		fawCnt = 0;
}
void CsStatusMachine::act_add(){
	for(auto i = actQueue.begin(); i < actQueue.end(); i++)
		(*i)++;
	if(!actQueue.empty())
		if(actQueue.front() >= tFAW)
			actQueue.pop_front();
}

CmdStatus CsStatusMachine::get_cmdStatus(const Req& req){
	
	CmdStatus tmp;
	
	if(req.bank >= bankNum){
		cout << "ERROR - req.bank >= bankNum in CsSM.get_cmdStatus" << endl;
		exit(0);
	}
	
	tmp = bsm[req.bank].get_cmdStatus(req);
	if((fawCnt != 0) || (rrdCnt != 0)){
		tmp.gntAct = 0;
	}

	if(curStatus == ref){
		tmp.gntPre = 0;
		tmp.gntAct = 0;
		tmp.gntWr = 0;
		tmp.gntRd = 0;
	}

	return tmp;
}

bool CsStatusMachine::send_pre(const Req& req){

	bool correct;
	
	if(req.bank >= bankNum){
		cout << "ERROR - req.bank >= bankNum in CsSM.send_pre" << endl;
		exit(0);
	}
	
	correct = bsm[req.bank].send_pre(req);

	return correct;
}

bool CsStatusMachine::send_act(const Req& req){

	bool correct;
	
	if(req.bank >= bankNum){
		cout << "ERROR - req.bank >= bankNum in CsSM.send_act" << endl;
		exit(0);
	}
	
	correct = bsm[req.bank].send_act(req);

	if(rrdCnt != 0){
		cout << "Error - send act req but rrdCnt!=0 in CsSM.send_act" << endl;
		correct = false;
	}
	if(fawCnt != 0){
		cout << "Error - send act req but fawCnt!=0 in CsSM.send_act" << endl;
		correct = false;
	}

	act_push();
	rrdCnt = tRRD;
	return correct;
}

bool CsStatusMachine::send_rd(const Req& req){

	bool correct;
	
	if(req.bank >= bankNum){
		cout << "ERROR - req.bank >= bankNum in CsSM.send_rd" << endl;
		exit(0);
	}
	
	correct = bsm[req.bank].send_rd(req);

	return correct;
}

bool CsStatusMachine::send_wr(const Req& req){

	bool correct;
	
	if(req.bank >= bankNum){
		cout << "ERROR - req.bank >= bankNum in CsSM.send_wr" << endl;
		exit(0);
	}
	
	correct = bsm[req.bank].send_wr(req);

	return correct;
}

bool CsStatusMachine::need_refresh(){
	if(curStatus == idle){
		if(refCnt == 0)
			return true;
	}
	return false;
}

bool CsStatusMachine::send_ref(){

	bool correct = true;

	if(curStatus != idle){
		cout << "Error - send ref request in not idle status in send_ref()" << endl;
		correct = false;
	}

	for(auto i = bsm.begin(); i < bsm.end(); i++){
		(*i).send_ref();
	}

	curStatus = ref;
	rfcCnt = tRFC;

	return correct;
}

void CsStatusMachine::run_step(){
}

void CsStatusMachine::update(){
	if(curStatus == ref){
		if(rfcCnt != 0)
			rfcCnt --;
		else{
			curStatus = idle;
			refCnt = tREF;
			cout << "    Refresh end and status change to idle in cs " << csId << endl;
		}
	}
	else{
		if(refCnt != 0)
			refCnt --;
		else
			cout << "Error - refCnt = 0 and not send ref request in update()" << endl;
	}

	if(fawCnt != 0)
		fawCnt--;
	if(rrdCnt != 0)
		rrdCnt--;
	act_add();
	for(auto i = bsm.begin(); i < bsm.end(); i++){
		(*i).update();
	}
}

