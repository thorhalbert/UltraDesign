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

#define FPF_ROMFONT 0
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

struct IntuiText SubText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "   640x400", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   640x200", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   320x400", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   320x200", NULL },
};

struct IntuiText ItemText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "UltraDesign File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "DXF Interchange File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "IntroCAD File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Quit", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Palette", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Margins", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Screen", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "UltraDesign File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "DXF Interchange File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "ILBM IFF Picture", NULL },
};

struct MenuItem SubItems[] =
{
	{
		&SubItems[1], 60, 0, 96, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		14, (APTR)&SubText[0], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[2], 60, 10, 96, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		13, (APTR)&SubText[1], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[3], 60, 20, 96, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		11, (APTR)&SubText[2], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 60, 30, 96, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		7, (APTR)&SubText[3], NULL, NULL, NULL, NULL 
	},
};

struct MenuItem Items[] =
{
	{
		&Items[1], 0, 0, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[0], NULL, NULL, NULL, NULL 
	},
	{
		&Items[2], 0, 10, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[1], NULL, NULL, NULL, NULL 
	},
	{
		&Items[3], 0, 20, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[2], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 30, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[3], NULL, NULL, NULL, NULL 
	},
	{
		&Items[5], 0, 0, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[4], NULL, NULL, NULL, NULL 
	},
	{
		&Items[6], 0, 10, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[5], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 20, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[6], NULL, NULL, &SubItems[0], NULL 
	},
	{
		&Items[8], 0, 0, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[7], NULL, NULL, NULL, NULL 
	},
	{
		&Items[9], 0, 10, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[8], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 20, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[9], NULL, NULL, NULL, NULL 
	},
};

struct Menu Titles[] =
{
	{ &Titles[1], 3, 0, 46, 0, MENUENABLED, "Load", &Items[0] },
	{ &Titles[2], 48, 0, 46, 0, MENUENABLED, "Edit", &Items[4] },
	{ NULL, 93, 0, 46, 0, MENUENABLED, "Save", &Items[7] },
};
