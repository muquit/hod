/*
**  A simple hex and octal dumper
**
**  Comments:
**  I hate od. Liked the output of a DOS hex dumper.. so wrote this one.
**
**  Development History:
**      who                  when           why
**      muquit@semcor.com   Apr-07-1995     first cut
**      muquit@muquit.com   Jan-28-2003     updated
*/

/*
 * $Author: Muquit $
 * $Revision: 5 $
 * $Date: 9/06/04 9:40a $
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifdef HAVE_TIME_H
#include <time.h>
#endif /* HAVE_TIME_H */

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif /* HAVE_CTYPE_H */

#define HOD_VERSION_S   "1.7"

#define MTOLOWER(c)  (c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c)
#define MHEX2_VAL(s)  \
    ((isalpha(s) ? (MTOLOWER(s)-'a'+10) : (MTOLOWER(s)-'0')) & 0xf)

#define REVERSE_HOD     0x01
#define REVERSE_RAW     0x02

static FILE *hod_open_file(const char *filename);


static int offset_in_decimal=0;

static void usage(char *progname)
{
    int
        i;

    char
        pad[BUFSIZ];

    if (progname == NULL || *progname == '\0')
        exit(1);

    for (i = 0; i < strlen(progname); i++)
    {
        pad[i] = ' ';
    }
    pad[i] = '\0';

    (void) fprintf(stderr, 
     "hod v%s\n"
     "http://www.muquit.com/\n\n"
     "usage: %s [options] <filename>\n"
     "Where the options are:\n"
     " -v      : show version information\n"
     " -h      : show this help\n"
     " -i      : output C header file\n"
     " -o      : dump in octal\n"
     " -8      : show as block of 8 bytes\n"
     " -x str  : convert a hex input to decimal\n"
     " -d      : show offsets in decimal\n"
     " -s      : show identical output lines\n"
     " -r      : reverse hod hexdump to binary\n"
     " -w      : reverse regular hex bytes to binary\n"
     "\nIf no filename specified, it will read from stdin\n"
     "\nExample:\n"
     "$ hod file\n"
     "$ hod < file\n"
     "$ cat file | hod\n"
     "$ cat file | hod -\n"
     "$ hod < file \n"
     "$ hod -o file\n"
     "$ echo \"hello\" | hod\n"
     "$ echo -n \"hello\" | hod\n"
     "$ hod -x 1c0\n"
     "1c0 : 448\n"
     "$ echo \"0a 01 ff ef 0b\" | hod -w > bin.bin\n"
     "$ hod bin.bin | hod -r > bin_again.bin\n",
        HOD_VERSION_S,
        progname);
       exit(0);
}

/*
**  Open a file and returns the FILE pointer
**
**  Parameters:
**      filename    to open
**
**  Limitations and Comments:
**      In windows file will be open in binary mode
**
**  Return Values:
**      pointer to FILE on success, NULL on failure
**
**  Development History:
**           who    when           why
**  muquit@muquit   Mar-21-2010    first cut
*/

static FILE *hod_open_file(const char *filename)
{
}

static int print_hexstr_to_dec(char *hex_str)
{
    unsigned int
        hex_num;

    char
        *cp = hex_str;

    if (!hex_str)
        return (1);

    if ((*cp == '0') && ((*(cp + 1) == 'x') || (*(cp + 1) == 'X')))
    {
        cp += 2;
    }

    hex_num = strtol((char *) cp, NULL, 16);
    (void) fprintf(stderr, "%s : %u\n", hex_str, hex_num);
    return (0);
}

static unsigned char * _hex_to_bin(char *hex_string, int len, int *olen)
{
    int
        j = 0,
        n = 0,
        i;

    unsigned char
        *out,
        space[4];

    char
        *cp=hex_string;

    if (!hex_string || !len)
        return (NULL);

    if ((*cp == '0') && ((*(cp + 1) == 'x') || (*(cp + 1) == 'X')))
    {
        cp += 2;
        n = 2;
    }

    memset(space, 0, sizeof (space));

    /* allocate len bytes space for out, we only need half of it */
    out = (unsigned char *) malloc(len * sizeof (unsigned char));
    if (out == (unsigned char *) NULL)
    {
        (void) fprintf(stderr, "Error: memory allocation problem\n");
        return (NULL);
    }
    memset(out,0,len);

    for (i = n; i < len; i += 2)
    {
        if (hex_string[i] == '\n' || hex_string[i] == '\r' ||
                hex_string[i] == ' ' || hex_string[i] == '\t')
        {
            i--;
            continue;
        }
        space[0] = hex_string[i];
        space[1] = hex_string[i + 1];
        out[j++] = (unsigned char) strtol((char *) space, NULL, 16);
    }
    *olen = j;

    return (out);
}

