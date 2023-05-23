/*
  emKeyboard.h - emKeyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  Amstrad Emailer mailboard driver
  Complerte rewrite by MichaelMcE
  May 2023
  
  Version 2.4 (March 2013)
  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com>, June 2010
  ** Modified for use with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **


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


#ifndef _EMKEYBOARD_H
#define _EMKEYBOARD_H


#include <Arduino.h> // for attachInterrupt, FALLING
#include "int_pins.h"
#include "EMailerKB.h"


#define IN_BUFFER_SIZE 32

/**
 * Purpose: Provides an easy access to PS2 keyboards
 * Author:  Christian Weichel
 */
class emKeyboard {
  public:
  	/**
  	 * This constructor does basically nothing. Please call the begin(int,int)
  	 * method before using any other method of this class.
  	 */
    emKeyboard ();

    /**
     * Starts the keyboard "service" by registering the external interrupt.
     * setting the pin modes correctly and driving those needed to high.
     * The propably best place to call this method is in the setup routine.
     */
    static void begin (uint8_t dataPin, uint8_t irq_pin);

    /**
     * Returns true if there is a char to be read, false if not.
     */
    static bool available ();

    /* Discards any received data, sets available() to false without a call to read()
    */
    static void clear ();

    /**
     * Retutns mailboard scan code - not a ps2 standard.
     */
    static uint8_t readScanCode ();
};

// init library here
void emKeyboard_begin (uint8_t dataPin, uint8_t irq_pin);

// check for new input
int emKeyboard_isAvailable ();

// returns key with up/down state and modifiers state (EM_STATE_xxxx)
uint32_t emGetKey (uint8_t *modifiersState);

// report only on key down event with modifiers precomputed
uint32_t emGetKeyModified ();

// returns current state of modifiers keys
uint8_t emGetModifiers ();

// key-to-name lookup of the top row function keys
const char *emKeyToName (const uint8_t code);

// computes new key code for that modifier
uint8_t emKeyToSymbol (const uint8_t code);
uint8_t emKeyToShift (const uint8_t code);

// mailboard to ascii code conversion
uint8_t emCodeToKey (uint8_t code);

// clear queue
void emKeyboard_clear ();

// returns true if the passed key is a toprow function key
bool emIsFunctionKey (uint8_t key);

uint32_t emGetScancode ();



#endif
