/*
 * Sketch of Transfer IR code.
 * Collect raw code for all kbro cable box for testing.
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
#include <IRremote.h>

IRsend irsend;

unsigned int sendRawCodes[71]={224, 892, 224, 2252, 224, 752, 224, 2796, 228, 1292, 232, 1292, 224, 1432, 232, 1836, 228, 14264, 228, 888, 228, 1024, 228, 748, 232, 744, 232, 748, 232, 2512, 236, 744, 224, 752, 228, 15468, 280, 840, 232, 2244, 232, 744, 224, 2792, 228, 1296, 232, 1288, 228, 1432, 228, 1836, 228, 14268, 224, 892, 224, 2112, 224, 1844, 220, 756, 224, 752, 228, 2520, 228, 748, 228, 752, 228}; //CHANNEL UP
//unsigned int sendRawCodes[71]={224, 892, 232, 2244, 232, 744, 232, 2788, 224, 1296, 228, 1296, 232, 1424, 228, 1840, 224, 14268, 232, 884, 232, 880, 224, 756, 224, 752, 228, 752, 224, 2656, 228, 752, 228, 748, 232, 15464, 232, 884, 232, 2244, 228, 748, 232, 2788, 232, 1288, 228, 1296, 228, 1428, 224, 1844, 232, 14260, 228, 888, 228, 1972, 228, 1840, 224, 752, 228, 748, 228, 2656, 228, 748, 232, 748, 232};  //CHANNEL DOWN
//unsigned int sendRawCodes[71]={228, 892, 224, 2248, 228, 752, 224, 2792, 228, 1296, 232, 1288, 228, 1432, 232, 1832, 228, 14268, 232, 880, 224, 756, 308, 668, 312, 668, 228, 748, 228, 2792, 228, 748, 232, 748, 232, 15420, 224, 896, 232, 2240, 232, 748, 232, 2784, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 224, 14268, 232, 884, 232, 1832, 232, 1836, 224, 752, 228, 752, 228, 2788, 232, 748, 228, 748, 232}; //POWER 
//unsigned int sendRawCodes[71]={224, 892, 228, 2248, 228, 748, 224, 2796, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 228, 14268, 228, 884, 224, 1164, 228, 748, 224, 756, 224, 1024, 232, 2108, 224, 752, 228, 748, 228, 15464, 224, 896, 224, 2248, 228, 752, 232, 2784, 224, 1300, 280, 1240, 232, 1428, 224, 1840, 228, 14268, 228, 884, 224, 2252, 224, 1840, 232, 748, 224, 1028, 224, 2112, 232, 744, 228, 752, 228};  //PINFO
//unsigned int sendRawCodes[71]={ 232, 888, 232, 2244, 220, 756, 228, 2788, 232, 1292, 224, 1300, 224, 1432, 232, 1836, 224, 14268, 228, 888, 284, 2596, 224, 756, 228, 748, 232, 748, 224, 888, 232, 748, 224, 752, 232, 15412, 228, 888, 232, 2244, 224, 752, 232, 2788, 232, 1288, 228, 1296, 228, 1428, 232, 1836, 224, 14268, 232, 884, 220, 1572, 228, 1840, 228, 752, 220, 756, 228, 884, 224, 756, 228, 748, 232};  //ONE
//unsigned int sendRawCodes[71]={ 232, 888, 232, 2240, 228, 752, 228, 2788, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 892, 224, 2520, 232, 748, 224, 752, 232, 748, 224, 1024, 228, 752, 220, 756, 228, 15452, 224, 892, 228, 2248, 228, 748, 228, 2792, 228, 1292, 224, 1300, 224, 1432, 232, 1836, 224, 14268, 232, 884, 224, 1436, 224, 1840, 232, 744, 228, 752, 228, 1020, 224, 756, 228, 748, 224};  //TWO
//unsigned int sendRawCodes[71]={ 232, 888, 232, 2240, 224, 756, 228, 2788, 232, 1292, 224, 1296, 232, 1428, 224, 1844, 224, 14268, 232, 884, 224, 2384, 232, 748, 224, 752, 228, 752, 304, 1080, 228, 752, 232, 744, 228, 15444, 224, 896, 224, 2248, 232, 748, 224, 2792, 228, 1296, 232, 1288, 224, 1436, 228, 1836, 224, 14272, 228, 884, 224, 1300, 224, 1840, 232, 748, 224, 752, 232, 1156, 224, 752, 312, 664, 224};  //THREE
//unsigned int sendRawCodes[71]={ 224, 896, 224, 2248, 228, 752, 316, 2700, 224, 1300, 228, 1292, 224, 1436, 228, 1840, 228, 14264, 308, 808, 228, 2244, 224, 756, 228, 748, 232, 744, 228, 1296, 232, 748, 224, 752, 316, 15356, 228, 888, 232, 2244, 224, 752, 312, 2708, 232, 1288, 228, 1296, 228, 1432, 224, 1840, 228, 14268, 232, 880, 228, 1160, 228, 1836, 224, 752, 316, 664, 308, 1212, 232, 748, 224, 752, 228};  //FOUR
//unsigned int sendRawCodes[71]={ 224, 896, 220, 2252, 228, 752, 228, 2788, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 892, 228, 2108, 224, 752, 232, 748, 224, 752, 312, 1348, 232, 748, 308, 668, 316, 15360, 228, 892, 228, 2244, 232, 748, 224, 2792, 228, 1296, 232, 1292, 224, 1432, 232, 1836, 224, 14268, 228, 888, 224, 1024, 228, 1840, 232, 744, 228, 752, 232, 1424, 224, 756, 228, 748, 224};  //FIVE
//unsigned int sendRawCodes[71]={ 224, 892, 224, 2252, 228, 748, 232, 2788, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 232, 14260, 228, 888, 232, 1972, 224, 752, 232, 748, 224, 752, 228, 1568, 220, 756, 228, 752, 220, 15420, 232, 888, 232, 2240, 228, 752, 228, 2788, 224, 1300, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 888, 232, 884, 224, 1844, 228, 748, 232, 744, 228, 1568, 232, 748, 224, 752, 232};  //SIX
//unsigned int sendRawCodes[71]={ 224, 892, 224, 2252, 228, 748, 224, 2796, 224, 1296, 232, 1292, 228, 1428, 228, 1840, 232, 14264, 224, 888, 232, 1836, 224, 752, 312, 668, 304, 672, 312, 1616, 236, 744, 312, 664, 316, 15344, 224, 892, 224, 2252, 228, 748, 236, 2784, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 220, 14272, 232, 884, 232, 748, 224, 1840, 232, 748, 224, 752, 312, 1616, 224, 756, 228, 752, 220};  //SEVEN
//unsigned int sendRawCodes[71]={ 232, 884, 224, 2252, 228, 752, 228, 2788, 224, 1296, 232, 1292, 224, 1432, 228, 1840, 232, 14264, 224, 888, 232, 1700, 224, 752, 228, 752, 304, 672, 312, 1756, 228, 748, 228, 752, 228, 15428, 224, 896, 220, 2252, 228, 752, 228, 2788, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 888, 232, 2788, 232, 1832, 228, 752, 232, 744, 228, 1840, 228, 748, 308, 620, 344};  //EIGHT
//unsigned int sendRawCodes[71]={ 232, 888, 232, 2240, 224, 756, 228, 2788, 224, 1300, 224, 1296, 232, 1428, 224, 1840, 228, 14268, 224, 888, 228, 1568, 232, 744, 312, 668, 312, 664, 312, 1892, 228, 748, 224, 756, 312, 15384, 232, 888, 228, 2248, 220, 756, 228, 2788, 232, 1292, 224, 1300, 228, 1428, 224, 1844, 224, 14268, 232, 884, 224, 2660, 228, 1836, 232, 744, 228, 752, 316, 1884, 228, 752, 228, 748, 224};  //NINE
//unsigned int sendRawCodes[71]={ 224, 896, 224, 2248, 228, 752, 232, 2784, 228, 1296, 228, 1292, 224, 1436, 228, 1836, 232, 14264, 224, 892, 228, 2788, 236, 744, 228, 748, 232, 748, 224, 752, 232, 748, 224, 752, 232, 15424, 232, 888, 224, 2252, 224, 752, 232, 2784, 228, 1296, 228, 1292, 232, 1428, 224, 1840, 232, 14264, 224, 892, 228, 1700, 224, 1844, 224, 752, 316, 664, 224, 752, 232, 744, 228, 752, 228};  //ZERO
//unsigned int sendRawCodes[71]={ 224, 896, 220, 2252, 228, 752, 232, 2784, 224, 1300, 228, 1292, 224, 1436, 224, 1840, 232, 14264, 224, 888, 232, 1292, 224, 752, 232, 748, 224, 1028, 224, 1976, 232, 748, 224, 752, 228, 15520, 224, 892, 228, 2248, 228, 748, 228, 2792, 228, 1292, 224, 1300, 228, 1428, 232, 1836, 232, 14260, 232, 884, 224, 2384, 228, 1840, 232, 744, 228, 1024, 232, 1972, 220, 756, 228, 748, 224};  //RETURN

void setup() {
  Serial.begin(9600);
}

void loop() {
  irsend.sendRaw(sendRawCodes,71,38);
  Serial.println("Sent raw");
  delay(500);
}