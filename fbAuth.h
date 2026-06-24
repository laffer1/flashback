/*-
 * Copyright (C) 2026 Lucas Holt. All rights reserved.
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
 * ANY EXPRESS OR IMPLIED WARRANTIES ARE DISCLAIMED.
 */

#ifndef FBAUTH_H
#define FBAUTH_H

#include <string>

/*
 * fbAuth - password hashing and HTTP Basic credential helpers.
 *
 * Passwords are hashed with PBKDF2-HMAC-SHA256 (OpenSSL libcrypto) using a
 * per-credential random salt. The salt and derived key are stored hex-encoded.
 * Plaintext passwords are never stored or logged.
 */

#define FBAUTH_DEFAULT_ITERATIONS 100000

/* Hash a password with a freshly generated random salt.
 * On success returns true and fills saltHex and hashHex with lowercase hex. */
bool fbAuthHashPassword(const std::string& password,
                        int iterations,
                        std::string& saltHex,
                        std::string& hashHex);

/* Verify a candidate password against a stored salt+hash using a
 * constant-time comparison. Returns true only on an exact match. */
bool fbAuthVerifyPassword(const std::string& password,
                          const std::string& saltHex,
                          const std::string& hashHex,
                          int iterations);

/* Decode an HTTP "Authorization" header value of the form "Basic <base64>"
 * into username and password (split on the first ':'). Returns true on
 * success. The scheme match is case-insensitive. */
bool fbAuthDecodeBasic(const std::string& authHeaderValue,
                       std::string& username,
                       std::string& password);

#endif
