#include <stdio.h>
#include <stdlib.h>
#include "strategy.h"
#include "log.h"
#include "actuators.h"
#include "propulsion.h"
#include "move.h"
#include "arm.h"




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void strategy_example(Action *actions)
{
    Propulsion_Parameters *param_propulsion = malloc(sizeof(Propulsion_Parameters));
    Actuators_Parameters *param_actuators = malloc(sizeof(Actuators_Parameters));

    param_propulsion->speed = 4;
    param_actuators->sensors = 31;
    action_add(actions, PROPULSION, "Propulsion test", __propulsion_move, param_propulsion, 2, __propulsion_move_failed_handler, NO);
    action_add(actions, ACTUATORS, "Actuator test", __actuators_arm, param_actuators, 2, __actuators_arm_failed_handler, NO);

    action_print_file(actions);
}


void strategy_handle_state(File *actions)
{
    Action *action = actions->first;
    if(action == NULL)
    {
        return;
    }

    if(action->state == DONE || action->state == FAILED)
        action_remove_by_index(actions, 1);

    action = action->next;
    if(action == NULL)
    {
        return;
    }

    if(action->state == DONE || action->state == FAILED)
        action_remove_by_index(actions, 2);

    action_print_file(actions);
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
