/* $Id: fbHttpResponse.h,v 1.4 2008/03/21 03:23:32 laffer1 Exp $ */
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

#ifndef FBHTTPRESPONSE_H
#define FBHTTPRESPONSE_H

#include "fbClient.h"
#include "fbThread.h"

#define SERVERID "FlashBack/1.0"

using namespace std;

/**
 * fbHttpResponse
 * Generate and transmit the webserver response to the client.
 * @author Lucas Holt
 * @date March 20, 2008
 */
class fbHttpResponse: public fbThread
{
public:
    fbHttpResponse(fbData* _data, fbClient * _client);   /// default constructor
    ~fbHttpResponse();  /// default destructor

    void startup();  /// Start HTTP server
    void shutdown();  /// Stop HTTP server

protected:
    fbData *data;
    fbClient *client;
    bool running;  /// is the response thread running?

    void run();     /// heavy lifting
    void index();  /// print index page for website.
    void notfound();
    void header( string name, string value );
    void status( string code, string msg );
};

#endif
