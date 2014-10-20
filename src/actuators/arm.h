#ifndef ARM_H
#define ARM_H

#include <stdio.h>
#include "action.h"

typedef struct Actuators_Parameters Actuators_Parameters;
struct Actuators_Parameters
{
    int sensors;
};

ACTION_STATE __actuators_arm(Actuators_Parameters *parameters);

void __actuators_arm_failed_handler(void);

#endif
