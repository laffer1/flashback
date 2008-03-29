/* $Id: global.h,v 1.10 2008/03/29 22:19:29 wyverex Exp $ */

#ifndef GLOBAL_H
#define GLOBAL_H

	#include <iostream>
	#include <string>
	//#include <sstream>
	#include <cstring>
	#include <cmath>
	#include <cstdlib>
	#include <cstdio>
	#include <fstream>
	#include <vector>
	using namespace std;

	#ifdef Win32
		#include <windows.h>
		#include <tchar.h>
		#include <strsafe.h>
	#else
		#include <pthread.h>
		#include <sys/stat.h>
	#endif

#endif
