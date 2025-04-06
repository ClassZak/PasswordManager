#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define PASSWORD_STRUCT_NAME_SIZE			50
#define PASSWORD_STRUCT_DESCRIPTION_SIZE	200
#define PASSWORD_STRUCT_LOGIN_SIZE			100
#define PASSWORD_STRUCT_PASSWORD_SIZE		100

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
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <param name="size"></param>
/// <returns>
/// 0 - успех
/// 1 - ошибка
/// </returns>
int WritePasswordStructs
(
	FILE** file,
	struct PasswordStruct* password_struct,
	size_t size
);

struct PasswordStruct* ReadAllPasswordStructs(FILE** file, size_t* size);
/// <summary>
/// 
/// </summary>
/// <param name="array"></param>
/// <param name="arraySize"></param>
/// <param name="newElement"></param>
/// <returns>
/// 0 - успешное добавление
/// 1 - ошибка выделения памяти
/// 2 - выбран неверный указатель на массив
/// 3 - выбран неверный указатель на длину массива
/// 4 - выбран неверный указатель на новый элемент
/// </returns>
int AddNewPasswordStruct(struct PasswordStruct** array,size_t* arraySize, struct PasswordStruct* newElement);
