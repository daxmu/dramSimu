#ifndef _DEFINE_H_
#define _DEFINE_H_

#include<iostream>
#include<stdint.h>
#include<time.h>
#define DEBUG_PRINT 
#define INSERT_STRATEGY_1
//#define INSERT_STRATEGY_2

typedef std::size_t Addr;
typedef std::size_t Data;

const std::size_t CHANNEL_NUM = 2;
const std::size_t CHANNEL_ROUTEBIT = 4;

const std::size_t COMMAND_QUEUE_LENGTH = 16;

const std::size_t CS_NUM = 2;
const std::size_t BANK_NUM = 8;

const std::size_t CS_BIT_NUM = 1;
const std::size_t CID_BIT_NUM = 0;
const std::size_t BG_BIT_NUM = 0;
const std::size_t BANK_BIT_NUM = 3;
const std::size_t ROW_LOWBIT_NUM = 10;
const std::size_t ROW_HIGHBIT_NUM = 0;
const std::size_t COL_LOWBIT_NUM = 4;
const std::size_t COL_HIGHBIT_NUM = 6;

const std::size_t CS_BIT_POS = 14;
const std::size_t CID_BIT_POS = 0;
const std::size_t BG_BIT_POS = 0;
const std::size_t BANK_BIT_POS = 11;
const std::size_t ROW_LOWBIT_POS = 15;
const std::size_t ROW_HIGHBIT_POS = 0;
const std::size_t COL_LOWBIT_POS = 0;
const std::size_t COL_HIGHBIT_POS = 5;

const std::size_t tCL	= 11;
const std::size_t tRAS	= 24;
const std::size_t tRCD	= 11;
const std::size_t tRRD	= 4;
const std::size_t tRC	= 38;
const std::size_t tRP	= 11;
const std::size_t tCCD	= 4;
const std::size_t tRTP	= 4;
const std::size_t tWTR	= 5;
const std::size_t tWR	= 12;
const std::size_t tRTRS	= 0;
const std::size_t tRFC	= 280;
const std::size_t tREF	= 6240;
const std::size_t tFAW	= 17;
const std::size_t tCKE	= 48;
const std::size_t tXP	= 5;

const std::size_t tAL = 0;
const std::size_t tWL = 7;
const std::size_t tRL = tAL + tCL;

//---------------same ba set-------------------------
const std::size_t t_p2p_sameba = 0;
const std::size_t t_p2a_sameba = tRP;
const std::size_t t_p2r_sameba = 0;
const std::size_t t_p2w_sameba = 0;

const std::size_t t_a2p_sameba = tRAS;
const std::size_t t_a2a_sameba = tRC;
const std::size_t t_a2r_sameba = tRCD;
const std::size_t t_a2w_sameba = tRCD;

const std::size_t t_r2p_sameba = tRTP;
const std::size_t t_r2a_sameba = 0;
const std::size_t t_r2r_sameba = tCCD;
const std::size_t t_r2w_sameba = (tRL + tCCD + 2 - tWL);

const std::size_t t_w2p_sameba = tWL + tCCD + tWR;
const std::size_t t_w2a_sameba = 0;
const std::size_t t_w2r_sameba = tWL + tCCD + tWTR;
const std::size_t t_w2w_sameba = tCCD;

//---------------same cs set-------------------------
const std::size_t t_p2p_samecs = 0;
const std::size_t t_p2a_samecs = 0;
const std::size_t t_p2r_samecs = 0;
const std::size_t t_p2w_samecs = 0;

const std::size_t t_a2p_samecs = 0;
const std::size_t t_a2a_samecs = 0;
const std::size_t t_a2r_samecs = 0;
const std::size_t t_a2w_samecs = 0;

const std::size_t t_r2p_samecs = tRTP;
const std::size_t t_r2a_samecs = 0;
const std::size_t t_r2r_samecs = tCCD;
const std::size_t t_r2w_samecs = (tRL + tCCD + 2 - tWL);

const std::size_t t_w2p_samecs = tWL + tCCD + tWR;
const std::size_t t_w2a_samecs = 0;
const std::size_t t_w2r_samecs = tWL + tCCD + tWTR;
const std::size_t t_w2w_samecs = tCCD;

//---------------diff cs set-------------------------
const std::size_t t_w2w_diffcs = tCCD;
const std::size_t t_w2r_diffcs = tCCD + 2;
const std::size_t t_r2w_diffcs = tRL + tCCD + 2 - tWL;
const std::size_t t_r2r_diffcs = tCCD;


uint64_t LOG2(uint64_t a);
uint64_t POW2(uint64_t a);
Addr get_corBits(Addr, std::size_t, std::size_t);

template<class T>
inline const T& max_(const T &a, const T &b){
	return a > b ? a : b;
}

void print_config_message();

#endif
