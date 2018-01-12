#include "Controller.h"

Controller::Controller(int id, int csNum_, int qLen_, int bankNum_)
	:ControllerId(id),
	 inPort("Controller_inPort", 1),
	 csNum(csNum_),
	 bankNum(bankNum_),
	 mcsm(csNum, bankNum_),
	 tempValid(false),
	 wrPrior(false){
	 	for(int i = 0; i < csNum_; i++){
			CsCmdQueue tmp(i, bankNum_, qLen_);
			wrCcq.push_back(tmp);
		}
	 	for(int i = 0; i < csNum_; i++){
			CsCmdQueue tmp(i, bankNum_, qLen_);
			rdCcq.push_back(tmp);
		}
	 }

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
	
// to do !
//void Controller::print_summary(std::size_t cycles){
//	cout << "Controller " << ControllerId << " print summary:" << endl;
//	cmdq.print_summary(cycles);
//	cout << endl;
//}

void Controller::print_summary(std::size_t cycles){
	cout << "Controller " << ControllerId << " print summary:" << endl;
	size_t rdCounter = 0, wrCounter = 0, reqCounter = 0, preCounter = 0, actCounter = 0;
	
	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++){
		rdCounter += (*i).rdCounter;
		wrCounter += (*i).wrCounter;
		preCounter += (*i).preCounter;
		actCounter += (*i).actCounter;
	}
	for(auto i = rdCcq.begin(); i < rdCcq.end(); i++){
		rdCounter += (*i).rdCounter;
		wrCounter += (*i).wrCounter;
		preCounter += (*i).preCounter;
		actCounter += (*i).actCounter;
	}
	reqCounter = wrCounter + rdCounter;

	cout << "    rd request num: " << rdCounter << endl 
		 << "    wr request num: " << wrCounter << endl
		 << "    rw summary: " << reqCounter << endl 
		 << endl 
		 << "    pre command num: " << preCounter << endl
		 << "    act command num: " << actCounter << endl;
	cout << "    the bus occupied rate = " << ((double)reqCounter*4/cycles) << endl;
}

void Controller::send_wrpre(const Req& req){
	cout << "    Controller send pre to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	wrCcq[req.cs].send_pre(req);
	mcsm.send_pre(req);
}
void Controller::send_wract(const Req& req){
	cout << "    Controller send act to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	wrCcq[req.cs].send_act(req);
	mcsm.send_act(req);
}
void Controller::send_rdpre(const Req& req){
	cout << "    Controller send pre to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	rdCcq[req.cs].send_pre(req);
	mcsm.send_pre(req);
}
void Controller::send_rdact(const Req& req){
	cout << "    Controller send act to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	rdCcq[req.cs].send_act(req);
	mcsm.send_act(req);
}
void Controller::send_rd(const Req& req){
	cout << "    Controller send rd to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	rdCcq[req.cs].send_rd(req);
	mcsm.send_rd(req);

	if(rdCnt!=0)
		rdCnt--;
}
void Controller::send_wr(const Req& req){
	cout << "    Controller send wr to cs " << req.cs << ", bank " << req.bank << " " << endl;;
	wrCcq[req.cs].send_wr(req);
	mcsm.send_wr(req);

	if(wrCnt!=0)
		wrCnt--;
}

bool Controller::check_status(const CmdStatus &status){
	if(((int)status.needPre + 
	    (int)status.needAct + 
	    (int)status.needRd + 
	    (int)status.needWr) >= 2)
		return false;
	if(((int)status.gntPre + 
	    (int)status.gntAct + 
	    (int)status.gntRd + 
	    (int)status.gntWr) >= 2)
		return false;
	return true;
}

bool Controller::refresh_control(){
	bool is_send = false;
	for(std::size_t i = 0; i < csNum; i++){
		if(mcsm.need_refresh(i)){
			mcsm.send_ref(i);
			is_send = true;
			break;
		}
	}
	return is_send;
}

