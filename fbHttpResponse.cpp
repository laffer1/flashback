/* $Id: fbHttpResponse.cpp,v 1.50 2011/06/21 12:39:36 laffer1 Exp $ */
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
#include "fbAuth.h"

#define MIMECOUNT 12

static string htmlEscape(const char *s)
{
    string out;
    for (; *s; s++) {
        switch (*s) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += "&quot;"; break;
            case '\'': out += "&#39;";  break;
            default:   out += *s;
        }
    }
    return out;
}

static const char * mime[][2] = {
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
    char *querystring;          // the ? part of the uri if it exists (modified by strsep)
    char *querystring_base = NULL; // original malloc'd pointer; strsep advances querystring
    char *loc;                       // the location of a query string if any
    size_t pathlen;               // length of path
    char **ap, *argv[1024];  // an argument array generated from the querystring
    char *firstvar;               // the first variable in the query string
    char *secondvar;          // the second var in the query string
    size_t arglen;

    // no path and we have a big problem.
    if ( (path = client->getPath()) == NULL)
    {
        internal();
        goto CLEANUP;
    }

    data->debug(NONE, "fbHttpResponse.run");
    /* Log the request path WITHOUT its query string, so that parameters such
     * as /settings?password=... never reach the log file. */
    {
        const char *q = strchr(path, '?');
        if (q != NULL)
        {
            string logpath(path, (size_t)(q - path));
            data->msg(NONE, "%s", logpath.c_str());
        }
        else
            data->msg(NONE, "%s", path);
    }

    /* Enforce the configured authentication policy before doing any work. */
    if (!authorized())
    {
        unauthorized();
        goto CLEANUP;
    }

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
        querystring_base = querystring; // strsep will advance querystring; keep original for free()
        data->debug(NONE, "Copy the query");
        strncpy( querystring, loc, pathlen );
	querystring[pathlen] = '\0';
        if ( strlen(querystring) < pathlen )
        {
           // break it up into an argument vector.
	   for (ap = argv; (*ap = strsep(&querystring, "&")) != NULL;)
                   if (**ap != '\0')
                           /* guard: stop before writing argv[1023] so loop body can't
                            * assign one past the array end at the top of the next iteration */
                           if (++ap >= &argv[1023])
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
                       fbDate *sdate = new fbDate();
                       fbTime *stime = new fbTime();

                       client->write("<form method=\"get\" >\n");
                       client->write("<fieldset>\n<p>Name: <input type=\"text\" name=\"name\" value=\"\" />\n");
                       client->write("<br />Path: <input type=\"text\" name=\"path\" value=\"\" />\n");
                       client->write("<br />Month/Day/Year: <input type=\"text\" size=\"2\" maxlength=\"2\" name=\"month\" value=\"");
                       char tmp[32];
                       snprintf(tmp, sizeof(tmp), "%d", sdate->getMonth());
                       client->write(tmp);
                       client->write("\" />/<input type=\"text\" size=\"2\" maxlength=\"2\" name=\"day\" value=\"\" />/<input type=\"text\" maxlength=\"4\" size=\"4\" name=\"year\" value=\"\" />\n");
                       client->write("<br />Time: <input type=\"text\" size=\"2\" maxlength=\"2\" name=\"hour\" value=\"\" />:<input size=\"2\" maxlength=\"2\" type=\"text\" name=\"min\" value=\"\" />\n");
                       client->write("</p></fieldset><p><input type=\"submit\" name=\"submit\" value=\"submit\" /></p>");
                       client->write("</form>\n");

                       delete sdate;
                       delete stime;
                   }
                   else
                   {
                      if ( argv[1] != NULL && argv[2] != NULL && argv[3] != NULL
                           && argv[4] != NULL && argv[5] != NULL && argv[6] != NULL)
                      {
                          /* Keep original calloc'd pointers so we can free them correctly.
                           * strtok returns interior pointers into these buffers — never free those. */
                          char *fv_buf, *sv_buf, *v3_buf, *v4_buf, *v5_buf, *v6_buf, *v7_buf;
                          char *fv_val, *sv_val, *v3_val, *v4_val, *v5_val, *v6_val, *v7_val;

                          // name
			  arglen = strlen(argv[0]);
                          fv_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( fv_buf, argv[0], arglen );
                          // path
			  arglen = strlen(argv[1]);
                          sv_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( sv_buf, argv[1], arglen );
                          // Month
			  arglen = strlen(argv[2]);
                          v3_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( v3_buf, argv[2], arglen );
                          // Day
			  arglen = strlen(argv[3]);
                          v4_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( v4_buf, argv[3], arglen );
                          // Year
			  arglen = strlen(argv[4]);
                          v5_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( v5_buf, argv[4], arglen );
                          // Hour
			  arglen = strlen(argv[5]);
                          v6_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( v6_buf, argv[5], arglen );
                          // Minute
			  arglen = strlen(argv[6]);
                          v7_buf = (char *) calloc(arglen +1, sizeof(char));
                          memcpy( v7_buf, argv[6], arglen );

                          // hack out the variable name and = so we can get to the values.
                          strtok( fv_buf, "=" );
                          fv_val = strtok( NULL, "=" );
                          strtok( sv_buf, "=" );
                          sv_val = strtok( NULL, "=" );
                          strtok( v3_buf, "=" );
                          v3_val = strtok( NULL, "=" );
                          strtok( v4_buf, "=" );
                          v4_val = strtok( NULL, "=" );
                          strtok( v5_buf, "=" );
                          v5_val = strtok( NULL, "=" );
                          strtok( v6_buf, "=" );
                          v6_val = strtok( NULL, "=" );
                          strtok( v7_buf, "=" );
                          v7_val = strtok( NULL, "=" );

                          if (fv_val == NULL || sv_val == NULL || v3_val == NULL ||
                              v4_val == NULL || v5_val == NULL || v6_val == NULL || v7_val == NULL)
                          {
                              client->write("Bad parameters");
                          }
                          else
                          {
                              sanitizestr( fv_val );
                              sanitizestr( sv_val );
                              sanitizestr( v3_val );
                              sanitizestr( v4_val );
                              sanitizestr( v5_val );
                              sanitizestr( v6_val );
                              sanitizestr( v7_val );

                              client->write(htmlEscape(fv_val).c_str());
                              client->write("<br />\n");
                              client->write(htmlEscape(sv_val).c_str());
                              client->write("<br />\n");
                              data->msg( NONE, "Scheduling job %s on %s", fv_val, sv_val );

                              fbDate *sdate = new fbDate ( // month, day, year
                                                        (int)strtol(v3_val, (char **)NULL, 10),
                                                        (int)strtol(v4_val, (char **)NULL, 10),
                                                        (int)strtol(v5_val, (char **)NULL, 10) );

                              fbTime *stime = new fbTime( // hour : min : second
                                                        (int)strtol(v6_val, (char **)NULL, 10),
                                                        (int)strtol(v7_val, (char **)NULL, 10),
                                                        0);

                              // addBackupJob takes ownership of sdate and stime and deletes them
                              data->addBackupJob(new string(fv_val), sdate, stime, new string(sv_val));
                          }

                          // free the original calloc'd buffers (not the interior strtok pointers)
                          free(fv_buf);
                          free(sv_buf);
                          free(v3_buf);
                          free(v4_buf);
                          free(v5_buf);
                          free(v6_buf);
                          free(v7_buf);
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
                       client->write("\t\t<td>\n\t\t\t Name \n\t\t</td>\n");
                       client->write("\t\t<td>\n\t\t\t Path \n\t\t</td>\n");
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
                               client->write("\t\t<td>\n\t\t\t " + htmlEscape(desc.c_str()) + " \n\t\t</td>\n");
                               client->write("\t\t<td>\n\t\t\t " + htmlEscape(path.c_str()) + " \n\t\t</td>\n");

                               d = "";
                               date.mdy(d);
                               t = "";
                               time.hms(t);

                               client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", d.c_str());
                               client->write("\t\t<td>\n\t\t\t %s \n\t\t</td>\n", t.c_str());

                               client->write("\t\t<td>\n\t\t\t <input type=\"submit\" name=\"file\" value=\"" + htmlEscape(tarfile.c_str()) + "\" /> \n\t\t</td>\n");
                               client->write("\t</tr>\n");
                           }

                       } while(ret);
                       client->write("</table>\n</form>\n");
                   }
                   else
                   {
                       if ( argv[1] != NULL)
                       {
                           /* Keep original calloc'd pointers for free(); strtok returns
                            * interior pointers — never free those. */
                           char *fv_buf = (char *) calloc(strlen(argv[0]) +1, sizeof(char));
                           memcpy( fv_buf, argv[0], strlen(argv[0]) );
                           char *sv_buf = (char *) calloc(strlen(argv[1]) +1, sizeof(char));
                           memcpy( sv_buf, argv[1], strlen(argv[1]) );

                           // hack out the variable name and = so we can get to the values.
                           strtok( fv_buf, "=" );
                           firstvar = strtok( NULL, "=" );
                           strtok( sv_buf, "=" );
                           secondvar = strtok( NULL, "=" );

                           if (firstvar == NULL || secondvar == NULL)
                           {
                               client->write("Bad parameters");
                           }
                           else
                           {
                               sanitizestr( firstvar );
                               sanitizestr( secondvar );

                               client->write(htmlEscape(secondvar).c_str());
                               client->write("<br />\n");
                               client->write(htmlEscape(firstvar).c_str());
                               client->write("<br />\n");

                               data->msg( NONE, "Restore %s to %s", secondvar, firstvar);
                               // firstvar is our file name and secondvar is the path to restore to
                               data->addRestoreJob( new string(secondvar), new string(firstvar) );
                           }

                           free(fv_buf);
                           free(sv_buf);
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
               settingspage(argv);
           }
           free(querystring_base);
        }
        else // can't be valid
        {
           free(querystring_base);
           internal();
        }
    }
    else  // Must be a file on the file system!
    {
        sendfile(path);
    }

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
*	authorized
*	Check the request against the configured authentication policy.
*	@return true if the request may proceed (auth disabled, or valid Basic creds)
*/
bool fbHttpResponse::authorized()
{
    string mode, user, salt, hash;
    int iters;

    data->db->getAuthConfig(mode, user, salt, hash, iters);

    if (mode != "basic")
        return true;   // authentication disabled (the default)

    const string& creds = client->getAuthorization();
    if (creds.empty())
        return false;

    string u, p;
    if (!fbAuthDecodeBasic(creds, u, p))
        return false;
    if (u != user)
        return false;
    if (hash.empty() || iters <= 0)
        return false;

    return fbAuthVerifyPassword(p, salt, hash, iters);
}


