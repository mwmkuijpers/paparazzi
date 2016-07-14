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
//#include "modules/loggers/file_logger.h"
#include "video_thread.h"
#include "state.h"
#include "subsystems/imu.h"
#include "subsystems/ins/ins_int.h"
#include "subsystems/abi.h"
#include <string.h>
#include <modules/sonar/agl_dist.h>
//#include <navdata.h>
static abi_event sonar_ev;
//static void sonar_cb(uint8_t sender_id, float distance);

void init_save_shot(void);
struct image_t *log_front(struct image_t *img);
struct image_t *log_bottom(struct image_t *img);


////////////////////////////////////////////////
// Code in de autopilot thread




float sonar_measurements;



volatile uint32_t trigger_bottom = 0;
volatile uint32_t trigger_front = 0;

void trigger_shot(uint32_t counter);
void trigger_shot(uint32_t counter)
{
	printf("[createdataset] MARLY trigger shot loaded\n");
	trigger_bottom = counter;
	printf("[createdataset] MARLY shot taken\n");
	trigger_front = counter;
}


//float distance1;
static void sonar_cb(uint8_t __attribute__((unused)) sender_id, float distance){
	printf("[createdataset] MARLY SONARDISTANCE %f\n", distance);
	sonar_measurements = distance;
}



static FILE *file_logger = NULL;

void init_createdataset(void)
{
	init_save_shot();

	AbiBindMsgAGL(ABI_BROADCAST, &sonar_ev, sonar_cb);

	cv_add_to_device(&front_camera, log_front);
	cv_add_to_device(&bottom_camera, log_bottom);
}


void file_logger_start(void)
{

	printf("[createdataset] MARLY fileloggerstart\n");
	// Open file

  //file_logger = fopen("/data/video/usb/logfile.csv", "wa");
  file_logger = fopen("/data/ftp/internal_000/images/logfile.csv", "wa");
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

  static uint32_t counter;
  trigger_shot(counter);
  //struct Int32Quat *quat = stateGetNedToBodyQuat_i();

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


/////////////////////////////////////////////////////////
// Code in de vision thread


/** The file pointer */
struct image_t img_jpeg1;
struct image_t img_jpeg2;
struct image_t img_small;

void init_save_shot(void)
{
  image_create(&img_jpeg1, 5000, 5000, IMAGE_JPEG);
  image_create(&img_jpeg2, 5000, 5000, IMAGE_JPEG);
  image_create(&img_small, 2048/4, 3320/4, IMAGE_YUV422);
}


#include "modules/computer_vision/lib/encoding/jpeg.h"


void save_shot(struct image_t *img, struct image_t *img_jpeg, char* camera_name, uint32_t count);
void save_shot(struct image_t *img, struct image_t *img_jpeg, char* camera_name, uint32_t count)
{

  // Search for a file where we can write to
  char save_name[128];

  sprintf(save_name, "/data/ftp/internal_000/images/%s_%05d.jpg", camera_name, count);

  // Check if file exists or not
  if (access(save_name, F_OK) == -1) {

    // Create a high quality image (99% JPEG encoded)
    jpeg_encode_image(img, img_jpeg, 99, TRUE);

    FILE *fp = fopen(save_name, "w");
    if (fp == NULL) {
      printf("[video_thread-thread] Could not write shot %s.\n", save_name);
    } else {
      // Save it to the file and close it
      fwrite(img_jpeg->buf, sizeof(uint8_t), img_jpeg->buf_size, fp);
      fclose(fp);
    }
  }
}

struct image_t *log_front(struct image_t *img)
{
	if (trigger_front>0)
	{
		printf("AP thread vraagt om eem front beeld.\n");

    image_yuv422_downsample(img, &img_small, 4);
		save_shot(&img_small, &img_jpeg1, "front", trigger_front);

    trigger_front = 0;
	}
	return img;
}

struct image_t *log_bottom(struct image_t *img)
{
	if (trigger_bottom>0)
	{
		printf("AP thread vraagt om eem bottom beeld.\n");
	  	save_shot(img, &img_jpeg2, "bottom", trigger_bottom);	

	  	trigger_bottom = 0;
	}
  	return img;
}

/** Start the file logger and open a new file
void video_usb_logger_start(void)
{
  // Create the jpeg image used later
  image_create(&img_jpeg, VIDEO_USB_LOGGER_WIDTH, VIDEO_USB_LOGGER_HEIGHT, IMAGE_JPEG);

  char filename[512];

  // Search and create a new folder
  sprintf(foldername, "%s/pprzvideo%05d", STRINGIFY(VIDEO_USB_LOGGER_PATH), counter);
  struct stat st = {0};

  while (stat(foldername, &st) >= 0) {
    counter++;
    sprintf(foldername, "%s/pprzvideo%05d", STRINGIFY(VIDEO_USB_LOGGER_PATH), counter);
  }
  mkdir(foldername, 0700);
*/