/* input_base arg is not used yet */
static void dump_file(char *filename, FILE * ofp, int base, int input_base,
        int e,int skip)
{
    int
        fp;

    int
    shown = 0, rcount = 0, count = 0, line = 0, c, i;

    unsigned char
        ohexbuf[16],
        hexbuf[16];

    if (strcmp(filename, "-") == 0)
        fp = fileno(stdin);
    else
    {
#ifdef WINNT
        fp = open(filename, O_RDONLY|O_BINARY);
#else
        fp = open(filename, O_RDONLY);
#endif /* WINNT */
        if (fp <= 0)
        {
            perror(filename);
            exit(0);
        }
    }
    memset(hexbuf, 0, sizeof (hexbuf));
    memset(ohexbuf, 0, sizeof (hexbuf));

    (void) fprintf(ofp, "%11d", 0);
    for (i = 1; i < base; i++)
    {
        (void) fprintf(ofp, (base == 16) ? "%3x" : "%4o", i);
        if (e && (i == (base >> 1) - 1))
            (void) fprintf(ofp, " ");
    }
    (void) fprintf(ofp, (base == 16) ? "   " : "    ");

    for (i = 0; i < base; i++)
    {
        (void) fprintf(ofp, "%x", i);

        if (e && (i == (base >> 1) - 1))
            (void) fprintf(ofp, " ");
    }
    (void) fprintf(ofp, "\n");
    (void) fflush(ofp);

    while (1)
    {
        rcount = read(fp,hexbuf,base);
        if (rcount <= 0)
        {
            break;
        }
        if (skip && (memcmp(ohexbuf, hexbuf, rcount) == 0))
        {
            if (!shown)
            {
                shown = 1;
                (void) fprintf(ofp, "%8c\n", '*');
            }
            line++;
            continue;
        }
        shown = 0;
        if (rcount < base)
        {
            memset(hexbuf + rcount, 0, 16 - rcount);
        }

        if (offset_in_decimal == 1)
        {
            (void) fprintf(ofp, "%8d: ", line * base);
        }
        else
        {
            if (base == 16)
            {
                (void) fprintf(ofp, "%8x: ", line * base);
            }
            else
            {
                (void) fprintf(ofp, "%8o: ", line * base);
            }
        }

        for (count = 0; count < base; count++)
        {
            if (count < rcount)
            {
                (void) fprintf(ofp, (base == 16) ? "%02x " : "%03o ",
                               hexbuf[count]);
            }
            else
            {
                (void) fprintf(ofp, (base == 16) ? "   " : "    ");
            }
            if (e && (count == (base >> 1)))
                (void) fprintf(ofp, " ");
        }

        (void) fprintf(ofp, " ");
        for (i = 0; i < base; i++)
        {
            if (i < rcount)
            {
                if (hexbuf[i] >= 32 && hexbuf[i] < 127)
                    (void) fprintf(ofp, "%c", hexbuf[i]);
                else
                    (void) fprintf(ofp, ".");
            }
            else
            {
                (void) fprintf(ofp, " ");
            }

            if (e && (i == (base >> 1) - 1))
                (void) fprintf(ofp, " ");

        }
        line++;
        (void) fprintf(ofp, "\n");
        (void) fflush(ofp);
        if (skip)
            memcpy(ohexbuf, hexbuf, base);
    }

    if (fp != fileno(stdin))
        (void) close(fp);
}

/**
 * remove new line from string
 * taken from somewhere.
 */
static char _chop_nl(char *string)
{
    char
        c;
    char
        *ptr;

    c='\0';

    if (string == NULL || *string == '\0')
        return(c);

    for (ptr = string; *ptr; ptr++);

    if (ptr != string)
    {
        c = *(--ptr);

        if (c == '\n')
        {
            *ptr='\0';
        }
    }

    return (c);
}

