#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libraries
#include "../deps/ini.h"


struct ConfigurationStruct
{
	char* key_file_path;
	char* iv_file_path;
};

static inline int ini_config_parse_handler(void* user, const char* section, const char* name, const char* value)
{
	struct ConfigurationStruct* pconfig = (struct ConfigurationStruct*)user;

	size_t lenght = strlen(value);

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("app", "key_file_path"))
		pconfig->key_file_path=strdup(value);
	else if (MATCH("app", "iv_file_path"))
		pconfig->iv_file_path=strdup(value);
	else
		return 0;  /* unknown section/app, error */

	return 1;
}



