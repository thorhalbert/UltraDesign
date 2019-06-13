DEFGLB(WORD,XSsize);				/* Physical Configuration */
DEFGLB(WORD,YSsize);
DEFGLB(WORD,Sdepth);
DEFGLI(BOOL,scinot,FALSE);

/*-------------------Persistent Margins, Switches and Settings----------------*/

DEFGLI(STDFLT,xsnap,1.0);		/* X axis mouse increment */
DEFGLI(STDFLT,ysnap,1.0);		/* Y axis mouse increment */
DEFGLI(STDFLT,xoffset,0.0);		/* X Origin offset for snap */
DEFGLI(STDFLT,yoffset,0.0);		/* Y Origin offset for snap */
DEFGLI(STDFLT,snaprot,0.0);		/* Rotation of snap plane */
DEFGLI(STDFLT,xgrid,1.0);		/* X Size of each grid increment */
DEFGLI(STDFLT,ygrid,1.0);		/* Y Size of each grid increment */
DEFGLI(STDFLT,xgridoff,0.0);		/* X Origin offset for grid */
DEFGLI(STDFLT,ygridoff,0.0);		/* Y Origin offset for grid */
DEFGLI(WORD,xgriddiv,5);		/* Number of subdivisions on x grid */
DEFGLI(WORD,ygriddiv,5);		/* Number of subdivisions on y grid */
DEFGLI(STDFLT,gridrot,0.0);		/* Rotation of grid plane */
DEFGLI(STDFLT,xviewMIN,0.0);		/* MIN x of view boundary */
DEFGLI(STDFLT,yviewMIN,0.0);		/* MIN y of view boundary */
DEFGLI(STDFLT,xviewMAX,420.0);		/* MAX x of view boundary */
DEFGLI(STDFLT,yviewMAX,297.0);		/* MAX y of view boundary */
DEFGLI(WORD,qtextlim,5);		/* Qtext constant in pixels high */
DEFGLI(STDFLT,paperunit,1.0);		/* Basic Unit of Paper */
DEFGLI(STDFLT,viewunit,1.0);		/* vs. basic unit of view */
DEFGLI(STDFLT,viewscale,1.0);		/* paperunit/viewunit cnv factor */
DEFGLI(STDFLT,Arrow_Angle,PI/9.0);	/* Angle of half arrowhead point */
DEFGLI(STDFLT,Arrow_Len,12.0);		/* Length of arrow head */
DEFGLI(STDFLT,orthoangle,0.0);		/* Orthogonal angle offset */
DEFGLI(STDFLT,filletrad,5.0);		/* Fillet radius */
DEFGLI(STDFLT,lintolerance,.001);	/* Linear unit tolerance */
DEFGLI(WORD,linpoints,3);		/* Number of dec pts in linear unit */
DEFGLI(STDFLT,angtolerance,.1);		/* Angular unit tolerance */
DEFGLI(WORD,angpoints,1);		/* Number of dec pts in angular unit */
DEFGLI(STDFLT,aspect,1.0);		/* Ratio of x/y on axis */
DEFGLI(STDFLT,skew,1.0);		/* Must be 1.0 - Unused */
DEFGLI(STDFLT,dot_size,.75);		/* Size of a plotter dot */

DEFGLI(BOOL,Center_On,FALSE);
DEFGLI(BOOL,Handles_On,FALSE);
DEFGLI(BOOL,Coord_On,TRUE);
DEFGLI(BOOL,Draw_Info,TRUE);
DEFGLI(BOOL,Snaps_On,TRUE);
DEFGLI(BOOL,Grid_On,TRUE);
DEFGLI(BOOL,Use_Bounds,TRUE);
DEFGLI(BOOL,Ortho_Switch,TRUE);
DEFGLI(BOOL,Library_Mode,FALSE);

DEFGLI(WORD,angtype,ANG_DEGD);
DEFGLI(WORD,lintype,LIN_DI);

DEFGLI(struct Text_Fonts *,Root_Font,NULL);	/* Font linked list */
DEFGLI(struct Text_Fonts *,Font_Last,NULL);

DEFGLI(struct ViewContext *,Root_View,NULL);
DEFGLI(struct ViewContext *,Last_View,NULL);
DEFGLI(struct ViewContext *,Current_View,NULL);

DEFGLI(struct Group_Attrib *,Current_Group,NULL);
DEFGLI(struct Layer_Attrib *,Current_Layer,NULL);
DEFGLI(struct Pen_Attrib *,Current_Pen,NULL);
DEFGLI(struct Lettering_Styles *,Current_Style,NULL);

