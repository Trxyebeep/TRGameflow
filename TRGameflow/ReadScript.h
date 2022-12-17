#pragma once
#include "include.h"

void ReadScript(FILE* fp);
void ReadMainOptions(FILE* fp);
void ReadTitleOptions(FILE* fp);
void ReadLevelOptions(FILE* fp, short* sequence, short level, long type);
void ReadGeneralStrings(FILE* fp);
