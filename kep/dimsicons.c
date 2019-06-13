#include "ucad.h"

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_1u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7EFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xF8F1, 
	0x7FFF, 	0xF071, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xF071, 
	0x7FFF, 	0xF8F1, 
	0x7FFF, 	0xFDF1, 
	0x7FF0, 	0x31, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x7EFF, 	0xFDF7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x100, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_1u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_1u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_1s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x7EFF, 	0xFDF1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x1, 
	0x100, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7EFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xF8F7, 
	0x7FFF, 	0xF077, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xF077, 
	0x7FFF, 	0xF8F7, 
	0x7FFF, 	0xFDF7, 
	0x7FF0, 	0x37, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_1s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_1s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_2u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7EFF, 	0xFDF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFAF1, 
	0x7FFF, 	0xF271, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xF271, 
	0x7FFF, 	0xFAF1, 
	0x7FFF, 	0xFFF1, 
	0x7FF0, 	0x31, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x7EFF, 	0xFDF7, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x7, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x207, 
	0x0, 	0x7, 

};

struct Image dims_2u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_2u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_2s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x7EFF, 	0xFDF1, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x1, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x201, 
	0x0, 	0x203, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7EFF, 	0xFDF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFAF7, 
	0x7FFF, 	0xF277, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xF277, 
	0x7FFF, 	0xFAF7, 
	0x7FFF, 	0xFFF7, 
	0x7FF0, 	0x37, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_2s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_2s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_3u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7E08, 	0x31, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xF8F1, 
	0x7FFF, 	0xF071, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xFDF1, 
	0x7FFF, 	0xF071, 
	0x7FFF, 	0xF8F1, 
	0x7FFF, 	0xFDF1, 
	0x7FF7, 	0xFDF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x7FF7, 	0xFDF7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x8, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_3u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_3u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_3s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x7FF7, 	0xFDF1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x1, 
	0x8, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7E08, 	0x37, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xF8F7, 
	0x7FFF, 	0xF077, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xFDF7, 
	0x7FFF, 	0xF077, 
	0x7FFF, 	0xF8F7, 
	0x7FFF, 	0xFDF7, 
	0x7FF7, 	0xFDF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_3s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_3s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_4u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7F82, 	0xFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFA, 	0xFFF1, 
	0x7FF2, 	0x7FF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7E02, 	0x7F1, 
	0x7EFF, 	0xF7F1, 
	0x7FFF, 	0xFFF1, 
	0x7EFF, 	0xF7F1, 
	0x7E02, 	0x7F1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FF2, 	0x7FF1, 
	0x7FFA, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7F82, 	0xFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x2, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_4u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_4u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_4s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x1, 
	0x2, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7F82, 	0xFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFA, 	0xFFF7, 
	0x7FF2, 	0x7FF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7E02, 	0x7F7, 
	0x7EFF, 	0xF7F7, 
	0x7FFF, 	0xFFF7, 
	0x7EFF, 	0xF7F7, 
	0x7E02, 	0x7F7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FF2, 	0x7FF7, 
	0x7FFA, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7F82, 	0xFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_4s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_4s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_5u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FF8, 	0x811, 
	0x7FFB, 	0xFFD1, 
	0x7FFF, 	0xFFF1, 
	0x7FCB, 	0xFFD1, 
	0x7F38, 	0x811, 
	0x7F7F, 	0xFFF1, 
	0x7C1F, 	0xFFF1, 
	0x7E3F, 	0xFFF1, 
	0x7F7F, 	0xFFF1, 
	0x7007, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7007, 	0xFFF1, 
	0x7F7F, 	0xFFF1, 
	0x7E3F, 	0xFFF1, 
	0x7C1F, 	0xFFF1, 
	0x7F7F, 	0xFFF1, 
	0x7F7F, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x807, 
	0x0, 	0x7, 

};

