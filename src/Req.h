#ifndef _REQ_H_
#define _REQ_H_

#include<iostream>
using namespace std;

struct Req{
	uint64_t addr;
	int cs;
	int cid;
	int bg;
	int bank;
	int row;
	int col;
	bool write;
	uint64_t data;
	bool valid;

	void print(){
		cout << "------------------Req print----------------" << endl;
		cout << "addr: " << addr << endl
			 << "cs: " << cs << endl
			 << "cid: " << cid << endl
			 << "bg: " << bg << endl
			 << "bank: " << bank << endl
			 << "row: " << row << endl
			 << "col: " << col << endl
			 << "write: " << write << endl
			 << "data: " << data << endl
			 << "valid: " << valid << endl;
	}
};

#endif
