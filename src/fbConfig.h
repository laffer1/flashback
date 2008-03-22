/* $Id: fbConfig.h,v 1.1 2008/03/22 19:22:53 ctubbsii Exp $ */

#ifndef FBCONFIG_H
#define FBCONFIG_H

#include "global.h"
#include "fbErrorLogger.h"

#define FBCONFIG_DEFAULT_ADDR "127.0.0.1"
#define FBCONFIG_DEFAULT_PORT 8080
#define FBCONFIG_DEFAULT_WEBROOT "/var/flashback/webroot"
#define FBCONFIG_DEFAULT_DBPATH "/etc/dbpath"

#define FBCONFIG_DEFAULT_CONFIGFILE "config.txt"

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
	fbConfig(const fbErrorLogger &err);
	fbConfig(const fbErrorLogger &err, const char* filename);
	~fbConfig();

    void loadDefaults();

    int load();
    int load(const char *filename);
    int save();
    int save(const char *filename);

	void setWebServerAddr(const char* strAddr);
	void setWebServerPort(const char* strPort);
	void setWebServerPort(const int port);
	void setWebServerRootPath(const char* strPath);
	void setDBPath(const char* strPath);

	const char* getWebServerAddr();
	const int   getWebServerPort();
	const char* setWebServerRootPath();
	const char* getDBPath();

private:
    char* addr;
    int port;
    char* webroot;
    char* dbpath;

    bool dirty;
};

#endif
