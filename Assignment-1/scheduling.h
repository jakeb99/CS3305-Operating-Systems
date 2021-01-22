#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "types.h"

rr_result *rr(int *queue, int np, int tq);

// helper funciton declarations
int getOrderSize(int *queue, int np, int tq);
int getTotalBurstTime(int *queue, int np);
void calcTurnarounds(rr_result *result, int *startTimes, int *endTimes, int np);

#endif
