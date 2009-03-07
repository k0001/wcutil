/**
 * Copyright (c) 2009, Renzo Carbonara <gnuk0001 [at] gmail [dot] com>
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


/* Type able to hold an Unicode Character (Code Point). 32 bits needed */
typedef unsigned int unichar;

/**
 * utf8_encode_c:
 *
 * Encodes an Unicode Character (Code Point) `codepoint` as UTF-8 and stores
 * the result in `octs`.  At most, 4 bytes will be written at `octs`.
 *
 * On success, the number of octets the encoded character occupies (1, 2, 3, 4)
 * is returned.
 *
 * On failure, -1 is returned and `errno` is set. Possible `errno` values:
 *
 *  EINVAL:
 *      Invalid `codepoint` value.
 **/
int utf8_encode_c(unichar codepoint, unsigned char *octs);
int utf8_encode_c(unichar codepoint, unsigned char *octs)
{
    if(codepoint <= 0x0000007f) {
        octs[0] = (char) codepoint;
        return 1;
    }
    if (codepoint <= 0x000007ff) {
        octs[0] = (char) (0xc0 | codepoint >> 6  & ~0xffffffe0);
        octs[1] = (char) (0x80 | codepoint       & ~0xffffffc0);
        return 2;
    }
    if (codepoint <= 0x0000ffff) {
        octs[0] = (char) (0xe0 | codepoint >> 12 & ~0xfffffff0);
        octs[1] = (char) (0x80 | codepoint >> 6  & ~0xffffffc0);
        octs[2] = (char) (0x80 | codepoint       & ~0xffffffc0);
        return 3;
    }
    if (codepoint <= 0x0010ffff) {
        octs[0] = (char) (0xf0 | codepoint >> 18 & ~0xfffffff8);
        octs[1] = (char) (0x80 | codepoint >> 12 & ~0xffffffc0);
        octs[2] = (char) (0x80 | codepoint >> 6  & ~0xffffffc0);
        octs[3] = (char) (0x80 | codepoint       & ~0xffffffc0);
        return 4;
    }
    /* codepoint > 0x0010ffff */
    errno = EINVAL;
    return -1;
}


/**
 * utf8_encode_s:
 *
 * Encodes an Unicode String `us` as UTF-8 and stores the result in `s`.  At
 * most, four times the length of `us` plus one will be written at `s`.
 *
 * On success, the lenght of the new encoded string will be returned.
 *
 * On failure, -1 is returned and `errno` is set. Possible `errno` values:
 *
 *  EINVAL:
 *      Invalid `codepoint` value.
 **/
int utf8_encode_s(unichar *us, unsigned char *s);
int utf8_encode_s(unichar *us, unsigned char *s)
{
    size_t usi, si;
    int nocts, j;
    unsigned char octs[4];

    for (usi = 0, si = 0; us[usi] != '\0'; ++usi) {
        if ((nocts = utf8_encode_c(us[usi], octs)) == -1)
            return -1;
        for (j = 0; j < nocts; ++j)
            s[si++] = octs[j];
    }
    s[si] = '\0';
    return si;
}

