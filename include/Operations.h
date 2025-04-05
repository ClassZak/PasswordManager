#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "PasswordStruct.h"

void print_with_color(const char* format, int color, ...);
inline void handle_char_input_error()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	print_with_color("Ошибка ввода!\n",91);
}
inline void print_passwords(struct PasswordStruct* passwords, size_t size)
{
	for(size_t i=0;i!=size;++i)
		printf
		(
			"Название:\t%s\n\
Описание:\t%s\n\
Логин:\t\t%s\n\
Пароль:\t\t%s\n\
\n",
			passwords[i].name, passwords[i].description, passwords[i].login, passwords[i].password
		);
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
inline void CLS(){system("cls"); };
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
inline struct PasswordStruct* GetAllPasswords(FILE** file, size_t* size)
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
/// Каждый параметр проверяется отдельно в соответсвии с тем, активен нужный флвг или нет.
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