struct Image dims_5u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_5u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_5s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x7FFF, 	0xFFF1, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x801, 
	0x0, 	0x803, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FF8, 	0x817, 
	0x7FFB, 	0xFFD7, 
	0x7FFF, 	0xFFF7, 
	0x7FCB, 	0xFFD7, 
	0x7F38, 	0x817, 
	0x7F7F, 	0xFFF7, 
	0x7C1F, 	0xFFF7, 
	0x7E3F, 	0xFFF7, 
	0x7F7F, 	0xFFF7, 
	0x7007, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7007, 	0xFFF7, 
	0x7F7F, 	0xFFF7, 
	0x7E3F, 	0xFFF7, 
	0x7C1F, 	0xFFF7, 
	0x7F7F, 	0xFFF7, 
	0x7F7F, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_5s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_5s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_6u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7C00, 	0xF1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xE3F1, 
	0x7FFF, 	0xC1F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xC1F1, 
	0x7FFF, 	0xE3F1, 
	0x7FFF, 	0xF7F1, 
	0x7FFF, 	0xF7F1, 
	0x7C00, 	0xF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_6u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_6u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_6s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7C00, 	0xF7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xE3F7, 
	0x7FFF, 	0xC1F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xC1F7, 
	0x7FFF, 	0xE3F7, 
	0x7FFF, 	0xF7F7, 
	0x7FFF, 	0xF7F7, 
	0x7C00, 	0xF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_6s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_6s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_7u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7EDF, 	0xEDF1, 
	0x7E9F, 	0xE5F1, 
	0x7E00, 	0x1F1, 
	0x7E9F, 	0xE5F1, 
	0x7EDF, 	0xEDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7EFF, 	0xFDF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_7u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_7u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_7s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7EDF, 	0xEDF7, 
	0x7E9F, 	0xE5F7, 
	0x7E00, 	0x1F7, 
	0x7E9F, 	0xE5F7, 
	0x7EDF, 	0xEDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7EFF, 	0xFDF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_7s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_7s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_8u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x60E0, 	0x2031, 
	0x7273, 	0x3331, 
	0x7333, 	0xF3F1, 
	0x7330, 	0xF0F1, 
	0x7333, 	0xF3F1, 
	0x7273, 	0x33F1, 
	0x60E0, 	0x21F1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_8u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_8u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_8s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x60E0, 	0x2037, 
	0x7273, 	0x3337, 
	0x7333, 	0xF3F7, 
	0x7330, 	0xF0F7, 
	0x7333, 	0xF3F7, 
	0x7273, 	0x33F7, 
	0x60E0, 	0x21F7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_8s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_8s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_9u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0x9FF1, 
	0x7FFF, 	0x1FF1, 
	0x7FFF, 	0x1FF1, 
	0x7FFE, 	0x1FF1, 
	0x7FFE, 	0x1FF1, 
	0x7FFD, 	0x9FF1, 
	0x7FFC, 	0xFFF1, 
	0x7FFB, 	0x3FF1, 
	0x7FF9, 	0xFFF1, 
	0x7FF6, 	0x7FF1, 
	0x7FF3, 	0xFFF1, 
	0x7FEC, 	0xFFF1, 
	0x7FE3, 	0xFFF1, 
	0x7FDF, 	0xFFF1, 
	0x7FDF, 	0xFFF1, 
	0x7FBF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x2007, 
	0x0, 	0x7, 
	0x0, 	0x6007, 
	0x0, 	0x8007, 
	0x0, 	0x6007, 
	0x1, 	0x8007, 
	0x0, 	0xC007, 
	0x3, 	0x7, 
	0x1, 	0x8007, 
	0x6, 	0x7, 
	0x3, 	0x7, 
	0xC, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dims_9u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_9u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD dims_9s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x2001, 
	0x0, 	0x1, 
	0x0, 	0x6001, 
	0x0, 	0x8001, 
	0x0, 	0x6001, 
	0x1, 	0x8001, 
	0x0, 	0xC001, 
	0x3, 	0x1, 
	0x1, 	0x8001, 
	0x6, 	0x1, 
	0x3, 	0x1, 
	0xC, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xBFF7, 
	0x7FFF, 	0x1FF7, 
	0x7FFF, 	0x7FF7, 
	0x7FFE, 	0x9FF7, 
	0x7FFE, 	0x7FF7, 
	0x7FFC, 	0x1FF7, 
	0x7FFC, 	0x3FF7, 
	0x7FF8, 	0x3FF7, 
	0x7FF8, 	0x7FF7, 
	0x7FF0, 	0x7FF7, 
	0x7FF0, 	0xFFF7, 
	0x7FE0, 	0xFFF7, 
	0x7FE3, 	0xFFF7, 
	0x7FDF, 	0xFFF7, 
	0x7FDF, 	0xFFF7, 
	0x7FBF, 	0xFFF7, 
	0x7F7F, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image dims_9s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&dims_9s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 7 ------ */
UWORD dimsb_1u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x6000, 	0x31, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_1u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,7,2,		/* Width, Height, Depth */
	&dimsb_1u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 7 ------ */
UWORD dimsb_1s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_1s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,7,2,		/* Width, Height, Depth */
	&dimsb_1s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 13 ------ */
UWORD dimsb_2u_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FF8, 	0xFFF1, 
	0x7FF8, 	0xFFF1, 
	0x7FF0, 	0x7FF1, 
	0x7FF0, 	0x7FF1, 
	0x7FE0, 	0x3FF1, 
	0x7FE5, 	0x3FF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_2u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,13,2,		/* Width, Height, Depth */
	&dimsb_2u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 13 ------ */
UWORD dimsb_2s_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_2s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,13,2,		/* Width, Height, Depth */
	&dimsb_2s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 11 ------ */
UWORD dimsb_3u_imageData[] = {

/*------ plane # 0: --------*/

	0x6000, 	0x31, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6FFF, 	0xFFB1, 
	0x6000, 	0x31, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_3u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,11,2,		/* Width, Height, Depth */
	&dimsb_3u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 11 ------ */
UWORD dimsb_3s_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_3s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,11,2,		/* Width, Height, Depth */
	&dimsb_3s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 13 ------ */
UWORD dimsb_4u_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FE5, 	0x3FF1, 
	0x7FE0, 	0x3FF1, 
	0x7FF0, 	0x7FF1, 
	0x7FF0, 	0x7FF1, 
	0x7FF8, 	0xFFF1, 
	0x7FF8, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_4u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,13,2,		/* Width, Height, Depth */
	&dimsb_4u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 13 ------ */
UWORD dimsb_4s_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_4s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,13,2,		/* Width, Height, Depth */
	&dimsb_4s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 5 ------ */
UWORD dimsb_5u_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x6000, 	0x31, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_5u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,5,2,		/* Width, Height, Depth */
	&dimsb_5u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 5 ------ */
UWORD dimsb_5s_imageData[] = {

/*------ plane # 0: --------*/

	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image dimsb_5s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,5,2,		/* Width, Height, Depth */
	&dimsb_5s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};