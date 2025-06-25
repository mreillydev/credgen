# credgen

credgen is a lightweight password generator written in C for Linux and BSD
using OpenBSD's arc4random library. In addition to keyset filtering and output
formatting options, it contains an embedded dictionary of English words as a
generation option.

# Platforms

This software is tested on 64 bit Debian and FreeBSD.

# Usage

usage: credgen [options] [ len | min max ]  
&emsp;Pseudorandomly generate password credentials. By default, a credential of  
&emsp;20 to 30 character length from the 94 character Qwerty keyset is generated.  
Options:  
&emsp;-h          Print usage text.  
&emsp;-v          Print version string.  
&emsp;-[aA1sS]    Choose keyset from a-z, A-Z, 0-9, and lower/upper symbols.  
&emsp;-e          Easy mode. Same as '-aaaA1 8 12'.  
&emsp;-w          Generate strings of English words (default 8 to 14 words).  
&emsp;-wordcount  Size of embedded words dictionary (for complexity calculations).  
&emsp;-d[=dlm]    Delimit characters (or words if -w) with spaces (or dlm).  
&emsp;-dn=n       Change delimiter frequency from 5 (not if -w).  
&emsp;len, min, and max allow changing range of possible password lengths.  
