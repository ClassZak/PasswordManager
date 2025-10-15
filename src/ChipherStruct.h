#pragma once

struct ChipherStruct
{
	unsigned long key_size;
	unsigned char* key;

	unsigned long iv_size;
	unsigned char* iv;
};

