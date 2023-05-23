/*
  emKeyboard.cpp - emKeyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  Amstrad Emailer mailboard driver
  Complerte rewrite from ps2keyboard 
  MichaelMcE
  May 2023
  
  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/emKeyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Version 2.4 (March 2013)
  - Support Teensy 3.0, Arduino Due, Arduino Leonardo & other boards
  - French keyboard layout, David Chochoi, tchoyyfr at yahoo dot fr

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "emKeyboard.h"
#include "EMailerKB.h"


static emKeyboard keyboard;

static volatile uint8_t inbuffer[IN_BUFFER_SIZE];
static volatile uint8_t head, tail;
static volatile int keyReady = 0;

static uint8_t DataPin;

// holds current state of that key
static uint8_t kstate[132];

// holds destination codepoint of that key
static uint8_t scode[132];

// hold the modifier codepoint tables
static uint8_t symbolsTable[EM_SYM_TOTAL];
static uint8_t shiftTable[EM_SHIFT_TOTAL];

const PROGMEM char *keyNames[] = {"",KNAMES};






uint8_t emKeyToShift (const uint8_t code)
{
	if (code >= EM_KEY_0 && code <= EM_KEY_9)
		return shiftTable[code-EM_KEY_0];
	
	else if (code >= EM_KEY_A && code <= EM_KEY_Z)
		return code-32;
		
	else if (code == EM_KEY_AT)
		return shiftTable[EM_SHIFT_AT];
	else if (code == EM_KEY_POINT)
		return shiftTable[EM_SHIFT_POINT];
	else if (code == EM_KEY_COMMA)
		return shiftTable[EM_SHIFT_COMMA];

	return 0;
}

uint8_t emKeyToSymbol (const uint8_t code)
{
	if (code >= EM_KEY_0 && code <= EM_KEY_9)
		return symbolsTable[code-EM_KEY_0];
		
	else if (code == EM_KEY_R)
		return symbolsTable[EM_SYM_R];
	else if (code == EM_KEY_T)
		return symbolsTable[EM_SYM_T];
	else if (code == EM_KEY_U)
		return symbolsTable[EM_SYM_U];
		
	else if (code == EM_KEY_A)
		return symbolsTable[EM_SYM_A];
	else if (code == EM_KEY_G)
		return symbolsTable[EM_SYM_G];
	else if (code == EM_KEY_H)
		return symbolsTable[EM_SYM_H];

	else if (code == EM_KEY_AT)
		return symbolsTable[EM_SYM_AT];
	else if (code == EM_KEY_POINT)
		return symbolsTable[EM_SYM_POINT];
	else if (code == EM_KEY_COMMA)
		return symbolsTable[EM_SYM_COMMA];

	return 0;
}

const char *emKeyToName (const uint8_t code)
{
	if (code < sizeof(keyNames) / sizeof(keyNames[0]))
		return keyNames[code];

	return NULL;
}

static uint8_t getKeyState (const uint8_t code)
{
	return kstate[code]&0x01;
}

bool emIsFunctionKey (uint8_t key)
{
	return (key >= EM_KEY_FUNC_FIRST && key <= EM_KEY_FUNC_LAST);
}

uint8_t emGetModifiers ()
{
	uint8_t modifiersState = \
		(getKeyState(EM_CODE_lshift)<<1) | 
		(getKeyState(EM_CODE_rshift)<<2) | 
		(getKeyState(EM_CODE_symbol)<<3) | 
		(getKeyState(EM_CODE_calc)  <<4);
		
	return modifiersState;
}

uint32_t emGetKey (uint8_t *state)
{
	uint32_t c = emGetScancode();
	if (c){
		int key = emCodeToKey(c);
		if (key){
			if (state)
				*state = (getKeyState(c)&EM_STATE_KEY) | emGetModifiers();
			return key;
		}
	}
	return 0;
}

uint32_t emGetKeyModified ()
{
	uint8_t kstate = 0;
	uint32_t key = emGetKey(&kstate);

	if (key && kstate&EM_STATE_KEY){
		if (key == EM_KEY_LSHIFT || key == EM_KEY_RSHIFT || key == EM_KEY_SYMBOL /*|| key == EM_KEY_CALC*/)
			return 0;

		if ((kstate&EM_STATE_LSHIFT) || (kstate&EM_STATE_RSHIFT)){
			uint8_t newcode = emKeyToShift(key);
			if (newcode) key = newcode;
		}
	    		
		if (kstate&EM_STATE_SYMBOL){
			uint8_t newcode = emKeyToSymbol(key);
			if (newcode) key = newcode;
		}

		if (kstate&EM_STATE_CALC){
			// nothing to do here
			// add your own 
		}

		return key;
	}
	return 0;
}


