*	UltraCAD Plotter Driver Interface 

	SECTION		plotter,CODE

*------ Included Files -----------------------------------------------

	INCLUDE		"exec/types.i"
	INCLUDE		"exec/nodes.i"
	INCLUDE		"exec/strings.i"

*------ Imported Files -----------------------------------------------

	XREF	_ps_initdriver
	XREF	_ps_dopreset
	XREF	_ps_initplot
	XREF	_ps_stepaction
	XREF	_ps_movetopoint
	XREF	_ps_drawaline
	XREF	_ps_setpenvelocity
	XREF	_ps_setpenaccel
	XREF	_ps_drawellipse
	XREF	_ps_setuppen
	XREF	_ps_endplot
	XREF	_ps_expungedriver
	XREF	_ps_beginpass

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
		DC.L	_ps_initdriver
		DC.L	_ps_dopreset
		DC.L	_ps_initplot
		DC.L	_ps_stepaction
		DC.L	_ps_movetopoint
		DC.L	_ps_drawaline
		DC.L	_ps_setpenvelocity
		DC.L	_ps_setpenaccel
		DC.L	0		; Not implemented _ps_drawellipse
		DC.L	_ps_setuppen
		DC.L	_ps_endplot
		DC.L	_ps_expungedriver
		DC.L	_ps_beginpass

	SECTION __MERGED,DATA

	xdef __FPERR
	xdef __SIGFPE
	xdef __ECS

__FPERR		dc.l	0
__SIGFPE	dc.l	0
__ECS		dc.l	0

		END
