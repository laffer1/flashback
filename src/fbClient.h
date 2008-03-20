/* $Id: fbClient.h,v 1.6 2008/03/20 18:46:13 wyverex Exp $ */
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
    ~fbClient();  /// Destructor

    void parseHeaders();  /// Parse the HTTP headers on the client connection.
    int begins_with( char * str1, char * str2 );

protected:
   fbData* data;
   FILE *clientfp;
  /* HTTP request stuff */
  enum HTTP_VERSION httpver;
  enum HTTP_TYPE httptype;
  enum HTTP_PROTOCOL httpproto;
  string *host;
  string *path;;
};

#endif
