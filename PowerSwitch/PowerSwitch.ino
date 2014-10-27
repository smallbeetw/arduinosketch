/*
 * Sketch of Control PC Power Switch 
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

const int ledPin = 11;		// Teensy has LED on 11, Teensy++ on 6
const int tlp521Pin = 01; 
char incomingByte;		// input numbers from USB to serial port
int seconds;			// How many seconds for similar to press button

// the setup() method runs once, when the sketch starts

void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(tlp521Pin, OUTPUT);
  
  Serial.begin(9600);	// USB is always 12 Mbit/sec
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {  
  while (Serial.available()) {
    incomingByte = Serial.read();
    // do something with incomingByte
    //Serial.println(incomingByte);  
    seconds = atoi(&incomingByte);
    digitalWrite(ledPin, HIGH);		// set the LED on
    digitalWrite(tlp521Pin, HIGH);
    delay(seconds * 1000);		// set 512 ping to high in seconds
    digitalWrite(ledPin, LOW);		// set the LED off
    digitalWrite(tlp521Pin, LOW);
  }
}

