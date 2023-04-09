#include "ReadScript.h"
#include "WriteScript.h"
#include "helpers.h"

/*script file parsing routines*/

GAMEFLOW_INFO gameflow;
char gameflow_description[256] = "Tomb Raider II PC Internal Development Version (c) Core Design Ltd 1997";
char* puzzle1_strings[NUM_ELEMENTS];
char* puzzle2_strings[NUM_ELEMENTS];
char* puzzle3_strings[NUM_ELEMENTS];
char* puzzle4_strings[NUM_ELEMENTS];
char* key1_strings[NUM_ELEMENTS];
char* key2_strings[NUM_ELEMENTS];
char* key3_strings[NUM_ELEMENTS];
char* key4_strings[NUM_ELEMENTS];
char* pickup1_strings[NUM_ELEMENTS];
char* pickup2_strings[NUM_ELEMENTS];
char* LevelName_Strings[NUM_ELEMENTS];
char* game_strings[GT_NUM_GAMESTRINGS];
char* pc_strings[PCSTR_NUM_STRINGS];
char* ps_strings[PSSTR_NUM_STRINGS];

short GF_valid_demos[NUM_ELEMENTS];

short num_titlefiles;
char* GF_titlefilenames[NUM_ELEMENTS];

short num_fmvfiles;
char* GF_fmvfilenames[NUM_ELEMENTS];

short num_picfiles;
char* GF_picfilenames[NUM_ELEMENTS];

short num_demos;
short num_levels = 1;	//0 is always gym
char* GF_levelfilenames[NUM_ELEMENTS];

short num_cutfiles;
char* GF_cutscenefilenames[NUM_ELEMENTS];

short GF_FrontEnd[MAX_SEQUENCE];
short* GF_level_sequence_list[NUM_ELEMENTS];

void ReadScript(FILE* fp)
{
	FILE* stringsFile;
	char* cmd;
	char* value;
	static char buf[512];

	InitScriptData();

	printf("begin reading script!\n");
	printf("\n");

	while (fgets(buf, MAX_LINE, fp))	//get main categories and go from there
	{
		if (!ReadLine(buf, &cmd, &value))
			continue;

		if (!strcmp(cmd, "DESCRIPTION"))
		{
			printf("Found Description\n");
			printf("\n");
			strcpy(gameflow_description, value);
			continue;
		}

		if (!strcmp(cmd, "OPTIONS"))
		{
			printf("Found main options\n");
			ReadMainOptions(fp);
			continue;
		}

		if (!strcmp(cmd, "TITLE"))
		{
			printf("Found title options\n");
			ReadTitleOptions(fp);
			continue;
		}

		if (!strcmp(cmd, "FRONTEND"))
		{
			printf("Found frontend options\n");
			ReadLevelOptions(fp, GF_FrontEnd, 0, 2);
			continue;
		}

		if (!strcmp(cmd, "GYM"))
		{
			printf("Found gym block\n");
			LevelName_Strings[0] = InsertString(value);
			GF_level_sequence_list[0] = (short*)malloc(sizeof(short) * MAX_SEQUENCE);
			ReadLevelOptions(fp, GF_level_sequence_list[0], 0, 1);
			gameflow.gym_enabled = 1;
			continue;
		}

		if (!strcmp(cmd, "LEVEL"))
		{
			printf("Found level block\n");
			LevelName_Strings[num_levels] = InsertString(value);
			GF_level_sequence_list[num_levels] = (short*)malloc(sizeof(short) * MAX_SEQUENCE);
			ReadLevelOptions(fp, GF_level_sequence_list[num_levels], num_levels, 1);
			num_levels++;
			continue;
		}

		if (!strcmp(cmd, "DEMOLEVEL"))
		{
			printf("Found demo level block\n");
			LevelName_Strings[num_levels] = InsertString(value);
			GF_level_sequence_list[num_levels] = (short*)malloc(sizeof(short) * MAX_SEQUENCE);
			GF_valid_demos[num_demos++] = num_levels;
			ReadLevelOptions(fp, GF_level_sequence_list[num_levels], num_levels, 1);
			num_levels++;
			continue;
		}

		if (!strcmp(cmd, "GAMESTRINGS"))
		{
			printf("Found game strings file: %s\n", value);
			stringsFile = fopen(value, "r");

			if (!stringsFile)
			{
				printf("Failed to open strings file!!! %s\n", value);
				continue;
			}

			ReadGeneralStrings(stringsFile);
			fclose(stringsFile);
			continue;
		}

		printf("FOUND UNKNOWN CMD: VALUE.. %s, %s\n", cmd, value);
	}

	gameflow.num_levels = num_levels;
	gameflow.num_fmvfiles = num_fmvfiles;
	gameflow.num_cutfiles = num_cutfiles;
	gameflow.num_picfiles = num_picfiles;
	gameflow.num_titlefiles = num_titlefiles;
	gameflow.num_demos = num_demos;

	printf("Finished reading script file\n");
}

