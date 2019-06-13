DEFGLI(BOOL,indialog,FALSE);			/* We are in dialog */
DEFGLI(APTR,scrollanswer,NULL);			/* What was the scrollers ans */
DEFGLI(BOOL,Text_Box_Dirty,TRUE);		/* Revaluate box dimensions */
DEFGLB(UBYTE,Color_Source);			/* Layer, Pen, Group Color */
DEFGLB(BOOL,quietupdate);			/* Render while loading? */

DEFGLI(BPTR,dialog_in,NULL);			/* Standard in for dialog */
DEFGLI(BPTR,dialog_out,NULL);			/* Standard out for dialog */

DEFGLB(UWORD,Color_Buff[16][3][2]);
DEFGLI(LONG,Screen_Counter,0);
DEFGLI(WORD,ClippedColor,4);			/* Clip color reg */
DEFGLI(WORD,SelectedColor,5);			/* Select color reg */
DEFGLI(WORD,Grid_Color,3);
DEFGLI(WORD,Mist_Color,1);
DEFGLI(WORD,Ghost_Color,1);
DEFGLI(WORD,Bounds_Color,2);
DEFGLI(WORD,Coord_Color,2);
DEFGLI(WORD,Draw_Color,2);
DEFGLI(WORD,Erased_Color,0);
DEFGLI(WORD,Default_Color,1);
DEFGLB(WORD,Scr_Color);
DEFGLI(BOOL,Win_Setup,FALSE);
DEFGLI(BOOL,ClearOutReq,FALSE);

DEFGLI(WORD,Max_Vectors,2048);			/* Area fill - max vectors */

DEFGLB(UBYTE *,ucadname);			/* Get name from here */

DEFGLB(struct drawing_node *,Root_Node);
DEFGLB(struct drawing_node *,Last_Node);

DEFGLB(LAYER_A,default_layer);
DEFGLB(GROUP_A,default_group);
DEFGLB(PEN_A,default_pen);

DEFGLB(BOOL,drawingdirty);
DEFGLI(BOOL,Savnam,FALSE);
DEFGLI(BOOL,Savlib,FALSE);
DEFGLB(UBYTE,Savefile[100]);
DEFGLB(UBYTE,Savepath[257]);
DEFGLB(UBYTE,defstart[100]);

DEFGLB(UBYTE,dialog_def[10]);
DEFGLB(UBYTE,dialog_wb1[60]);
DEFGLB(UBYTE,dialog_wb2[60]);
DEFGLB(UBYTE,dialog_mn1[60]);
DEFGLB(UBYTE,dialog_mn2[60]);

DEFGLB(UBYTE,pbuffer[258]);

DEFGLB(WORD,XSsize);				/* Physical Configuration */
DEFGLB(WORD,YSsize);
DEFGLB(WORD,Sdepth);
DEFGLB(WORD,XBegin);
DEFGLB(WORD,XWidth);
DEFGLB(WORD,YBegin);
DEFGLB(WORD,YWidth);
DEFGLB(WORD,barsize);

/*-------------------Persistent Margins, Switches and Settings----------------*/

DEFGLI(STDFLT,xsnap,1.0);		/* X axis mouse increment */
DEFGLI(STDFLT,ysnap,1.0);		/* Y axis mouse increment */
DEFGLI(STDFLT,xoffset,0.0);		/* X Origin offset for snap */
DEFGLI(STDFLT,yoffset,0.0);		/* Y Origin offset for snap */
DEFGLI(STDFLT,snaprot,0.0);		/* Rotation of snap plane */
DEFGLI(STDFLT,xgrid,20.0);		/* X Size of each grid increment */
DEFGLI(STDFLT,ygrid,20.0);		/* Y Size of each grid increment */
DEFGLI(STDFLT,xgridoff,0.0);		/* X Origin offset for grid */
DEFGLI(STDFLT,ygridoff,0.0);		/* Y Origin offset for grid */
DEFGLI(WORD,xgriddiv,2);		/* Number of subdivisions on x grid */
DEFGLI(WORD,ygriddiv,2);		/* Number of subdivisions on y grid */
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
DEFGLI(STDFLT,lintolerance,.01);	/* Linear unit tolerance */
DEFGLI(WORD,linpoints,2);		/* Number of dec pts in linear unit */
DEFGLI(STDFLT,angtolerance,.1);		/* Angular unit tolerance */
DEFGLI(WORD,angpoints,1);		/* Number of dec pts in angular unit */
DEFGLI(STDFLT,aspect,1.0);		/* Ratio of x/y on axis */
DEFGLI(STDFLT,skew,1.0);		/* Must be 1.0 - Unused */
DEFGLI(STDFLT,multi_space,1.0);		/* Multi-line spacing */
DEFGLI(WORD,multi_line,2);		/* Number of Multi-lines */
DEFGLI(WORD,free_pixels,0);		/* Free hand pixel jump */
DEFGLI(BOOL,FreeHandFB,FALSE);		/* Use free hand pointer style */

