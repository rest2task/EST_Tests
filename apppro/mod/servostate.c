#include "servostate.h"
#include "oprintf.h" 
#include "language.h"
#include "servostate.h"

#define PHASEMOTOR_FILE			    DATA_PATH"phase_motor.map"
#define INOVANCEMOTOR_FILE			DATA_PATH"inovance_motor.map"
#define CAN_STATE                   DATA_PATH"can_state.map"
#define SERVO_RUN_STATE             DATA_PATH"servo_run_state.map"


PHASH_TBL pmap_motortbl=NULL;//PHASE
PHASH_TBL pmap_motortblx=NULL;//INOVANCE
PHASH_TBL pmap_runstatetbl=NULL;//servo run state
PHASH_TBL pmap_tblx1=NULL;//can state
UI8 nServoId=1;

void ServoStateInital(PCMOD pmod)
{
	ParseCSVFile(PHASEMOTOR_FILE,&pmap_motortbl);
	ParseCSVFile(INOVANCEMOTOR_FILE,&pmap_motortblx);
	ParseCSVFile(CAN_STATE,&pmap_tblx1);//can comm state
	ParseCSVFile(SERVO_RUN_STATE,&pmap_runstatetbl);//servo run state
}

