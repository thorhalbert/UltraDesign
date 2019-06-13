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

#define FPF_ROMFONT 0
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

struct IntuiText SubText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "At Root    $MVR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "At Current $MVC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "At Root    $MLR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "At Current $MLC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "In   $ZIN$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Full $ZFU$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "200% $Z20$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "100% $Z10$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "50%  $Z50$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Edit    $EPL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Default $RPL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Layer Color  $CBL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Group Color  $CBG$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Pen Color    $CBP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Set Temporary  $STO$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Restore        $RTO$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Set Permanent  $SPO$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load for use$DLB$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Place setup $ISL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Select item $SEL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Place item  $PLI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "EndPoint      $SKE$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "MidPoint      $SKM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Perpendicular $SKP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Center        $SKC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Layers            $ALY$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Pen Styles        $APS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Groups            $AGR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Lettering Styles  $ALS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Delete Item      $DLI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Add To Clip      $ATC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Remove From Clip $RFC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Select Item      $SLI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan On Screen   $SOS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan In Clip     $SIC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan Same Layer  $SSL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan Same Pen    $SSP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan Same Group  $SSG$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Scan Same Style  $SSS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Hook    $LNH$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Unhook  $LNU$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "In  $BXI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "All $BXA$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Out $BXO$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip        $CPS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip=String $CES$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip=Cur Layer $CCL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip=Cur Pen   $CCP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip=Cur Group $CCG$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip=Cur Style $CCS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip To First  $CTF$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clip To Last   $CTL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "X Flip   $XFL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Y Flip   $YFL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Centers   $SHC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Auto Sel  $ATS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Coord Disp$COD$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Draw Info $DRI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Curs Snap $CUS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Grids     $GRI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Bound Lock$BNL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Ortho Swit$OTS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Q Hatching$QHA$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Eng X' X.X\"  $EDC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Eng X' X-X/X\"$ENA$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Eng Feet X.X'$ENF$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Eng Inch X.X\"$ENI$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Miles        $ENM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Millimeters  $MMM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Centimeters  $MCM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Meters       $MTR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Kilometers   $MKM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Degrees X.X°$DDC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Deg X° X.X' $DHM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Deg X° X' X\"$DMS$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Radians     $RAD$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Gradians    $GRD$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Percent     $PRC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Menus    $RPM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Margins  $RPR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Layers   $RPL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Pens     $RPP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Groups   $RPG$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Styles   $RPS$", NULL },
};

struct IntuiText ItemText[] =
{
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load   $LVW$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Save   $SVW$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Update $UVW$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Merge...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Create $CVW$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Quit   $QUT$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Load   $LLB$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Save   $SLB$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Update $ULB$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Merge...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Create $CLB$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Repaint   $REP$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Zoom...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Palette...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Color By...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Origins...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Library Element...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Lines           $CLN$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Ellipse/Circles $CEL$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Text            $CTX$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Dimensioning    $CDM$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "   Hatching/Filling$CHF$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Seek...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Attributes...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Scanner...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Razor       $RAZ$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Scan Scope...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Edit Text   $EDT$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Line...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Dialog      $DIA$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Box...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "String...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Group...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Paint Order...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Save Clip $SVC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clear     $CLR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Kill      $KLC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Drag      $DRC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Clone     $CLC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Scale     $SCC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Rotate    $ROC$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Flip...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Margins $MAR$", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Options...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Linear...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Angular...", NULL },
	{ 0, 1, JAM1, 0, 1, &taPlain, "Reports...", NULL },
};

