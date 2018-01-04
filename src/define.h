#ifndef _DEFINE_H_
#define _DEFINE_H_

#include<iostream>
#include<stdint.h>
#define DEBUG_PRINT 

typedef std::size_t Addr;
typedef std::size_t Data;

const std::size_t CHANNEL_NUM = 2;
const std::size_t CHANNEL_ROUTEBIT = 4;

const std::size_t CS_BIT_NUM = 0;
const std::size_t CID_BIT_NUM = 0;
const std::size_t BG_BIT_NUM = 2;
const std::size_t BANK_BIT_NUM = 2;
const std::size_t ROW_LOWBIT_NUM = 10;
const std::size_t ROW_HIGHBIT_NUM = 0;
const std::size_t COL_LOWBIT_NUM = 4;
const std::size_t COL_HIGHBIT_NUM = 6;

const std::size_t CS_BIT_POS = 0;
const std::size_t CID_BIT_POS = 0;
const std::size_t BG_BIT_POS = 13;
const std::size_t BANK_BIT_POS = 11;
const std::size_t ROW_LOWBIT_POS = 15;
const std::size_t ROW_HIGHBIT_POS = 0;
const std::size_t COL_LOWBIT_POS = 0;
const std::size_t COL_HIGHBIT_POS = 5;


uint64_t LOG2(uint64_t a);
uint64_t POW2(uint64_t a);
Addr get_corBits(Addr, std::size_t, std::size_t);

template<class T>
inline const T& max(const T &a, const T &b){
	return a > b ? a : b;
}

#endif
