/*
 * Sketch of Re-transmit IR code
 *
 * Copyright (C) 2014 Smallbee.TW <smallbee.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <IRremote.h>

//#define DEBUG                // unmark this if you want print out RAW code

const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);       // receive IR from 11 pin, IR receiver
IRsend irsend;	               // define send from D3 pin, IR LED
			       // the specific transmit pin is 10 pin on Teensy 2.0
decode_results results;

unsigned int rawbuf[RAWBUF];

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();		// Start the receiver
  irrecv.blink13(true);
}

void loop() {
  if (irrecv.decode(&results)) {
    /* convert volatile unsigned int to unsigned int array.
     * ignore results.rawbuf[0].
     */
#ifdef DEBUG
    Serial.print(results.value, HEX);
    Serial.print(" (");
    Serial.print(results.bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(results.rawlen, DEC);
    Serial.println("): ");
#endif
    for (int i = 0; i < results.rawlen - 1; i++) {
      rawbuf[i] = results.rawbuf[i + 1] * USECPERTICK;
#ifdef DEBUG
      Serial.print(rawbuf[i], DEC);
      if (i < results.rawlen - 2)
        Serial.print(",");
      else
        Serial.println("");
#endif
    }

    /* send raw data */
    irsend.sendRaw(rawbuf, results.rawlen - 1, 38);
    delay(1000);
    
    /* enable interrupt for receive again */
    irrecv.enableIRIn();
    irrecv.resume();			// Waiting to receive the next IR code
  }
}
