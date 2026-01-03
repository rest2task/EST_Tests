#include "stdarg.h"
#include "com.h"
#include "uarthw.h"
#include "uart.h"
#include "hwcfg.h"
#include "printf.h"

#define DB_PRT_UART USART2

#if 0
typedef char *va_list;
#define __va_sz(mode)  (((sizeof(mode) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_dcl int va_alist;
#define va_start(list) (list = (char *) &va_alist)
#define va_end(list)
#define va_arg(list,mode) (*((mode *)((list += __va_sz(mode)) - __va_sz(mode))))
#endif

#define FL_LJUST        0x0001          /* left-justify field */
#define FL_SIGN         0x0002          /* sign in signed conversions */
#define FL_SPACE        0x0004          /* space in signed conversions */
#define FL_ALT          0x0008          /* alternate form */
#define FL_ZEROFILL     0x0010          /* fill with zero's */
#define FL_SHORT        0x0020          /* optional h */
#define FL_LONG         0x0040          /* optional l */
#define FL_LONGDOUBLE   0x0080          /* optional L */
#define FL_WIDTHSPEC    0x0100          /* field width is specified */
#define FL_PRECSPEC     0x0200          /* precision is specified */
#define FL_SIGNEDCONV   0x0400          /* may contain a sign */
#define FL_NOASSIGN     0x0800          /* do not assign (in scanf) */
#define FL_NOMORE       0x1000          /* all flags collected */
#define set_pointer(flags)      (flags |= FL_LONG)
#define toupper(c) ( c >= 'a' ?  c- ('a' - 'A') : c)

/*
 * doprnt.c - print formatted output
 */
/* $Header: /opt/proj/minix/cvsroot/src/lib/stdio/doprnt.c,v 1.1.1.1 2005/04/21 14:56:34 beng Exp $ */
/* gnum() is used to get the width and precision fields of a format. */

static char buf[40];
__forceinline 
static const char * gnum(register const char *f, int *ip, va_list *app)
{
        register int    i, c;

        if (*f == '*') {
                *ip = va_arg((*app), int);
                f++;
        } else {
                i = 0;
                while ((c = *f - '0') >= 0 && c <= 9) {
                        i = i*10 + c;
                        f++;
                }
                *ip = i;
        }
        return f;
}

__forceinline 
static  char* _i_compute(unsigned long val, int base, char *s, int nrdigits)
{
	int c;
	char t;
	char* p;

	if(s >= buf + sizeof(buf))
		return s;
		
	p = s;
	do
	{
		c= val % base ;
		val /= base;
		*s++ = (c>9 ? c-10+'a' : c+'0');
		nrdigits--;
	}while((val > 0 ||  nrdigits > 0) && p < buf + sizeof(buf));

	/*reverse char*/
	c = (s -p) ;
	p = s - c;
	while(c > 1)
	{
		c--;
		t = *p;
		*p = *(p + c);
		*(p + c) = t;
		p++;
		c--;
	}
	
	return s;
};


/* print an ordinal number */
__forceinline 
static  char *o_print(va_list *ap, int flags, char *s, char c, int precision, int is_signed)
{
        long signed_val;
        unsigned long unsigned_val;
        char *old_s = s;
        int base = 0;

        switch (flags & (FL_SHORT | FL_LONG)) {
        case FL_SHORT:
                if (is_signed) {
                        signed_val = (short) va_arg(*ap, int);
                } else {
                        unsigned_val = (unsigned short) va_arg(*ap, unsigned);
                }
                break;
        case FL_LONG:
                if (is_signed) {
                        signed_val = va_arg(*ap, long);
                } else {
                        unsigned_val = va_arg(*ap, unsigned long);
                }
                break;
        default:
                if (is_signed) {
                        signed_val = va_arg(*ap, int);
                } else {
                        unsigned_val = va_arg(*ap, unsigned int);
                }
                break;
        }

        if (is_signed) {
                if (signed_val < 0) {
                        *s++ = '-';
                        signed_val = -signed_val;
                } else if (flags & FL_SIGN) *s++ = '+';
                else if (flags & FL_SPACE) *s++ = ' ';
                unsigned_val = signed_val;
        }
        if ((flags & FL_ALT) && (c == 'o')) *s++ = '0';
        if (!unsigned_val) {
                 if (!precision)
                        return s;
        } else if (((flags & FL_ALT) && (c == 'x' || c == 'X'))
                    || c == 'p') {
                *s++ = '0';
                *s++ = (c == 'X' ? 'X' : 'x');
        }

        switch (c) {
        case 'b':       base = 2;       break;
        case 'o':       base = 8;       break;
        case 'd':
        case 'i':
        case 'u':       base = 10;      break;
        case 'x':
        case 'X':
        case 'p':       base = 16;      break;
        }

        s = _i_compute(unsigned_val, base, s, precision);

        if (c == 'X')
                while (old_s != s) {
                        *old_s = toupper(*old_s);
                        old_s++;
                }

        return s;
}

__forceinline static char* putc(char c, char*vbuf)
{
	if(vbuf == NULL)
	{

	}
	else
	{
		*vbuf++ = c;
	}

	return vbuf;
}