struct MenuItem SubItems[] =
{
	{
		&SubItems[1], 100, 0, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[0], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 100, 10, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[1], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[3], 100, 0, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[2], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 100, 10, 144, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[3], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[5], 124, 0, 96, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[4], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[6], 124, 10, 96, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[5], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[7], 124, 20, 96, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[6], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[8], 124, 30, 96, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[7], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 40, 96, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[8], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[10], 124, 0, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[9], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 10, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[10], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[12], 124, 0, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		6, (APTR)&SubText[11], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[13], 124, 10, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		5, (APTR)&SubText[12], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 20, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		3, (APTR)&SubText[13], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[15], 124, 0, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[14], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[16], 124, 10, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[15], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 20, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[16], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[18], 196, 0, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[17], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[19], 196, 10, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[18], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[20], 196, 20, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[19], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 196, 30, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[20], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[22], 140, 0, 168, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[21], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[23], 140, 10, 168, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[22], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[24], 140, 20, 168, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[23], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 140, 30, 168, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[24], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[26], 140, 0, 200, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[25], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[27], 140, 10, 200, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[26], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[28], 140, 20, 200, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[27], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 140, 30, 200, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[28], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[30], 140, 0, 192, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[29], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[31], 140, 10, 192, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[30], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[32], 140, 20, 192, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[31], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 140, 30, 192, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[32], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[34], 140, 0, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[33], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[35], 140, 10, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[34], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[36], 140, 20, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[35], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[37], 140, 30, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[36], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[38], 140, 40, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[37], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 140, 50, 216, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[38], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[40], 140, 0, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[39], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 140, 10, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[40], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[42], 124, 0, 88, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[41], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[43], 124, 10, 88, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[42], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 20, 88, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[43], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[45], 124, 0, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[44], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 10, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[45], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[47], 124, 0, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[46], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[48], 124, 10, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[47], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[49], 124, 20, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[48], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 30, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[49], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[51], 124, 0, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[50], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 10, 176, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[51], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[53], 124, 0, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[52], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 124, 10, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[53], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[55], 108, 0, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[54], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[56], 108, 10, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[55], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[57], 108, 20, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[56], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[58], 108, 30, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[57], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[59], 108, 40, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[58], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[60], 108, 50, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[59], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[61], 108, 60, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[60], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[62], 108, 70, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[61], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 108, 80, 160, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		0, (APTR)&SubText[62], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[64], 108, 0, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		510, (APTR)&SubText[63], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[65], 108, 10, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		509, (APTR)&SubText[64], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[66], 108, 20, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		507, (APTR)&SubText[65], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[67], 108, 30, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		503, (APTR)&SubText[66], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[68], 108, 40, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		495, (APTR)&SubText[67], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[69], 108, 50, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		479, (APTR)&SubText[68], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[70], 108, 60, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		447, (APTR)&SubText[69], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[71], 108, 70, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		383, (APTR)&SubText[70], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 108, 80, 184, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		255, (APTR)&SubText[71], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[73], 108, 0, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		62, (APTR)&SubText[72], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[74], 108, 10, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		61, (APTR)&SubText[73], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[75], 108, 20, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		59, (APTR)&SubText[74], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[76], 108, 30, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		55, (APTR)&SubText[75], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[77], 108, 40, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		47, (APTR)&SubText[76], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 108, 50, 176, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		31, (APTR)&SubText[77], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[79], 108, 0, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[78], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[80], 108, 10, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[79], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[81], 108, 20, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[80], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[82], 108, 30, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[81], NULL, NULL, NULL, NULL 
	},
	{
		&SubItems[83], 108, 40, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[82], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 108, 50, 128, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&SubText[83], NULL, NULL, NULL, NULL 
	},
};

