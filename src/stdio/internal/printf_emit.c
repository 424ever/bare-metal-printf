#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "printf_impl.h"

void __putchar_emit(void *cookie, char c)
{
	(void) cookie;
	putchar(c);
}

void __strappend_emit(void *cookie, char c)
{
	sprintf_cookie *_cookie;

	_cookie = (sprintf_cookie *) cookie;

	if (!_cookie->limited)
	{
		_cookie->base[_cookie->i] = c;
		++_cookie->i;
	}
	else if (_cookie->remaining > 1)
	{
		--_cookie->remaining;
		_cookie->base[_cookie->i] = c;
		++_cookie->i;
	}

	_cookie->base[_cookie->i] = '\0';
}

void __emit_string(printf_emit emit, const char *str)
{
	size_t i;
	size_t len;

	len = strlen(str);

	for (i = 0; i < len; ++i)
		emit.emit(emit.cookie, str[i]);
}

void __emit_char(printf_emit emit, char c)
{
	emit.emit(emit.cookie, c);
}
