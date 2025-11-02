#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void print_with_color(const char* format, int color, ...);
static inline void scan_string(char* buffer,size_t size)
{
	if(!size)
		return;

	if(!fgets(buffer,size,stdin))
		if(feof(stdin))
			return;
	
	size_t str_len=strlen(buffer);
	(buffer)[size-1]='\0';
	
	if(str_len)
		buffer[str_len-1]='\0';
}
static inline void scan_long_string(char* buffer)
{
	scan_string(buffer, 1 << sizeof(size_t));
}
static inline void pause_prompt()
{
#ifdef _WIN32
	system("pause");
#elif defined __unix__
	printf("Press the \"Enter\" key to continue\n");
	getchar();
#endif
}





