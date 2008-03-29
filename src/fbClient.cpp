/* $Id: fbClient.cpp,v 1.18 2008/03/29 22:19:29 wyverex Exp $ */

/*-
 * Copyright (C) 2008 Lucas Holt. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "fbClient.h"

/**
 * fbClient implementation
 * @author Lucas Holt
 * @date March 8, 2008
 */

fbClient::fbClient(fbData* _data, int sock ):data(_data)
{
    data->debug(NONE, "fbClient.this");
    if ( sock == ETCPACCEPTFAIL )
    { 
       /* handle this ? */
	   //data->warn(ERRORCODE, "Message");
        data->debug(NONE, "fbClient.this Invalid client socket descriptor.");
        clientfp = NULL; // weŕe screwed.
    } 
    else
    {
        if ( ( clientfp = fdopen( sock, "a+" ) ) == NULL )
        {
            /* log/handle */
            data->debug(NONE, "fbClient.this  Could not convert file descriptor to FILE *");
        }
    }
}

fbClient::~fbClient()
{
    data->debug(NONE, "fbClient.~this");
    if (clientfp != NULL)
        close();

    if ( host != NULL );
        delete host;;
    if ( path != NULL );
        delete path;
}

void fbClient::parseHeaders()
{
    char *reqstr = NULL;
    char *tmp = NULL;
    char *tmp2 = NULL;

    reqstr = (char *)calloc( MAX_REQUEST,  sizeof(char));
    
    tmp2 = reqstr;

    // To grab the first line  GET / HTTP/1.0 ... etc
    fgets( reqstr, MAX_REQUEST, clientfp ); 

    tmp = strsep( &tmp2, " \t" );  // SP in HTTP spec

    if ( tmp != NULL )
    {
        if ( strcmp( tmp, "GET" ) == 0 ) 
            httptype = GET;
        else if ( strcmp( tmp, "POST" ) == 0 )
           httptype = POST;
        else if ( strcmp( tmp, "HEAD" ) == 0 )
           httptype = HEAD;
        else
           httptype = NOTSUPPORTED;
    }

    // url
    tmp = strsep( &tmp2, " \t" );

    if ( tmp != NULL )
    {
         if ( begins_with( tmp, "http://" ) == 1 )
         {
          
         }
         else if ( begins_with( tmp, "https://" ) == 1 )
         {

         }
         else // relative url
         {
              path = new string(tmp);
              host = new string("*"); // host was not defined here, probably a host header
         }
    }   

    // TODO: figure out HTTP version.  Not important for now.
    free(reqstr);
}


int fbClient::begins_with( const char * str1, const char * str2 )
{
    size_t str2len;
    size_t str1len;
    unsigned int i;

    str2len = strlen( str2 );
    str1len = strlen( str1 );

    if ( str1len < str2len )
        return -1;  // error
    else if ( str1len == 0 )
        return -1; // error
    else
        for ( i = 0; i < str2len; i++ )
            if ( str1[i] != str2[i] )
                return 0; // doesn't match

    return 1; // matches
}


char * fbClient::getHost()
{
    return strdup(host->c_str());
}

char * fbClient::getPath()
{
    return strdup(path->c_str());
}

void fbClient::write( string val )
{
    fprintf( clientfp, "%s", val.c_str() );
    if (ferror( clientfp) )
       data->warn( NONE, "fbClient.write() Error writing on socket" ); 
}

void fbClient::write( int c )
{
    fputc( c, clientfp );
    if (ferror( clientfp) )
       data->warn( NONE, "fbClient.write() Error writing on socket" );

}
 

void fbClient::close()
{
    fclose( clientfp );
}

