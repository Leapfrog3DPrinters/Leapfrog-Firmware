#ifndef PINS_H
#define PINS_H

#if MOTHERBOARD == 1  // 
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif

// Single config or Dual config Right extruder
#define X0_STEP_PIN         28 // 28
#define X0_DIR_PIN          63 // 63
#define X0_ENABLE_PIN       29 // 29
#ifdef DUAL_X // DualX has endstop for X0 on right hand side (max), whereas single has just one endstop on the left hand side (min)
	#define X0_MIN_PIN          -1
	#define X0_MAX_PIN          45   //2 //Max endstops default to disabled "-1", set to commented value to enable.
#else
	#define X0_MIN_PIN          47
	#define X0_MAX_PIN          -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.
#endif
// Only for dual config: Left extruder
#ifdef DUAL_X
	#define X1_STEP_PIN         65   //65
	#define X1_DIR_PIN          64   //64
	#define X1_ENABLE_PIN       66   //66
	#define X1_MIN_PIN          47   //47
	#define X1_MAX_PIN          -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.
#else
	#define X1_STEP_PIN         -1
	#define X1_DIR_PIN          -1
	#define X1_ENABLE_PIN       -1
	#define X1_MIN_PIN          -1
	#define X1_MAX_PIN          -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.
#endif



#define Y_STEP_PIN         14 // A6
#define Y_DIR_PIN          15 // A0
#define Y_ENABLE_PIN       39
#define Y_MIN_PIN          -1 //48
#define Y_MAX_PIN          48 //15  

//CENTER
#define Z_STEP_PIN         31 // A2
#define Z_DIR_PIN          32 // A6
#define Z_ENABLE_PIN       30 // A1
#define Z_MIN_PIN          49 // fixed for HS from 44
#define Z_MAX_PIN          -1

#ifdef ENABLE_ZPROBE
        //all Z motors use pin 30 as enable
        //LEFT
	#define Z2_STEP_PIN         4
	#define Z2_DIR_PIN          44
        //RIGHT
	#define Z3_STEP_PIN         42
	#define Z3_DIR_PIN          19


	//#define PIEZO_PIN           44 //piezo now on Z_MIN_PIN
#endif
	
//EMOT
#define E0_STEP_PIN        34 //34
#define E0_DIR_PIN         35 //35
#define E0_ENABLE_PIN      33 //33

//EMOT2
#define E1_STEP_PIN        37 //37
#define E1_DIR_PIN         40 //40
#define E1_ENABLE_PIN      36 //36

//#define FILAMENT_E0         44 // 3RD pin in connector
//#define FILAMENT_E1         42 // 3RD pin in connector
#define DOOR_PIN            -1

#define LED_PIN            13
#define FAN_PIN             5 // Pin change for Production HS Board HBW
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN       9
#define HEATER_1_PIN       8 // 12 
#define HEATER_2_PIN       -1 //-1 // 13
#define TEMP_0_PIN         13 //D27   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN         15 // 1
#define TEMP_2_PIN         -1 // 2
#define HEATER_BED_PIN     10 // 14/15
#define TEMP_BED_PIN       14 // 1,2 or I2C

/* Head PCB detection pins:
 *
 * Truth table:
 * Scenario					| HEAD_PCB_OLD_x_PIN	| LOW_TEMP_HOTEND_x_PIN
 * -------------------------+-----------------------+----------------------
 * Old head PCB				| HIGH					| HIGH
 * New head PCB, low temp	| LOW					| HIGH
 * New head PCB, high temp	| LOW					| LOW
 * -------------------------+-----------------------+----------------------
 */
#define HEAD_PCB_OLD_0_PIN		25	// PA3 AD3
#define HEAD_PCB_OLD_1_PIN		22	// PA0 AD0
#define LOW_TEMP_HOTEND_0_PIN	26	// PA4 AD4
#define LOW_TEMP_HOTEND_1_PIN	23	// PA1 AD1

/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) 24 (27) 28 (29) (30) (31)  */
#endif

#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN, HEAD_PCB_OLD_0_PIN, LOW_TEMP_HOTEND_0_PIN, 
#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN, HEAD_PCB_OLD_1_PIN, LOW_TEMP_HOTEND_1_PIN, 
#else
  #define _E1_PINS
#endif
#if EXTRUDERS > 2
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN,
#else
  #define _E2_PINS
#endif

#ifdef DISABLE_MAX_ENDSTOPS
#define X_MAX_PIN          -1
#define Y_MAX_PIN          -1
#define Z_MAX_PIN          -1
#endif

#ifdef DUAL_X
	#define SENSITIVE_PINS {0, 1, X0_STEP_PIN, X0_DIR_PIN, X0_ENABLE_PIN, X0_MIN_PIN, X0_MAX_PIN, X1_STEP_PIN, X1_DIR_PIN, X1_ENABLE_PIN, X1_MIN_PIN, X1_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z2_STEP_PIN, Z2_DIR_PIN, Z3_STEP_PIN, Z3_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, LED_PIN, PS_ON_PIN, \
	                        HEATER_BED_PIN, FAN_PIN,                  \
	                        _E0_PINS _E1_PINS _E2_PINS             \
	                        TEMP_0_PIN, TEMP_1_PIN, TEMP_2_PIN, TEMP_BED_PIN, HEAD_PCB_OLD_0_PIN, HEAD_PCB_OLD_1_PIN, LOW_TEMP_HOTEND_0_PIN, LOW_TEMP_HOTEND_1_PIN }
#else
    #define SENSITIVE_PINS {0, 1, X0_STEP_PIN, X0_DIR_PIN, X0_ENABLE_PIN, X0_MIN_PIN, X0_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z2_STEP_PIN, Z2_DIR_PIN, Z3_STEP_PIN, Z3_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, LED_PIN, PS_ON_PIN, \
						    HEATER_BED_PIN, FAN_PIN,                  \
						    _E0_PINS _E1_PINS _E2_PINS             \
						    TEMP_0_PIN, TEMP_1_PIN, TEMP_2_PIN, TEMP_BED_PIN, HEAD_PCB_OLD_0_PIN, HEAD_PCB_OLD_1_PIN, LOW_TEMP_HOTEND_0_PIN, LOW_TEMP_HOTEND_1_PIN }
#endif

#endif
