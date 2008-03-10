/* $Id: fbClient.h,v 1.3 2008/03/10 17:48:57 laffer1 Exp $ */

#ifndef FBCLIENT_H
#define FBCLIENT_H

#include "global.h"
#include "fbErrorLogger.h"
#include "sockets/tcp.h"

/**
 * fbClient
 * Client connted via socket for use with HTTP.
* Stores HTTP Request and state of this www user.
 * @author Lucas Holt
 * @date March 8, 2008
 */
class fbClient
{
public:
    fbClient( fbErrorLogger &err, int sock );   /// Default Constructor
    ~fbClient();  /// Destructor

protected:
   fbErrorLogger *log;
   FILE *clientfp;
};

#endif
