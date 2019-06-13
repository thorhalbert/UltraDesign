#ifndef FONT_INFO
#define FONT_INFO
struct Stroke_Font {

	WORD	Font_Magic;	/* Magic number for stroke font data */
	WORD	Font_Version;	/* Version number of this font */

	STDFLT	Font_Width;	/* Width of characters */
	STDFLT	Font_Vertical;	/* Vertical multiplier for unit size */
	STDFLT	Font_Horiz;	/* Horizontal mult for unit */

	WORD	Char_Xtable[256]; /* Character access table */

	WORD	Num_Chars;	/* Number of words in stroke table */
	WORD	*Word_Table;	/* The stroke table */
	
	STDFLT	Char_Widths[256]; /* Standard width of each char */
};

struct Load_Font_20 {

	WORD	Font_Magic;	/* Magic number for stroke font data */
	WORD	Font_Version;	/* Version number of this font */

	PORTFLOAT	Font_Width;	/* Width of characters */
	PORTFLOAT	Font_Vertical;	/* Vertical multiplier for unit size */
	PORTFLOAT	Font_Horiz;	/* Horizontal mult for unit */

	WORD	Char_Xtable[256]; /* Character access table */

	WORD	Num_Chars;	/* Number of words in stroke table */
	WORD	*Word_Table;	/* The stroke table */
	
	PORTFLOAT	Char_Widths[256]; /* Standard width of each char */
};

#define CUR_STROKE_VER 20	/* Version 1.0 of the font data */
#define CUR_MAGIC_NUM 31751	/* Font magic number */
#endif
