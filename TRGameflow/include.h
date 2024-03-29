#pragma once
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

#define NUM_ELEMENTS	50
#define MAX_SEQUENCE	10000
#define MAX_LINE		384

typedef enum title_options_t
{
	STARTGAME = 0,
	STARTSAVEDGAME = 0x100,
	STARTCINE = 0x200,
	STARTFMV = 0x300,
	STARTDEMO = 0x400,
	EXIT_TO_TITLE = 0x500,
	LEVELCOMPLETE = 0x600,
	EXITGAME = 0x700,
	EXIT_TO_OPTION = 0x800,
	TITLE_DESELECT = 0x900,
	DOSTORYSOFAR = 0xA00,
	DOLEVELSELECT = 0xB00
} title_options;

typedef enum gf_event_types_t
{
	GFE_PICTURE,
	GFE_LIST_START,
	GFE_LIST_END,
	GFE_PLAYFMV,
	GFE_STARTLEVEL,
	GFE_CUTSCENE,
	GFE_LEVCOMPLETE,
	GFE_DEMOPLAY,
	GFE_JUMPTO_SEQ,
	GFE_END_SEQ,
	GFE_SETTRACK,
	GFE_SUNSET,
	GFE_LOADINGPIC,
	GFE_DEADLY_WATER,
	GFE_REMOVE_WEAPONS,
	GFE_GAMECOMPLETE,
	GFE_CUTANGLE,
	GFE_NOFLOOR,
	GFE_ADD2INV,
	GFE_STARTANIM,
	GFE_NUMSECRETS,
	GFE_KILL2COMPLETE,
	GFE_REMOVE_AMMO,
	//NEW EVENTS:
	GFE_RAIN,
	GFE_SNOW,
	GFE_WATER_PARTS,
	GFE_COLD,
	GFE_DEATHTILE,
	GFE_WATERCLR,
} gf_event_types;

typedef enum game_string_ids_t
{
	GT_MAIN_HEADING,
	GT_OPTION_HEADING,
	GT_KEYS_HEADING,
	GT_GAMEOVER_HEADING,
	GT_LOADGAME,
	GT_SAVEGAME,
	GT_STARTGAME,
	GT_RESTARTLEVEL,
	GT_EXIT2TITLE,
	GT_EXITDEMO,
	GT_EXITGAME,
	GT_SELECTLEVEL,
	GT_SAVEPOSITION,
	GT_DETAIL,
	GT_HIGH_DETAIL,
	GT_MEDIUM_DETAIL,
	GT_LOW_DETAIL,
	GT_WALK,
	GT_ROLL,
	GT_RUN,
	GT_LEFT,
	GT_RIGHT,
	GT_BACK,
	GT_STEPLEFT1,
	GT_STEPLEFT2,
	GT_STEPRIGHT1,
	GT_STEPRIGHT2,
	GT_LOOK,
	GT_JUMP,
	GT_ACTION,
	GT_DRAWWEAPON1,
	GT_DRAWWEAPON2,
	GT_INVENTORY,
	GT_USEFLARE,
	GT_STEPSHIFT,
	GT_STOPWATCH,
	GT_PISTOLS,
	GT_SHOTGUN,
	GT_AUTOPISTOLS,
	GT_UZIS,
	GT_HARPOON,
	GT_M16,
	GT_ROCKETLAUNCHER,
	GT_GRENADELAUNCHER,
	GT_FLARE,
	GT_PISTOLCLIPS,
	GT_SHOTGUNSHELLS,
	GT_AUTOPISTOLCLIPS,
	GT_UZICLIPS,
	GT_HARPOONBOLTS,
	GT_M16CLIPS,
	GT_ROCKETS,
	GT_GRENADES,
	GT_SMALLMEDI,
	GT_LARGEMEDI,
	GT_PICKUP,
	GT_PUZZLE,
	GT_KEY,
	GT_GAME,
	GT_GYM,
	GT_LOADING,
	GT_STAT_TIME,
	GT_STAT_SECRETS,
	GT_STAT_LOCATION,
	GT_STAT_KILLS,
	GT_STAT_AMMO,
	GT_STAT_RATIO,
	GT_STAT_SAVES,
	GT_STAT_DISTANCE,
	GT_STAT_HEALTH,
	GT_SECURITY_TAG,
	GT_NONE,
	GT_FINISH,
	GT_BESTTIMES,
	GT_NOTIMES,
	GT_NOTAVAILABLE,
	GT_CURRENTPOS,
	GT_GAMESTATS,
	GT_OF,
	GT_STORY_SO_FAR,
	GT_ICON1,
	GT_ICON2,
	GT_ICON3,
	GT_ICON4,
	GT_CRYSTAL,
	GT_LSLONDON,
	GT_LSNEVADA,
	GT_LSSPAC,
	GT_LSANTARC,
	GT_LSPERU,
	GT_LEVELSELECT,
	GT_SPACE,
	GT_NUM_GAMESTRINGS
} game_string_ids;

