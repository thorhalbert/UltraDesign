/*-------------------------Table forward refs-----------------------------*/

BOOL	Load_View();
BOOL	N_Save_View();
BOOL	Load_Libr();
BOOL	N_Save_Libr();
BOOL	Clip_View();
BOOL	Use_Libr();
BOOL	Root_Merge();
BOOL	Curr_Merge();
BOOL	Clear_View();
BOOL	Clear_Libr();
BOOL	N_Update();
BOOL	Quit_Out();
BOOL	Style_Current();
BOOL	Style_Add();
BOOL	Style_Rename();
BOOL	Style_Merge();
BOOL	Style_Kill();
BOOL	Style_Expunge();
BOOL	Layer_Current();
BOOL	Layer_Add();
BOOL	Layer_Rename();
BOOL	Layer_Merge();
BOOL	Layer_Kill();
BOOL	Layer_Expunge();
BOOL	Pen_Current();
BOOL	Pen_Add();
BOOL	Pen_Rename();
BOOL	Pen_Merge();
BOOL	Pen_Kill();
BOOL	Pen_Expunge();
BOOL	Group_Current();
BOOL	Group_Add();
BOOL	Group_Rename();
BOOL	Group_Merge();
BOOL	Group_Prune();
BOOL	Group_Graft();
BOOL	Group_Expunge();
BOOL	Color_Layer();
BOOL	Color_Group();
BOOL	Color_Pen();
BOOL	RepaintWindow();
BOOL	Zoom_In();
BOOL	Zoom_All();
BOOL	Zoom_Full();
BOOL	Zoom_O50();
BOOL	Zoom_O100();
BOOL	Zoom_O200();
BOOL	Margin_Current();
BOOL	PalDo();
BOOL	RestorePalette();
BOOL	TempOrigin();
BOOL	ResetOrigin();
BOOL	PermOrigin();
BOOL	Draw_Line();
BOOL	Draw_Ellipse();
BOOL	Draw_Text();
BOOL	Draw_Dims();
BOOL	Draw_Hatch();
BOOL	SeekMid();
BOOL	SeekEnd();
BOOL	SeekPerp();
BOOL	SeekCent();
BOOL	SeekTang();
BOOL	SeekText();
BOOL	LineIntersection();
BOOL	LineFilet();
BOOL	DelCurrent();
BOOL	ClipCurrent();
BOOL	UnClipCurrent();
BOOL	BoxIn();
BOOL	BoxAll();
BOOL 	Box_Out();
BOOL	group_group();
BOOL	group_layer();
BOOL	group_pen();
BOOL	group_lettering();
BOOL	Lib_CurrentItem();
BOOL	Library_Settings();
BOOL	Place_Item();
BOOL	Clip_Clear();
BOOL	Clip_Kill();
BOOL	Clip_Drag();
BOOL	Clip_Clone();
BOOL	Clip_Rotate();
BOOL	Razor_Line();
BOOL	EditItem();
BOOL	Scale_Even();
BOOL	x_flip();
BOOL	y_flip();
BOOL	LineHook();
BOOL	LineUnhook();
BOOL	Load_Font();
BOOL	Ang_DegD();
BOOL	Ang_DegM();
BOOL	Ang_DegMS();
BOOL	Ang_Rad();
BOOL	Ang_Grad();
BOOL	Lin_EDI();
BOOL	Lin_EFI();
BOOL	Lin_DF();
BOOL	Lin_DI();
BOOL	Lin_MM();
BOOL	Lin_CM();
BOOL	Lin_M();
BOOL	Lin_MILE();
BOOL	Lin_KM();
BOOL	Searcher();
BOOL	Report_Menus();
BOOL	ReportStyle();
BOOL	ReportLayer();
BOOL	ReportGroup();
BOOL	ReportPen();
BOOL 	OpenDialogWB();
BOOL 	OpenDialogUS();
BOOL	CloseDialog();
BOOL	Roll_Left();
BOOL	Roll_Right();
BOOL 	Clip_String_Open();
BOOL	Clip_String_Closed();
BOOL	Sort_Clip();
BOOL	Copy_Top();
BOOL	Copy_Bottom();
BOOL	ReCenter();
BOOL	LineUnhend();
BOOL	LineUnhendC();
BOOL	MarginsReport();
BOOL	Dims_Requester();
BOOL	Object_Editor();

