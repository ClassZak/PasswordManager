#include "Functions.h"

void print_with_color(const char* format, int color, ...)
{
	printf("\x1b[%dm", color);

	va_list args;
	va_start(args, color);
	vprintf(format, args);
	va_end(args);

	printf("\x1b[0m");
}

