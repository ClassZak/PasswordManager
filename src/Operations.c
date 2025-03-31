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



void ShowCommandList()
{
	print_with_color("Выход\t\t\t\t\t\t%d\n",					31,	COMMAND_EXIT					);
	print_with_color("Просмотр списка комманд\t\t\t\t%d\n",		36, COMMAND_SHOW_COMMAND_LIST		);
	print_with_color("Очистить экран\t\t\t\t\t%d\n",			36, COMMAND_CLS						);
	print_with_color("Записать новый пароль\t\t\t\t%d\n",		32, COMMAND_ADD_NEW					);
	print_with_color("Удалить пароль\t\t\t\t\t%d\n",			91, COMMAND_DELETE_PASSWORD			);
	print_with_color("Удалить пароль с названием\t\t\t%d\n",	91, COMMAND_DELETE_PASSWORD_BY_NAME	);
	print_with_color("Удалить пароль с логином\t\t\t%d\n",		91, COMMAND_DELETE_PASSWORD_BY_LOGIN);
	print_with_color("Найти пароль\t\t\t\t\t%d\n",				96, COMMAND_FIND_PASSWORDS			);
	print_with_color("Показать все пароли\t\t\t\t%d\n",			96, COMMAND_SHOW_ALL_PASSWORDS		);
}

void Dialog(FILE** file)
{
	char input[256];
	long long_command;
	int command;

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

		switch (command)
		{
			case COMMAND_EXIT:
				print_with_color("Выход\n",31);
				return;
			break;
			case COMMAND_SHOW_COMMAND_LIST:
				ShowCommandList();
				continue;
			break;
			case COMMAND_CLS:
				system("cls");
			break;
			case COMMAND_ADD_NEW:
				struct PasswordStruct password;
				int res=AddNewPassword(file,&password);
				if(res==EXIT_SUCCESS)
					print_with_color("Новый пароль успешно добавлен", 92);
				else
				if(res==EXIT_FAILURE)
					print_with_color("Не удалось добавить новый пароль",91);
				else
					print_with_color("Не удалось добавить новый пароль. Ошибка ввода", 91);
			break;



			case COMMAND_SHOW_ALL_PASSWORDS:
				struct PasswordStruct* passwords;
				size_t passwords_quantity;
				passwords=GetAllPasswords(file,&passwords_quantity);

				if (passwords == NULL)
				{
					print_with_color("Ошибка чтения паролей",91);
					continue;
				}

				for (int i = 0; i != passwords_quantity; ++i)
				{
					printf("Название\t:%s\
					Описание:\t%s\
					Логин:\t%s\
					Пароль:\t%s\
					\n",
						passwords[i].name,passwords[i].description,passwords[i].login,passwords[i].password
					);
				}
				free(passwords);
			break;
		}

	}
}

int AddNewPassword(FILE** file, struct PasswordStruct* password_struct)
{
	printf("Введите значения для нового пароля:\n");

	int scan_res;
	printf("Название пароля\t->");
	scan_res = scanf("%s",password_struct->name);
	if(scan_res!=1)
		return 2;

	printf("Описание пароля\t->");
	scan_res = scanf("%s",password_struct->description);
	if (scan_res != 1)
		return 2;

	printf("Логин\t->");
	scan_res = scanf("%s",password_struct->login);
	if (scan_res != 1)
		return 2;

	printf("Пароль\t->");
	scan_res = scanf("%s",password_struct->password);
	if (scan_res != 1)
		return 2;

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
	return 0;
}

int DeletePasswordByName(FILE** file, const char* name)
{
	return 0;
}

int DeletePasswordByLogin(FILE** file, const char* login)
{
	return 0;
}

struct PasswordStruct* FindPasswords(struct PasswordStruct* params, int mode)
{
	return NULL;
}