int _doprnt(register const char *fmt, va_list ap, char *vbuf)
{
	char   *s;
	int    j;
        int             i = 0, c = 0, width = 0, precision = 0, zfill = 0, flags = 0, between_fill = 0;
        int             nrchars=0;
        const char      *oldfmt;
        char            *s1;

        while((c = *fmt++) != '\0') 
        {
                if (c != '%') 
                {
			vbuf = putc(c, vbuf);
                        nrchars++;
                        continue;
                }

                flags = 0;
                do 
                {
                        switch(*fmt) {
                        case '-':       flags |= FL_LJUST;      break;
                        case '+':       flags |= FL_SIGN;       break;
                        case ' ':       flags |= FL_SPACE;      break;
                        case '#':       flags |= FL_ALT;        break;
                        case '0':       flags |= FL_ZEROFILL;   break;
                        default:        flags |= FL_NOMORE;     continue;
                        }
                        fmt++;
                } while(!(flags & FL_NOMORE));

                oldfmt = fmt;
                fmt = gnum(fmt, &width, &ap);
                if (fmt != oldfmt) flags |= FL_WIDTHSPEC;

                if (*fmt == '.')
                {
                        fmt++; oldfmt = fmt;
                        fmt = gnum(fmt, &precision, &ap);
                        if (precision >= 0) flags |= FL_PRECSPEC;
                }

                if ((flags & FL_WIDTHSPEC) && width < 0) 
                {
                        width = -width;
                        flags |= FL_LJUST;
                }
                
                if (!(flags & FL_WIDTHSPEC)) width = 0;

                if (flags & FL_SIGN) flags &= ~FL_SPACE;

                if (flags & FL_LJUST) flags &= ~FL_ZEROFILL;


                s = s1 = buf;

                switch (*fmt) 
                {
	                case 'h':       flags |= FL_SHORT; fmt++; break;
	                case 'l':       flags |= FL_LONG; fmt++; break;
	                case 'L':       flags |= FL_LONGDOUBLE; fmt++; break;
                }

                switch (c = *fmt++) 
                {
	                default:
				vbuf = putc(c, vbuf);
	                        nrchars++;
	                        continue;
	                case 'n':
	                        if (flags & FL_SHORT)
	                                *va_arg(ap, short *) = (short) nrchars;
	                        else if (flags & FL_LONG)
	                                *va_arg(ap, long *) = (long) nrchars;
	                        else
	                                *va_arg(ap, int *) = (int) nrchars;
	                        continue;
	                case 's':
	                        s1 = va_arg(ap, char *);
	                        if (s1 == NULL)
	                                s1 = "(null)";
	                        s = s1;
	                        while (precision || !(flags & FL_PRECSPEC)) 
	                        {
	                                if (*s == '\0')
	                                        break;
	                                s++;
	                                precision--;
	                        }
	                        break;
	                case 'p':
	                        set_pointer(flags);
	                        /* fallthrough */
	                case 'b':
	                case 'o':
	                case 'u':
	                case 'x':
	                case 'X':
	                        if (!(flags & FL_PRECSPEC)) precision = 1;
	                        else if (c != 'p') flags &= ~FL_ZEROFILL;
	                        s = o_print(&ap, flags, s, c, precision, 0);
	                        break;
	                case 'd':
	                case 'i':
	                        flags |= FL_SIGNEDCONV;
	                        if (!(flags & FL_PRECSPEC)) precision = 1;
	                        else flags &= ~FL_ZEROFILL;
	                        s = o_print(&ap, flags, s, c, precision, 1);
	                        break;
	                case 'c':
	                        *s++ = va_arg(ap, int);
	                        break;
	                case 'r':
	                        ap = va_arg(ap, va_list);
	                        fmt = va_arg(ap, char *);
	                        continue;
                }

                zfill = ' ';
                if (flags & FL_ZEROFILL) zfill = '0';
                j = s - s1;

                /* between_fill is true under the following conditions:
                 * 1- the fill character is '0'
                 * and
                 * 2a- the number is of the form 0x... or 0X...
                 * or
                 * 2b- the number contains a sign or space
                 */
                between_fill = 0;
                if ((flags & FL_ZEROFILL) && (((c == 'x' || c == 'X') && (flags & FL_ALT)) || (c == 'p')
                        || ((flags & FL_SIGNEDCONV) && ( *s1 == '+' || *s1 == '-' || *s1 == ' '))))
                        between_fill++;

                if ((i = width - j) > 0)
                {
                        if (!(flags & FL_LJUST))
                        {      /* right justify */
                                nrchars += i;
                                if (between_fill) 
                                {
					if (flags & FL_SIGNEDCONV) 
					{
	                                        j--; nrchars++;
						vbuf = putc(*s1++, vbuf);
					}
					else 
					{
	                                        j -= 2; nrchars += 2;
						vbuf = putc(*s1++, vbuf);
						vbuf = putc(*s1++, vbuf);
					}
                                }
                                do 
                                {
					vbuf = putc(zfill, vbuf);
                                } while (--i);
                        }
		}
		
                nrchars += j;
                while (--j >= 0)	vbuf = putc(*s1++, vbuf);

                if (i > 0) nrchars += i;
                while (--i >= 0)	vbuf = putc(zfill, vbuf);
        }

        return nrchars;
}

#ifdef DEBUG
int db_printf(const char *format, ...)
{
	int retval;
	retval = printf(format);
	return retval;
}
#endif

int mprintf(const char *format, ...)
{
	int retval;
	retval = printf(format);
	return retval;
}


int v_printf(char* vbuf, const char *format, ...)
{
	va_list ap;
	int retval;
	va_start(ap, format);
	retval = _doprnt(format, ap, vbuf);
	va_end(ap);
	return retval;
}
