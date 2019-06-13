*	UltraCAD Plotter Driver Interface 

	SECTION		plotter,CODE

*------ Included Files -----------------------------------------------

	INCLUDE		"exec/types.i"
	INCLUDE		"exec/nodes.i"
	INCLUDE		"exec/strings.i"

*------ Imported Files -----------------------------------------------

	XREF	_pref_initdriver
	XREF	_pref_dopreset
	XREF	_pref_initplot
	XREF	_pref_stepaction
	XREF	_pref_movetopoint
	XREF	_pref_drawaline
	XREF	_pref_setpenvelocity
	XREF	_pref_setpenaccel
	XREF	_pref_drawellipse
	XREF	_pref_setuppen
	XREF	_pref_endplot
	XREF	_pref_expungedriver
	XREF	_pref_beginpass

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
		DC.L	_pref_initdriver
		DC.L	_pref_dopreset
		DC.L	_pref_initplot
		DC.L	_pref_stepaction
		DC.L	_pref_movetopoint
		DC.L	_pref_drawaline
		DC.L	_pref_setpenvelocity
		DC.L	_pref_setpenaccel
		DC.L	0		; Not implemented _pref_drawellipse
		DC.L	_pref_setuppen
		DC.L	_pref_endplot
		DC.L	_pref_expungedriver
		DC.L	_pref_beginpass

	SECTION __MERGED,DATA

	xdef __FPERR
	xdef __SIGFPE
	xdef __ECS

__FPERR		dc.l	0
__SIGFPE	dc.l	0
__ECS		dc.l	0

		END
