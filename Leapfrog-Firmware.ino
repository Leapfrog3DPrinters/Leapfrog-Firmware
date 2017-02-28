/* -*- c++ -*- */

/*
	Reprap firmware based on Sprinter and grbl.
 Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /*
  This firmware is a mashup between Sprinter and grbl.
   (https://github.com/kliment/Sprinter)
   (https://github.com/simen/grbl/tree)

  It has preliminary support for Matthew Roberts advance algorithm
	 http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
 */

#include "Marlin.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "EEPROMwrite.h"
#include "language.h"
#include "pins_arduino.h"

#define VERSION_STRING  "1.0.0"


 //Implemented Codes
 //-------------------G
 // G0  -> G1
 // G1  - Coordinated Movement X Y Z E
 // G4  - Dwell S<seconds> or P<milliseconds>
 // G10 - retract filament according to settings of M207
 // G11 - retract recover filament according to settings of M208
 // G28 - Home all Axis
 // G29 - Test if Zprobe solenoid is working
 // G32 - level printing bed
 // G40 - Print amount of steps missed since last reset
 // G90 - Use Absolute Coordinates
 // G91 - Use Relative Coordinates
 // G92 - Set current position to cordinates given

 // M Codes
 // M104 - Set extruder target temp
 // M105 - Read current temp
 // M106 - Fan on
 // M107 - Fan off
 // M109 - Wait for extruder current temp to reach target temp.
 // M140 - Set bed target temp
 // M190 - Wait for bed current temp to reach target temp.
 // M114 - Display current position

 //Custom M Codes
 // M17  - Enable/Power all stepper motors
 // M18  - Disable all stepper motors; same as M84
 // M42  - Change pin status via gcode
 // M50  - Set Extruder 2 Offset. Does NOT reset with firmware reset M502. Works in DualX mode.
 // M80  - Turn on Power Supply
 // M81  - Turn off Power Supply
 // M82  - Set E codes absolute (default)
 // M83  - Set E codes relative while in Absolute Coordinates (G90) mode
 // M84  - Disable steppers until next move,
 //        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
 // M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 // M92  - Set axis_steps_per_unit - same syntax as G92
 // M114 - Output current position to serial port
 // M119 - Output Endstop status to serial port
 // M200 - Set filament diameter
 // M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 // M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
 // M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
 // M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
 // M206 - set additional homeing offset
 // M207 - set T0 maximum temperature. Syntax: M207 S<desired max temp> T<desired tool>
 // M220 - S<factor in percent> set speed factor override percentage (M220 S110)
 // M221 - S<factor in percent> set extrude factor override percentage (M221 S75)
 // M301 - Set PID parameters P I and D
 // M302 - Allow cold extrudes
 // M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
 // M304 - Set Bed PID parameters P I and D
 // M400 - Finish all moves
 // M500 - stores parameters in EEPROM
 // M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
 // M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
 // M503 - print the current settings (from memory not from eeprom)
 // M601 - Set calibration offsets (X, Y, Z)
 // M605 - Synchronous mode - Syncs all control (movement and heating) of T0 with T1. Use toolswitch T0/T1, M605 S1, or G28 to disable.
 // M606 - Set parking position
 // M999 - Restart after being stopped by error

 //Stepper Movement Variables

 /* ======== TO DO: ADD NOMENCLATURE OF IMPORTANT VARIABLES   ================  */

 //===========================================================================
 //=============================imported variables============================
 //===========================================================================


 //===========================================================================
 //=============================public variables=============================
 //===========================================================================
float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
volatile int feedmultiply = 100; //100->1 1000->2
int saved_feedmultiply;
volatile bool feedmultiplychanged = false;
volatile int extrudemultiply = 100; //100->1 1000->2
float bed_width_correction = 0.0;
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
float add_homeing[3] = { 0.0,0.0,0.0 };
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
#ifdef DUAL_X
float parking_pos[2] = { X0_PARK_POS,X1_PARK_POS }; // Only X-axis, TODO: Store in EEPROM
#endif
uint8_t active_extruder = 0;
unsigned char FanSpeed = 0;
bool door_status = false;
int swap_dir = 1; // Used to swap homing direction for X-axis. 1 = normal, -1 = opposite direction

#if SAFE_MOVES
bool position_known[3] = { false, false, false }; // If current position is unknown, a homing sequence is required (only for X, Y, Z)
#endif

// Extruder offset, only in XY plane
float extruder_offset[2][EXTRUDERS] = {
#if defined(EXTRUDER_OFFSET_X) && defined(EXTRUDER_OFFSET_Y)
  EXTRUDER_OFFSET_X, EXTRUDER_OFFSET_Y
#endif
};


