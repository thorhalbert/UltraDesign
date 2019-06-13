#include "ucad.h"

VOID obhandler();
VOID linemvalid();
VOID ellipmvalid();
VOID textmvalid();
VOID hatchmvalid();

#define TURN_OFF(x) x.Flags &= ~SELECTED
#define TURN_ON(x) x.Flags |= SELECTED

#define GADGET_OFF(x) OffGadget(x,w,&ObjectRequester)
#define GADGET_ON(x) OnGadget(x,w,&ObjectRequester)

#define REQWID 368
#define REQHIT 192

LOCAL BOOL exflag;
LOCAL int postaction;

LOCAL O_LINE *ob_line;
LOCAL O_ELLIP *ob_ellip;
LOCAL O_TEXT *ob_text;
LOCAL O_HATCH *ob_hatch;

LOCAL STDFLT firstx,firsty,secx,secy,radx,rady,angb,ange,rot,lang,llen;
LOCAL ULONG lineflg,ellipflag,textflg,texhan;
LOCAL STDFLT pscale;

LOCAL GROUP_A *ob_group;
LOCAL LAYER_A *ob_layer;
LOCAL PEN_A *ob_pen;
LOCAL STYLE_A *ob_style;
LOCAL T_HATCH *ob_pattern;

extern struct Image cancel_u_image;
extern struct Image cancel_s_image;
extern struct Image create_u_image;
extern struct Image create_s_image;
extern struct Image ellipse_u_image;
extern struct Image ellipse_s_image;
extern struct Image fill_u_image;
extern struct Image fill_s_image;
extern struct Image hatch_u_image;
extern struct Image hatch_s_image;
extern struct Image lines_u_image;
extern struct Image lines_s_image;
extern struct Image next_u_image;
extern struct Image next_s_image;
extern struct Image ok_u_image;
extern struct Image ok_s_image;
extern struct Image prev_u_image;
extern struct Image prev_s_image;
extern struct Image text_u_image;
extern struct Image text_s_image;

#define GAD_STR(seq,x,y,width,strinfo,id,service) \
 { &GADNAM[seq+1],x,y,width,9,GADGHCOMP,RELVERIFY|GADGIMMEDIATE, \
   STRGADGET|REQGADGET, NULL, NULL, NULL, 0, &strinfo, id, service},

#define GAD_STRR(seq,x,y,width,strinfo,id,service) \
 { &GADNAM[seq+1],x,y,width,9,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, \
   STRGADGET|REQGADGET, NULL, NULL, NULL, 0, &strinfo, id, service},

#define GAD_BUT(seq,x,y,width,text,id,service) \
 { &GADNAM[seq+1],x,y,width*8,8,GADGHCOMP,GADGIMMEDIATE|RELVERIFY,	\
   BOOLGADGET|REQGADGET,NULL,NULL,&text,0,NULL,id,(APTR) service},
   
#define GAD_IMG(seq,x,y,width,height,norim,selim,id,service) \
 { &GADNAM[seq+1],x,y,width,height,GADGIMAGE|GADGHIMAGE, \
   GADGIMMEDIATE|RELVERIFY,BOOLGADGET|REQGADGET, \
   &norim,&selim,NULL,0,NULL,id,service},

#define GAD_IMT(seq,x,y,width,height,norim,selim,id,service) \
 { &GADNAM[seq+1],x,y,width,height,GADGIMAGE|GADGHIMAGE, \
   GADGIMMEDIATE|RELVERIFY|TOGGLESELECT,BOOLGADGET|REQGADGET, \
   &norim,&selim,NULL,0,NULL,id,service},

#define GAD_STR_T(seq,x,y,width,strinfo,id,service) \
 { seq,x,y,width,9,GADGHCOMP,RELVERIFY|GADGIMMEDIATE, \
   STRGADGET|REQGADGET, NULL, NULL, NULL, 0, &strinfo, id, service},

#define GAD_STRR_T(seq,x,y,width,strinfo,id,service) \
 { seq,x,y,width,9,GADGHCOMP,RELVERIFY|GADGIMMEDIATE|STRINGRIGHT, \
   STRGADGET|REQGADGET, NULL, NULL, NULL, 0, &strinfo, id, service},

#define GAD_BUT_T(seq,x,y,width,text,id,service) \
 { seq,x,y,width*8,8,GADGHCOMP,GADGIMMEDIATE|RELVERIFY,	\
   BOOLGADGET|REQGADGET,NULL,NULL,&text,0,NULL,id,(APTR) service},
   
#define GAD_IMG_T(seq,x,y,width,height,norim,selim,id,service) \
 { seq,x,y,width,height,GADGIMAGE|GADGHIMAGE, \
   GADGIMMEDIATE|RELVERIFY,BOOLGADGET|REQGADGET, \
   &norim,&selim,NULL,0,NULL,id,service},

#define GAD_IMT_T(seq,x,y,width,height,norim,selim,id,service) \
 { seq,x,y,width,height,GADGIMAGE|GADGHIMAGE, \
   GADGIMMEDIATE|RELVERIFY|TOGGLESELECT,BOOLGADGET|REQGADGET, \
   &norim,&selim,NULL,0,NULL,id,service},

#define START_GADGET struct Gadget GADNAM[] = {
#define END_GADGET };

LOCAL struct IntuiText ObjectHeadings[] = {
	{ 3,1,JAM2, 8,  6,NULL,"Edit Drawing Objects:",&ObjectHeadings[1] },
	{ 2,1,JAM2,45,REQHIT-50,NULL,"Layer:",&ObjectHeadings[2] },
	{ 2,1,JAM2,45,REQHIT-40,NULL,"  Pen:",&ObjectHeadings[3] },
	{ 2,1,JAM2,45,REQHIT-30,NULL,"Group:",NULL },
};

