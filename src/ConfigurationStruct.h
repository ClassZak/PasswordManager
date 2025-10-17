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

static inline int ini_config_parse_handler(void* config, const char* section, const char* name, const char* value)
{
	struct ConfigurationStruct* pconfig = (struct ConfigurationStruct*)config;

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

// Checking config
// Return codes:
// 0 - correct
// 1 - NULL pointer
// 2 - no key_file_path
// 3 - no iv_file_path
static inline int check_config(struct ConfigurationStruct* config) 
{
	if(!config)
		return 1;
	if(!config->key_file_path)
		return 2;
	if(!config->iv_file_path)
		return 3;

	return EXIT_SUCCESS;
}



