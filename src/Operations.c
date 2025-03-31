#include "Operations.h"

void print_with_color(const char* format, int color, ...)
{
	printf("\x1b[%dm", color);

	va_list args;
	va_start(args, color);
	vprintf(format, args);
	va_end(args);

	printf("\x1b[0m");
}

int AddNewPasswordStructEmplace(struct PasswordStruct*** array, size_t* arraySize, struct PasswordStruct* newElement)
{
	if (!array)
		return 2;
	if (!arraySize)
		return 3;

	if (!newElement)
		return 4;
	
	if (!*array)
	{
		*array=malloc(sizeof(struct PasswordStruct*));
		if (*array)
			*array[0]=newElement;

		*arraySize=1;

		return EXIT_SUCCESS;
	}
	else
	{
		struct PasswordStruct** old_array=malloc(sizeof(struct PasswordStruct*)*(*arraySize));
		if (old_array != NULL)
			for (size_t i = 0; i != *arraySize; i++)
				old_array[i]=*array[i];

		free(*array);
		*array=malloc(sizeof(struct PasswordStruct*)*((*arraySize)+1));

		if (old_array && *array)
		{
			for (size_t i = 0; i != *arraySize; ++i)
				*array[i]=old_array[i];

			(*array)[(*arraySize)++]=newElement;

			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
}


void ShowCommandList()
{
	print_with_color("Выход\t\t\t\t%d\n",					31,	COMMAND_EXIT					);
	print_with_color("Просмотр списка комманд\t\t%d\n",		36, COMMAND_SHOW_COMMAND_LIST		);
	print_with_color("Очистить экран\t\t\t%d\n",			36, COMMAND_CLS						);
	print_with_color("Записать новый пароль\t\t%d\n",		32, COMMAND_ADD_NEW					);
	print_with_color("Удалить пароль\t\t\t%d\n",			91, COMMAND_DELETE_PASSWORD			);
	print_with_color("Удалить пароль с названием\t%d\n",	91, COMMAND_DELETE_PASSWORD_BY_NAME	);
	print_with_color("Удалить пароль с логином\t%d\n",		91, COMMAND_DELETE_PASSWORD_BY_LOGIN);
	print_with_color("Найти пароль\t\t\t%d\n",				96, COMMAND_FIND_PASSWORDS			);
	print_with_color("Показать все пароли\t\t%d\n",			96, COMMAND_SHOW_ALL_PASSWORDS		);
}

void Dialog(FILE** file)
{
	char input[256];
	long long_command;
	int command;
	long file_size=0;

	ShowCommandList();

	while (1)
	{
		printf(">");
		fflush(stdout);

		if (!fgets(input, sizeof(input), stdin))
		{
			if (feof(stdin))
				return;
		}

		size_t len = strlen(input);
		if (len > 0 && input[len - 1] == '\n')
		{
			input[len - 1] = '\0';
			len--;
		}

		if (len == 0)
			continue;

		
		char* endptr;
		errno = 0;
		long_command = strtol(input, &endptr, 10);

		// Проверка ошибок преобразования
		if (*endptr != '\0' || errno == ERANGE)
		{
			print_with_color("Ошибка! Введите корректное число\n",31);
			continue;
		}
		// Проверка диапазона int
		if (long_command < INT_MIN || long_command > INT_MAX)
		{
			print_with_color("Ошибка! Число вне допустимого числового диапазона\n",31);
			continue;
		}
		if (long_command < COMMNAD_FIRST || long_command > COMMNAD_LAST)
		{
			print_with_color("Ошибка! Неверная комманда\n",31);
			system("pause");
			system("cls");
			ShowCommandList();
			continue;
		}

		// Приведение к int после всех проверок
		command = (int)long_command;


		file_size=getFileSize(file);

		switch (command)
		{
			case COMMAND_EXIT:
			{
				print_with_color("Выход\n",31);
				return;

				break;
			}
			case COMMAND_SHOW_COMMAND_LIST:
			{
				ShowCommandList();
				continue;

				break;
			}
			case COMMAND_CLS:
			{
				system("cls");
				break;
			}
			case COMMAND_ADD_NEW:
			{
				struct PasswordStruct password;
				printf("Введите значения для нового пароля:\n");

				int scan_res;
				printf("Название пароля\t->");
				scan_res = scanf("%s", password.name);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Описание пароля\t->");
				scan_res = scanf("%s", password.description);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Логин\t\t->");
				scan_res = scanf("%s", password.login);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Пароль\t\t->");
				scan_res = scanf("%s", password.password);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				password.name		[sizeof(password.name		) - 1] = '\0';
				password.description[sizeof(password.description) - 1] = '\0';
				password.login		[sizeof(password.login		) - 1] = '\0';
				password.password	[sizeof(password.password	) - 1] = '\0';
				int res=AddNewPassword(file,&password);
				if(res==EXIT_SUCCESS)
					print_with_color("Новый пароль успешно добавлен\n", 92);
				else
				if(res==EXIT_FAILURE)
					print_with_color("Не удалось добавить новый пароль\n",91);
				else
					print_with_color("Не удалось добавить новый пароль. Ошибка ввода\n", 91);

				break;
			}
			case COMMAND_DELETE_PASSWORD:
			{
				struct PasswordStruct password;
				printf("Введите значения пароля для удаления:\n");

				int scan_res;
				printf("Название пароля\t->");
				scan_res = scanf("%s", password.name);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Описание пароля\t->");
				scan_res = scanf("%s", password.description);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Логин\t\t->");
				scan_res = scanf("%s", password.login);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				printf("Пароль\t\t->");
				scan_res = scanf("%s", password.password);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				password.name		[sizeof(password.name		) - 1] = '\0';
				password.description[sizeof(password.description) - 1] = '\0';
				password.login		[sizeof(password.login		) - 1] = '\0';
				password.password	[sizeof(password.password	) - 1] = '\0';
				int res = DeletePassword(file, &password);

				switch (res)
				{
					case 0:
						print_with_color("Записи успешно удалены",96);
					break;
					case 1:
						print_with_color("Не удалось перезаписать файл после удаления записей", 91);
					break;
					case 2:
						if (file_size == 0)
						{
							print_with_color("Файл с паролями пуст", 95);
							break;
						}
						print_with_color("Не удалось прочитать файл с записями",91);
					break;
					case 3:
						print_with_color("Не найдено записей для удаления", 91);
					break;
				}
				printf("\n");
					
				break;
			}
			case COMMAND_DELETE_PASSWORD_BY_NAME:
			{
				char name[PASSWORD_STRUCT_NAME_SIZE];
				printf("Введите имя пароля для удаления:\n");

				int scan_res;
				printf("Название пароля\t->");
				scan_res = scanf("%s", name);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				int res=DeletePasswordByName(file,name);
				switch (res)
				{
				case 0:
					print_with_color("Записи успешно удалены", 96);
					break;
				case 1:
					print_with_color("Не удалось перезаписать файл после удаления записей", 91);
					break;
				case 2:
					if (file_size == 0)
					{
						print_with_color("Файл с паролями пуст", 95);
						break;
					}
					print_with_color("Не удалось прочитать файл с записями", 91);
					break;
				case 3:
					print_with_color("Не найдено записей для удаления", 91);
					break;
				}
				printf("\n");
				break;
			}
			case COMMAND_DELETE_PASSWORD_BY_LOGIN:
			{
				char login[PASSWORD_STRUCT_LOGIN_SIZE];
				printf("Введите логин для удаления записей:\n");

				int scan_res;
				printf("Логин\t\t->");
				scan_res = scanf("%s", login);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				int res = DeletePasswordByLogin(file, login);
				switch (res)
				{
				case 0:
					print_with_color("Записи успешно удалены", 96);
					break;
				case 1:
					print_with_color("Не удалось перезаписать файл после удаления записей", 91);
					break;
				case 2:
					if (file_size == 0)
					{
						print_with_color("Файл с паролями пуст", 95);
						break;
					}
					print_with_color("Не удалось прочитать файл с записями", 91);
					break;
				case 3:
					print_with_color("Не найдено записей для удаления", 91);
					break;
				}
				printf("\n");
				break;
			}
			


			case COMMAND_SHOW_ALL_PASSWORDS:
			{

				struct PasswordStruct* passwords;
				size_t passwords_quantity;

				{
					if (file_size == 0)
					{
						print_with_color("Файл с паролями пуст\n",95);
						continue;
					}
				}
					
				passwords=GetAllPasswords(file,&passwords_quantity);

				if (passwords == NULL)
				{
					print_with_color("Ошибка чтения паролей\n",91);
					continue;
				}

				for (int i = 0; i != passwords_quantity; ++i)
				{
					printf("Название:\t%s\n\
Описание:\t%s\n\
Логин:\t\t%s\n\
Пароль:\t\t%s\n\
\n",
						passwords[i].name,passwords[i].description,passwords[i].login,passwords[i].password
					);
				}
				free(passwords);

				break;
			}
		}

	}
}

int AddNewPassword(FILE** file, struct PasswordStruct* password_struct)
{
	long prev_file_pos=ftell(*file);
	fseek(*file,0,SEEK_END);
	if (fwrite(password_struct, sizeof(struct PasswordStruct), 1, *file)==1)
	{
		fseek(*file,prev_file_pos,SEEK_SET);
		return EXIT_SUCCESS;
	}
	else
	{
		fseek(*file,prev_file_pos,SEEK_SET);
		return EXIT_FAILURE;
	}
}

int DeletePassword(FILE** file, struct PasswordStruct* password_struct)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords=GetAllPasswords(file,&passwords_quantity);
	
	size_t passwords_for_remove_quantity=0;
	struct PasswordStruct** passwords_for_remove=NULL;

	if(!passwords)
		return 2;

	
	for(size_t i=0;i!=passwords_quantity;++i)
		if(
			strcmp(passwords[i].name		,password_struct->name			)==0 &&
			strcmp(passwords[i].description	,password_struct->description	)==0 &&
			strcmp(passwords[i].login		,password_struct->login			)==0 &&
			strcmp(passwords[i].password	,password_struct->password		)==0
		)
			AddNewPasswordStructEmplace(&passwords_for_remove, &passwords_for_remove_quantity, passwords+i);

	if(!passwords_for_remove_quantity)
		return 3;

	size_t passwords_for_rewrite_quantity=0;
	struct PasswordStruct* passwords_for_rewrite=NULL;

	for (size_t i = 0; i != passwords_quantity; ++i)
	{
		bool in_deleted=false;
		for (size_t j = 0; j != passwords_for_remove_quantity; ++j)
		{
			if (passwords + i == passwords_for_remove[j])
			{
				in_deleted=true;
				break;
			}
		}
		
		if(!in_deleted)
			AddNewPasswordStruct(&passwords_for_rewrite,&passwords_for_rewrite_quantity, passwords + i);
	}

	*file = freopen(PASSWORD_FILE, "w", *file);
	int res= WritePasswordStructs(file, passwords_for_rewrite, passwords_for_rewrite_quantity);

	*file = freopen(PASSWORD_FILE, "r+", *file);

	if(res)
		return EXIT_FAILURE;

	if(passwords_for_rewrite)
		free(passwords_for_rewrite);
	if(passwords_for_remove)
		free(passwords_for_remove);
	if(passwords)
		free(passwords);

	return EXIT_SUCCESS;
}

int DeletePasswordByName(FILE** file, const char* name)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords = GetAllPasswords(file, &passwords_quantity);

	size_t passwords_for_remove_quantity = 0;
	struct PasswordStruct** passwords_for_remove = NULL;

	if (!passwords)
		return 2;

	for (size_t i = 0; i != passwords_quantity; ++i)
		if (strcmp(passwords[i].name, name) == 0)
			AddNewPasswordStructEmplace(&passwords_for_remove, &passwords_for_remove_quantity, passwords + i);

	if (!passwords_for_remove_quantity)
		return 3;

	size_t passwords_for_rewrite_quantity = 0;
	struct PasswordStruct* passwords_for_rewrite = NULL;

	for (size_t i = 0; i != passwords_quantity; ++i)
	{
		bool in_deleted = false;
		for (size_t j = 0; j != passwords_for_remove_quantity; ++j)
		{
			if (passwords + i == passwords_for_remove[j])
			{
				in_deleted = true;
				break;
			}
		}

		if (!in_deleted)
			AddNewPasswordStruct(&passwords_for_rewrite, &passwords_for_rewrite_quantity, passwords + i);
	}

	*file = freopen(PASSWORD_FILE, "w", *file);
	int res = WritePasswordStructs(file, passwords_for_rewrite, passwords_for_rewrite_quantity);

	*file = freopen(PASSWORD_FILE, "r+", *file);

	if (res)
		return EXIT_FAILURE;

	if (passwords_for_rewrite)
		free(passwords_for_rewrite);
	if (passwords_for_remove)
		free(passwords_for_remove);
	if (passwords)
		free(passwords);

	return EXIT_SUCCESS;
}

