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
 * @file "modules/loop/loop.c"
 * @author Marly Kuijpers
 * waypoints following
 */

#include "modules/loop/loop.h"
#include <stdint.h>
#include "firmwares/rotorcraft/navigation.h"
#include "generated/flight_plan.h"


void loop_init(void) 
{

}
float my_time = 0;
void loop_periodic(void)
{
my_time+=0.005;
int32_t x = 1024 * 4 * cos(my_time);
int32_t y = 1024 * 4 * sin(my_time);

waypoint_set_xy_i(WP_STDBY,x ,y);
}