DEFGLI(BOOL,Center_On,FALSE);
DEFGLI(BOOL,Handles_On,FALSE);
DEFGLI(BOOL,Coord_On,TRUE);
DEFGLI(BOOL,Draw_Info,TRUE);
DEFGLI(BOOL,Snaps_On,TRUE);
DEFGLI(BOOL,Grid_On,TRUE);
DEFGLI(BOOL,Use_Bounds,TRUE);
DEFGLI(BOOL,Ortho_Switch,TRUE);
DEFGLI(BOOL,Cursors_On,FALSE);
DEFGLI(BOOL,Q_Hatch,FALSE);
DEFGLI(BOOL,Library_Mode,FALSE);

DEFGLB(UBYTE,ViewPath[PATHMAX+1]);
DEFGLB(UBYTE,LibrPath[PATHMAX+1]);
DEFGLB(UBYTE,StrokePath[PATHMAX+1]);
DEFGLB(UBYTE,TemplatePath[PATHMAX+1]);
DEFGLB(UBYTE,HomePath[PATHMAX+1]);
DEFGLB(UBYTE,HatchPath[PATHMAX+1]);
DEFGLB(UBYTE,RexxPath[PATHMAX+1]);
DEFGLB(UBYTE,DialogPath[PATHMAX+1]);
DEFGLB(UBYTE,ReportDev[40]);

DEFGLI(BOOL,Screen_Save_On,TRUE);
DEFGLI(int,Screen_Save_Time,300);

DEFGLI(BOOL,Scan_Clip,FALSE);
DEFGLI(BOOL,Scan_Group,FALSE);
DEFGLI(BOOL,Scan_Layer,FALSE);
DEFGLI(BOOL,Scan_Pen,FALSE);
DEFGLI(BOOL,Scan_Text,FALSE);

DEFGLI(WORD,angtype,ANG_DEGD);
DEFGLI(WORD,lintype,LIN_MM);

DEFGLB(STDFLT,xvbeg[10]);
DEFGLB(STDFLT,yvbeg[10]);
DEFGLB(STDFLT,xvsiz[10]);
DEFGLB(STDFLT,yvsiz[10]);
DEFGLB(BOOL,vset[10]);

DEFGLB(STDFLT,xviewsize);			/* Current Temporary Margins */
DEFGLB(STDFLT,yviewsize);
DEFGLB(STDFLT,xviewbegin);
DEFGLB(STDFLT,yviewbegin);
DEFGLI(STDFLT,viewrot,0.0);
DEFGLB(STDFLT,xtsnap);
DEFGLB(STDFLT,ytsnap);
DEFGLB(STDFLT,xpixsize);
DEFGLB(STDFLT,ypixsize);
DEFGLI(STDFLT,x_origin,0.0);
DEFGLI(STDFLT,y_origin,0.0);

DEFGLB(T_FONT *,Root_Font);		/* Font linked list */
DEFGLB(T_FONT *,Font_Last);

DEFGLB(STYLE_A *,Root_Style);	/* Style linked list */
DEFGLB(STYLE_A *,Style_Last);
DEFGLB(STYLE_A *,Current_Style);
DEFGLB(STYLE_A *,Default_Style);

DEFGLB(LAYER_A *,Root_Layer);	/* Layer linked list */
DEFGLB(LAYER_A *,Last_Layer);
DEFGLB(LAYER_A *,Current_Layer);

DEFGLB(GROUP_A *,Root_Group);	
DEFGLB(GROUP_A *,Current_Group);

DEFGLB(PEN_A *,Root_Pen);
DEFGLB(PEN_A *,Last_Pen);
DEFGLB(PEN_A *,Current_Pen);
DEFGLB(PEN_A *,Default_Pen);

DEFGLI(T_HATCH *,Root_Hatch,NULL);
DEFGLI(T_HATCH *,Last_Hatch,NULL);
DEFGLI(T_HATCH *,Current_Hatch,NULL);

DEFGLB(UBYTE,ClipMask[MASKMAX+1]);
DEFGLB(UBYTE,ViewMask[MASKMAX+1]);
DEFGLB(UBYTE,LibrMask[MASKMAX+1]);
DEFGLB(UBYTE,FontMask[MASKMAX+1]);
DEFGLB(UBYTE,LayerMask[MASKMAX+1]);
DEFGLB(UBYTE,StyleMask[MASKMAX+1]);
DEFGLB(UBYTE,GroupMask[MASKMAX+1]);
DEFGLB(UBYTE,PenMask[MASKMAX+1]);
DEFGLB(UBYTE,FntMask[MASKMAX+1]);
DEFGLB(UBYTE,ItemMask[MASKMAX+1]);
DEFGLB(UBYTE,HatchMask[MASKMAX+1]);

