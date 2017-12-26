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
};
