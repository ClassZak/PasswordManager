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
struct PasswordStruct* parse_password_structs(const void* buf, size_t* offset)
{
	struct PasswordStruct* passwords;

}