uint32_t emGetScancode ()
{
	uint8_t c = (uint8_t)keyboard.readScanCode();
    if (c){
    	if (c != 0xF0){
    		kstate[c] = 1;

    	}else{		// 0xF0 indicates following key is released
    		delay(5);		
    		c = keyboard.readScanCode();
    		kstate[c] = 0;
    	}
    }
    return c;
}

uint8_t emCodeToKey (uint8_t code)
{
	return scode[code];
}

void emKeyboard_begin (uint8_t dataPin, uint8_t irq_pin)
{
	keyboard.begin(dataPin, irq_pin);
}

int emKeyboard_isAvailable ()
{
	return keyboard.available();
}

void emKeyboard_clear ()
{
	return keyboard.clear();
}

void emSetScanTable ()
{
	scode[EM_CODE_store] = EM_KEY_STORE;		// 70
	scode[EM_CODE_setup] = EM_KEY_SETUP;		// 7A
	scode[EM_CODE_services] = EM_KEY_SERVICES;	// 46
	scode[EM_CODE_games] = EM_KEY_GAMES;		// 7C
	scode[EM_CODE_internet] = EM_KEY_INTERNET;	// 77
	scode[EM_CODE_home] = EM_KEY_HOME;			// 72
	scode[EM_CODE_office] = EM_KEY_OFFICE;		// 69
	scode[EM_CODE_mobile] = EM_KEY_MOBILE;		// 1A
	scode[EM_CODE_textmsg] = EM_KEY_TEXTMSG;	// 2A
	scode[EM_CODE_email] = EM_KEY_EMAIL;		// 1C
	scode[EM_CODE_fax] = EM_KEY_FAX;			// 15

	scode[EM_CODE_stop] = EM_KEY_STOP;	//  71
	scode[EM_CODE_1] = EM_KEY_1;		//  74
	scode[EM_CODE_2] = EM_KEY_2;		//  73
	scode[EM_CODE_3] = EM_KEY_3;		//  6B
	scode[EM_CODE_4] = EM_KEY_4;		//  22
	scode[EM_CODE_5] = EM_KEY_5;		//  1B
	scode[EM_CODE_6] = EM_KEY_6;		//  1D
	scode[EM_CODE_7] = EM_KEY_7;		//  1E
	scode[EM_CODE_8] = EM_KEY_8;		//  79
	scode[EM_CODE_9] = EM_KEY_9;		//  7D
	scode[EM_CODE_0] = EM_KEY_0;		//  75
	scode[EM_CODE_back] = EM_KEY_BACK;	//	6C

	scode[EM_CODE_q] = EM_KEY_Q;		//  21
	scode[EM_CODE_w] = EM_KEY_W;		//  23
	scode[EM_CODE_e] = EM_KEY_E;		//  24
	scode[EM_CODE_r] = EM_KEY_R;		//  26
	scode[EM_CODE_t] = EM_KEY_T;		//  52
	scode[EM_CODE_y] = EM_KEY_Y;		//  5D
	scode[EM_CODE_u] = EM_KEY_U;		//  0D
	scode[EM_CODE_i] = EM_KEY_I;		//  0E
	scode[EM_CODE_o] = EM_KEY_O;		//  32
	scode[EM_CODE_p] = EM_KEY_P;		//  34

	scode[EM_CODE_enter] = EM_KEY_ENTER;//	2C

	scode[EM_CODE_a] = EM_KEY_A;		//  31
	scode[EM_CODE_s] = EM_KEY_S;		//  33
	scode[EM_CODE_d] = EM_KEY_D;		//  35
	scode[EM_CODE_f] = EM_KEY_F;		//  36
	scode[EM_CODE_g] = EM_KEY_G;		//  29
	scode[EM_CODE_h] = EM_KEY_H;		//  5B
	scode[EM_CODE_j] = EM_KEY_J;		//  03
	scode[EM_CODE_k] = EM_KEY_K;		//  76
	scode[EM_CODE_l] = EM_KEY_L;		//  3A
	scode[EM_CODE_AT] = EM_KEY_AT;		//  3B		@

	scode[EM_CODE_lshift] = EM_KEY_LSHIFT;	// 3C
	scode[EM_CODE_z] = EM_KEY_Z;		//  3D
	scode[EM_CODE_x] = EM_KEY_X;		//  4E
	scode[EM_CODE_c] = EM_KEY_C;		//  54
	scode[EM_CODE_v] = EM_KEY_V;		//  0B
	scode[EM_CODE_b] = EM_KEY_B;		//  05
	scode[EM_CODE_n] = EM_KEY_N;		//  41
	scode[EM_CODE_m] = EM_KEY_M;		//  42
	scode[EM_CODE_point] = EM_KEY_POINT;		// 43  		// .
	scode[EM_CODE_rshift] = EM_KEY_RSHIFT;		// 55

	scode[EM_CODE_addrbook] = EM_KEY_ADDRBOOK;	// 06
	
	scode[EM_CODE_calc] = EM_KEY_CALC;			// 83
	scode[EM_CODE_symbol] = EM_KEY_SYMBOL;		// 49
	scode[EM_CODE_space] = EM_KEY_SPACE;		// 4B
	scode[EM_CODE_comma] = EM_KEY_COMMA;		// 44 		// ,

	scode[EM_CODE_left ] = EM_KEY_LEFT;			// 16
	scode[EM_CODE_right] = EM_KEY_RIGHT;		// 09
	scode[EM_CODE_up] = EM_KEY_UP;				// 3E
	scode[EM_CODE_down] = EM_KEY_DOWN;			// 2E


	symbolsTable[EM_SYM_0] = EM_SYMCODE_0;
	symbolsTable[EM_SYM_1] = EM_SYMCODE_1;
	symbolsTable[EM_SYM_2] = EM_SYMCODE_2;
	symbolsTable[EM_SYM_3] = EM_SYMCODE_3;
	symbolsTable[EM_SYM_4] = EM_SYMCODE_4;
	symbolsTable[EM_SYM_5] = EM_SYMCODE_5;
	symbolsTable[EM_SYM_6] = EM_SYMCODE_6;
	symbolsTable[EM_SYM_7] = EM_SYMCODE_7;
	symbolsTable[EM_SYM_8] = EM_SYMCODE_8;
	symbolsTable[EM_SYM_9] = EM_SYMCODE_9;
       
	symbolsTable[EM_SYM_R] = EM_SYMCODE_R;
	symbolsTable[EM_SYM_T] = EM_SYMCODE_T;
	symbolsTable[EM_SYM_U] = EM_SYMCODE_U;
	            
	symbolsTable[EM_SYM_A] = EM_SYMCODE_A;
	symbolsTable[EM_SYM_G] = EM_SYMCODE_G;
	symbolsTable[EM_SYM_H] = EM_SYMCODE_H;
	symbolsTable[EM_SYM_AT] = EM_SYMCODE_AT;

	symbolsTable[EM_SYM_POINT] = EM_SYMCODE_POINT;
	symbolsTable[EM_SYM_COMMA] = EM_SYMCODE_COMMA;
	
	shiftTable[EM_SHIFT_0] = EM_SHIFTCODE_0;
	shiftTable[EM_SHIFT_1] = EM_SHIFTCODE_1;
	shiftTable[EM_SHIFT_2] = EM_SHIFTCODE_2;
	shiftTable[EM_SHIFT_3] = EM_SHIFTCODE_3;
	shiftTable[EM_SHIFT_4] = EM_SHIFTCODE_4;
	shiftTable[EM_SHIFT_5] = EM_SHIFTCODE_5;
	shiftTable[EM_SHIFT_6] = EM_SHIFTCODE_6;
	shiftTable[EM_SHIFT_7] = EM_SHIFTCODE_7;
	shiftTable[EM_SHIFT_8] = EM_SHIFTCODE_8;
	shiftTable[EM_SHIFT_9] = EM_SHIFTCODE_9;

	shiftTable[EM_SHIFT_AT] = EM_SHIFTCODE_AT;
	shiftTable[EM_SHIFT_POINT] = EM_SHIFTCODE_POINT;
	shiftTable[EM_SHIFT_COMMA] = EM_SHIFTCODE_COMMA;	

};