void ReadMainOptions(FILE* fp)
{
	char* cmd;
	char* value;
	static char buf[MAX_LINE + (MAX_LINE >> 2)];

	printf("reading main options:\n");

	while (fgets(buf, MAX_LINE, fp))
	{
		if (!ReadLine(buf, &cmd, &value))
			continue;

		if (!strcmp(cmd, "LANGUAGE"))
		{
			printf("Found language: %s\n", value);
			gameflow.language = ReadLanguage(value);
			continue;
		}

		if (!strcmp(cmd, "SECRET_TRACK"))
		{
			printf("Found secret track: %s\n", value);
			gameflow.secret_track = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "CYPHER_CODE"))
		{
			printf("Found cypher code: %s\n", value);
			gameflow.cyphered_strings = 1;
			gameflow.cypher_code = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "FIRSTOPTION"))
		{
			printf("Found first option: %s\n", value);
			gameflow.firstOption = ReadOption(value);
			continue;
		}

		if (!strcmp(cmd, "TITLE_REPLACE"))
		{
			printf("Found title replace: %s\n", value);
			gameflow.title_replace = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "ONDEATH_DEMO_MODE"))
		{
			printf("Found ondeath demo: %s\n", value);
			gameflow.ondeath_demo_mode = ReadOption(value);
			continue;
		}

		if (!strcmp(cmd, "ONDEATH_INGAME"))
		{
			printf("Found ondeath ingame: %s\n", value);
			gameflow.ondeath_ingame = ReadOption(value);
			continue;
		}

		if (!strcmp(cmd, "ON_DEMO_INTERRUPT"))
		{
			printf("Found ondemo interrupt: %s\n", value);
			gameflow.on_demo_interrupt = ReadOption(value);
			continue;
		}

		if (!strcmp(cmd, "ON_DEMO_END"))
		{
			printf("Found ondemo end: %s\n", value);
			gameflow.on_demo_end = ReadOption(value);
			continue;
		}

		if (!strcmp(cmd, "NOINPUT_TIME"))
		{
			printf("Found noinput time: %s\n", value);
			gameflow.noinput_time = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "SINGLELEVEL"))
		{
			printf("Found single level: %s\n", value);
			gameflow.singlelevel = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "DEMOVERSION"))
		{
			printf("Found demo mode");
			gameflow.demoversion = 1;
			continue;
		}

		if (!strcmp(cmd, "TITLE_DISABLED"))
		{
			printf("Found disable title");
			gameflow.title_disabled = 1;
			continue;
		}

		if (!strcmp(cmd, "CHEATMODECHECK_DISABLED"))
		{
			printf("Found cheatmodecheck disabled");
			gameflow.cheatmodecheck_disabled = 1;
			continue;
		}

		if (!strcmp(cmd, "NOINPUT_TIMEOUT"))
		{
			printf("Found noinput timeout\n");
			gameflow.noinput_timeout = 1;
			continue;
		}

		if (!strcmp(cmd, "LOADSAVE_DISABLED"))
		{
			printf("Found loadsave disabled");
			gameflow.loadsave_disabled = 1;
			continue;
		}

		if (!strcmp(cmd, "SCREENSIZING_DISABLED"))
		{
			printf("Found screensizing disabled");
			gameflow.screensizing_disabled = 1;
			continue;
		}

		if (!strcmp(cmd, "LOCKOUT_OPTIONRING"))
		{
			printf("Found lockout optionring");
			gameflow.lockout_optionring = 1;
			continue;
		}

		if (!strcmp(cmd, "DOZY_CHEAT_ENABLED"))
		{
			printf("Found dozy cheat enabled");
			gameflow.dozy_cheat_enabled = 1;
			continue;
		}

		if (!strcmp(cmd, "SELECT_ANY_LEVEL"))
		{
			printf("Found select any level");
			gameflow.play_any_level = 1;
			continue;
		}

		if (!strcmp(cmd, "ENABLE_CHEAT_KEY"))
		{
			printf("Found enable cheats");
			gameflow.cheat_enable = 1;
			continue;
		}

		if (!strcmp(cmd, "USE_SECURITY_TAG"))
		{
			printf("Found security tag");
			gameflow.securitytag = 1;
			continue;
		}

		/*New main settings commands*/

		if (!strcmp(cmd, "MAP"))
		{
			printf("Found map option");
			gameflow.map_enabled = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "GLOBE"))
		{
			printf("Found globe option");
			gameflow.globe_enabled = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "WATER"))
		{
			printf("Found force water color option");
			gameflow.force_water_color = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "END"))
		{
			printf("Found END.\n");
			break;
		}

		printf("FOUND UNKNOWN COMMAND. CMD: VALUE.. %s, %s\n", cmd, value);
	}

	printf("Finished reading main options\n");
	printf("\n");
}

