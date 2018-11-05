#ifndef THERMISTORTABLES_H_
#define THERMISTORTABLES_H_

#include "Marlin.h"

#define OVERSAMPLENR 16

#if (THERMISTORHEATER_0 == 1) || (THERMISTORHEATER_1 == 1)  || (THERMISTORHEATER_2 == 1) || (THERMISTORBED == 1) //100k bed thermistor

// RTD Temperature table
//const short temptable_1[][2] PROGMEM = {
//	{ 25363, 450 },
//	{ 25451, 440 },
//	{ 25539, 430 },
//	{ 25628, 420 },
//	{ 25716, 410 },
//	{ 25804, 400 },
//	{ 25893, 390 },
//	{ 25981, 380 },
//	{ 26069, 370 },
//	{ 26158, 360 },
//	{ 26246, 350 },
//	{ 26334, 340 },
//	{ 26423, 330 },
//	{ 26511, 320 },
//	{ 26599, 310 },
//	{ 26688, 300 },
//	{ 26776, 290 },
//	{ 26864, 280 },
//	{ 26953, 270 },
//	{ 27041, 260 },
//	{ 27129, 250 },
//	{ 27218, 240 },
//	{ 27306, 230 },
//	{ 27395, 220 },
//	{ 27483, 210 },
//	{ 27571, 200 },
//	{ 27660, 190 },
//	{ 27748, 180 },
//	{ 27836, 170 },
//	{ 27925, 160 },
//	{ 28013, 150 },
//	{ 28101, 140 },
//	{ 28190, 130 },
//	{ 28278, 120 },
//	{ 28366, 110 },
//	{ 28455, 100 },
//	{ 28543, 90 },
//	{ 28631, 80 },
//	{ 28720, 70 },
//	{ 28808, 60 },
//	{ 28896, 50 },
//	{ 28985, 40 },
//	{ 29073, 30 },
//	{ 29161, 20 },
//	{ 29250, 10 },
//	{ 29338, 0 }
//};

