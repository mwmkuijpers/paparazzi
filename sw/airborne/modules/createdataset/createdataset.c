/*
 * Copyright (C) Marly Kuijpers
 *
 * This file is part of paparazzi
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
 * @file "modules/createdataset/createdataset.c"
 * @author Marly Kuijpers
 * Creates dataset by triggering front and bottom camera to take shots and logging ac data
 */
#include <stdio.h>
#include "std.h"
#include "modules/createdataset/createdataset.h"
#include "modules/loggers/file_logger.h"
#include "video_thread.h"
#include "video_thread2.h"
#include "state.h"
#include "subsystems/imu.h"
#include "subsystems/ins/ins_int.h"
#include "subsystems/abi.h"
#include <string.h>
#include <modules/sonar/agl_dist.h>
#include <navdata.h>
static abi_event sonar_ev;
//static void sonar_cb(uint8_t sender_id, float distance);


float sonar_measurements;


void trigger_shot(void)
{

printf("[createdataset] MARLY trigger shot loaded\n");

video_thread_take_shot(TRUE);
printf("[createdataset] MARLY shot taken\n");
video_thread2_take_shot2(TRUE);

}
//float distance1;
static void sonar_cb(uint8_t __attribute__((unused)) sender_id, float distance){
printf("[createdataset] MARLY SONARDISTANCE %f\n", distance);
sonar_measurements = distance;
}


void load_sonar_data(void){
//sonar_measurements =10;
}


static FILE *file_logger = NULL;

void init_createdataset(void)
{
AbiBindMsgAGL(ABI_BROADCAST, &sonar_ev, sonar_cb);
//printf("[createdataset] loadsonardata");
	//load_sonar_data();	
}
void file_logger_start(void)
{
video_thread_start();
video_thread2_start();


printf("[createdataset] MARLY fileloggerstart\n");
	// Open file

	file_logger = fopen("/data/video/usb/logfile.txt", "wa");
	if (file_logger < 0 ){
	file_logger = NULL;
	printf("MARLY createdataset logger file cannot be created");
	return;
	}
}

void file_logger_periodic(void)
{
printf("[createdataset] MARLY fileloggerperiodic loaded");
 
 if (file_logger<0) {
printf("[createdataset] MARLY filelogger does not work");
   return;
  }

  trigger_shot();
  static uint32_t counter;
  struct Int32Quat *quat = stateGetNedToBodyQuat_i();

  fprintf(file_logger, "%d,%d,%d,%d,%d,%d,%d,%f,\n",
          counter,
	  imu.gyro.p,
	  imu.gyro.q,
	  imu.gyro.r,
	  imu.accel.x,
	  imu.accel.y,
	  imu.accel.z,
	  sonar_measurements

         );
  counter++; 
}



void file_logger_stop(void){

if (file_logger != NULL) {
    fclose(file_logger);
    file_logger = NULL;
  }
}
