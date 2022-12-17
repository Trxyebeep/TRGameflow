#include "WriteScript.h"
#include "ReadScript.h"
#include "helpers.h"

void WriteScriptFile(FILE* fp)
{
	ulong version;
	short size;

	printf("Writing script file\n");

	printf("Writing version\n");
	version = 3;
	fwrite(&version, sizeof(ulong), 1, fp);

	printf("Writing description\n");
	fwrite(gameflow_description, 1, 256, fp);

	size = sizeof(GAMEFLOW_INFO);
	printf("Writing size: %i\n", size);
	fwrite(&size, sizeof(short), 1, fp);

	printf("Writing gameflow data\n");
	fwrite(&gameflow, size, 1, fp);

	printf("Writing general strings\n");
	WriteGameStrings(fp);

	printf("Writing sequence data\n");
	WriteSequences(fp);

	if (gameflow.num_demos)
	{
		printf("Writing demo level list\n");
		fwrite(&GF_valid_demos, sizeof(short), gameflow.num_demos, fp);
	}
	else
		printf("No demo level list to write\n");

	printf("Writing GAME and PC strings\n");
	WriteGeneralStrings(fp);

	printf("Writie item strings\n");
	WriteItemStrings(fp);

	printf("Finished writing script file\n");

	FreeLists();
}

void WriteStringList(FILE* fp, long nStrings, char** list)
{
	char* str;
	long lp, l;
	short offset;
	uchar key;

	//write string offsets. relative to the start of the string list, not real file offsets, hence the 0.
	offset = 0;

	for (lp = 0; lp < nStrings; lp++)
	{
		str = list[lp];
		l = strlen(str);

		fwrite(&offset, sizeof(short), 1, fp);	//write current offset
		offset += (short)l + 1;					//go to next string
	}

	fwrite(&offset, sizeof(short), 1, fp);	//write final offset = the size of the entire list

	//encrypt strings if needed
	key = 0;	//encrypt/decrypt key

	if (gameflow.cyphered_strings)
	{
		key = gameflow.cypher_code;

		for (lp = 0; lp < nStrings; lp++)
		{
			str = list[lp];
			l = strlen(str);

			for (int j = 0; j < l; j++)
				str[j] ^= key;
		}
	}

	//now write the strings as they are

	for (lp = 0; lp < nStrings; lp++)
	{
		str = list[lp];
		fprintf(fp, str);
		fwrite(&key, sizeof(uchar), 1, fp);
	}
}

void WriteGameStrings(FILE* fp)
{
	printf("Writing level names\n");
	WriteStringList(fp, gameflow.num_levels, LevelName_Strings);

	printf("Writing picture names\n");
	WriteStringList(fp, gameflow.num_picfiles, GF_picfilenames);
	
	printf("Writing title file names\n");
	WriteStringList(fp, gameflow.num_titlefiles, GF_titlefilenames);

	printf("Writing fmv file names\n");
	WriteStringList(fp, gameflow.num_fmvfiles, GF_fmvfilenames);

	printf("Writing level file names\n");
	WriteStringList(fp, gameflow.num_levels, GF_levelfilenames);

	printf("Writing cut file names\n");
	WriteStringList(fp, gameflow.num_cutfiles, GF_cutscenefilenames);
}

void WriteSequences(FILE* fp)
{
	long lp;
	short offset;

	//first, the sequence offsets
	offset = 0;

	for (lp = -1; lp < gameflow.num_levels; lp++)
	{
		if (lp < 0) //first sequence is always the frontend one!
		{
			fwrite(&offset, sizeof(short), 1, fp);
			offset = GetSequenceSize(GF_FrontEnd);
			fwrite(&offset, sizeof(short), 1, fp);
			continue;
		}

		offset += GetSequenceSize(GF_level_sequence_list[lp]);
		fwrite(&offset, sizeof(short), 1, fp);
	}

	//now the sequences themselves

	for (lp = -1; lp < gameflow.num_levels; lp++)
	{
		if (lp < 0) //again, first is frontend
		{
			fwrite(GF_FrontEnd, 1, GetSequenceSize(GF_FrontEnd), fp);
			continue;
		}

		fwrite(GF_level_sequence_list[lp], 1, GetSequenceSize(GF_level_sequence_list[lp]), fp);
	}
}

void WriteGeneralStrings(FILE* fp)
{
	short n;

	//game strings store the number of, first
	n = GT_NUM_GAMESTRINGS;
	fwrite(&n, sizeof(short), 1, fp);

	//now write the strings themselves
	WriteStringList(fp, n, game_strings);

	//PC strings
	WriteStringList(fp, PCSTR_NUM_STRINGS, pc_strings);
}

void WriteItemStrings(FILE* fp)
{
	//write puzzle item strings
	WriteStringList(fp, gameflow.num_levels, puzzle1_strings);
	WriteStringList(fp, gameflow.num_levels, puzzle2_strings);
	WriteStringList(fp, gameflow.num_levels, puzzle3_strings);
	WriteStringList(fp, gameflow.num_levels, puzzle4_strings);

	//write pickup item strings
	WriteStringList(fp, gameflow.num_levels, pickup1_strings);
	WriteStringList(fp, gameflow.num_levels, pickup2_strings);

	//write key item strings
	WriteStringList(fp, gameflow.num_levels, key1_strings);
	WriteStringList(fp, gameflow.num_levels, key2_strings);
	WriteStringList(fp, gameflow.num_levels, key3_strings);
	WriteStringList(fp, gameflow.num_levels, key4_strings);
}
