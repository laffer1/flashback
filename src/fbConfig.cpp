#include "fbConfig.h"

fbConfig::fbConfig(const fbErrorLogger &err)
{
    loadDefaults(); // assume defaults
    if (load() == -1)
    {
        // INFO: can't load settings from default config file, using defaults
        dirty = true;
    }
    else
        dirty = false;
}

fbConfig::fbConfig(const fbErrorLogger &err, const char* filename)
{
    loadDefaults(); // assume defaults
    if (load(filename) == -1)
    {
        // INFO: can't load settings from explicit config file, using defaults
    }
    dirty = true; // save to default config file regardless
}

fbConfig::~fbConfig()
{
    /* we can try to save here, but don't want to rely on it */
    /* also, should we save to the default file or to the one that was opened? */
    if (dirty) save();
    delete [] addr;
    delete [] webroot;
    delete [] dbpath;
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

int fbConfig::load(const char *filename)
{
    uint32_t i = 0;
    string s;
    ifstream myfile;
    myfile.open(filename, ios::in);
    if (myfile.fail())
    {
        // ERROR: can't open file
        return -1; // error
    }
    while (!myfile.eof() && !myfile.fail())
    {
        getline(myfile, s, '\n');
        if (myfile.fail()) break;
        if ((i = s.find("WebServerAddr=",0)) != string::npos)
        {
            setWebServerAddr(s.substr(i+14).c_str());
        }
        else if ((i = s.find("WebServerPort=",0)) != string::npos)
        {
            setWebServerPort(s.substr(i+14).c_str());
        }
        else if ((i = s.find("WebServerRootPath=",0)) != string::npos)
        {
            setWebServerRootPath(s.substr(i+18).c_str());
        }
        else if ((i = s.find("DBPath=",0)) != string::npos)
        {
            setDBPath(s.substr(i+7).c_str());
        }
        else
        {
            /* WARNING: config file contains unrecognized line */
        }
    }

    if (myfile.fail() && !myfile.eof())
    {
        myfile.close();
        // ERROR: error loading from config file
        return -1;
    }

    myfile.close();
    // INFO: successfully loaded some settings. remaining settings from defaults
    return 0;
}

int fbConfig::save()
{
    int i = 0;
    if ((i = save(FBCONFIG_DEFAULT_CONFIGFILE)))
        dirty = false;
    return i;
}

int fbConfig::save(const char *filename)
{
    ofstream myfile;
    myfile.open(filename, ios::out);
    if (myfile.fail())
    {
        // ERROR: error opening config file to save
        return -1;
    }
    myfile << "WebServerAddr=" << addr << endl
        << "WebServerPort=" << port << endl
        << "WebServerRootPath=" << webroot << endl
        << "DBPath=" << dbpath << endl;
    if (myfile.fail())
    {
        // ERROR: problem writing to config file
        myfile.close();
        return -1;
    }
    myfile.close();
    return 0;
}

void fbConfig::setWebServerAddr(const char* strAddr)
{
    dirty = true;
    if (addr != NULL) delete [] addr;
    addr = new char[strlen(strAddr)+1];
    strcpy(addr,strAddr);
}

void fbConfig::setWebServerPort(const char* strPort)
{
    dirty = true;
    port = atoi(strPort); // may need checking for validity
}

void fbConfig::setWebServerPort(int intPort)
{
    dirty = true;
    port = intPort; // may need checking for validity
}

void fbConfig::setWebServerRootPath(const char* strPath)
{
    dirty = true;
    if (webroot != NULL) delete [] webroot;
    webroot = new char[strlen(strPath)+1];
    strcpy(webroot,strPath);
}

void fbConfig::setDBPath(const char* strPath)
{
    dirty = true;
    if (dbpath != NULL) delete [] dbpath;
    dbpath = new char[strlen(strPath)+1];
    strcpy(dbpath,strPath);
}

const char* fbConfig::getWebServerAddr()
{
    return addr;
}

const int fbConfig::getWebServerPort()
{
    return port;
}

const char* fbConfig::setWebServerRootPath()
{
    return webroot;
}

const char* fbConfig::getDBPath()
{
    return dbpath;
}
