/* $Id: fbHttpResponse.cpp,v 1.2 2008/03/21 02:36:59 laffer1 Exp $ */
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

#include "fbHttpServer.h"
#include "fbSocket.h"

fbHttpServer:: fbHttpResponse(fbData* _data):fbThread(_data), data(_data),  running(false)
{
}

fbHttpServer::~fbHttpResponse()
{
    if (running)
        shutdown();
}

void fbHttpResponse::startup()
{
    if (running)
        return;

    start();
}

void fbHttpResponse::shutdown()
{
    if (!running)
        return;

    stop();
    running = false; // stop it gracefully
}

void fbHttpResponse::run()
{
   // we should probably check this during the request  while(!isStopping())
   char *path;
   path = client->getPath();

   if ( strcmp(path, "/") == 0 )
      index();
  else if ( strcmp(path, "/index") == 0 )
      index();
  else
      notfound();

   shutdown();  // clean up 
}

void fbHttpResponse::index()
{

}

void fbHttpResponse::notfound()
{

}

