#include <stdio.h>
#include <stdlib.h>
#include "propulsion.h"
#include "log.h"
#include "timer.h"
#include "move.h"


volatile Action *propulsion_action_handler;
volatile bool propulsion_timer_expired;
bool propulsion_set_timer;




/**************************************

          PRIVATE FUNCTIONS

**************************************/

void propulsion_handle_state(Action *propulsion, ACTION_STATE ret)
{
    if(propulsion_timer_expired == false)
    {
        propulsion->state = ret;

        if(propulsion->state == DONE)
        {
            timer1_reset();
            propulsion_timer_expired = false;
            propulsion_set_timer = false;
        }

    }
    else
    {

        propulsion->state = FAILED;
        timer1_reset();
        propulsion_timer_expired = false;
        propulsion_set_timer = false;

    }
}


void propulsion_callback_timer(int signal)
{
    LOG(LOG_DEBUG, "Propulsion Timer Expired");
    if(propulsion_action_handler->failed_handler != NULL)
    {
        propulsion_action_handler->failed_handler();
    }
    propulsion_action_handler->action = NULL;
    propulsion_action_handler->failed_handler = NULL;
    propulsion_timer_expired = true;
}


void propulsion_management(Action *propulsion)
{
    ACTION_STATE ret;

    if(propulsion_set_timer == false)
    {
        propulsion_action_handler = propulsion;
        timer1_set(propulsion->timeout);
        propulsion_set_timer = true;
    }

    if(propulsion->action != NULL)
    {
        ret = propulsion->action(propulsion->data);
    }
    propulsion_handle_state(propulsion, ret);
}




/**************************************

          PUBLIC FUNCTIONS

**************************************/

void propulsion_init(void)
{
    LOG(LOG_INFO, "Init Propulsion");
    propulsion_timer_expired = false;
    propulsion_set_timer == false;
    timer1_add(propulsion_callback_timer);
}


void propulsion_loop(File *actions)
{
    Action *propulsion = actions->first;
    if(propulsion == NULL)
    {
        return;
    }

    LOG(LOG_INFO, "Loop Propulsion");

    if(propulsion->type == PROPULSION  || (propulsion->priority > NO))
    {

        if(propulsion->type == PROPULSION && (propulsion->state == INIT || propulsion->state == ONGOING))
        {
            propulsion_management(propulsion);
        }

    }else{

        propulsion = propulsion->next;
        if(propulsion == NULL)
        {
            return;
        }

        if(propulsion->type == PROPULSION && (propulsion->state == INIT || propulsion->state == ONGOING))
        {

            propulsion_management(propulsion);

        }

    }
}
