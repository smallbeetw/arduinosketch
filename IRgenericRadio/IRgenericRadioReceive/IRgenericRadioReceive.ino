/*
 * Sketch of Transfer Generic IR code
 *
 * Copyright (C) 2015 Smallbee.TW <smallbee.tw@gmail.com>
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
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

struct decode_results_rf {
  int decode_type;  // NEC, SONY, RC5, UNKNOWN
  union {           // This is used for decoding Panasonic and Sharp data
    unsigned int panasonicAddress;
    unsigned int sharpAddress;
  };
  unsigned long value; // Decoded value
  int bits;            // Number of bits in decoded value
};

/* IR sender config */
IRsend irsend;

/* RF config */
/* Set up nRF24L01 radio on SPI bus plus pins 9 & 10 */
RF24 radio(9,10);

/* Topology */
/* Radio pipe addresses for the 2 nodes to communicate. */
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void)
{
  /* Print preamble */
#ifdef DEBUG
  Serial.begin(9600);
  printf_begin();
  printf("\n\rIRTatungRadioReceive\n\r");
#endif

  /* Setup and configure RF radio */
  radio.begin();

  /* optionally, increase the delay between retries & # of retries */
  radio.setRetries(15,15);

  /* Open pipes to other nodes for communication */
  /* send IR code through radio to receive node */
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  /* Start listening */
  radio.startListening(); 

  /* Dump the configuration of the rf unit for debugging */
#ifdef DEBUG
  radio.printDetails();
#endif
}

void loop(void)
{
  /* if received RF data */
  if (radio.available()) {
      // Dump the payloads until we've gotten everything
      decode_results_rf results_rf;
      bool done = false;
      
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read(&results_rf, sizeof(decode_results_rf));
#ifdef DEBUG
        printf("Got payload %lu...", results_rf.value);
#endif
	// Delay just a little bit to let the other unit
	// make the transition to receiver
	delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();
      
      /* send IR signature */
      if (results_rf.decode_type == NEC)
        irsend.sendNEC(results_rf.value, results_rf.bits);
      else if (results_rf.decode_type == PANASONIC)
        irsend.sendPanasonic(results_rf.panasonicAddress, results_rf.value);
      else if (results_rf.decode_type == SONY) {
        for (int i=0; i < 3; i++) {
          irsend.sendSony(results_rf.value, results_rf.bits);
          delay(100);
        }
      }

      // Send the final one back.
      radio.write(&results_rf.value, sizeof(unsigned long));
#ifdef DEBUG
      printf("Sent response.\n\r");
#endif

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }    
}