static void _liberate_memory(void **memory)
{
    if (memory == NULL)
        return;

    free(*memory);
    *memory=(void *) NULL;
}


static void _free_zero(void *buf,int size)
{
    if (buf)
    {
        memset(buf,0,size);
        _liberate_memory(&buf);
    }
}
/**
 * another version of hex to binary which takes odd length input strings
 * @note    - borrowed from net-snmp tools.c
 * @return  length of output on success -1 on failure
 * @note    memory is allocated for *output, caller is responsible to free
 *          it
 */
static int _hex_to_bin2(char *hex_string,int len,char **output)
{
    int
        olen;

    char
        *s,
        *op;

    char
        *ip;

    olen=(len / 2) + (len % 2);
    s=(char *) calloc(1,(olen) ? olen : 1);
    if (s == NULL)
        return(-1);

    op=s;
    ip=hex_string;
    *output=NULL;
    *op=0;

    if (len % 2)
    {
        if (! isxdigit(*ip))
            goto exit_processing;

        *op++ = MHEX2_VAL(*ip);
        ip++;
    }

    while ((ip - hex_string) < len)
    {
        if (! isxdigit(*ip))
            goto exit_processing;

        *op=MHEX2_VAL(*ip) << 4;
        ip++;

        if (! isxdigit(*ip))
            goto exit_processing;

        *op++ += MHEX2_VAL(*ip);
        ip++;
    }
    *output=s;
    return(olen);

exit_processing:
    _free_zero(s,olen);
    return(-1);
}

/**
 * remove all white spaces from the string
 * @param str - the string to modify
 * @return    - pointer to the string
 * @note      - The string is changed.
 */
static char *_rm_white_spaces(char *str)
{
    char
        *obuf, 
        *nbuf;

    if (str) 
    {
        for (obuf=str, nbuf=str; *obuf; ++obuf)
        {
            if (!isspace(*obuf))
                *nbuf++ = *obuf;
        }
        *nbuf='\0';

    }
    return (str);
}   


static void print_c_header(char *filename,FILE *out)
{
    FILE 
        *fp=NULL;

    int
        reading_from_stdin=0,
        len,
        i=0,
        c;

    char
        *pbuf=NULL,
        *p,
        *q;

    time_t
        t;

    if (filename == NULL || *filename == '\0')
        exit(0);

    if (strncmp(filename,"-",1) == 0)
    {
        fp=stdin;
        reading_from_stdin=1;
    }

    if (fp == NULL)
    {
        fp=fopen(filename,"rb");
        if (fp == NULL)
        {
            (void) fprintf(stderr,"Could open open file %s for reading\n",filename);
            exit(0);
        }
    }
    i=0;

    if (!reading_from_stdin)
    {
        len=strlen(filename) + 1;
        pbuf=(char *) malloc(len * sizeof(char));
        if (pbuf == NULL)
        {
            (void) fprintf(stderr,"malloc failed\n");
            exit(0);
        }
        memset(pbuf,0,len);
        q=pbuf;
        for (p=filename; *p; p++)
        {
            if (!isalnum(*p))
            {
                *q++='_';
            }
            else
            {
                *q++=(*p);
            }
        }
    }
    else
    {
        pbuf=strdup("_octets");
        if (pbuf == NULL)
        {
            (void) fprintf(stderr,"malloc failed\n");
            exit(0);
        }
    }
    t=time(NULL);

    (void) fprintf(out,
"/*\n"
"** This file is created by hod %s by running:\n"
"**  hod -i %s\n"
"** on %s"
"** hod is a free software available from: http://www.muquit.com/\n"
"*/\n\n",
    HOD_VERSION_S,
    filename,
    (char *) ctime(&t));

    (void) fprintf(out,
"#ifndef %s_H\n"
"#define %s_H 1\n\n"
"const unsigned char %s[]=\n"
"{", pbuf,pbuf,pbuf);

    while((c=fgetc(fp)) != EOF)
    {
        if (i++ % 15 == 0)
            (void) fprintf(out,"\n");
        (void) fprintf(out,"0x%02x,",c);
    }
    (void) fprintf(out,"\n%s\n","};");
    (void) fprintf(out,
"const unsigned int %s_len=%d;\n\n",pbuf,i);
    (void) fprintf(out,
"#endif /* %s_H */\n",pbuf);

}



