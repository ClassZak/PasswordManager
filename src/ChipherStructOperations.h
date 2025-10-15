#pragma once
#include "ChipherStruct.h"

static inline int load_chipher_struct(struct ChipherStruct* chipher, struct ConfigurationStruct* config)
{
	if(!chipher)
		chipher = (struct ChipherStruct*)malloc(sizeof(struct ChipherStruct));
	if(!config)
		return EXIT_FAILURE;
	
	chipher->key =	(unsigned char*)read_file((const char*)config->key_file_path, (size_t*)&chipher->key_size);
	chipher->iv =	(unsigned char*)read_file((const char*)config->iv_file_path, (size_t*)&chipher->iv_size);
	if(!chipher->key)
	{
		print_with_color("Failed to load key of chipher",92);
		return EXIT_FAILURE;
	}
	if(!chipher->iv)
	{
		print_with_color("Failed to load iv of chipher",92);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

