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
	print_with_color("�����\t\t\t\t\t\t%d\n",					31,	COMMAND_EXIT					);
	print_with_color("�������� ������ �������\t\t\t\t%d\n",		36, COMMAND_SHOW_COMMAND_LIST		);
	print_with_color("�������� �����\t\t\t\t\t%d\n",			36, COMMAND_CLS						);
	print_with_color("�������� ����� ������\t\t\t\t%d\n",		32, COMMAND_ADD_NEW					);
	print_with_color("������� ������\t\t\t\t\t%d\n",			91, COMMAND_DELETE_PASSWORD			);
	print_with_color("������� ������ � ���������\t\t\t%d\n",	91, COMMAND_DELETE_PASSWORD_BY_NAME	);
	print_with_color("������� ������ � �������\t\t\t%d\n",		91, COMMAND_DELETE_PASSWORD_BY_LOGIN);
	print_with_color("����� ������\t\t\t\t\t%d\n",				96, COMMAND_FIND_PASSWORDS			);
	print_with_color("�������� ��� ������\t\t\t\t%d\n",			96, COMMAND_SHOW_ALL_PASSWORDS		);
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

		// �������� ������ ��������������
		if (*endptr != '\0' || errno == ERANGE)
		{
			print_with_color("������! ������� ���������� �����\n",31);
			continue;
		}
		// �������� ��������� int
		if (long_command < INT_MIN || long_command > INT_MAX)
		{
			print_with_color("������! ����� ��� ����������� ��������� ���������\n",31);
			continue;
		}
		if (long_command < COMMNAD_FIRST || long_command > COMMNAD_LAST)
		{
			print_with_color("������! �������� ��������\n",31);
			system("pause");
			system("cls");
			ShowCommandList();
			continue;
		}

		// ���������� � int ����� ���� ��������
		command = (int)long_command;

		switch (command)
		{
			case COMMAND_EXIT:
				print_with_color("�����\n",31);
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
					print_with_color("����� ������ ������� ��������", 92);
				else
				if(res==EXIT_FAILURE)
					print_with_color("�� ������� �������� ����� ������",91);
				else
					print_with_color("�� ������� �������� ����� ������. ������ �����", 91);
			break;



			case COMMAND_SHOW_ALL_PASSWORDS:
				struct PasswordStruct* passwords;
				size_t passwords_quantity;
				passwords=GetAllPasswords(file,&passwords_quantity);

				if (passwords == NULL)
				{
					print_with_color("������ ������ �������",91);
					continue;
				}

				for (int i = 0; i != passwords_quantity; ++i)
				{
					printf("��������\t:%s\
					��������:\t%s\
					�����:\t%s\
					������:\t%s\
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
	printf("������� �������� ��� ������ ������:\n");

	int scan_res;
	printf("�������� ������\t->");
	scan_res = scanf("%s",password_struct->name);
	if(scan_res!=1)
		return 2;

	printf("�������� ������\t->");
	scan_res = scanf("%s",password_struct->description);
	if (scan_res != 1)
		return 2;

	printf("�����\t->");
	scan_res = scanf("%s",password_struct->login);
	if (scan_res != 1)
		return 2;

	printf("������\t->");
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