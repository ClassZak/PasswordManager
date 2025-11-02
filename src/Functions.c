#include "Functions.h"
#ifdef _WIN32
#include <windows.h>
#endif

void print_with_color(const char* format, int color, ...)
{
#ifndef _WIN32
	printf("\x1b[%dm", color);
#else
	static HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_handle, color);
#endif

	va_list args;
	va_start(args, color);
	vprintf(format, args);
	va_end(args);

#ifndef _WIN32
	printf("\x1b[0m");
#else
	SetConsoleTextAttribute(console_handle, FOREGROUND_INTENSITY - 1);
#endif
}