typedef enum pc_string_ids_t
{
	PCSTR_DETAILLEVEL,
	PCSTR_DEMOMODE,
	PCSTR_SOUND,
	PCSTR_CONTROLS,
	PCSTR_GAMMA,
	PCSTR_SETVOLUME,
	PCSTR_USERKEYS,
	PCSTR_SAVEMESS1,
	PCSTR_SAVEMESS2,
	PCSTR_SAVEMESS3,
	PCSTR_SAVEMESS4,
	PCSTR_SAVEMESS5,
	PCSTR_SAVEMESS6,
	PCSTR_SAVEMESS7,
	PCSTR_SAVEMESS8,
	PCSTR_SAVESLOT,
	PCSTR_OFF,
	PCSTR_ON,
	PCSTR_SETUPSOUND,
	PCSTR_DEFAULTKEYS,
	PCSTR_DOZY_STRING,
	PCSTR_VIDEOTITLE,
	PCSTR_RESOLUTION,
	PCSTR_ZBUFFER,
	PCSTR_FILTERING,
	PCSTR_DITHER,
	PCSTR_TRUEALPHA,
	PCSTR_SKY,
	PCSTR_SPARE8,
	PCSTR_SPARE9,
	PCSTR_SPARE10,
	PCSTR_SPARE11,
	PCSTR_SPARE12,
	PCSTR_SPARE13,
	PCSTR_SPARE14,
	PCSTR_SPARE15,
	PCSTR_SPARE16,
	PCSTR_SPARE17,
	PCSTR_SPARE18,
	PCSTR_SPARE19,
	PCSTR_SPARE20,
	PCSTR_NUM_STRINGS
} pc_string_ids;

typedef enum psx_string_ids_t
{
	PSSTR_DETAILLEVEL,
	PSSTR_DEMOMODE,
	PSSTR_SOUND,
	PSSTR_CONTROLS,
	PSSTR_GAMMA,
	PSSTR_SETVOLUME,
	PSSTR_USERKEYS,
	PSSTR_SAVEMESS1,
	PSSTR_SAVEMESS2,
	PSSTR_SAVEMESS3,
	PSSTR_SAVEMESS4,
	PSSTR_SAVEMESS5,
	PSSTR_SAVEMESS6,
	PSSTR_SAVEMESS7,
	PSSTR_SAVEMESS8,
	PSSTR_SAVESLOT,
	PSSTR_DEMOTEXT,
	PSSTR_MOVESTR0,
	PSSTR_MOVESTR1,
	PSSTR_SELECT_TEXT,
	PSSTR_DESELECT_TEXT,
	PSSTR_CONTINUE_TEXT,
	PSSTR_PAUSED_TEXT,
	PSSTR_NOJOY_TEXT,
	PSSTR_GAPSTRING0,
	PSSTR_GAPSTRING1,
	PSSTR_GAPSTRING2,
	PSSTR_GAPSTRING3,
	PSSTR_GAPSTRING4,
	PSSTR_GAPSTRING5,
	PSSTR_GAPSTRING6,
	PSSTR_GAPSTRING7,
	PSSTR_GAPSTRING8,
	PSSTR_GAPSTRING9,
	PSSTR_GAPSTRING10,
	PSSTR_GAPSTRING11,
	PSSTR_GAPSTRING12,
	PSSTR_GAPSTRING13,
	PSSTR_GAPSTRING14,
	PSSTR_GAPSTRING15,
	PSSTR_GAPSTRING16,
	PSSTR_GAPSTRING17,
	PSSTR_GAPSTRING18,
	PSSTR_GAPSTRING19,
	PSSTR_GAPSTRING20,
	PSSTR_GAPSTRING21,
	PSSTR_GAPSTRING22,
	PSSTR_GAPSTRING23,
	PSSTR_GAPSTRING24,
	PSSTR_GAPSTRING25,
	PSSTR_GAPSTRING26,
	PSSTR_GAPSTRING27,
	PSSTR_GAPSTRING28,
	PSSTR_GAPSTRING29,
	PSSTR_GAPSTRING30,
	PSSTR_GAPSTRING31,
	PSSTR_GAPSTRING32,
	PSSTR_GAPSTRING33,
	PSSTR_GAPSTRING34,
	PSSTR_GAPSTRING35,
	PSSTR_GAPSTRING36,
	PSSTR_GAPSTRING37,
	PSSTR_FILENAMEPAL,
	PSSTR_FILENAMENTSC,
	PSSTR_LOADOK,
	PSSTR_LOADFAIL,
	PSSTR_SAVEOK,
	PSSTR_SAVEFAIL,
	PSSTR_FORMATOK,
	PSSTR_FORMATFAIL,
	PSSTR_VIBOFF,
	PSSTR_VIBON,
	PSSTR_SPARE13,
	PSSTR_SPARE14,
	PSSTR_SPARE15,
	PSSTR_SPARE16,
	PSSTR_SPARE17,
	PSSTR_SPARE18,
	PSSTR_SPARE19,
	PSSTR_SPARE20,
	PSSTR_NUM_STRINGS
} psx_string_ids;