//===========================================================================
//=============================private variables=============================
//===========================================================================
const char axis_codes[NUM_AXIS] = { 'X', 'Y', 'Z', 'E' };
static float destination[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
static float offset = 0.0;
static bool home_all_axis = true;
static float feedrate = 1500.0, next_feedrate, saved_feedrate;
static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

static bool relative_mode = false;  //Determines Absolute or Relative Coordinates
static bool relative_mode_e = false;  //Determines Absolute or Relative E Codes while in Absolute Coordinates mode. E is always relative in Relative Coordinates mode.

static float yoffset = 0.0; // Used for tool-changes to correct for y offset (x is managed differently)

static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
static bool fromsd[BUFSIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

const int sensitive_pins[] = SENSITIVE_PINS; // Sensitive pin list for M42

//Inactivity shutdown variables
static unsigned long previous_millis_cmd = 0;
static unsigned long max_inactive_time = 0;
static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME * 1000l;

static unsigned long starttime = 0;
static unsigned long stoptime = 0;

static uint8_t tmp_extruder;
static uint8_t target_extruder;

static unsigned long readTimeout = 0;
static unsigned long stopAt = 0;
static bool includePwm = false;

#ifdef DUAL_X
static bool is_parked[2] = { false, false };
static float dual_x_offset = 0.0; // Requested x-offset
static float actual_dual_x_offset = 0.0; // Calculated x-offset
static float temp_offset = 0.0; // Requested temperature difference between left and right in sync mode
static bool parking_disabled = false;
static float last_known_x[2] = { X_MAX_POS, X_MIN_POS };
static int dual_x_mode = 1; // 0 = No parking, 1 = auto parking, 2 = sync, 3 = sync mirrored. Defaults to 1
#endif

static float target_temp; // Target temperature when heating. Stored here because may be used twice in DUAL_X mode 
extern int heater_0_maxtemp;
extern int heater_1_maxtemp;
extern float heater_0_offset;
extern float heater_1_offset;

int toolnum;

bool Stopped = false;



//===========================================================================
//=============================ROUTINES=============================
//===========================================================================

bool setTargetedHotend(int code);

void serial_echopair_P(const char *s_P, float v)
{
	serialprintPGM(s_P); SERIAL_ECHO(v);
}
void serial_echopair_P(const char *s_P, double v)
{
	serialprintPGM(s_P); SERIAL_ECHO(v);
}
void serial_echopair_P(const char *s_P, unsigned long v)
{
	serialprintPGM(s_P); SERIAL_ECHO(v);
}

extern "C" {
	extern unsigned int __bss_end;
	extern unsigned int __heap_start;
	extern void *__brkval;

	int freeMemory() {
		int free_memory;

		if ((int)__brkval == 0)
			free_memory = ((int)&free_memory) - ((int)&__bss_end);
		else
			free_memory = ((int)&free_memory) - ((int)__brkval);

		return free_memory;
	}
}

//adds an command to the main command buffer
//thats really done in a non-safe way.
//needs overworking someday
void enquecommand(const char *cmd)
{
	if (buflen < BUFSIZE)
	{
		//this is dangerous if a mixing of serial and this happsens
		strcpy(&(cmdbuffer[bufindw][0]), cmd);
		SERIAL_ECHO_START;
		SERIAL_ECHOPGM("enqueing \"");
		SERIAL_ECHO(cmdbuffer[bufindw]);
		SERIAL_ECHOLNPGM("\"");
		bufindw = (bufindw + 1) % BUFSIZE;
		buflen += 1;
	}
}

void setup_killpin()
{
#if( KILL_PIN>-1 )
	pinMode(KILL_PIN, INPUT);
	WRITE(KILL_PIN, HIGH);
#endif
}

void setup_doorpin()
{
#if defined(DOOR_PIN) && DOOR_PIN > -1
	pinMode(DOOR_PIN, INPUT);
	WRITE(DOOR_PIN, LOW);
	door_status = READ(DOOR_PIN);
#endif
}

void setup_filament_pins()
{
#if defined(FILAMENT_E0_PIN) && FILAMENT_E0_PIN > -1
	pinMode(FILAMENT_E0_PIN, INPUT);
	WRITE(FILAMENT_E0_PIN, HIGH);
#endif
#if defined(FILAMENT_E1_PIN) && FILAMENT_E1_PIN > -1
	pinMode(FILAMENT_E1_PIN, INPUT);
	WRITE(FILAMENT_E1_PIN, HIGH);
#endif
}


void setup_powerhold()
{
#ifdef SUICIDE_PIN
#if (SUICIDE_PIN> -1)
	SET_OUTPUT(SUICIDE_PIN);
	WRITE(SUICIDE_PIN, HIGH);
#endif
#endif
}

void suicide()
{
#ifdef SUICIDE_PIN
#if (SUICIDE_PIN> -1)
	SET_OUTPUT(SUICIDE_PIN);
	WRITE(SUICIDE_PIN, LOW);
#endif
#endif
}


void setup()
{
	setup_killpin();
	setup_powerhold();
	MYSERIAL.begin(BAUDRATE);
	SERIAL_ECHO_START;

	// Check startup - does nothing if bootloader sets MCUSR to 0
	byte mcu = MCUSR;
	if (mcu & 1) SERIAL_ECHOLNPGM(MSG_POWERUP);
	if (mcu & 2) SERIAL_ECHOLNPGM(MSG_EXTERNAL_RESET);
	if (mcu & 4) SERIAL_ECHOLNPGM(MSG_BROWNOUT_RESET);
	if (mcu & 8) SERIAL_ECHOLNPGM(MSG_WATCHDOG_RESET);
	if (mcu & 32) SERIAL_ECHOLNPGM(MSG_SOFTWARE_RESET);
	MCUSR = 0;

	SERIAL_ECHOPGM(MSG_MARLIN);
	SERIAL_ECHOLNPGM(VERSION_STRING);
#ifdef STRING_VERSION_CONFIG_H
#ifdef STRING_CONFIG_H_AUTHOR
	SERIAL_ECHO_START;
	SERIAL_ECHOPGM(MSG_CONFIGURATION_VER);
	SERIAL_ECHOPGM(STRING_VERSION_CONFIG_H);
	SERIAL_ECHOPGM(MSG_AUTHOR);
	SERIAL_ECHOLNPGM(STRING_CONFIG_H_AUTHOR);
#endif
#endif
	SERIAL_ECHO_START;
	SERIAL_ECHOPGM(MSG_FREE_MEMORY);
	SERIAL_ECHO(freeMemory());
	SERIAL_ECHOPGM(MSG_PLANNER_BUFFER_BYTES);
	SERIAL_ECHOLN((int)sizeof(block_t)*BLOCK_BUFFER_SIZE);
	for (int8_t i = 0; i < BUFSIZE; i++)
	{
		fromsd[i] = false;
	}
	EEPROM_RetrieveSettings(); // loads data from EEPROM if available

	for (int8_t i = 0; i < NUM_AXIS; i++)
	{
		axis_steps_per_sqr_second[i] = max_acceleration_units_per_sq_second[i] * axis_steps_per_unit[i];
	}


	tp_init();    // Initialize temperature loop
	plan_init();  // Initialize planner;
	st_init();    // Initialize stepper;
	setup_doorpin();
	setup_filament_pins();
	SERIAL_PROTOCOLLNPGM("start");
}


void loop()
{
	if (buflen < (BUFSIZE - 1))
		get_command();
	if (buflen)
	{
		process_commands();
		buflen = (buflen - 1);
		bufindr = (bufindr + 1) % BUFSIZE;
	}
	//check heater every n milliseconds
	manage_heater();
	manage_inactivity();
	checkHitEndstops();
}

void get_command()
{
	while (MYSERIAL.available() > 0 && buflen < BUFSIZE)
	{
		serial_char = MYSERIAL.read();
		if (serial_char == '\n' ||
			serial_char == '\r' ||
			(serial_char == ':' && comment_mode == false) ||
			serial_count >= (MAX_CMD_SIZE - 1))
		{
			if (!serial_count)
			{ //if empty line
				comment_mode = false; //for new command
				return;
			}
			cmdbuffer[bufindw][serial_count] = 0; //terminate string
			if (!comment_mode)
			{
				comment_mode = false; //for new command
				fromsd[bufindw] = false;
				if (strstr(cmdbuffer[bufindw], "N") != NULL)
				{
					strchr_pointer = strchr(cmdbuffer[bufindw], 'N');
					gcode_N = (strtol(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL, 10));
					if (gcode_N != gcode_LastN + 1 && (strstr(cmdbuffer[bufindw], "M110") == NULL))
					{
						SERIAL_ERROR_START;
						SERIAL_ERRORPGM(MSG_ERR_LINE_NO);
						SERIAL_ERRORLN(gcode_LastN);
						//Serial.println(gcode_N);
						FlushSerialRequestResend();
						serial_count = 0;
						return;
					}
					if (strstr(cmdbuffer[bufindw], "*") != NULL)
					{
						byte checksum = 0;
						byte count = 0;
						while (cmdbuffer[bufindw][count] != '*') checksum = checksum^cmdbuffer[bufindw][count++];
						strchr_pointer = strchr(cmdbuffer[bufindw], '*');

						if ((int)(strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL)) != checksum)
						{
							SERIAL_ERROR_START;
							SERIAL_ERRORPGM(MSG_ERR_CHECKSUM_MISMATCH);
							SERIAL_ERRORLN(gcode_LastN);
							FlushSerialRequestResend();
							serial_count = 0;
							return;
						}
						//if no errors, continue parsing
					}
					else
					{
						SERIAL_ERROR_START;
						SERIAL_ERRORPGM(MSG_ERR_NO_CHECKSUM);
						SERIAL_ERRORLN(gcode_LastN);
						FlushSerialRequestResend();
						serial_count = 0;
						return;
					}
					gcode_LastN = gcode_N;
					//if no errors, continue parsing
				}
				else  // if we don't receive 'N' but still see '*'
				{
					if ((strstr(cmdbuffer[bufindw], "*") != NULL))
					{
						SERIAL_ERROR_START;
						SERIAL_ERRORPGM(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM);
						SERIAL_ERRORLN(gcode_LastN);
						serial_count = 0;
						return;
					}
				}
				if ((strstr(cmdbuffer[bufindw], "G") != NULL))
				{
					strchr_pointer = strchr(cmdbuffer[bufindw], 'G');
					switch ((int)((strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL))))
					{
					case 0:
					case 1:
					case 2:
					case 3:
						if (Stopped == true) {
							SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
						}
						break;
					default:
						break;
					}
				}
				bufindw = (bufindw + 1) % BUFSIZE;
				buflen += 1;
			}
			serial_count = 0; //clear buffer
		}
		else
		{
			if (serial_char == ';') comment_mode = true;
			if (!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char;
		}
	}
}


float code_value()
{
	return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL));
}

long code_value_long()
{
	return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10));
}

bool code_seen(char code_string[]) //Return True if the string was found
{
	return (strstr(cmdbuffer[bufindr], code_string) != NULL);
}

bool code_seen(char code)
{
	strchr_pointer = strchr(cmdbuffer[bufindr], code);
	return (strchr_pointer != NULL);  //Return True if a character was found
}

