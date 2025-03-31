#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "PasswordStruct.h"

void print_with_color(const char* format, int color, ...);


#define COMMAND_EXIT						0
#define COMMAND_SHOW_COMMAND_LIST			1
#define COMMAND_CLS							2
#define COMMAND_ADD_NEW						3
#define COMMAND_DELETE_PASSWORD				4
#define COMMAND_DELETE_PASSWORD_BY_NAME		5
#define COMMAND_DELETE_PASSWORD_BY_LOGIN	6
#define COMMAND_FIND_PASSWORDS				7
#define COMMAND_SHOW_ALL_PASSWORDS			8

#define COMMNAD_FIRST	COMMAND_EXIT
#define COMMNAD_LAST	COMMAND_SHOW_ALL_PASSWORDS
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
int AddNewPasswordStructEmplace(struct PasswordStruct*** array, size_t* arraySize, struct PasswordStruct* newElement);



void Dialog(FILE** file);

void ShowCommandList();
inline void CLS(){system("cls"); };
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - �����
/// 1 - ������ ������ � ����
/// 2 - ������ ����� ������
/// </returns>
int AddNewPassword(FILE** file, struct PasswordStruct* password_struct);
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - �����
/// 1 - ������ ����������� �����
/// 2 - ������ ����� ������
/// 3 - �� ������� ������ ��� ��������
/// </returns>
int DeletePassword(FILE** file, struct PasswordStruct* password_struct);
int DeletePasswordByName(FILE** file, const char* name);
int DeletePasswordByLogin(FILE** file, const char* login);
inline struct PasswordStruct* GetAllPasswords(FILE** file, size_t* size)
{
	return ReadAllPasswordStructs(file, size);
}

#define PASSWOR_STRUCT_FIND_BY_NAME			0b10000
#define PASSWOR_STRUCT_FIND_BY_LOGIN		0b01000
#define PASSWOR_STRUCT_FIND_BY_PASSWORD		0b00100
#define PASSWOR_STRUCT_FIND_BY_DESCRIPTION	0b00010
#define PASSWOR_STRUCT_FIND_CONTAINS		0b00001
/// <summary>
/// ������� ������ �������
/// </summary>
/// <param name="params">
/// ������ ���������� ������
/// ������ �������� ����������� �������� � ����������� � ���, ������� ������ ���� ��� ���.
/// ��������, ����� ��������� ����� ������ �� ����� � ������.
/// ����� 5 ��������� ������� ��� ������, � ��������� ���������� ������� �����
/// ����������� � ������� ��������������� �������� ��� ������.
/// </param>
/// <param name="mode">
/// 0 - ������ ���������� �� ����� �����������
/// 16 - ���������� �� �����
/// 8 - ���������� �� ������
/// 4 - ���������� �� ������
/// 2 - ���������� �� ��������
/// 1 - �������� ������ ���������
/// </param>
/// <returns></returns>
struct PasswordStruct* FindPasswords(struct PasswordStruct* params, int mode);
