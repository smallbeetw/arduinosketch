/*
 * Sketch of Re-transmit IR code
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

//#define DEBUG 1

#define MAP_END -1

/* LF-BOX1 Panasonic CATV(1)/DVD(1) */
#define RECE_TYPE   UNKNOWN
#define CATV1_POWER   0xBB415335
#define CATV1_ONE     0x26726EFF
#define CATV1_TWO     0xDB91E2E9
#define CATV1_THREE   0xDCCD8C25
#define CATV1_FOUR    0x976306CB
#define CATV1_FIVE    0xC4B22325
#define CATV1_SIX     0xC1863ECB
#define CATV1_SEVEN   0xC23EA807
#define CATV1_EIGHT   0x6CECF4E9
#define CATV1_NINE    0x6C9AC189
#define CATV1_ZERO    0xAD98BAF
//#define CATV1_CHANNELUP      0x2B94164D
//#define CATV1_CHANNELDOWN    0x286831F3
#define CATV1_UP      0x2B94164D
#define CATV1_DOWN    0x286831F3
#define CATV1_CENTER  0x38B044FB
#define CATV1_RIGHT   0x85CCACC7
#define CATV1_LEFT    0xDC3FF4A1
#define CATV1_SOUNDS  0xFE44F9F3
#define CATV1_SATELLITES 0xA4381B63

#define DVD1_POWER   0x406A954D
#define DVD1_PLAY    0xDEC919A5
#define DVD1_PASS    0xAA7B2167
#define DVD1_STOP    0x1D15F4D7
#define DVD1_LAST    0x1F6E01C9
#define DVD1_NEXT    0x2E8C2A89
#define DVD1_FORWARD 0x59135E7
#define DVD1_BACKWARD 0xA23B903F
#define DVD1_UP      0x699FD5C8
#define DVD1_DOWN    0xA1B6A60E
#define DVD1_CENTER  0xB520CEC
#define DVD1_LEFT    0x20F40F38
#define DVD1_RIGHT   0x3DCA1A8
#define DVD1_ONE     0x51731C91
#define DVD1_TWO     0x4A1144F9
#define DVD1_THREE   0x226873C1
#define DVD1_FOUR    0x10957521
#define DVD1_FIVE    0xC0E837B9
#define DVD1_SIX     0x7A3D9EE1
#define DVD1_SEVEN   0x52118DA9
#define DVD1_EIGHT   0x403E8F09
#define DVD1_NINE    0x303A9883
#define DVD1_ZERO    0x38A296A3
#define DVD1_CONFIRM 0xB520CEC

/* Tivo */
#define EMIT_TYPE  NEC
#define TIVO       0xA10CE00F
#define LIVE_TV    0xA10C8807
#define UP         0xA10C2807
#define DOWN       0xA10C6807
#define RIGHT      0xA10CA807
#define LEFT       0xA10CE807
#define INFO       0xA10CC807
#define SELECT     0xA10C9807
#define CHANNELUP 0xA10C7807
#define CHANNELDOWN 0xA10CF807
#define PLAY       0xA10C840B
#define PASS       0xA10CC40B
#define FORWARD    0xA10C240B
#define BACKWARD   0xA10C440B
#define BACKSECONDS  0xA10C640B
#define HEAD_END   0xA10CE40B
#define ONE        0xA10C140B
#define TWO        0xA10C940B
#define THREE      0xA10C540B
#define FOUR       0xA10CD40B
#define FIVE       0xA10C340B
#define SIX        0xA10CB40B
#define SEVEN      0xA10C740B
#define EIGHT      0xA10CF40B
#define NINE       0xA10C0C03
#define ZERO       0xA10C8C03
#define ENTER      0xA10CCC03
#define CLEAR      0xA10C4C03
  
const int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

struct transf_entry {
  long receCode;                 // Receive Code: LF-BOX1 Panasonic
  long emitCode;                 // Emit Code: Tivo
  unsigned int delay_ms;     // delay ms for listen next code
};

static struct transf_entry ir_transf_map[] = {
  {CATV1_ONE, ONE, 200},
  {CATV1_TWO, TWO, 200},
  {CATV1_THREE, THREE, 200},
  {CATV1_FOUR, FOUR, 200},
  {CATV1_FIVE, FIVE, 200},
  {CATV1_SIX, SIX, 200},
  {CATV1_SEVEN, SEVEN, 200},
  {CATV1_EIGHT, EIGHT, 200},
  {CATV1_NINE, NINE, 200},
  {CATV1_ZERO, ZERO, 200},
  {CATV1_SOUNDS, CHANNELUP, 500},
  {CATV1_SATELLITES, CHANNELDOWN, 500},
  {CATV1_UP, UP, 500},
  {CATV1_DOWN, DOWN, 500},
  {CATV1_CENTER, SELECT, 500},
  {CATV1_RIGHT, RIGHT, 500},
  {CATV1_LEFT, LEFT, 500},
  {DVD1_PLAY, PLAY, 500},
  {DVD1_PASS, PASS, 500},
  //{DVD1_STOP, STOP, 500},
  {DVD1_LAST, HEAD_END, 500},
  {DVD1_NEXT, HEAD_END, 500},
  {DVD1_FORWARD, FORWARD, 500},
  {DVD1_BACKWARD, BACKWARD, 500},
  {DVD1_PLAY, PLAY, 500},
  {DVD1_UP, UP, 500},
  {DVD1_DOWN, DOWN, 500},
  {DVD1_CENTER, SELECT, 500},
  {DVD1_RIGHT, RIGHT, 500},
  {DVD1_LEFT, LEFT, 500},
  {DVD1_ONE, ONE, 200},
  {DVD1_TWO, TWO, 200},
  {DVD1_THREE, THREE, 200},
  {DVD1_FOUR, FOUR, 200},
  {DVD1_FIVE, FIVE, 200},
  {DVD1_SIX, SIX, 200},
  {DVD1_SEVEN, SEVEN, 200},
  {DVD1_EIGHT, EIGHT, 200},
  {DVD1_NINE, NINE, 200},
  {DVD1_ZERO, ZERO, 200},
  {DVD1_CONFIRM, BACKSECONDS, 500},
  {MAP_END, MAP_END, 0},
};

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  irrecv.enableIRIn();		// Start the receiver
  irrecv.blink13(true);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.decode_type == RECE_TYPE) {
        int i = 0;
        /* search transfer map */
        while (ir_transf_map[i].receCode != MAP_END) {
        if (results.value == ir_transf_map[i].receCode) {
          /* send raw data when paired in map */
          irsend.sendNEC(ir_transf_map[i].emitCode, 32);
#ifdef DEBUG
          Serial.println(results.value, HEX);
#endif
          delay(ir_transf_map[i].delay_ms);
          break;
        }
        i++;
      }
    }
    /* enable interrupt for receive again */
    irrecv.enableIRIn();
    irrecv.resume();
  }
}
