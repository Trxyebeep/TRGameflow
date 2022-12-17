#include "helpers.h"

static long num_namebuffer;
static char GF_namebuffer[256][256];

static long num_stringbuffer;
static char GF_stringbuffer[1000][256];	//1k possible strings, 256 chars max each

static long num_itemnamebuffer;
static char GF_itemnamebuffer[1000][256];

static char* inv_item_refs[30] =
{
	"PISTOLS",
	"SHOTGUN",
	"AUTOPISTOLS",
	"UZIS",
	"HARPOON",
	"M16",
	"ROCKET",
	"GRENADE",
	"PISTOLS_AMMO",
	"SHOTGUN_AMMO",
	"AUTOPISTOLS_AMMO",
	"UZI_AMMO",
	"HARPOON_AMMO",
	"M16_AMMO",
	"ROCKET_AMMO",
	"GRENADE_AMMO",
	"FLARES",
	"MEDI",
	"BIGMEDI",
	"PICKUP1",
	"PICKUP2",
	"PUZZLE1",
	"PUZZLE2",
	"PUZZLE3",
	"PUZZLE4",
	"KEY1",
	"KEY2",
	"KEY3",
	"KEY4",
	"CRYSTAL"
};

void InitScriptData()
{
	//write out default puzzle/key/pickup strings
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		puzzle1_strings[i] = InsertItemName("P1");
		puzzle2_strings[i] = InsertItemName("P2");
		puzzle3_strings[i] = InsertItemName("P3");
		puzzle4_strings[i] = InsertItemName("P4");

		key1_strings[i] = InsertItemName("K1");
		key2_strings[i] = InsertItemName("K2");
		key3_strings[i] = InsertItemName("K3");
		key4_strings[i] = InsertItemName("K4");

		pickup1_strings[i] = InsertItemName("P1");
		pickup2_strings[i] = InsertItemName("P2");
	}

	//init default gameflow data
	gameflow.firstOption = EXIT_TO_TITLE;
	gameflow.title_replace = -1;
	gameflow.ondeath_demo_mode = EXIT_TO_TITLE;
	gameflow.ondeath_ingame = 0;
	gameflow.noinput_time = 9000;
	gameflow.on_demo_interrupt = EXIT_TO_TITLE;
	gameflow.on_demo_end = EXIT_TO_TITLE;
	gameflow.num_levels = 0;
	gameflow.num_picfiles = 0;
	gameflow.num_titlefiles = 0;
	gameflow.num_fmvfiles = 0;
	gameflow.num_cutfiles = 0;
	gameflow.num_demos = 0;
	gameflow.title_track = 2;
	gameflow.singlelevel = -1;

	gameflow.demoversion = 0;
	gameflow.title_disabled = 0;
	gameflow.cheatmodecheck_disabled = 0;
	gameflow.noinput_timeout = 0;
	gameflow.loadsave_disabled = 0;
	gameflow.screensizing_disabled = 0;
	gameflow.lockout_optionring = 0;
	gameflow.dozy_cheat_enabled = 0;
	gameflow.cyphered_strings = 0;
	gameflow.gym_enabled = 0;
	gameflow.play_any_level = 0;
	gameflow.cheat_enable = 0;
	gameflow.securitytag = 0;

	gameflow.cypher_code = 0;
	gameflow.language = 0;
	gameflow.secret_track = 0;
	gameflow.stats_track = 0;
}

long ReadLine(char* buf, char** cmd, char** value)	//read buf then seperate the content into cmd: value
{
	long l, skip, nLine, i, n, foundVal;
	static char line[MAX_LINE + (MAX_LINE >> 2)];

	memset(line, '\0', sizeof(line));	//clear out line

	l = strlen(buf);
	nLine = 0;
	skip = -1;

	//clean up line from comments/new lines. output in line array.
	for (i = 0, n = 0; i < l; i++)
	{
		if (buf[i] == '/' && buf[i + 1] == '*')	// /**/ style comment
		{
			i++;	//skip *
			n++;
			continue;
		}

		if (buf[i] == '*' && buf[i + 1] == '/')	//end of /**/ style comment
		{
			if (!n)
			{
				printf("found end of comment without a start\n");
				return 0;
			}

			i++;	//skip /
			n--;
			continue;
		}

		if ((buf[i] == '/' && buf[i + 1] == '/') ||	// //style comment
			buf[i] == '*' ||						//part of a comment
			buf[i] == '\n')							//or line end
		{
			//mark line is done

			if (skip < 0)
				line[nLine] = '\0';
			else
				line[skip] = '\0';

			break;
		}

		if (!n)	//not in a comment
		{
			if (buf[i] == ' ' || buf[i] == '\t')	//skip spaces and tabs
			{
				if (skip < 0)
					skip = nLine;
			}
			else
				skip = -1;

			line[nLine++] = buf[i];
		}
	}

	//now read resulting line and separate into cmd: value
	l = strlen(line);
	foundVal = 0;

	for (i = 0; i < MAX_LINE; i++)
	{
		if (line[i] == ':')	//found colon
		{
			while (line[i + 1] == ':') i++;	//in case of multiple colon typos..

			foundVal = 1;
			break;
		}

		if (!line[i])	//reached end of line
			break;
	}

	if (!i)
		return 0;

	nLine = 0;
	while (line[nLine] == ' ' || line[nLine] == '\t') nLine++;	//skip spaces and tabs

	*cmd = &line[nLine];	//this is where the command starts

	if (!foundVal)
	{
		*value = &line[nLine];	//didn't find a value so point to command
		return 1;
	}

	for (; nLine < i; nLine++)
		line[nLine] = toupper(line[nLine]);

	line[i] = '\0';	//mark we are done with the command

	nLine = i + 1;
	while (line[nLine] == ' ' || line[nLine] == '\t') nLine++;	//skip any spaces and tabs between cmd and value

	if (line[nLine] == '"' || line[nLine] == '\'')	//found " or ' around the value (in case of a string/character!)
	{
		nLine++;	//skip first one
		l = strlen(&line[nLine]);

		if (line[nLine + l - 1] == '"' || line[nLine + l - 1] == '\'')
			line[nLine + l - 1] = '\0';	//and delete the second
	}

	*value = &line[nLine];
	return 1;
}

