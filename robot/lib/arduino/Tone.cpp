/* Tone.cpp

  A Tone Generator Library

  Written by Brett Hagman

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

Version Modified By Date     Comments
------- ----------- -------- --------
0001    B Hagman    09/08/02 Initial coding
0002    B Hagman    09/08/18 Multiple pins
0003    B Hagman    09/08/18 Moved initialization from constructor to begin()
0004    B Hagman    09/09/26 Fixed problems with ATmega8
0005    B Hagman    09/11/23 Scanned prescalars for best fit on 8 bit timers
                    09/11/25 Changed pin toggle method to XOR
                    09/11/25 Fixed timer0 from being excluded
0006    D Mellis    09/12/29 Replaced objects with functions
0007    M Sproul    10/08/29 Changed #ifdefs from cpu to register
0008    S Kanemoto  12/06/22 Fixed for Leonardo by @maris_HY
0009    J Reucker   15/04/10 Issue #292 Fixed problems with ATmega8 (thanks to Pete62)
0010    jipp        15/04/13 added additional define check #2923
*************************************************/

#include "Arduino.h"
#include "pins_arduino.h"
#include <NewTone.h>


volatile uint8_t tonePin = 0; //переменная для хранения номера пина зуммера
volatile uint16_t toneFrequency = 0; //переменная для хранения частоты зуммера
volatile uint32_t toneDuration = 0; //переменная для хранения длительности звучания зуммера



void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  // Set the pinMode as OUTPUT
  pinMode(_pin, OUTPUT);
  
  // Save the pin number, frequency and duration
  tonePin = _pin;
  toneFrequency = frequency;
  toneDuration = duration;
  
   if (duration) {
	   tone(tonePin, toneFrequency,duration);
	   delay(duration * 4 / 3);
   }
   else {
	   tone(tonePin, toneFrequency); 
   delay(1);
   }
  
  
}

void noTone(uint8_t _pin)
{
  noTone(_pin); // Turn off the tone.
}



// XXX: this function only works properly for timer 2 (the only one we use
// currently).  for the others, it should end the tone, but won't restore
// proper PWM functionality for the timer.