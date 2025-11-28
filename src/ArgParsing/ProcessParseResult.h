#pragma once
#include "ArgParsing.h"

static inline void ProcessParseResult(enum ArgParsingResult pasrsing_result)
{
	if (pasrsing_result != Default_ParsingResult)
	{
		switch (pasrsing_result)
		{
			case Version_ParsingResult:
			{
				printf("1.0.0\n");
				break;
			}
			case Help_ParsingResult:
			{
				ShowHelp();
				break;
			}
			default:
			{
				print_with_color("Error of argument parsing",31);
				ShowHelp();
				exit(EXIT_FAILURE);
			}
		}
		
		exit(EXIT_SUCCESS);
	}
}
