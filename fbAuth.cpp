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

#include "fbAuth.h"

#include <cctype>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

#define FBAUTH_SALT_LEN 16
#define FBAUTH_HASH_LEN 32

static std::string toHex(const unsigned char* buf, size_t len)
{
    static const char* hexdigits = "0123456789abcdef";
    std::string out;
    out.reserve(len * 2);
    for (size_t i = 0; i < len; i++) {
        out += hexdigits[(buf[i] >> 4) & 0xF];
        out += hexdigits[buf[i] & 0xF];
    }
    return out;
}

static int hexVal(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static bool fromHex(const std::string& hex, unsigned char* out, size_t outlen)
{
    if (hex.size() != outlen * 2)
        return false;
    for (size_t i = 0; i < outlen; i++) {
        int hi = hexVal(hex[i * 2]);
        int lo = hexVal(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0)
            return false;
        out[i] = (unsigned char)((hi << 4) | lo);
    }
    return true;
}

static bool base64Decode(const std::string& in, std::string& out)
{
    if (in.empty() || (in.size() % 4) != 0)
        return false;

    std::vector<unsigned char> buf(in.size()); /* decoded length <= input length */
    int len = EVP_DecodeBlock(buf.data(),
                              (const unsigned char*)in.data(),
                              (int)in.size());
    if (len < 0)
        return false;

    /* EVP_DecodeBlock counts '=' padding as decoded zero bytes; remove them. */
    int pad = 0;
    if (in[in.size() - 1] == '=') pad++;
    if (in.size() >= 2 && in[in.size() - 2] == '=') pad++;
    len -= pad;
    if (len < 0)
        return false;

    out.assign((const char*)buf.data(), (size_t)len);
    return true;
}

bool fbAuthHashPassword(const std::string& password,
                        int iterations,
                        std::string& saltHex,
                        std::string& hashHex)
{
    if (iterations <= 0)
        return false;

    unsigned char salt[FBAUTH_SALT_LEN];
    if (RAND_bytes(salt, sizeof(salt)) != 1)
        return false;

    unsigned char hash[FBAUTH_HASH_LEN];
    if (PKCS5_PBKDF2_HMAC(password.c_str(), (int)password.size(),
                          salt, FBAUTH_SALT_LEN,
                          iterations, EVP_sha256(),
                          FBAUTH_HASH_LEN, hash) != 1)
        return false;

    saltHex = toHex(salt, FBAUTH_SALT_LEN);
    hashHex = toHex(hash, FBAUTH_HASH_LEN);
    return true;
}

bool fbAuthVerifyPassword(const std::string& password,
                          const std::string& saltHex,
                          const std::string& hashHex,
                          int iterations)
{
    if (iterations <= 0)
        return false;

    unsigned char salt[FBAUTH_SALT_LEN];
    unsigned char expected[FBAUTH_HASH_LEN];
    unsigned char actual[FBAUTH_HASH_LEN];

    if (!fromHex(saltHex, salt, FBAUTH_SALT_LEN))
        return false;
    if (!fromHex(hashHex, expected, FBAUTH_HASH_LEN))
        return false;

    if (PKCS5_PBKDF2_HMAC(password.c_str(), (int)password.size(),
                          salt, FBAUTH_SALT_LEN,
                          iterations, EVP_sha256(),
                          FBAUTH_HASH_LEN, actual) != 1)
        return false;

    /* constant-time comparison to avoid timing oracles */
    return CRYPTO_memcmp(expected, actual, FBAUTH_HASH_LEN) == 0;
}

bool fbAuthDecodeBasic(const std::string& v,
                       std::string& username,
                       std::string& password)
{
    size_t i = 0;
    while (i < v.size() && (v[i] == ' ' || v[i] == '\t'))
        i++;

    /* case-insensitive match of the "Basic" scheme token */
    static const char* scheme = "basic";
    size_t s = 0;
    while (s < 5 && i < v.size() &&
           tolower((unsigned char)v[i]) == scheme[s]) {
        i++;
        s++;
    }
    if (s != 5)
        return false;
    if (i >= v.size() || (v[i] != ' ' && v[i] != '\t'))
        return false;
    while (i < v.size() && (v[i] == ' ' || v[i] == '\t'))
        i++;

    std::string b64 = v.substr(i);
    while (!b64.empty() &&
           (b64[b64.size() - 1] == '\r' || b64[b64.size() - 1] == '\n' ||
            b64[b64.size() - 1] == ' '  || b64[b64.size() - 1] == '\t'))
        b64.erase(b64.size() - 1);

    std::string decoded;
    if (!base64Decode(b64, decoded))
        return false;

    size_t colon = decoded.find(':');
    if (colon == std::string::npos)
        return false;

    username = decoded.substr(0, colon);
    password = decoded.substr(colon + 1);
    return true;
}