/**
*	unauthorized
*	Send a 401 challenge so the browser prompts for HTTP Basic credentials.
*/
void fbHttpResponse::unauthorized()
{
    data->debug(NONE, "fbHttpResponse.unauthorized");
    status( "401", "Unauthorized" );
    header( "Server", SERVERID );
    headdate();
    header( "WWW-Authenticate", "Basic realm=\"FlashBack\"" );
    header( "Connection", "close" );
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    client->write("\r\n");
    client->write("<html>\n<head>\n\t<title>401 Unauthorized</title>\n</head>\n");
    client->write("<body>\n<h1>401 Unauthorized</h1>\n<p>Authentication is required to use FlashBack.</p>\n</body>\n</html>\n");
}


/**
*	settingspage
*	Render the /settings page and process authentication-settings submissions.
*	The form uses GET; the request logger strips query strings so the password
*	is not written to the log. Passwords are stored hashed, never in plaintext.
*/
void fbHttpResponse::settingspage( char **argv )
{
    string mode, username, password;
    bool submitted = false;

    /* Parse the query parameters (each argv entry is "name=value"; the first
     * may carry a leading '?'). Values are URL-decoded via sanitizestr. */
    for (int i = 0; argv[i] != NULL; i++)
    {
        const char *p = argv[i];
        if (*p == '?') p++;
        const char *eq = strchr(p, '=');
        if (eq == NULL) continue;

        string name(p, (size_t)(eq - p));
        char *vbuf = strdup(eq + 1);
        if (vbuf == NULL) continue;
        sanitizestr(vbuf);
        string value(vbuf);
        free(vbuf);

        if (name == "mode") { mode = value; submitted = true; }
        else if (name == "username") username = value;
        else if (name == "password") password = value;
    }

    /* Current settings: used to pre-fill the form and to allow re-saving Basic
     * auth with a blank password (keeping the existing credential). */
    string curMode, curUser, curSalt, curHash;
    int curIter = 0;
    data->db->getAuthConfig(curMode, curUser, curSalt, curHash, curIter);

    string status_msg;

    if (submitted)
    {
        if (mode == "none")
        {
            status_msg = data->db->setAuthConfig("none", "", "", "", 0)
                       ? "Authentication disabled." : "Failed to save settings.";
            curMode = "none"; curUser = ""; curSalt = ""; curHash = ""; curIter = 0;
        }
        else if (mode == "basic")
        {
            if (!password.empty())
            {
                if (username.empty())
                    status_msg = "A username is required to enable Basic authentication.";
                else
                {
                    string salt, hash;
                    if (fbAuthHashPassword(password, FBAUTH_DEFAULT_ITERATIONS, salt, hash) &&
                        data->db->setAuthConfig("basic", username, salt, hash, FBAUTH_DEFAULT_ITERATIONS))
                    {
                        status_msg = "Basic authentication enabled.";
                        curMode = "basic"; curUser = username;
                        curSalt = salt; curHash = hash; curIter = FBAUTH_DEFAULT_ITERATIONS;
                    }
                    else
                        status_msg = "Failed to hash or save the password.";
                }
            }
            else if (curMode == "basic" && !curHash.empty())
            {
                string keepUser = username.empty() ? curUser : username;
                if (data->db->setAuthConfig("basic", keepUser, curSalt, curHash, curIter))
                {
                    status_msg = "Basic authentication settings updated.";
                    curUser = keepUser;
                }
                else
                    status_msg = "Failed to save settings.";
            }
            else
                status_msg = "A password is required to enable Basic authentication.";
        }
        else
            status_msg = "Unknown authentication mode.";
    }

    dynamichead("FlashBack :: Settings");
    client->write("<div id=\"container\">\n");
    client->write("<h2>Settings</h2>\n");

    if (!status_msg.empty())
    {
        client->write("<p><strong>");
        client->write(htmlEscape(status_msg.c_str()));
        client->write("</strong></p>\n");
    }

    client->write("<h3>Authentication</h3>\n");
    client->write("<form method=\"get\" action=\"/settings\">\n<fieldset>\n");
    client->write("<p>Mode:\n<select name=\"mode\">\n");
    client->write("<option value=\"none\"");
    if (curMode != "basic") client->write(" selected=\"selected\"");
    client->write(">None (no login; intended for localhost)</option>\n");
    client->write("<option value=\"basic\"");
    if (curMode == "basic") client->write(" selected=\"selected\"");
    client->write(">HTTP Basic (username and password)</option>\n");
    client->write("</select></p>\n");

    client->write("<p>Username: <input type=\"text\" name=\"username\" value=\"");
    client->write(htmlEscape(curUser.c_str()));
    client->write("\" /></p>\n");

    client->write("<p>Password: <input type=\"password\" name=\"password\" value=\"\" />\n");
    client->write("<br /><small>Leave the password blank to keep the current one when Basic authentication is already enabled.</small></p>\n");

    client->write("<p><input type=\"submit\" name=\"submit\" value=\"Save\" /></p>\n");
    client->write("</fieldset>\n</form>\n");
    client->write("<p><small>Credentials are submitted in the URL over plain HTTP, so run FlashBack only on a trusted or localhost network. Passwords are stored hashed with PBKDF2-HMAC-SHA256.</small></p>\n");

    client->write("</div>\n");
    dynamicfoot();
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
    if (result == NULL)
        return;
    /* resultlen <= len (percent-decoding shrinks or equals); copy only what was decoded
     * and NUL-terminate within the original allocation (which is len+1 bytes). */
    memcpy( str, result, resultlen );
    str[resultlen] = '\0';
    free(result);
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

    client->write("<!DOCTYPE html>\n");
    client->write("<html lang=\"en\">\n<head>\n\t<meta charset=\"utf-8\" />\n\t<title>");
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
    char * pathptr = realpath(realp.c_str(), resolved);

   /* The file is not there */
    if (pathptr == NULL || !*resolved)
    {
        notfound();
        return;
    }

    if (strncmp(resolved, data->getWebServerRootPath().c_str(), data->getWebServerRootPath().length()) != 0)
    {
        free(resolved);
        notfound();
        return;
    }
    /* Require that the next character after the root prefix is '/' (subdirectory) or
     * '\0' (exact match) so that "/webrootEVIL" cannot pass as a sibling of "/webroot". */
    {
        char next = resolved[data->getWebServerRootPath().length()];
        if (next != '/' && next != '\0')
        {
            free(resolved);
            notfound();
            return;
        }
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

    if ( (fp = fopen( resolved, "rb" ) ) == NULL ) {
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

    {
        char buf[65536];   // 64 KB read buffer
        size_t n;
        while ( (n = fread(buf, 1, sizeof(buf), fp)) > 0 )
            client->write(string(buf, n));
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
    r.append(htmlEscape(path));
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
