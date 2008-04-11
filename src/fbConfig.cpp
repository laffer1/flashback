/* $Id: fbConfig.cpp,v 1.11 2008/04/11 05:09:29 ctubbsii Exp $ */

#include "fbConfig.h"

/**
 * fbConfig
 *
 * Provides an interface for loading and saving configuration data.
 * @author Christopher Tubbs
 * @date March 18, 2008
 */
fbConfig::fbConfig(fbErrorLogger* err): port(0), errlog(err), dirty(true)
{
    loadDefaults(); // assume defaults
    if (load() < 0)
    {
    // INFO: can't load settings from default config file, using defaults
        errlog->warn(CONFIGFAILEDTOLOAD, "using defaults");
        dirty = true;
    }
    else
        dirty = false;

    errlog->debug(NONE, "fbConfig.this");
}

fbConfig::fbConfig(fbErrorLogger* err, const char* filename): port(0), errlog(err), dirty(true)
{
    loadDefaults(); // assume defaults
    if (load(filename) < 0)
    {
        // INFO: can't load settings from explicit config file, using defaults
        errlog->warn(CONFIGFAILEDTOLOAD, "using defaults");
    }
    dirty = true; // save to default config file regardless

    errlog->debug(NONE, "fbConfig.this");
}

fbConfig::~fbConfig()
{
    /* we can try to save here, but don't want to rely on it */
    /* also, should we save to the default file or to the one that
        initially provided the settings? */
    if (dirty) save();
    errlog->debug(NONE, "fbConfig.~this");
}

void fbConfig::loadDefaults()
{
    addr = FBCONFIG_DEFAULT_ADDR;
    port = FBCONFIG_DEFAULT_PORT;
    webroot = FBCONFIG_DEFAULT_WEBROOT;
    dbpath = FBCONFIG_DEFAULT_DBPATH;
}

int fbConfig::load()
{
    return load(FBCONFIG_DEFAULT_CONFIGFILE);
}

int fbConfig::load(const char* filename)
{
    string s;
    ifstream myfile;

    errlog->debug(NONE, "Loading config file: %s", filename);

    myfile.open(filename, ios::in);
    if (myfile.fail())
    {
        errlog->warn(NONE, "Cannot open config file: %s", filename);
        return -1;
    }
    while (!myfile.eof() && !myfile.fail())
    {
        string::size_type i;
        getline(myfile, s, '\n');
        if (myfile.fail()) break;

        if ((i = s.find("WebServerAddr=",0) != string::npos))
        {
            setWebServerAddr(s.substr((int)i+13));
            errlog->debug(NONE, "Loaded WebServerAddr=%s", s.substr((int)i+13).c_str());
        }
        else if ((i = s.find("WebServerPort=",0) != string::npos))
        {
            setWebServerPort(s.substr((int)i+13));
            errlog->debug(NONE, "Loaded WebServerPort=%s", s.substr((int)i+13).c_str());
        }
        else if ((i = s.find("WebServerRootPath=",0) != string::npos))
        {
            setWebServerRootPath(s.substr((int)i+17));
            errlog->debug(NONE, "Loaded WebServerRootPath=%s", s.substr((int)i+17).c_str());
        }
        else if ((i = s.find("DBPath=",0) != string::npos))
        {
            setDBPath(s.substr((int)i+6));
            errlog->debug(NONE, "Loaded DBPath=%s", s.substr((int)i+6).c_str());
        }
        else
        {
            errlog->warn(CONFIGUNKOWNVALUE, s);
        }
    }

    if (myfile.fail() && !myfile.eof())
    {
        myfile.close();
        // ERROR: error loading from config file
        errlog->err(CONFIGFAILEDTOLOAD, "Cannot load from config file: %s", filename);
        return -1;
    }

    myfile.close();
    // INFO: successfully loaded some settings. remaining settings from defaults
    errlog->msg(NONE, "Successfully loaded settings from config file: %s", filename);
    return 0;
}

int fbConfig::save()
{
    int i = 0;
    if ((i = save(FBCONFIG_DEFAULT_CONFIGFILE)))
        dirty = false;
    return i;
}

int fbConfig::save(const char* filename)
{
    ofstream myfile;
    myfile.open(filename, ios::out);

    errlog->debug(NONE, "Attempting to save config file: %s", filename);
    if (myfile.fail())
    {
        // ERROR: error opening config file to save
        errlog->err(CONFIGFAILEDTOSAVE, "error opening config file to save");
        return -1;
    }
    myfile << "WebServerAddr=" << addr << endl
        << "WebServerPort=" << port << endl
        << "WebServerRootPath=" << webroot << endl
        << "DBPath=" << dbpath << endl;
    if (myfile.fail())
    {
        // ERROR: problem writing to config file
        errlog->err(CONFIGFAILEDTOSAVE, "problem writing to config file");
        myfile.close();
        return -1;
    }
    myfile.close();

    errlog->debug(NONE, "Saved config file: %s", filename);
    return 0;
}

void fbConfig::setWebServerAddr(const string& strAddr)
{
    dirty = true;
    addr = strAddr;
}

void fbConfig::setWebServerPort(const string& strPort)
{
    //istringstream s(strPort);
    //s >> port;
    port = atol(strPort.c_str());
    dirty = true;
}

void fbConfig::setWebServerPort(int intPort)
{
    dirty = true;
    port = intPort; // may need checking for validity
}

void fbConfig::setWebServerRootPath(const string& strPath)
{
    dirty = true;
    webroot = strPath;
}

void fbConfig::setDBPath(const string& strPath)
{
    dirty = true;
    dbpath = strPath;
}

const string& fbConfig::getWebServerAddr()
{
    return addr;
}

const int fbConfig::getWebServerPort()
{
    return port;
}

const string& fbConfig::getWebServerRootPath()
{
    return webroot;
}

const string& fbConfig::getDBPath()
{
    return dbpath;
}
