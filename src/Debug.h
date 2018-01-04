#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>

#ifdef DEBUG_PRINT
	#define DEBUG(str) std::cout << str << std::endl;
#else
	#define DEBUG(str);
#endif

#endif //for file
