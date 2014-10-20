#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <stdio.h>
#include "action.h"

void actuators_init(void);

void actuators_loop(File *actions);

#endif
