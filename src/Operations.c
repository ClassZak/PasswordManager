#include "Operations.h"
#include "ChipherStructOperations.h"

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
				old_array[i]=(*array)[i];

		free(*array);
		*array=malloc(sizeof(struct PasswordStruct*)*((*arraySize)+1));

		if (old_array && *array)
		{
			for (size_t i = 0; i != *arraySize; ++i)
				(*array)[i]=old_array[i];

			(*array)[(*arraySize)++]=newElement;

			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
}


void ShowCommandList()
{
	print_with_color("Выход\t\t\t\t%d\n",					31, COMMAND_EXIT						);
	print_with_color("Просмотр списка команд\t\t%d\n",		36, COMMAND_SHOW_COMMAND_LIST		);
	print_with_color("Очистить экран\t\t\t%d\n",			36, COMMAND_CLS						);
	print_with_color("Записать новый пароль\t\t%d\n",		32, COMMAND_ADD_NEW					);
	print_with_color("Удалить пароль\t\t\t%d\n",			91, COMMAND_DELETE_PASSWORD			);
	print_with_color("Удалить пароль с названием\t%d\n",	91, COMMAND_DELETE_PASSWORD_BY_NAME	);
	print_with_color("Удалить пароль с логином\t%d\n",		91, COMMAND_DELETE_PASSWORD_BY_LOGIN	);
	print_with_color("Найти пароль\t\t\t%d\n",				96, COMMAND_FIND_PASSWORDS			);
	print_with_color("Показать все пароли\t\t%d\n",			96, COMMAND_SHOW_ALL_PASSWORDS		);
}

void Dialog(const char* filename,struct ConfigurationStruct* config)
{
	// Chipher initialization
	struct ChipherStruct chipher = {0, NULL, 0, NULL};
	load_chipher_struct(&chipher, config);
	printf("key:\"%s\"\niv:\"%s\"", chipher.key, chipher.iv);

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
			print_with_color("Ошибка! Неверная команда\n",31);

#ifdef _WIN32
			system("pause");
#elif defined __unix__
			printf("Press the \"Enter\" key to continue\n");
			getchar();
#endif
#ifdef _WIN32
			system("cls");
#elif defined __unix__
			printf("\033[2J\033[H");
#endif

			ShowCommandList();
			continue;
		}
		
		// Приведение к int после всех проверок
		command = (int)long_command;
		
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
#ifdef _WIN32
				system("cls");
#elif defined __unix__
				printf("\033[2J\033[H");
