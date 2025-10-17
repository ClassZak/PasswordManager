#include "FileFunctions.h"
#include <openssl/evp.h>
#include <openssl/err.h>

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
	*size = 0;
	long file_size = get_file_size(&file);
	if(file_size == -1)
	{
		fclose(file);
		return NULL;
	}
	
	void* data = malloc(file_size+1);
	if(!data)
		return NULL;
		
	size_t recieved = fread(data, 1, file_size, file);

	//printf("\"%s\"\n%d\n\n",(char*)data,recieved);

	fclose(file);
	if(recieved!=file_size)
		return NULL;
	else
	{
		*size = recieved;
		return data;
	}
}


//TODO: переделать

//Расшифровка данных из файла
void* decrypt_buffer(void* input, size_t size, size_t* out_size, struct ChipherStruct* chipher)
{
	if(!size
	//||size < EVP_CIPHER_CTX_get_block_size(EVP_aes_256_cbc())
	)
	{
		*out_size = 0;
		return NULL;
	}

	EVP_CIPHER_CTX* ctx;
	int len;
	unsigned char* plaintext;
	int plaintext_len;

	if(!(ctx = EVP_CIPHER_CTX_new()))
	{
		ERR_print_errors_fp(stderr);
		return NULL;
	}

	if(1!=EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, chipher->key, chipher->iv))
	{
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}

	plaintext = malloc(size + EVP_CIPHER_CTX_block_size(ctx));
	if(!plaintext)
	{
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}

	if(1!=EVP_DecryptUpdate(ctx, plaintext, &len, input, size))
	{
		free(plaintext);
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	plaintext_len=len;

	if(1!=EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
	{
		free(plaintext);
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	plaintext_len+=len;

	EVP_CIPHER_CTX_free(ctx);

	*out_size = plaintext_len;
	return plaintext;
}

//Парсинг полученных данных
struct PasswordStruct* parse_password_structs(const void* buf, size_t data_size, size_t* out_size)
{
	struct PasswordStruct* passwords = NULL;
	size_t array_size=0;
	struct PasswordStruct curr_password = {0,NULL,0,NULL,0,NULL,0,NULL};
	size_t offset = 0;
	char* pointer = (char*)buf;
	while(offset != data_size)
	{
		curr_password.name_size					= *(unsigned long*)(pointer);
		curr_password.name						= malloc(curr_password.name_size+1);
		if (!curr_password.name)
			return NULL;
		memcpy(curr_password.name, pointer + sizeof(unsigned long), curr_password.name_size);
		curr_password.name[curr_password.name_size]='\0';
		offset += curr_password.name_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.description_size			= *(unsigned long*)(pointer);
		curr_password.description				= malloc(curr_password.description_size+1);
		if (!curr_password.description)
			return NULL;
		memcpy(curr_password.description, pointer + sizeof(unsigned long), curr_password.description_size);
		curr_password.description[curr_password.description_size]='\0';
		offset += curr_password.description_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.login_size				= *(unsigned long*)(pointer);
		curr_password.login						= malloc(curr_password.login_size+1);
		if (!curr_password.login)
			return NULL;
		memcpy(curr_password.login, pointer + sizeof(unsigned long), curr_password.login_size);
		curr_password.login[curr_password.login_size]='\0';
		offset += curr_password.login_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		curr_password.password_size				= *(unsigned long*)(pointer);
		curr_password.password					= malloc(curr_password.password_size+1);
		if (!curr_password.password)
			return NULL;
		memcpy(curr_password.password, pointer + sizeof(unsigned long), curr_password.password_size);
		curr_password.password[curr_password.password_size]='\0';
		offset += curr_password.password_size + sizeof(unsigned long);
		pointer = (char*)buf + offset;
		
		int res = AddNewPasswordStruct(&passwords, &array_size, &curr_password);
		if(res)
			printf("error\n");
	}

	*out_size = array_size;
	return passwords;
}

//Запись в файл 0 - успех
int write_file(const char* filename, const char* modes, const char* data, size_t data_size)
{
	FILE* file = fopen(filename, modes);
	if(!file)
		return EXIT_FAILURE;
	
	size_t res = fwrite(data, sizeof(char), data_size, file);
	fclose(file);
	if(res != data_size)
		return EXIT_FAILURE;
		
	
	return EXIT_SUCCESS;
}
//Шифрование данных
void* encrypt_buffer(void* input, size_t size, size_t* out_size, struct ChipherStruct* chipher)
{
	if(!size
	//||size < EVP_CIPHER_CTX_get_block_size(EVP_aes_256_cbc())
	)
	{
		*out_size = 0;
		return NULL;
	}

	EVP_CIPHER_CTX *ctx;
	int len;
	unsigned char* ciphertext;
	int ciphertext_len;
	
	if(!(ctx = EVP_CIPHER_CTX_new()))
	{
		ERR_print_errors_fp(stderr);
		return NULL;
	}
	
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, chipher->key, chipher->iv))
	{
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	
	ciphertext = malloc(size + EVP_CIPHER_CTX_block_size(ctx));
	if (!ciphertext)
	{
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, input, size))
	{
		ERR_print_errors_fp(stderr);
		free(ciphertext);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	ciphertext_len = len;
	
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
	{
		ERR_print_errors_fp(stderr);
		free(ciphertext);
		EVP_CIPHER_CTX_free(ctx);
		return NULL;
	}
	ciphertext_len += len;
	
	EVP_CIPHER_CTX_free(ctx);
	*out_size = ciphertext_len;
	return ciphertext;
}
//Преобразование данных в буфер символов
void* deparse_password_structs(const struct PasswordStruct* passwords, size_t count, size_t* out_size)
{
	size_t size = *out_size = 0;
	for(size_t i = 0; i != count;++i)
	{
		size += sizeof(	passwords[i].name_size);
		size += 		passwords[i].name_size;
		size += sizeof(	passwords[i].description_size);
		size += 		passwords[i].description_size;
		size += sizeof(	passwords[i].login_size);
		size += 		passwords[i].login_size;
		size += sizeof(	passwords[i].password_size);
		size +=			passwords[i].password_size;
	}
	if(!size)
		return NULL;
	char* buffer = (char*)malloc(size + 1);
	if(!buffer)
		return NULL;
	buffer[size - 1] = '\0';

	char* pointer = buffer;
	for(size_t i = 0; i != count; ++i)
	{
		memcpy(pointer, &passwords[i].name_size, sizeof(passwords[i].name_size));
		pointer += sizeof(passwords[i].name_size);
		memcpy(pointer, passwords[i].name, passwords[i].name_size);
		pointer += passwords[i].name_size;

		memcpy(pointer, &passwords[i].description_size, sizeof(passwords[i].description_size));
		pointer += sizeof(passwords[i].description_size);
		memcpy(pointer, passwords[i].description, passwords[i].description_size);
		pointer += passwords[i].description_size;

		memcpy(pointer, &passwords[i].login_size, sizeof(passwords[i].login_size));
		pointer += sizeof(passwords[i].login_size);
		memcpy(pointer, passwords[i].login, passwords[i].login_size);
		pointer += passwords[i].login_size;

		memcpy(pointer, &passwords[i].password_size, sizeof(passwords[i].password_size));
		pointer += sizeof(passwords[i].password_size);
		memcpy(pointer, passwords[i].password, passwords[i].password_size);
		pointer += passwords[i].password_size;
	}

	*out_size = size;
	return (void*)buffer;
}