#define DEFINE_PGM_READ_ANY(type, reader)		\
static inline type pgm_read_any(const type *p)	\
{ return pgm_read_##reader##_near(p); }

DEFINE_PGM_READ_ANY(float, float);
DEFINE_PGM_READ_ANY(signed char, byte);

#define XYZ_CONSTS_FROM_CONFIG(type, array, CONFIG)	\
static const PROGMEM type array##_P[3] =		\
{ X_##CONFIG, Y_##CONFIG, Z_##CONFIG };		\
static inline type array(int axis)			\
{ return pgm_read_any(&array##_P[axis]); } 

XYZ_CONSTS_FROM_CONFIG(float, base_min_pos, MIN_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_max_pos, MAX_POS);
XYZ_CONSTS_FROM_CONFIG(float, base_home_pos, HOME_POS);
XYZ_CONSTS_FROM_CONFIG(float, max_length, MAX_LENGTH);
XYZ_CONSTS_FROM_CONFIG(float, home_retract_mm, HOME_RETRACT_MM);
XYZ_CONSTS_FROM_CONFIG(signed char, home_dir, HOME_DIR);

static inline float bed_width() { return base_max_pos(X_AXIS) + bed_width_correction; }

static void axis_is_at_home(int axis)
{
#ifdef DUAL_X
	if (axis == X_AXIS)
	{
		// TODO: Make more dynamic. It now assumes X0 homes right, X1 homes left.
		if (syncmode_enabled)
		{
			current_position[axis] = X_MIN_POS + add_homeing[axis];
			min_pos[axis] = base_min_pos(axis) + add_homeing[axis];

			if (inverted_x0_mode)
				max_pos[axis] = bed_width() / 2.0 + add_homeing[axis] + extruder_offset[X_AXIS][1] / 2.0;
			else
				max_pos[axis] = bed_width() + add_homeing[axis] - actual_dual_x_offset;
		}
		else if (active_extruder == 1)
		{
			current_position[axis] = base_min_pos(axis) + add_homeing[axis];
			min_pos[axis] = base_min_pos(axis) + add_homeing[axis];
			//max_pos[axis] =          bed_width() + add_homeing[axis] + extruder_offset[X_AXIS][1];
			max_pos[axis] = min(bed_width(), last_known_x[0]) + extruder_offset[X_AXIS][1];
		}
		else
		{
			current_position[axis] = bed_width() + add_homeing[axis];
			//min_pos[axis] =          base_min_pos(axis) + add_homeing[axis] - extruder_offset[X_AXIS][1];
			min_pos[axis] = max(base_min_pos(axis), last_known_x[1]) - extruder_offset[X_AXIS][1];
			max_pos[axis] = bed_width() + add_homeing[axis];
		}
	}
	else
	{
		current_position[axis] = base_home_pos(axis) + add_homeing[axis];
		min_pos[axis] = base_min_pos(axis) + add_homeing[axis];
		max_pos[axis] = bed_width() + add_homeing[axis];
	}
#else
	current_position[axis] = base_home_pos(axis) + add_homeing[axis];
	min_pos[axis] = base_min_pos(axis) + add_homeing[axis];
	max_pos[axis] = bed_width() + add_homeing[axis];
#endif

#if SAFE_MOVES
	position_known[axis] = true;
#endif

}


static void homeaxis(int axis)
{
#define HOMEAXIS_DO(LETTER) \
  ((LETTER##_MIN_PIN > -1 && LETTER##_HOME_DIR==-1) || (LETTER##_MAX_PIN > -1 && LETTER##_HOME_DIR==1))
#define HOMEAXIS_DO_X \ 
	((X0_MIN_PIN > -1 && X_HOME_DIR == -1) || (X0_MAX_PIN > -1 && X_HOME_DIR == 1))

		if (axis == X_AXIS ? HOMEAXIS_DO_X :
			axis == Y_AXIS ? HOMEAXIS_DO(Y) :
			axis == Z_AXIS ? HOMEAXIS_DO(Z) :
			0)
		{
			// For X: homing will only be performed for active_extruder. Determine whether direction needs to be swapped
			//TODO: Save seperate X0 and X1 homing dir in config
#ifdef DUAL_X
			swap_dir = axis == X_AXIS && active_extruder == 1 && X1_HOME_DIR == -1 ? -1 : 1;
			if (axis == X_AXIS) is_parked[active_extruder] = false;
#else
			swap_dir = 1;
#endif
			
			current_position[axis] = 0;
			plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
			destination[axis] = 1.5 * max_length(axis) * home_dir(axis) * swap_dir;
			feedrate = homing_feedrate[axis];
			plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate / 60, active_extruder);
			st_synchronize();

			current_position[axis] = 0;
			plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
			destination[axis] = -home_retract_mm(axis) * home_dir(axis) * swap_dir;
			plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate / 60, active_extruder);
			st_synchronize();


			destination[axis] = 2 * home_retract_mm(axis) * home_dir(axis) * swap_dir;
			feedrate = homing_feedrate[axis] / 2;
			plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate / 60, active_extruder);
			st_synchronize();

			axis_is_at_home(axis);
			destination[axis] = current_position[axis];
			feedrate = 0.0;
			endstops_hit_on_purpose();

#ifdef DUAL_X
			last_known_x[active_extruder] = current_position[X_AXIS];
#endif
		}
}
#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)


// Planner shorthand inline functions
inline void line_to_current_position() {
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
}
inline void line_to_z(float zPosition) {
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], zPosition, current_position[E_AXIS], feedrate / 60, active_extruder);
}

inline void line_to_destination(float mm_m) {
	plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], mm_m / 60, active_extruder);
}

