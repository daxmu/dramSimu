#ifndef _BASESTRUCT_H_
#define _BASESTRUCT_H_

#include<iostream>
#include<deque>
#include"define.h"

template <class T>
class FIFO{
private:
	std::deque<T> queue;
	uint32_t length;
public:
	FIFO() = default;
	FIFO(int l):length(l){}

	void set_length(int l){
		length = l;
	}

	bool is_full() const{
	#ifdef DEBUG_PRINT
		if(queue.size() > length){
			std::cout << "ERROR - FIFO is overflow in is_full" << std::endl;
		}
	#endif
		return queue.size() >= length;
	}
	bool is_empty() const{
		return queue.empty();
	}
	bool write(const T a){
	#ifdef DEBUG_PRINT
		if(queue.size() > length){
			std::cout << "ERROR - FIFO is overflow in write" << std::endl;
		}
	#endif
		if(queue.size() < length){
			queue.push_back(a);
			return true;
		}
		return false;
	}
	T read(){
	#ifdef DEBUG_PRINT
		if(queue.empty()){
			std::cout << "ERROR - FIFO is empty in read" << std::endl;
			return T{0};
		}
	#endif
		T tmp = queue.front();
		queue.pop_front();
		return tmp;
	}
	const T& get_head(){
	#ifdef DEBUG_PRINT
		if(queue.empty()){
			std::cout << "ERROR - FIFO is empty in read" << std::endl;
			exit(0);
		}
	#endif
		return queue[0];
	}
	void print_status(){
		std::cout << "    " << queue.size() << std::endl;
		for(auto i = queue.rbegin(); i < queue.rend(); i++){
			std::cout << "    " << std::hex << (*i).addr << std::dec << std::endl;
		}
	}
};

#endif