#endif
				ShowCommandList();
				
				break;
			}
			case COMMAND_ADD_NEW:
			{
				printf("Введите значения для нового пароля:\n");
				
				struct PasswordStruct* password = scan_password_struct();
				
				int res;
				if(password)
					res=AddNewPassword(filename, password, &chipher);
				else
					res=2;

				if(res==EXIT_SUCCESS)
					print_with_color("Новый пароль успешно добавлен\n", 32);
				else
				if(res==EXIT_FAILURE)
					print_with_color("Не удалось добавить новый пароль\n",91);
				else
					print_with_color("Не удалось добавить новый пароль. Ошибка ввода\n", 91);

				break;
			}
			case COMMAND_DELETE_PASSWORD:
			{
				printf("Введите значения пароля для удаления:\n");
				
				struct PasswordStruct* password = scan_password_struct();

				int res;
				if (password)
					res = AddNewPassword(filename, password, &chipher);
				else
					res = 4;

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
					case 4:
						print_with_color("Не удалось добавить новый пароль. Ошибка ввода", 91);
					break;
				}
				printf("\n");
					
				break;
			}
			case COMMAND_DELETE_PASSWORD_BY_NAME:
			{
				char name[100];

				printf("Введите имя пароля для удаления:\n");
				printf("Название пароля\t->");
				scan_long_string(name);

				int res=DeletePasswordByName(filename,name,&chipher);
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
				char login[100];

				printf("Введите логин для удаления записей:\n");
				scan_long_string(login);

				int res = DeletePasswordByLogin(filename, login, &chipher);
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
			case COMMAND_FIND_PASSWORDS:
			{
				printf("Выберете атрибуты для поиска\n");
				bool name_attr=false,description_attr=false,login_attr=false,password_attr=false,find_by_content=false;
				int scan_res;
				int scan_bool;
				printf("Включить поиск по названию\t\t(0 чтобы отклонить) ->");
				scan_res = scanf("%d", &scan_bool);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				name_attr= scan_bool;

				printf("Включить поиск по описанию\t\t(0 чтобы отклонить) ->");
				scan_res = scanf("%d", &scan_bool);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				description_attr = scan_bool;

				printf("Включить поиск по логину\t\t(0 чтобы отклонить) ->");
				scan_res = scanf("%d", &scan_bool);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				login_attr = scan_bool;

				printf("Включить поиск по паролю\t\t(0 чтобы отклонить) ->");
				scan_res = scanf("%d", &scan_bool);
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}
				password_attr = scan_bool;

				printf("Выберете тип поиска:\n");
				print_with_color("0 - Поиск по точным совпадениям. 1 - Поиск по содержимому\n",90);
				printf("->");
				scan_res = scanf("%d", &scan_bool);
				clear_scan_buffer();
				find_by_content = !scan_bool;
				if (scan_res != 1)
				{
					handle_char_input_error();
					continue;
				}

				if (!name_attr && !description_attr && !login_attr && !password_attr)
				{
					print_with_color("Нет никаких критериев поиска\n",91);
					continue;
				}

				size_t passwords_quantity;
				struct PasswordStruct password = {0,NULL,0,NULL,0,NULL,0,NULL};
				printf("Введите значения пароля для поиска:\n");

				password.name = malloc((size_t)1 << sizeof(size_t));
				if(!password.name)
					continue;
				password.description = malloc((size_t)1 << sizeof(size_t));
				if (!password.description)
					continue;
				password.login = malloc((size_t)1 << sizeof(size_t));
				if (!password.login)
					continue;
				password.password = malloc((size_t)1 << sizeof(size_t));
				if (!password.password)
					continue;
				if (name_attr)
				{
					printf("Название пароля\t->");
					scan_long_string(password.name);
				}
				if (description_attr)
				{
					printf("Описание пароля\t->");
					scan_long_string(password.description);
				}
				if (login_attr)
				{
					printf("Логин\t\t->");
					scan_long_string(password.login);
				}
				if (password_attr)
				{
					printf("Пароль\t\t->");
					scan_long_string(password.password);
				}
				password.name		[sizeof(password.name		) - 1] = '\0';
				password.description[sizeof(password.description) - 1] = '\0';
				password.login		[sizeof(password.login		) - 1] = '\0';
				password.password	[sizeof(password.password	) - 1] = '\0';

				int flags=
						(PASSWORD_STRUCT_FIND_BY_NAME			& (name_attr		<< 4))|
						(PASSWORD_STRUCT_FIND_BY_LOGIN			& (login_attr		<< 3))|
						(PASSWORD_STRUCT_FIND_BY_PASSWORD		& (password_attr	<< 2))|
						(PASSWORD_STRUCT_FIND_BY_DESCRIPTION	& (description_attr	<< 1))|
						(PASSWORD_STRUCT_FIND_CONTAINS			& (find_by_content	<< 0));
				struct PasswordStruct* founded_passwords=NULL;
				size_t founded_passwords_quantity=0;
				int res=FindPasswords
				(
					filename,
					&password,
					&founded_passwords,
					&founded_passwords_quantity,
					flags
				);

				printf("\n");
				switch (res)
				{
					case EXIT_SUCCESS:
					{
						print_with_color("Найденные записи: \n",96);
						print_passwords(founded_passwords, founded_passwords_quantity);
						break;
					}
					case EXIT_FAILURE:
					{
						print_with_color("Не удалось прочитать данные из файла: \n",96);
						break;
					}
					case 2:
					{
						print_with_color("Файл с паролями пуст\n", 95);
						break;
					}
					case 3:
					{
						print_with_color("Записей не найдено\n", 93);
						break;
					}
				default:
					break;
				}

				break;
			}
			case COMMAND_SHOW_ALL_PASSWORDS:
			{
				struct PasswordStruct* passwords;
				size_t passwords_quantity;

				{
					FILE* file = fopen(filename, "r");
					if(!file)
					{
						print_with_color("Ошибка чтения паролей\n", 91);
						continue;
					}
					file_size = get_file_size(&file);
					if (file_size == 0)
					{
						print_with_color("Файл с паролями пуст\n",95);
						continue;
					}
					fclose(file);
				}
				
				int result = GetAllPasswordStructs(&passwords, &passwords_quantity, filename);

				if (passwords == NULL)
				{
					print_with_color("Ошибка чтения паролей\n",91);
					continue;
				}

				print_passwords(passwords,passwords_quantity);
				free(passwords);

				break;
			}
		}
	}
}

