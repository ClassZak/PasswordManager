#pragma once
#include <stdio.h>
#include <stdlib.h>


#ifndef PASSWORD_FILE 
#define PASSWORD_FILE ".Passwords.bin"
#endif // !PASSWORD_FILE 

struct PasswordStruct
{
	char description[200];

	char login[100];
	char password[100];

	char name[50];
};

long getFileSize(FILE** file);

int ReadPasswordStruct
(
	FILE** file,
	struct PasswordStruct* password_struct
);
int WritePasswordStruct
(
	FILE** file,
	struct PasswordStruct* password_struct
);

struct PasswordStruct* ReadAllPasswordStructs(FILE** file, size_t* size);
