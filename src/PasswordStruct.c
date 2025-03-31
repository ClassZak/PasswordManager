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

struct PasswordStruct* ReadAllPasswordStructs(FILE** file, size_t* size)
{
	*size=0;
	long file_size= getFileSize(file);
	if(file_size%sizeof(struct PasswordStruct) || !file_size)
		return NULL;

	int passwords_quantity=file_size/sizeof(struct PasswordStruct);
	struct PasswordStruct* passwords=malloc(passwords_quantity*sizeof(struct PasswordStruct));

	for(int i=0;i<passwords_quantity;++i)
		ReadPasswordStruct(file,passwords+i);
	
	*size=passwords_quantity;

	return passwords;
}