/**
 * convert a hod hexdump back to binary
 */
static int reverse_hod_hexdump(char *filename,FILE *out,int type)
{
    FILE
        *ifp=NULL;

    char
        *bin_out,
        *p,
        buf[256];
        
    int
        ofd,
        rc;

    const char *header="          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f   0123456789abcdef";

    ofd=fileno(out);
#ifdef WINNT
        setmode(ofd,O_BINARY); /* Make sure the write mode of out file is binary */
#endif /* WINNT */
    if (strncmp(filename,"-",1) == 0)
    {
        ifp=stdin;
    }
    else
    {
        ifp=fopen(filename,"rb");
        if (ifp == NULL)
        {
            (void) fprintf(stderr,"Could not open hexdump file (%s) for reading (%s)\n",filename,strerror(errno));
            return(-1);
        }
    }
    while((fgets(buf,sizeof(buf)-1,ifp)) != NULL)
    {
        _chop_nl(buf);
        if (type == REVERSE_HOD)
        {
            if (strncmp(buf,header,80) == 0)
            {
                continue;
            }
            p=buf;
            p += 10;
            *(p+48)='\0';
        }
        else
        {
            p=buf;
        }
        _rm_white_spaces(p);
        rc=_hex_to_bin2(p,strlen(p),&bin_out);
        write(ofd,(char *) bin_out,rc);
        (void) free((char *) bin_out);
    }

    if (ifp != stdin)
        (void) fclose(ifp);
    return(0);
}

int main(int argc, char *argv[])
{
    int
        input_base = 2, base = 16, i = 0, skip = 1;

    char
        *filename = NULL, *option;

    int
        output_c_header=0,
        reverse_hod=0,
        reverse_raw=0,
        e = 0;

    for (i = 1; i < argc; i++)
    {
        option = argv[i];
        if (strncmp(option, "-h", 2) == 0)
        {
            usage(argv[0]);
            /* won't be here */
        }

        if (strncmp(option, "-x", 2) == 0)
        {
            /* convert a hex number to decimal */
            input_base = 16;
            i++;
            if (i == argc)
            {
                (void) fprintf(stderr, "Missing hex offset with -x\n");
                return (1);
            }
            print_hexstr_to_dec(argv[i]);
            return (0);   /* outta here */
        }

        /* put a space after 8 bytes */
        if (strncmp(option, "-8", 2) == 0)
        {
            e = 1;
            continue;
        }

        if (strncmp(option, "-o", 2) == 0)
        {
            base = 8;
            continue;
        }

        if (strncmp(option, "-s", 2) == 0)
        {
            skip = 0;
            continue;
        }
        if (strncmp(option, "-r", 2) == 0)
        {
            reverse_hod=1;
            continue;
        }

        if (strncmp(option, "-w", 2) == 0)
        {
            reverse_raw=1;
            continue;
        }

        if (strncmp(option,"-d",2) == 0)
        {
            offset_in_decimal=1;
            continue;
        }
        if (strncmp(option,"-i",2) == 0)
        {
            output_c_header=1;
            continue;
        }


        if ((strncmp(option, "-v", 2) == 0) ||
            (strncmp(option, "-V", 2) == 0))
        {
            (void) fprintf(stdout,"hod Version: @(#) hod v%s\n",
                    HOD_VERSION_S);
            exit(0);
        }
        filename = argv[i];
    }

    if (filename == NULL)
    {
        filename = "-";
        /* won't be here */
    }
    if (output_c_header)
    {
        print_c_header(filename,stdout);
        return(0);
    }

    if (reverse_hod && reverse_raw)
    {
        (void) fprintf(stderr,"-r and -rb are mutually exclusive\n");
        return(1);
    }
    if (reverse_hod)
    {
        reverse_hod_hexdump(filename,stdout,REVERSE_HOD);
    }
    else if (reverse_raw)
    {
        reverse_hod_hexdump(filename,stdout,REVERSE_RAW);
    }
    else
    {
        dump_file(filename,stdout,base,input_base,e,skip);
    }

    return (1);
}
