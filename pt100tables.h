#ifndef PT100TABLES_H_
#define PT100TABLES_H_

#include "Marlin.h"

#if (defined(HEATER_0_USES_DETECTION) || defined(HEATER_1_USES_DETECTION))
// PT-100 temperature sensor.
// PT-100 is positive temperature coefficient: no inversion applied to the raw measured ADC value
// Measured ADC result is 10 bits, put into an unsigned 16 bit integer (rght adjusted).
// 0V corresponds to 0, 5V corresponds to 1023
// TODO provide documentation and link
const short temptable_pt100[][2] PROGMEM = { 
    { OVERSAMPLENR * 415, 0 },
{ OVERSAMPLENR * 423, 5 },
{ OVERSAMPLENR * 431, 10 },
{ OVERSAMPLENR * 439, 15 },
{ OVERSAMPLENR * 447, 20 },
{ OVERSAMPLENR * 454, 25 },
{ OVERSAMPLENR * 462, 30 },
{ OVERSAMPLENR * 470, 35 },
{ OVERSAMPLENR * 478, 40 },
{ OVERSAMPLENR * 486, 45 },
{ OVERSAMPLENR * 494, 50 },
{ OVERSAMPLENR * 501, 55 },
{ OVERSAMPLENR * 509, 60 },
{ OVERSAMPLENR * 517, 65 },
{ OVERSAMPLENR * 524, 70 },
{ OVERSAMPLENR * 532, 75 },
{ OVERSAMPLENR * 540, 80 },
{ OVERSAMPLENR * 547, 85 },
{ OVERSAMPLENR * 555, 90 },
{ OVERSAMPLENR * 563, 95 },
{ OVERSAMPLENR * 570, 100 },
{ OVERSAMPLENR * 578, 105 },
{ OVERSAMPLENR * 585, 110 },
{ OVERSAMPLENR * 593, 115 },
{ OVERSAMPLENR * 600, 120 },
{ OVERSAMPLENR * 608, 125 },
{ OVERSAMPLENR * 615, 130 },
{ OVERSAMPLENR * 623, 135 },
{ OVERSAMPLENR * 630, 140 },
{ OVERSAMPLENR * 638, 145 },
{ OVERSAMPLENR * 645, 150 },
{ OVERSAMPLENR * 653, 155 },
{ OVERSAMPLENR * 660, 160 },
{ OVERSAMPLENR * 667, 165 },
{ OVERSAMPLENR * 675, 170 },
{ OVERSAMPLENR * 682, 175 },
{ OVERSAMPLENR * 689, 180 },
{ OVERSAMPLENR * 697, 185 },
{ OVERSAMPLENR * 704, 190 },
{ OVERSAMPLENR * 711, 195 },
{ OVERSAMPLENR * 718, 200 },
{ OVERSAMPLENR * 726, 205 },
{ OVERSAMPLENR * 733, 210 },
{ OVERSAMPLENR * 740, 215 },
{ OVERSAMPLENR * 747, 220 },
{ OVERSAMPLENR * 754, 225 },
{ OVERSAMPLENR * 762, 230 },
{ OVERSAMPLENR * 769, 235 },
{ OVERSAMPLENR * 776, 240 },
{ OVERSAMPLENR * 783, 245 },
{ OVERSAMPLENR * 790, 250 },
{ OVERSAMPLENR * 797, 255 },
{ OVERSAMPLENR * 804, 260 },
{ OVERSAMPLENR * 811, 265 },
{ OVERSAMPLENR * 818, 270 },
{ OVERSAMPLENR * 825, 275 },
{ OVERSAMPLENR * 832, 280 },
{ OVERSAMPLENR * 839, 285 },
{ OVERSAMPLENR * 846, 290 },
{ OVERSAMPLENR * 853, 295 },
{ OVERSAMPLENR * 860, 300 },
{ OVERSAMPLENR * 867, 305 },
{ OVERSAMPLENR * 874, 310 },
{ OVERSAMPLENR * 880, 315 },
{ OVERSAMPLENR * 887, 320 },
{ OVERSAMPLENR * 894, 325 },
{ OVERSAMPLENR * 901, 330 },
{ OVERSAMPLENR * 908, 335 },
{ OVERSAMPLENR * 915, 340 },
{ OVERSAMPLENR * 921, 345 },
{ OVERSAMPLENR * 928, 350 },
{ OVERSAMPLENR * 935, 355 },
{ OVERSAMPLENR * 941, 360 },
{ OVERSAMPLENR * 948, 365 },
{ OVERSAMPLENR * 955, 370 },
{ OVERSAMPLENR * 962, 375 },
{ OVERSAMPLENR * 968, 380 },
{ OVERSAMPLENR * 975, 385 },
{ OVERSAMPLENR * 981, 390 },
{ OVERSAMPLENR * 988, 395 },
{ OVERSAMPLENR * 995, 400 }

};
#define temptable_pt100_len (sizeof(temptable_pt100) / sizeof(temptable_pt100[0]))
#endif

#endif /* PT100TABLES_H_ */