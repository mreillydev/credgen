credgen is a lightweight password generator written in C for Linux and BSD
using OpenBSD's arc4random library. In addition to keyset filtering and output
formatting options, it contains an embedded dictionary of English words as a
generation option.

This software is tested on 64 bit Debian and FreeBSD.

usage: credgen [options] [ len | min max ]
    Pseudorandomly generate password credentials. By default, a credential of
    20 to 30 character length from the 94 character Qwerty keyset is generated.
Options:
    -h        Print usage text.
    -v        Print version string.
    -[aA1sS]  Choose keyset from a-z, A-Z, 0-9, and lower/upper symbols.
    -e        Easy mode. Same as '-aaaA1 8 12'.
    -w        Generate strings of common words (last word may exceed max).
    -d[=dlm]  Delimit characters (or words if -w) with spaces (or dlm).
    -dn=n     Change delimiter frequency from 5 (not if -w).
    len, min, and max allow changing range of possible password lengths.
