#ifndef AI_TES_H
#define AI_TES_H

#include "head_dlltest.h"
#include "reversing/reverser.h"
#include "reversing/rev_replstate.h"

void ai_conduction(joydata* jo,replstate& re,revdata& rd);
void ai_init();
void dumpdat();

#endif