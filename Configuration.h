#ifndef CONFIGURATION_H
  #define CONFIGURATION_H

  // This configurtion file contains the basic settings.
  // Advanced settings can be found in Configuration_adv.h
  // BASIC SETTINGS: select your board type, temperature sensor type, axis scaling, and endstop configuration

  //User specified version info of THIS file to display in [Pronterface, etc] terminal window during startup.
  //Implementation of an idea by Prof Braino to inform user that any changes made
  //to THIS file by the user have been successfully uploaded into firmware.
  #define STRING_VERSION_CONFIG_H __DATE__ " " __TIME__ //Personal revision number for changes to THIS file.
  #define STRING_CONFIG_H_AUTHOR "Leapfrog 3D Printers" //Who made the changes.

  #define LEAPFROG_LMC_VERSION "LMC v3"
  #define LEAPFROG_FIRMWARE_VERSION "3.1.0"

  #define LEAPFROG_MODEL "BoltPro2" //this is actually built on bolt firmware

  // This determines the communication speed of the printer
  #define BAUDRATE 250000

  // The following define selects which electronics board you have. Please choose the one that matches your setup
  #ifndef MOTHERBOARD
  #define MOTHERBOARD 1
  #endif

  //===========================================================================
  //=============================Thermal Settings  ============================
  //===========================================================================
  //
  // 1 is 100k thermistor - best choice for EPCOS 100k (4.7k pullup)
  // 60 is 100k Thermistor with adapted table by Erik Heidstra

  #define TEMP_SENSOR_0 1      // Use detection mechanism: PT-100 / thermistor
  #define TEMP_SENSOR_1 1      // Use detection mechanism: PT-100 / thermistor
  #define TEMP_SENSOR_2 0
  #define TEMP_SENSOR_BED 1
  // #define USE_RTD             // Define when using RTD instead of thermistor
  
  // If defined, use PT100 sensor with adapted table by Maurits Hartman (if it is detected)
  #define HEATER_0_USES_DETECTION
  #define HEATER_1_USES_DETECTION

  // Actual temperature must be close to target for this long before M109 returns success
  #define TEMP_RESIDENCY_TIME 10	// (seconds)
  #define TEMP_HYSTERESIS 3       // (degC) range of +/- temperatures considered "close" to the target one
  #define TEMP_WINDOW     1       // (degC) Window around target to start the recidency timer x degC early.

