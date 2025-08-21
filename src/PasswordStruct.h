#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileFunctions.h"

#ifndef PASSWORD_FILE 
#define PASSWORD_FILE ".Passwords.bin"
#endif // !PASSWORD_FILE 

struct PasswordStruct
{
	unsigned long name_size;
	char* name;

	unsigned long description_size;
	char* description;

	unsigned long login_size;
	char* login;

	unsigned long password_size;
	char* password;
};
void FreePasswordStruct(struct PasswordStruct* password_struct);
void FreePasswordStructs(struct PasswordStruct* password_structs, size_t count);

int AddNewPasswordStruct(struct PasswordStruct** array, size_t* arraySize, struct PasswordStruct* newElement);



