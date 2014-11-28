/******************************************************************************
 **  Copyright (c) 2014, RoseTeam Software. All Rights Reserved.
 **
 **  This file is part of RoseTeam Software.
 **
 **  RoseTeam Software is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  RoseTeam Software is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 **
 ******************************************************************************/

#ifndef MOVE_H
#define MOVE_H

#include <stdio.h>
#include "action.h"

typedef struct Propulsion_Move Propulsion_Move;
struct Propulsion_Move
{
    int speed;
    int angle;
    int x;
    int y;
};

ACTION_STATE __propulsion_move(Propulsion_Move *parameters);

void __propulsion_move_failed_handler(void);

#endif