// Heating checks. If enabled checks if hot end is actually increasing in temperature when heating is on
  #define ENABLE_HEATER_CHECK false
  #define COOLING_WARNING_DELTA 10.0 // (degC) if temperature remains below target-delta, heating is disabled for safety 
  #define HEATING_WAIT_FOR_CHECK 42 // (seconds) wait before checking if temperature is increasing
  #define HEATING_CHECK_INTERVAL 5 // (seconds) interval between each temperature delta check when heater is full on
  #define HEATING_CHECK_POWER 63 // (0-127) Heater power for which to check temperature increase 
  #define COOLING_CHECK_INTERVAL 5 // (seconds) interval between each temperature delta check when temperature should be constant

  // The minimal temperature defines the temperature below which the heater will not be enabled It is used
  // to check that the wiring to the thermistor is not broken.
  // Otherwise this would lead to the heater being powered on all the time.
  #define HEATER_0_MINTEMP 5
  #define HEATER_1_MINTEMP 5
  #define HEATER_2_MINTEMP 5
  #define BED_MINTEMP 5

  // When temperature exceeds max temp, your heater will be switched off.
  // This feature exists to protect your hotend from overheating accidentally, but *NOT* from thermistor short/failure!
  // You should use MINTEMP for thermistor short/failure protection.
  // TODO: variable MAXTEMP based on high and low temp hotends?
  #define HEATER_LOW_TEMP_MAX 275
  #define HEATER_HIGH_TEMP_MAX 400
  #define BED_MAXTEMP 100

  #define USE_BED_PWM


  // PID settings:
  // Comment the following line to disable PID and enable bang-bang.
  #define PIDTEMP
  #define PID_MAX 255 // limits current to nozzle; 255=full current
  #ifdef PIDTEMP
    //#define PID_DEBUG // Sends debug data to the serial port.
    //#define PID_OPENLOOP 1 // Puts PID in open loop. M104 sets the output power in %
    #define PID_INTEGRAL_DRIVE_MAX 255  //limit for the integral term
    #define K1 0.95 //smoothing factor withing the PID
    #define PID_dT ((16.0 * 8.0)/(F_CPU / 64.0 / 256.0)) //sampling period of the

	  // If you are using a preconfigured hotend then you can use one of the value sets by uncommenting it
	  // Creatr HS Old
	  //    #define  DEFAULT_Kp 12.68
	  //    #define  DEFAULT_Ki 0.64
	  //    #define  DEFAULT_KiEFAULT_Kd 62.75
	  // Creatr HS
      #define  DEFAULT_Kp 12.00
      #define  DEFAULT_Ki 0.20
      #define  DEFAULT_Kd 110.0


  #endif // PIDTEMP

	#ifdef USE_BED_PWM
	  #define BED_PID_THRESHOLD 4 // Temperature error (target-current) at which PID control becomes active. On/off control is used outside this range
	  #define BED_K1 0.95 //smoothing factor withing the PID
	  #define DEFAULT_BED_Kp 72.79
	  #define DEFAULT_BED_Ki 6.17
	  #define DEFAULT_BED_Kd 800.0
	#endif //USE_BED_PWM

  //this prevents dangerous Extruder moves, i.e. if the temperature is under the limit
  //can be software-disabled for whatever purposes by
  #define PREVENT_DANGEROUS_EXTRUDE
  //if PREVENT_DANGEROUS_EXTRUDE is on, you can still disable (uncomment) very long bits of extrusion separately.
  #define PREVENT_LENGTHY_EXTRUDE

  #define EXTRUDE_MINTEMP 150
  #define EXTRUDE_MAXLENGTH (X_MAX_LENGTH+Y_MAX_LENGTH) //prevent extrusion of very large distances.

  //===========================================================================
  //=============================Mechanical Settings===========================
  //===========================================================================

  //// MOVEMENT SETTINGS
  #define NUM_AXIS 4 // The axis order in all axis related arrays is X, Y, Z, E
  #define HOMING_FEEDRATE {100*60, 100*60, 600, 0}  // set the homing speeds (mm/min)
  #define PARKING_FEEDRATE 300 * 60

  // default settings
  #define DEFAULT_AXIS_STEPS_PER_UNIT   {(100.0/3.0)*4.0, (160.0/3.0)*2.0, 12000.0/5.0, 99.30*2.0}  //55.465*2 HS Creatr settings for 16 steps all 16 steping
  #define DEFAULT_MAX_FEEDRATE          {400, 400, 20, 200}    // (mm/sec) Creatr was 200, 200, 10, 100, 100
  #define DEFAULT_MAX_ACCELERATION      {400, 400, 100, 30}    // X, Y, Z, E maximum start speed for accelerated moves. E default values are good for skeinforge 40+, for older versions raise them a lot.

  #define DEFAULT_ACCELERATION          400   // X, Y, Z and E max acceleration in mm/s^2 for printing moves
  #define DEFAULT_RETRACT_ACCELERATION  1250   // X, Y, Z and E max acceleration in mm/s^2 for r retracts

  #define DEFAULT_XYJERK                20.0   // (mm/sec)
  #define DEFAULT_ZJERK                 0.4     // (mm/sec)
  #define DEFAULT_EJERK                 5.0    // (mm/sec)

  #define SAFE_MOVES					false // If true, firmware does not allow any move except homing. Also disables G92 and G28 position override

  //===========================================================================
  //=============================Additional Features===========================
  //===========================================================================

  // EEPROM
  // the microcontroller can store settings in the EEPROM, e.g. max velocity...
  // M500 - stores paramters in EEPROM
  // M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
  // M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
  //define this to enable eeprom support
  #define EEPROM_SETTINGS
  //to disable EEPROM Serial responses and decrease program space by ~1700 byte: comment this out:
  // please keep turned on if you can.
  #define EEPROM_CHITCHAT

  // Increase the FAN pwm frequency. Removes the PWM noise but increases heating in the FET/Arduino
  #define FAST_PWM_FAN

  ///////////////////////
  // Auto bed leveling //
  ///////////////////////
  #define ENABLE_ZPROBE
  #ifdef ENABLE_ZPROBE
  #include "math3d.h"
  
  //16 probing coordinates
  static float ZP_COORDS[][3] =
  { 
    /*{-37.0, -33.0, 0.0}, {85.3, -33.0, 0.0}, {207.7, -33.0, 0.0}, {330.0, -33.0, 0.0},
    {-37.0,  85.3, 0.0}, {85.3,  85.3, 0.0}, {207.7,  85.3, 0.0}, {330.0,  85.3, 0.0},
    {-37.0, 203.7, 0.0}, {85.3, 203.7, 0.0}, {207.7, 203.7, 0.0}, {330.0, 203.7, 0.0},
    {-37.0, 322.0, 0.0}, {85.3, 322.0, 0.0}, {207.7, 322.0, 0.0}, {330.0, 322.0, 0.0}*/
    
    //safe ranges for testing
    /*{0.0, 0.0, 0.0}, {85.3, 0.0, 0.0}, {207.7, 0.0, 0.0}, {300.0, 0.0, 0.0},
    {0.0,  85.3, 0.0}, {85.3,  85.3, 0.0}, {207.7,  85.3, 0.0}, {300.0,  85.3, 0.0},
    {0.0, 203.7, 0.0}, {85.3, 203.7, 0.0}, {207.7, 203.7, 0.0}, {300.0, 203.7, 0.0},
    {0.0, 300.0, 0.0}, {85.3, 300.0, 0.0}, {207.7, 300.0, 0.0}, {300.0, 300.0, 0.0}*/
    
    {15.0, 20.0, 0.0},  //left front
    {300.0, 20.0, 0.0},  //right front
    {15.0, 300.0, 0.0},  //left back
    {300.0, 300.0, 0.0},   //right back
    {175.5, 250.0, 0.0},  //mid back
    (175.5, 160.0, 0.0)  //mid mid
  };
 
  static Vector3d xbasis(1,0,0);
  static Vector3d ybasis(0,1,0);
  static Vector3d zbasis(0,0,1);
 
  #endif
  
  #include "Configuration_Bolt_DualX.h"
  #include "Configuration_adv.h"
  #include "thermistortables.h"
  #include "pt100tables.h"

#endif //__CONFIGURATION_H