// Ward temperature table
//const short temptable_1[][2] PROGMEM = {
//{  132 ,  400 },
//{  138 ,  395 },
//{  144 ,  390 },
//{  151 ,  385 },
//{  159 ,  380 },
//{  166 ,  375 },
//{  175 ,  370 },
//{  184 ,  365 },
//{  193 ,  360 },
//{  203 ,  355 },
//{  214 ,  350 },
//{  226 ,  345 },
//{  238 ,  340 },
//{  252 ,  335 },
//{  266 ,  330 },
//{  282 ,  325 },
//{  299 ,  320 },
//{  317 ,  315 },
//{  337 ,  310 },
//{  358 ,  305 },
//{  381 ,  300 },
//{  406 ,  295 },
//{  433 ,  290 },
//{  463 ,  285 },
//{  495 ,  280 },
//{  530 ,  275 },
//{  568 ,  270 },
//{  610 ,  265 },
//{  655 ,  260 },
//{  705 ,  255 },
//{  760 ,  250 },
//{  820 ,  245 },
//{  885 ,  240 },
//{  958 ,  235 },
//{  1037 ,  230 },
//{  1125 ,  225 },
//{  1221 ,  220 },
//{  1328 ,  215 },
//{  1445 ,  210 },
//{  1575 ,  205 },
//{  1718 ,  200 },
//{  1876 ,  195 },
//{  2050 ,  190 },
//{  2242 ,  185 },
//{  2455 ,  180 },
//{  2689 ,  175 },
//{  2946 ,  170 },
//{  3229 ,  165 },
//{  3539 ,  160 },
//{  3878 ,  155 },
//{  4247 ,  150 },
//{  4648 ,  145 },
//{  5080 ,  140 },
//{  5545 ,  135 },
//{  6040 ,  130 },
//{  6565 ,  125 },
//{  7117 ,  120 },
//{  7692 ,  115 },
//{  8285 ,  110 },
//{  8890 ,  105 },
//{  9502 ,  100 },
//{  10112 ,  95 },
//{  10714 ,  90 },
//{  11299 ,  85 },
//{  11861 ,  80 },
//{  12395 ,  75 },
//{  12894 ,  70 },
//{  13355 ,  65 },
//{  13776 ,  60 },
//{  14156 ,  55 },
//{  14494 ,  50 },
//{  14793 ,  45 },
//{  15053 ,  40 },
//{  15278 ,  35 },
//{  15470 ,  30 },
//{  15633 ,  25 },
//{  15770 ,  20 },
//{  15885 ,  15 },
//{  15980 ,  10 },
//{  16058 ,  5 },
//{  16122 ,  0 }
//};
 const short temptable_1[][2] PROGMEM = {
  {  149, 400 },
  { 157 ,  395 },
  { 166 ,  390 },
  { 175 ,  385 },
  { 185 ,  380 },
  { 195 ,  375 },
  { 206 ,  370 },
  { 218 ,  365 },
  { 231 ,  360 },
  { 245 ,  355 },
  { 260 ,  350 },
  { 276 ,  345 },
  { 293 ,  340 },
  { 312 ,  335 },
  { 332 ,  330 },
  { 353 ,  325 },
  { 377 ,  320 },
  { 402 ,  315 },
  { 429 ,  310 },
  { 459 ,  305 },
 {       23*OVERSAMPLENR ,       300     },
 {       25*OVERSAMPLENR ,       295     },
 {       27*OVERSAMPLENR ,       290     },
 {       28*OVERSAMPLENR ,       285     },
 {       31*OVERSAMPLENR ,       280     },
 {       33*OVERSAMPLENR ,       275     },
 {       35*OVERSAMPLENR ,       270     },
 {       38*OVERSAMPLENR ,       265     },
 {       41*OVERSAMPLENR ,       260     },
 {       44*OVERSAMPLENR ,       255     },
 {       48*OVERSAMPLENR ,       250     },
 {       52*OVERSAMPLENR ,       245     },
 {       56*OVERSAMPLENR ,       240     },
 {       61*OVERSAMPLENR ,       235     },
 {       66*OVERSAMPLENR ,       230     },
 {       71*OVERSAMPLENR ,       225     },
 {       78*OVERSAMPLENR ,       220     },
 {       84*OVERSAMPLENR ,       215     },
 {       92*OVERSAMPLENR ,       210     },
 {       100*OVERSAMPLENR        ,       205     },
 {       109*OVERSAMPLENR        ,       200     },
 {       120*OVERSAMPLENR        ,       195     },
 {       131*OVERSAMPLENR        ,       190     },
 {       143*OVERSAMPLENR        ,       185     },
 {       156*OVERSAMPLENR        ,       180     },
 {       171*OVERSAMPLENR        ,       175     },
 {       187*OVERSAMPLENR        ,       170     },
 {       205*OVERSAMPLENR        ,       165     },
 {       224*OVERSAMPLENR        ,       160     },
 {       245*OVERSAMPLENR        ,       155     },
 {       268*OVERSAMPLENR        ,       150     },
 {       293*OVERSAMPLENR        ,       145     },
 {       320*OVERSAMPLENR        ,       140     },
 {       348*OVERSAMPLENR        ,       135     },
 {       379*OVERSAMPLENR        ,       130     },
 {       411*OVERSAMPLENR        ,       125     },
 {       445*OVERSAMPLENR        ,       120     },
 {       480*OVERSAMPLENR        ,       115     },
 {       516*OVERSAMPLENR        ,       110     },
 {       553*OVERSAMPLENR        ,       105     },
 {       591*OVERSAMPLENR        ,       100     },
 {       628*OVERSAMPLENR        ,       95      },
 {       665*OVERSAMPLENR        ,       90      },
 {       702*OVERSAMPLENR        ,       85      },
 {       737*OVERSAMPLENR        ,       80      },
 {       770*OVERSAMPLENR        ,       75      },
 {       801*OVERSAMPLENR        ,       70      },
 {       830*OVERSAMPLENR        ,       65      },
 {       857*OVERSAMPLENR        ,       60      },
 {       881*OVERSAMPLENR        ,       55      },
 {       903*OVERSAMPLENR        ,       50      },
 {       922*OVERSAMPLENR        ,       45      },
 {       939*OVERSAMPLENR        ,       40      },
 {       954*OVERSAMPLENR        ,       35      },
 {       966*OVERSAMPLENR        ,       30      },
 {       977*OVERSAMPLENR        ,       25      },
 {       985*OVERSAMPLENR        ,       20      },
 {       993*OVERSAMPLENR        ,       15      },
 {       999*OVERSAMPLENR        ,       10      },
 {       1004*OVERSAMPLENR       ,       5       },
 {       1008*OVERSAMPLENR       ,       0       } //safety
 };
