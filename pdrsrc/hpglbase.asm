*	UltraCAD Plotter Driver Interface 

	SECTION		plotter,CODE

*------ Included Files -----------------------------------------------

	INCLUDE		"exec/types.i"
	INCLUDE		"exec/nodes.i"
	INCLUDE		"exec/strings.i"

*------ Imported Files -----------------------------------------------

	XREF	_hp_initdriver
	XREF	_hp_dopreset
	XREF	_hp_initplot
	XREF	_hp_stepaction
	XREF	_hp_movetopoint
	XREF	_hp_drawaline
	XREF	_hp_setpenvelocity
	XREF	_hp_setpenaccel
	XREF	_hp_drawellipse
	XREF	_hp_setuppen
	XREF	_hp_endplot
	XREF	_hp_expungedriver
	XREF	_hp_beginpass

*------ Exported Names -----------------------------------------------

	XDEF	_PLTData
	XDEF	.begin

**********************************************************************

.begin:
		MOVEQ	#0,D0		; show error for OpenLibrary()
		RTS
		DC.W	1		; VERSION
		DC.W	1		; REVISION
_PLTData:
		DC.L	0			; Custom Area NULL
		DC.W	0			; Custom Area Empty
		DC.W	14		; Total number of avail subroutines
		DC.L	_hp_initdriver
		DC.L	_hp_dopreset
		DC.L	_hp_initplot
		DC.L	_hp_stepaction
		DC.L	_hp_movetopoint
		DC.L	_hp_drawaline
		DC.L	_hp_setpenvelocity
		DC.L	_hp_setpenaccel
		DC.L	0		; Not implemented _hp_drawellipse
		DC.L	_hp_setuppen
		DC.L	_hp_endplot
		DC.L	_hp_expungedriver
		DC.L	_hp_beginpass

	SECTION __MERGED,DATA

	xdef __FPERR
	xdef __SIGFPE
	xdef __ECS

__FPERR		dc.l	0
__SIGFPE	dc.l	0
__ECS		dc.l	0

		END
