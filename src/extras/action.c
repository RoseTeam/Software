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
#include "action.h"
#include "log.h"




/**************************************

          PRIVATE FUNCTIONS

**************************************/




/**************************************

          PUBLIC FUNCTIONS

**************************************/

File *action_init()
{
    File *file = malloc(sizeof(*file));
    file->first = NULL;

    return file;
}


void action_add(File *file, ACTION_TYPE type, char* name, void *action, void *data, int timeout, void *failed_handler, ACTION_PRIORITY priority)
{
    Action *new = malloc(sizeof(*new));
    if (file == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new->type = type;
    new->name = malloc(sizeof(strlen(name)));
    memcpy(new->name, name, strlen(name));
    new->action = action;
    new->data = data;
    new->state = INIT;
    new->timeout = timeout;
    new->failed_handler = failed_handler;
    new->priority = priority;
    new->next = NULL;

    if (file->first != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        Action *current = file->first;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->first = new;
    }
}


void action_remove_first(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        Action *action = file->first;
        file->first = action->next;
        free(action->name);
        free(action->data);
        free(action);
    }
}


void action_remove_by_index(File *file, int index)
{
    int i = 1;

    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if(index == 1)
    {
        action_remove_first(file);
        return;
    }


    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        Action *action_prev = file->first;
        while(i != (index-1)  && action_prev != NULL)
        {
            action_prev = action_prev->next;
            i++;
        }

        Action *action_to_delete = action_prev->next;

        action_prev->next = action_to_delete->next;
        
        free(action_to_delete->name);
        free(action_to_delete->data);
        free(action_to_delete);
    }
}


void action_remove_failed(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        Action *action = file->first;
        while(action->state == FAILED)
        {
            action_remove_first(file);
            action = file->first;
            if(action == NULL)
            {
                return;
            }
        }

        Action *action_next = action->next;

        while(action_next != NULL)
        {
            if(action_next->state == FAILED)
            {
                action->next = action_next->next;
                free(action_next->name);
                free(action_next->data);
                free(action_next);
            }
            action = action->next;
            action_next = action->next;
        }
    }
}


void action_remove_all(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        Action *action = file->first;
        Action *buffer;
     
        while(action == NULL)
        {
            buffer = action->next;
            free(action->name);
            free(action->data);
            free(action);
            action = buffer;
        }
    }

    file->first = NULL;

}


int action_get_index_by_name(File *file, char *name)
{
    int index = 0;
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Action *action = file->first;

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        while(action != NULL)
        {
            index++;
            if(!strcmp(name, action->name))
            {
                return index;
            }
            action = action->next;
        }
    }
    return 0;
}


Action *action_get_object(File *file, int index)
{
    int i = 1;

    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }


    /* On vérifie s'il y a quelque chose à défiler */
    if (file->first != NULL)
    {
        Action *action = file->first;
        while(i != (index-1) && action->next != NULL)
        {
            action = action->next;
            i++;
        }
        if(action != NULL)
        {
            return action;
        } else {
            return NULL;
        }
    }

}


ACTION_TYPE action_get_type(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return UNKNOWN_TYPE;
    } else {
        return action->type;
    }
}


char *action_get_name(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return NULL;
    } else {
        return action->name;
    }
}


void *action_get_action(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return NULL;
    } else {
        return action->action;
    }
}


void *action_get_data(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return NULL;
    } else {
        return action->data;
    }
}


int action_get_timeout(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return 0;
    } else {
        return action->timeout;
    }
}


void *action_get_failed_handler(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return NULL;
    } else {
        return action->failed_handler;
    }
}


ACTION_STATE action_get_state(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return UNKNOWN_STATE;
    } else {
        return action->state;
    }
}


ACTION_PRIORITY action_get_priority(File *file, int index)
{
    Action *action = action_get_object(file,index);
    if(action == NULL)
    {
        return UNKNOWN_PRIORITY;
    } else {
        return action->priority;
    }
}


void action_set_type(File *file, int index, ACTION_TYPE type)
{
    Action *action = action_get_object(file,index);
    action->type = type;
}


void action_set_name(File *file, int index, char *name)
{
    Action *action = action_get_object(file,index);
    free(action->name);
    action->name = malloc(sizeof(strlen(name)));
    memcpy(action->name, name, strlen(name));
}


void action_set_action(File *file, int index, void *action)
{    
    Action *action_t = action_get_object(file,index);
    action_t->action = action;
}


void action_set_data(File *file, int index, void *data)
{
    Action *action = action_get_object(file,index);
    action->data = data;
}


void action_set_timeout(File *file, int index, int timeout)
{
    Action *action = action_get_object(file,index);
    action->timeout = timeout;
}


void action_set_failed_handler(File *file, int index, void *failed_handler)
{
    Action *action = action_get_object(file,index);
    action->failed_handler = failed_handler;
}


void action_set_state(File *file, int index, ACTION_STATE state)
{
    Action *action = action_get_object(file,index);
    action->state = state;
}


void action_set_priority(File *file, int index, ACTION_PRIORITY priority)
{
    Action *action = action_get_object(file,index);
    action->priority = priority;
}


void action_print_file(File *file)
{     
    Action *action = file->first;
    LOG(LOG_DEBUG,"Type:\tName:\t\tTimeout:\tState:\tPriority:");
    if(action == NULL)
    {
        return;
    }

    while (action != NULL)
    {
        LOG(LOG_DEBUG,"%d\t%s\t\t%d\t%d\t%d", action->type, action->name, action->timeout, action->state, action->priority);
        action = action->next;
    }
}
