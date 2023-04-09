#pragma once
#include "include.h"

void InitScriptData();
long ReadLine(char* buf, char** cmd, char** value);
uchar ReadLanguage(char* buf);
long ReadOption(char* buf);
short ReadDeath(char* buf);
long ReadColor(char* buf);
char* InsertFileName(char* buf);
char* InsertString(char* buf);
char* InsertItemName(char* buf);
long GetInvitemRef(char* buf);
short GetSequenceSize(short* sequence);
void FreeLists();
void RenameExtension(char* name, char* ext);
