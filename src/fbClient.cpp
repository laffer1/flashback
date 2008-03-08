/* $Id: fbClient.cpp,v 1.1 2008/03/08 06:15:55 laffer1 Exp $ */

#include "fbClient.h"

/**
 * fbClient implementation
 * @author Lucas Holt
 * @date March 8, 2008
 */

fbClient::fbClient( int sock )
{
    if ( sock == ETCPACCEPTFAIL )
    { 
        /* log this */
       /* handle this ? */
   }

    if ( ( clientfp == fdopen( sock, "r+" ) ) == NULL )
    {
        /* log/handle */
    }
}

fbClient::~fbClient()
{
    fclose(clientfp);
}

