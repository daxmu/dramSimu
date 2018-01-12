#include <vector>
#include "define.h"
using namespace std;

uint64_t LOG2(uint64_t a){
	int tmp = 0;
	while(a!=0){
		a = a >> 1;
		tmp++;
	}
	return tmp;
}

uint64_t POW2(uint64_t a){
	int tmp = 1;
	while(a!=0){
		a--;
		tmp *= 2;
	}
	return tmp;
}

Addr get_corBits(Addr a, std::size_t num, std::size_t pos){
	Addr validBit = POW2(num)-1;
	return (a >> pos) & validBit;
}


void print_config_message(){
	time_t timep;
	time(&timep);
	std::cout << asctime(gmtime(&timep)) << std::endl;

	std::cout << "*************The dramSimu config message*****************" << std::endl;
	std::cout << "CHANNEL_NUM           = "  << CHANNEL_NUM << std::endl 
			  << "CHANNEL_ROUTEBIT      = "  << CHANNEL_ROUTEBIT << std::endl 

			  << "COMMAND_QUEUE_LENGTH  = "  << COMMAND_QUEUE_LENGTH << std::endl 

			  << "CS_NUM                = "  << CS_NUM << std::endl 
			  << "BANK_NUM              = "  << BANK_NUM << std::endl 

			  << "CS_BIT_NUM            = "  << CS_BIT_NUM << std::endl 
			  << "CID_BIT_NUM           = "  << CID_BIT_NUM << std::endl 
			  << "BG_BIT_NUM            = "  << BG_BIT_NUM << std::endl 
			  << "BANK_BIT_NUM          = "  << BANK_BIT_NUM << std::endl 
			  << "ROW_LOWBIT_NUM        = "  << ROW_LOWBIT_NUM << std::endl 
			  << "ROW_HIGHBIT_NUM       = "  << ROW_HIGHBIT_NUM << std::endl 
			  << "COL_LOWBIT_NUM        = "  << COL_LOWBIT_NUM << std::endl 
			  << "COL_HIGHBIT_NUM       = "  << COL_HIGHBIT_NUM << std::endl 

			  << "CS_BIT_POS            = "  << CS_BIT_POS << std::endl 
			  << "CID_BIT_POS           = "  << CID_BIT_POS << std::endl 
			  << "BG_BIT_POS            = "  << BG_BIT_POS << std::endl 
			  << "BANK_BIT_POS          = "  << BANK_BIT_POS << std::endl 
			  << "ROW_LOWBIT_POS        = "  << ROW_LOWBIT_POS << std::endl 
			  << "ROW_HIGHBIT_POS       = "  << ROW_HIGHBIT_POS << std::endl 
			  << "COL_LOWBIT_POS        = "  << COL_LOWBIT_POS << std::endl 
			  << "COL_HIGHBIT_POS       = "  << COL_HIGHBIT_POS << std::endl 

			  << "tCL                   = "  << tCL	<< std::endl 
			  << "tRAS                  = "  << tRAS	<< std::endl 
			  << "tRCD                  = "  << tRCD	<< std::endl 
			  << "tRRD                  = "  << tRRD	<< std::endl 
			  << "tRC                   = "  << tRC	<< std::endl 
			  << "tRP                   = "  << tRP	<< std::endl 
			  << "tCCD                  = "  << tCCD	<< std::endl 
			  << "tRTP                  = "  << tRTP	<< std::endl 
			  << "tWTR                  = "  << tWTR	<< std::endl 
			  << "tWR                   = "  << tWR	<< std::endl 
			  << "tRTRS                 = "  << tRTRS	<< std::endl 
			  << "tRFC                  = "  << tRFC	<< std::endl 
			  << "tFAW                  = "  << tFAW	<< std::endl 
			  << "tCKE                  = "  << tCKE	<< std::endl 
			  << "tXP                   = "  << tXP	<< std::endl 

			  << "tAL                   = "  << tAL << std::endl 
			  << "tWL                   = "  << tWL << std::endl 
			  << "tRL                   = "  << tRL << std::endl 

			  << "t_p2p_sameba          = "  << t_p2p_sameba << std::endl 
			  << "t_p2a_sameba          = "  << t_p2a_sameba << std::endl 
			  << "t_p2r_sameba          = "  << t_p2r_sameba << std::endl 
			  << "t_p2w_sameba          = "  << t_p2w_sameba << std::endl 

			  << "t_a2p_sameba          = "  << t_a2p_sameba << std::endl 
			  << "t_a2a_sameba          = "  << t_a2a_sameba << std::endl 
			  << "t_a2r_sameba          = "  << t_a2r_sameba << std::endl 
			  << "t_a2w_sameba          = "  << t_a2w_sameba << std::endl 

			  << "t_r2p_sameba          = "  << t_r2p_sameba << std::endl 
			  << "t_r2a_sameba          = "  << t_r2a_sameba << std::endl 
			  << "t_r2r_sameba          = "  << t_r2r_sameba << std::endl 
			  << "t_r2w_sameba          = "  << t_r2w_sameba << std::endl 

			  << "t_w2p_sameba          = "  << t_w2p_sameba << std::endl 
			  << "t_w2a_sameba          = "  << t_w2a_sameba << std::endl 
			  << "t_w2r_sameba          = "  << t_w2r_sameba << std::endl 
			  << "t_w2w_sameba          = "  << t_w2w_sameba << std::endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << std::endl << std::endl;
}

bool check_config(){
	vector<int> start, end;

	start.push_back(CHANNEL_ROUTEBIT);
	end.push_back(CHANNEL_ROUTEBIT + 1);

	if(CS_BIT_NUM){
		start.push_back(CS_BIT_POS);
		end.push_back(CS_BIT_POS + CS_BIT_NUM);
	}
	if(CID_BIT_NUM){
		start.push_back(CID_BIT_POS);
		end.push_back(CID_BIT_POS + CID_BIT_NUM);
	}
	if(BG_BIT_NUM){
		start.push_back(BG_BIT_POS);
		end.push_back(BG_BIT_POS + BG_BIT_NUM);
	}
	if(BANK_BIT_NUM){
		start.push_back(BANK_BIT_POS);
		end.push_back(BANK_BIT_POS + BANK_BIT_NUM);
	}
	if(ROW_LOWBIT_NUM){
		start.push_back(ROW_LOWBIT_POS);
		end.push_back(ROW_LOWBIT_POS + ROW_LOWBIT_NUM);
	}
	if(ROW_HIGHBIT_NUM){
		start.push_back(ROW_HIGHBIT_POS);
		end.push_back(ROW_HIGHBIT_POS + ROW_HIGHBIT_NUM);
	}
	if(COL_LOWBIT_NUM){
		start.push_back(COL_LOWBIT_POS);
		end.push_back(COL_LOWBIT_POS + COL_LOWBIT_NUM);
	}
	if(COL_HIGHBIT_NUM){
		start.push_back(COL_HIGHBIT_POS);
		end.push_back(COL_HIGHBIT_POS + COL_HIGHBIT_NUM);
	}

	for(auto i = start.begin(); i < start.end(); i++){
		for(auto j = end.begin(); j < end.end(); j++){
			auto k = start.begin() + (j - end.begin());
			if(i != k){
				if(((*i) < (*j)) && ((*i) >= (*k))){
					cout << "ERROR - in check config " << endl;
					cout << *i << " " << *k << " " << *j << endl;
					exit(0);
				}
			}
		}
	}

	cout << "Check config Success!" << endl;
	return true;
}



