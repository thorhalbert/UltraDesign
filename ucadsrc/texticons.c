#include "ucad.h"

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_1u_imageData[] = {

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
	0x7001, 	0x31, 
	0x77FF, 	0xFFB1, 
	0x77FF, 	0xFFB1, 
	0x77FF, 	0xFFB1, 
	0x77FF, 	0xFFB1, 
	0x77FF, 	0xFFB1, 
	0x77FF, 	0xFFB1, 
	0x7FFF, 	0xFFF9, 
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
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x7FFF, 	0xFFFF, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x1, 	0x7, 
	0x0, 	0x7, 

};

struct Image text_1u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_1u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_1s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x7FFF, 	0xFFF9, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x1, 
	0x1, 	0x3, 
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
	0x7001, 	0x37, 
	0x77FF, 	0xFFB7, 
	0x77FF, 	0xFFB7, 
	0x77FF, 	0xFFB7, 
	0x77FF, 	0xFFB7, 
	0x77FF, 	0xFFB7, 
	0x77FF, 	0xFFB7, 
	0x7FFF, 	0xFFFF, 
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

struct Image text_1s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_1s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_2u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xF3F1, 
	0x7FFF, 	0xBF1, 
	0x7FF8, 	0x7BF1, 
	0x7FC7, 	0xFDF1, 
	0x7E3F, 	0xFDF1, 
	0x61FF, 	0xFDF1, 
	0x77FF, 	0xFDF1, 
	0x7FFF, 	0xFFF1, 
	0x77FF, 	0x87F1, 
	0x7BFC, 	0x3FF1, 
	0x7BE3, 	0xFFF1, 
	0x7B1F, 	0xFFF1, 
	0x7CFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x107, 
	0x0, 	0x7, 

};

struct Image text_2u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_2u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_2s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x7FFF, 	0xFFF1, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x101, 
	0x0, 	0x103, 
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
	0x7FFF, 	0xF3F7, 
	0x7FFF, 	0xBF7, 
	0x7FF8, 	0x7BF7, 
	0x7FC7, 	0xFDF7, 
	0x7E3F, 	0xFDF7, 
	0x61FF, 	0xFDF7, 
	0x77FF, 	0xFDF7, 
	0x7FFF, 	0xFFF7, 
	0x77FF, 	0x87F7, 
	0x7BFC, 	0x3FF7, 
	0x7BE3, 	0xFFF7, 
	0x7B1F, 	0xFFF7, 
	0x7CFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_2s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_2s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_3u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7CE0, 	0x7871, 
	0x7873, 	0x3331, 
	0x7873, 	0x27F1, 
	0x7330, 	0x67F1, 
	0x7033, 	0x27F1, 
	0x6793, 	0x3331, 
	0x6780, 	0x7871, 
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

struct Image text_3u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_3u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_3s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x7CE0, 	0x7877, 
	0x7873, 	0x3337, 
	0x7873, 	0x27F7, 
	0x7330, 	0x67F7, 
	0x7033, 	0x27F7, 
	0x6793, 	0x3337, 
	0x6780, 	0x7877, 
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

struct Image text_3s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_3s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_4u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
	0x7DF7, 	0xFFF1, 
	0x7DEF, 	0xFFF1, 
	0x7DDF, 	0xFFF1, 
	0x7DBF, 	0xFFF1, 
	0x7D7F, 	0xFFF1, 
	0x7D00, 	0x71, 
	0x7D7F, 	0xFFF1, 
	0x7DBF, 	0xFFF1, 
	0x7DDF, 	0xFFF1, 
	0x7DEF, 	0xFFF1, 
	0x7DF7, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
	0x7DFF, 	0xFFF1, 
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