void ReadTitleOptions(FILE* fp)
{
	char* cmd;
	char* value;
	static char buf[MAX_LINE + (MAX_LINE >> 2)];

	printf("reading title options:\n");

	while (fgets(buf, MAX_LINE, fp))
	{
		if (!ReadLine(buf, &cmd, &value))
			continue;

		if (!strcmp(cmd, "GAME"))
		{
			printf("Found title level file %s\n", value);
			GF_titlefilenames[num_titlefiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "PCFILE"))
		{
			printf("Found PC file: %s\n", value);
			GF_titlefilenames[num_titlefiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "PSXFILE"))
		{
			printf("Found PSX file: ignored\n");
			continue;
		}

		if (!strcmp(cmd, "TRACK"))
		{
			printf("Found title track: %s\n", value);
			gameflow.title_track = atoi(value);
			continue;
		}

		if (!strcmp(cmd, "END"))
		{
			printf("Found END.\n");
			break;
		}

		printf("FOUND UNKNOWN COMMAND. CMD: VALUE.. %s, %s\n", cmd, value);
	}

	printf("Finished reading title options\n");
	printf("\n");
}

void ReadLevelOptions(FILE* fp, short* sequence, short level, long type)
{
	char* cmd;
	char* value;
	long l;
	static char buf[MAX_LINE + (MAX_LINE >> 2)];

	printf("reading level options:\n");

	while (fgets(buf, MAX_LINE, fp))
	{
		if (!ReadLine(buf, &cmd, &value))
			continue;

		if (!strcmp(cmd, "FMV"))	//front end (fmv) stuff here because levels can have FMV commands :]
		{
			printf("Found fmv %s, #%i\n", value, num_fmvfiles);
			_strupr(value);
			sequence[0] = GFE_PLAYFMV;
			sequence[1] = num_fmvfiles;
			sequence += 2;
			GF_fmvfilenames[num_fmvfiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "PCFMV"))
		{
			printf("Found PCFmv %s, #%i\n", value, num_fmvfiles);
			GF_fmvfilenames[num_fmvfiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "PSXFMV"))
		{
			printf("Found PSXFmv, ignoring");
			continue;
		}

		if (!strcmp(cmd, "FMV_START") || !strcmp(cmd, "FMV_END"))
		{
			printf("Found FMV_START/END, ignoring\n");
			continue;
		}

		if (!strcmp(cmd, "GAME"))
		{
			printf("Found level file %s\n", value);
			sequence[0] = GFE_STARTLEVEL;
			sequence[1] = level;
			sequence += 2;
			GF_levelfilenames[level] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "CUT"))
		{
			printf("Found cut file %s\n", value);
			sequence[0] = GFE_CUTSCENE;
			sequence[1] = num_cutfiles;
			sequence += 2;
			GF_cutscenefilenames[num_cutfiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "CUTANGLE"))
		{
			printf("Found cut angle %s\n", value);
			sequence[0] = GFE_CUTANGLE;
			sequence[1] = atoi(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "DEMO"))
		{
			printf("Found demo level file %s\n", value);
			sequence[0] = GFE_DEMOPLAY;
			sequence[1] = level;
			sequence += 2;
			GF_levelfilenames[level] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "TRACK"))
		{
			printf("Found track %s\n", value);
			sequence[0] = GFE_SETTRACK;
			sequence[1] = atoi(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "SUNSET"))
		{
			printf("Found sunset\n");
			*sequence++ = GFE_SUNSET;
			continue;
		}

		if (!strcmp(cmd, "LOAD_PIC"))
		{
			printf("Found load pic %s, #%i\n", value, num_picfiles);
			sequence[0] = GFE_LOADINGPIC;
			sequence[1] = num_picfiles;
			sequence += 2;
			GF_picfilenames[num_picfiles++] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "DEADLY_WATER"))
		{
			printf("Found deadly water\n");
			*sequence++ = GFE_DEADLY_WATER;
			continue;
		}

		if (!strcmp(cmd, "REMOVE_WEAPONS"))
		{
			printf("Found remove weps\n");
			*sequence++ = GFE_REMOVE_WEAPONS;
			continue;
		}

		if (!strcmp(cmd, "REMOVE_AMMO"))
		{
			printf("Found remove ammo\n");
			*sequence++ = GFE_REMOVE_AMMO;
			continue;
		}

		if (!strcmp(cmd, "NOFLOOR"))
		{
			printf("Found nofloor\n");
			sequence[0] = GFE_NOFLOOR;
			sequence[1] = atoi(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "BONUS"))
		{
			printf("Found bonus %s\n", value);
			l = GetInvitemRef(value);

			if (l == -1)
			{
				printf("GetInvitemRef failed to get %s\n", value);
				continue;
			}

			sequence[0] = GFE_ADD2INV;
			sequence[1] = (short)l;
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "STARTINV"))
		{
			printf("Found startinv %s\n", value);
			l = GetInvitemRef(value);

			if (l == -1)
			{
				printf("GetInvitemRef failed to get %s\n", value);
				continue;
			}

			sequence[0] = GFE_ADD2INV;
			sequence[1] = (short)l + 1000;	//this is how the engine differentiates between BONUS and STARTINV
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "STARTANIM"))
		{
			printf("Found startanim\n");
			sequence[0] = GFE_STARTANIM;
			sequence[1] = atoi(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "SECRETS"))
		{
			printf("Found secrets\n");
			sequence[0] = GFE_NUMSECRETS;
			sequence[1] = atoi(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "KILLTOCOMPLETE"))
		{
			printf("Found kill2complete\n");
			*sequence++ = GFE_KILL2COMPLETE;
			continue;
		}

		if (!strcmp(cmd, "PUZZLE1"))
		{
			printf("Found puzzle1 %s\n", value);

			if (type != 2)
				puzzle1_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PUZZLE2"))
		{
			printf("Found puzzle2 %s\n", value);

			if (type != 2)
				puzzle2_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PUZZLE3"))
		{
			printf("Found puzzle3 %s\n", value);

			if (type != 2)
				puzzle3_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PUZZLE4"))
		{
			printf("Found puzzle4 %s\n", value);

			if (type != 2)
				puzzle4_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PICKUP1"))
		{
			printf("Found pickup1 %s\n", value);

			if (type != 2)
				pickup1_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PICKUP2"))
		{
			printf("Found pickup2 %s\n", value);

			if (type != 2)
				pickup2_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "KEY1"))
		{
			printf("Found key1 %s\n", value);

			if (type != 2)
				key1_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "KEY2"))
		{
			printf("Found key2 %s\n", value);

			if (type != 2)
				key2_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "KEY3"))
		{
			printf("Found key3 %s\n", value);

			if (type != 2)
				key3_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "KEY4"))
		{
			printf("Found key4 %s\n", value);

			if (type != 2)
				key4_strings[level] = InsertString(value);

			continue;
		}

		if (!strcmp(cmd, "PCDEMO"))
		{
			printf("Found pcdemo %s\n", value);
			sequence[0] = GFE_DEMOPLAY;
			sequence[1] = level;
			sequence += 2;
			GF_levelfilenames[level] = InsertFileName(value);
			continue;
		}

		if (!strcmp(cmd, "PSXDEMO"))
		{
			printf("Found psxdemo, use pcdemo instead!");
			continue;
		}

		/*NEW EVENTS*/

		if (!strcmp(cmd, "RAIN"))
		{
			printf("Found rain\n");
			*sequence++ = GFE_RAIN;
			continue;
		}

		if (!strcmp(cmd, "SNOW"))
		{
			printf("Found snow\n");
			*sequence++ = GFE_SNOW;
			continue;
		}

		if (!strcmp(cmd, "WATERPARTS"))
		{
			printf("Found waterparts\n");
			*sequence++ = GFE_WATER_PARTS;
			continue;
		}

		if (!strcmp(cmd, "COLD"))
		{
			printf("Found cold\n");
			*sequence++ = GFE_COLD;
			continue;
		}

		if (!strcmp(cmd, "DEATH"))
		{
			printf("Found death\n");
			sequence[0] = GFE_DEATHTILE;
			sequence[1] = ReadDeath(value);
			sequence += 2;
			continue;
		}

		if (!strcmp(cmd, "WATERCLR"))
		{
			printf("Found water color\n");
			l = ReadColor(value);
			sequence[0] = GFE_WATERCLR;
			sequence[1] = l & 0xFFFF;			//LOWORD
			sequence[2] = (l >> 16) & 0xFFFF;	//HIWORD
			sequence += 3;
		}

		if (!strcmp(cmd, "COMPLETE"))
		{
			printf("Found complete\n");
			*sequence++ = GFE_LEVCOMPLETE;
			continue;
		}

		if (!strcmp(cmd, "GAMECOMPLETE"))
		{
			printf("Found game complete\n");
			*sequence++ = GFE_GAMECOMPLETE;
			continue;
		}

		if (!strcmp(cmd, "END"))
		{
			printf("Found END.\n");
			sequence[0] = GFE_END_SEQ;
			break;
		}

		printf("FOUND UNKNOWN COMMAND. CMD: VALUE.. %s, %s\n", cmd, value);
	}

	printf("Finished reading level options\n");
	printf("\n");
}

