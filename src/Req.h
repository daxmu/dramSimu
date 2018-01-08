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
	bool write;
	bool valid;
	int masterId;

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
