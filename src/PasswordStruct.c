#include "PasswordStruct.h"

long getFileSize(FILE** file)
{
	long size, prev_pos = ftell(*file);

	fseek(*file, 0, SEEK_END);
	size = ftell(*file);
	fseek(*file, prev_pos,SEEK_SET);

	return size;
}


int ReadPasswordStruct
(
	FILE** file,
	struct PasswordStruct* password_struct
)
{
	if(!password_struct)
		return EXIT_FAILURE;

	fread(password_struct,	sizeof(struct PasswordStruct),1,*file);

	password_struct->description[sizeof(password_struct->description) - 1] = '\0';
	password_struct->login		[sizeof(password_struct->login		) - 1] = '\0';
	password_struct->password	[sizeof(password_struct->password	) - 1] = '\0';
	password_struct->name		[sizeof(password_struct->name		) - 1] = '\0';

	return EXIT_SUCCESS;
}

int WritePasswordStruct
(
	FILE** file,
	struct PasswordStruct* password_struct
)
{
	if (!password_struct)
		return EXIT_FAILURE;

	size_t res=
	fwrite(password_struct,sizeof(struct PasswordStruct),1,*file);

	return (int)!res;
}

int WritePasswordStructs(FILE** file, struct PasswordStruct* password_struct, size_t size)
{
	if (!password_struct || !size)
		return EXIT_FAILURE;

	size_t res =
	fwrite(password_struct, sizeof(struct PasswordStruct), size, *file);

	return (int)!res;
}

struct PasswordStruct* ReadAllPasswordStructs(FILE** file, size_t* size)
{
	*size=0;

	long prev_pos=ftell(*file);
	fseek(*file,0,SEEK_SET);


	long file_size= getFileSize(file);
	if(file_size%sizeof(struct PasswordStruct) || !file_size)
		return NULL;

	int passwords_quantity=file_size/sizeof(struct PasswordStruct);
	struct PasswordStruct* passwords=malloc(passwords_quantity*sizeof(struct PasswordStruct));

	for(int i=0;i<passwords_quantity;++i)
		ReadPasswordStruct(file,passwords+i);
	
	*size=passwords_quantity;

	fseek(*file,prev_pos,SEEK_SET);

	return passwords;
}

int AddNewPasswordStruct(struct PasswordStruct** array, size_t* arraySize, struct PasswordStruct* newElement)
{
	if(!array)
		return 2;
	if (!arraySize)
		return 3;
		
	if(!newElement)
		return 4;

	if (!*array)
	{
		*array=malloc(sizeof(struct PasswordStruct));
		if (*array != NULL)
		{
			strcpy((*array)->name		, newElement->name		);
			strcpy((*array)->description, newElement->description);
			strcpy((*array)->login		, newElement->login		);
			strcpy((*array)->password	, newElement->password	);
		}
		else
			return EXIT_FAILURE;

		*arraySize=1;

		return EXIT_SUCCESS;
	}
	else
	{
		struct PasswordStruct* old_array=malloc(sizeof(struct PasswordStruct)*(*arraySize));

		if(old_array!=NULL)
			for (size_t i = 0; i != *arraySize; i++)
			{
				strcpy(old_array[i].name		, (*array)->name		);
				strcpy(old_array[i].description	, (*array)->description	);
				strcpy(old_array[i].login		, (*array)->login		);
				strcpy(old_array[i].password	, (*array)->password	);
			}

		free(*array);
		*array=malloc(sizeof(struct PasswordStruct) * ((*arraySize)+1));

		if (old_array && *array)
		{
			for (size_t i = 0; i != *arraySize; ++i)
			{
				strcpy(((*array)+i)->name		,old_array[i].name			);
				strcpy(((*array)+i)->description,old_array[i].description	);
				strcpy(((*array)+i)->login		,old_array[i].login			);
				strcpy(((*array)+i)->password	,old_array[i].password		);
			}

			strcpy(((*array)[*arraySize]).name			, newElement->name			);
			strcpy(((*array)[*arraySize]).description	, newElement->description	);
			strcpy(((*array)[*arraySize]).login			, newElement->login			);
			strcpy(((*array)[*arraySize]).password		, newElement->password		);

			++(*arraySize);

			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
}

