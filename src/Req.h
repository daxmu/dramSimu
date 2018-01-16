#ifndef _REQ_H_
#define _REQ_H_

#include<iostream>
#include "define.h"

struct Req{
	Addr addr;
	std::size_t cs;
	std::size_t cid;
	std::size_t bg;
	std::size_t bank;
	std::size_t row;
	std::size_t col;
	std::size_t data;
	std::size_t length;
	bool write;
	bool valid;
	int masterId;

	Req():addr(0), cs(0), cid(0), bg(0), bank(0), row(0), 
		  col(0), data(0), length(0), write(0), valid(0), masterId(0){}

	Req(int a):addr(0), cs(0), cid(0), bg(0), bank(0), row(0), 
		  col(0), data(0), length(0), write(0), valid(0), masterId(0){}

	friend bool operator==(const Req& lhs, const Req& rhs){
		if((lhs.addr == rhs.addr) &&
		   (lhs.cs   == rhs.cs) &&
		   (lhs.cid  == rhs.cid) &&
		   (lhs.bg   == rhs.bg) &&
		   (lhs.bank == rhs.bank) &&
		   (lhs.row  == rhs.row) &&
		   (lhs.col  == rhs.col) &&
		   (lhs.data == rhs.data) &&
		   (lhs.write== rhs.write) &&
		   (lhs.valid== rhs.valid) &&
		   (lhs.masterId  == rhs.masterId))
			return true;
		else
			return false;
	}
	friend bool operator!=(const Req& lhs, const Req& rhs){
		return !(lhs==rhs);
	}
	void print() const {
		//std::cout << "------------------Req print----------------" << std::endl;
		std::cout << std::hex;
		std::cout << "addr: " 	<< addr << std::endl
			 << "masterId " << masterId << std::endl
			 << "cs: " 		<< cs << std::endl
			 << "cid: " 	<< cid << std::endl
			 << "bg: " 		<< bg << std::endl
			 << "bank: " 	<< bank << std::endl
			 << "row: " 	<< row << std::endl
			 << "col: " 	<< col << std::endl
			 << "write: " 	<< write << std::endl
			 << "data: " 	<< data << std::endl
			 << "valid: " 	<< valid << std::endl;
		std::cout << std::dec;
	}
};

#endif