DEFGLB(ULONG,StoSecs);
DEFGLB(ULONG,StoMics);

DEFGLB(WORD,keepx);		/* Last Crosshair Position */
DEFGLB(WORD,keepy);

DEFGLB(struct Window *,w);
DEFGLB(struct Screen *,screen);
DEFGLB(struct RastPort *,rp);
DEFGLB(struct ViewPort *,vp);
DEFGLB(struct IntuiMessage *,message);

DEFGLI(UBYTE *,Main_Mode,NULL);
DEFGLI(UBYTE *,Last_Mode,NULL);
DEFGLI(UBYTE *,Save_Mode,NULL);

DEFGLI(struct mesdef *,Main_Mess,NULL);
DEFGLI(struct mesdef *,Last_Mess,NULL);
DEFGLI(struct mesdef *,Save_Mess,NULL);
DEFGLI(struct mesdef *,Main_Out,NULL);

DEFGLB(UBYTE,ArgText[257]);
DEFGLB(UBYTE,undobuf[257]);

DEFGLI(APTR,DriverSegHook,0);

DEFGLB(struct main_interface,maininter);
DEFGLB(struct plotter_base,mainbase);
DEFGLB(struct plot_info,maininfo);

/*-----------------------Constants for pasteup screen------------------------*/

DEFGLB(UBYTE,PlotName[255]);
DEFGLI(WORD,PosterX,1);		/* Number of poster sheets in x direction */
DEFGLI(WORD,PosterY,1);		/* Number of sheets in y direction */
DEFGLI(LONG,circstep,12);	/* Rough minimum pixels per arc step */

DEFGLI(WORD,XBegin,9);
DEFGLI(WORD,XWidth,640-20);
DEFGLI(WORD,YBegin,14);
DEFGLI(WORD,YWidth,158-14);

DEFGLB(STDFLT,xviewsize);			/* Current Temporary Margins */
DEFGLB(STDFLT,yviewsize);
DEFGLB(STDFLT,xviewbegin);
DEFGLB(STDFLT,yviewbegin);
DEFGLI(STDFLT,viewrot,0.0);
DEFGLB(STDFLT,xpixsize);
DEFGLB(STDFLT,ypixsize);

DEFGLB(STDFLT,MinIX);
DEFGLB(STDFLT,MaxIX);
DEFGLB(STDFLT,MinIY);
DEFGLB(STDFLT,MaxIY);
DEFGLB(STDFLT,MinDX);
DEFGLB(STDFLT,MaxDX);
DEFGLB(STDFLT,MinDY);
DEFGLB(STDFLT,MaxDY);

DEFGLB(STDFLT,SheetMinIX);
DEFGLB(STDFLT,SheetMaxIX);
DEFGLB(STDFLT,SheetMinIY);
DEFGLB(STDFLT,SheetMaxIY);

DEFGLB(WORD,clip_sret);
DEFGLB(STDFLT,lastx);
DEFGLB(STDFLT,lasty);

DEFGLB(UBYTE,DrivMask[40]);
DEFGLB(UBYTE,DrivPath[101]);
DEFGLB(UBYTE,DriverName[255]);
DEFGLB(UBYTE,ViewMask[40]);
DEFGLB(UBYTE,DefsMask[40]);
DEFGLB(UBYTE,SetsMask[40]);
DEFGLB(UBYTE,HatchMask[40]);
DEFGLB(UBYTE,TemplatePath[101]);
DEFGLB(UBYTE,StrokePath[101]);
DEFGLB(UBYTE,DefName[255]);
DEFGLB(UBYTE,DefsPath[101]);
DEFGLB(UBYTE,ViewPath[101]);
DEFGLB(UBYTE,HomePath[101]);
DEFGLB(UBYTE,HatchPath[101]);
DEFGLI(BOOL,Pause_Page,FALSE);
DEFGLI(BOOL,Pause_Plot,TRUE);

DEFGLB(UBYTE,Fill_Hatch_File[40]);
DEFGLB(UBYTE,Fill_Hatch_Name[40]);
DEFGLI(STDFLT,Fill_Hatch_Scale,1.0);

DEFGLI(T_HATCH *,Root_Hatch,NULL);
DEFGLI(T_HATCH *,Last_Hatch,NULL);
DEFGLI(T_HATCH *,Current_Hatch,NULL);