void ReadGeneralStrings(FILE* fp)
{
	char* cmd;
	char* value;
	long l;
	static char buf[MAX_LINE + (MAX_LINE >> 2)];

	printf("reading general strings:\n");

	while (fgets(buf, MAX_LINE, fp))
	{
		if (!ReadLine(buf, &cmd, &value))
			continue;

		if (!strcmp(cmd, "GAME_STRINGS"))
		{
			printf("Found game strings");
			l = 0;

			while (fgets(buf, MAX_LINE, fp))
			{
				if (!ReadLine(buf, &cmd, &value))
					continue;

				if (!strcmp(cmd, "END"))
				{
					printf("Found game strings end. number of strings: %i\n", l);
					break;
				}

				game_strings[l++] = InsertString(value);
			}

			continue;
		}

		if (!strcmp(cmd, "PC_STRINGS"))
		{
			printf("Found PC strings");
			l = 0;

			while (fgets(buf, MAX_LINE, fp))
			{
				if (!ReadLine(buf, &cmd, &value))
					continue;

				if (!strcmp(cmd, "END"))
				{
					printf("Found PC strings end. number of strings: %i\n", l);
					break;
				}

				pc_strings[l++] = InsertString(value);
			}

			continue;
		}

		if (!strcmp(cmd, "PSX_STRINGS"))
		{
			printf("Found PSX strings");
			l = 0;

			while (fgets(buf, MAX_LINE, fp))
			{
				if (!ReadLine(buf, &cmd, &value))
					continue;

				if (!strcmp(cmd, "END"))
				{
					printf("Found PSX strings end. number of strings: %i\n", l);
					break;
				}

				ps_strings[l++] = InsertString(value);
			}

			continue;
		}

		if (!strcmp(cmd, "END"))
		{
			printf("Found END.\n");
			break;
		}
	}

	printf("Finished reading general strings\n");
	printf("\n");
}
