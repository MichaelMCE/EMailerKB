
/*
  EMailerKB.h - emKeyboard driver example

  Amstrad Emailer mailboard driver
  Complerte rewrite by MichaelMcE
  May 2023
  

  Requries both pins specified for begin()

  emKeyboard_begin(data_pin, irq_pin);
  
  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 3.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11
  
  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
  
  Like the Original library and example this is under LGPL license.
  
  Modified by Cuninganreset@gmail.com on 2010-03-22
  Modified by Paul Stoffregen <paul@pjrc.com> June 2010
*/


   
#include "emKeyboard.h"



#define DataPin  8
#define IRQpin   5





void setup ()
{
	emKeyboard_begin(DataPin, IRQpin);
	
	Serial.begin(115200);
	while (!Serial);
	
	Serial.println("Keyboard Test:\r\n");
	emKeyboard_clear();
}

void loop ()
{
	if (emKeyboard_isAvailable()){

		uint8_t key = emGetKeyModified();
		if (key == EM_KEY_ENTER){
			printf("\r\n");
			
		}else if (key == EM_KEY_Q && (emGetModifiers()&EM_STATE_SYMBOL)){
			printf("Teensy\n");
			
		}else if (emIsFunctionKey(key) || key == EM_KEY_STOP || key == EM_KEY_ADDRBOOK || key == EM_KEY_CALC){
			const char *name = emKeyToName(key);
			printf("%s\n", name);
		
		}else if (key){
			const char *name = emKeyToName(key);		// left, right, etc..
			if (name)
				printf("%s\n", name);
			else
				printf("%c\n", key);
		}
	}

	delay(20);
}
