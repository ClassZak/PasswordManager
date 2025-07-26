#pragma once
#include "PasswordStruct.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


//Получение размера файла -1 - ошибка
long get_file_size(FILE** file);
//Чтение данных из файла
void* read_file(const char* filename, size_t* size);
//Расшифровка данных из файла
void* decrypt_buffer(void* input, size_t size, size_t* out_size);
//Парсинг полученных данных
struct PasswordStruct* parse_password_structs(const void* buf, size_t data_size);

