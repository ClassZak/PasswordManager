#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else 
#ifdef __unix__
#include <errno.h>
#include <unistd.h>
#endif
#endif

#include "PasswordStruct.h"
#include "Operations.h"

#ifndef PASSWORD_FILE 
#define PASSWORD_FILE ".Passwords.bin"
#endif // !PASSWORD_FILE 

// Libraries
#include "../deps/ini.h"




// Config
#include "ConfigurationStruct.h"
// Chipher
#include "ChipherStruct.h"
#include "ChipherStructOperations.h"


int TryToCreateFile(const char** filename) // 0 - успех. Иначе - код ошибки
{
#ifdef _WIN32
	HANDLE hFile = CreateFileA
	(
		(LPCSTR)filename,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
#elif defined __unix__
	FILE* file = fopen((const char*)filename, "w");
#endif

#ifdef _WIN32
	if (hFile == INVALID_HANDLE_VALUE)
#elif defined __unix__
	if (!file)
#endif
	{
		int last_error;
#ifdef _WIN32	
		last_error = GetLastError();
#elif defined __unix__
		last_error = errno;
#endif
		return last_error;
	}

#ifdef _WIN32	
	CloseHandle(hFile);
#elif defined __unix__
	fclose(file);
#endif

	return EXIT_SUCCESS;
}

void CheckPasswordStorage()
{
	bool file_exists = true;

#ifdef _WIN32
	HANDLE hFile = CreateFileA
	(
		PASSWORD_FILE,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		printf("Ошибка при открытии файла-хранилища \"%s\"\n", PASSWORD_FILE);
		printf("Ошибка: ");

		switch (error)
		{
			case ERROR_FILE_NOT_FOUND:
			{
				file_exists = false;
				printf("файл не найден\n");
				break;
			}
			case ERROR_PATH_NOT_FOUND:
			{
				file_exists = false;
				printf("путь не найден\n");
				break;
			}
			case ERROR_ACCESS_DENIED:
			{
				printf("нет доступа\n");
				break;
			}
			case ERROR_SHARING_VIOLATION:
			{
				printf("файл занят другим процессом");
				break;
			}
			default:
			{
				printf("%d", error);
				break;
			}
		}
		switch (error)
		{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				break;
			case ERROR_ACCESS_DENIED:
			case ERROR_SHARING_VIOLATION:
				exit(error);
			default:
			{
				exit(error);
				break;
			}
		}
	}
	CloseHandle(hFile);
#elif defined __unix__
	FILE* file = fopen(PASSWORD_FILE, "rb");
	if (!file)
	{
		printf("Ошибка при открытии файла-хранилища \"%s\"\n", PASSWORD_FILE);
		printf("Ошибка: ");
		switch (errno)
		{
			case ENOENT:
			{
				printf("файл не найден");
				file_exists = false;
				break;
			}
			case EACCES:
			{
				printf("нет доступа к файлу");
				break;
			}
			case EBUSY:
			{
				printf("файл занят другим процессом");
				break;
			}
			default:
			{
				printf("%s", strerror(errno));
				break;
			}
			}
			switch (errno)
			{
			case ENOENT:
				break;
			case EACCES:
			case EBUSY:
				exit(errno);
			default:
			{
				exit(errno);
				break;
			}
		}
	}
	if(file)
		fclose(file);
#endif
	if (!file_exists)
	{
		printf("Создание нового файла-хранилища \"%s\"\n", PASSWORD_FILE);
		int creation_code = TryToCreateFile((const char**)PASSWORD_FILE);
		if (creation_code)
		{
			printf("Ошибка создания файла \"%s\": %d\n", PASSWORD_FILE, creation_code);
			exit(creation_code);
		}
		else
			printf("Успешно создан файл для хранения паролей\n");
	}
}


#ifdef _WIN32
void EnableVTMode()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
#endif // _WIN32


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
#ifdef _WIN32
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	EnableVTMode();
#endif // _WIN32
	
	struct ConfigurationStruct config;
	if(ini_parse("config.ini", ini_config_parse_handler, &config) < 0)
	{
		print_with_color("Failed to load app config!\n", 91);
		return EXIT_FAILURE;
	}
	printf("cipher_file_path:\"%s\"\niv_file_path:\"%s\"\n", config.key_file_path, config.iv_file_path);
	struct ChipherStruct chipher={0, NULL, 0, NULL};
	if(load_chipher_struct(&chipher, &config))
		return EXIT_FAILURE;
	
	CheckPasswordStorage();
	Dialog(PASSWORD_FILE, &config);
	
#ifdef _WIN32
	system("pause");
#elif defined __unix__
	printf("Press the \"Enter\" key to continue\n");
	getchar();
#endif
	
	return EXIT_SUCCESS;
}
