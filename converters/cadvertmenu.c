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

struct IntuiText ItemText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "UltraDesign File (.uvw)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "DXF Interchange File (.dxf)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "IntroCAD File (.cad)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Aegis Draw File", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "HPGL File (.hpgl)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Roland DXY File (.dxy)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Quit", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Palette", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Margins", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Repaint", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "UltraDesign File (.uvw)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "DXF Interchange File (.dxf)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "ILBM IFF Picture (.pic)", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "UltraDesign Hatch File (.uhat)", NULL },
};

struct MenuItem Items[] =
{
	{
		&Items[1], 0, 0, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[0], NULL, NULL, NULL, NULL 
	},
	{
		&Items[2], 0, 10, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[1], NULL, NULL, NULL, NULL 
	},
	{
		&Items[3], 0, 20, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[2], NULL, NULL, NULL, NULL 
	},
	{
		&Items[4], 0, 30, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[3], NULL, NULL, NULL, NULL 
	},
	{
		&Items[5], 0, 40, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[4], NULL, NULL, NULL, NULL 
	},
	{
		&Items[6], 0, 50, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[5], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 60, 232, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[6], NULL, NULL, NULL, NULL 
	},
	{
		&Items[8], 0, 0, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[7], NULL, NULL, NULL, NULL 
	},
	{
		&Items[9], 0, 10, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[8], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 20, 72, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[9], NULL, NULL, NULL, NULL 
	},
	{
		&Items[11], 0, 0, 256, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[10], NULL, NULL, NULL, NULL 
	},
	{
		&Items[12], 0, 10, 256, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[11], NULL, NULL, NULL, NULL 
	},
	{
		&Items[13], 0, 20, 256, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[12], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 30, 256, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[13], NULL, NULL, NULL, NULL 
	},
};

struct Menu Titles[] =
{
	{ &Titles[1], 3, 0, 46, 10, MENUENABLED, "Load", &Items[0] },
	{ &Titles[2], 48, 0, 46, 10, MENUENABLED, "Edit", &Items[7] },
	{ NULL, 93, 0, 46, 10, MENUENABLED, "Save", &Items[10] },
};