inline void line_to_destination() {
	line_to_destination(feedrate);
}
inline void sync_plan_position() {
	plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
inline void set_current_to_destination() { memcpy(current_position, destination, sizeof(current_position)); }
inline void set_destination_to_current() { memcpy(destination, current_position, sizeof(destination)); }

void process_commands()
{
	unsigned long codenum; //throw away variable
	char *starpos = NULL;
	bool relative_mode_backup;
	float maxAdjust;
	int probeIterations;
	if (code_seen('G'))
	{
		switch ((int)code_value())
		{
		case 0: // G0 -> G1
		case 1: // G1
			if (Stopped == false)
			{
				get_coordinates(); // For X Y Z E F
				prepare_move();
#ifdef FILAMENT_DETECTION
				checkFilamentError();
#endif
				//ClearToSend();
			}
			break;
		case 4: // G4 dwell
			codenum = 0;
			if (code_seen('P')) codenum = code_value(); // milliseconds to wait
			if (code_seen('S')) codenum = code_value() * 1000; // seconds to wait

			st_synchronize();
			codenum += millis();  // keep track of when we started waiting
			previous_millis_cmd = millis();
			while (millis() < codenum) {
				manage_heater();
				manage_inactivity();
			}
			break;
		case 28: //G28 Home all Axis one at a timer
		{
			saved_feedrate = feedrate;
			saved_feedmultiply = feedmultiply;
			feedmultiply = 100;
			previous_millis_cmd = millis();

			enable_endstops(true);

			set_destination_to_current();

			feedrate = 0.0;

			bool  homeX = code_seen(axis_codes[X_AXIS]),
				homeY = code_seen(axis_codes[Y_AXIS]),
				homeZ = code_seen(axis_codes[Z_AXIS]);

			home_all_axis = !(homeX || homeY || homeZ) || (homeX && homeY && homeZ);

			if (home_all_axis || homeX)
			{
				
#ifdef DUAL_X
				homeDualX();
#else
				HOMEAXIS(X);
#endif
			}

			// Home Y
			if (home_all_axis || homeY) HOMEAXIS(Y);

			// Home Z
			if (home_all_axis || homeZ)  HOMEAXIS(Z);

			// Set the X position and add M206
			if (code_seen(axis_codes[X_AXIS])) {
				float v = code_value();
				if (v) current_position[X_AXIS] = v + add_homeing[0];
			}

			// Set the Y position and add M206
			if (code_seen(axis_codes[Y_AXIS])) {
				float v = code_value();
				if (v) current_position[Y_AXIS] = v + add_homeing[1];
			}

			// Set the Z position and add M206
			if (code_seen(axis_codes[Z_AXIS])) {
				float v = code_value();
				if (v) current_position[Z_AXIS] = v + add_homeing[2];
			}

			sync_plan_position();


#ifdef ENDSTOPS_ONLY_FOR_HOMING
			enable_endstops(false);
#endif

			feedrate = saved_feedrate;
			feedmultiply = saved_feedmultiply;
			previous_millis_cmd = millis();

			endstops_hit_on_purpose(); // clear endstop hit flags
			break;
		}
		case 90: // G90
			relative_mode = false;
			break;
		case 91: // G91
			relative_mode = true;
			break;
		case 92: // G92
			if (!code_seen(axis_codes[E_AXIS]))
				st_synchronize();
			for (int8_t i = 0; i < NUM_AXIS; i++)
			{
				if (code_seen(axis_codes[i]))
				{
					if (i == E_AXIS)
					{
						current_position[i] = (float)code_value();
						plan_set_e_position(current_position[E_AXIS]);
					}
					else
					{
						current_position[i] = code_value() + add_homeing[i];
						plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
					}
				}
			}
			break;
		}
	}
	else if (code_seen('M'))
	{
		switch ((int)code_value())
		{
		case 42: //M42 -Change pin status via gcode
			if (code_seen('S'))
			{
				int pin_status = code_value();
				if (code_seen('P') && pin_status >= 0 && pin_status <= 255)
				{
					int pin_number = code_value();
					for (int8_t i = 0; i < (int8_t)sizeof(sensitive_pins); i++)
					{
						if (sensitive_pins[i] == pin_number)
						{
							pin_number = -1;
							break;
						}
					}

					if (pin_number > -1)
					{
						pinMode(pin_number, OUTPUT);
						digitalWrite(pin_number, pin_status);
						analogWrite(pin_number, pin_status);
					}
				}
			}
			break;
		case 605: // M605
		{
#ifndef DUAL_X

			SERIAL_ECHO_START;
			SERIAL_ECHOLN(MSG_NO_DUALX);

#else

			if (code_seen('S'))
				dual_x_mode = code_value();
			else
				dual_x_mode = 1;

			if (code_seen('X'))
				dual_x_offset = code_value();
			else
				dual_x_offset = 0;

			if (code_seen('R'))
				temp_offset = code_value();
			else
				temp_offset = 0;
			
			if (dual_x_mode == 3)
				beginSyncMode(0.0, true);
			else if (dual_x_mode == 2)
				beginSyncMode(dual_x_offset, false);
			else if (dual_x_mode == 1)
			{
				endSyncMode();
				parking_disabled = false;
			}
			else
			{
				endSyncMode();
				parking_disabled = true;
			}
#endif
		}
		break;
		case 606: // M606
		{
#ifndef DUAL_X
			SERIAL_ECHO_START;
			SERIAL_ECHOLN(MSG_NO_DUALX);
#else
			float parking_offset = DEFAULT_PARK_OFFSET;

			if (code_seen('P')) parking_offset = code_value();
			if (code_seen('T'))
			{
				target_extruder = code_value();
				setParkingOffset(target_extruder, parking_offset);
			}
			else
			{
				setParkingOffset(0, parking_offset);
				setParkingOffset(1, parking_offset);
			}
#endif
		}
		break;
	
		case 199:
			//TODO: Remove this

			// Dump status about X
			dumpstatus();
			break;
		case 104: // M104
			if (setTargetedHotend(104))
			{
				break;
			}
			if (code_seen('S')) {
				target_temp = code_value();
				setTargetHotend(target_temp, target_extruder);

#ifdef DUAL_X
				if (syncmode_enabled)
				{
					if (target_temp == 0)
						setTargetHotend(0, 1 - target_extruder);
					else
						setTargetHotend(target_temp + (temp_offset*(target_extruder * 2 - 1)), 1 - target_extruder); // Negative offset if right extruder is targeted
				}
#endif
			}
			setWatch();
			break;
		case 140: // M140 set bed temp
			if (code_seen('S')) setTargetBed(code_value());
			break;
		case 105: // M105
			if (setTargetedHotend(105))
			{
				break;
			}
#if (TEMP_0_PIN > -1)
			SERIAL_PROTOCOLPGM("ok ");
#if TEMP_BED_PIN > -1
			printTemperatures();
#endif //TEMP_BED_PIN
#else
			SERIAL_ERROR_START;
			SERIAL_ERRORLNPGM(MSG_ERR_NO_THERMISTORS);
#endif
			SERIAL_PROTOCOLLN("");
			return;
			break;
		case 109:
		{
			// M109 - Wait for extruder heater to reach target.
			if (setTargetedHotend(109))
			{
				break;
			}
#ifdef AUTOTEMP
			autotemp_enabled = false;
#endif
			if (code_seen('S')) {
				target_temp = code_value();
				setTargetHotend(target_temp, target_extruder);

#ifdef DUAL_X
				if (syncmode_enabled)
				{
					if (target_temp == 0)
						setTargetHotend(0, 1 - target_extruder);
					else
						setTargetHotend(target_temp + (temp_offset*(target_extruder * 2 - 1)), 1 - target_extruder); // Negative offset if right extruder is targeted
				}
#endif
			}
#ifdef AUTOTEMP
			if (code_seen('S')) autotemp_min = code_value();
			if (code_seen('B')) autotemp_max = code_value();
			if (code_seen('F'))
			{
				autotemp_factor = code_value();
				autotemp_enabled = true;
			}
#endif
			setWatch();

			waitForTargetExtruderTemp();

#ifdef DUAL_X
			if (syncmode_enabled)
			{
				target_extruder = 1 - target_extruder;
				waitForTargetExtruderTemp();
			}
#endif

			break;
		}
		case 190: // M190 - Wait for bed heater to reach target.
		{
#if TEMP_BED_PIN > -1
			if (code_seen('S')) setTargetBed(code_value());
			codenum = millis();
			while (isHeatingBed())
			{
				if ((millis() - codenum) > 1000) //Print Temp Reading every 1 second while heating up.
				{
					printTemperatures();
					SERIAL_PROTOCOLLN("");
					codenum = millis();
				}
				manage_heater();
				manage_inactivity();
			}
			previous_millis_cmd = millis();
#endif
			break;
		}
#if FAN_PIN > -1
		case 106: //M106 Fan On
			if (code_seen('S')) {
				FanSpeed = constrain(code_value(), 0, 255);
			}
			else
			{
				FanSpeed = 255;
			}
			break;
		case 107: //M107 Fan Off
			FanSpeed = 0;
			break;
#endif //FAN_PIN
		case 50: // set Extruder2 offset
		  // Old version used an offset that was depicted in mm to move. New system uses offset coordinates
		  // Which means that to stay with the old notation we need to invert the values.
		  // Movement offset = -Extruder coordinates 
			if (code_seen('X')) extruder_offset[X_AXIS][1] = -code_value();
			if (code_seen('Y')) extruder_offset[Y_AXIS][1] = -code_value();
			SERIAL_PROTOCOLPGM("X: ");
			SERIAL_PROTOCOL(-extruder_offset[X_AXIS][1]);
			SERIAL_PROTOCOLPGM(" Y: ");
			SERIAL_PROTOCOLLN(-extruder_offset[Y_AXIS][1]);
			break;
		case 82:
			axis_relative_modes[3] = false;
			break;
		case 83:
			axis_relative_modes[3] = true;
			break;
		case 18: //compatibility
		case 84: // M84
			if (code_seen('S'))
			{
				stepper_inactive_time = code_value() * 1000;
			}
			else
			{
				bool all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])) || (code_seen(axis_codes[3])));
				if (all_axis)
				{
					st_synchronize();
					disable_e0();
					disable_e1();
					disable_e2();
					finishAndDisableSteppers();
				}
				else
				{
					st_synchronize();
					if (code_seen('X')) { disable_x0(); disable_x1(); }
					if (code_seen('Y')) disable_y();
					if (code_seen('Z')) disable_z();
#if ((E0_ENABLE_PIN != X_ENABLE_PIN) && (E1_ENABLE_PIN != Y_ENABLE_PIN)) // Only enable on boards that have seperate ENABLE_PINS
					if (code_seen('E'))
					{
						disable_e0();
						disable_e1();
						disable_e2();
					}
#endif
				}
			}
			break;
		case 85: // M85
			code_seen('S');
			max_inactive_time = code_value() * 1000;
			break;
		case 92: // M92
			for (int8_t i = 0; i < NUM_AXIS; i++)
			{
				if (code_seen(axis_codes[i]))
				{
					if (i == 3)
					{ // E
						float value = code_value();
						if (value < 20.0)
						{
							float factor = axis_steps_per_unit[i] / value; // increase e constants if M92 E14 is given for netfab.
							max_e_jerk *= factor;
							max_feedrate[i] *= factor;
							axis_steps_per_sqr_second[i] *= factor;
						}
						axis_steps_per_unit[i] = value;
					}
					else
					{
						axis_steps_per_unit[i] = code_value();
					}
				}
			}
			break;
		case 114: // M114
			outputCurrentPosition();
			break;
		case 115: // M115 Firmware info string 
			SERIAL_ECHO_START;
			SERIAL_PROTOCOLPGM(MSG_M115_REPORT);
			SERIAL_PROTOCOLPGM(" Extruder offset ");
			SERIAL_PROTOCOLPGM("X: ");
			SERIAL_PROTOCOL(-extruder_offset[X_AXIS][1]);
			SERIAL_PROTOCOLPGM(" Y: ");
			SERIAL_PROTOCOL(-extruder_offset[Y_AXIS][1]);
			SERIAL_PROTOCOLPGM(" bed_width_correction:");
			SERIAL_PROTOCOLLN(-bed_width_correction);

			break;
		case 120: // M120
			enable_endstops(false);
			break;
		case 121: // M121
			enable_endstops(true);
			break;
		case 119: // M119
			SERIAL_PROTOCOLLN(MSG_M119_REPORT);
