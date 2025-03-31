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
/// <summary>
/// 
/// </summary>
/// <param name="array"></param>
/// <param name="arraySize"></param>
/// <param name="newElement"></param>
/// <returns>
/// 0 - �������� ����������
/// 1 - ������ ��������� ������
/// 2 - ������ �������� ��������� �� ������
/// 3 - ������ �������� ��������� �� ����� �������
/// 4 - ������ �������� ��������� �� ����� �������
/// </returns>
int AddNewPasswordStruct(struct PasswordStruct** array,size_t* arraySize, struct PasswordStruct* newElement);
