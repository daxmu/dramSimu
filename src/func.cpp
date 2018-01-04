#include "define.h"

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