bool Controller::arbitrate_and_send(){
	vector<Req> wrpre, wract, rdpre, rdact, rd, wr, wrnop, rdnop;

	for(auto i = rdCcq.begin(); i < rdCcq.end(); i++){
		for(auto j = (*i).bcq.begin(); j < (*i).bcq.end(); j++){
			if(!(*j).valid())
				continue;
			const Req &tmpR = (*j).get_head();
			CmdStatus tmpS = mcsm.get_cmdStatus(tmpR);
			if(!check_status(tmpS)){
				cout << "ERROR - check_status failed in Controller arbitrate()" << endl;
				exit(0);
			}
			if(tmpS.needPre && tmpS.gntPre)
				rdpre.push_back(tmpR);
			else if(tmpS.needAct && tmpS.gntAct)
				rdact.push_back(tmpR);
			else if(tmpS.needRd && tmpS.gntRd)
				rd.push_back(tmpR);
			else
				rdnop.push_back(tmpR);
		}
	}

	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++){
		for(auto j = (*i).bcq.begin(); j < (*i).bcq.end(); j++){
			if(!(*j).valid())
				continue;
			const Req &tmpR = (*j).get_head();
			CmdStatus tmpS = mcsm.get_cmdStatus(tmpR);
			if(!check_status(tmpS)){
				cout << "ERROR - check_status failed in Controller arbitrate()" << endl;
				exit(0);
			}
			if(tmpS.needPre && tmpS.gntPre)
				wrpre.push_back(tmpR);
			else if(tmpS.needAct && tmpS.gntAct)
				wract.push_back(tmpR);
			else if(tmpS.needWr && tmpS.gntWr)
				wr.push_back(tmpR);
			else
				wrnop.push_back(tmpR);
		}
	}

	if(wrPrior){
		if(wr.empty() && wrpre.empty() && wract.empty())
			return false;
		if(!wr.empty()){
			send_wr(wr.front());
			return true;
		}
		else if(!wract.empty()){
			send_wract(wract.front());
			return true;
		}
		else if(!wrpre.empty()){
			send_wrpre(wrpre.front());
			return true;
		}
		return false;
	}
	else{
		if(rd.empty() && rdpre.empty() && rdact.empty())
			return false;
		if(!rd.empty()){
			send_rd(rd.front());
			return true;
		}
		else if(!rdact.empty()){
			send_rdact(rdact.front());
			return true;
		}
		else if(!rdpre.empty()){
			send_rdpre(rdpre.front());
			return true;
		}
		return false;
	}

	return false;
}
void Controller::reorder(){
	for(size_t i = 0; i < csNum; i++){
		for(size_t j = 0; j < bankNum; j++){
			size_t row = mcsm.get_actRow(i,j);
			wrCcq[i].bcq[j].reorder(row);
			rdCcq[i].bcq[j].reorder(row);
		}
	}
}

void Controller::run_step(){
	cout << "Controller " << ControllerId << " run_step()..." << endl;
	if(inPort.valid()){
		if(!tempValid){
			tempReq = inPort.get_req();
			tempValid = true;
			map_addr(tempReq);
		}
	}
	
	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++){
		(*i).accept_req();
	}
	for(auto i = rdCcq.begin(); i < rdCcq.end(); i++){
		(*i).accept_req();
	}

	bool is_send_ref;
	is_send_ref = refresh_control();

	if(!is_send_ref)
		arbitrate_and_send();

	mcsm.run_step();
}

size_t Controller::get_wrreqNum(){
	size_t sum = 0;
	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++)
		sum += (*i).get_reqNum();
	return sum;
}

void Controller::update(){

	if(tempValid){
		if(tempReq.write){
			if(wrCcq[tempReq.cs].ready()){
				wrCcq[tempReq.cs].receive_req(tempReq);
				tempValid = false;
			}
		}
		else{
			if(rdCcq[tempReq.cs].ready()){
				rdCcq[tempReq.cs].receive_req(tempReq);
				tempValid = false;
			}
		}
	}
	
	bool wrValid = false;
	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++){
		wrValid = wrValid || (*i).valid();
	}
	bool rdValid = false;
	for(auto i = rdCcq.begin(); i < rdCcq.end(); i++){
		rdValid = rdValid || (*i).valid();
	}
	if(wrPrior){
		if(!wrValid || (wrCnt == 0)){
			wrPrior = false;
			rdCnt = RD_TURNCNT;
			reorder();
		}
	}
	else{
		if(!rdValid || ((get_wrreqNum() >= WR_CMDTH_H) && (rdCnt == 0))){
			wrPrior = true;
			wrCnt = WR_TURNCNT;
			reorder();
		}
	}

	for(auto i = wrCcq.begin(); i < wrCcq.end(); i++)
		(*i).update();
	for(auto i = rdCcq.begin(); i < rdCcq.end(); i++)
		(*i).update();
	mcsm.update();
}


