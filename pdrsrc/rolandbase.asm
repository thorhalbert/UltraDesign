*	UltraCAD Plotter Driver Interface 

	SECTION		"plotter",CODE

*------ Included Files -----------------------------------------------

	INCLUDE		"exec/types.i"
	INCLUDE		"exec/nodes.i"
	INCLUDE		"exec/strings.i"

*------ Imported Files -----------------------------------------------

	XREF	_rol_initdriver
	XREF	_rol_dopreset
	XREF	_rol_initplot
	XREF	_rol_stepaction
	XREF	_rol_movetopoint
	XREF	_rol_drawaline
	XREF	_rol_setpenvelocity
	XREF	_rol_setpenaccel
	XREF	_rol_drawellipse
	XREF	_rol_setuppen
	XREF	_rol_endplot
	XREF	_rol_expungedriver
	XREF	_rol_beginpass

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
		DC.L	_rol_initdriver
		DC.L	_rol_dopreset
		DC.L	_rol_initplot
		DC.L	_rol_stepaction
		DC.L	_rol_movetopoint
		DC.L	_rol_drawaline
		DC.L	_rol_setpenvelocity
		DC.L	_rol_setpenaccel
		DC.L	0		; not implemented _rol_drawellipse
		DC.L	_rol_setuppen
		DC.L	_rol_endplot
		DC.L	_rol_expungedriver
		DC.L	_rol_beginpass

	SECTION __MERGED,DATA

	xdef __FPERR
	xdef __SIGFPE
	xdef __ECS

__FPERR		dc.l	0
__SIGFPE	dc.l	0
__ECS		dc.l	0

		END
