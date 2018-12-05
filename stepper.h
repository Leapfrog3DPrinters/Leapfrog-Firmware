/*
  stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
 Part of Grbl

 Copyright (c) 2009-2011 Simen Svale Skogsrud

 Grbl is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Grbl is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef stepper_h
#define stepper_h

#include "planner.h"

#if EXTRUDERS > 2
 // TODO: Assess whether this is still needed. If so, adjust for dual x-axis
#define WRITE_E_STEP(v) { if(current_block->active_extruder == 2) { WRITE(E2_STEP_PIN, v); } else { if(current_block->active_extruder == 1) { WRITE(E1_STEP_PIN, v); } else { WRITE(E0_STEP_PIN, v); }}}
#define NORM_E_DIR() { if(current_block->active_extruder == 2) { WRITE(!E2_DIR_PIN, INVERT_E2_DIR); } else { if(current_block->active_extruder == 1) { WRITE(!E1_DIR_PIN, INVERT_E1_DIR); } else { WRITE(E0_DIR_PIN, !INVERT_E0_DIR); }}}
#define REV_E_DIR() { if(current_block->active_extruder == 2) { WRITE(E2_DIR_PIN, INVERT_E2_DIR); } else { if(current_block->active_extruder == 1) { WRITE(E1_DIR_PIN, INVERT_E1_DIR); } else { WRITE(E0_DIR_PIN, INVERT_E0_DIR); }}}

#elif EXTRUDERS > 1
#define WRITE_E_STEP(v) { \
	if(syncmode_enabled) { WRITE(E0_STEP_PIN, v); WRITE(E1_STEP_PIN, v);} \
 	else if(current_block->active_extruder == 1) { WRITE(E1_STEP_PIN, v); } else { WRITE(E0_STEP_PIN, v); } \
 }
#define NORM_E_DIR() { \
 if(syncmode_enabled) { WRITE(E0_DIR_PIN, !INVERT_E0_DIR); WRITE(E1_DIR_PIN, !INVERT_E1_DIR);}\
 else if(current_block->active_extruder == 1) { WRITE(E1_DIR_PIN, !INVERT_E1_DIR); } else { WRITE(E0_DIR_PIN, !INVERT_E0_DIR); }\
}
#define REV_E_DIR() { \
	if(syncmode_enabled) { WRITE(E0_DIR_PIN, INVERT_E0_DIR); WRITE(E1_DIR_PIN, INVERT_E1_DIR);  }  \
 	else if(current_block->active_extruder == 1) { WRITE(E1_DIR_PIN, INVERT_E1_DIR); } else { WRITE(E0_DIR_PIN, INVERT_E0_DIR); }  \
}

#else
#define WRITE_E_STEP(v) WRITE(E0_STEP_PIN, v)
#define NORM_E_DIR() WRITE(E0_DIR_PIN, !INVERT_E0_DIR)
#define REV_E_DIR() WRITE(E0_DIR_PIN, INVERT_E0_DIR)


#endif

#ifdef DUAL_X
#define WRITE_X_STEP(v) { \
	if(syncmode_enabled) { WRITE(X0_STEP_PIN, v);  WRITE(X1_STEP_PIN, v);  } \
 	else if(current_block->active_extruder == 1) { WRITE(X1_STEP_PIN, v); } else { WRITE(X0_STEP_PIN, v); } \
 }
#else
  #define WRITE_X_STEP(v) WRITE(X0_STEP_PIN, v)
#endif


// Initialize and start the stepper motor subsystem
void st_init();

// Block until all buffered steps are executed
void st_synchronize();

// Set current position in steps
void st_set_position(const long &x, const long &y, const long &z, const long &e);
void st_set_e_position(const long &e);

// Get current position in steps
long st_get_position(uint8_t axis);

// The stepper subsystem goes to sleep when it runs out of things to execute. Call this
// to notify the subsystem that it is time to go to work.
void st_wake_up();


void checkHitEndstops(); //call from somwhere to create an serial error message with the locations the endstops where hit, in case they were triggered
void endstops_hit_on_purpose(); //avoid creation of the message, i.e. after homeing and before a routine call of checkHitEndstops();

void enable_endstops(bool x, bool y, bool z); // Enable/disable endstop checking
void enable_piezo(bool z);

void finishAndDisableSteppers();

extern block_t *current_block;  // A pointer to the block currently being traced
extern bool syncmode_enabled; // Sync movement for X0 and X1
extern bool inverted_x0_mode; // true ->  x0 moves in opposite direction as x1

#ifdef FILAMENT_DETECTION
	extern int total_errors;
	extern int total_s_errors;
	extern int total_l_errors;
	extern int s_error;
	extern int l_error;
	extern bool filament_error;
	void checkFilament();
	void pulseChecker();
	void initPulseChecker();
	void readFilamentStatus();
#endif

void quickStop();

#endif
