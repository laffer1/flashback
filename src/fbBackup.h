/* $Id: fbBackup.h,v 1.1 2008/03/29 23:33:07 wyverex Exp $ */

#ifndef fbBACKUP_H
#define fbBACKUP_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"

//uses system("tar -cf %to %from");
//gross but dont have libarchive yet!

class fbBackup: public fbThread
{
public:
	fbBackup(fbData* _data, string& src, string& dest);
	~fbBackup();

private:
	fbData* data;
	string from;
	string to;

	void run();

};

#endif

