#ifndef _REQ_H_
#define _REQ_H_

#include<iostream>
#include "define.h"
using namespace std;

struct Req{
	Addr addr;
	std::size_t cs;
	std::size_t cid;
	std::size_t bg;
	std::size_t bank;
	std::size_t row;
	std::size_t col;
	bool write;
	uint64_t data;
	bool valid;
	int masterId;

	void print() const {
		//cout << "------------------Req print----------------" << endl;
		cout << hex;
		cout << "addr: " 	<< addr << endl
			 << "masterId " << masterId << endl
			 << "cs: " 		<< cs << endl
			 << "cid: " 	<< cid << endl
			 << "bg: " 		<< bg << endl
			 << "bank: " 	<< bank << endl
			 << "row: " 	<< row << endl
			 << "col: " 	<< col << endl
			 << "write: " 	<< write << endl
			 << "data: " 	<< data << endl
			 << "valid: " 	<< valid << endl;
		cout << dec;
	}
};

#endif
