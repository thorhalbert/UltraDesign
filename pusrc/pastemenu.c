/*****************************************************
 *                  Menu Definitions                 *
 *                                                   *
 *             Created with Menu Creator             *
 *                        by                         *
 *                   David Pehrson                   *
 *                                                   *
 *         Copyright (C) 1986  David Pehrson         *
 *                 Mantis Development                *
 *                                                   *
 *****************************************************/

#include <exec/types.h>
#include <intuition/intuition.h>

char stTopaz[] = "pearl2.font";

struct TextAttr taPlain =
{
	stTopaz, 8, FS_NORMAL, FPF_ROMFONT
};

struct TextAttr taBIU =
{
	stTopaz, 8, FSF_BOLD | FSF_ITALIC | FSF_UNDERLINED, FPF_ROMFONT
};

struct TextAttr taBU =
{
	stTopaz, 8, FSF_BOLD | FSF_UNDERLINED, FPF_ROMFONT
};

struct TextAttr taBI =
{
	stTopaz, 8, FSF_BOLD | FSF_ITALIC, FPF_ROMFONT
};

struct TextAttr taB =
{
	stTopaz, 8, FSF_BOLD, FPF_ROMFONT
};

struct TextAttr taIU =
{
	stTopaz, 8, FSF_ITALIC | FSF_UNDERLINED, FPF_ROMFONT
};

struct TextAttr taI =
{
	stTopaz, 8, FSF_ITALIC, FPF_ROMFONT
};

struct TextAttr taU =
{
	stTopaz, 8, FSF_UNDERLINED, FPF_ROMFONT
};

struct IntuiText ItemText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load Setup", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Save Setup As...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Change Setup", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load Driver", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Quit", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Paste Drawing", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load Pasteup", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Save Pasteup", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Preview", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Plot Out", NULL },
};

struct MenuItem Items[] =
{
	{
		&Items[1], 0, 0, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[0], NULL, NULL, NULL, NULL 
	},
	{
		&Items[2], 0, 10, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[1], NULL, NULL, NULL, NULL 
	},
	{
		&Items[3], 0, 20, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[2], NULL, NULL, NULL, NULL 
	},
	{
		&Items[4], 0, 30, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[3], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 40, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[4], NULL, NULL, NULL, NULL 
	},
	{
		&Items[6], 0, 0, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[5], NULL, NULL, NULL, NULL 
	},
	{
		&Items[7], 0, 0, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[6], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 10, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[7], NULL, NULL, NULL, NULL 
	},
	{
		&Items[9], 0, 0, 80, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[8], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 10, 80, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[9], NULL, NULL, NULL, NULL 
	},
};

struct Menu Titles[] =
{
	{ &Titles[1], 3, 0, 78, 10, MENUENABLED, " Setup", &Items[0] },
	{ &Titles[2], 80, 0, 118, 10, MENUENABLED, " Pasteup", &Items[6] },
	{ NULL, 197, 0, 54, 10, MENUENABLED, " Plot", &Items[8] },
};

