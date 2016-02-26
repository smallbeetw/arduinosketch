/*
 * Sketch of Transfer IR code
 *
 * Copyright (C) 2016 Smallbee.TW <smallbee.tw@gmail.com>
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
#include <avr/pgmspace.h>
#include <IRremote.h>

//#define DEBUG 1

#define MAP_END -1

/* LF-BOX1 Panasonic CATV(1) */
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
#define CATV1_UP      0x2B94164D  //CHANNEL UP
#define CATV1_DOWN    0x286831F3  //CHANNEL DOWN
#define CATV1_CENTER  0x38B044FB
#define CATV1_RIGHT   0x85CCACC7
#define CATV1_LEFT    0xDC3FF4A1
#define CATV1_SOUNDS  0xFE44F9F3
#define CATV1_SATELLITES 0xA4381B63

static unsigned int kbro_power[71] PROGMEM = {228, 892, 224, 2248, 228, 752, 224, 2792, 228, 1296, 232, 1288, 228, 1432, 232, 1832, 228, 14268, 232, 880, 224, 756, 308, 668, 312, 668, 228, 748, 228, 2792, 228, 748, 232, 748, 232, 15420, 224, 896, 232, 2240, 232, 748, 232, 2784, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 224, 14268, 232, 884, 232, 1832, 232, 1836, 224, 752, 228, 752, 228, 2788, 232, 748, 228, 748, 232};
static unsigned int kbro_chup[71] PROGMEM = {224, 892, 224, 2252, 224, 752, 224, 2796, 228, 1292, 232, 1292, 224, 1432, 232, 1836, 228, 14264, 228, 888, 228, 1024, 228, 748, 232, 744, 232, 748, 232, 2512, 236, 744, 224, 752, 228, 15468, 280, 840, 232, 2244, 232, 744, 224, 2792, 228, 1296, 232, 1288, 228, 1432, 228, 1836, 228, 14268, 224, 892, 224, 2112, 224, 1844, 220, 756, 224, 752, 228, 2520, 228, 748, 228, 752, 228};
static unsigned int kbro_chdown[71] PROGMEM = {224, 892, 232, 2244, 232, 744, 232, 2788, 224, 1296, 228, 1296, 232, 1424, 228, 1840, 224, 14268, 232, 884, 232, 880, 224, 756, 224, 752, 228, 752, 224, 2656, 228, 752, 228, 748, 232, 15464, 232, 884, 232, 2244, 228, 748, 232, 2788, 232, 1288, 228, 1296, 228, 1428, 224, 1844, 232, 14260, 228, 888, 228, 1972, 228, 1840, 224, 752, 228, 748, 228, 2656, 228, 748, 232, 748, 232};
static unsigned int kbro_one[71] PROGMEM = {232, 888, 232, 2244, 220, 756, 228, 2788, 232, 1292, 224, 1300, 224, 1432, 232, 1836, 224, 14268, 228, 888, 284, 2596, 224, 756, 228, 748, 232, 748, 224, 888, 232, 748, 224, 752, 232, 15412, 228, 888, 232, 2244, 224, 752, 232, 2788, 232, 1288, 228, 1296, 228, 1428, 232, 1836, 224, 14268, 232, 884, 220, 1572, 228, 1840, 228, 752, 220, 756, 228, 884, 224, 756, 228, 748, 232};    //KBRO_ONE
static unsigned int kbro_two[71] PROGMEM = {232, 888, 232, 2240, 228, 752, 228, 2788, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 892, 224, 2520, 232, 748, 224, 752, 232, 748, 224, 1024, 228, 752, 220, 756, 228, 15452, 224, 892, 228, 2248, 228, 748, 228, 2792, 228, 1292, 224, 1300, 224, 1432, 232, 1836, 224, 14268, 232, 884, 224, 1436, 224, 1840, 232, 744, 228, 752, 228, 1020, 224, 756, 228, 748, 224};
static unsigned int kbro_three[71] PROGMEM = {232, 888, 232, 2240, 224, 756, 228, 2788, 232, 1292, 224, 1296, 232, 1428, 224, 1844, 224, 14268, 232, 884, 224, 2384, 232, 748, 224, 752, 228, 752, 304, 1080, 228, 752, 232, 744, 228, 15444, 224, 896, 224, 2248, 232, 748, 224, 2792, 228, 1296, 232, 1288, 224, 1436, 228, 1836, 224, 14272, 228, 884, 224, 1300, 224, 1840, 232, 748, 224, 752, 232, 1156, 224, 752, 312, 664, 224};
static unsigned int kbro_four[71] PROGMEM = {224, 896, 224, 2248, 228, 752, 316, 2700, 224, 1300, 228, 1292, 224, 1436, 228, 1840, 228, 14264, 308, 808, 228, 2244, 224, 756, 228, 748, 232, 744, 228, 1296, 232, 748, 224, 752, 316, 15356, 228, 888, 232, 2244, 224, 752, 312, 2708, 232, 1288, 228, 1296, 228, 1432, 224, 1840, 228, 14268, 232, 880, 228, 1160, 228, 1836, 224, 752, 316, 664, 308, 1212, 232, 748, 224, 752, 228};
static unsigned int kbro_five[71] PROGMEM = {224, 896, 220, 2252, 228, 752, 228, 2788, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 892, 228, 2108, 224, 752, 232, 748, 224, 752, 312, 1348, 232, 748, 308, 668, 316, 15360, 228, 892, 228, 2244, 232, 748, 224, 2792, 228, 1296, 232, 1292, 224, 1432, 232, 1836, 224, 14268, 228, 888, 224, 1024, 228, 1840, 232, 744, 228, 752, 232, 1424, 224, 756, 228, 748, 224};
static unsigned int kbro_six[71] PROGMEM = {224, 892, 224, 2252, 228, 748, 232, 2788, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 232, 14260, 228, 888, 232, 1972, 224, 752, 232, 748, 224, 752, 228, 1568, 220, 756, 228, 752, 220, 15420, 232, 888, 232, 2240, 228, 752, 228, 2788, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 888, 232, 884, 224, 1844, 228, 748, 232, 744, 228, 1568, 232, 748, 224, 752, 232};  //KBRO_SIX, limit
static unsigned int kbro_seven[71] PROGMEM = {224, 892, 224, 2252, 228, 748, 224, 2796, 224, 1296, 232, 1292, 228, 1428, 228, 1840, 232, 14264, 224, 888, 232, 1836, 224, 752, 312, 668, 304, 672, 312, 1616, 236, 744, 312, 664, 316, 15344, 224, 892, 224, 2252, 228, 748, 236, 2784, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 220, 14272, 232, 884, 232, 748, 224, 1840, 232, 748, 224, 752, 312, 1616, 224, 756, 228, 752, 220};
static unsigned int kbro_eight[71] PROGMEM = {232, 884, 224, 2252, 228, 752, 228, 2788, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 232, 14264, 224, 888, 232, 1700, 224, 752, 228, 752, 304, 672, 312, 1756, 228, 748, 228, 752, 228, 15428, 224, 896, 220, 2252, 228, 752, 228, 2788, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 888, 232, 2788, 232, 1832, 228, 752, 232, 744, 228, 1840, 228, 748, 308, 620, 344};
static unsigned int kbro_nine[71] PROGMEM = {232, 888, 232, 2240, 224, 756, 228, 2788, 224, 1300, 224, 1296, 232, 1428, 224, 1840, 228, 14268, 224, 888, 228, 1568, 232, 744, 312, 668, 312, 664, 312, 1892, 228, 748, 224, 756, 312, 15384, 232, 888, 228, 2248, 220, 756, 228, 2788, 232, 1292, 224, 1300, 228, 1428, 224, 1844, 224, 14268, 232, 884, 224, 2660, 228, 1836, 232, 744, 228, 752, 316, 1884, 228, 752, 228, 748, 224};
static unsigned int kbro_zero[71] PROGMEM = {224, 896, 224, 2248, 228, 752, 232, 2784, 228, 1296, 228, 1292, 224, 1436, 228, 1836, 232, 14264, 224, 892, 228, 2788, 236, 744, 228, 748, 232, 748, 224, 752, 232, 748, 224, 752, 232, 15424, 232, 888, 224, 2252, 224, 752, 232, 2784, 228, 1296, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 892, 228, 1700, 224, 1844, 224, 752, 316, 664, 224, 752, 232, 744, 228, 752, 228};    //KBRO_ZERO
static unsigned int kbro_return[71] PROGMEM = {224, 896, 220, 2252, 228, 752, 232, 2784, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 888, 232, 1292, 224, 752, 232, 748, 224, 1028, 224, 1976, 232, 748, 224, 752, 228, 15520, 224, 892, 228, 2248, 228, 748, 228, 2792, 228, 1292, 224, 1300, 228, 1428, 232, 1836, 232, 14260, 232, 884, 224, 2384, 228, 1840, 232, 744, 228, 1024, 232, 1972, 220, 756, 228, 748, 224};  //KBRO_RETURN
static unsigned int kbro_pinfo[71] PROGMEM = {224, 892, 228, 2248, 228, 748, 224, 2796, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 228, 14268, 228, 884, 224, 1164, 228, 748, 224, 756, 224, 1024, 232, 2108, 224, 752, 228, 748, 228, 15464, 224, 896, 224, 2248, 228, 752, 232, 2784, 224, 1300, 280, 1240, 232, 1428, 224, 1840, 228, 14268, 228, 884, 224, 2252, 224, 1840, 232, 748, 224, 1028, 224, 2112, 232, 744, 228, 752, 228};  //KBRO_PINFO
static unsigned int kbro_ram_buf[71];

