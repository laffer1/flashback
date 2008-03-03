#ifndef GLOBAL_H
#define GLOBAL_H

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <cmath>
	#include <cstdlib>
	#include <cstdio>
	#include <fstream>
	using namespace std;

	#ifdef Win32
		#include <windows.h>
	#else
		#include <pthread.h>
		#include <fnctl.h>
	#endif


#endif