struct Image text_4u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_4u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_4s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x7DFF, 	0xFFF7, 
	0x7DFF, 	0xFFF7, 
	0x7DFF, 	0xFFF7, 
	0x7DF7, 	0xFFF7, 
	0x7DEF, 	0xFFF7, 
	0x7DDF, 	0xFFF7, 
	0x7DBF, 	0xFFF7, 
	0x7D7F, 	0xFFF7, 
	0x7D00, 	0x77, 
	0x7D7F, 	0xFFF7, 
	0x7DBF, 	0xFFF7, 
	0x7DDF, 	0xFFF7, 
	0x7DEF, 	0xFFF7, 
	0x7DF7, 	0xFFF7, 
	0x7DFF, 	0xFFF7, 
	0x7DFF, 	0xFFF7, 
	0x7DFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_4s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_4s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_5u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FDE, 	0xF7F1, 
	0x7FEE, 	0xEFF1, 
	0x7FF6, 	0xDFF1, 
	0x7FFA, 	0xBFF1, 
	0x7002, 	0x8031, 
	0x7FFA, 	0xBFF1, 
	0x7FF6, 	0xDFF1, 
	0x7FEE, 	0xEFF1, 
	0x7FDE, 	0xF7F1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
	0x7FFE, 	0xFFF1, 
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

struct Image text_5u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_5u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_5s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FDE, 	0xF7F7, 
	0x7FEE, 	0xEFF7, 
	0x7FF6, 	0xDFF7, 
	0x7FFA, 	0xBFF7, 
	0x7002, 	0x8037, 
	0x7FFA, 	0xBFF7, 
	0x7FF6, 	0xDFF7, 
	0x7FEE, 	0xEFF7, 
	0x7FDE, 	0xF7F7, 
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FFE, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_5s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_5s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_6u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFEF1, 
	0x7FFF, 	0xFEF1, 
	0x7FFF, 	0xFEF1, 
	0x7FFF, 	0xBEF1, 
	0x7FFF, 	0xDEF1, 
	0x7FFF, 	0xEEF1, 
	0x7FFF, 	0xF6F1, 
	0x7FFF, 	0xFAF1, 
	0x7800, 	0x2F1, 
	0x7FFF, 	0xFAF1, 
	0x7FFF, 	0xF6F1, 
	0x7FFF, 	0xEEF1, 
	0x7FFF, 	0xDEF1, 
	0x7FFF, 	0xBEF1, 
	0x7FFF, 	0xFEF1, 
	0x7FFF, 	0xFEF1, 
	0x7FFF, 	0xFEF1, 
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

struct Image text_6u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_6u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_6s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xBEF7, 
	0x7FFF, 	0xDEF7, 
	0x7FFF, 	0xEEF7, 
	0x7FFF, 	0xF6F7, 
	0x7FFF, 	0xFAF7, 
	0x7800, 	0x2F7, 
	0x7FFF, 	0xFAF7, 
	0x7FFF, 	0xF6F7, 
	0x7FFF, 	0xEEF7, 
	0x7FFF, 	0xDEF7, 
	0x7FFF, 	0xBEF7, 
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xFEF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_6s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_6s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_7u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x70C7, 	0x8831, 
	0x6DEF, 	0xDB71, 
	0x6DEF, 	0xDB71, 
	0x6DEF, 	0xDF71, 
	0x6DEF, 	0xDF71, 
	0x6DEF, 	0xDF71, 
	0x71EF, 	0xDC71, 
	0x7DEF, 	0xDF71, 
	0x7DEF, 	0xDF71, 
	0x7DEE, 	0xDF71, 
	0x7DEE, 	0xDF71, 
	0x7DEE, 	0xDF71, 
	0x78C6, 	0xE31, 
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

struct Image text_7u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_7u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_7s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x70C7, 	0x8837, 
	0x6DEF, 	0xDB77, 
	0x6DEF, 	0xDB77, 
	0x6DEF, 	0xDF77, 
	0x6DEF, 	0xDF77, 
	0x6DEF, 	0xDF77, 
	0x71EF, 	0xDC77, 
	0x7DEF, 	0xDF77, 
	0x7DEF, 	0xDF77, 
	0x7DEE, 	0xDF77, 
	0x7DEE, 	0xDF77, 
	0x7DEE, 	0xDF77, 
	0x78C6, 	0xE37, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_7s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_7s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_8u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7071, 	0x8C31, 
	0x76FB, 	0xDB71, 
	0x76FB, 	0xDB71, 
	0x7EFB, 	0xDB71, 
	0x7EFB, 	0xDB71, 
	0x7EFB, 	0xDB71, 
	0x78FB, 	0xDC71, 
	0x7EFB, 	0xDF71, 
	0x7EFB, 	0xDF71, 
	0x7EDB, 	0xDF71, 
	0x7EDB, 	0xDF71, 
	0x7EDB, 	0xDF71, 
	0x7C41, 	0x8E31, 
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

