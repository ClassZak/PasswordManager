#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "PasswordStruct.h"

void print_with_color(const char* format, int color, ...);
static inline void handle_char_input_error()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	print_with_color("Ошибка ввода!\n",91);
}
static inline void scan_string(char* buffer,size_t size)
{
	if(!size)
		return;

	fgets(buffer,size,stdin);
	size_t str_len=strlen(buffer);
	(buffer)[size-1]='\0';

	if(str_len)
		buffer[str_len-1]='\0';
}
static inline void print_passwords(struct PasswordStruct* passwords, size_t size)
{
	char last_char='\0';
	size_t string_length=0;

	for (size_t i = 0; i != size; ++i)
	{
		printf("Название:\t");
		string_length=strlen(passwords[i].name);
		if(!string_length)
			printf("\n");
		if
		(
			passwords[i].name[string_length - 1] == ' ' ||
			passwords[i].name[string_length - 1] == '\t' || 
			passwords[i].name[string_length - 1] == '\n'
		)
			printf("\"%s\"\n",passwords[i].name);
		else
			printf("%s\n", passwords[i].name);
			
		printf("Описание:\t");
		string_length=strlen(passwords[i].description);
		if(!string_length)
			printf("\n");
		if
			(
				passwords[i].description[string_length - 1] == ' ' ||
				passwords[i].description[string_length - 1] == '\t' ||
				passwords[i].description[string_length - 1] == '\n'
				)
		{
			print_with_color("\"", 36);
			printf("\"%s\"\n",passwords[i].description);
			print_with_color("\"", 36);
		}
		else
			printf("%s\n", passwords[i].description);

		printf("Логин:\t\t");
		string_length=strlen(passwords[i].login);
		if(!string_length)
			printf("\n");
		if
			(
				string_length &&
				(
					passwords[i].login[string_length - 1] == ' ' ||
					passwords[i].login[string_length - 1] == '\t' ||
					passwords[i].login[string_length - 1] == '\n'
					)
				)
		{
			print_with_color("\"", 36);
			printf("%s\n",passwords[i].login);
			print_with_color("\"", 36);
		}
		else
			printf("%s\n", passwords[i].login);
			
		printf("Пароль:\t\t");
		string_length=strlen(passwords[i].password);
		if(!string_length)
			printf("\n");
		if
		(
			passwords[i].password[string_length - 1] == ' ' ||
			passwords[i].password[string_length - 1] == '\t' || 
			passwords[i].password[string_length - 1] == '\n'
		)
		{
			print_with_color("\"", 36);
			printf("%s\n", passwords[i].password);
			print_with_color("\"", 36);
		}
		else
			printf("%s\n", passwords[i].password);

		printf("\n");
	}
}


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
/// 0 - успешное добавление
/// 1 - ошибка выделения памяти
/// 2 - выбран неверный указатель на массив
/// 3 - выбран неверный указатель на длину массива
/// 4 - выбран неверный указатель на новый элемент
/// </returns>
int AddNewPasswordStructEmplace(struct PasswordStruct*** array, size_t* arraySize, struct PasswordStruct* newElement);



void Dialog(FILE** file);

void ShowCommandList();
static inline void CLS()
{
	system("cls");
};
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - успех
/// 1 - ошибка записи в файл
/// </returns>
int AddNewPassword(FILE** file, struct PasswordStruct* password_struct);
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - успех
/// 1 - ошибка перезаписи файла
/// 2 - ошибка чтения файла
/// 3 - не найдены пароли для удаления
/// </returns>
int DeletePassword(FILE** file, struct PasswordStruct* password_struct);
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - успех
/// 1 - ошибка перезаписи файла
/// 2 - ошибка чтения файла
/// 3 - не найдены пароли для удаления
/// </returns>
int DeletePasswordByName(FILE** file, const char* name);
/// <summary>
/// 
/// </summary>
/// <param name="file"></param>
/// <param name="password_struct"></param>
/// <returns>
/// 0 - успех
/// 1 - ошибка перезаписи файла
/// 2 - ошибка чтения файла
/// 3 - не найдены пароли для удаления
/// </returns>
int DeletePasswordByLogin(FILE** file, const char* login);
static inline struct PasswordStruct* GetAllPasswords(FILE** file, size_t* size)
{
	return ReadAllPasswordStructs(file, size);
}

#define PASSWORD_STRUCT_FIND_BY_NAME		0b10000
#define PASSWORD_STRUCT_FIND_BY_LOGIN		0b01000
#define PASSWORD_STRUCT_FIND_BY_PASSWORD	0b00100
#define PASSWORD_STRUCT_FIND_BY_DESCRIPTION	0b00010
#define PASSWORD_STRUCT_FIND_CONTAINS		0b00001
/// <summary>
/// Функция поиска паролей
/// </summary>
/// <param name="params">
/// Список параметров поиска
/// Каждый параметр проверяется отдельно в соответствии с тем, активен нужный флаг или нет.
/// Например, можно выполнить поиск только по имени и логину.
/// Режим 5 позволяет вывести все записи, в выбранных параметрах которых будут
/// содержаться в строках соответствующие значения для поиска.
/// </param>
/// <param name="mode">
/// 0 - Точное совпадение со всеми параметрами
/// 16 - Совпадение по имени
/// 8 - Совпадение по логину
/// 4 - Совпадение по паролю
/// 2 - Совпадение по описанию
/// 1 - Содержит внутри параметра
/// </param>
/// <returns>
/// 0 - успех
/// 1 - ошибка чтения файла
/// 2 - файл с записями пуст
/// 3 - записей не найдено
/// </returns>
int FindPasswords
(
	FILE** file,
	struct PasswordStruct* params,
	struct PasswordStruct** founded_passwords,
	size_t* founded_passwords_quantity,
	int flags
);
