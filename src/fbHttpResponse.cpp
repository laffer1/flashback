/* $Id: fbHttpResponse.cpp,v 1.47 2008/04/22 17:23:10 wyverex Exp $ */
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <stdlib.h>

#include "fbHttpServer.h"
#include "fbHttpResponse.h"
#include "fbSocket.h"

#define MIMECOUNT 12

static const char *  mime[][2] = { 
    { ".html", "text/html" },
    { ".htm", "text/htm" },
    { ".png", "image/png" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".gif", "image/gif" },
    { ".txt", "text/plain" },
    { ".css", "text/css" },
    { ".xml", "text/xml" },
    { ".xsl", "text/xml" },
    { ".xslt", "text/xml" },
    { ".tar", "multipart/x-tar" },
    { NULL, NULL }
};


/**
*	fbHttpResponse
*	Default Server response to client constructor
*	@note Initilize all member vars
*/
fbHttpResponse:: fbHttpResponse(fbData * _data, fbClient * _client): fbThread(_data), data(_data),   client(_client), running(false)
{
    data->debug(NONE, "fbHttpResponse.this");
}


/**
*	fbHttpResponse
*	Default server response to client destructor
*	@note This object deletes itself as part of the intereaction with fbThread
*/
fbHttpResponse::~fbHttpResponse()
{
    if (running)
        shutdown();

    data->debug(NONE, "fbHttpResponse.~this");
}


/**
*	startup
*	Used to begin execution of the run loop on the thread.
*/
void fbHttpResponse::startup()
{
    data->debug(NONE, "fbHttpResponse.startup");

    if (running)
        return;

    running = true;
    startDelete();
}


/**
*	shutdown
*	Prepares for an exit from the main run loop.  Occurs at client
*      tear down.
*/
void fbHttpResponse::shutdown()
{
    if (!running) return;

    data->debug(NONE, "fbHttpResponse.shutdown");
    running = false; // stop it gracefully
}