#endif
#if (THERMISTORHEATER_0 == 2) || (THERMISTORHEATER_1 == 2) || (THERMISTORHEATER_2 == 2) || (THERMISTORBED == 2) //200k bed thermistor
const short temptable_2[][2] PROGMEM = {
//200k ATC Semitec 204GT-2
//Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
   {1*OVERSAMPLENR, 848},
   {30*OVERSAMPLENR, 300}, //top rating 300C
   {34*OVERSAMPLENR, 290},
   {39*OVERSAMPLENR, 280},
   {46*OVERSAMPLENR, 270},
   {53*OVERSAMPLENR, 260},
   {63*OVERSAMPLENR, 250},
   {74*OVERSAMPLENR, 240},
   {87*OVERSAMPLENR, 230},
   {104*OVERSAMPLENR, 220},
   {124*OVERSAMPLENR, 210},
   {148*OVERSAMPLENR, 200},
   {176*OVERSAMPLENR, 190},
   {211*OVERSAMPLENR, 180},
   {252*OVERSAMPLENR, 170},
   {301*OVERSAMPLENR, 160},
   {357*OVERSAMPLENR, 150},
   {420*OVERSAMPLENR, 140},
   {489*OVERSAMPLENR, 130},
   {562*OVERSAMPLENR, 120},
   {636*OVERSAMPLENR, 110},
   {708*OVERSAMPLENR, 100},
   {775*OVERSAMPLENR, 90},
   {835*OVERSAMPLENR, 80},
   {884*OVERSAMPLENR, 70},
   {924*OVERSAMPLENR, 60},
   {955*OVERSAMPLENR, 50},
   {977*OVERSAMPLENR, 40},
   {993*OVERSAMPLENR, 30},
   {1004*OVERSAMPLENR, 20},
   {1012*OVERSAMPLENR, 10},
   {1016*OVERSAMPLENR, 0},
};

#endif
#if (THERMISTORHEATER_0 == 3) || (THERMISTORHEATER_1 == 3) || (THERMISTORHEATER_2 == 3) || (THERMISTORBED == 3) //mendel-parts
const short temptable_3[][2] PROGMEM = {
                {1*OVERSAMPLENR,864},
                {21*OVERSAMPLENR,300},
                {25*OVERSAMPLENR,290},
                {29*OVERSAMPLENR,280},
                {33*OVERSAMPLENR,270},
                {39*OVERSAMPLENR,260},
                {46*OVERSAMPLENR,250},
                {54*OVERSAMPLENR,240},
                {64*OVERSAMPLENR,230},
                {75*OVERSAMPLENR,220},
                {90*OVERSAMPLENR,210},
                {107*OVERSAMPLENR,200},
                {128*OVERSAMPLENR,190},
                {154*OVERSAMPLENR,180},
                {184*OVERSAMPLENR,170},
                {221*OVERSAMPLENR,160},
                {265*OVERSAMPLENR,150},
                {316*OVERSAMPLENR,140},
                {375*OVERSAMPLENR,130},
                {441*OVERSAMPLENR,120},
                {513*OVERSAMPLENR,110},
                {588*OVERSAMPLENR,100},
                {734*OVERSAMPLENR,80},
                {856*OVERSAMPLENR,60},
                {938*OVERSAMPLENR,40},
                {986*OVERSAMPLENR,20},
                {1008*OVERSAMPLENR,0},
                {1018*OVERSAMPLENR,-20}
        };

