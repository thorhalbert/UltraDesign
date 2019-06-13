*	UltraCAD Plotter Driver Interface 

	SECTION		"plotter",CODE

*------ Included Files -----------------------------------------------

	INCLUDE		"exec/types.i"
	INCLUDE		"exec/nodes.i"
	INCLUDE		"exec/strings.i"

*------ Imported Files -----------------------------------------------

	XREF	_dmpl_initdriver
	XREF	_dmpl_dopreset
	XREF	_dmpl_initplot
	XREF	_dmpl_stepaction
	XREF	_dmpl_movetopoint
	XREF	_dmpl_drawaline
	XREF	_dmpl_setpenvelocity
	XREF	_dmpl_setpenaccel
	XREF	_dmpl_drawellipse
	XREF	_dmpl_setuppen
	XREF	_dmpl_endplot
	XREF	_dmpl_expungedriver
	XREF	_dmpl_beginpass

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
		DC.L	_dmpl_initdriver
		DC.L	_dmpl_dopreset
		DC.L	_dmpl_initplot
		DC.L	_dmpl_stepaction
		DC.L	_dmpl_movetopoint
		DC.L	_dmpl_drawaline
		DC.L	_dmpl_setpenvelocity
		DC.L	_dmpl_setpenaccel
		DC.L	0		; not implemented _dmpl_drawellipse
		DC.L	_dmpl_setuppen
		DC.L	_dmpl_endplot
		DC.L	_dmpl_expungedriver
		DC.L	_dmpl_beginpass

	SECTION __MERGED,DATA

	xdef __FPERR
	xdef __SIGFPE
	xdef __ECS

__FPERR		dc.l	0
__SIGFPE	dc.l	0
__ECS		dc.l	0

		END
