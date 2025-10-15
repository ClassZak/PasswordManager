#pragma once
#include "PasswordStruct.h"
#include "ConfigurationStruct.h"
#include "ChipherStruct.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/err.h>

#define FILE_ERROR_READING   2
#define FILE_ERROR_NOT_FOUND 3
#define FILE_ERROR_EMPTY     4

//Получение размера файла -1 - ошибка
long get_file_size(FILE** file);

//Чтение данных из файла
void* read_file(const char* filename, size_t* size);
//Расшифровка данных из файла
void* decrypt_buffer(void* input, size_t size, size_t* out_size);
//Парсинг полученных данных
struct PasswordStruct* parse_password_structs(const void* buf, size_t data_size, size_t* out_size);

//Запись в файл 0 - успех
int write_file(const char* filename, const char* modes, const char* data, size_t data_size);
//Шифрование данных
void* encrypt_buffer(void* input, size_t size, size_t* out_size, struct ChipherStruct* chipher);
//Преобразование данных в буфер символов
void* deparse_password_structs(const struct PasswordStruct* passwords, size_t count, size_t* out_size);

//Получение всех структур паролей
static inline int GetAllPasswordStructs(struct PasswordStruct** passwords, size_t* size, const char* filename)
{
	*size = 0;
	size_t data_size;
	void* data = read_file(filename, &data_size);
	if(!data)
		return EXIT_FAILURE;

	size_t decrypted_data_size;
	void* decrypted_data = decrypt_buffer(data, data_size, &decrypted_data_size);
	if (!decrypted_data)
		return EXIT_FAILURE;

	size_t passwords_count;
	struct PasswordStruct* parsed_passwords = parse_password_structs(decrypted_data, decrypted_data_size, &passwords_count);
	if (!parsed_passwords)
		return EXIT_FAILURE;
	
	*passwords = parsed_passwords;
	*size = passwords_count;
	
	return EXIT_SUCCESS;
}

//Запись всех структур паролей
static inline int WriteAllPasswordStructs(const char* filename, struct PasswordStruct* passwords, size_t passwords_count, struct ChipherStruct* chipher)
{
	size_t deparsed_data_size = 0;
	void* deparsed_data = deparse_password_structs(passwords, passwords_count, &deparsed_data_size);
	size_t encrypt_data_size = 0;
	void* encrypt_data = encrypt_buffer(deparsed_data, deparsed_data_size, &encrypt_data_size, chipher);

	if (write_file(filename, "w", (const char*)encrypt_data,encrypt_data_size))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

