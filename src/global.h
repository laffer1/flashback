/* $Id: global.h,v 1.15 2008/04/23 01:42:26 ctubbsii Exp $ */

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
	#include <queue>
	#include <csignal>
	#include <cstdarg>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>

	using namespace std;

	#ifdef Win32
		#include <windows.h>
		#include <tchar.h>
		#include <strsafe.h>
		#define PATH_NAME_SEPARATOR '\\'
	#else
		#include <pthread.h>
		#include <sys/stat.h>
		#define PATH_NAME_SEPARATOR '/'
	#endif

#endif
