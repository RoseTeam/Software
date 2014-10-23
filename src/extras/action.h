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

#ifndef ACTION_H
#define ACTION_H


typedef enum
{
    PROPULSION,
    ACTUATORS,
    UNKNOWN_TYPE
}ACTION_TYPE;


typedef enum
{
    INIT,
    ONGOING,
    DONE,
    FAILED,
    UNKNOWN_STATE
}ACTION_STATE;


typedef enum
{
    NO,
    MEDIUM,
    HIGH,
    UNKNOWN_PRIORITY
}ACTION_PRIORITY;


typedef struct Action Action;
struct Action
{
    ACTION_TYPE type;
    char *name;
    ACTION_STATE (*action) (void *data);
    void *data;
    int timeout;
    void (*failed_handler) (void);
    ACTION_STATE state;
    ACTION_PRIORITY priority;
    Action *next;
};


typedef struct File File;
struct File
{
    Action *first;
};


File *action_init();

/* Add functions */
void action_add(File *file, ACTION_TYPE type, char* name, void *action, void *data, int timeout, void *failed_handler, ACTION_PRIORITY priority);

/* Remove functions */
void action_remove_first(File *file);

void action_remove_by_index(File *file, int index);

void action_remove_failed(File *file);

void action_remove_all(File *file);

/* Get functions */
int action_get_index_by_name(File *file, char* name);

Action *action_get_object(File *file, int index);

ACTION_TYPE action_get_type(File *file, int index);

char *action_get_name(File *file, int index);

void *action_get_action(File *file, int index);

void *action_get_data(File *file, int index);

int action_get_timeout(File *file, int index);

void *action_get_failed_handler(File *file, int index);

ACTION_STATE action_get_state(File *file, int index);

ACTION_PRIORITY action_get_priority(File *file, int index);

/* Set functions */
void action_set_type(File *file, int index, ACTION_TYPE type);

void action_set_name(File *file, int index, char *name);

void action_set_action(File *file, int index, void *action);

void action_set_data(File *file, int index, void *data);

void action_set_timeout(File *file, int index, int timeout);

void action_set_failed_handler(File *file, int index, void *failed_handler);

void action_set_state(File *file, int index, ACTION_STATE state);

void action_set_priority(File *file, int index, ACTION_PRIORITY priority);

/* Log functions */
void action_print_file(File *file);

void action_journal(File *file);


#endif