struct MenuItem Items[] =
{
	{
		&Items[1], 0, 0, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[0], NULL, NULL, NULL, NULL 
	},
	{
		&Items[2], 0, 10, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[1], NULL, NULL, NULL, NULL 
	},
	{
		&Items[3], 0, 20, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[2], NULL, NULL, NULL, NULL 
	},
	{
		&Items[4], 0, 30, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[3], NULL, NULL, &SubItems[0], NULL 
	},
	{
		&Items[5], 0, 40, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[4], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 50, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[5], NULL, NULL, NULL, NULL 
	},
	{
		&Items[7], 0, 0, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[6], NULL, NULL, NULL, NULL 
	},
	{
		&Items[8], 0, 10, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[7], NULL, NULL, NULL, NULL 
	},
	{
		&Items[9], 0, 20, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[8], NULL, NULL, NULL, NULL 
	},
	{
		&Items[10], 0, 30, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[9], NULL, NULL, &SubItems[2], NULL 
	},
	{
		NULL, 0, 40, 112, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[10], NULL, NULL, NULL, NULL 
	},
	{
		&Items[12], 0, 0, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[11], NULL, NULL, NULL, NULL 
	},
	{
		&Items[13], 0, 10, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[12], NULL, NULL, &SubItems[4], NULL 
	},
	{
		&Items[14], 0, 20, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[13], NULL, NULL, &SubItems[9], NULL 
	},
	{
		&Items[15], 0, 30, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[14], NULL, NULL, &SubItems[11], NULL 
	},
	{
		NULL, 0, 40, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[15], NULL, NULL, &SubItems[14], NULL 
	},
	{
		&Items[17], 0, 0, 208, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[16], NULL, NULL, &SubItems[17], NULL 
	},
	{
		&Items[18], 0, 10, 208, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		60, (APTR)&ItemText[17], NULL, NULL, NULL, NULL 
	},
	{
		&Items[19], 0, 20, 208, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		58, (APTR)&ItemText[18], NULL, NULL, NULL, NULL 
	},
	{
		&Items[20], 0, 30, 208, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		54, (APTR)&ItemText[19], NULL, NULL, NULL, NULL 
	},
	{
		&Items[21], 0, 40, 208, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		46, (APTR)&ItemText[20], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 50, 208, 10, ITEMENABLED | ITEMTEXT | CHECKIT | HIGHCOMP,
		30, (APTR)&ItemText[21], NULL, NULL, NULL, NULL 
	},
	{
		&Items[23], 0, 0, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[22], NULL, NULL, &SubItems[21], NULL 
	},
	{
		&Items[24], 0, 10, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[23], NULL, NULL, &SubItems[25], NULL 
	},
	{
		&Items[25], 0, 20, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[24], NULL, NULL, &SubItems[29], NULL 
	},
	{
		&Items[26], 0, 30, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[25], NULL, NULL, NULL, NULL 
	},
	{
		&Items[27], 0, 40, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[26], NULL, NULL, &SubItems[33], NULL 
	},
	{
		&Items[28], 0, 50, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[27], NULL, NULL, NULL, NULL 
	},
	{
		&Items[29], 0, 60, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[28], NULL, NULL, &SubItems[39], NULL 
	},
	{
		NULL, 0, 70, 152, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[29], NULL, NULL, NULL, NULL 
	},
	{
		&Items[31], 0, 0, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[30], NULL, NULL, &SubItems[41], NULL 
	},
	{
		&Items[32], 0, 10, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[31], NULL, NULL, &SubItems[44], NULL 
	},
	{
		&Items[33], 0, 20, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[32], NULL, NULL, &SubItems[46], NULL 
	},
	{
		&Items[34], 0, 30, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[33], NULL, NULL, &SubItems[50], NULL 
	},
	{
		&Items[35], 0, 40, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[34], NULL, NULL, NULL, NULL 
	},
	{
		&Items[36], 0, 50, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[35], NULL, NULL, NULL, NULL 
	},
	{
		&Items[37], 0, 60, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[36], NULL, NULL, NULL, NULL 
	},
	{
		&Items[38], 0, 70, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[37], NULL, NULL, NULL, NULL 
	},
	{
		&Items[39], 0, 80, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[38], NULL, NULL, NULL, NULL 
	},
	{
		&Items[40], 0, 90, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[39], NULL, NULL, NULL, NULL 
	},
	{
		&Items[41], 0, 100, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[40], NULL, NULL, NULL, NULL 
	},
	{
		NULL, 0, 110, 136, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[41], NULL, NULL, &SubItems[52], NULL 
	},
	{
		&Items[43], 0, 0, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[42], NULL, NULL, NULL, NULL 
	},
	{
		&Items[44], 0, 10, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[43], NULL, NULL, &SubItems[54], NULL 
	},
	{
		&Items[45], 0, 20, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[44], NULL, NULL, &SubItems[63], NULL 
	},
	{
		&Items[46], 0, 30, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[45], NULL, NULL, &SubItems[72], NULL 
	},
	{
		NULL, 0, 40, 120, 10, ITEMENABLED | ITEMTEXT | HIGHCOMP,
		0, (APTR)&ItemText[46], NULL, NULL, &SubItems[78], NULL 
	},
};

struct Menu Titles[] =
{
	{ &Titles[1], 3, 0, 46, 10, MENUENABLED, "View", &Items[0] },
	{ &Titles[2], 48, 0, 70, 10, MENUENABLED, "Library", &Items[6] },
	{ &Titles[3], 117, 0, 62, 10, MENUENABLED, "Screen", &Items[11] },
	{ &Titles[4], 178, 0, 62, 10, MENUENABLED, "Insert", &Items[16] },
	{ &Titles[5], 239, 0, 46, 10, MENUENABLED, "Edit", &Items[22] },
	{ &Titles[6], 284, 0, 46, 10, MENUENABLED, "Clip", &Items[30] },
	{ NULL, 329, 0, 78, 10, MENUENABLED, "Settings", &Items[42] },
};
