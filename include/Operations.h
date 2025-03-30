#pragma once
#include "PasswordStruct.h"

void Dialog();

int AddNewPassword(FILE** file,const char* );
int DeletePassword(FILE** file);
struct PasswordStruct* FindPasswords();

