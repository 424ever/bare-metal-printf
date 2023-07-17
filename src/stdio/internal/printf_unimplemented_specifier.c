#include "printf_impl.h"

#ifdef DEBUG
void __printf_unimplemented_specifier(char specifier, printf_emit emit)
{
	__emit_string(emit, "<unimplemented:");
	emit.emit(emit.cookie, specifier);
	__emit_string(emit, "> ");
}
#else
void __printf_unimplemented_specifier(char specifier, printf_emit emit)
{
	(void) specifier;
	(void) emit;
}
#endif