int AddNewPassword(const char* filename, struct PasswordStruct* password_struct, struct ChipherStruct* chipher)
{
	size_t file_data_size = 0;
	void* file_data = read_file(filename, &file_data_size);
	size_t decrypt_file_data_size = 0;
	void* decrypt_file_data = decrypt_buffer(file_data, file_data_size, &decrypt_file_data_size);
	size_t passwords_count = 0;
	struct PasswordStruct* passwords = parse_password_structs(decrypt_file_data, decrypt_file_data_size, &passwords_count);

	if(AddNewPasswordStruct(&passwords, &passwords_count, password_struct))
		return EXIT_FAILURE;
	if(WriteAllPasswordStructs(filename,passwords,passwords_count, chipher))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int DeletePassword(const char* filename, struct PasswordStruct* password_struct,struct ChipherStruct* chipher)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords;
	GetAllPasswordStructs(&passwords, &passwords_quantity, filename);
	
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

	int res = WriteAllPasswordStructs(filename, passwords_for_rewrite, passwords_for_rewrite_quantity, chipher);
	if(passwords_for_remove)
		free(passwords_for_remove);
	if(passwords)
		free(passwords);
	if(passwords_for_rewrite)
		free(passwords_for_rewrite);


	if(res)
		return res;
	else
		return EXIT_SUCCESS;
}

int DeletePasswordByName(const char* filename, const char* name, struct ChipherStruct* chipher)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords;
	GetAllPasswordStructs(&passwords, &passwords_quantity, filename);

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

	int res = WriteAllPasswordStructs(filename, passwords_for_rewrite, passwords_for_rewrite_quantity, chipher);
	if (passwords_for_rewrite)
		free(passwords_for_rewrite);
	if (passwords_for_remove)
		free(passwords_for_remove);
	if (passwords)
		free(passwords);

	if(res)
		return res;
	else
		return EXIT_SUCCESS;
}

int DeletePasswordByLogin(const char* filename, const char* login, struct ChipherStruct* chipher)
{
	size_t passwords_quantity;
	struct PasswordStruct* passwords;
	GetAllPasswordStructs(&passwords, &passwords_quantity, filename);

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

	int res = WriteAllPasswordStructs(filename, passwords_for_rewrite, passwords_for_rewrite_quantity, chipher);
	if (passwords_for_rewrite)
		free(passwords_for_rewrite);
	if (passwords_for_remove)
		free(passwords_for_remove);
	if (passwords)
		free(passwords);

	if (res)
		return res;
	else
		return EXIT_SUCCESS;
}

int FindPasswords
(
	const char* filename,
	struct PasswordStruct* params,
	struct PasswordStruct** founded_passwords,
	size_t* founded_passwords_quantity,
	int flags
)
{
	if(params==NULL)
		return 3;

	{
		FILE* file = fopen(filename, "rb");
		if(!file)
			return 1;
		size_t file_size = get_file_size(&file);
		fclose(file);
		if(!file_size)
			return 2;
	}
	size_t passwords_quantity;
	struct PasswordStruct* passwords;
	GetAllPasswordStructs(&passwords, &passwords_quantity, filename);

	if (*founded_passwords != NULL)
	{
		free(*founded_passwords);
		*founded_passwords = NULL;
	}

	if (flags & PASSWORD_STRUCT_FIND_CONTAINS)
	{
		for (size_t i = 0; i != passwords_quantity; ++i)
		{
			bool equals=true;
			if (flags & PASSWORD_STRUCT_FIND_BY_NAME)
			{
				equals=strcmp(params->name,passwords[i].name)==0;
				if(!equals)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_DESCRIPTION)
			{
				equals=strcmp(params->description,passwords[i].description)==0;
				if (!equals)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_LOGIN)
			{
				equals=strcmp(params->login,passwords[i].login)==0;
				if (!equals)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_PASSWORD)
			{
				equals=strcmp(params->password,passwords[i].password)==0;
				if (!equals)
					continue;
			}

			AddNewPasswordStruct(founded_passwords,founded_passwords_quantity,passwords+i);
		}
	}
	else
	{
		for (size_t i = 0; i != passwords_quantity; ++i)
		{
			bool contains = true;
			if (flags & PASSWORD_STRUCT_FIND_BY_NAME)
			{
				contains = strstr(passwords[i].name,params->name);
				if (!contains)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_DESCRIPTION)
			{
				contains = strstr(passwords[i].description, params->description);
				if (!contains)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_LOGIN)
			{
				contains = strstr(passwords[i].login, params->login);
				if (!contains)
					continue;
			}

			if (flags & PASSWORD_STRUCT_FIND_BY_PASSWORD)
			{
				contains = strstr(passwords[i].password, params->password);
				if (!contains)
					continue;
			}

			AddNewPasswordStruct(founded_passwords, founded_passwords_quantity, passwords + i);
		}
	}

	if(!(*founded_passwords))
		return 3;
	
	return EXIT_SUCCESS;
}