#if (X0_MIN_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_X0_MIN);
			SERIAL_PROTOCOLLN(((READ(X0_MIN_PIN) ^ X0_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#if (X0_MAX_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_X0_MAX);
			SERIAL_PROTOCOLLN(((READ(X0_MAX_PIN) ^ X0_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#ifdef DUAL_X
#if (X1_MIN_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_X1_MIN);
			SERIAL_PROTOCOLLN(((READ(X1_MIN_PIN) ^ X1_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#if (X1_MAX_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_X1_MAX);
			SERIAL_PROTOCOLLN(((READ(X1_MAX_PIN) ^ X1_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#endif
#if (Y_MIN_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_Y_MIN);
			SERIAL_PROTOCOLLN(((READ(Y_MIN_PIN) ^ Y_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#if (Y_MAX_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_Y_MAX);
			SERIAL_PROTOCOLLN(((READ(Y_MAX_PIN) ^ Y_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#if (Z_MIN_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_Z_MIN);
			SERIAL_PROTOCOLLN(((READ(Z_MIN_PIN) ^ Z_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
#if (Z_MAX_PIN > -1)
			SERIAL_PROTOCOLPGM(MSG_Z_MAX);
			SERIAL_PROTOCOLLN(((READ(Z_MAX_PIN) ^ Z_ENDSTOPS_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
#endif
			break;
			//TODO: update for all axis, use for loop
		case 201: // M201
			for (int8_t i = 0; i < NUM_AXIS; i++)
			{
				if (code_seen(axis_codes[i]))
				{
					max_acceleration_units_per_sq_second[i] = code_value();
					axis_steps_per_sqr_second[i] = code_value() * axis_steps_per_unit[i];
				}
			}
			break;
		case 203: // M203 max feedrate mm/sec
			for (int8_t i = 0; i < NUM_AXIS; i++)
			{
				if (code_seen(axis_codes[i])) max_feedrate[i] = code_value();
			}
			break;
		case 204: // M204 acclereration S normal moves T filmanent only moves
			if (code_seen('S')) acceleration = code_value();
			if (code_seen('T')) retract_acceleration = code_value();
			break;
		case 205: //M205 advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
			if (code_seen('S')) minimumfeedrate = code_value();
			if (code_seen('T')) mintravelfeedrate = code_value();
			if (code_seen('B')) minsegmenttime = code_value();
			if (code_seen('X')) max_xy_jerk = code_value();
			if (code_seen('Z')) max_z_jerk = code_value();
			if (code_seen('E')) max_e_jerk = code_value();
			break;
		case 206: // M206 additional homeing offset
			for (int8_t i = 0; i < 3; i++)
			{
				if (code_seen(axis_codes[i])) add_homeing[i] = code_value();
			}
			break;
		case 207: //  M207 set maxtemp values
			if (code_seen('T')) toolnum = code_value();
			if (toolnum == 0) {
				if (code_seen('S')) heater_0_maxtemp = code_value();
			}
			else if (toolnum == 1) {
				if (code_seen('S')) heater_1_maxtemp = code_value();
			}

			SERIAL_PROTOCOLPGM(" HEATER ");
			SERIAL_PROTOCOL(toolnum);
			SERIAL_PROTOCOLPGM(" MAXTEMP: ");
			if (toolnum == 0) { SERIAL_PROTOCOLLN(heater_0_maxtemp); }
			else if (toolnum == 1) { SERIAL_PROTOCOLLN(heater_1_maxtemp); }

			updateMaxTemp();

			break;
		case 209:
			if (code_seen('T')) toolnum = code_value();
			if (toolnum == 0) {
				if (code_seen('R')) heater_0_offset = code_value();
			}
			else if (toolnum == 1) {
				if (code_seen('R')) heater_1_offset = code_value();
			}
			break;
		case 219: // M219 S<diff in mm> Set bed width offset to calibrate dual carriage movement
			if (code_seen('S'))
			{
				bed_width_correction = constrain(-code_value(), -15, 15);
				updateXMinMaxPos();
			}
			break;
		case 220: // M220 S<factor in percent>- set speed factor override percentage
			if (code_seen('S'))
			{
				feedmultiply = code_value();
				feedmultiplychanged = true;
			}
			break;
		case 221: // M221 S<factor in percent>- set extrude factor override percentage
			if (code_seen('S'))
			{
				extrudemultiply = code_value();
			}
			break;
#ifdef PIDTEMP
		case 301: // M301
			if (code_seen('P')) Kp = code_value();
			if (code_seen('I')) Ki = code_value()*PID_dT;
			if (code_seen('D')) Kd = code_value() / PID_dT;
#ifdef PID_ADD_EXTRUSION_RATE
			if (code_seen('C')) Kc = code_value();
#endif
			updatePID();
			SERIAL_PROTOCOL(MSG_OK);
			SERIAL_PROTOCOL(" p:");
			SERIAL_PROTOCOL(Kp);
			SERIAL_PROTOCOL(" i:");
			SERIAL_PROTOCOL(Ki / PID_dT);
			SERIAL_PROTOCOL(" d:");
			SERIAL_PROTOCOL(Kd*PID_dT);
#ifdef PID_ADD_EXTRUSION_RATE
			SERIAL_PROTOCOL(" c:");
			SERIAL_PROTOCOL(Kc*PID_dT);
#endif
			SERIAL_PROTOCOLLN("");
			break;
#endif //PIDTEMP
		case 302: // allow cold extrudes
			allow_cold_extrudes(true);
			break;
		case 303: // M303 PID autotune
		{
#ifdef USE_BED_PWM
			float temp;
			if (code_seen('E') && code_value() == -1)
			{
				temp = 45;
				if (code_seen('S')) temp = code_value();
				PID_autotune_bed(temp);
			}
			else
			{
				temp = 150.0;
				if (code_seen('S')) temp = code_value();
				PID_autotune(temp);
			}
#else
			float temp = 150.0;
			if (code_seen('S')) temp = code_value();
			PID_autotune(temp);
#endif
			break;
		}
		case 304: // M304
#ifdef USE_BED_PWM
			if (code_seen('P')) bed_Kp = code_value();
			if (code_seen('I')) bed_Ki = code_value()*PID_dT;
			if (code_seen('D')) bed_Kd = code_value() / PID_dT;

			updateBedPID();
			SERIAL_PROTOCOL(MSG_OK);
			SERIAL_PROTOCOL(" p:");
			SERIAL_PROTOCOL(bed_Kp);
			SERIAL_PROTOCOL(" i:");
			SERIAL_PROTOCOL(bed_Ki / PID_dT);
			SERIAL_PROTOCOL(" d:");
			SERIAL_PROTOCOL(bed_Kd*PID_dT);
			SERIAL_PROTOCOLLN("");
#else
			SERIAL_PROTOCOLLN("Bed PWM is not enabled.");
#endif
			break;
		case 400: // M400 finish all moves
			st_synchronize();
			break;
		case 500: // Store settings in EEPROM
			EEPROM_StoreSettings();
			break;
		case 501: // Read settings from EEPROM
			EEPROM_RetrieveSettings();
			break;
		case 502: // Revert to default settings
			EEPROM_RetrieveSettings(true);
			break;
		case 503: // print settings currently in memory
			EEPROM_printSettings();
			break;
		case 999: // Restart after being stopped
			Stopped = false;
			gcode_LastN = Stopped_gcode_LastN;
			FlushSerialRequestResend();
			break;
		}
	}
	else if (code_seen('T'))
	{
		tmp_extruder = code_value();
		if (tmp_extruder >= EXTRUDERS)
		{
			SERIAL_ECHO_START;
			SERIAL_ECHO("T");
			SERIAL_ECHO(tmp_extruder);
			SERIAL_ECHOLN(MSG_INVALID_EXTRUDER);
		}

		else
		{
			target_extruder = tmp_extruder;
			boolean make_move = false;
			if (code_seen('F'))
			{
				make_move = true;
				next_feedrate = code_value();
				if (next_feedrate > 0.0)
				{
					feedrate = next_feedrate;
				}
			}
#ifdef DUAL_X
			// Toolswitch is ignored when in sync mode
			if (!syncmode_enabled && parking_disabled && tmp_extruder != active_extruder)
			{
				// Save current position to return to after applying extruder offset
				set_destination_to_current();

				last_known_x[active_extruder] = current_position[X_AXIS];

				// Offset extruder (only by Y)
				current_position[X_AXIS] = last_known_x[tmp_extruder];
				current_position[Y_AXIS] += extruder_offset[Y_AXIS][tmp_extruder] - extruder_offset[Y_AXIS][active_extruder];

				// Set the new active extruder and position
				active_extruder = tmp_extruder;

				// Update software endstops
				updateXMinMaxPos();

				sync_plan_position();

				// Move to the old position if 'F' was in the parameters
				if (make_move && Stopped == false)
				{
					prepare_move();
				}
			}
			else if (!syncmode_enabled && tmp_extruder != active_extruder)
			{
				// Basically do a G28
				saved_feedrate = feedrate;
				saved_feedmultiply = feedmultiply;
				feedmultiply = 100;
				yoffset = extruder_offset[Y_AXIS][tmp_extruder] - extruder_offset[Y_AXIS][active_extruder];

				enable_endstops(true);

				set_destination_to_current();

				// Start parking current tool
				feedrate = PARKING_FEEDRATE;
				destination[X_AXIS] = parking_pos[active_extruder];

				prepare_move();
				st_synchronize();
				last_known_x[active_extruder] = current_position[X_AXIS];
				is_parked[active_extruder] = true;

				// Activate target tool
				active_extruder = tmp_extruder;
				if (active_extruder == 0)
				{
					if (is_parked[0])
						current_position[X_AXIS] = last_known_x[0];
					else
						HOMEAXIS(X);

					is_parked[0] = false;
				}
				else
				{
					if (is_parked[1])
						current_position[X_AXIS] = last_known_x[1];
					else
						HOMEAXIS(X);

					is_parked[1] = false;
				}

				updateXMinMaxPos();

				// Save offsetted y position in temp variable
				current_position[Y_AXIS] += yoffset;
				sync_plan_position();

#ifdef ENDSTOPS_ONLY_FOR_HOMING
				enable_endstops(false);
#endif

				feedrate = saved_feedrate;
				feedmultiply = saved_feedmultiply;
				previous_millis_cmd = millis();


				// Move to the old position if 'F' was in the parameters
				if (make_move && Stopped == false)
				{
					prepare_move();
				}

			}
#elif EXTRUDERS > 1 // No Dual_X, but with two extruders
			if (tmp_extruder != active_extruder)
			{
				// Save current position to return to after applying extruder offset
				set_destination_to_current();
				// Offset extruder (only by XY)
				for (int i = 0; i < 2; i++)
				{
					current_position[i] += extruder_offset[i][tmp_extruder] - extruder_offset[i][active_extruder];
				}
				// Set the new active extruder and position
				active_extruder = tmp_extruder;
				sync_plan_position();
				// Move to the old position if 'F' was in the parameters
				if (make_move && Stopped == false)
				{
					prepare_move();
				}
			}
#endif
			SERIAL_ECHO_START;
			SERIAL_ECHO(MSG_ACTIVE_EXTRUDER);
			SERIAL_PROTOCOLLN((int)active_extruder);
		}
	}
	else
	{
		SERIAL_ECHO_START;
		SERIAL_ECHOPGM(MSG_UNKNOWN_COMMAND);
		SERIAL_ECHO(cmdbuffer[bufindr]);
		SERIAL_ECHOLNPGM("\"");
	}
	ClearToSend();
}

void FlushSerialRequestResend()
{
	//char cmdbuffer[bufindr][100]="Resend:";
	MYSERIAL.flush();
	SERIAL_PROTOCOLPGM(MSG_RESEND);
	SERIAL_PROTOCOLLN(gcode_LastN + 1);
	ClearToSend();
}

void ClearToSend()
{
	previous_millis_cmd = millis();
	SERIAL_PROTOCOLLNPGM(MSG_OK);
	// SERIAL_PROTOCOLPGM(" ");
	 //SERIAL_PROTOCOL_F(degHotend(active_extruder),1);
	 //#if TEMP_BED_PIN > -1
	 //SERIAL_PROTOCOLPGM(" ");
	 //SERIAL_PROTOCOL_F(degBed(),1);
	 //#endif //TEMP_BED_PIN
	 //SERIAL_PROTOCOLLNPGM("");
}

void get_coordinates()
{
	for (int8_t i = 0; i < NUM_AXIS; i++)
	{
		if (code_seen(axis_codes[i]))
			destination[i] = (float)code_value() + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
		else
			destination[i] = current_position[i];
	}
	if (code_seen('F'))
	{
		next_feedrate = code_value();
		if (next_feedrate > 0.0) feedrate = next_feedrate;
	}
}


void clamp_to_software_endstops(float target[3])
{
	if (min_software_endstops)
	{
		if (target[X_AXIS] < min_pos[X_AXIS]) target[X_AXIS] = min_pos[X_AXIS];
		if (target[Y_AXIS] < min_pos[Y_AXIS]) target[Y_AXIS] = min_pos[Y_AXIS];
		if (target[Z_AXIS] < min_pos[Z_AXIS]) target[Z_AXIS] = min_pos[Z_AXIS];
	}

	if (max_software_endstops)
	{
		if (target[X_AXIS] > max_pos[X_AXIS]) target[X_AXIS] = max_pos[X_AXIS];
		if (target[Y_AXIS] > max_pos[Y_AXIS]) target[Y_AXIS] = max_pos[Y_AXIS];
		if (target[Z_AXIS] > max_pos[Z_AXIS]) target[Z_AXIS] = max_pos[Z_AXIS];
	}
}

void prepare_move()
{
	clamp_to_software_endstops(destination);
	previous_millis_cmd = millis();

	// Do not use feedmultiply for E or Z only moves
	if ((current_position[X_AXIS] == destination[X_AXIS]) && (current_position[Y_AXIS] == destination[Y_AXIS]))
	{
		line_to_destination();
	}
	else
	{
		line_to_destination(feedrate * feedmultiply / 100.0);
	}
	set_current_to_destination();

#ifdef DUAL_X
	last_known_x[active_extruder] = current_position[X_AXIS];
#endif
}


void manage_inactivity()
{
	if ((millis() - previous_millis_cmd) > max_inactive_time)
		if (max_inactive_time)
			kill();
	if (stepper_inactive_time) {
		if ((millis() - previous_millis_cmd) > stepper_inactive_time)
		{
			if (blocks_queued() == false) {
				disable_x0();
				disable_x1();
				disable_y();
				disable_z();
				disable_e0();
				disable_e1();
				disable_e2();
			}
		}
	}
#if( KILL_PIN>-1 )
	if (0 == READ(KILL_PIN))
		kill();
#endif
#if defined(DOOR_PIN) && DOOR_PIN > -1
	if (0 == READ(DOOR_PIN) && !door_status)
	{
		door_open();
	}
	if (0 != READ(DOOR_PIN) && door_status)
	{
		door_closed();
	}
#endif
	check_axes_activity();
}

void kill()
{
	cli(); // Stop interrupts
	disable_heater();

	disable_x0();
	disable_x1();
	disable_y();
	disable_z();
	disable_e0();
	disable_e1();
	disable_e2();

	if (PS_ON_PIN > -1) pinMode(PS_ON_PIN, INPUT);
	SERIAL_ERROR_START;
	SERIAL_ERRORLNPGM(MSG_ERR_KILLED);
	suicide();
	while (1); // Wait for reset
}

#if defined(DOOR_PIN) && DOOR_PIN > -1
void door_open()
{
	door_status = !door_status;
	SERIAL_PROTOCOLLN("//action:door_open");
}

void door_closed()
{
	door_status = !door_status;
	SERIAL_PROTOCOLLN("//action:door_closed");
}
#endif

#ifdef FILAMENT_DETECTION
void checkFilamentError() {
	if (filament_error)
	{
		SERIAL_PROTOCOLLN("//action:filament");
	}
}
#endif

void NotSupported()
{
	SERIAL_ERROR_START;
	SERIAL_ERRORLNPGM(MSG_ERR_NOT_SUPPORTED);
	Stop();
}

void Stop()
{
	disable_heater();
	if (Stopped == false)
	{
		Stopped = true;
		Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
		SERIAL_ERROR_START;
		SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
	}
}

bool IsStopped() { return Stopped; };

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val)
{
	val &= 0x07;
	switch (digitalPinToTimer(pin))
	{

#if defined(TCCR0A)
	case TIMER0A:
	case TIMER0B:
		//         TCCR0B &= ~(CS00 | CS01 | CS02);
		//         TCCR0B |= val;
		break;
#endif

#if defined(TCCR1A)
	case TIMER1A:
	case TIMER1B:
		//         TCCR1B &= ~(CS10 | CS11 | CS12);
		//         TCCR1B |= val;
		break;
#endif

#if defined(TCCR2)
	case TIMER2:
	case TIMER2:
		TCCR2 &= ~(CS10 | CS11 | CS12);
		TCCR2 |= val;
		break;
#endif

#if defined(TCCR2A)
	case TIMER2A:
	case TIMER2B:
		TCCR2B &= ~(CS20 | CS21 | CS22);
		TCCR2B |= val;
		break;
#endif

#if defined(TCCR3A)
	case TIMER3A:
	case TIMER3B:
	case TIMER3C:
		TCCR3B &= ~(CS30 | CS31 | CS32);
		TCCR3B |= val;
		break;
#endif

#if defined(TCCR4A)
	case TIMER4A:
	case TIMER4B:
	case TIMER4C:
		TCCR4B &= ~(CS40 | CS41 | CS42);
		TCCR4B |= val;
		break;
#endif

#if defined(TCCR5A)
	case TIMER5A:
	case TIMER5B:
	case TIMER5C:
		TCCR5B &= ~(CS50 | CS51 | CS52);
		TCCR5B |= val;
		break;
#endif
	}
}
#endif //FAST_PWM_FAN

bool setTargetedHotend(int code) {
	target_extruder = active_extruder;
	if (code_seen('T')) {
		target_extruder = code_value();
		if (target_extruder >= EXTRUDERS) {
			SERIAL_ECHO_START;
			switch (code) {
			case 104:
				SERIAL_ECHO(MSG_M104_INVALID_EXTRUDER);
				break;
			case 105:
				SERIAL_ECHO(MSG_M105_INVALID_EXTRUDER);
				break;
			case 109:
				SERIAL_ECHO(MSG_M109_INVALID_EXTRUDER);
				break;
			}
			SERIAL_ECHOLN(target_extruder);
			return true;
		}
	}
	return false;
}

void printTemperatures()
{
	for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder)
	{
		SERIAL_PROTOCOLPGM("T");
		SERIAL_PROTOCOL(cur_extruder);
		SERIAL_PROTOCOLPGM(":");
		SERIAL_PROTOCOL_F(degHotend(cur_extruder), 1);
		SERIAL_PROTOCOLPGM(" /");
		SERIAL_PROTOCOL_F(degTargetHotend(cur_extruder), 1);
		SERIAL_PROTOCOLPGM(" ");
	}
#if TEMP_BED_PIN > -1
	SERIAL_PROTOCOLPGM("B:");
	SERIAL_PROTOCOL_F(degBed(), 1);
	SERIAL_PROTOCOLPGM(" /");
	SERIAL_PROTOCOL_F(degTargetBed(), 1);
#endif //TEMP_BED_PIN
#ifdef PIDTEMP
	SERIAL_PROTOCOLPGM(" @:");
	SERIAL_PROTOCOL(getHeaterPower(target_extruder));
#endif
}

void dumpstatus()
{
	SERIAL_ECHO("t: ");
	SERIAL_ECHOLN(millis());

	SERIAL_ECHO("Sync mode: ");
	SERIAL_ECHOLN(syncmode_enabled);

	SERIAL_ECHO("Extruder: ");
	SERIAL_ECHOLN(active_extruder);

	SERIAL_ECHO("Position E: ");
	SERIAL_ECHOLN(current_position[E_AXIS]);

	SERIAL_ECHO("Destination E: ");
	SERIAL_ECHOLN(destination[E_AXIS]);
	
	SERIAL_ECHO("Position X: ");
	SERIAL_ECHOLN(current_position[X_AXIS]);

	SERIAL_ECHO("Destination X: ");
	SERIAL_ECHOLN(destination[X_AXIS]);
	
	SERIAL_ECHO("Min/Max X:");
	SERIAL_ECHO(min_pos[X_AXIS]);
	SERIAL_ECHO("/");
	SERIAL_ECHOLN(max_pos[X_AXIS]);

#ifdef DUAL_X
	SERIAL_ECHO("Parking: ");
	SERIAL_ECHOLN(parking_disabled ? "disabled" : "enabled");

	SERIAL_ECHO("Parked: ");
	SERIAL_ECHO(is_parked[0]);
	SERIAL_ECHO("/");
	SERIAL_ECHOLN(is_parked[1]);

	SERIAL_ECHO("Park pos: ");
	SERIAL_ECHO(parking_pos[0]);
	SERIAL_ECHO("/");
	SERIAL_ECHOLN(parking_pos[1]);

	SERIAL_ECHO("Last known pos: ");
	SERIAL_ECHO(last_known_x[0]);
	SERIAL_ECHO("/");
	SERIAL_ECHOLN(last_known_x[100]);

	SERIAL_ECHO("Temp offset: ");
	SERIAL_ECHOLN(temp_offset);

	SERIAL_ECHO("X offset: ");
	SERIAL_ECHO(actual_dual_x_offset);
	SERIAL_ECHO("/");
	SERIAL_ECHOLN(dual_x_offset);
#endif
	SERIAL_ECHO("Home dir: ");
	SERIAL_ECHOLN(home_dir(X_AXIS));

	SERIAL_ECHO("Movement mode: ");
	SERIAL_ECHOLN(axis_relative_modes[X_AXIS] || relative_mode ? "relative" : "absolute");

	SERIAL_ECHO("Extrusion mode: ");
	SERIAL_ECHOLN(axis_relative_modes[E_AXIS] || relative_mode ? "relative" : "absolute");

#if SAFE_MOVES
	SERIAL_ECHO("Position known: ");
	SERIAL_ECHO(position_known[X_AXIS]);
	SERIAL_ECHO(" ");
	SERIAL_ECHO(position_known[Y_AXIS]);
	SERIAL_ECHO(" ");
	SERIAL_ECHOLN(position_known[Z_AXIS]);
#endif
}

void waitForTargetExtruderTemp()
{
	unsigned long codenum;
	/* See if we are heating up or cooling down */
	bool target_direction = isHeatingHotend(target_extruder); // true if heating, false if cooling

#ifdef TEMP_RESIDENCY_TIME
	long residencyStart;
	residencyStart = -1;
	/* continue to loop until we have reached the target temp
	_and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
	while ((residencyStart == -1) ||
		(residencyStart >= 0 && (((unsigned int)(millis() - residencyStart)) < (TEMP_RESIDENCY_TIME * 1000UL))))
	{
#else
	while (target_direction ? (isHeatingHotend(target_extruder)) : (isCoolingHotend(target_extruder) && (CooldownNoWait == false)))
	{
#endif //TEMP_RESIDENCY_TIME
		if ((millis() - codenum) > 1000UL)
		{ //Print Temp Reading and remaining time every 1 second while heating up/cooling down
			printTemperatures();
#ifdef TEMP_RESIDENCY_TIME
			SERIAL_PROTOCOLPGM(" W:");
			if (residencyStart > -1)
			{
				codenum = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residencyStart)) / 1000UL;
				SERIAL_PROTOCOL(codenum);
			}
			else
			{
				SERIAL_PROTOCOL("?");
			}
#endif
			SERIAL_PROTOCOLLN("");
			codenum = millis();
		}
		manage_heater();
		manage_inactivity();
#ifdef TEMP_RESIDENCY_TIME
		/* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
		or when current temp falls outside the hysteresis after target temp was reached */
		if ((residencyStart == -1 && target_direction && (degHotend(target_extruder) >= (degTargetHotend(target_extruder) - TEMP_WINDOW))) ||
			(residencyStart == -1 && !target_direction && (degHotend(target_extruder) <= (degTargetHotend(target_extruder) + TEMP_WINDOW))) ||
			(residencyStart > -1 && labs(degHotend(target_extruder) - degTargetHotend(target_extruder)) > TEMP_HYSTERESIS))
		{
			residencyStart = millis();
		}
#endif //TEMP_RESIDENCY_TIME
	}
	starttime = millis();
	previous_millis_cmd = millis();
}

#ifdef DUAL_X
// Begins syncing left and right x carriage. Requires G28 to get into position
// offset:        distance between tools. Min: extruder_offset, Max: bed_width() - extruder_offset. Value 0
//                will set the offset to half of the print area size
// do_invert_x0:  true enables mirrored synchronous mode
static void beginSyncMode(float offset, bool do_invert_x0)
{
	// Check if temperatures are in sync, update if necessary
	float current_left_temp_target = degTargetHotend(1);
	if (current_left_temp_target != 0)
		setTargetHotend(current_left_temp_target + temp_offset, 0);
	else
		setTargetHotend(0, 0);

	// Correct offset 
	// Limit offset to reasonable values. Use center for offset=0
	if (offset < 0.01) // offset is a float, so compare with error margin
		offset = (bed_width() - extruder_offset[X_AXIS][1]) / 2.0; // Move halfway !! Something weird happens here: adding scalars to arrays = bad
	else if (offset < -extruder_offset[X_AXIS][1])
		offset = -extruder_offset[X_AXIS][1];
	else if (offset > bed_width() + extruder_offset[X_AXIS][1])
		offset = bed_width() + extruder_offset[X_AXIS][1] - 50.0; // Arbitrary max offset 

	if (!syncmode_enabled || do_invert_x0 != inverted_x0_mode || offset != actual_dual_x_offset)
	{
		// Wait for last move to complete
		st_synchronize();

		// Store the calculated offset
		actual_dual_x_offset = offset;

		// Enable syncmode
		syncmode_enabled = true;

		// Enable/disable mirrormode
		inverted_x0_mode = do_invert_x0;

		// Move left and right in position
		//homeDualX();
	}
}

// Disables (mirrored) synchronous mode
static void endSyncMode()
{
	if (syncmode_enabled)
	{
		// Wait for last move to complete
		st_synchronize();

		// Return to individual movement of t0 and t1
		inverted_x0_mode = false;
		syncmode_enabled = false;
	}
}

// Sets the parking position of a tool
// extruder: tool number
// offset: offset from printing area; must be >= 0
static void setParkingOffset(int extruder, float offset)
{
	// Limit offset to half bed size 
	// offset = constrain(offset, 0, bed_width() / 2.0 + extruder_offset[X_AXIS][1] / 2.0); // Could cause some funny side effects - removed for now.

	// 'Forget' the extruder may have been parked (to prevent toolswitch assuming wrong parking pos)
	is_parked[extruder] = false;

	if (extruder == 0)
		parking_pos[extruder] = bed_width() - offset;
	else if (extruder == 1)
		parking_pos[extruder] = bed_width() + offset;
}

// Updates the software endstops based on the last known position of the other extruder
// Used when performing a toolswitch or switching between synchronous mode and individual mode
static void updateXMinMaxPos()
{
	if (active_extruder == 0)
	{
		// Right tool
		min_pos[X_AXIS] = last_known_x[1] - extruder_offset[X_AXIS][1];
		max_pos[X_AXIS] = bed_width() + add_homeing[X_AXIS];
	}
	else
	{
		// Left tool
		min_pos[X_AXIS] = base_min_pos(X_AXIS) + add_homeing[X_AXIS];
		max_pos[X_AXIS] = last_known_x[0] + add_homeing[X_AXIS] + extruder_offset[X_AXIS][1];
	}
}

static void homeDualX()
{
	// First home the alternative carriage for dual x mode
	if (!syncmode_enabled)
	{
		tmp_extruder = active_extruder;

		active_extruder = 1 - tmp_extruder;
		HOMEAXIS(X);

		// Always do the switch before homing!
		active_extruder = tmp_extruder;

		HOMEAXIS(X);
	}
	else
	{
		bool was_inverted_x0_mode = inverted_x0_mode;

		// First disable synchronous movement, as this may disturb homing
		syncmode_enabled = false;
		inverted_x0_mode = false;

		// First home left
		active_extruder = 1;
		HOMEAXIS(X);

		// Then right
		active_extruder = 0;
		HOMEAXIS(X);

		sync_plan_position();

		if (!was_inverted_x0_mode)
		{
			// Bring right tool in position
			feedrate = homing_feedrate[X_AXIS];
			destination[X_AXIS] = actual_dual_x_offset;
			line_to_destination();

			// Wait for it
			st_synchronize();
		}
		else
		{
			// Tools are in their outermost positions, this allows to mirror movement.
			inverted_x0_mode = true;
		}

		// Set first x-axis to determine movement, syncmode_enable will copy moves
		active_extruder = 1;
		syncmode_enabled = true;

		//Update software endstops, set current X position to X_MIN_POS
		axis_is_at_home(X_AXIS);

		sync_plan_position();
		set_destination_to_current();
	}
}
#endif

static void outputCurrentPosition()
{
	SERIAL_PROTOCOLPGM("X:");
	SERIAL_PROTOCOL(current_position[X_AXIS]);
	SERIAL_PROTOCOLPGM("Y:");
	SERIAL_PROTOCOL(current_position[Y_AXIS]);
	SERIAL_PROTOCOLPGM("Z:");
	SERIAL_PROTOCOL(current_position[Z_AXIS]);
	SERIAL_PROTOCOLPGM("E:");
	SERIAL_PROTOCOL(current_position[E_AXIS]);

	SERIAL_PROTOCOLPGM(MSG_COUNT_X);
	SERIAL_PROTOCOL(float(st_get_position(X_AXIS)) / axis_steps_per_unit[X_AXIS]);
	SERIAL_PROTOCOLPGM("Y:");
	SERIAL_PROTOCOL(float(st_get_position(Y_AXIS)) / axis_steps_per_unit[Y_AXIS]);
	SERIAL_PROTOCOLPGM("Z:");
	SERIAL_PROTOCOL(float(st_get_position(Z_AXIS)) / axis_steps_per_unit[Z_AXIS]);

	SERIAL_PROTOCOLLN("");
}

static void outputDestination()
{
	SERIAL_PROTOCOLPGM("X:");
	SERIAL_PROTOCOL(destination[X_AXIS]);
	SERIAL_PROTOCOLPGM("Y:");
	SERIAL_PROTOCOL(destination[Y_AXIS]);
	SERIAL_PROTOCOLPGM("Z:");
	SERIAL_PROTOCOL(destination[Z_AXIS]);
	SERIAL_PROTOCOLPGM("E:");
	SERIAL_PROTOCOL(destination[E_AXIS]);


	SERIAL_PROTOCOLLN("");
}
