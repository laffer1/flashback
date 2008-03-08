/* $Id: fbClient.h,v 1.2 2008/03/08 06:15:55 laffer1 Exp $ */

#ifndef FBCLIENT_H
#define FBCLIENT_H

#include "global.h"
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
    fbClient( /*const fbErrorLogger &err, */ int sock );   /// Default Constructor
    ~fbClient();  /// Destructor

protected:
    //fbErrorLogger log;
   FILE *clientfp;
};

#endif