// The ISR for the external interrupt
void ps2interrupt ()
{
	static uint8_t bitcount = 0;
	static uint8_t incoming = 0;
	static uint32_t prev_ms = millis();

	uint8_t val = digitalRead(DataPin);
	uint32_t now_ms = millis();
	
	if (now_ms - prev_ms > 100) {
		bitcount = 0;
		incoming = 0;
	}
	
	prev_ms = now_ms;
	uint8_t n = bitcount - 1;
	
	if (n <= 7)
		incoming |= (val << n);

	if (++bitcount == 11) {
		uint8_t i = head + 1;
		
		if (i >= IN_BUFFER_SIZE)
			i = 0;
			
		if (i != tail){
			inbuffer[i] = incoming;
			head = i;
			keyReady++;
		}

		bitcount = 0;
		incoming = 0;
	}
}

static inline uint8_t get_scan_code ()
{
	uint8_t i = tail;
	if (i == head)
		return 0;

	if (++i >= IN_BUFFER_SIZE)
		i = 0;

	uint8_t c = inbuffer[i];
	tail = i;
	
	return c;
}

bool emKeyboard::available ()
{
	return (keyReady > 0);
}

void emKeyboard::clear()
{
	keyReady = 0;
}

uint8_t emKeyboard::readScanCode ()
{
	int code = get_scan_code();
	if (code)
		keyReady--;
	else
		keyReady = 0;
	return code;
}

