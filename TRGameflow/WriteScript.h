#pragma once
#include "include.h"

void WriteScriptFile(FILE* fp);
void WriteStringList(FILE* fp, long nStrings, char** list);
void WriteGameStrings(FILE* fp);
void WriteSequences(FILE* fp);
void WriteGeneralStrings(FILE* fp);
void WriteItemStrings(FILE* fp);
