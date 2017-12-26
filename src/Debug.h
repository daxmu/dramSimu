#include <iostream>
using namespace std;


#ifdef DEBUG_PRINT
	#define DEBUG(str) cout << str << endl;
#else
	#define DEBUG(str);
#endif
