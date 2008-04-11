/* $Id: global.h,v 1.13 2008/04/11 05:09:29 ctubbsii Exp $ */

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
	#include <csignal>
	#include <cstdarg>

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
