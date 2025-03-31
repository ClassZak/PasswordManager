#pragma once
#include <stdlib.h>

static char** HistoryBuffer;
static size_t HistoryLenght;

void AddCommandToHistory(const char* command);