/**
*	run
*	The main execution point.  This is where the magic happens.  Client
*      receives HTML and HTTP response from this run.
*/
void fbHttpResponse::run()
{
    char *path;                     // the virtual path 
    char *querystring;          // the ? part of the uri if it exists
    char *loc;                       // the location of a query string if any
    size_t pathlen;               // length of path
    char **ap, *argv[1024];  // an argument array generated from the querystring
    char *firstvar;               // the first variable in the query string
    char *secondvar;          // the second var in the query string

    // no path and we have a big problem.
    if ( (path = client->getPath()) == NULL)
    {
        internal();
        goto CLEANUP;
    }

    data->debug(NONE, "fbHttpResponse.run");
    data->msg(NONE, path); // log webserver request

    /* deal with / and /index it should access our default index.html */
    if ( strcmp(path, "/") == 0 || strcmp( path, "/index" ) == 0 ) 
    {
        free(path);
        path = strdup("/index.html");
    }

    data->debug(NONE, "Test for querystring");
    // Is this a built in command.. ? for forms
    if ( ( loc = strstr( path, "?" ) ) != NULL )
    {
        pathlen = strlen(path);
        querystring = (char *) malloc(pathlen + 1); // it's actually smaller than that.
        if (querystring == NULL)
        {
            data->err(NONE,"Could not allocate memory");
            internal();
            goto CLEANUP;
        }
        data->debug(NONE, "Copy the query");
        strncpy( querystring, loc, pathlen );
        if ( strlen(querystring) < pathlen )
        {
           // break it up into an argument vector.
	   for (ap = argv; (*ap = strsep(&querystring, "&")) != NULL;)
                   if (**ap != '\0')
                           if (++ap >= &argv[1024])
                                   break;

           data->debug(NONE, "query in argv array");
           loc[0] = '\0'; // whack the ?
           // it's path testing time
           if ( strcmp( path, "/current" ) == 0 )
           {
               dynamichead("FlashBack :: Current Jobs");
               client->write("<div id=\"container\">\n");
               client->write("<h2>Current Jobs</h2>\n");
               client->write("<pre>");
               string  bl = data->db->getBackupList();
               client->write(bl);
               //delete bl;
               client->write("</pre>");
               client->write("</div>\n");
               dynamicfoot();
           }
           else if ( strcmp( path, "/schedule" ) == 0 )
           {
               dynamichead("FlashBack :: Schedule Jobs");
               client->write("<div id=\"container\">\n");
               client->write("<h2>Schedule Jobs</h2>\n");
               if (argv[0] != NULL)
              {
                   if ( strcmp( argv[0], "?show" ) == 0 )
                   {
                       client->write("<form method=\"get\" >\n");
                       client->write("<fieldset>\n<p>Name: <input type=\"text\" name=\"name\" value=\"\" />\n");
                       client->write("<br />Path: <input type=\"text\" name=\"path\" value=\"\" />\n");
                       client->write("</p></fieldset><p><input type=\"submit\" name=\"submit\" value=\"submit\" /></p>");
                       client->write("</form>\n");
                   } 
                   else 
                   {
                       if ( argv[1] != NULL)
                      {
                          firstvar = (char *) calloc(strlen(argv[0]) +1, sizeof(char));
                          strcpy( firstvar, argv[0] );
                          secondvar = (char *) calloc(strlen(argv[1]) +1, sizeof(char));
                          strcpy( secondvar, argv[1] );

                          // hack out the variable name and = so we can get to the values.
                          strtok( firstvar, "=" );
                          firstvar = strtok( NULL, "=" );
                          strtok( secondvar, "=" );
                          secondvar = strtok( NULL, "=" );

                          sanitizestr( firstvar );
                          sanitizestr( secondvar );
                          client->write(firstvar);
                          client->write("<br />\n");
                          client->write(secondvar);
                          client->write("<br />\n");
                          data->msg( NONE, "Scheduling job %s on %s", firstvar, secondvar );
                          // perform the backup.  firstvar is our name and secondvar is the path to backup
                          data->addBackupJob(new string(firstvar), new fbDate, new fbTime, new string(secondvar));
                          //free(firstvar);
                          //free(secondvar);
                      }
                      else
                      {
                          client->write("Bad parameters"); 
                     }
                 }
              }
              client->write("</div>\n");
              dynamicfoot();
           }
           else if ( strcmp( path, "/restore" ) == 0 )
           {
               dynamichead("FlashBack :: Restore from Backup");
               client->write("<div id=\"container\">\n");
               client->write("<h2>Restore from Backup</h2>\n");
               if (argv[0] != NULL)
               {
                   if ( strcmp( argv[0], "?show" ) == 0 )
                   {
                       client->write("<form method=\"get\" >\n<fieldset>\n");
                       //client->write("<p>File: <input type=\"text\" name=\"file\" value=\"\" />\n");
                       //client->write("<br />Extract to: <input type=\"text\" name=\"path\" value=\"\" />\n");
                       //client->write("</p></fieldset><p><input type=\"submit\" name=\"submit\" value=\"submit\" /></p>");
                       //client->write("</form>\n");
			client->write("Extract to: <input type=\"text\" name=\"path\" value=\"\" />\n");
			client->write("</fieldset>\n");
			
			bool ret;
			string desc, path, tarfile, d, t;
			fbDate date;
			fbTime time;
			int id;

			data->queryRepo();
			client->write("<table width=780px>\n");

			client->write("\t<tr>\n");
			client->write("\t\t<td>\n\t\t\t ID \n\t\t</td>\n");
			client->write("\t\t<td width=25%>\n\t\t\t Name \n\t\t</td>\n");
			client->write("\t\t<td width=25%>\n\t\t\t Path \n\t\t</td>\n");
			client->write("\t\t<td>\n\t\t\t Date \n\t\t</td>\n");
			client->write("\t\t<td>\n\t\t\t Time \n\t\t</td>\n");
			client->write("\t\t<td>\n\t\t\t Restore \n\t\t</td>\n");
			client->write("\t</tr>\n");

			do
			{
				ret = data->db->getRepoRow(desc, date, time, path, tarfile, &id);
				if(ret)
				{
					client->write("\t<tr>\n");

					client->write("\t\t<td>\n\t\t\t %d \n\t\t</td>\n", id);
					client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", desc.c_str());
					client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", path.c_str());

					d = "";
					date.mdy(d);
					t = "";
					time.hms(t);

					client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", d.c_str());
					client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", t.c_str());

					client->write("\t\t<td>\n\t\t\t <input type=\"submit\" name=\"file\" value=\"%s\" /> \n\t\t</td>\n", tarfile.c_str());
					client->write("\t</tr>\n");
				}

			}while(ret);
			client->write("</table>\n</form>\n");


                   } 
                   else 
                   {
                       if ( argv[1] != NULL)
                      {
			  //Had to switch first var and second var... hope this works! -B

                          firstvar = (char *) calloc(strlen(argv[0]) +1, sizeof(char));
                          strcpy( firstvar, argv[0] );
                          secondvar = (char *) calloc(strlen(argv[1]) +1, sizeof(char));
                          strcpy( secondvar, argv[1] );



                          // hack out the variable name and = so we can get to the values.
                          strtok( firstvar, "=" );
                          firstvar = strtok( NULL, "=" );
                          strtok( secondvar, "=" );
                          secondvar = strtok( NULL, "=" );

                          sanitizestr( firstvar );
                          sanitizestr( secondvar );
          
                          client->write(secondvar);
                          client->write("<br />\n");
     	                  client->write(firstvar);
                          client->write("<br />\n");

                          data->msg( NONE, "Restore %s to %s", secondvar, firstvar);
                          // firstvar is our file name and secondvar is the path to restore to
                          data->addRestoreJob( new string(secondvar), new string(firstvar) );

                          //free(firstvar);
                          //free(secondvar);
                      }
                      else
                      {
                          client->write("Bad parameters"); 
                     }
                 }
              }
               client->write("</div>\n");
               dynamicfoot();
           }
           else if ( strcmp( path, "/settings" ) == 0 )
           {
               dynamichead("FlashBack :: Settings");
                client->write("<div id=\"container\">\n");
               client->write("<h2>Settings</h2>\n");
               client->write("</div>\n");
               dynamicfoot();
           }
           free(querystring);
        }
        else // can't be valid
           internal();
    }
    else  // Must be a file on the file system!
        sendfile(path);

CLEANUP:
    data->debug(NONE, "fbHttpResponse.run() free path memory");
    // we're mallocing this elsewhere.
    if ( path != NULL )
       free( path );

    data->debug(NONE, "fbHttpResponse.run() delete client");
    delete client;

    shutdown();  // clean up 
}


