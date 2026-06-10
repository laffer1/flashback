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

#include <strings.h>
#include "fbClient.h"

/**
 * fbClient implementation
 * @author Lucas Holt
 * @date March 8, 2008
 */


/**
*	fbClient
*	Default client  constructor
*	@note Initilize all member vars
*/
fbClient::fbClient(fbData* _data, int sock ):data(_data), host(NULL), path(NULL)
{
    data->debug(NONE, "fbClient.this");
    if ( sock == ETCPACCEPTFAIL )
    { 
        data->debug(NONE, "fbClient.this Invalid client socket descriptor.");
        clientfp = NULL; // we're screwed.
    } 
    else
    {
        if ( ( clientfp = fdopen( sock, "r+" ) ) == NULL )
        {
            /* log/handle */
            data->debug(NONE, "fbClient.this  Could not convert file descriptor to FILE *");
        }
    }
}

/**
*	fbClient
*	Default client destructor
*/
fbClient::~fbClient()
{
    data->debug(NONE, "fbClient.~this");

    if (clientfp != NULL) {
        close();
    }

    if ( host != NULL ) {
        delete host;
    }

    if ( path != NULL ) {
        delete path;
    }
}


/**
*	parseHeaders
*	Read the HTTP headers
*/
void fbClient::parseHeaders()
{
    char *reqstr = NULL;  // the request as a string (uri)
    char *tmp = NULL;    // a working variable to "hack" out the space seperated string
    char *tmp2 = NULL;  // a copy of the request to work on

    reqstr = (char *)calloc( MAX_REQUEST,  sizeof(char));
    httptype = NOTSUPPORTED;
    contentLength = 0;

    tmp2 = reqstr;

    // To grab the first line  GET / HTTP/1.0 ... etc
    char * firstLinePtr = fgets( reqstr, MAX_REQUEST, clientfp );
    if (firstLinePtr == NULL) {
	httptype = NOTSUPPORTED;
	return;
    }

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
             // in theory this could be set but it's not in practice with HTTP 1.0
         }
         else if ( begins_with( tmp, "https://" ) == 1 )
         {
             // in theory this could be set but not in http 1.0
         }
         else // relative url
         {
              path = new string(tmp);
              host = new string("*"); // host was not defined here, probably a host header
         }
    }   

    // TODO: figure out HTTP version.  Not important for now.

    /* Drain the remaining request headers (previously left unread, which could
     * deadlock writes while the client was still sending). We retain the headers
     * needed for authentication (Authorization) and CSRF defense (Host, Origin,
     * Referer) plus Content-Length so a POST body can be read. */
    {
        char hdr[MAX_REQUEST];
        int guard = 0;
        while (guard++ < 200 && fgets(hdr, MAX_REQUEST, clientfp) != NULL)
        {
            // a blank line (just CR/LF) terminates the header section
            if (hdr[0] == '\r' || hdr[0] == '\n')
                break;

            char *colon = strchr(hdr, ':');
            if (colon == NULL)
                continue;
            size_t namelen = (size_t)(colon - hdr);

            char *val = colon + 1;
            while (*val == ' ' || *val == '\t')
                val++;
            size_t vl = strlen(val);
            while (vl > 0 && (val[vl-1] == '\r' || val[vl-1] == '\n' ||
                              val[vl-1] == ' '  || val[vl-1] == '\t'))
                val[--vl] = '\0';

            if (namelen == 13 && strncasecmp(hdr, "authorization", 13) == 0)
                authorization = val;
            else if (namelen == 4 && strncasecmp(hdr, "host", 4) == 0)
                hostHeader = val;
            else if (namelen == 6 && strncasecmp(hdr, "origin", 6) == 0)
                origin = val;
            else if (namelen == 7 && strncasecmp(hdr, "referer", 7) == 0)
                referer = val;
            else if (namelen == 14 && strncasecmp(hdr, "content-length", 14) == 0)
                contentLength = atol(val);
        }
    }

    /* Read the request body for POST submissions (bounded to avoid abuse). */
    if (httptype == POST && contentLength > 0 && contentLength <= 65536)
    {
        vector<char> buf((size_t)contentLength);
        size_t got = fread(buf.data(), 1, (size_t)contentLength, clientfp);
        body.assign(buf.data(), got);
    }

    free(reqstr);
}


/**
*	begins_with
*      Does a string start with another string (aka a substring at the beginning)
*/
int fbClient::begins_with( const char * str1, const char * str2 )
{
    size_t str2len;  // the length of the second string
    size_t str1len;  // the length of the first string
    unsigned int i;  // result of comparison

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


/**
*	getHost
*	the requesting host
*	@note call free on this value
*/
char * fbClient::getHost()
{
    if (host == NULL)
        return NULL;
    return strdup(host->c_str());
}


/**
*	getPath
*       the path of the request (after the host)
*	@note call free on the return value
*/
char * fbClient::getPath()
{
    if (path == NULL)
        return NULL;
    return strdup(path->c_str());
}


/**
*	getAuthorization
*	the value of the Authorization request header, or "" if none was sent
*/
const string& fbClient::getAuthorization()
{
    return authorization;
}


/**
*	header / body / method accessors
*/
const string& fbClient::getHostHeader() { return hostHeader; }
const string& fbClient::getOrigin()     { return origin; }
const string& fbClient::getReferer()    { return referer; }
const string& fbClient::getBody()       { return body; }
enum HTTP_TYPE fbClient::getType()      { return httptype; }


/**
*	write
*	write a string to client
*/
void fbClient::write( string val )
{
    fprintf( clientfp, "%s", val.c_str() );
    if (ferror( clientfp) )
       data->warn( NONE, "fbClient.write() Error writing on socket" ); 
}

/**
*	write
*	write a single character to client
*/
void fbClient::write( int c )
{
    fputc( c, clientfp );
    if (ferror( clientfp) )
       data->warn( NONE, "fbClient.write() Error writing on socket" );

}
 
/**
*	write
*	write a string to client using va_list input
*/
void fbClient::write(const char* str, ...)
{
    va_list list;  

    va_start(list, str);  //build the list
    vfprintf( clientfp, str, list);
    va_end(list);

    if (ferror( clientfp) )
       data->warn( NONE, "fbClient.write() (va_list) Error writing on socket" );

}

/**
*	close
*       close the file descriptor/client connection
*/
void fbClient::close()
{
    fclose( clientfp );
}

