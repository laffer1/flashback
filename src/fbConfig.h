/* $Id: fbConfig.h,v 1.8 2008/04/23 00:41:11 ctubbsii Exp $ */
/*
 * Copyright (c) 2008 Chris Tubbs
 * All rights reserved.
 * Do whatever you want with this code.
 *
*/
#ifndef FBCONFIG_H
#define FBCONFIG_H

#include "global.h"
#include "fbErrorLogger.h"

#define FBCONFIG_DEFAULT_ADDR "127.0.0.1"
#define FBCONFIG_DEFAULT_PORT 8080
#define FBCONFIG_DEFAULT_WEBROOT "/usr/local/share/flashback/www/"
#define FBCONFIG_DEFAULT_DBPATH "/var/flashback/flashback.db"


#ifdef WIN32
	#define FBCONFIG_DEFAULT_CONFIGFILE "flashback.txt"
#else
	#define FBCONFIG_DEFAULT_CONFIGFILE "/etc/flashback"
#endif

/**
 * fbConfig
 *
 * Provides an interface for loading and saving configuration data.
 * @author Christopher Tubbs
 * @date March 18, 2008
 */
class fbConfig
{
public:
	fbConfig(fbErrorLogger* err);
	fbConfig(fbErrorLogger* err, const char* filename);
	~fbConfig();

    void loadDefaults();

    int load();
    int save();

	void setWebServerAddr(const string& strAddr);
	void setWebServerPort(const int port);
	void setWebServerRootPath(const string& strPath);
	void setDBPath(const string& strPath);

	const string& getWebServerAddr();
	const int   getWebServerPort();
	const string& getWebServerRootPath();
	const string& getDBPath();

private:
    int load(const char *filename);
    int save(const char *filename);
    void setWebServerPort(const string& strPort);

    string addr;
    string webroot;
    string dbpath;

    int port;

    fbErrorLogger* errlog;

    bool dirty;
};

#endif
