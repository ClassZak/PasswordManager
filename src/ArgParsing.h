#pragma once
#include <stdio.h>

enum ArgParsingResult
{
	Default_ParsingResult,
	Error_ParsingResult,
	Help_ParsingResult
};


static inline int ParseArgs(int argc, char** argv)
{
#ifdef _DEBUG
	for(int i = 0; i != argc; ++i)
		printf("%s\n",argv[i]);
#endif

	return Default_ParsingResult;
}


