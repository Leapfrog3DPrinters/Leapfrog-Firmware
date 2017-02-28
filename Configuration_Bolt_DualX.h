//===========================================================================
//=============================Mechanical Settings===========================
//===========================================================================
// This defines the number of extruders
#define EXTRUDERS 2

// Define if there are two x-steppers, each driving its own carriage
#define DUAL_X

// In mm that needs to be taken from the second extruder to the first extruder
// See M50 Gcode commando for more info 
#define EXTR2_X_OFFSET 35.0  // Actual offset + parking distance
#define EXTR2_Y_OFFSET 0.0      // Was -1.25

// corse Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors

#ifndef ENDSTOPPULLUPS
  // fine Enstop settings: Individual Pullups. will be ignord if ENDSTOPPULLUPS is defined
  #define ENDSTOPPULLUP_X0MAX
  #define ENDSTOPPULLUP_X1MAX
  #define ENDSTOPPULLUP_YMAX
  #define ENDSTOPPULLUP_ZMAX
  #define ENDSTOPPULLUP_X0MIN
  #define ENDSTOPPULLUP_X0MIN
  #define ENDSTOPPULLUP_YMIN
  #define ENDSTOPPULLUP_ZMIN
#endif

#ifdef ENDSTOPPULLUPS
  #define ENDSTOPPULLUP_X0MAX
  #define ENDSTOPPULLUP_X1MAX
  #define ENDSTOPPULLUP_YMAX
  #define ENDSTOPPULLUP_ZMAX
  #define ENDSTOPPULLUP_X0MIN
  #define ENDSTOPPULLUP_X1MIN
  #define ENDSTOPPULLUP_YMIN
  #define ENDSTOPPULLUP_ZMIN
#endif


#define ENDSTOPPULLUP_ZMIN
// The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
const bool X0_ENDSTOPS_INVERTING = false; // set to true to invert the logic of the endstops.
const bool X1_ENDSTOPS_INVERTING = false; // set to true to invert the logic of the endstops.
const bool Y_ENDSTOPS_INVERTING = false; // set to true to invert the logic of the endstops.
const bool Z_ENDSTOPS_INVERTING = true; // set to true to invert the logic of the endstops.
//#define DISABLE_MAX_ENDSTOPS

// For Inverting Stepper Enable Pins (Active Low) use 0, Non Inverting (Active High) use 1
#define X0_ENABLE_ON 0
#define X1_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0 // For all extruders

// Disables axis when it's not being used.
#define DISABLE_X0 false
#define DISABLE_X1 false
#define DISABLE_Y false
#define DISABLE_Z false // prevented stripping
#define DISABLE_E true // there are some new funtions in the new version of Merlin

#define INVERT_X0_DIR false    // for Creatr PRO set to true
#define INVERT_X1_DIR false    // for Creatr PRO set to false
#define INVERT_Y_DIR false    // for Creatr HS set to false
#define INVERT_Z_DIR false    // for Creatr Pro set to false
#define INVERT_E0_DIR false   // fo BOLT! single to true
#define INVERT_E1_DIR true    // for direct drive extruder v9 set to true, for geared extruder set to false
#define INVERT_E2_DIR true   // for direct drive extruder v9 set to true, for geared extruder set to false

// ENDSTOP SETTINGS:
// Sets direction of endstops when homing; 1=MAX, -1=MIN
// Make sure to swap pins in pins.h for single config!
#define X_HOME_DIR 1 // For dual config: tool0 home to right, For single config: tool0 home to left (-1)
#define X1_HOME_DIR -1 
#define Y_HOME_DIR 1 // Creatr PRO set to 1
#define Z_HOME_DIR -1

#define min_software_endstops true //If true, axis won't move to coordinates less than HOME_POS.
#define max_software_endstops true  //If true, axis won't move to coordinates greater than the defined lengths below.

// Travel limits after homing
#define X_MIN_POS -37.0
#define Y_MIN_POS -33.0  //0 original
#define Z_MIN_POS 0.0
#define X_MAX_POS 330.0     // 360.2 original
#define Y_MAX_POS 322.0     //350 original; 5 mm removed for safety
#define Z_MAX_POS 205.0

// The position of the homing switches. Use MAX_LENGTH * -0.5 if the center should be 0, 0, 0
#define X_HOME_POS X_MAX_POS // Switch to MIN_POS for single config
#define Y_HOME_POS Y_MAX_POS  //350 original
#define Z_HOME_POS Z_MIN_POS

#define DEFAULT_PARK_OFFSET 1.0

#define X0_PARK_POS X_MAX_POS - DEFAULT_PARK_OFFSET
#define X1_PARK_POS X_MIN_POS + DEFAULT_PARK_OFFSET

// Max length 
#define X_MAX_LENGTH (X_MAX_POS - X_MIN_POS)
#define Y_MAX_LENGTH (Y_MAX_POS - Y_MIN_POS)
#define Z_MAX_LENGTH (Z_MAX_POS - Z_MIN_POS)

// Offset of the extruders (uncomment if using more than one and relying on firmware to position when changing).
// The offset has to be X=0, Y=0 for the extruder 0 hotend (default extruder).
// For the other hotends it is their distance from the extruder 0 hotend.
#define EXTRUDER_OFFSET_X {0.0, -EXTR2_X_OFFSET} // (in mm) for each extruder, offset of the hotend on the X axis
#define EXTRUDER_OFFSET_Y {0.0, -EXTR2_Y_OFFSET}  // (in mm) for each extruder, offset of the hotend on the Y axis
