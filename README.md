# credgen

credgen is a lightweight password generator written in C for Linux and BSD
using OpenBSD's arc4random library. In addition to keyset filtering and output
formatting options, it contains an embedded dictionary of English words as a
generation option.

# Platforms

This software is tested on 64 bit Debian and FreeBSD.

# Usage

```
usage: credgen [options] [ len | min max ]
    Pseudorandomly generate password credentials. By default, a credential of
    20 to 30 character length from the 94 character Qwerty keyset is generated.
Options:
    -h          Print usage text.
    -v          Print version string.
    -[aA1sS]    Choose keyset from a-z, A-Z, 0-9, and lower/upper symbols.
    -e          Easy mode. Same as '-aaaA1 8 12'.
    -w          Generate strings of English words (default 8 to 14 words).
    -wordcount  Size of embedded words dictionary (for complexity calculations).
    -d[=dlm]    Delimit characters (or words if -w) with spaces (or dlm).
    -dn=n       Change delimiter frequency from 5 (not if -w).
    len, min, and max allow changing range of possible password lengths.
```

# Examples

```console
$ credgen
T^n?8u?|#ac%xqMB_t))2Bxy=wM"#R
```

```console
$ credgen 6 10
%SUcaTFN*Y
```

```console
$ credgen 6 10
?7z}<4B
```

```console
$ credgen -a1
stb8m9ig0f7l173uacq45fo2xn6
```

```console
$ credgen -A -d 15
RXWRQ KSUCS GLWFM
```

```console
$ credgen -A -d=_ -dn=2 15  # assuming same random state as previous
RX_WR_QK_SU_CS_GL_WF_M
```

```console
$ credgen -w -d 8
engineer attest minna electronegative jubbulpore politics daylong epicotyl
```

```console
$ credgen -wordcount
53809
```

# License

Licensed under the BSD-3 License.