/**
*	sanitizestr
*	Convert special encodings into real characters.
*      + becomes a space, etc.
*	@note Modifies input string
*/
void fbHttpResponse::sanitizestr( char * str )
{
    size_t len;           // length of input string
    size_t resultlen;   // length of output string
    char *result;        // the result

    if ( str == NULL )
       return;

    len = strlen( str );

    for (unsigned int i = 0; i < len; i++ )
    {
        if ( str[i] == '+')
           str[i] = ' ';
    }

    resultlen = len;
    result = spc_decode_url(str, &resultlen);  
    strncpy( str, result, len );
}


/**
*	dynamicfoot
*	print the HTML footer on dynamic urls (? types)
*/
void fbHttpResponse::dynamicfoot()
{
     client->write("<div class=\"clear\"></div>\n");
     client->write("<hr />\n");
     client->write("<div id=\"footer\">");
     client->write("&#169; 2008 &#8211; Lucas Holt, Byron Heads, Chris Tubbs, and John Markus");
     client->write("</div>\n");
     client->write("</body>\n");
     client->write("</html>\n");
}


/**
*	dynamichead
*	Print the HTTP and HTML header on the dynamic responses
*      from the webserver.
*	@note Gets us to the body of the HTML document.
*/
void fbHttpResponse::dynamichead( const char * title )
{
    data->debug(NONE, "fbHttpResponse.dyamichead");
    status( "200", "OK" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html" );
    header( "Content-Language", "en-US" );
    client->write("\r\n"); // end header section

    client->write("<html>\n<head>\n\t<title>");
    if (title != NULL)
        client->write(title);
    client->write("</title>\n");
    client->write("\t<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">\n");
    client->write("\t<link rel=\"stylesheet\" type=\"text/css\" href=\"buttons.css\">\n");
    client->write("\t<link rel=\"stylesheet\" type=\"text/css\" href=\"forms.css\">\n");
    client->write("<style type=\"text/css\">\n");
    client->write("#goodies-bar { width: 780px; height: auto; margin: 1em auto; padding: 1em;}\n");
    client->write("#thebar { padding: 0px; width: 780px; margin: -10px auto 0 auto; border: 1px dotted #e3ebf1; background: #e3ebf1; text-align: center; float: left; -moz-border-radius: 7px; -webkit-border-radius: 7px; }\n");
    client->write("#thebar a { padding: 32px 0 0 0; background-repeat: no-repeat; overflow: hidden; height: 0px !important; height: /**/:32px; display: block; float: left; margin: 10px; }\n");
    client->write("#current a { background-image: url(\"current32.png\"); width: 141px; }\n");
    client->write("#schedule a { background-image: url(\"schedule32.png\"); width: 153px; }\n");
    client->write("#restore a { background-image: url(\"restore32.png\"); width: 137px; }\n");
    client->write("#settings a { background-image: url(\"settings32.png\"); width: 143px; }\n");
    client->write("#help a { background-image: url(\"help32.png\"); width: 93px; }\n");
    client->write("</style>\n");
    client->write("</head>\n");
    client->write("<body>\n");
    client->write("	<div id=\"header\">\n");
    client->write("		<h1><a href=\"index.html\" title=\"FlashBack Homepage\">FlashBack: Data Backup Solution</a></h1>\n");
    client->write("	</div>\n");
    client->write("	<div id=\"goodies-bar\">\n");
    client->write("		<div id=\"thebar\">\n");
    client->write("			<div id=\"current\"><a href=\"/current?show\">Current</a></div>\n");
    client->write("			<div id=\"schedule\"><a href=\"/schedule?show\">Schedule</a></div>\n");
    client->write("			<div id=\"restore\"><a href=\"/restore?show\">Restore</a></div>\n");
    client->write("			<div id=\"settings\"><a href=\"/settings?show\">Settings</a></div>\n");
    client->write("			<div id=\"help\"><a href=\"help.html\">Help</a></div>\n");
    client->write("		</div>\n");
    client->write("	</div>\n");
    client->write("	<div class=\"clear\"></div>\n");
}


/**
*	sendfile
*	Send a static file to the client. (html, css, image, etc)
*/
void fbHttpResponse::sendfile( const char * path )
{
    FILE *fp;           // the file to send to the client
    string realp = data->getWebServerRootPath();
    int c;                  // an individual character we're going to write to stream
    char *resolved;  // The path after it has been tested with realpath
    struct stat st;  // the information about a file from lstat call

    data->debug(NONE, "fbHttpResponse.sendfile");

    if (path != NULL)
        realp.append(path);
    else
    {
        notfound();
        return; // TODO: Error logging
    }

    resolved = (char *)calloc(PATH_MAX, sizeof(char));
    if (resolved == NULL)
    {
        internal();
        return;
    }
    realpath(realp.c_str(), resolved);

   /* The file is not there */
    if (!*resolved)
    {
        notfound();
        return;
    }

    /* Find out if it's a symlink */
    if ( lstat( resolved, &st ) == -1 )
   {
       /* since the system call failed, let's assume we can't read the file. */
       free(resolved);
       notfound();
       return;
   }
   else
   {
       if (S_ISLNK(st.st_mode) )
       { 
          // this means it's a symlink which we don't support.
         // TODO: permissions error instead?
         free(resolved);
          notfound();
          return;
       }
   }

    if ( (fp = fopen( resolved, "r" ) ) == NULL ) {
        //data->msg(NONE, "fbHttpResponse: Unable to open file");
        //data->msg(NONE, resolved);
        free(resolved);
	notfound();
        return;
    }

    status( "200", "OK" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", matchmimetype(resolved) );
    header( "Content-Language", "en-US" );
    client->write("\r\n"); // end header section

    while ( (c = fgetc(fp)) != EOF && !ferror(fp))
    {
        if (c != EOF)
            client->write(c);
    }

    fclose(fp);
    free(resolved);
    data->debug(NONE, "fbHttpResponse.sendfile() done writing file");
}


/**
*       notfound
*	Generate a 404 not found page for cases where the file does not exist that is requested.
*	@note Can be called from almost anywhere, but no HTTP message can be sent before the call.
*/
void fbHttpResponse::notfound()
{
    string r;         // response for client
    char *path;    // The path the client wanted.

    data->debug(NONE, "fbHttpServer.notfound");

    path = client->getPath();
    if ( path == NULL )
    {
        internal();
        return;
    }

    status( "404", "Not Found" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    // TODO: Expires header

    r.append("\r\n"); // extra to start response as required per spec
    r.append( "<html>\n<head>\n\t<title>404 Not Found</title>\n</head>\n");
    r.append("<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on the server.</p>\n");
    r.append("<p>Invalid path: ");
    r.append(path);
    r.append("</p>\n<hr><p>");
    r.append(SERVERID);
    r.append("</p>\n</body>\n</html>\n");

    client->write(r);

    if (path != NULL)
        free( path );
}


/**
*	internal
*	For cases where a 404 wont' do.  Seriously, this is when we screwed up bad.
*/
void fbHttpResponse::internal()
{
    data->debug(NONE, "fbHttpServer.internal");
    status( "500", "Internal Server Error" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    client->write("\r\n");;
}

/**
*	status
*	Send a status code like 404, 200 ok, etc.
*	@note Invalid for HTTP .9
*/
void fbHttpResponse::status( string code, string msg )
{
    string r;  // response to client

    r.append( "HTTP/1.0 " );
    r.append( code );
    r.append( " " );
    r.append( msg );
    r.append( "\r\n" );
    client->write(r);
}


/**
*	header
*	Generate an HTTP header from input
*	@note name and vlaue must be valid strings
*/
void fbHttpResponse::header( string name, string value )
{
    string r;  // response to client.

    r.append(name);
    r.append(": ");
    r.append(value);
    r.append("\r\n");
    client->write(r);
}


/**
*	headdate
*	Write a date header using HTTP 1.1 guidelines which are OK in HTTP 1.0
*/
void fbHttpResponse::headdate()
{
    struct tm *tm;  // formated time right now
    time_t now;     //  time right now
    char date[50];  // a string representation of the date in GMT
    string r;           // response to client

    now = time( 0 );
    tm = gmtime( &now ); /* HTTP 1.1 spec rev 06 sez GMT only */
    strftime( date, 50, "%a, %d %b %Y %H:%M:%S GMT", tm );
    r.append(date);
    header( "Date", r );
}


/**
*	matchmimetype
*	Guess at the mime type by the file extension.
*	@note not 100% accurate and dependant on a structure of types that must be updated.
*/
const char * fbHttpResponse::matchmimetype( const char *filename )
{
    size_t len;       // length of filename
    size_t extlen;  // lenghth of file extension

    if (filename == NULL)
        return "text/plain";

    len = strlen(filename);

    for ( int i = 0; i < MIMECOUNT; i++ )
    {
        extlen = strlen( mime[i][0] );
        if (strcasecmp( mime[i][0], 
                 filename + (len - extlen)) == 0)
            return mime[i][1];
    }

    return "text/plain";
}
