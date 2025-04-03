#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else 
#ifdef _UNIX
#include <errno.h>
#include <unistd.h>
#endif
#endif

#include "PasswordStruct.h"
#include "Operations.h"

#ifndef PASSWORD_FILE 
#define PASSWORD_FILE ".Passwords.bin"
#endif // !PASSWORD_FILE 

int TryToCreateFile(const char** filename) // 0 - óñïåõ. Îñòàëüíîå - îøèáêà
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
#else _UNIX
	FILE* file = fopen(filename, "w");
#endif

#ifdef _WIN32
	if (hFile == INVALID_HANDLE_VALUE)
#else _UNIX
	if (!file)
#endif
	{
		int last_error;
#ifdef _WIN32	
		last_error = GetLastError();
#else _UNIX
		last_error = errno;
#endif
		return last_error;
	}

#ifdef _WIN32	
	CloseHandle(hFile);
#else _UNIX
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
		printf("Îøèáêà ïðè îòêðûòèè ôàéëà-õðàíèëèùà \"%s\"\n", PASSWORD_FILE);
		printf("Îøèáêà: ");

		switch (error)
		{
			case ERROR_FILE_NOT_FOUND:
			{
				file_exists = false;
				printf("ôàéë íå íàéäåí\n");
				break;
			}
			case ERROR_PATH_NOT_FOUND:
			{
				file_exists = false;
				printf("ïóòü íå íàéäåí\n");
				break;
			}
			case ERROR_ACCESS_DENIED:
			{
				printf("íåò äîñòóïà\n");
				break;
			}
			case ERROR_SHARING_VIOLATION:
			{
				printf("ôàéë çàíÿò äðóãèì ïðîöåññîì");
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
#else
#ifdef _UNIX
	FILE* file = fopen(PASSWORD_FILE, 'rb');
	if (!file)
	{
		printf("Îøèáêà ïðè îòêðûòèè ôàéëà-õðàíèëèùà \"%s\"\n", PASSWORD_FILE);
		printf("Îøèáêà: ");
		switch (errno)
		{
			case ENOENT:
			{
				printf("ôàéë íå íåéäåí");
				file_exists = false;
				break;
			}
			case EACCES:
			{
				printf("íåò äîñòóïà ê ôàéëó");
				break;
			}
			case EBUSY:
			{
				printf("ôàéë çàíÿò äðóãèì ïðîöåññîì");
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
	fclose(file);
#endif
	if (!file_exists)
	{
		printf("Ñîçäàíèå íîâîãî ôàéëà-õðàíèëèùà \"%s\"\n", PASSWORD_FILE);
		int creation_code = TryToCreateFile((const char**)PASSWORD_FILE);
		if (creation_code)
		{
			printf("Îøèáêà ñîçäàíèÿ ôàéëà \"%s\": %d", PASSWORD_FILE, creation_code);
			exit(creation_code);
		}
		else
			printf("Óñïåøíî ñîçäàí ôàéë äëÿ õðàíåíèÿ ïàðîëåé");
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
