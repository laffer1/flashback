/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An library to abstract the C sockets interface from the programmer.  This
file describes function prototypes for the TCP protocol implementation.  
*/

/*-
 * Copyright (C) 2005, 2008 Lucas Holt. All rights reserved.
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

#ifndef TCPH
#define TCPH

/* assumes C++ bool type */

/* library errors */
#define ETCPACCEPTFAIL -7
#define ETCPLISTENFAIL -6
#define ETCPBINDFAIL   -5
#define ETCPCONNFAIL   -4
#define ETCPBADADDRESS -3
#define ETCPMAXCONS    -2
#define ETCPGENERIC    -1
/* end errors */

typedef int socketdesc; /* similar to a file descriptor.. an index for
                           created connections */

void inittcp( void );
socketdesc opentcp( bool server, char * address, int port );
void closetcp( socketdesc sd );
char * readtcp( socketdesc sd );
char * readntcp( socketdesc sd, int size );
int tcpserverclient( socketdesc sd );
int tcpclientfd( socketdesc sd );
int writetcp( socketdesc sd, char * str );
int writentcp( socketdesc sd, char * str, int len );
int tcpputs( socketdesc sd, char * str );

#endif
