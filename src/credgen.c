// Copyright 2025 Michael Reilly <mreilly@mreilly.dev>
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the names of the copyright holders nor the names of the
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // memset
#include <stdint.h> // uint32_t
#include <limits.h> // CHAR_BIT
#ifdef __linux__
    #include <bsd/bsd.h> // arc4random
#endif

#include "credgen_words.h"

// Constants
#define MIN_NORMAL 20
#define MAX_NORMAL 30
#define MIN_EASY    8
#define MAX_EASY   12
#define DLM_NORMAL " "
#define DLN_NORMAL  5

// Strings
char const Qwerty[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?";

char const Usage[] =
"usage: credgen [options] [ len | min max ]\n"
"    Pseudorandomly generate password credentials. By default, a credential of\n"
"    %d to %d character length from the %zu character Qwerty keyset is generated.\n"
"Options:\n"
"    -h        Print usage text.\n"
"    -v        Print version string.\n"
"    -[aA1sS]  Choose keyset from a-z, A-Z, 0-9, and lower/upper symbols.\n"
"    -e        Easy mode. Same as '-aaaA1 %d %d'.\n"
"    -w        Generate strings of English words (last word may exceed max).\n"
"    -d[=dlm]  Delimit characters (or words if -w) with spaces (or dlm).\n"
"    -dn=n     Change delimiter frequency from %d (not if -w).\n"
"    len, min, and max allow changing range of possible password lengths.\n";

// Buffer for character set available to be used during generation
char Keyset[1024];

// Fill Keyset with subset of Qwerty specified by at and len
int acquire(int at, int len, char **dst)
{
    if(Keyset + sizeof Keyset - 1 - *dst < len)
    {
        fprintf(stderr, "excessive argument\n");
        exit(1);
    }
    strncpy(*dst, Qwerty + at, len);
    *dst += len;
    return 0;
}

// Return string for (*position)-th word in NUL-separated words data
// If (*position) is larger than total word count, then set it to total count
const char* nthword(uint32_t *position)
{
    uint32_t index = 0;
    for(const char *str = credgen_words_txt, *nxt = NULL;; str = nxt+1, index++)
    {
        if(*str == '\0' || ! (nxt = strchr(str, 0)))
            break;
        if(index == *position)
            return str;
    }
    *position = index;
    return NULL;
}

uint32_t randint(uint32_t upper)
{
    return arc4random_uniform(upper);
}

int main(int argc, char **argv)
{
    int pass_len = 0, minpl = 0, maxpl = 0, dln = 0;
    int min_default = MIN_NORMAL, max_default = MAX_NORMAL;
    uint32_t wordspace = 0, pickSize = 0;
    char *dst = Keyset, *dlm = NULL;

    memset(Keyset, '\0', sizeof Keyset);

    // Process args and establish keyset
    while(argc >= 2)
    {
        // -h usage string
        if(strcmp(argv[1], "-h") == 0)
        {
            printf(Usage, MIN_NORMAL, MAX_NORMAL, strlen(Qwerty), MIN_EASY,
                   MAX_EASY, DLN_NORMAL);
            goto end;
        }
        // -v version string
        else if(strcmp(argv[1], "-v") == 0)
        {
            printf("%s\n", BUILD_DATE);
            goto end;
        }
        // -w embedded words dictionary
        else if(strcmp(argv[1], "-w") == 0)
        {
            wordspace = UINT_MAX;
            nthword(&wordspace);
        }
        // -e easy mode
        else if(strcmp(argv[1], "-e") == 0)
        {
            min_default = MIN_EASY;
            max_default = MAX_EASY;
            acquire( 0, 26, &dst); // a
            acquire( 0, 26, &dst); // a
            acquire( 0, 26, &dst); // a
            acquire(26, 26, &dst); // A
            acquire(52, 10, &dst); // 1
        }
        // other flags
        else if(argv[1][0] == '-')
        {
            if(argv[1][1] == 'd')
            {
                // -dn delimiter frequency
                if(argv[1][2] == 'n')
                {
                    if(argv[1][3] != '=' || (dln = atoi(argv[1]+4)) < 1)
                    {
                        fprintf(stderr, "bad argument to -dn\n");
                        goto bad;
                    }
                }
                // -d delimiter specification
                else
                {
                    if(argv[1][2] == '=')
                        dlm = argv[1] + 3;
                    else
                        dlm = DLM_NORMAL;
                    if(strlen(dlm) < 1)
                    {
                        fprintf(stderr, "bad argument to -d\n");
                        goto bad;
                    }
                }
            }
            // -[aA1sS] keyset specification
            else
            {
                int ix = 1; for( ; argv[1][ix] != '\0'; ix++)
                {
                    switch(argv[1][ix])
                    {
                    case 'a': acquire( 0, 26, &dst); break;
                    case 'A': acquire(26, 26, &dst); break;
                    case '1': acquire(52, 10, &dst); break;
                    case 's': acquire(62, 11, &dst); break;
                    case 'S': acquire(73, 21, &dst); break;
                    default:
                    unrecognized:
                        fprintf(stderr,"unrecognized argument '%s'\n", argv[1]);
                        goto bad;
                    }
                }
                if(ix == 1)
                    goto unrecognized;
            }
        }
        // min length
        else if(minpl == 0)
        {
            minpl = atoi(argv[1]);
            if(minpl <= 0)
            {
                fprintf(stderr, "bad min argument\n");
                goto bad;
            }
        }
        // max length
        else if(maxpl == 0)
        {
            maxpl = atoi(argv[1]);
            if(maxpl <= 0 || maxpl < minpl)
            {
                fprintf(stderr, "bad max argument\n");
                goto bad;
            }
        }
        // excess arguments
        else
        {
            fprintf(stderr, "too many arguments\n");
            goto bad;
        }
        // advance arg
        argc--;
        argv++;
    }

    // Prohibit specification of keyset or -dn with -w
    if(wordspace && (Keyset[0] || dln > 0))
    {
        fprintf(stderr, "incompatible with -w\n");
        goto bad;
    }

    // Set defaults if unspecified
    if(wordspace == 0 && Keyset[0] == '\0')
        strncpy(Keyset, Qwerty, sizeof Keyset);
    Keyset[sizeof Keyset - 1] = '\0';
    if(Keyset[0])
        pickSize = strlen(Keyset);
    else
        pickSize = wordspace;
    if(dlm && dln == 0)
        dln = DLN_NORMAL;
    if(maxpl == 0)
    {
        if(minpl == 0)
            maxpl = max_default;
        else
            maxpl = minpl;
    }
    if(minpl == 0)
        minpl = min_default;

    // Choose length
    if(minpl > maxpl)
        goto internal_error;
    pass_len = minpl + randint(maxpl + 1 - minpl);

    // Repeatedly generate bounded random ints and index into keyset or words
    for(int pd = 0; pd < pass_len; )
    {
        uint32_t res = randint(pickSize);
        if(Keyset[0])
        {
            printf("%s%c", ((dlm && pd && (pd%dln)==0)? dlm : ""), Keyset[res]);
            pd++;
        }
        else
        {
            const char *tmp = nthword(&res);
            if( ! tmp)
                goto internal_error;
            printf("%s%s", ((dlm && pd) ? dlm : ""), tmp);
            pd += strlen(tmp);
        }
    }
    printf("\n");

    // End
end:
    exit(0);
bad:
    exit(1);
internal_error:
    fprintf(stderr, "internal error\n");
    exit(2);
}