const int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

struct transf_entry {
  long receCode;                         // Hash code of Panasonic
  unsigned int *kbroCode;
  unsigned int delay_ms;     // delay ms for listen next code
};

static struct transf_entry ir_map1[] = {
  {CATV1_POWER, kbro_power, 400},
  {CATV1_UP, kbro_chup, 155},
  {CATV1_DOWN, kbro_chdown, 155},
  {CATV1_ONE, kbro_one, 155},
  {CATV1_TWO, kbro_two, 155},
  {CATV1_THREE, kbro_three, 155},
  {CATV1_FOUR, kbro_four, 155},
  {CATV1_FIVE, kbro_five, 155},
  {CATV1_SIX, kbro_six, 155},
  {CATV1_SEVEN, kbro_seven, 155},
  {CATV1_EIGHT, kbro_eight, 155},
  {CATV1_NINE, kbro_nine, 155},
  {CATV1_ZERO, kbro_zero, 155},
  {CATV1_SATELLITES, kbro_return, 200},
  {CATV1_SOUNDS, kbro_pinfo, 200},
  {MAP_END, NULL, 0},
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
#ifdef DEBUG
      Serial.println(results.value, HEX);
#endif
      /* search transfer map */
        while (ir_map1[i].receCode != MAP_END) {
        if (results.value == ir_map1[i].receCode) {
#ifdef DEBUG
      Serial.println("Found");
#endif
          /* send raw data when paired in map */
          memcpy_P (kbro_ram_buf, ir_map1[i].kbroCode, 71);
          irsend.sendRaw(kbro_ram_buf, 71, 38);
          delay(ir_map1[i].delay_ms);
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
