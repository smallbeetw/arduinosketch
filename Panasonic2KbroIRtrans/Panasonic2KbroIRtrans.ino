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

#define MAP_SIZE 15

#define PANASONIC_OK      0xBB0ED9E1
#define PANASONIC_RETURN  0xD28EF217
#define PANASONIC_UP      0x4DE74847
#define PANASONIC_DOWN    0xB8781EF
#define PANASONIC_ONE     0xF7283C77
#define PANASONIC_TWO     0x757FB4DF
#define PANASONIC_THREE   0xB33B4597
#define PANASONIC_FOUR    0x3C03E507
#define PANASONIC_FIVE    0xE705551F
#define PANASONIC_SIX     0xA4A58EC7
#define PANASONIC_SEVEN   0xE2E45F7F
#define PANASONIC_EIGHT   0x6BACFEEF
#define PANASONIC_NINE    0xE88E91F
#define PANASONIC_ZERO    0x7D168BCF

const int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

struct transf_entry {
  long panasonicCode;                         // Hash code of Panasonic
  unsigned int kbroRawCode[17];          // IR raw code of KBRO
};

static struct transf_entry ir_transf_map[MAP_SIZE] = {
  {PANASONIC_OK, {250,850,250,1800,250,750,200,750,200,1050,200,1450,200,750,200,750,250}},
  {PANASONIC_RETURN, {250,850,250,1300,200,750,200,750,250,1000,200,2000,200,750,200,750,250}},
  /* Channel UP */
  {PANASONIC_UP, {250,850,250,1000,250,700,250,750,200,750,250,2450,250,750,200,750,250}},
  /* Channel DOWN */
  {PANASONIC_DOWN, {250,850,250,850,250,750,200,750,250,700,250,2600,250,750,200,750,250}},
  {PANASONIC_ONE, {250,850,250,2650,200,750,200,750,250,750,200,900,200,750,200,750,250}},
  {PANASONIC_TWO, {250,850,250,2450,250,750,200,750,250,700,250,1000,250,750,200,750,200}},
  {PANASONIC_THREE, {200,900,200,2400,200,750,200,750,250,750,200,1150,200,750,250,750,200}},
  {PANASONIC_FOUR, {200,900,200,2250,200,750,200,750,250,750,200,1300,200,750,250,700,250}},
  {PANASONIC_FIVE, {250,850,250,2100,200,750,250,700,250,750,200,1400,250,750,200,750,250}},
  {PANASONIC_SIX, {250,850,250,1950,200,750,250,750,200,750,200,1600,200,750,200,750,250}},
  {PANASONIC_SEVEN, {200,900,200,1850,200,750,250,750,200,750,200,1700,250,700,250,750,200}},
  {PANASONIC_EIGHT, {250,850,250,1650,250,750,200,750,250,750,200,1800,250,750,200,750,250}},
  {PANASONIC_NINE, {200,900,250,1550,200,750,200,750,250,750,200,1950,250,750,200,750,200}},
  {PANASONIC_ZERO, {200,900,200,2800,200,750,200,750,250,750,200,750,200,750,250,750,200}},
};

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();		// Start the receiver
  irrecv.blink13(true);
}

void loop() {
  if (irrecv.decode(&results)) {
    /* Panasonic IR code is UNKNOWN type */
    if (results.decode_type == UNKNOWN) { 
      /* search transfer map */
      Serial.print(results.value, HEX);
      for (int i = 0; i < MAP_SIZE; i++) {
        if (results.value == ir_transf_map[i].panasonicCode) {
          /* send raw data when paired in map */
          irsend.sendRaw(ir_transf_map[i].kbroRawCode, 17, 38);
          delay(30);    /* Good response result using 30 in this case */
          break;
        }
      }
    }
    /* enable interrupt for receive again */
    irrecv.enableIRIn();
    irrecv.resume();
  }
}
