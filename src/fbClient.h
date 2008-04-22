/* $Id: fbClient.h,v 1.16 2008/04/22 17:23:10 wyverex Exp $ */
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

/*
*	Added client write va_list version...
*	-Byron
*/

#ifndef FBCLIENT_H
#define FBCLIENT_H

#include "global.h"
#include "fbData.h"
#include "sockets/tcp.h"

enum HTTP_VERSION 
{
  HTTP_09, 
  HTTP_10, 
  HTTP_11
};  /// The HTTP Version must be either 0.9, 1.0 or 1.1

enum HTTP_TYPE 
{
    GET,
    POST,
    HEAD,
    NOTSUPPORTED
/* TODO: add the rest */
};  /// We're only supporting GET, POST and possibly HEAD

enum HTTP_PROTOCOL 
{
    HTTP,
    HTTPS
};  /// Encrypted SSL traffic or regular HTTP speak  

#define MAX_REQUEST 2048

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
    fbClient(fbData* _data, int sock );   /// Default Constructor
    ~fbClient();                                     /// Destructor

    void parseHeaders();          /// Parse the HTTP headers on the client connection.
    int begins_with( const char * str1, const char * str2 );  /// determine if string starts with something

    char * getPath();                             /// get the client path (like what's after the server address)
    char * getHost();                             /// hostname it sent
    void write( string val );                    /// write a string to the client
    void write( int c );                            /// write a character to the client
    void write( const char* str, ... );		    /// write string to client, using vfprintf..
    void close();                                    /// terminate the client connection.

protected:
     fbData* data;                                 /// the reference to data
     FILE *clientfp;                                /// a file pointer on top of the socket with client
    /* HTTP request stuff */
    enum HTTP_VERSION httpver;     /// http version
    enum HTTP_TYPE httptype;          /// request type like GET or POST
    enum HTTP_PROTOCOL httpproto;  /// ssl or "regular"
    string *host;                                  /// the server host of the request (if we did named hosting)
    string *path;                                  /// the URL (NOT URI)
};

#endif

