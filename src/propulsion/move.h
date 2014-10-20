#ifndef MOVE_H
#define MOVE_H

#include <stdio.h>
#include "action.h"

typedef struct Propulsion_Parameters Propulsion_Parameters;
struct Propulsion_Parameters
{
    int speed;
};

ACTION_STATE __propulsion_move(Propulsion_Parameters *parameters);

void __propulsion_move_failed_handler(void);

#endif