emKeyboard::emKeyboard ()
{
	emSetScanTable();
}

void emKeyboard::begin (uint8_t data_pin, uint8_t irq_pin)
{
  uint8_t irq_num = 255;

  DataPin = data_pin;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(data_pin, INPUT_PULLUP);
#else
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
#endif

#ifdef CORE_INT_EVERY_PIN
  irq_num = irq_pin;

#else
  switch(irq_pin) {
    #ifdef CORE_INT0_PIN
    case CORE_INT0_PIN:
      irq_num = 0;
      break;
    #endif
    #ifdef CORE_INT1_PIN
    case CORE_INT1_PIN:
      irq_num = 1;
      break;
    #endif
    #ifdef CORE_INT2_PIN
    case CORE_INT2_PIN:
      irq_num = 2;
      break;
    #endif
    #ifdef CORE_INT3_PIN
    case CORE_INT3_PIN:
      irq_num = 3;
      break;
    #endif
    #ifdef CORE_INT4_PIN
    case CORE_INT4_PIN:
      irq_num = 4;
      break;
    #endif
    #ifdef CORE_INT5_PIN
    case CORE_INT5_PIN:
      irq_num = 5;
      break;
    #endif
    #ifdef CORE_INT6_PIN
    case CORE_INT6_PIN:
      irq_num = 6;
      break;
    #endif
    #ifdef CORE_INT7_PIN
    case CORE_INT7_PIN:
      irq_num = 7;
      break;
    #endif
    #ifdef CORE_INT8_PIN
    case CORE_INT8_PIN:
      irq_num = 8;
      break;
    #endif
    #ifdef CORE_INT9_PIN
    case CORE_INT9_PIN:
      irq_num = 9;
      break;
    #endif
    #ifdef CORE_INT10_PIN
    case CORE_INT10_PIN:
      irq_num = 10;
      break;
    #endif
    #ifdef CORE_INT11_PIN
    case CORE_INT11_PIN:
      irq_num = 11;
      break;
    #endif
    #ifdef CORE_INT12_PIN
    case CORE_INT12_PIN:
      irq_num = 12;
      break;
    #endif
    #ifdef CORE_INT13_PIN
    case CORE_INT13_PIN:
      irq_num = 13;
      break;
    #endif
    #ifdef CORE_INT14_PIN
    case CORE_INT14_PIN:
      irq_num = 14;
      break;
    #endif
    #ifdef CORE_INT15_PIN
    case CORE_INT15_PIN:
      irq_num = 15;
      break;
    #endif
    #ifdef CORE_INT16_PIN
    case CORE_INT16_PIN:
      irq_num = 16;
      break;
    #endif
    #ifdef CORE_INT17_PIN
    case CORE_INT17_PIN:
      irq_num = 17;
      break;
    #endif
    #ifdef CORE_INT18_PIN
    case CORE_INT18_PIN:
      irq_num = 18;
      break;
    #endif
    #ifdef CORE_INT19_PIN
    case CORE_INT19_PIN:
      irq_num = 19;
      break;
    #endif
    #ifdef CORE_INT20_PIN
    case CORE_INT20_PIN:
      irq_num = 20;
      break;
    #endif
    #ifdef CORE_INT21_PIN
    case CORE_INT21_PIN:
      irq_num = 21;
      break;
    #endif
    #ifdef CORE_INT22_PIN
    case CORE_INT22_PIN:
      irq_num = 22;
      break;
    #endif
    #ifdef CORE_INT23_PIN
    case CORE_INT23_PIN:
      irq_num = 23;
      break;
    #endif
  }
#endif

	head = 0;
	tail = 0;

	if (irq_num < 255)
		attachInterrupt(irq_num, ps2interrupt, FALLING);

}


