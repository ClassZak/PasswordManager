#pragma once
#include <stdio.h>

#define HELP_STRING R"(Usage: PasswordManager [OPTION]

Options:
  -h, --help    for display this message
  -v, --version for show version

)"

static inline void ShowHelp()
{
	printf(HELP_STRING);
}
