
#ifndef _EMAILERKB_H
#define _EMAILERKB_H


/*
  EMailerKB.h - emKeyboard library

  Amstrad Emailer mailboard driver
  Complerte rewrite by MichaelMcE
  May 2023
  
*/


#define EM_CODE_store		0x70
#define EM_CODE_setup		0x7A
#define EM_CODE_services	0x46
#define EM_CODE_games		0x7C
#define EM_CODE_internet	0x77
#define EM_CODE_home		0x72
#define EM_CODE_office		0x69
#define EM_CODE_mobile		0x1A
#define EM_CODE_textmsg		0x2A
#define EM_CODE_email		0x1C
#define EM_CODE_fax			0x15

#define EM_CODE_stop		0x71
#define EM_CODE_1			0x74
#define EM_CODE_2			0x73
#define EM_CODE_3			0x6B
#define EM_CODE_4			0x22
#define EM_CODE_5			0x1B
#define EM_CODE_6			0x1D
#define EM_CODE_7			0x1E
#define EM_CODE_8			0x79
#define EM_CODE_9			0x7D
#define EM_CODE_0			0x75
#define EM_CODE_back		0x6C

#define EM_CODE_q			0x21
#define EM_CODE_w			0x23
#define EM_CODE_e			0x24
#define EM_CODE_r			0x26
#define EM_CODE_t			0x52
#define EM_CODE_y			0x5D
#define EM_CODE_u			0x0D
#define EM_CODE_i			0x0E
#define EM_CODE_o			0x32
#define EM_CODE_p			0x34

#define EM_CODE_enter		0x2C

#define EM_CODE_a			0x31
#define EM_CODE_s			0x33
#define EM_CODE_d			0x35
#define EM_CODE_f			0x36
#define EM_CODE_g			0x29
#define EM_CODE_h			0x5B
#define EM_CODE_j			0x03
#define EM_CODE_k			0x76
#define EM_CODE_l			0x3A
#define EM_CODE_AT			0x3B		/* @ */

#define EM_CODE_lshift		0x3C
#define EM_CODE_z			0x3D
#define EM_CODE_x			0x4E
#define EM_CODE_c			0x54
#define EM_CODE_v			0x0B
#define EM_CODE_b			0x05
#define EM_CODE_n			0x41
#define EM_CODE_m			0x42
#define EM_CODE_point		0x43		
#define EM_CODE_rshift		0x55

#define EM_CODE_calc		0x83
#define EM_CODE_addrbook	0x06
#define EM_CODE_symbol		0x49
#define EM_CODE_space		0x4B
#define EM_CODE_comma		0x44

#define EM_CODE_left 		0x16
#define EM_CODE_right		0x09
#define EM_CODE_up			0x3E
#define EM_CODE_down		0x2E


#define EM_STATE_KEY		0x01
#define EM_STATE_LSHIFT		0x02
#define EM_STATE_RSHIFT		0x04
#define EM_STATE_SYMBOL		0x08
#define EM_STATE_CALC		0x10
#define EM_STATE_SHIFT		(EM_STATE_LSHIFT|EM_STATE_RSHIFT)


#define KNAMES	  \
	"Store",	  \
	"Setup",	  \
	"Services",	  \
	"Games",	  \
	"Internet",	  \
	"Home",		  \
	"Office",	  \
	"Mobile",	  \
	"TextMsg",	  \
	"Email",	  \
	"Fax",		  \
	"Back",		  \
	"Enter",	  \
	"Stop",		  \
	"AddressBook",\
	"Left",		  \
	"Right",	  \
	"Up",		  \
	"Down",		  \
	"LeftShift",  \
	"RightShift", \
	"Symbol",	  \
	"Calculator"  \


#define EM_BASEKEY	1

enum _keys {
	EM_KEY_STORE = EM_BASEKEY,
	EM_KEY_SETUP,
	EM_KEY_SERVICES,
	EM_KEY_GAMES,
	EM_KEY_INTERNET,
	EM_KEY_HOME,
	EM_KEY_OFFICE,
	EM_KEY_MOBILE,
	EM_KEY_TEXTMSG,
	EM_KEY_EMAIL,
	EM_KEY_FAX,
	
	EM_KEY_FUNC_FIRST = EM_KEY_STORE,
	EM_KEY_FUNC_LAST = EM_KEY_FAX,
	
	
	EM_KEY_BACK  = 12,
	EM_KEY_ENTER = 13,
	EM_KEY_STOP,
	EM_KEY_ADDRBOOK,
	
