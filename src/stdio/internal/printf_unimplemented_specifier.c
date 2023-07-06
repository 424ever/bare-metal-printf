#include "printf_impl.h"

#ifdef DEBUG
void _printf_unimplemented_specifier(char specifier, emit_func emit,
				     void *cookie)
{
	emit_string(emit, cookie, "<unimplemented:");
	emit(cookie, specifier);
	emit_string(emit, cookie, "> ");
}
#else
void _printf_unimplemented_specifier(char specifier, emit_func emit,
				     void *cookie)
{
	(void) specifier;
	(void) emit;
	(void) cookie;
}
#endif