struct Image text_8u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_8u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_8s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x7071, 	0x8C37, 
	0x76FB, 	0xDB77, 
	0x76FB, 	0xDB77, 
	0x7EFB, 	0xDB77, 
	0x7EFB, 	0xDB77, 
	0x7EFB, 	0xDB77, 
	0x78FB, 	0xDC77, 
	0x7EFB, 	0xDF77, 
	0x7EFB, 	0xDF77, 
	0x7EDB, 	0xDF77, 
	0x7EDB, 	0xDF77, 
	0x7EDB, 	0xDF77, 
	0x7C41, 	0x8E37, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_8s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_8s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_9u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x6383, 	0x18F1, 
	0x77DB, 	0xBDF1, 
	0x77DB, 	0xBDF1, 
	0x77DB, 	0xBDF1, 
	0x77DF, 	0xBDF1, 
	0x77DF, 	0xBDF1, 
	0x71DF, 	0xBC71, 
	0x77DF, 	0xBDB1, 
	0x77DF, 	0xBDB1, 
	0x77DF, 	0xBDB1, 
	0x76DF, 	0xBDB1, 
	0x76DF, 	0xBDB1, 
	0x608F, 	0x1871, 
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

struct Image text_9u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_9u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_9s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
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
	0x6383, 	0x18F7, 
	0x77DB, 	0xBDF7, 
	0x77DB, 	0xBDF7, 
	0x77DB, 	0xBDF7, 
	0x77DF, 	0xBDF7, 
	0x77DF, 	0xBDF7, 
	0x71DF, 	0xBC77, 
	0x77DF, 	0xBDB7, 
	0x77DF, 	0xBDB7, 
	0x77DF, 	0xBDB7, 
	0x76DF, 	0xBDB7, 
	0x76DF, 	0xBDB7, 
	0x608F, 	0x1877, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_9s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_9s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_10u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x7FFF, 	0xFFF1, 
	0x7FFF, 	0xFFF1, 
	0x7FFD, 	0xFFF1, 
	0x7FF8, 	0xFFF1, 
	0x7FFB, 	0xFFF1, 
	0x7FF4, 	0xFFF1, 
	0x7FF2, 	0xFFF1, 
	0x7FE0, 	0xE7F1, 
	0x7FE1, 	0xE7F1, 
	0x7FC1, 	0xE7F1, 
	0x7FC3, 	0xC3F1, 
	0x7F83, 	0x81F1, 
	0x7F87, 	0xF1, 
	0x7F06, 	0x71, 
	0x7F1C, 	0x31, 
	0x7EFC, 	0x31, 
	0x7EFC, 	0x31, 
	0x7DFC, 	0x31, 
	0x7BFE, 	0x71, 
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
	0xC, 	0x7, 
	0x6, 	0x7, 
	0x18, 	0x7, 
	0xC, 	0x7, 
	0x30, 	0x7, 
	0x18, 	0x7, 
	0x60, 	0xFF07, 
	0x0, 	0x7, 
	0x1, 	0xFF87, 
	0x1, 	0xFF87, 
	0x0, 	0xFF07, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image text_10u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_10u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_10s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x8000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0xC, 	0x1, 
	0x6, 	0x1, 
	0x18, 	0x1, 
	0xC, 	0x1, 
	0x30, 	0x1, 
	0x18, 	0x1, 
	0x60, 	0xFF01, 
	0x0, 	0x1, 
	0x1, 	0xFF81, 
	0x1, 	0xFF81, 
	0x0, 	0xFF01, 
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
	0x7FFD, 	0xFFF7, 
	0x7FF8, 	0xFFF7, 
	0x7FFB, 	0xFFF7, 
	0x7FF4, 	0xFFF7, 
	0x7FF2, 	0xFFF7, 
	0x7FE0, 	0xE7F7, 
	0x7FE1, 	0xE7F7, 
	0x7FC1, 	0xE7F7, 
	0x7FC3, 	0xC3F7, 
	0x7F83, 	0x81F7, 
	0x7F87, 	0xF7, 
	0x7F06, 	0x77, 
	0x7F1C, 	0x37, 
	0x7EFC, 	0x37, 
	0x7EFC, 	0x37, 
	0x7DFC, 	0x37, 
	0x7BFE, 	0x77, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x7FFF, 	0xFFF7, 
	0x0, 	0x7, 

};

