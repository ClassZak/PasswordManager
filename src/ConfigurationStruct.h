#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libraries
#include "../deps/ini.h"


struct ConfigurationStruct
{
	char* cipher_file_path;
};

static inline int ini_config_parse_handler(void* user, const char* section, const char* name, const char* value)
{
	struct ConfigurationStruct* pconfig = (struct ConfigurationStruct*)user;

	size_t lenght = strlen(value);

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("app", "cipher_file_path"))
	{
		pconfig->cipher_file_path = malloc(lenght+1);
		strcpy(pconfig->cipher_file_path, value);
	}
	else
		return 0;  /* unknown section/app, error */

	pconfig->cipher_file_path[lenght] = '\0';

	return 1;
}



