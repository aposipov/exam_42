#include <unistd.h>
#include <stdarg.h>

char *readint(char *f, int *w)
{
	*w = 0;
	while ('0' <= *f && *f <= '9')
		*w = *w * 10 + *f++ - '0';
	return (f);
}

int rpt(char c, int cnt)
{
	int rv = 0;
	while (rv < cnt)
		rv += write(1, &c, 1);
	return (rv);
}

int putstr(char *s, int w, int p, char *nullstr)
{
	int l = 0;
	s = s ? s : nullstr;
	while (s[l] && (p > -1 ? (l < p) : 1))
		l++;
	return (rpt(' ', w - l) + write(1, s, l));
}

int intlen(long n, int b)
{
	int l = 0;
	while (n && ++l)
		n /= b;
	return (l ? l : 1);
}

int putb(long n, char *s, int b)
{
	int rv = n <= -b || n >= b ? putb(n / b, s, b) : 0;
	return (rv + write(1, n < 0 ? (s - n % b) : (s + n % b), 1));
}

int put(long n, int w, int p, char *s, int b)
{
	int l = intlen(n, b);
	int pl = l < p || (!p && !n) ? p : l;
	int rv = rpt(' ', w - pl - (n < 0));
	rv += n < 0 ? write(1, "-", 1) : 0;
	rv += rpt('0', pl - l);
	rv += pl ? putb(n, s, b) : 0;
	return (rv);
}

int ft_printf(const char *f, ...)
{
	va_list ap;
	int rv = 0, w, p;

	va_start(ap, f);
	while (*f)
		if (*f == '%')
		{
			f = readint((char *)++f, &w);
			p = -1;
			f = *f == '.' ? readint((char *)++f, &p) : f;
			rv += *f == 's' ? putstr(va_arg(ap, char *), w, p, "(null)") : 0;
			rv += *f == 'd' ? put((long)va_arg(ap, int), w, p, "0123456789", 10) : 0;
			rv += *f++ == 'x' ? put((long)va_arg(ap, unsigned int), w, p, "0123456789abcdef", 16) : 0;
		}
		else
			rv += write(1, f++, 1);
	va_end(ap);
	return (rv);
}