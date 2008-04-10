/* $Id: global.h,v 1.12 2008/04/10 18:33:36 wyverex Exp $ */

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
	#else
		#include <pthread.h>
		#include <sys/stat.h>
	#endif

#endif
