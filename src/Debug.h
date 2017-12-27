#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>
using namespace std;


#ifdef DEBUG_PRINT
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str);
#endif

#endif //for file
