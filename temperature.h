/*
  temperature.h - temperature controller
  Part of Marlin

  Copyright (c) 2011 Erik van der Zalm

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

#ifndef temperature_h
#define temperature_h 

#include "Marlin.h"
#include "planner.h"
#ifdef PID_ADD_EXTRUSION_RATE
  #include "stepper.h"
#endif

// public functions
void tp_init();  //initialise the heating
void manage_heater(); //it is critical that this is called periodically.

// low level conversion routines
// do not use this routines and variables outsie of temperature.cpp
int temp2analog(int celsius, uint8_t e);
int temp2analogBed(int celsius);
float analog2temp(int raw, uint8_t e);
float analog2tempBed(int raw);
extern int target_raw[EXTRUDERS];  
extern int current_raw[EXTRUDERS];        // TODO: ADC returns unsigned 16 bit. Perhaps better to use uint16_t instead?
extern int target_raw_bed;
extern int current_raw_bed;

#if ENABLE_HEATER_CHECK
	extern long heating_started[EXTRUDERS]; // Contains the time (in ms) at which heating started, i.e. new target > last target
	extern long last_heater_check[EXTRUDERS]; // Contains the last time the heater was checked
	extern float previous_temp[EXTRUDERS]; // Contains the temperature at the last check, to allow to calculate the temperature delta over time
#endif

#ifdef BED_LIMIT_SWITCHING
  extern int target_bed_low_temp ;  
  extern int target_bed_high_temp ;
#endif
extern float Kp,Ki,Kd,Kc;
extern float bed_Kp,bed_Ki,bed_Kd;

#ifdef PIDTEMP
  extern float pid_setpoint[EXTRUDERS];
#endif

#ifdef USE_BED_PWM
  extern float pid_bed_setpoint;
#endif  
// #ifdef WATCHP  ERIOD
  extern int watch_raw[EXTRUDERS] ;
//   extern unsigned long watchmillis;
// #endif
extern int heater_0_maxtemp;
extern int heater_1_maxtemp;

#ifdef USE_RTD
  extern float heater_0_offset;
  extern float heater_1_offset;
#endif


//high level conversion routines, for use outside of temperature.cpp
//inline so that there is no performance decrease.
//deg=degreeCelsius

FORCE_INLINE float degHotend(uint8_t extruder) {  
  return analog2temp(current_raw[extruder], extruder);
};

FORCE_INLINE int degHotendRaw(uint8_t extruder) {  
  return current_raw[extruder];
};

FORCE_INLINE float degBed() {
  return analog2tempBed(current_raw_bed);
};

FORCE_INLINE float degTargetHotend(uint8_t extruder) {  
  return analog2temp(target_raw[extruder], extruder);
};

FORCE_INLINE float degTargetBed() {   
  return analog2tempBed(target_raw_bed);
};

FORCE_INLINE void setTargetHotend(const float &celsius, uint8_t extruder) {  
  target_raw[extruder] = temp2analog(celsius, extruder);
#ifdef PIDTEMP
	#if ENABLE_HEATER_CHECK
	  if (celsius > pid_setpoint[extruder])
	  {
		  heating_started[extruder] = millis();
		  previous_temp[extruder] = degHotend(extruder);
	  }
	#endif
  
  pid_setpoint[extruder] = celsius;
#endif //PIDTEMP
};

FORCE_INLINE void setTargetBed(const float &celsius) {  
  
  target_raw_bed = temp2analogBed(celsius);

  #ifdef USE_BED_PWM
    pid_bed_setpoint = celsius;
  #endif

  #ifdef BED_LIMIT_SWITCHING
    if(celsius>BED_HYSTERESIS)
    {
    target_bed_low_temp= temp2analogBed(celsius-BED_HYSTERESIS);
    target_bed_high_temp= temp2analogBed(celsius+BED_HYSTERESIS);
    }
    else
    { 
      target_bed_low_temp=0;
      target_bed_high_temp=0;
    }
  #endif
};

FORCE_INLINE bool isHeatingHotend(uint8_t extruder){  
  return target_raw[extruder] > current_raw[extruder];
};

FORCE_INLINE bool isHeatingBed() {
  return target_raw_bed > current_raw_bed;
};

FORCE_INLINE bool isCoolingHotend(uint8_t extruder) {  
  return target_raw[extruder] < current_raw[extruder];
};

FORCE_INLINE bool isCoolingBed() {
  return target_raw_bed < current_raw_bed;
};

#define degHotend0() degHotend(0)
#define degTargetHotend0() degTargetHotend(0)
#define setTargetHotend0(_celsius) setTargetHotend((_celsius), 0)
#define isHeatingHotend0() isHeatingHotend(0)
#define isCoolingHotend0() isCoolingHotend(0)
#if EXTRUDERS > 1
#define degHotend1() degHotend(1)
#define degTargetHotend1() degTargetHotend(1)
#define setTargetHotend1(_celsius) setTargetHotend((_celsius), 1)
#define isHeatingHotend1() isHeatingHotend(1)
#define isCoolingHotend1() isCoolingHotend(1)
#else
#define setTargetHotend1(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 2
#define degHotend2() degHotend(2)
#define degTargetHotend2() degTargetHotend(2)
#define setTargetHotend2(_celsius) setTargetHotend((_celsius), 2)
#define isHeatingHotend2() isHeatingHotend(2)
#define isCoolingHotend2() isCoolingHotend(2)
#else
#define setTargetHotend2(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 3
#error Invalid number of extruders
#endif



int getHeaterPower(int heater);

void disable_heater();
void setWatch();
void updatePID();

void updateMaxTemp();

FORCE_INLINE void autotempShutdown(){
 #ifdef AUTOTEMP
 if(autotemp_enabled)
 {
  autotemp_enabled=false;
  if(degTargetHotend(active_extruder)>autotemp_min)
    setTargetHotend(0,active_extruder);
 }
 #endif
}

void PID_autotune(float temp);

#ifdef USE_BED_PWM
void PID_autotune_bed(float temp);
void updateBedPID();
int getHeaterPowerBed();
#endif

#endif