uchar ReadLanguage(char* buf)
{
	_strupr(buf);

	if (!strcmp(buf, "ENGLISH"))
		return 0;

	if (!strcmp(buf, "FRENCH"))
		return 1;

	if (!strcmp(buf, "GERMAN"))
		return 2;

	if (!strcmp(buf, "AMERICAN"))
		return 3;

	if (!strcmp(buf, "JAPANESE"))
		return 4;

	if (!strcmp(buf, "ITALIAN"))
		return 5;

	if (!strcmp(buf, "SPANISH"))
		return 6;

	return 0;	//if none just default to English.
}

long ReadOption(char* buf)
{
	//strncmp because some values might have extra data after the keyword

	if (!strncmp(buf, "EXIT_TO_TITLE", strlen(buf)))
		return EXIT_TO_TITLE;

	if (!strncmp(buf, "LEVEL", strlen(buf)))
		return STARTGAME | atoi(buf + strlen("LEVEL"));

	if (!strncmp(buf, "DEMO", strlen(buf)))
		return STARTDEMO | atoi(buf + strlen("DEMO"));

	if (!strncmp(buf, "SEQUENCE", strlen(buf)))
		return STARTGAME | atoi(buf + strlen("SEQUENCE"));

	if (!strncmp(buf, "EXITGAME", strlen(buf)))
		return EXITGAME;

	return -1;	//this is the "do nothing" flag
}

char* InsertFileName(char* buf)
{
	strcpy(GF_namebuffer[num_namebuffer++], buf);
	return GF_namebuffer[num_namebuffer - 1];
}

char* InsertString(char* buf)
{
	strcpy(GF_stringbuffer[num_stringbuffer++], buf);
	return GF_stringbuffer[num_stringbuffer - 1];
}

char* InsertItemName(char* buf)
{
	strcpy(GF_itemnamebuffer[num_itemnamebuffer++], buf);
	return GF_itemnamebuffer[num_itemnamebuffer - 1];
}

long GetInvitemRef(char* buf)
{
	for (int i = 0; i < sizeof(inv_item_refs) / sizeof(char*); i++)
	{
		if (!strcmp(buf, inv_item_refs[i]))
			return i;
	}

	return -1;
}

short GetSequenceSize(short* sequence)	//returns sequence size in BYTES, not shorts!
{
	for (int i = 0; i < MAX_SEQUENCE; i++)
	{
		switch (sequence[i])
		{
			//These events have one extra 2bytes of data so increment and continue
		case GFE_PICTURE:
		case GFE_PLAYFMV:
		case GFE_STARTLEVEL:
		case GFE_CUTSCENE:
		case GFE_DEMOPLAY:
		case GFE_JUMPTO_SEQ:
		case GFE_SETTRACK:
		case GFE_LOADINGPIC:
		case GFE_CUTANGLE:
		case GFE_NOFLOOR:
		case GFE_ADD2INV:
		case GFE_STARTANIM:
		case GFE_NUMSECRETS:
			i++;

			//These events do not have any extra data so continue
		case GFE_LIST_START:
		case GFE_LIST_END:
		case GFE_LEVCOMPLETE:
		case GFE_SUNSET:
		case GFE_DEADLY_WATER:
		case GFE_REMOVE_WEAPONS:
		case GFE_GAMECOMPLETE:
		case GFE_KILL2COMPLETE:
		case GFE_REMOVE_AMMO:
			break;

			//End of sequence: return amount of 2bytes we cycled through
		case GFE_END_SEQ:
			return (i * sizeof(short)) + sizeof(short);	//+2 for the end sequence itself
		}
	}

	printf("Sequence has no END!!!! exitting");
	_fcloseall();
	exit(0);
}

void FreeLists()
{
	for (int i = 0; i < num_levels; i++)
		free(GF_level_sequence_list[i]);
}

void RenameExtension(char* name, char* ext)
{
	long l, n;

	n = 0;
	l = strlen(name);

	while (name[l] != '.' && l > 0)
	{
		l--;
		n++;
	}

	if (!l)
	{
		//file has no extension, so just print new one at the end
		strcat(name, ext);
		return;
	}

	//now clear out original extension
	l = strlen(name);
	name[l - n] = '\0';

	//now add the new extension
	strcat(name, ext);
}
