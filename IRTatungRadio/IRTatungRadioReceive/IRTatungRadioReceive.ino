/*
 * Sketch of Transfer IR code
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
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define MAP_SIZE 14

#define NEC_POWER        0x10EFA05F
#define NEC_RETURN       0x10EF30CF
//#define NEC_PINFO
#define NEC_CHANNELUP    0x10EF40BF
#define NEC_CHANNELDOWN  0x10EFC03F
#define NEC_ONE          0x10EF8877
#define NEC_TWO          0x10EF48B7
#define NEC_THREE        0x10EFC837
#define NEC_FOUR         0x10EF28D7
#define NEC_FIVE         0x10EFA857
#define NEC_SIX          0x10EF6897
#define NEC_SEVEN        0x10EFE817
#define NEC_EIGHT        0x10EF18E7
#define NEC_NINE         0x10EF9867
#define NEC_ZERO         0x10EF08F7

/* IR sender config */
IRsend irsend;

struct transf_entry {
  long tatungCode;                 // Hash code of tatung
  unsigned int kbroRawCode[17];    // IR raw code of KBRO
};

static struct transf_entry ir_transf_map[MAP_SIZE] = {
  {NEC_POWER, {200,900,200,750,250,750,200,750,200,750,250,2750,250,750,200,750,200}},
  {NEC_RETURN, {250,850,250,1300,200,750,200,750,250,1000,200,2000,200,750,200,750,250}},
  //{PANASONIC_PINFO, {250,850,250,1150,200,750,250,700,250,1000,250,2050,250,750,200,750,250}},
  /* Channel UP */
  {NEC_CHANNELUP, {250,850,250,1000,250,700,250,750,200,750,250,2450,250,750,200,750,250}},
  /* Channel DOWN */
  {NEC_CHANNELDOWN, {250,850,250,850,250,750,200,750,250,700,250,2600,250,750,200,750,250}},
  {NEC_ONE, {250,850,250,2650,200,750,200,750,250,750,200,900,200,750,200,750,250}},
  {NEC_TWO, {250,850,250,2450,250,750,200,750,250,700,250,1000,250,750,200,750,200}},
  {NEC_THREE, {200,900,200,2400,200,750,200,750,250,750,200,1150,200,750,250,750,200}},
  {NEC_FOUR, {200,900,200,2250,200,750,200,750,250,750,200,1300,200,750,250,700,250}},
  {NEC_FIVE, {250,850,250,2100,200,750,250,700,250,750,200,1400,250,750,200,750,250}},
  {NEC_SIX, {250,850,250,1950,200,750,250,750,200,750,200,1600,200,750,200,750,250}},
  {NEC_SEVEN, {200,900,200,1850,200,750,250,750,200,750,200,1700,250,700,250,750,200}},
  {NEC_EIGHT, {250,850,250,1650,250,750,200,750,250,750,200,1800,250,750,200,750,250}},
  {NEC_NINE, {200,900,250,1550,200,750,200,750,250,750,200,1950,250,750,200,750,200}},
  {NEC_ZERO, {200,900,200,2800,200,750,200,750,250,750,200,750,200,750,250,750,200}},
};

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
      unsigned long got_ircode;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_ircode, sizeof(unsigned long) );
#ifdef DEBUG
        printf("Got payload %lu...",got_ircode);
#endif
	// Delay just a little bit to let the other unit
	// make the transition to receiver
	delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();
      
      /* send IR signature */
      for (int i = 0; i < MAP_SIZE; i++) {
        if (got_ircode == ir_transf_map[i].tatungCode) {
          /* send raw data when paired in map */
          irsend.sendRaw(ir_transf_map[i].kbroRawCode, 17, 38);

          break;
        }
      }

      // Send the final one back.
      radio.write( &got_ircode, sizeof(unsigned long) );
#ifdef DEBUG
      printf("Sent response.\n\r");
#endif

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }    
}