/*-----------------------Internal Access Table----------------------------*/

struct internal_tab access_tables[] = {
	{ "LVW", NULL, Load_View, 	TRUE },
	{ "SVW", NULL, N_Save_View,	TRUE },
	{ "LLB", NULL, Load_Libr,	TRUE },
	{ "SLB", NULL, N_Save_Libr,	TRUE },
	{ "UVW", NULL, N_Update,	TRUE },
	{ "ULB", NULL, N_Update,	TRUE },
	{ "MVR", NULL, Root_Merge,	TRUE },
	{ "MVC", NULL, Curr_Merge,	TRUE },
	{ "SVC", NULL, Clip_View,	TRUE },
	{ "DLB", NULL, Use_Libr,	TRUE },
	{ "CVW", NULL, Clear_View,	TRUE },
	{ "CLB", NULL, Clear_Libr,	TRUE },
	{ "QUT", NULL, Quit_Out,	TRUE },
	{ "ALS", NULL, Style_Current, 	TRUE },
	{ "ALY", NULL, Layer_Current, 	TRUE },
	{ "AGR", NULL, Group_Current, 	TRUE },
	{ "APS", NULL, Pen_Current,	TRUE },
	{ "CBL", NULL, Color_Layer,	TRUE,	0x11 },
	{ "CBG", NULL, Color_Group, 	TRUE,	0x10 },
	{ "CBP", NULL, Color_Pen,	TRUE,	0x12 },
	{ "REP", NULL, RepaintWindow,	TRUE },
	{ "ZCN", NULL, ReCenter,	TRUE },
	{ "ZIN", NULL, Zoom_In,		TRUE },
	{ "ZFU", NULL, Zoom_Full, 	TRUE },
	{ "Z50", NULL, Zoom_O50,	TRUE },
	{ "Z10", NULL, Zoom_O100,	TRUE },
	{ "Z20", NULL, Zoom_O200,	TRUE },
	{ "MAR", NULL, Margin_Current,	TRUE },
	{ "DMM", NULL, Dims_Requester,	TRUE },
	{ "EPL", NULL, PalDo,		TRUE },
	{ "RPA", NULL, RestorePalette,	TRUE },
	{ "STO", NULL, TempOrigin,	TRUE },
	{ "RTO", NULL, ResetOrigin,	TRUE },
	{ "SPO", NULL, PermOrigin,	TRUE },
	{ "CLN", NULL, Draw_Line,	TRUE,	0x50 },
	{ "CEL", NULL, Draw_Ellipse,	TRUE,	0x51 },
	{ "CTX", NULL, Draw_Text, 	TRUE,	0x52 },
	{ "CDM", NULL, Draw_Dims, 	TRUE,	0x53 },
	{ "CHF", NULL, Draw_Hatch,	TRUE,	0x54 },
	{ "SKM", NULL, SeekMid,		TRUE },
	{ "SKE", NULL, SeekEnd,		TRUE },
	{ "SKP", NULL, SeekPerp,	TRUE },
	{ "SKC", NULL, SeekCent,	TRUE },
	{ "SKI", NULL, LineIntersection,TRUE },
	{ "DIA", NULL, OpenDialogWB,	TRUE },
	{ "DIM", NULL, OpenDialogUS,	TRUE },
	{ "DIC", NULL, CloseDialog,	TRUE },
	{ "DLI", NULL, DelCurrent,	TRUE },
	{ "ATC", NULL, ClipCurrent,	TRUE },
	{ "RFC", NULL, UnClipCurrent, 	TRUE },
	{ "SLI", NULL, Searcher,	TRUE },
	{ "BXI", NULL, BoxIn,		TRUE },
	{ "BXA", NULL, BoxAll, 		TRUE },
	{ "BXO", NULL, Box_Out,		TRUE },
	{ "CCG", NULL, group_group,	TRUE },
	{ "CCL", NULL, group_layer, 	TRUE },
	{ "CCP", NULL, group_pen,	TRUE },
	{ "CCS", NULL, group_lettering, TRUE },
	{ "RAZ", NULL, Razor_Line, 	TRUE },
	{ "SIC", &Scan_Clip, NULL,	TRUE },
	{ "SSG", &Scan_Group, NULL,	TRUE },
	{ "SSL", &Scan_Layer, NULL,	TRUE },
	{ "SSP", &Scan_Pen, NULL, 	TRUE },
	{ "SSS", &Scan_Text, NULL, 	TRUE },
	{ "CES", NULL, Clip_String_Open,TRUE },
	{ "CEO", NULL, Clip_String_Closed,TRUE },
	{ "CSR", NULL, Sort_Clip,	TRUE },
	{ "EDT", NULL, EditItem,	TRUE },
	{ "LNH", NULL, LineHook,	TRUE },
	{ "LNU", NULL, LineUnhook,	TRUE },
	{ "LMP", NULL, LineUnhend,	TRUE },
	{ "LMC", NULL, LineUnhendC,	TRUE },
	{ "FIL", NULL, LineFilet,	TRUE },
	{ "SEL", NULL, Lib_CurrentItem, TRUE },
	{ "ISL", NULL, Library_Settings, TRUE },
	{ "CTF", NULL, Copy_Top,	TRUE },
	{ "CTL", NULL, Copy_Bottom,	TRUE },
	{ "PLI", NULL, Place_Item,	TRUE },
	{ "CLR", NULL, Clip_Clear,	TRUE },
	{ "KLC", NULL, Clip_Kill,	TRUE },
	{ "DRC", NULL, Clip_Drag, 	TRUE },
	{ "CLC", NULL, Clip_Clone,	TRUE },
	{ "ROC", NULL, Clip_Rotate, 	TRUE },
	{ "SCC", NULL, Scale_Even,	TRUE },
	{ "XFL", NULL, x_flip,		TRUE },
	{ "YFL", NULL, y_flip,		TRUE },
	{ "SHC", &Center_On, NULL,	TRUE },
	{ "ATS", &Handles_On, NULL,	TRUE },
	{ "COD", &Coord_On,	NULL,	TRUE },
	{ "DRI", &Draw_Info, NULL,	TRUE },
	{ "CUS", &Snaps_On,	NULL,	TRUE },
	{ "GRI", &Grid_On, NULL,	TRUE },
	{ "BNL", &Use_Bounds, NULL,	TRUE },
	{ "OTS", &Ortho_Switch, NULL, 	TRUE },
	{ "CUR", &Cursors_On, NULL,	TRUE },
	{ "QHA", &Q_Hatch, NULL,	TRUE },
	{ "DDC", NULL, Ang_DegD,	TRUE,	0x101 },
	{ "DHM", NULL, Ang_DegM,	TRUE,	0x102 },
	{ "DMS", NULL, Ang_DegMS,	TRUE,	0x103 },
	{ "RAD", NULL, Ang_Rad,		TRUE,	0x104 },
	{ "GRD", NULL, Ang_Grad,	TRUE,	0x105 },
	{ "EDC", NULL, Lin_EDI,		TRUE,	0x111 },
	{ "ENA", NULL, Lin_EFI,		TRUE,	0x112 },
	{ "ENF", NULL, Lin_DF,		TRUE,	0x113 },
	{ "ENI", NULL, Lin_DI,		TRUE,	0x114 },
	{ "MMM", NULL, Lin_MM,		TRUE,	0x115 },
	{ "MCM", NULL, Lin_CM,		TRUE,	0x116 },
	{ "MTR", NULL, Lin_M,		TRUE,	0x117 },
	{ "ENM", NULL, Lin_MILE,	TRUE,	0x118 },
	{ "MKM", NULL, Lin_KM,		TRUE,	0x119 },
	{ "RPM", NULL, Report_Menus,	TRUE },
	{ "RPS", NULL, ReportStyle,	TRUE },
	{ "RLY", NULL, ReportLayer,	TRUE },
	{ "RPG", NULL, ReportGroup,	TRUE },
	{ "RPN", NULL, ReportPen,	TRUE },
	{ "RMR", NULL, MarginsReport,	TRUE },
	{ "OBJ", NULL, Object_Editor, 	TRUE },
	{ NULL, NULL, NULL, FALSE },
};
