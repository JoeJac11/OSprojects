/*I promise that this source code file has, in its entirety, been
written by myself or a member of my group and by no other person or
persons. If at any time an exact copy of this source code is found to
be used by another person outside my group in this term, I understand
that all members of my group and the members of the group that
submitted the copy will receive a zero on this assignment.
*/
//Joseph Jacobson
#include <stdio.h>
#include <stdlib.h> 
#include "assn5.h"

#define MAX_PROCS (100)
#define TIME_Q (100)

void SJF(int *arr, int *burst, int count) {
    int ready[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS];
    int clock = 0;
    int ta = 0, wait = 0, resp = 0;
    int cur = 0, next = 0, insert = 0, inQ = 0;

    for (int i = 0; i < count; ++i) {
        if (cur == insert) {
            ready[insert++] = next;
            clock = arr[next];
            ++inQ;
            ++next;
        }
        start[ready[cur]] = clock;
        clock += burst[ready[cur]];
        end[ready[cur]] = clock;
        inQ;
        ta += end[ready[cur]] - arr[ready[cur]];
        wait += start[ready[cur]] - arr[ready[cur]];
        resp += start[ready[cur]] - arr[ready[cur]];
        while (next < count && arr[next] < clock){
            ready[insert++] = next++;
            ++inQ;
            for(int i = cur + 1; i < inQ + 1; ++i) {
                for (int j = i + 1; j < inQ; ++j){
                    if(burst[ready[i]] > burst[ready[j]]) {
                        int temp = ready[i];
                        ready[i] = ready[j];
                        ready[j] = temp;
                    }
                }
            }
        }
        ++cur;
    }
    printf("Shortest Job First\n");

    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count, (float) wait / count);
}

void SRTF(int *arr, int *burst, int count) {
    int ready[MAX_PROCS], remain[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS];
    int clock = 0;
    int ta = 0, wait = 0, resp = 0;  //turnaround, wait and response time
    int next = 0, curr = 0, insert = 0, inQ = 0;
    for (int i = 0; i < 100; ++i) {
        remain[i] = 0;
    }

    for (int i = 0; i < 100; ++i) { //still within arrays & not full
        if (insert <= count) {
            /*fill empty ready queue*/
            if (curr == insert) { //no jobs in ready queue, need to put one in
                ready[insert] = next;  //ready contains pointer to which job
                remain[next] = burst[next];
                clock = arr[next];
                start[next] = -1;
                next++;
                insert++;
                inQ++;
            }
            //run the next job in the RQ
            if (start[ready[curr]] < 0) { start[ready[curr]] = clock; }

            /*if it will have time left*/
            if (clock + remain[ready[curr]] > arr[next] && next < count) { //if something arrives while it's running
                remain[ready[curr]] -= arr[next] - clock;//calculate new remaining time
                clock = arr[next]; //move clock to next job arrival
            } else { /*if it is done*/
                clock += remain[ready[curr]]; //move clock to when it is done
                remain[ready[curr]] -= remain[ready[curr]];  //subtract remaining time
                end[ready[curr]] = clock; //record end time
                wait += end[ready[curr]] - burst[ready[curr]] - arr[ready[curr]];
                ta += end[ready[curr]] - arr[ready[curr]];
                resp += start[ready[curr]] - arr[ready[curr]];
                --inQ;
            }

            /*move arrived bursts to ready queue*/
            while (next < count && arr[next] <= clock) { //while more have arrived
                ready[insert] = next;
                remain[next] = burst[next];
                start[next] = -1;
                next++;
                insert++;
                inQ++;
            }

            // find next shortest job
            int min = 0;
            for (int i = 0; i < insert; ++i) {
                if (remain[i] != 0) {
                    if (min == 0) {
                        min = i;
                    } else if (remain[i] < remain[min]) {
                        min = i;
                    }
                }
                curr = min;
                if (inQ == 0) {
                    curr = insert;
                }
            }
        }
    }
    printf("Shortest Time Remaining First: \n");

    //calculate stats
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count,
           (float) wait / count);
}