#define NO_OBJECT 0
#define LINE_OBJECT 1
#define ELLIPSE_OBJECT 2
#define TEXT_OBJECT 3
#define FILL_OBJECT 4
#define HATCH_OBJECT 5

int what_mode = NO_OBJECT;

/*-------------------------Main Gadgets--------------------------------*/

LOCAL struct IntuiText BottomText[] = {
	{ 2,3,JAM2,0,0,NULL," Next ",NULL },
	{ 2,3,JAM2,0,0,NULL," Prev ",NULL },
	{ 2,3,JAM2,0,0,NULL," Change ",NULL },
	{ 2,3,JAM2,0,0,NULL," Create ",NULL },
	{ 2,3,JAM2,0,0,NULL," Cancel ",NULL },
};

LOCAL UBYTE OBText[3][32];

LOCAL struct IntuiText OBList[] = {
	{ 3,2,JAM2,0,0,NULL,&OBText[0][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&OBText[1][0],NULL },
	{ 3,2,JAM2,0,0,NULL,&OBText[2][0],NULL },
};

#define GADNAM main_gads
START_GADGET

	GAD_BUT(     0, 20,REQHIT-16, 6, BottomText[0], 10, obhandler)
	GAD_BUT(     1, 78,REQHIT-16, 6, BottomText[1], 11, obhandler)
	GAD_BUT(     2,136,REQHIT-16, 8, BottomText[2], 12, obhandler)
	GAD_BUT(     3,210,REQHIT-16, 8, BottomText[3], 13, obhandler)
	GAD_BUT(     4,284,REQHIT-16, 8, BottomText[4], 14, obhandler)

	GAD_BUT(     5,100,REQHIT-50, 30, OBList[0], 15, obhandler)
	GAD_BUT(     6,100,REQHIT-40, 30, OBList[1], 16, obhandler)
	GAD_BUT(     7,100,REQHIT-30, 30, OBList[2], 17, obhandler)

	GAD_IMG(     8, 20,23,64,15,lines_u_image,lines_s_image,20,obhandler)
	GAD_IMG(     9, 86,23,64,15,ellipse_u_image,ellipse_s_image,21,obhandler)
	GAD_IMG(    10,156,23,64,15,text_u_image,text_s_image,22,obhandler)
	GAD_IMG(    11,222,23,64,15,fill_u_image,fill_s_image,23,obhandler)
	GAD_IMG_T(NULL,288,23,64,15,hatch_u_image,hatch_s_image,24,obhandler)
	
END_GADGET

BOOL change=TRUE;

#define CHANGE_ON GADGET_ON(&main_gads[2]), change=TRUE
#define CHANGE_OFF GADGET_OFF(&main_gads[2]), change=FALSE

/*-------------------------Line Gadgets---------------------------------*/

extern struct Image line_1u_image;
extern struct Image line_1s_image;
extern struct Image line_2u_image;
extern struct Image line_2s_image;

UBYTE linedata[4][32];


#define LT 45
#define OF 30

LOCAL struct IntuiText LineHeads[] = {
	{ 0,1,JAM2,OF+ 20,LT,NULL,"   ---------Line Mode---------",&LineHeads[1] },
	{ 2,1,JAM2,OF+ 50-32,LT+10,NULL,"X1: ",&LineHeads[2] },
	{ 2,1,JAM2,OF+200-32,LT+10,NULL,"Y1: ",&LineHeads[3] },
	{ 2,1,JAM2,OF+ 50-32,LT+20,NULL,"X2: ",&LineHeads[4] },
	{ 2,1,JAM2,OF+200-32,LT+20,NULL,"Y2: ",&LineHeads[5] },
	{ 2,1,JAM2,OF+ 50-64,LT+30,NULL," Angle: ",&LineHeads[6] },
	{ 2,1,JAM2,OF+200-64,LT+30,NULL,"Length: ",NULL },
};

struct StringInfo linestr[] = {
	{ &linedata[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &linedata[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &linedata[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &linedata[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &linedata[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &linedata[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
};

#undef GADNAM
#define GADNAM line_mode
START_GADGET

	GAD_STRR( 0,OF+ 50,LT+10,80,linestr[0],10,linemvalid)
	GAD_STRR( 1,OF+200,LT+10,80,linestr[1],11,linemvalid)
	GAD_STRR( 2,OF+ 50,LT+20,80,linestr[2],12,linemvalid)
	GAD_STRR( 3,OF+200,LT+20,80,linestr[3],13,linemvalid)
	GAD_STRR( 4,OF+ 50,LT+30,80,linestr[4],14,linemvalid)
	GAD_STRR( 5,OF+200,LT+30,80,linestr[5],15,linemvalid)
	
#define AGG (31+31+8)
#define LEF ((REQWID-AGG)/2)
#define RIG (LEF+31+8)
	
	GAD_IMT( 6,LEF,LT+50,31,27,line_1u_image,line_1s_image,16,linemvalid)
	GAD_IMT_T(NULL,RIG,LT+50,31,27,line_2u_image,line_2s_image,17,linemvalid)

END_GADGET

/*-------------------------Ellipse Gadgets---------------------------------*/


UBYTE ellipdata[7][32];

#undef LT
#undef OF
#define LT 60
#define OF 50

LOCAL struct IntuiText EllipHeads[] = {
	{ 0,1,JAM2,OF,LT,NULL,"-----------Ellipse Mode-----------",
			&EllipHeads[1] },
	{ 2,1,JAM2,OF+ 50-90,LT+10,NULL," X Center: ",&EllipHeads[2] },
	{ 2,1,JAM2,OF+230-90,LT+10,NULL," Y Center: ",&EllipHeads[3] },
	{ 2,1,JAM2,OF+ 50-90,LT+20,NULL," X Radius: ",&EllipHeads[4] },
	{ 2,1,JAM2,OF+230-90,LT+20,NULL," Y Radius: ",&EllipHeads[5] },
	{ 2,1,JAM2,OF+ 50-90,LT+30,NULL,"Start Ang: ",&EllipHeads[6] },
	{ 2,1,JAM2,OF+230-90,LT+30,NULL,"  End Ang: ",&EllipHeads[7] },
	{ 2,1,JAM2,OF+ 50-90,LT+40,NULL," Rotation: ",NULL },
};

struct StringInfo ellipstr[] = {
	{ &ellipdata[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[3][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[4][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[5][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &ellipdata[6][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
};

#undef GADNAM
#define GADNAM ellip_mode
START_GADGET

	GAD_STRR( 0,OF+ 50,LT+10,80,ellipstr[0],10,ellipmvalid)
	GAD_STRR( 1,OF+230,LT+10,80,ellipstr[1],11,ellipmvalid)
	GAD_STRR( 2,OF+ 50,LT+20,80,ellipstr[2],12,ellipmvalid)
	GAD_STRR( 3,OF+230,LT+20,80,ellipstr[3],13,ellipmvalid)
	GAD_STRR( 4,OF+ 50,LT+30,80,ellipstr[4],14,ellipmvalid)
	GAD_STRR( 5,OF+230,LT+30,80,ellipstr[5],15,ellipmvalid)
	GAD_STRR( 6,OF+ 50,LT+40,80,ellipstr[6],16,ellipmvalid)

END_GADGET

/*-------------------------Text Gadgets---------------------------------*/

extern struct Image text_4s_image;
extern struct Image text_4u_image;
extern struct Image text_5s_image;
extern struct Image text_5u_image;
extern struct Image text_6s_image;
extern struct Image text_6u_image;
extern struct Image text_7s_image;
extern struct Image text_7u_image;
extern struct Image text_8s_image;
extern struct Image text_8u_image;
extern struct Image text_9s_image;
extern struct Image text_9u_image;

UBYTE textdata[3][32];
UBYTE textbuf[300];

#undef LT
#undef OF

#define LT 45
#define OF 40

LOCAL struct IntuiText TextHeads[] = {
	{ 0,1,JAM2,OF+ 20,LT,NULL,
		"   ---------Text Mode---------",&TextHeads[1] },
	{ 2,1,JAM2,OF+ 50-72,LT+10,NULL,"Handle X:",&TextHeads[2] },
	{ 2,1,JAM2,OF+230-72,LT+10,NULL,"Handle Y:",&TextHeads[3] },
	{ 2,1,JAM2,OF+ 50-72,LT+20,NULL,"Rotation:",&TextHeads[4] },
	{ 2,1,JAM2,OF+ 50-72,LT+30,NULL,"Text: ",&TextHeads[5] },
	{ 2,1,JAM2,45,REQHIT-60,NULL,"Style:",NULL },
};

LOCAL UBYTE TXText[32];

LOCAL struct IntuiText TXList[] = {
	{ 3,2,JAM2,0,0,NULL,&TXText[0],NULL },
};

struct StringInfo textstr[] = {
	{ &textdata[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &textdata[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &textdata[2][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &textbuf[0],     undobuf, 0,254,0,0,0,0,0,0,NULL,0,NULL },
};

#undef GADNAM
#define GADNAM text_mode
START_GADGET

	GAD_STRR( 0,OF+ 50,LT+10,80,textstr[0],10,textmvalid)
	GAD_STRR( 1,OF+230,LT+10,80,textstr[1],11,textmvalid)
	GAD_STRR( 2,OF+ 50,LT+20,80,textstr[2],12,textmvalid)
	GAD_STR( 3,OF+ 20,LT+30,280,textstr[3],13,textmvalid)

	GAD_IMT( 4, 74,LT+50,31,27,text_4u_image,text_4s_image,14,textmvalid)
	GAD_IMT( 5,109,LT+50,31,27,text_5u_image,text_5s_image,15,textmvalid)
	GAD_IMT( 6,144,LT+50,31,27,text_6u_image,text_6s_image,16,textmvalid)
	GAD_IMT( 7,194,LT+50,31,27,text_7u_image,text_7s_image,17,textmvalid)
	GAD_IMT( 8,229,LT+50,31,27,text_8u_image,text_8s_image,18,textmvalid)
	GAD_IMT( 9,264,LT+50,31,27,text_9u_image,text_9s_image,19,textmvalid)

	GAD_BUT_T(NULL,100,REQHIT-60, 30, TXList[0], 18, obhandler)

END_GADGET

/*-------------------------Fill Gadgets---------------------------------*/

#undef LT
#undef OF

#define LT 45
#define OF 40

LOCAL struct IntuiText FillHeads[] = {
	{ 0,1,JAM2,OF+ 20,LT,NULL,
		"   ---------Fill Mode---------",NULL },
};

/*-------------------------Hatch Gadgets---------------------------------*/

UBYTE hatchdata[2][32];

#undef LT
#undef OF

#define LT 45
#define OF 40

LOCAL struct IntuiText HatchHeads[] = {
	{ 0,1,JAM2,OF+ 20,LT,NULL,
		"   ---------Hatch Mode---------",&HatchHeads[1] },
	{ 2,1,JAM2,OF+ 50-72,LT+10,NULL,"   Scale:",&HatchHeads[2] },
	{ 2,1,JAM2,OF+230-72,LT+10,NULL,"Rotation:",&HatchHeads[3] },

	{ 2,1,JAM2,45-16,REQHIT-60,NULL,"Pattern:",NULL },
};

LOCAL UBYTE HTText[32];

LOCAL struct IntuiText HTList[] = {
	{ 3,2,JAM2,0,0,NULL,&HTText[0],NULL },
};

struct StringInfo hatchstr[] = {
	{ &hatchdata[0][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
	{ &hatchdata[1][0], undobuf, 0, 30,0,0,0,0,0,0,NULL,0,NULL },
};

#undef GADNAM
#define GADNAM hatch_mode
START_GADGET

	GAD_STRR( 0,OF+ 50,LT+10,80,hatchstr[0],10,hatchmvalid)
	GAD_STRR( 1,OF+230,LT+10,80,hatchstr[1],11,hatchmvalid)

	GAD_BUT_T(NULL,100,REQHIT-60, 30, HTList[0], 19, obhandler)

END_GADGET

/*--------------------------------------------------------------------------*/

LOCAL struct Requester ObjectRequester;

VOID setal()	/* Compute lang, llen from firstx,firsty,secx,secy */
{

	STDFLT dx,dy;
	
	dx = secx - firstx;
	dy = secy - firsty;
	
	llen = sqrt(dx*dx+dy*dy);
	
	if (dx==0.0&&dy==0.0) return;
	
	lang = atan2(dy,dx);

}

VOID setxy()	/* Compute secx,secy from firstx,firsty,lang,llen */
{

	secx = firstx + (cos(lang) * llen);
	secy = firsty + (sin(lang) * llen);
	
}

Fill_Boxes()
{
	int a;


	switch (what_mode) {
	case LINE_OBJECT:

		sprintf(&linedata[0][0],"%s",DispLin(firstx));
		sprintf(&linedata[1][0],"%s",DispLin(firsty));
		sprintf(&linedata[2][0],"%s",DispLin(secx));
		sprintf(&linedata[3][0],"%s",DispLin(secy));
		setal();
		sprintf(&linedata[4][0],"%s",DispAng(lang));
		sprintf(&linedata[5][0],"%s",DispLin(llen));

		TURN_OFF(line_mode[6]);
		TURN_OFF(line_mode[7]);
		
		if (lineflg&LINE_P1ARROW) TURN_ON(line_mode[6]);
		if (lineflg&LINE_P2ARROW) TURN_ON(line_mode[7]);
		
		for (a=0; a<4; a++) StgSet(&linestr[a]);
		break;
		
	case ELLIPSE_OBJECT:
	
		sprintf(&ellipdata[0][0],"%s",DispLin(firstx));
		sprintf(&ellipdata[1][0],"%s",DispLin(firsty));
		sprintf(&ellipdata[2][0],"%s",DispLin(radx));
		sprintf(&ellipdata[3][0],"%s",DispLin(rady));
		sprintf(&ellipdata[4][0],"%s",DispAng(angb));
		sprintf(&ellipdata[5][0],"%s",DispAng(ange));
		sprintf(&ellipdata[6][0],"%s",DispAng(rot));

		for (a=0; a<7; a++) StgSet(&ellipstr[a]);
		break;
		
	case TEXT_OBJECT:
	
		sprintf(&textdata[0][0],"%s",DispLin(firstx));
		sprintf(&textdata[1][0],"%s",DispLin(firsty));
		sprintf(&textdata[2][0],"%s",DispAng(rot));

		for (a=0; a<3; a++) StgSet(&textstr[a]);
		
		TURN_OFF(text_mode[4]);
		TURN_OFF(text_mode[5]);
		TURN_OFF(text_mode[6]);
		TURN_OFF(text_mode[7]);
		TURN_OFF(text_mode[8]);
		TURN_OFF(text_mode[9]);
		
		if (texhan==LEFT_HANDLE)	TURN_ON(text_mode[4]);
		if (texhan==CENTER_HANDLE)	TURN_ON(text_mode[5]);
		if (texhan==RIGHT_HANDLE)	TURN_ON(text_mode[6]);

		if (textflg&TEXT_FLIPX)		TURN_ON(text_mode[7]);
		if (textflg&TEXT_CHRFL)		TURN_ON(text_mode[8]);
		if (textflg&TEXT_FLIPY)		TURN_ON(text_mode[9]);

		break;

	case HATCH_OBJECT:
	
		sprintf(&hatchdata[0][0],"%.4g",pscale);
		sprintf(&hatchdata[1][0],"%s",DispAng(rot));
		
		for (a=0; a<2; a++) StgSet(&hatchstr[a]);
		
		break;
	}
}

Create_Current_Ob()
{

	GROUP_A *sgroup;
	LAYER_A *slayer;
	PEN_A *spen;
	int i;

	O_HATCH	newhat;
	
	sgroup = Current_Group;
	slayer = Current_Layer;
	spen = Current_Pen;
	
	Current_Group = ob_group;
	Current_Layer = ob_layer;
	Current_Pen = ob_pen;
	
	switch (what_mode) {
	case LINE_OBJECT:
		i = 0;
		if (line_mode[6].Flags&SELECTED) i |= LINE_P1ARROW;
		if (line_mode[7].Flags&SELECTED) i |= LINE_P2ARROW;

		CreateLine(firstx,firsty,secx,secy,i);

		break;	
	case ELLIPSE_OBJECT:

		if (radx<=0.0||
		    rady<=0.0) {	
			complain(NULL,"PROCEED",0,0,0,0,
				"Ellipse radii must both be greater",
				"than zero",NULL,NULL);
			break;
		}

		CreateEllipse(firstx,firsty,radx,rady,rot,angb,ange);
						
		break;
		
	case TEXT_OBJECT:
	
		if (strlen(textbuf)<=0) {
			complain(NULL,"PROCEED",0,0,0,0,
				"Text string cannot be empty",
				NULL,NULL,NULL);
			break;
		}
		
		i = 0;
		if (text_mode[7].Flags&SELECTED) i |= TEXT_FLIPX;
		if (text_mode[8].Flags&SELECTED) i |= TEXT_CHRFL;
		if (text_mode[9].Flags&SELECTED) i |= TEXT_FLIPY;

		CreateText(textbuf,firstx,firsty,rot * 180.0/PI,
			texhan,i,ob_style);
		
		break;
	
	case FILL_OBJECT:
	
		newhat.hatch_type = HATCH_SOLID;
		newhat.hatch_flags = 0;
		newhat.pattern_rot = 0.0;
		newhat.pattern_scale = 1.0;
		newhat.hatch_pattern = NULL;
		newhat.number_polys = 1;
		
		CreateHatch(&newhat,NULL);
		
		break;
		
	case HATCH_OBJECT:
	
		if (ob_pattern==NULL) {
			complain(NULL,"PROCEED",0,0,0,0,
				"A hatching pattern must be selected!"
				"If a hatch pattern file needs to loaded,",
				"do it in the hatch mode setup requester",
				NULL);
			break;
		}
		
		if (pscale<=0.0) {
			complain(NULL,"PROCEED",0,0,0,0,
				"Scale must be greater than zero",
				NULL,NULL,NULL);
			break;
		}
			
		newhat.hatch_type = HATCH_PATTERN;
		newhat.hatch_flags = 0;
		newhat.pattern_rot = rot;
		newhat.pattern_scale = pscale;
		newhat.hatch_pattern = ob_pattern;
		newhat.number_polys = 1;
		
		CreateHatch(&newhat,NULL);
		
		break;
	}
	
	Current_Group = sgroup;
	Current_Layer = slayer;
	Current_Pen = spen;

}

Save_Current_Ob()
{

	UBYTE *newtex;

	switch (what_mode) {
	case LINE_OBJECT:
		ob_line = GetLineData(CurrentRoller);
		
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		CurrentRoller->node_flags |= NODE_DELETED;
		GenericRender(CurrentRoller);
		
		ob_line->x1 = firstx;
		ob_line->y1 = firsty;
		ob_line->x2 = secx;
		ob_line->y2 = secy;
		
		ob_line->line_flags &= ~(LINE_P1ARROW|LINE_P2ARROW);
		if (line_mode[6].Flags&SELECTED) 
			ob_line->line_flags |= LINE_P1ARROW;
		if (line_mode[7].Flags&SELECTED) 
			ob_line->line_flags |= LINE_P2ARROW;

		CurrentRoller->node_layer = ob_layer;
		CurrentRoller->pen_attach = ob_pen;
		CurrentRoller->above_group = ob_group;
		
		CurrentRoller->node_flags &= (~NODE_DELETED);
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
		
		break;
	
	case ELLIPSE_OBJECT:

		if (radx<=0.0||
		    rady<=0.0) {	
			complain(NULL,"PROCEED",0,0,0,0,
				"Ellipse radii must both be greater",
				"than zero",NULL,NULL);
			return;
		}
		
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		CurrentRoller->node_flags |= NODE_DELETED;
		GenericRender(CurrentRoller);

		ob_ellip = GetEllipData(CurrentRoller);
		
		ob_ellip->x1 = firstx;
		ob_ellip->y1 = firsty;
		ob_ellip->xradius = radx;
		ob_ellip->yradius = rady;
		ob_ellip->fromang = angb;
		ob_ellip->toangle = ange;
		ob_ellip->rotation = rot;
		
		CurrentRoller->node_layer = ob_layer;
		CurrentRoller->pen_attach = ob_pen;
		CurrentRoller->above_group = ob_group;
		
		CurrentRoller->node_flags &= (~NODE_DELETED);
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
		
		break;
		
	case TEXT_OBJECT:
	
		if (strlen(textbuf)<=0) {
			complain(NULL,"PROCEED",0,0,0,0,
				"Text string cannot be empty",
				NULL,NULL,NULL);
			return;
		}
		
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		CurrentRoller->node_flags |= NODE_DELETED;
		GenericRender(CurrentRoller);
		
		ob_text = GetTextData(CurrentRoller);
		
		ob_text->x1 = firstx;
		ob_text->y1 = firsty;
		
		ob_text->Handle_Flag = texhan;
		
		newtex = GI_DrawAlloc(strlen(textbuf)+2);
		if (newtex==NULL) return;
		strcpy(newtex,textbuf);
		ob_text->text = newtex;
		
		ob_text->letstyle = ob_style;
		
		ob_text->ang = rot * 180.0/PI;

		ob_text->Text_Bits &= ~(TEXT_FLIPX|TEXT_CHRFL|TEXT_FLIPY);
		if (text_mode[7].Flags&SELECTED) 
			ob_text->Text_Bits |= TEXT_FLIPX;
		if (text_mode[8].Flags&SELECTED) 
			ob_text->Text_Bits |= TEXT_CHRFL;
		if (text_mode[9].Flags&SELECTED) 
			ob_text->Text_Bits |= TEXT_FLIPY;

		CurrentRoller->node_layer = ob_layer;
		CurrentRoller->pen_attach = ob_pen;
		CurrentRoller->above_group = ob_group;
		
		CurrentRoller->node_flags &= (~NODE_DELETED);
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
				
		break;
	
	case FILL_OBJECT:
	
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		CurrentRoller->node_flags |= NODE_DELETED;
		GenericRender(CurrentRoller);
		
		ob_hatch = GetHatchData(CurrentRoller);
		ob_hatch->hatch_type = HATCH_SOLID;
		ob_hatch->pattern_rot = 0.0;
		ob_hatch->pattern_scale = 1.0;
		ob_hatch->hatch_pattern = NULL;
		
		CurrentRoller->node_layer = ob_layer;
		CurrentRoller->pen_attach = ob_pen;
		CurrentRoller->above_group = ob_group;
		
		CurrentRoller->node_flags &= (~NODE_DELETED);
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
		break;
		
	case HATCH_OBJECT:
	
		if (ob_pattern==NULL) {
			complain(NULL,"PROCEED",0,0,0,0,
				"A hatching pattern must be selected!"
				"If a hatch pattern file needs to loaded,",
				"do it in the hatch mode setup requester",
				NULL);
			return;
		}
		
		if (pscale<=0.0) {
			complain(NULL,"PROCEED",0,0,0,0,
				"Scale must be greater than zero",
				NULL,NULL,NULL);
			return;
		}
			
		CurrentRoller->node_flags &= (~NODE_SELECTED);
		CurrentRoller->node_flags |= NODE_DELETED;
		GenericRender(CurrentRoller);
		
		ob_hatch = GetHatchData(CurrentRoller);
		
		ob_hatch->hatch_type=HATCH_PATTERN;

		ob_hatch->pattern_scale = pscale;
		ob_hatch->pattern_rot = rot;
		ob_hatch->hatch_pattern = ob_pattern;
			
		CurrentRoller->node_layer = ob_layer;
		CurrentRoller->pen_attach = ob_pen;
		CurrentRoller->above_group = ob_group;
		
		CurrentRoller->node_flags &= (~NODE_DELETED);
		CurrentRoller->node_flags |= NODE_SELECTED;
		GenericRender(CurrentRoller);
		
		break;
	}


}

VOID obhandler(ingad)
struct Gadget *ingad;
{

	postaction = 0;
	CloseRequest(&ObjectRequester,w);

	switch (ingad->GadgetID) {

	case 10:	Roll_Right();
			Setup_Current_Ob();
			Fill_Boxes();
			Setup_Attributes_Ob();
			break;
			
	case 11:	Roll_Left();
			Setup_Current_Ob();
			Fill_Boxes();
			Setup_Attributes_Ob();
			break;
			
	case 12:	if (!change) break; 	/* User hit return but
						   change was disabled */
			Save_Current_Ob();	/* Write out */
			Setup_Current_Ob();
			Fill_Boxes();
			Setup_Attributes_Ob();
			break;
	
	case 13:	Create_Current_Ob();
			Setup_Current_Ob();
			Fill_Boxes();
			Setup_Attributes_Ob();
			break;
		
	case 14:	postaction = 1;
			return;
			
	case 15:	postaction = 10;	break;
	case 16:	postaction = 11;	break;
	case 17:	postaction = 12;	break;
	case 18:	postaction = 13;	break;
	case 19:	postaction = 14;	break;
			
	case 20:	if (what_mode!=LINE_OBJECT) CHANGE_OFF;
			what_mode = LINE_OBJECT;
			Fill_Boxes();
			break;
			
	case 21:	if (what_mode!=ELLIPSE_OBJECT) CHANGE_OFF;
			what_mode = ELLIPSE_OBJECT;
			Fill_Boxes();
			break;
			
	case 22:	if (what_mode!=TEXT_OBJECT) CHANGE_OFF;
			what_mode = TEXT_OBJECT;
			Fill_Boxes();
			break;
			
	case 23:	if (what_mode!=FILL_OBJECT&&
			    what_mode!=HATCH_OBJECT) CHANGE_OFF;
			what_mode = FILL_OBJECT;
			Fill_Boxes();
			break;
			
	case 24:	if (what_mode!=FILL_OBJECT&&
			    what_mode!=HATCH_OBJECT) CHANGE_OFF;
			what_mode = HATCH_OBJECT;
			Fill_Boxes();
			break;
		
	}
}

VOID linemvalid(ingad)
struct Gadget *ingad;
{

	int gpos;
	STDFLT ans;
	BOOL err,ang;

	ang = FALSE;	
	switch(ingad->GadgetID) {
	case 14: ang=TRUE;
	case 10:
	case 11:
	case 12:
	case 13:
	case 15:
		gpos = RemoveGList(w,ingad,1);

		if (ang) ans = ParAng(&linedata[ingad->GadgetID-10][0],&err);
		else	 ans = ParLin(&linedata[ingad->GadgetID-10][0],&err);
		
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 10:	firstx = ans;	setal(); break;
			case 11:	firsty = ans;	setal(); break;
			case 12:	secx = ans;	setal(); break;
			case 13:	secy = ans;	setal(); break;
			case 14:	lang = ans;	setxy(); break;
			case 15:	llen = ans;	setxy(); break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		sprintf(&linedata[2][0],"%s",DispLin(secx));
		sprintf(&linedata[3][0],"%s",DispLin(secy));
		sprintf(&linedata[4][0],"%s",DispAng(lang));
		sprintf(&linedata[5][0],"%s",DispLin(llen));
		RefreshGList(&line_mode[0],w,&ObjectRequester,6);

	}
}

VOID ellipmvalid(ingad)
struct Gadget *ingad;
{

	int gpos;
	STDFLT ans;
	BOOL err;

	switch(ingad->GadgetID) {
	case 10:
	case 11:
	case 12:
	case 13:
		gpos = RemoveGList(w,ingad,1);

		ans = ParLin(&ellipdata[ingad->GadgetID-10][0],&err);
		
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 10:	firstx = ans;	break;
			case 11:	firsty = ans;	break;
			case 12:	radx = ans;	break;
			case 13:	rady = ans;	break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		RefreshGList(ingad,w,&ObjectRequester,1);
		return;
	}
	
	switch(ingad->GadgetID) {
	case 14:
	case 15:
	case 16:
		gpos = RemoveGList(w,ingad,1);

		ans = ParAng(&ellipdata[ingad->GadgetID-10][0],&err);
		
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 14:	angb = ans;	break;
			case 15:	ange = ans;	break;
			case 16:	rot = ans;	break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		RefreshGList(ingad,w,&ObjectRequester,1);
		return;
	}
}

VOID textmvalid(ingad)
struct Gadget *ingad;
{

	int gpos;
	STDFLT ans;
	BOOL err;

	switch(ingad->GadgetID) {
	case 10:
	case 11:
		gpos = RemoveGList(w,ingad,1);

		ans = ParLin(&textdata[ingad->GadgetID-10][0],&err);
		
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 10:	firstx = ans;	break;
			case 11:	firsty = ans;	break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		RefreshGList(ingad,w,&ObjectRequester,1);
		return;
	}
	switch(ingad->GadgetID) {
	case 12:
		gpos = RemoveGList(w,ingad,1);

		ans = ParAng(&textdata[ingad->GadgetID-10][0],&err);
		
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 12:	rot = ans;	break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		RefreshGList(ingad,w,&ObjectRequester,1);
		return;
	}
	
	switch(ingad->GadgetID) {
	
	case 14:
		TURN_ON(text_mode[4]);
		TURN_OFF(text_mode[5]);
		TURN_OFF(text_mode[6]);
		RefreshGList(&text_mode[4],w,&ObjectRequester,3);
		texhan = LEFT_HANDLE;
		break;
	case 15:
		TURN_OFF(text_mode[4]);
		TURN_ON(text_mode[5]);
		TURN_OFF(text_mode[6]);
		RefreshGList(&text_mode[4],w,&ObjectRequester,3);
		texhan = CENTER_HANDLE;
		break;
	case 16:
		TURN_OFF(text_mode[4]);
		TURN_OFF(text_mode[5]);
		TURN_ON(text_mode[6]);
		RefreshGList(&text_mode[4],w,&ObjectRequester,3);
		texhan = RIGHT_HANDLE;
		break;
	}
	
}

VOID hatchmvalid(ingad)
struct Gadget *ingad;
{
	int gpos,i;
	STDFLT ans;
	BOOL err;

	switch(ingad->GadgetID) {
	case 10:
		gpos = RemoveGList(w,ingad,1);
		i = sscanf(&hatchdata[0][0],"%f",&ans);
		err = (i==0)? TRUE:FALSE;
		break;
	case 11:
		gpos = RemoveGList(w,ingad,1);
		ans = ParAng(&hatchdata[1][0],&err);
		break;
	}		
	
	switch(ingad->GadgetID) {
	case 10:
	case 11:
		if (err)
			DisplayBeep(screen);

		else {
			switch(ingad->GadgetID) {
			case 10:	pscale = ans;	break;
			case 11:	rot = ans;	break;
			}
		}
		
		Fill_Boxes();		
		if (gpos>=0) AddGList(w,ingad,gpos,1,&ObjectRequester);
		RefreshGList(ingad,w,&ObjectRequester,1);

	}

}

Setup_Current_Ob()
{

	what_mode = NO_OBJECT;

	firstx = firsty = 0.0;
	
	secx = secy = 0.0;
	lang = llen = 0.0;
	radx = rady = angb = ange = 0.0;
	rot = 0.0;
	pscale = 1.0;
	lineflg=ellipflag=textflg=0;
	texhan=LEFT_HANDLE;

	ob_style = Current_Style;
	ob_pattern = Current_Hatch;
	ob_group = Current_Group;
	ob_pen = Current_Pen;
	ob_layer = Current_Layer;

	if (CurrentRoller==NULL) Roll_Right();
	if (CurrentRoller==NULL) return;
	
	CHANGE_ON;
	
	ob_layer = CurrentRoller->node_layer;
	ob_pen = CurrentRoller->pen_attach;
	ob_group = CurrentRoller->above_group;

	switch (CurrentRoller->node_id) {
	case NODE_LINE:
		what_mode = LINE_OBJECT;
		ob_line = GetLineData(CurrentRoller);
		firstx = ob_line->x1;
		firsty = ob_line->y1;
		secx = ob_line->x2;
		secy = ob_line->y2;
		setal();
		lineflg = ob_line->line_flags;
		break;
	
	case NODE_ELLIP:
		what_mode = ELLIPSE_OBJECT;
		ob_ellip = GetEllipData(CurrentRoller);
		firstx = ob_ellip->x1;
		firsty = ob_ellip->y1;
		radx = ob_ellip->xradius;
		rady = ob_ellip->yradius;
		angb = ob_ellip->fromang;
		ange = ob_ellip->toangle;
		rot = ob_ellip->rotation;
		break;
		
	case NODE_TEXT:
		what_mode = TEXT_OBJECT;
		ob_text = GetTextData(CurrentRoller);
		firstx = ob_text->x1;
		firsty = ob_text->y1;
		textflg = ob_text->Text_Bits;
		texhan = ob_text->Handle_Flag;
		strcpy(textbuf,ob_text->text);
		ob_style = ob_text->letstyle;
		rot = ob_text->ang * PI/180.0;
		break;
	
	case NODE_HATCH:
		what_mode = FILL_OBJECT;
		ob_hatch = GetHatchData(CurrentRoller);
		if (ob_hatch->hatch_type==HATCH_PATTERN) {
			what_mode = HATCH_OBJECT;
			ob_pattern = ob_hatch->hatch_pattern;
		}
		pscale = ob_hatch->pattern_scale;
		rot = ob_hatch->pattern_rot;
		if (ob_hatch->hatch_pattern!=NULL) 
			ob_pattern = ob_hatch->hatch_pattern;
		break;
	}
}

Setup_Attributes_Ob()
{
	UBYTE centerbuf[55],*dbuf,*incbuf;
	int numspa;
	/* Setup the attribute buttons */

	if (ob_layer!=NULL) dbuf = ob_layer->Layer_Name;
	else	dbuf = "[No Layer Selected]";
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(&OBText[0][0],"%-30.30s",centerbuf);
	
	if (ob_pen!=NULL) dbuf = ob_pen->Pen_Name;
	else	dbuf = "[No Pen Selected]";
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(&OBText[1][0],"%-30.30s",centerbuf);
	
	if (ob_group!=NULL) dbuf = ob_group->Group_Name;
	else	dbuf = "[No Group Selected]";
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(&OBText[2][0],"%-30.30s",centerbuf);
	
	if (ob_style!=NULL) dbuf = ob_style->Letter_Style_ID;
	else	dbuf = "[No Style Selected]";
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(&TXText[0],"%-30.30s",centerbuf);
	
	if (ob_pattern!=NULL) dbuf = ob_pattern->Hatch_Name;
	else	dbuf = "[No Pattern Selected]";
	numspa = (30 - strlen(dbuf))/2;
	for (incbuf = centerbuf; numspa>=0; numspa--,incbuf++) 
				*incbuf=' ';
	sprintf(incbuf,"%-30.30s",dbuf);
	sprintf(&HTText[0],"%-30.30s",centerbuf);
	
}	
		
Object_Editor()
{

	struct RastPort *reqrp;
	int numgads;
	GROUP_A *bgroup,*tgroup;
	LAYER_A *blayer,*tlayer;
	PEN_A *bpen,*tpen;
	STYLE_A *bstyle,*tstyle;
	T_HATCH *tpatt;
		
	Setup_Current_Ob();
	Fill_Boxes();
	Setup_Attributes_Ob();
	
	exflag = TRUE;
	while (exflag) {

		/* Clean up gadget link lists (messed up by AddGList) */
	
		main_gads[12].NextGadget = NULL;
	
		InitRequester(&ObjectRequester);

		ObjectRequester.Width=REQWID;
		ObjectRequester.Height=REQHIT;
		ObjectRequester.LeftEdge=(XSsize-ObjectRequester.Width)/2;
		ObjectRequester.TopEdge=(YSsize-ObjectRequester.Height)/2;

		ObjectRequester.BackFill=1;

		ObjectRequester.ReqText = &ObjectHeadings[0];
		ObjectRequester.ReqGadget = &main_gads[0];

#ifdef _TURNSY
		ObjectRequester.Flags = NOISYREQ;
#endif
			
		SafeRequest(&ObjectRequester, w);

		postaction = 0;

		reqrp = ObjectRequester.ReqLayer->rp;	/* Nasty */

		DoBorders(&ObjectRequester,15,19,42);

		numgads = 13;

		switch(what_mode) {
		case LINE_OBJECT:

			SetAPen(reqrp, 3);	/* Outline arrow box */
			SetBPen(reqrp, 3);
			SetOPen(reqrp, 2);
			RectFill(reqrp, 8,55+35, 
				ObjectRequester.Width-8,55+72); 

			PrintIText(reqrp,&LineHeads[0],0,0);

			AddGList(w,&line_mode[0],-1,8,&ObjectRequester);
		
			numgads += 8;
			
			break;
			
		case ELLIPSE_OBJECT:
		
			PrintIText(reqrp,&EllipHeads[0],0,0);

			AddGList(w,&ellip_mode[0],-1,7,&ObjectRequester);
		
			numgads += 7;
			
			break;
			
		case TEXT_OBJECT:
		
			SetAPen(reqrp, 3);	/* Outline box */
			SetBPen(reqrp, 3);
			SetOPen(reqrp, 2);
			RectFill(reqrp, 8,55+35, 
				ObjectRequester.Width-8,55+72); 

			PrintIText(reqrp,&TextHeads[0],0,0);

			AddGList(w,&text_mode[0],-1,11,&ObjectRequester);
		
			numgads += 11;
			
			break;
		
		case FILL_OBJECT:
			PrintIText(reqrp,&FillHeads[0],0,0);

			break;
		
		case HATCH_OBJECT:
			PrintIText(reqrp,&HatchHeads[0],0,0);

			AddGList(w,&hatch_mode[0],-1,3,&ObjectRequester);
		
			numgads += 3;
			
			break;
		}
		
		RefreshGList(&main_gads[0],w,&ObjectRequester,numgads);
		
		sit_n_spin(&ObjectRequester,0,&main_gads[4],&main_gads[2]);
			/* Wait for requesters to exit */

		switch (postaction) {
		case 1:
			exflag = FALSE;
			break;
		case 10:
			if (!Layer_NewCurrent()) break;
			tlayer = (LAYER_A *) scrollanswer;
			if (tlayer!=NULL) ob_layer = tlayer;
			Setup_Attributes_Ob();
			break;
		
		case 11:
			if (!Pen_NewCurrent()) break;
			tpen = (PEN_A *) scrollanswer;
			if (tpen!=NULL) ob_pen = tpen;
			Setup_Attributes_Ob();
			break;
		
		case 12:
			bgroup = Current_Group;
			if (!Group_GetCurrent()) {
				Current_Group = bgroup;
				setpath();
				break;
			}
			tgroup = Current_Group;
			if (tgroup!=NULL) ob_group = tgroup;
			Current_Group = bgroup;
			setpath();
			Setup_Attributes_Ob();
			break;
		case 13:
			if (!Style_NewCurrent()) break;
			tstyle = (STYLE_A *) scrollanswer;
			if (tstyle!=NULL) ob_style = tstyle;
			Setup_Attributes_Ob();
			break;
		case 14:
			if (!Hatch_Get()) break;
			if (scrollanswer!=NULL) 
				ob_pattern = (T_HATCH *) scrollanswer;
			Setup_Attributes_Ob();
			break;
			
		}
		
				
	}

}
