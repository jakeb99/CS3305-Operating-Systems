#include <stdlib.h>
#include "scheduling.h"

/*
##  Round-Robin Scheduling Algorithm  ##

	Parameters:-

	queue: 1D int array containing the CPU burst time for every process in a ready queue
	np: number of processes (length of `queue` array)
	tq: time quantum for the RR algorithm


	Returns:-

	rr_result struct pointer, where
	rr_result -> order: 1D int array, containing the execution order of processes (i.e. `(rr_result -> order)[0]` is the process number of the first process)
		** hint: p1 is always the first item in order of selection in round-robin and FCFS
    rr_result -> order_n: int, representing the size of the `order` array, described above ^
	rr_result -> turnarounds: 1D int array, containing the turnaround time for each process (i.e. `(rr_result -> turnarounds)[0]` is the turnaround time for p1)
*/
rr_result *rr(int *queue, int np, int tq)
{
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);

	// code here to assign values to result->turnarounds, result->order, and result->order_n
	
	/* get the size of the order array */
	int size = getOrderSize(queue, np, tq);
	result->order = malloc(sizeof(int) * size);		// allocate memory for order array
	result->order_n = size;							// set size of order array

	int time = 0;			// counter for totala burt time elapsed
	int timeRemaining = 0;	// burst time remaining after a process is executed
	int startTimes[np];		// array to track start time of each process
	int endTimes[np];		// array to track end time of each process

	int j = 0;				// counter for index of order

	/* calculate total burst time */
	int totalBurstTime = getTotalBurstTime(queue, np);

	/* while there are processes with remaining burt time */
	while (time != totalBurstTime)
	{
		/* loop over the queue */
		for (int i = 0; i < np; i++)
		{
			int current = queue[i];			// the process to execute

			/* check if the process has already finished */
			if (current == 0)
				continue;			// go to next i
			else
			{
				/* add start time of process if this is the
				first time a process has been executed */
				if (j < np)
					startTimes[i] = time;
				timeRemaining = current - tq;

				/* if true, insert how much time remaing for the process */
				if (current > tq)
				{
					queue[i] = timeRemaining;
					time += tq;
				}
				else	// i.e., current <= tq
				{
					queue[i] = 0;			// burst time remaining is zero
					time += current;
					endTimes[i] = time;		// process is complete so record its end time
				}

				/* add process number to order */
				result->order[j] = i;		
				j++;
			}
			/* check if all processes have complete */
			if (time == totalBurstTime)
				break;
		}
	}
	/* calculate turnaround time for each process */
	calcTurnarounds(result, startTimes, endTimes, np);

	return result;
}

// helper function to get the size of the order array
// divides each burst time by the tq (rounding up) to
// get the size of order array
int getOrderSize(int *queue, int np, int tq)
{
	int size = 0;
	for (int i = 0; i < np; i++)
	{
		int x = queue[i];
		size += x/tq + (x % tq != 0);
	}
	return size;
}

// helper function to calculate the total
// burst time of the processes in the queue
int getTotalBurstTime(int *queue, int np)
{
	int total = 0;
  	for (int i = 0; i < np; i++)
  	{
    	total += queue[i];
  	}
  	return total;
}

// helper function to calc turn around time for each process
void calcTurnarounds(rr_result *result, int *startTimes, int *endTimes, int np)
{
	int *turnarroundArr = calloc(np, sizeof(int));
  	for (int i = 0; i < np; i++)
  	{
    	result->turnarounds[i] = endTimes[i] - startTimes[i];
  	}
}