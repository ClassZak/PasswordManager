#include "PasswordStruct.h"

void FreePasswordStruct(struct PasswordStruct* password_struct)
{
	
	if(password_struct->name)
		free(password_struct->name);
	if(password_struct->description)
		free(password_struct->description);
	if(password_struct->login)
		free(password_struct->login);
	if(password_struct->password)
		free(password_struct->password);
}
void FreePasswordStructs(struct PasswordStruct* password_structs, size_t count)
{
	for(size_t i = 0; i != count; ++i)
		FreePasswordStruct(password_structs + i);
}


int AddNewPasswordStruct(struct PasswordStruct** array, size_t* arraySize, struct PasswordStruct* newElement)
{
	if (!array)
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
			(*array)->name_size			= newElement->name_size;
			(*array)->description_size	= newElement->description_size;
			(*array)->login_size		= newElement->login_size;
			(*array)->password_size		= newElement->password_size;

			(*array)->name			= (char*)malloc((*array)->name_size			+1);
			(*array)->description	= (char*)malloc((*array)->description_size	+1);
			(*array)->login			= (char*)malloc((*array)->login_size		+1);
			(*array)->password		= (char*)malloc((*array)->password_size		+1);
			if(
				(*array)->name==NULL		||
				(*array)->description==NULL	||
				(*array)->login==NULL		||
				(*array)->password==NULL
			)
				return EXIT_FAILURE;

			strcpy((*array)->name,			newElement->name		);
			strcpy((*array)->description,	newElement->description);
			strcpy((*array)->login,			newElement->login		);
			strcpy((*array)->password,		newElement->password	);
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
			for (size_t i = 0; i != *arraySize; ++i)
			{
				old_array[i].name_size			= (*array + i)->name_size;
				old_array[i].description_size	= (*array + i)->description_size;
				old_array[i].login_size			= (*array + i)->login_size;
				old_array[i].password_size		= (*array + i)->password_size;

				old_array[i].name			= malloc((*array + i)->name_size		+1);
				old_array[i].description	= malloc((*array + i)->description_size	+1);
				old_array[i].login			= malloc((*array + i)->login_size		+1);
				old_array[i].password		= malloc((*array + i)->password_size	+1);

				if (
					old_array[i].name == NULL ||
					old_array[i].description == NULL ||
					old_array[i].login == NULL ||
					old_array[i].password == NULL
				)
					return EXIT_FAILURE;

				strcpy(old_array[i].name,			(*array + i)->name);
				strcpy(old_array[i].description,	(*array + i)->description);
				strcpy(old_array[i].login,			(*array + i)->login);
				strcpy(old_array[i].password,		(*array + i)->password);
			}
		
		free(*array);
		*array=malloc(sizeof(struct PasswordStruct) * ((*arraySize)+1));
		
		if (old_array && *array)
		{
			for (size_t i = 0; i != *arraySize; ++i)
			{
				((*array)+i)->name_size			= old_array[i].name_size;
				((*array)+i)->description_size	= old_array[i].description_size;
				((*array)+i)->login_size		= old_array[i].login_size;
				((*array)+i)->password_size		= old_array[i].password_size;

				((*array) + i)->name			= malloc(old_array[i].name_size			+1);
				((*array) + i)->description		= malloc(old_array[i].description_size	+1);
				((*array) + i)->login			= malloc(old_array[i].login_size		+1);
				((*array) + i)->password		= malloc(old_array[i].password_size		+1);

				if(
					((*array) + i)->name		== NULL ||
					((*array) + i)->description	== NULL ||
					((*array) + i)->login		== NULL ||
					((*array) + i)->password	== NULL
				)
					return EXIT_FAILURE;

				strcpy(((*array)+i)->name		,old_array[i].name			);
				strcpy(((*array)+i)->description,old_array[i].description	);
				strcpy(((*array)+i)->login		,old_array[i].login			);
				strcpy(((*array)+i)->password	,old_array[i].password		);
				
			}
			
			((*array)[*arraySize]).name_size		= newElement->name_size;
			((*array)[*arraySize]).description_size	= newElement->description_size;
			((*array)[*arraySize]).login_size		= newElement->login_size;
			((*array)[*arraySize]).password_size	= newElement->password_size;

			((*array)[*arraySize]).name			= malloc(newElement->name_size			+ 1);
			((*array)[*arraySize]).description	= malloc(newElement->description_size	+ 1);
			((*array)[*arraySize]).login		= malloc(newElement->login_size			+ 1);
			((*array)[*arraySize]).password		= malloc(newElement->password_size		+ 1);

			if (
				((*array)[*arraySize]).name == NULL ||
				((*array)[*arraySize]).description == NULL ||
				((*array)[*arraySize]).login == NULL ||
				((*array)[*arraySize]).password == NULL
			)
				return EXIT_FAILURE;

			
			strcpy(((*array)[*arraySize]).name,			newElement->name		);
			strcpy(((*array)[*arraySize]).description,	newElement->description	);
			strcpy(((*array)[*arraySize]).login,		newElement->login		);
			strcpy(((*array)[*arraySize]).password,		newElement->password	);
			
			
			++(*arraySize);
			
			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
}

