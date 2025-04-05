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
	FILE* file = fopen(filename, "w");
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
				printf("%d", strerror(errno));
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
			printf("Ошибка создания файла \"%s\": %d", PASSWORD_FILE, creation_code);
			exit(creation_code);
		}
		else
			printf("Успешно создан файл для хранения паролей");
	}
}




int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	CheckPasswordStorage();

	FILE* file=fopen(PASSWORD_FILE,"r+");

	Dialog(&file);
	fclose(file);
	system("pause");

	return EXIT_SUCCESS;
}
