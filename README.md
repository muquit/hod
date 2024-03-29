# Introduction

**hod** is a very simple, nice little hexadecimal and octal data dumper. It is very useful when dealing with binary data. I wrote it long long time ago, because I did not like the complexities and the output format of od and such programs. I hope you will find it useful.

Suggestions, bug reports are always welcome.

# Synopsis

    usage: hod [options]
    Where the options are:
      -v      : show version information
      -h      : show this help
      -i      : output C header file
      -o      : dump in octal
      -8      : show as block of 8 bytes
      -x str  : convert a hex input to decimal
      -d      : show offsets in decimal
      -s      : show identical output lines
      -r      : reverse hod hexdump to binary
      -w      : reverse regular hex bytes to binary

If no filename specified, input will be read from stdin

    Example:
    $ hod file
    $ hod < file
    $ cat file | hod
    $ cat file | hod -
    $ hod < file
    $ hod -o file
    $ echo "hello" | hod
    $ echo -n "hello" | hod
    $ hod -x 1c0
    1c0 : 448
    $ echo "0a 01 ff ef 0b" | hod -w > bin.bin
    $ hod bin.bin | hod -r > bin_again.bin

    Note: -r and -w works with hexadecimal only.

# Examples

## hexdump a binary file

    $ hod foo.bin
          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f   0123456789abcdef
       0: 0c d6 9a aa 49 43 63 c2 d4 ce 6d 5a 75 e0 7f da  ....ICc...mZu...
      10: aa 9c 9a 69 5b 8d 1e 2a fd 32 34 b5 b1 2e 57 b7  ...i[..*.24...W.
      20: 20 6e 41 c3 dc 90 fc 4c ef 88 d7 22 f4 77 fb 0c   nA....L...`"`.w..
      30: a9 fd 20 a7 b4 b5 9b 9b f3 f0 90 2c ca c9 73 ad  .. ........,..s.
      40: b3 da 6d 5a f7 e3 af 4d df 35 2e 7c 53 54 d7 4e  ..mZ...M.5.|ST.N
      50: 5d d7 74 ec ea ef b5 ff 8d 61 ef 4f 59 e2 51 50  ].t......a.OY.QP
      60: 39 b8 cd e6 13 3d ee dc f1 dd 33 b6 9e 5b f1 ca  9....=....3..[..
      70: 88 1a 2f b6 ea d9 2b 3b 7f c5 07 af 67 2f 5b d6  ../...+;....g/[.
      80: d4 f9 69 95 6a c7 8a a0 39 2a 7e 70 fe 1b 40 f4  ..i.j...9*~p..@.
      90: 06 3d 86 10 2e 00                                .=....

## Octal dump

    $ hod -o foo.bin
          0   1   2   3   4   5   6   7    01234567
       0: 014 326 232 252 111 103 143 302  ....ICc.
      10: 324 316 155 132 165 340 177 332  ..mZu...
      20: 252 234 232 151 133 215 036 052  ...i[..*
      30: 375 062 064 265 261 056 127 267  .24...W.
      40: 040 156 101 303 334 220 374 114   nA....L
      50: 357 210 327 042 364 167 373 014  ...`"`.w..
      60: 251 375 040 247 264 265 233 233  .. .....
      70: 363 360 220 054 312 311 163 255  ...,..s.
     100: 263 332 155 132 367 343 257 115  ..mZ...M
     110: 337 065 056 174 123 124 327 116  .5.|ST.N
     120: 135 327 164 354 352 357 265 377  ].t.....
     130: 215 141 357 117 131 342 121 120  .a.OY.QP
     140: 071 270 315 346 023 075 356 334  9....=..
     150: 361 335 063 266 236 133 361 312  ..3..[..
     160: 210 032 057 266 352 331 053 073  ../...+;
     170: 177 305 007 257 147 057 133 326  ....g/[.
     200: 324 371 151 225 152 307 212 240  ..i.j...
     210: 071 052 176 160 376 033 100 364  9*~p..@.
     220: 006 075 206 020 056 000 006 075  .=.....=
     230: 206 020 056 000                  ....

## See the offset in decimal

    $ hod -d foo.bin
          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f   0123456789abcdef
       0: 0c d6 9a aa 49 43 63 c2 d4 ce 6d 5a 75 e0 7f da  ....ICc...mZu...
      16: aa 9c 9a 69 5b 8d 1e 2a fd 32 34 b5 b1 2e 57 b7  ...i[..*.24...W.
      32: 20 6e 41 c3 dc 90 fc 4c ef 88 d7 22 f4 77 fb 0c   nA....L...`"`.w..
      48: a9 fd 20 a7 b4 b5 9b 9b f3 f0 90 2c ca c9 73 ad  .. ........,..s.
      64: b3 da 6d 5a f7 e3 af 4d df 35 2e 7c 53 54 d7 4e  ..mZ...M.5.|ST.N
      80: 5d d7 74 ec ea ef b5 ff 8d 61 ef 4f 59 e2 51 50  ].t......a.OY.QP
      96: 39 b8 cd e6 13 3d ee dc f1 dd 33 b6 9e 5b f1 ca  9....=....3..[..
     112: 88 1a 2f b6 ea d9 2b 3b 7f c5 07 af 67 2f 5b d6  ../...+;....g/[.
     128: d4 f9 69 95 6a c7 8a a0 39 2a 7e 70 fe 1b 40 f4  ..i.j...9*~p..@.
     144: 06 3d 86 10 2e 00                                .=....

    $ hod x.db
          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f   0123456789abcdef
       0: 00 06 15 61 00 00 00 02 00 00 04 d2 00 00 10 00  ...a............
      10: 00 00 00 0c 00 00 01 00 00 00 01 00 00 00 00 08  ................
      20: 00 00 00 01 00 00 00 02 00 00 00 01 00 00 00 03  ................
      30: 00 00 00 01 00 01 00 00 00 00 00 02 00 00 00 01  ................
      40: 95 6e 7d e3 00 00 00 00 00 00 00 02 00 00 00 02  .n}.............
      50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
       *
      c0: 00 00 00 00 08 01 00 00 00 00 00 00 00 00 00 00  ................
      d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
       *
    1000: 04 00 f9 0f f8 0f ed 0f dd 0f cf 0f dd 0f ff ff  ................
    1010: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff  ................
       *
    1fd0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff 27 27  ..............''
    1fe0: 9b 65 b7 4b b4 fc 90 2e db 8c 11 f4 6c 67 6c 6f  .e.K........lglo
    1ff0: 62 61 6c 2d 73 61 6c 74 03 56 65 72 73 69 6f 6e  bal-salt.Version
    2000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
       *
    3000: fd ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff  ................
    3010: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff  ................
       *

Here `*` indicates identical lines. To show identical lines use the flag -s

## Create C header file

    $ hod -i square.gif > square.h

The header file `square.h` will look like:

    /*
    ** This file is created by hod 1.7 by running:
    **  hod -i square.gif
    ** on Sun May 13 14:44:25 2012
    ** hod is a free software available from: http://www.muquit.com/
    */

    #ifndef square_gif_H
    #define square_gif_H 1

    const unsigned char square_gif[]=
    {
    0x47,0x49,0x46,0x38,0x39,0x61,0x10,0x00,0x10,0x00,0xf3,0x00,0x00,0x00,0x00,
    0x00,0x10,0x0f,0x0e,0x20,0x1e,0x1b,0x30,0x2d,0x29,0x3f,0x3b,0x36,0x4f,0x4a,
    0x44,0x5f,0x59,0x52,0x6f,0x68,0x5f,0x7f,0x77,0x6d,0x8f,0x86,0x7a,0x9f,0x95,
    0x88,0xaf,0xa4,0x96,0xbe,0xb2,0xa3,0xce,0xc1,0xb1,0xde,0xd0,0xbe,0xee,0xdf,
    0xcc,0x21,0xf9,0x04,0x00,0x00,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x00,0x10,
    0x00,0x10,0x00,0x00,0x04,0x4e,0xf0,0xc9,0x49,0xeb,0x73,0x38,0xeb,0xed,0x9a,
    0xff,0x60,0xd8,0x30,0x64,0x69,0x9e,0xcc,0xa2,0xae,0x6c,0xbb,0x28,0x70,0x2c,
    0xcf,0x4a,0x62,0xdf,0x78,0x9e,0x20,0x7c,0xef,0xff,0x88,0x83,0x70,0x48,0x2c,
    0x1e,0x0c,0xc8,0xa4,0x72,0x69,0x28,0x38,0x9f,0xd0,0x68,0x81,0x40,0xad,0x5a,
    0xaf,0x84,0x81,0x76,0xcb,0xed,0x0e,0x04,0xe0,0xb0,0x78,0x2c,0x08,0x98,0xcf,
    0xe8,0x74,0x00,0xc0,0x6e,0xbb,0xdf,0x80,0x08,0x00,0x3b,
    };
    const unsigned int square_gif_len=161;

    #endif /* square_gif_H */

# Compile/Install

## Linux/Unix/Mac OS X

Go to the source directory and type:

    $ sh ./configure
    $ make clean
    $ make
    # make install

or

    $ sudo make install

## Linux Debian package

A pre-built Debian package is available. To install the package, type

    sudo dpkg -i hod_1.7-ubuntu_i386.deb

To remove the package, type:

    sudo dpkg -r hod

## Microsoft Windows

A compiled binary for MS Windows is already supplied. However, if you need to modify something or want to compile it yourself, you will need MS Visual Studio (I used MS Visual Studio 10, freely available from Microsoft). To compile, open a command shell and type:

    c:\> nmake -f Makefile.win clean
    c:\> nmake -f Makefile.win