typedef enum death_tiles_t
{
	DEATH_LAVA,
	DEATH_RAPIDS,
	DEATH_ELECTRIC
} death_tiles;

typedef struct gameflow_info_t
{
	long firstOption;
	long title_replace;
	long ondeath_demo_mode;
	long ondeath_ingame;
	long noinput_time;
	long on_demo_interrupt;
	long on_demo_end;
	long unused1[9];
	short num_levels;
	short num_picfiles;
	short num_titlefiles;
	short num_fmvfiles;
	short num_cutfiles;
	short num_demos;
	short title_track;
	short singlelevel;
	short unused2[16];
	ushort demoversion : 1;
	ushort title_disabled : 1;
	ushort cheatmodecheck_disabled : 1;
	ushort noinput_timeout : 1;
	ushort loadsave_disabled : 1;
	ushort screensizing_disabled : 1;
	ushort lockout_optionring : 1;
	ushort dozy_cheat_enabled : 1;
	ushort cyphered_strings : 1;
	ushort gym_enabled : 1;
	ushort play_any_level : 1;
	ushort cheat_enable : 1;
	ushort securitytag : 1;
//	ushort unused3[3];
	ushort unused3[2];
	ushort map_enabled : 1;
	ushort globe_enabled : 1;
	ushort force_water_color : 1;
	uchar cypher_code;
	uchar language;
	uchar secret_track;
	uchar stats_track;
	char pads[4];
} GAMEFLOW_INFO;
#pragma pack(pop)

extern GAMEFLOW_INFO gameflow;
extern char gameflow_description[256];
extern char* puzzle1_strings[NUM_ELEMENTS];
extern char* puzzle2_strings[NUM_ELEMENTS];
extern char* puzzle3_strings[NUM_ELEMENTS];
extern char* puzzle4_strings[NUM_ELEMENTS];
extern char* key1_strings[NUM_ELEMENTS];
extern char* key2_strings[NUM_ELEMENTS];
extern char* key3_strings[NUM_ELEMENTS];
extern char* key4_strings[NUM_ELEMENTS];
extern char* pickup1_strings[NUM_ELEMENTS];
extern char* pickup2_strings[NUM_ELEMENTS];
extern char* LevelName_Strings[NUM_ELEMENTS];
extern char* game_strings[GT_NUM_GAMESTRINGS];
extern char* pc_strings[PCSTR_NUM_STRINGS];
extern char* ps_strings[PSSTR_NUM_STRINGS];

extern short GF_valid_demos[NUM_ELEMENTS];

extern short num_titlefiles;
extern char* GF_titlefilenames[NUM_ELEMENTS];

extern short num_fmvfiles;
extern char* GF_fmvfilenames[NUM_ELEMENTS];

extern short num_picfiles;
extern char* GF_picfilenames[NUM_ELEMENTS];

extern short num_demos;
extern short num_levels;
extern char* GF_levelfilenames[NUM_ELEMENTS];

extern short num_cutfiles;
extern char* GF_cutscenefilenames[NUM_ELEMENTS];

extern short GF_FrontEnd[MAX_SEQUENCE];
extern short* GF_level_sequence_list[NUM_ELEMENTS];
