/* $Id: fbClient.cpp,v 1.3 2008/03/10 17:48:57 laffer1 Exp $ */

#include "fbClient.h"

/**
 * fbClient implementation
 * @author Lucas Holt
 * @date March 8, 2008
 */

fbClient::fbClient( fbErrorLogger &err, int sock )
{
    log = &err;

    if ( sock == ETCPACCEPTFAIL )
    { 
        /* log this */
       /* handle this ? */
   }

    if ( ( clientfp = fdopen( sock, "r+" ) ) == NULL )
    {
        /* log/handle */
    }
}

fbClient::~fbClient()
{
    fclose(clientfp);
}