struct Image text_10s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_10s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_11u_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x1CDF, 	0xFFF1, 
	0x4E6F, 	0xFFF1, 
	0x6737, 	0xFFF1, 
	0x739B, 	0xFFF1, 
	0x79CD, 	0xFFF1, 
	0x7CE6, 	0xFFF1, 
	0x7E73, 	0x7FF1, 
	0x7F39, 	0xBFF1, 
	0x7F9C, 	0xDFF1, 
	0x7FCE, 	0xFF1, 
	0x7FE7, 	0x7F1, 
	0x7FF2, 	0x3F1, 
	0x7FF8, 	0x1F1, 
	0x7FF8, 	0xF1, 
	0x7FF0, 	0x1F1, 
	0x7FF8, 	0x3F1, 
	0x7FFE, 	0x7F1, 
	0x7FFF, 	0x7F1, 
	0x0, 	0x7F1, 
	0x7FFF, 	0xEBF1, 
	0x7FFF, 	0xD5F1, 
	0x7FFF, 	0xF6F1, 
	0x7FFF, 	0xF7F3, 
	0x0, 	0x7, 

/*------ plane # 1: --------*/

	0xFFFF, 	0xFFFF, 
	0xFFFF, 	0xFFFF, 
	0x0, 	0x7, 
	0x1CE0, 	0x7, 
	0x4E70, 	0x7, 
	0x6738, 	0x7, 
	0x739C, 	0x7, 
	0x39CE, 	0x7, 
	0x1CE7, 	0x7, 
	0xE73, 	0x8007, 
	0x739, 	0xC007, 
	0x39C, 	0xC007, 
	0x1CE, 	0x7, 
	0xE7, 	0x1807, 
	0x72, 	0x3C07, 
	0x38, 	0x7E07, 
	0x18, 	0xFF07, 
	0x1, 	0xFE07, 
	0x3, 	0xFC07, 
	0x1, 	0xF807, 
	0x0, 	0xF807, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 
	0x0, 	0x7, 

};

struct Image text_11u_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_11u_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};

/*----- bitmap : w = 32, h = 27 ------ */
UWORD text_11s_imageData[] = {

/*------ plane # 0: --------*/

	0xC000, 	0x1, 
	0x0, 	0x1, 
	0x0, 	0x1, 
	0x1CE0, 	0x1, 
	0x4E70, 	0x1, 
	0x6738, 	0x1, 
	0x739C, 	0x1, 
	0x39CE, 	0x1, 
	0x1CE7, 	0x1, 
	0xE73, 	0x8001, 
	0x739, 	0xC001, 
	0x39C, 	0xC001, 
	0x1CE, 	0x1, 
	0xE7, 	0x1801, 
	0x72, 	0x3C01, 
	0x38, 	0x7E01, 
	0x18, 	0xFF01, 
	0x1, 	0xFF01, 
	0x3, 	0xFE01, 
	0x3, 	0xFC01, 
	0x1, 	0xF801, 
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
	0x1CDF, 	0xFFF7, 
	0x4E6F, 	0xFFF7, 
	0x6737, 	0xFFF7, 
	0x739B, 	0xFFF7, 
	0x79CD, 	0xFFF7, 
	0x7CE6, 	0xFFF7, 
	0x7E73, 	0x7FF7, 
	0x7F39, 	0xBFF7, 
	0x7F9C, 	0xDFF7, 
	0x7FCE, 	0xFF7, 
	0x7FE7, 	0x7F7, 
	0x7FF2, 	0x3F7, 
	0x7FF8, 	0x1F7, 
	0x7FF8, 	0xF7, 
	0x7FF0, 	0xF7, 
	0x7FF8, 	0x1F7, 
	0x7FFC, 	0x3F7, 
	0x7FFE, 	0x7F7, 
	0x0, 	0x7F7, 
	0x7FFF, 	0xEBF7, 
	0x7FFF, 	0xD5F7, 
	0x7FFF, 	0xF6F7, 
	0x7FFF, 	0xF7F7, 
	0x0, 	0x7, 

};

struct Image text_11s_image = {
	0,0,		/* LeftEdge, TopEdge */
	31,27,2,		/* Width, Height, Depth */
	&text_11s_imageData[0],	/* Image Data */
	3,0,		/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
