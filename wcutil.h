/**
 * Copyright (c) 2008, Renzo Carbonara <gnuk0001 [at] gmail [dot] com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *    * Neither the name of the Renzo Carbonara nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 **/


#include <errno.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


/**
 * wcs_encode:
 *
 * Wrapper around iconv() to encode a wide character string `wcs' using
 * iconv's conversion descriptor `cd'.  This conversion descriptor's "from"
 * codeset *must* be WCHAR_T, otherwhise unexpected results might happen.
 *
 * On success, returns the newly allocated encoded (multi) byte string.
 * The caller is responsible for freeing the allocated memory using free().
 *
 * On error, returns NULL and `errno' is set according to iconv().
 **/
char *wcs_encode(const wchar_t *wcs, iconv_t cd);
char *wcs_encode(const wchar_t *wcs, iconv_t cd)
{
    char *out, *wp;
    char *rp = (char *) wcs;
    size_t isiz = sizeof(wchar_t) * wcslen(wcs);
    size_t osiz = 1 + isiz;

    if ((out = malloc(osiz)) == NULL)
        return NULL;
    wp = out;
    if ((iconv(cd, (void *) &rp, &isiz, &wp, &osiz)) == (size_t) -1) {
        free(out);
        return NULL;
    }
    *wp = '\0';
    return out;
}


/**
 * wcs_decode:
 *
 * Wrapper around iconv() to decode an encoded (multi) byte string `mbs' as a
 * wide character string using iconv's conversion descriptor `cd'.  This
 * conversion descriptor's "to" codeset *must* be WCHAR_T, otherwhise unexpected
 * results might happen.
 *
 * On success, returns the newly allocated decoded wide character string.
 *
 * The caller is responsible for freeing the allocated memory using free().
 *
 * On error, returns NULL and `errno' is set according to iconv().
 **/
wchar_t *wcs_decode(const char *mbs, iconv_t cd);
wchar_t *wcs_decode(const char *mbs, iconv_t cd)
{
    int en;
    char *out, *wp;
    char *rp = (char *) mbs;
    size_t isiz = sizeof(char) * strlen(mbs);
    size_t osiz = 1 + sizeof(wchar_t) * isiz;

    if ((out = malloc(osiz)) == NULL)
        return NULL;
    wp = out;
    if ((iconv(cd, (void *) &rp, &isiz, &wp, &osiz)) == (size_t) -1) {
        free(out);
        return NULL;
    }
    *wp = '\0';
    return (wchar_t *) out;
}

