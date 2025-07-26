#include "FileFunctions.h"

//Получение размера файла -1 - ошибка
long get_file_size(FILE** file)
{
	if(!*file)
		return -1;
	
	long size, prev_pos = ftell(*file);
	
	fseek(*file, 0, SEEK_END);
	size = ftell(*file);
	fseek(*file, prev_pos,SEEK_SET);
	
	return size;
}

//Чтение данных из файла
void* read_file(const char* filename, size_t* size)
{
	FILE* file = fopen(filename,"rb");
	long file_size = get_file_size(&file);
	if(file_size == -1)
		return NULL;
	
	void* data = malloc(file_size);
	unsigned int recieved = fread(data, file_size, 1, file);
	if(recieved!=file_size)
		return NULL;
	else
		return data;
}


//TODO: переделать

//Расшифровка данных из файла
void* decrypt_buffer(void* input, size_t size, size_t* out_size)
{
	*out_size=size;
	void* decrypted_data = malloc(size);
	memcpy(decrypted_data,input,size);

	return decrypted_data;
}

//Парсинг полученных данных
struct PasswordStruct* parse_password_structs(const void* buf, size_t data_size)
{
	struct PasswordStruct* passwords = NULL;
	size_t array_size;
	struct PasswordStruct curr_password;
	size_t offset = 0;
	char* pointer = (char*)buf;
	while(offset != data_size)
	{
		curr_password.name_size					= *(unsigned long*)(pointer);
		curr_password.name						= malloc(curr_password.name_size+1);
		memcpy(curr_password.name, pointer + sizeof(unsigned long), curr_password.name_size);
		curr_password.name[curr_password.name_size]='\0';
		offset += curr_password.password_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.description_size			= *(unsigned long*)(pointer);
		curr_password.description				= malloc(curr_password.description_size+1);
		memcpy(curr_password.description, pointer + sizeof(unsigned long), curr_password.description_size);
		curr_password.description[curr_password.description_size]='\0';
		offset += curr_password.password_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.login_size				= *(unsigned long*)(pointer);
		curr_password.login						= malloc(curr_password.login_size+1);
		memcpy(curr_password.login, pointer + sizeof(unsigned long), curr_password.login_size);
		curr_password.login[curr_password.login_size]='\0';
		offset += curr_password.password_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.password_size				= *(unsigned long*)(pointer);
		curr_password.password					= malloc(curr_password.password_size+1);
		memcpy(curr_password.password, pointer + sizeof(unsigned long), curr_password.password_size);
		curr_password.password[curr_password.password_size]='\0';
		offset += curr_password.password_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		int res = AddNewPasswordStruct(&passwords, &array_size, &curr_password);
		if(res)
			printf("error\n");
	}

	
	return passwords;
}


