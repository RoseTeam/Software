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

#include <stdio.h>
#include <stdlib.h>
#include "strategy.h"
#include "log.h"
#include "actuators.h"
#include "propulsion.h"
#include "move.h"
#include "arm.h"

Propulsion_Move *param_move;




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void strategy_example(File *actions)
{
    param_move = malloc(sizeof(Propulsion_Move));
    Actuators_Parameters *param_actuators = malloc(sizeof(Actuators_Parameters));

    param_move->speed = 4;
    param_move->angle = 0;
    param_move->x = 6;
    param_move->y = 6;
    param_actuators->sensors = 31;
    action_add(actions, PROPULSION, "Path finder", __propulsion_move, param_move, 2, __propulsion_move_failed_handler, NO);
    action_add(actions, ACTUATORS, "Actuator test", __actuators_arm, param_actuators, 2, __actuators_arm_failed_handler, NO);

    action_print_file(actions);
}


void strategy_handle_path_finder(Action *action)
{
    LOG(LOG_DEBUG, "Path finder handler");

    if (!strcmp(action->name,"Path finder"))
    {
        if (param_move != NULL)
        {
            if (param_move->x != 6)
            {
                LOG(LOG_DEBUG, "Didn't reach the position");
                LOG(LOG_DEBUG, "Actual position: %d",param_move->x);
            }else
                LOG(LOG_DEBUG, "Reach the position");
        }
    }
}


void strategy_management(Action *action)
{

    strategy_handle_path_finder(action);

}


void strategy_handle_state(File *actions)
{
    Action *action = actions->first;
    if (action == NULL)
        return;

    if (action->state == DONE || action->state == FAILED)
    {
        strategy_management(action);
        action_remove_by_index(actions, 1);
        action = action->next;
        if (action == NULL)
            return;

        if (action->state == DONE || action->state == FAILED)
        {
            strategy_management(action);
            action_remove_by_index(actions, 1);
        }

        return;
    }else
        strategy_management(action);

    action = action->next;
    if (action == NULL)
        return;

    if (action->state == DONE || action->state == FAILED)
    {
        strategy_management(action);
        action_remove_by_index(actions, 2);
    }else
        strategy_management(action);

}





/**************************************

          PUBLIC FUNCTIONS

**************************************/

void strategy_init(File *actions)
{
    LOG(LOG_INFO, "Init Strategy");

    strategy_example(actions);

}


void strategy_loop(File *actions)
{
    LOG(LOG_INFO, "Loop Strategy");

    strategy_handle_state(actions);
}
