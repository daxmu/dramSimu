#ifndef _BASESTRUCT_H_
#define _BASESTRUCT_H_

#include<iostream>
#include<deque>
#include"define.h"


using namespace std;

template <class T>
class FIFO{
private:
	deque<T> queue;
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
			cout << "ERROR - FIFO is overflow in is_full" << endl;
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
			cout << "ERROR - FIFO is overflow in write" << endl;
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
			cout << "ERROR - FIFO is empty in read" << endl;
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
			cout << "ERROR - FIFO is empty in read" << endl;
			exit(0);
		}
	#endif
		return queue[0];
	}
};

#endif