#endif
#if (THERMISTORHEATER_0 == 4) || (THERMISTORHEATER_1 == 4) || (THERMISTORHEATER_2 == 4) || (THERMISTORBED == 4) //10k thermistor
const short temptable_4[][2] PROGMEM = {
   {1*OVERSAMPLENR, 430},
   {54*OVERSAMPLENR, 137},
   {107*OVERSAMPLENR, 107},
   {160*OVERSAMPLENR, 91},
   {213*OVERSAMPLENR, 80},
   {266*OVERSAMPLENR, 71},
   {319*OVERSAMPLENR, 64},
   {372*OVERSAMPLENR, 57},
   {425*OVERSAMPLENR, 51},
   {478*OVERSAMPLENR, 46},
   {531*OVERSAMPLENR, 41},
   {584*OVERSAMPLENR, 35},
   {637*OVERSAMPLENR, 30},
   {690*OVERSAMPLENR, 25},
   {743*OVERSAMPLENR, 20},
   {796*OVERSAMPLENR, 14},
   {849*OVERSAMPLENR, 7},
   {902*OVERSAMPLENR, 0},
   {955*OVERSAMPLENR, -11},
   {1008*OVERSAMPLENR, -35}
};
#endif

#if (THERMISTORHEATER_0 == 5) || (THERMISTORHEATER_1 == 5) || (THERMISTORHEATER_2 == 5) || (THERMISTORBED == 5) //100k ParCan thermistor (104GT-2)
const short temptable_5[][2] PROGMEM = {
// ATC Semitec 104GT-2 (Used in ParCan)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
   {1*OVERSAMPLENR, 713},
   {17*OVERSAMPLENR, 300}, //top rating 300C
   {20*OVERSAMPLENR, 290},
   {23*OVERSAMPLENR, 280},
   {27*OVERSAMPLENR, 270},
   {31*OVERSAMPLENR, 260},
   {37*OVERSAMPLENR, 250},
   {43*OVERSAMPLENR, 240},
   {51*OVERSAMPLENR, 230},
   {61*OVERSAMPLENR, 220},
   {73*OVERSAMPLENR, 210},
   {87*OVERSAMPLENR, 200},
   {106*OVERSAMPLENR, 190},
   {128*OVERSAMPLENR, 180},
   {155*OVERSAMPLENR, 170},
   {189*OVERSAMPLENR, 160},
   {230*OVERSAMPLENR, 150},
   {278*OVERSAMPLENR, 140},
   {336*OVERSAMPLENR, 130},
   {402*OVERSAMPLENR, 120},
   {476*OVERSAMPLENR, 110},
   {554*OVERSAMPLENR, 100},
   {635*OVERSAMPLENR, 90},
   {713*OVERSAMPLENR, 80},
   {784*OVERSAMPLENR, 70},
   {846*OVERSAMPLENR, 60},
   {897*OVERSAMPLENR, 50},
   {937*OVERSAMPLENR, 40},
   {966*OVERSAMPLENR, 30},
   {986*OVERSAMPLENR, 20},
   {1000*OVERSAMPLENR, 10},
   {1010*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 6) || (THERMISTORHEATER_1 == 6) || (THERMISTORHEATER_2 == 6) || (THERMISTORBED == 6) // 100k Epcos thermistor
const short temptable_6[][2] PROGMEM = {
   {1*OVERSAMPLENR, 350},
   {28*OVERSAMPLENR, 250}, //top rating 250C
   {31*OVERSAMPLENR, 245},
   {35*OVERSAMPLENR, 240},
   {39*OVERSAMPLENR, 235},
   {42*OVERSAMPLENR, 230},
   {44*OVERSAMPLENR, 225},
   {49*OVERSAMPLENR, 220},
   {53*OVERSAMPLENR, 215},
   {62*OVERSAMPLENR, 210},
   {71*OVERSAMPLENR, 205}, //fitted graphically
   {78*OVERSAMPLENR, 200}, //fitted graphically
   {94*OVERSAMPLENR, 190},
   {102*OVERSAMPLENR, 185},
   {116*OVERSAMPLENR, 170},
   {143*OVERSAMPLENR, 160},
   {183*OVERSAMPLENR, 150},
   {223*OVERSAMPLENR, 140},
   {270*OVERSAMPLENR, 130},
   {318*OVERSAMPLENR, 120},
   {383*OVERSAMPLENR, 110},
   {413*OVERSAMPLENR, 105},
   {439*OVERSAMPLENR, 100},
   {484*OVERSAMPLENR, 95},
   {513*OVERSAMPLENR, 90},
   {607*OVERSAMPLENR, 80},
   {664*OVERSAMPLENR, 70},
   {781*OVERSAMPLENR, 60},
   {810*OVERSAMPLENR, 55},
   {849*OVERSAMPLENR, 50},
   {914*OVERSAMPLENR, 45},
   {914*OVERSAMPLENR, 40},
   {935*OVERSAMPLENR, 35},
   {954*OVERSAMPLENR, 30},
   {970*OVERSAMPLENR, 25},
   {978*OVERSAMPLENR, 22},
   {1008*OVERSAMPLENR, 3}
};
#endif

#if (THERMISTORHEATER_0 == 7) || (THERMISTORHEATER_1 == 7) || (THERMISTORHEATER_2 == 7) || (THERMISTORBED == 7) // 100k Honeywell 135-104LAG-J01
const short temptable_7[][2] PROGMEM = {
   {1*OVERSAMPLENR, 500},
   {46*OVERSAMPLENR, 270}, //top rating 300C
   {50*OVERSAMPLENR, 265},
   {54*OVERSAMPLENR, 260},
   {58*OVERSAMPLENR, 255},
   {62*OVERSAMPLENR, 250},
   {67*OVERSAMPLENR, 245},
   {72*OVERSAMPLENR, 240},
   {79*OVERSAMPLENR, 235},
   {85*OVERSAMPLENR, 230},
   {91*OVERSAMPLENR, 225},
   {99*OVERSAMPLENR, 220},
   {107*OVERSAMPLENR, 215},
   {116*OVERSAMPLENR, 210},
   {126*OVERSAMPLENR, 205},
   {136*OVERSAMPLENR, 200},
   {149*OVERSAMPLENR, 195},
   {160*OVERSAMPLENR, 190},
   {175*OVERSAMPLENR, 185},
   {191*OVERSAMPLENR, 180},
   {209*OVERSAMPLENR, 175},
   {224*OVERSAMPLENR, 170},
   {246*OVERSAMPLENR, 165},
   {267*OVERSAMPLENR, 160},
   {293*OVERSAMPLENR, 155},
   {316*OVERSAMPLENR, 150},
   {340*OVERSAMPLENR, 145},
   {364*OVERSAMPLENR, 140},
   {396*OVERSAMPLENR, 135},
   {425*OVERSAMPLENR, 130},
   {460*OVERSAMPLENR, 125},
   {489*OVERSAMPLENR, 120},
   {526*OVERSAMPLENR, 115},
   {558*OVERSAMPLENR, 110},
   {591*OVERSAMPLENR, 105},
   {628*OVERSAMPLENR, 100},
   {660*OVERSAMPLENR, 95},
   {696*OVERSAMPLENR, 90},
   {733*OVERSAMPLENR, 85},
   {761*OVERSAMPLENR, 80},
   {794*OVERSAMPLENR, 75},
   {819*OVERSAMPLENR, 70},
   {847*OVERSAMPLENR, 65},
   {870*OVERSAMPLENR, 60},
   {892*OVERSAMPLENR, 55},
   {911*OVERSAMPLENR, 50},
   {929*OVERSAMPLENR, 45},
   {944*OVERSAMPLENR, 40},
   {959*OVERSAMPLENR, 35},
   {971*OVERSAMPLENR, 30},
   {981*OVERSAMPLENR, 25},
   {989*OVERSAMPLENR, 20},
   {994*OVERSAMPLENR, 15},
   {1001*OVERSAMPLENR, 10},
   {1005*OVERSAMPLENR, 5}
};
#endif

#if (THERMISTORHEATER_0 == 51) || (THERMISTORHEATER_1 == 51) || (THERMISTORHEATER_2 == 51) || (THERMISTORBED == 51) 
// 100k EPCOS (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee.
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: Twice the resolution and better linearity from 150C to 200C
const short temptable_51[][2] PROGMEM = {
   {1*OVERSAMPLENR, 350},
   {190*OVERSAMPLENR, 250}, //top rating 250C
   {203*OVERSAMPLENR, 245},
   {217*OVERSAMPLENR, 240},
   {232*OVERSAMPLENR, 235},
   {248*OVERSAMPLENR, 230},
   {265*OVERSAMPLENR, 225},
   {283*OVERSAMPLENR, 220},
   {302*OVERSAMPLENR, 215},
   {322*OVERSAMPLENR, 210},
   {344*OVERSAMPLENR, 205},
   {366*OVERSAMPLENR, 200},
   {390*OVERSAMPLENR, 195},
   {415*OVERSAMPLENR, 190},
   {440*OVERSAMPLENR, 185},
   {467*OVERSAMPLENR, 180},
   {494*OVERSAMPLENR, 175},
   {522*OVERSAMPLENR, 170},
   {551*OVERSAMPLENR, 165},
   {580*OVERSAMPLENR, 160},
   {609*OVERSAMPLENR, 155},
   {638*OVERSAMPLENR, 150},
   {666*OVERSAMPLENR, 145},
   {695*OVERSAMPLENR, 140},
   {722*OVERSAMPLENR, 135},
   {749*OVERSAMPLENR, 130},
   {775*OVERSAMPLENR, 125},
   {800*OVERSAMPLENR, 120},
   {823*OVERSAMPLENR, 115},
   {845*OVERSAMPLENR, 110},
   {865*OVERSAMPLENR, 105},
   {884*OVERSAMPLENR, 100},
   {901*OVERSAMPLENR, 95},
   {917*OVERSAMPLENR, 90},
   {932*OVERSAMPLENR, 85},
   {944*OVERSAMPLENR, 80},
   {956*OVERSAMPLENR, 75},
   {966*OVERSAMPLENR, 70},
   {975*OVERSAMPLENR, 65},
   {982*OVERSAMPLENR, 60},
   {989*OVERSAMPLENR, 55},
   {995*OVERSAMPLENR, 50},
   {1000*OVERSAMPLENR, 45},
   {1004*OVERSAMPLENR, 40},
   {1007*OVERSAMPLENR, 35},
   {1010*OVERSAMPLENR, 30},
   {1013*OVERSAMPLENR, 25},
   {1015*OVERSAMPLENR, 20},
   {1017*OVERSAMPLENR, 15},
   {1018*OVERSAMPLENR, 10},
   {1019*OVERSAMPLENR, 5},
   {1020*OVERSAMPLENR, 0},
   {1021*OVERSAMPLENR, -5}
};
#endif

#if (THERMISTORHEATER_0 == 52) || (THERMISTORHEATER_1 == 52) || (THERMISTORHEATER_2 == 52) || (THERMISTORBED == 52) 
// 200k ATC Semitec 204GT-2 (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_52[][2] PROGMEM = {
   {1*OVERSAMPLENR, 500},
   {125*OVERSAMPLENR, 300}, //top rating 300C
   {142*OVERSAMPLENR, 290},
   {162*OVERSAMPLENR, 280},
   {185*OVERSAMPLENR, 270},
   {211*OVERSAMPLENR, 260},
   {240*OVERSAMPLENR, 250},
   {274*OVERSAMPLENR, 240},
   {312*OVERSAMPLENR, 230},
   {355*OVERSAMPLENR, 220},
   {401*OVERSAMPLENR, 210},
   {452*OVERSAMPLENR, 200},
   {506*OVERSAMPLENR, 190},
   {563*OVERSAMPLENR, 180},
   {620*OVERSAMPLENR, 170},
   {677*OVERSAMPLENR, 160},
   {732*OVERSAMPLENR, 150},
   {783*OVERSAMPLENR, 140},
   {830*OVERSAMPLENR, 130},
   {871*OVERSAMPLENR, 120},
   {906*OVERSAMPLENR, 110},
   {935*OVERSAMPLENR, 100},
   {958*OVERSAMPLENR, 90},
   {976*OVERSAMPLENR, 80},
   {990*OVERSAMPLENR, 70},
   {1000*OVERSAMPLENR, 60},
   {1008*OVERSAMPLENR, 50},
   {1013*OVERSAMPLENR, 40},
   {1017*OVERSAMPLENR, 30},
   {1019*OVERSAMPLENR, 20},
   {1021*OVERSAMPLENR, 10},
   {1022*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 55) || (THERMISTORHEATER_1 == 55) || (THERMISTORHEATER_2 == 55) || (THERMISTORBED == 55) 
// 100k ATC Semitec 104GT-2 (Used on ParCan) (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_55[][2] PROGMEM = {
   {1*OVERSAMPLENR, 500},
   {76*OVERSAMPLENR, 300},
   {87*OVERSAMPLENR, 290},
   {100*OVERSAMPLENR, 280},
   {114*OVERSAMPLENR, 270},
   {131*OVERSAMPLENR, 260},
   {152*OVERSAMPLENR, 250},
   {175*OVERSAMPLENR, 240},
   {202*OVERSAMPLENR, 230},
   {234*OVERSAMPLENR, 220},
   {271*OVERSAMPLENR, 210},
   {312*OVERSAMPLENR, 200},
   {359*OVERSAMPLENR, 190},
   {411*OVERSAMPLENR, 180},
   {467*OVERSAMPLENR, 170},
   {527*OVERSAMPLENR, 160},
   {590*OVERSAMPLENR, 150},
   {652*OVERSAMPLENR, 140},
   {713*OVERSAMPLENR, 130},
   {770*OVERSAMPLENR, 120},
   {822*OVERSAMPLENR, 110},
   {867*OVERSAMPLENR, 100},
   {905*OVERSAMPLENR, 90},
   {936*OVERSAMPLENR, 80},
   {961*OVERSAMPLENR, 70},
   {979*OVERSAMPLENR, 60},
   {993*OVERSAMPLENR, 50},
   {1003*OVERSAMPLENR, 40},
   {1010*OVERSAMPLENR, 30},
   {1015*OVERSAMPLENR, 20},
   {1018*OVERSAMPLENR, 10},
   {1020*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 60) || (THERMISTORHEATER_1 == 60) || (THERMISTORHEATER_2 == 60) || (THERMISTORBED == 60) 
// 100k Epcos thermistor - Leapfrog Test Table
const short temptable_60[][2] PROGMEM = {
    {  149 ,  400 },
    {  157 ,  395 },
    {  166 ,  390 },
    {  175 ,  385 },
    {  185 ,  380 },
    {  195 ,  375 },
    {  206 ,  370 },
    {  218 ,  365 },
    {  231 ,  360 },
    {  245 ,  355 },
    {  260 ,  350 },
    {  276 ,  345 },
    {  293 ,  340 },
    {  312 ,  335 },
    {  332 ,  330 },
    {  353 ,  325 },
    {  377 ,  320 },
    {  402 ,  315 },
    {  429 ,  310 },
    {  459 ,  305 },
    {  491 ,  300 },
    {  526 ,  295 },
    {  563 ,  290 },
    {  604 ,  285 },
    {  649 ,  280 },
    {  697 ,  275 },
    {  750 ,  270 },
    {  807 ,  265 },
    {  870 ,  260 },
    {  938 ,  255 },
    {  1013 ,  250 },
    {  1094 ,  245 },
    {  1183 ,  240 },
    {  1281 ,  235 },
    {  1387 ,  230 },
    {  1503 ,  225 },
    {  1630 ,  220 },
    {  1769 ,  215 },
    {  1921 ,  210 },
    {  2088 ,  205 },
    {  2269 ,  200 },
    {  2467 ,  195 },
    {  2683 ,  190 },
    {  2918 ,  185 },
    {  3173 ,  180 },
    {  3450 ,  175 },
    {  3750 ,  170 },
    {  4074 ,  165 },
    {  4422 ,  160 },
    {  4795 ,  155 },
    {  5194 ,  150 },
    {  5618 ,  145 },
    {  6066 ,  140 },
    {  6536 ,  135 },
    {  7028 ,  130 },
    {  7538 ,  125 },
    {  8063 ,  120 },
    {  8600 ,  115 },
    {  9143 ,  110 },
    {  9689 ,  105 },
    {  10231 ,  100 },
    {  10766 ,  95 },
    {  11287 ,  90 },
    {  11790 ,  85 },
    {  12271 ,  80 },
    {  12725 ,  75 },
    {  13151 ,  70 },
    {  13547 ,  65 },
    {  13909 ,  60 },
    {  14240 ,  55 },
    {  14537 ,  50 },
    {  14803 ,  45 },
    {  15039 ,  40 },
    {  15246 ,  35 },
    {  15426 ,  30 },
    {  15583 ,  25 },
    {  15717 ,  20 },
    {  15831 ,  15 },
    {  15928 ,  10 },
    {  16010 ,  5 },
    {  16078 ,  0 },
};
#endif 

#define _TT_NAME(_N) temptable_ ## _N
#define TT_NAME(_N) _TT_NAME(_N)

#ifdef THERMISTORHEATER_0
  #define heater_0_temptable TT_NAME(THERMISTORHEATER_0)
  #define heater_0_temptable_len (sizeof(heater_0_temptable)/sizeof(*heater_0_temptable))
#else
#ifdef HEATER_0_USES_THERMISTOR
  #error No heater 0 thermistor table specified
#else  // HEATER_0_USES_THERMISTOR
  #define heater_0_temptable 0
  #define heater_0_temptable_len 0
#endif // HEATER_0_USES_THERMISTOR
#endif

#ifdef THERMISTORHEATER_1
  #define heater_1_temptable TT_NAME(THERMISTORHEATER_1)
  #define heater_1_temptable_len (sizeof(heater_1_temptable)/sizeof(*heater_1_temptable))
#else
#ifdef HEATER_1_USES_THERMISTOR
  #error No heater 1 thermistor table specified
#else  // HEATER_1_USES_THERMISTOR
  #define heater_1_temptable 0
  #define heater_1_temptable_len 0
#endif // HEATER_1_USES_THERMISTOR
#endif

#ifdef THERMISTORHEATER_2
  #define heater_2_temptable TT_NAME(THERMISTORHEATER_2)
  #define heater_2_temptable_len (sizeof(heater_2_temptable)/sizeof(*heater_2_temptable))
#else
#ifdef HEATER_2_USES_THERMISTOR
  #error No heater 2 thermistor table specified
#else  // HEATER_2_USES_THERMISTOR
  #define heater_2_temptable 0
  #define heater_2_temptable_len 0
#endif // HEATER_2_USES_THERMISTOR
#endif

#ifdef THERMISTORBED
  #define bedtemptable TT_NAME(THERMISTORBED)
  #define bedtemptable_len (sizeof(bedtemptable)/sizeof(*bedtemptable))
#else
#ifdef BED_USES_THERMISTOR
  #error No bed thermistor table specified
#endif // BED_USES_THERMISTOR
#endif

#endif //THERMISTORTABLES_H_