DEFGLB(ULONG,StoSecs);
DEFGLB(ULONG,StoMics);

DEFGLB(STDFLT,dotx);
DEFGLB(STDFLT,doty);
DEFGLB(STDFLT,keyx[10]);
DEFGLB(STDFLT,keyy[10]);
DEFGLB(BOOL,keyset[10]);
DEFGLB(BOOL,dotset);

DEFGLB(WORD,keepx);		/* Last Crosshair Position */
DEFGLB(WORD,keepy);

DEFGLI(BOOL,PoolsSome,FALSE);
DEFGLB(struct drawing_node *,lastnode);

DEFGLI(struct drawing_node *,CurrentRoller,NULL);

DEFGLB(GROUP_A **,Group_Array);
DEFGLB(PEN_A **,Pen_Array);
DEFGLB(LAYER_A **,Layer_Array);
DEFGLB(STYLE_A **,Style_Array);

DEFGLB(WORD,grpoffs);
DEFGLB(WORD,grpmax);

DEFGLI(BOOL,regroup,FALSE);

DEFGLI(struct Window *,w,NULL);
DEFGLI(struct Window *,dialwin,NULL);
DEFGLI(struct Screen *,screen,NULL);
DEFGLI(struct RastPort *,rp,NULL);
DEFGLI(struct ViewPort *,vp,NULL);
DEFGLI(struct IntuiMessage *,message,NULL);

DEFGLI(UBYTE *,Main_Mode,NULL);
DEFGLI(UBYTE *,Last_Mode,NULL);
DEFGLI(UBYTE *,Save_Mode,NULL);
DEFGLI(UBYTE *,SaveSide_Mode,NULL);
DEFGLI(UBYTE *,Side_Mode,NULL);

DEFGLI(struct mesdef *,Main_Mess,NULL);
DEFGLI(struct mesdef *,Last_Mess,NULL);
DEFGLI(struct mesdef *,Save_Mess,NULL);
DEFGLI(struct mesdef *,SaveSide_Mess,NULL);
DEFGLI(struct mesdef *,Main_Out,NULL);
DEFGLI(struct mesdef *,Side_Mess,NULL);
DEFGLI(struct mesdef *,Side_Out,NULL);

DEFGLB(WORD,clip_sret);
DEFGLB(WORD,EllipseClip);

DEFGLB(STDFLT,lastx);
DEFGLB(STDFLT,lasty);

DEFGLB(UBYTE,ArgText[257]);
DEFGLB(UBYTE,undobuf[257]);

DEFGLI(LAYER_A *,Wit_Layer,NULL);
DEFGLI(GROUP_A *,Wit_Group,NULL);
DEFGLI(PEN_A *,Wit_Pen,NULL);

DEFGLI(LAYER_A *,Dim_Line_Layer,NULL);
DEFGLI(GROUP_A *,Dim_Line_Group,NULL);
DEFGLI(PEN_A *,Dim_Line_Pen,NULL);

DEFGLI(LAYER_A *,Dim_Box_Layer,NULL);
DEFGLI(GROUP_A *,Dim_Box_Group,NULL);
DEFGLI(PEN_A *,Dim_Box_Pen,NULL);
DEFGLI(STYLE_A *,Dim_Box_Style,NULL);

DEFGLI(BOOL,B_Wit_Layer,FALSE);
DEFGLI(BOOL,B_Wit_Group,FALSE);
DEFGLI(BOOL,B_Wit_Pen,FALSE);

DEFGLI(BOOL,B_Dim_Line_Layer,FALSE);
DEFGLI(BOOL,B_Dim_Line_Group,FALSE);
DEFGLI(BOOL,B_Dim_Line_Pen,FALSE);

DEFGLI(BOOL,B_Dim_Box_Layer,FALSE);
DEFGLI(BOOL,B_Dim_Box_Group,FALSE);
DEFGLI(BOOL,B_Dim_Box_Pen,FALSE);
DEFGLI(BOOL,B_Dim_Box_Style,FALSE);

DEFGLI(STDFLT,Wit_Pick,1.0);
DEFGLI(STDFLT,Wit_Over,2.0);
DEFGLI(STDFLT,Dim_Line_Offset,.1);
DEFGLI(STDFLT,Dim_Box_Offset,1.0);
DEFGLI(BOOL,Use_Fixed_Angle,TRUE);
DEFGLI(STDFLT,Fixed_Angle,0.0);
DEFGLI(STDFLT,Free_Flip_From,PI/2.0+.01);
DEFGLI(STDFLT,Free_Flip_To,3.0*PI/2.0-.01);
DEFGLI(STDFLT,Free_Offset,0.0);
