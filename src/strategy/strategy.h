#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdio.h>
#include "action.h"

void strategy_init(File *actions);

void strategy_loop(File *actions);

#endif
