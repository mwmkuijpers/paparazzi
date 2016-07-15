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
 * @file "modules/createdataset/createdataset.h"
 * @author Marly Kuijpers
 * Creates dataset by triggering front and bottom camera to take shots and logging ac data
 */

#ifndef CREATEDATASET_H
#define CREATEDATASET_H

void init_createdataset(void);
//void trigger_shot(void);
void file_logger_start(void);
void createdataset_periodic(void);
void load_sonar_data(void);
//static void sonar_cb(uint8_t __attribute__((unused)) sender_id, float distance);
void file_logger_stop(void);
//void agl_dist_init(void);
//float agl_dist_value_filtered;
//static void sonar_cb(uint8_t __attribute__((unused)) sender_id, float distance);
#endif

