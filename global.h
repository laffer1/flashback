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
	using namespace std;

	#ifdef _WIN32
		#include <windows.h>
		#include <tchar.h>
		#include <strsafe.h>
		#define PATH_NAME_SEPARATOR '\\'

		/* strsep is not available on Windows */
		static inline char *strsep(char **sp, const char *delim) {
			char *s, *p;
			if ((s = *sp) == NULL) return NULL;
			p = strpbrk(s, delim);
			if (p != NULL) { *p = '\0'; *sp = p + 1; }
			else *sp = NULL;
			return s;
		}
	#else
		#include <unistd.h>
		#include <sys/stat.h>
		#include <sys/types.h>
		#include <pthread.h>
		#define PATH_NAME_SEPARATOR '/'
	#endif

	/* PATH_MAX is not defined on Windows; provide a fallback */
	#ifndef PATH_MAX
	#define PATH_MAX 4096
	#endif

#endif
