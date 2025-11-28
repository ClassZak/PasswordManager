#pragma once
#include <string.h>
#include "ShowHelp.h"
#include "../Functions.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#endif // _WIN32

enum ArgParsingResult
{
	Default_ParsingResult,
	Error_ParsingResult,
	Help_ParsingResult,
	Version_ParsingResult,
};


static inline int ParseArgs(int argc, char** argv)
{
#ifdef _DEBUG
	for(int i = 0; i != argc; ++i)
		printf("%s\n",argv[i]);
#endif
	if (argc==1)
		return Default_ParsingResult;

	if (argc!=2)
		return Error_ParsingResult;
	
	if (!strcmp(argv[1], "-v") || !(strcmp(argv[1], "--version")))
		return Version_ParsingResult;
	else if(!strcmp(argv[1], "-h") || !(strcmp(argv[1], "--help")))
		return Help_ParsingResult;
	else
		return Error_ParsingResult;
}