	EM_KEY_LEFT,
	EM_KEY_RIGHT,
	EM_KEY_UP,
	EM_KEY_DOWN,

	EM_KEY_LSHIFT,
	EM_KEY_RSHIFT,
	EM_KEY_SYMBOL,
	EM_KEY_CALC,

	EM_KEY_SPACE = 32,
	EM_KEY_COMMA = 44,
	EM_KEY_POINT = 46,

	EM_KEY_0     = 48,
	EM_KEY_1,
	EM_KEY_2,
	EM_KEY_3,
	EM_KEY_4,
	EM_KEY_5,
	EM_KEY_6,
	EM_KEY_7,
	EM_KEY_8,
	EM_KEY_9,

	EM_KEY_AT = 64,			/* @ */

	EM_KEY_A  = 97,
	EM_KEY_B,
	EM_KEY_C,
	EM_KEY_D,
	EM_KEY_E,
	EM_KEY_F,
	EM_KEY_G,
	EM_KEY_H,
	EM_KEY_I,
	EM_KEY_J,
	EM_KEY_K,
	EM_KEY_L,
	EM_KEY_M,
	EM_KEY_N,
	EM_KEY_O,
	EM_KEY_P,
	EM_KEY_Q,
	EM_KEY_R,
	EM_KEY_S,
	EM_KEY_T,
	EM_KEY_U,
	EM_KEY_V,
	EM_KEY_W,
	EM_KEY_X,
	EM_KEY_Y,
	EM_KEY_Z,
	
	EM_KEY_TOTAL
};


enum _symbols {
	EM_SYM_0,
	EM_SYM_1,
	EM_SYM_2,
	EM_SYM_3,
	EM_SYM_4,
	EM_SYM_5,
	EM_SYM_6,
	EM_SYM_7,
	EM_SYM_8,
	EM_SYM_9,
	
	EM_SYM_R,
	EM_SYM_T,
	EM_SYM_U,
	
	EM_SYM_A,
	EM_SYM_G,
	EM_SYM_H,
	
	EM_SYM_AT,
	EM_SYM_POINT,
	EM_SYM_COMMA,
	
	EM_SYM_TOTAL
};

enum _shift {
	EM_SHIFT_0,
	EM_SHIFT_1,
	EM_SHIFT_2,
	EM_SHIFT_3,
	EM_SHIFT_4,
	EM_SHIFT_5,
	EM_SHIFT_6,
	EM_SHIFT_7,
	EM_SHIFT_8,
	EM_SHIFT_9,
	
	EM_SHIFT_AT,
	EM_SHIFT_POINT,
	EM_SHIFT_COMMA,

	EM_SHIFT_TOTAL
};

// symbol assigned to key ..
enum _symbolcode {
	EM_SYMCODE_1	 = '<',
	EM_SYMCODE_2	 = '>',
	EM_SYMCODE_3	 = 128,		// euro (works for me)
	EM_SYMCODE_4	 = '#',
	EM_SYMCODE_5	 = '&',
	EM_SYMCODE_6	 = '+',
	EM_SYMCODE_7	 = ']',
	EM_SYMCODE_8	 = '{',
	EM_SYMCODE_9	 = ')',
	EM_SYMCODE_0	 = '_',
	
	EM_SYMCODE_R	 = 176,
	EM_SYMCODE_T	 = 126,				
	EM_SYMCODE_U	 = 168, 	
	
	EM_SYMCODE_A	 = 146,				
	EM_SYMCODE_G	 = 145,				
	EM_SYMCODE_H	 = 136,
	
	EM_SYMCODE_AT	 = '\\',
	EM_SYMCODE_POINT = '/',
	EM_SYMCODE_COMMA = ':'
};


enum _shiftcode {
	EM_SHIFTCODE_1	= '!',
	EM_SHIFTCODE_2	= 34,
	EM_SHIFTCODE_3	= 163,		// UK Pound
	EM_SHIFTCODE_4	= '$',
	EM_SHIFTCODE_5	= '%',
	EM_SHIFTCODE_6	= '=',
	EM_SHIFTCODE_7	= '[',
	EM_SHIFTCODE_8	= '*',
	EM_SHIFTCODE_9	= '(',
	EM_SHIFTCODE_0	= '-',
	
	EM_SHIFTCODE_AT	   = 39,
	EM_SHIFTCODE_POINT = 63,
	EM_SHIFTCODE_COMMA = ';'
};



	
#endif
