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