int DeletePasswordByLogin(FILE** file, const char* login)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords = GetAllPasswords(file, &passwords_quantity);

	size_t passwords_for_remove_quantity = 0;
	struct PasswordStruct** passwords_for_remove = NULL;

	if (!passwords)
		return 2;

	for (size_t i = 0; i != passwords_quantity; ++i)
		if (strcmp(passwords[i].login, login) == 0)
			AddNewPasswordStructEmplace(&passwords_for_remove, &passwords_for_remove_quantity, passwords + i);

	if (!passwords_for_remove_quantity)
		return 3;

	size_t passwords_for_rewrite_quantity = 0;
	struct PasswordStruct* passwords_for_rewrite = NULL;

	for (size_t i = 0; i != passwords_quantity; ++i)
	{
		bool in_deleted = false;
		for (size_t j = 0; j != passwords_for_remove_quantity; ++j)
		{
			if (passwords + i == passwords_for_remove[j])
			{
				in_deleted = true;
				break;
			}
		}

		if (!in_deleted)
			AddNewPasswordStruct(&passwords_for_rewrite, &passwords_for_rewrite_quantity, passwords + i);
	}

	*file = freopen(PASSWORD_FILE, "w", *file);
	int res = WritePasswordStructs(file, passwords_for_rewrite, passwords_for_rewrite_quantity);

	*file = freopen(PASSWORD_FILE, "r+", *file);

	if (res)
		return EXIT_FAILURE;

	if (passwords_for_rewrite)
		free(passwords_for_rewrite);
	if (passwords_for_remove)
		free(passwords_for_remove);
	if (passwords)
		free(passwords);

	return EXIT_SUCCESS;
}

struct PasswordStruct* FindPasswords(struct PasswordStruct* params, int flags)
{
	return NULL;
}