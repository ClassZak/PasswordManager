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
			for (size_t i = 0; i != *arraySize; i++)
			{
				strcpy(old_array[i].name,			(*array)->name);
				strcpy(old_array[i].description,	(*array)->description);
				strcpy(old_array[i].login,		(*array)->login);
				strcpy(old_array[i].password,		(*array)->password);
				
				old_array[i].name_size			= (*array)->name_size;
				old_array[i].description_size	= (*array)->description_size;
				old_array[i].login_size			= (*array)->login_size;
				old_array[i].password_size		= (*array)->password_size;
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
				
				((*array)+1)->name_size			= old_array[i].name_size;
				((*array)+1)->description_size	= old_array[i].description_size;
				((*array)+1)->login_size		= old_array[i].login_size;
				((*array)+1)->password_size		= old_array[i].password_size;
			}
			
			
			strcpy(((*array)[*arraySize]).name,			newElement->name			);
			strcpy(((*array)[*arraySize]).description,	newElement->description	);
			strcpy(((*array)[*arraySize]).login,			newElement->login			);
			strcpy(((*array)[*arraySize]).password,		newElement->password		);
			
			((*array)[*arraySize]).name_size		= newElement->name_size;
			((*array)[*arraySize]).description_size	= newElement->description_size;
			((*array)[*arraySize]).login_size		= newElement->login_size;
			((*array)[*arraySize]).password_size	= newElement->password_size;
			
			++(*arraySize);
			
			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
}

