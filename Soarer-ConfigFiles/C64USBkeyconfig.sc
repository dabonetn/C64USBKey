# Matrix setup for COMMODORE 64

matrix
	scanrate 1
	debounce 5
	blocking 1

	sense		PB6			PB2			PB3			PF4			PF7		PF6			PF5			PB1			PD3			PD2
	strobe	PB5	BACKSPACE	ENTER		RIGHT		F7			F1		F3			F5			DOWN		UNASSIGNED	UNASSIGNED
	strobe	PD0	3			W			A			4			Z		S			E			LSHIFT		PAD_2		PAD_ASTERIX
	strobe	PD4	5			R			D			6			C		F			T			X			PAD_4		PAD_MINUS
	strobe	PC6	7			Y			G			8			B		H			U			V			PAD_6		PAD_PLUS
	strobe	PD7	9			I			J			0			M		K			O			N			PAD_5		SCROLL_LOCK
	strobe	PE6	MINUS		P			L			EQUAL		PERIOD	SEMICOLON	LEFT_BRACE	COMMA		PAD_3		PAD_1
	strobe	PB4	INSERT		RIGHT_BRACE	QUOTE		HOME		RSHIFT	BACKSLASH	DELETE		SLASH		F10			UNASSIGNED
	strobe	PD1	1			ESC			TAB			2			SPACE	LCTRL		Q			LALT		PAD_8		PAD_SLASH
end


macroblock

#Left Shift Keys....

	macro DOWN LSHIFT # Left Shift + Down = Up
		CLEAR_META LSHIFT
		MAKE UP
		onbreak
		BREAK UP
	endmacro
	
	macro RIGHT LSHIFT # Left Shift + Right = Left
		CLEAR_META LSHIFT
		MAKE LEFT
		onbreak
		BREAK LEFT
	endmacro
	
	macro F1 LSHIFT # Left Shift + F1 = F2
		CLEAR_META LSHIFT
		MAKE F2
		onbreak
		BREAK F2
	endmacro
	
	macro F3 LSHIFT # Left Shift + F3 = F4
		CLEAR_META LSHIFT
		MAKE F4
		onbreak
		BREAK F4
	endmacro
	
	macro F5 LSHIFT # Left Shift + F5 = F6
		CLEAR_META LSHIFT
		MAKE F6
		onbreak
		BREAK F6
	endmacro
	
	macro F7 LSHIFT # Left Shift + F7 = F8
		CLEAR_META LSHIFT
		MAKE F8
		onbreak
		BREAK F8
	endmacro
	
	macro F10 LSHIFT # Left Shift + Restore = F12
		CLEAR_META LSHIFT
		MAKE F12
		onbreak
		BREAK F12
	endmacro
	
# ALT Keys (Runstop Key)
	
	
	macro F1 LALT # Left Shift + F7 = F8
		BREAK LALT
		MAKE F11
		onbreak
		BREAK F11
		endmacro
	
	macro F3 LALT # Left Shift + F7 = F8
		BREAK LALT
		MAKE BACK_QUOTE
		onbreak
		BREAK BACK_QUOTE
	endmacro
	
	macro F5 LALT # Left Shift + F7 = F8
		BREAK LALT
		MAKE PAGE_UP
		onbreak
		BREAK PAGE_UP
	endmacro
	
	macro F7 LALT # Left Shift + F7 = F8
		BREAK LALT
		MAKE PAGE_DOWN
		onbreak
		BREAK PAGE_DOWN
	endmacro
	
	macro F10 LALT # Left Shift + F7 = F8
		BREAK LALT
		MAKE F9
		onbreak
		BREAK F9
	endmacro
	
endblock