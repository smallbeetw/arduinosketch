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

//#define DEBUG

struct decode_results_rf {
  int decode_type;  // NEC, SONY, RC5, UNKNOWN
  union {           // This is used for decoding Panasonic and Sharp data
    unsigned int panasonicAddress;
    unsigned int sharpAddress;
  };
  unsigned long value; // Decoded value
  int bits;            // Number of bits in decoded value
};

/* IR receiver config */
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;

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
  printf("\n\rIRTatungRadioSend/\n\r");
#endif

  /* Setup and configure RF radio */
  radio.begin();

  /* optionally, increase the delay between retries & # of retries */
  radio.setRetries(15,15);

  /* Open pipes to other nodes for communication */
  /* send IR code through radio to receive node */
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  /* Start listening */
  radio.startListening(); //TODO: listen feedback from receive node

  /* Dump the configuration of the rf unit for debugging */
#ifdef DEBUG
  radio.printDetails();
#endif

  /* setup IR receive pin */
  irrecv.enableIRIn();		// Start the receiver
  irrecv.blink13(true);
}

void loop(void)
{
  //check received IR signal
  if (irrecv.decode(&results)) {
    decode_results_rf results_rf;
    bool send_ret;
    
#ifdef DEBUG
    printf("got IR\n");
#endif
    
    /* copy results for sending through RF */
    results_rf.decode_type = results.decode_type;
    if (results.decode_type == PANASONIC)
      results_rf.panasonicAddress = results.panasonicAddress;
    if (results.decode_type == SHARP)
      results_rf.sharpAddress = results.sharpAddress;
    results_rf.value = results.value;
    results_rf.bits = results.bits;
      
    /* Stop listening so we can send IR code through RF */
    radio.stopListening();
#ifdef DEBUG
    printf("Now sending %lu...", results_rf.value);
#endif
    send_ret = radio.write(&results_rf, sizeof(decode_results_rf));
#ifdef DEBUG
    if (send_ret)
      printf("sent...");
    else
      printf("send failed.\n\r");
#endif
    /* Continue listening for response from receive node */
    radio.startListening();

    /* Wait here until we get a response, or timeout (250ms) */
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    /* Got response or time out*/
#ifdef DEBUG
    if (timeout)
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long response_code;
      radio.read(&response_code, sizeof(unsigned long));     
      printf("Got response code %lu\n", response_code);
    }
#endif

    /* enable interrupt for receive again */
    irrecv.enableIRIn();
    irrecv.resume();
  }
}
