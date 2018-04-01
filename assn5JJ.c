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
    int ready[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS], remain[MAX_PROCS];
    int clock = 0;
    int ta = 0, wait = 0, resp = 0;
    int next = 0, cur = 0, insert = 0, finished = 0;
   
    for (int i = 0; i < count; ++i){
        start[i] = -1;
        remain[i] = burst[i];
    }
 
    while (finished < count)  {
        if (cur == insert) {//empty queue
            ready[insert] = next;
            clock = arr[next];
            insert = (insert + 1) % MAX_PROCS;
            ++next;
        }

        if (start[ready[cur]] < 0) start[ready[cur]] = clock;

        if (remain[ready[cur]] == 0) {
            cur = (cur + 1) % MAX_PROCS;
            continue;
        }

        if (clock + remain[ready[cur]] < arr[next]){//job is done
            clock += remain[ready[cur]];
            remain[ready[cur]] = 0;
            end[ready[cur]] = clock;
            ++finished;
            cur = (cur + 1) % MAX_PROCS; //remove from queue
        }
        else {//has time left
            remain[ready[cur]] -= arr[next] - clock;
            ready[insert] = next;
            insert = (insert + 1) % MAX_PROCS;
            clock = arr[next];
            ++next; //add a new job to the queue
            ready[insert] = ready[cur];
            insert = (insert + 1) % MAX_PROCS;
            cur = (cur + 1) % MAX_PROCS;
        }
        
        for(int i = cur; i != insert; ++i) {//sort ready queue
            for (int j = i + 1; j != insert; ++j){
                if(remain[ready[i]] > remain[ready[j]]) {
                    int temp = ready[i];
                    ready[i] = ready[j];
                    ready[j] = temp;
                }
            }
        }
    }
        
 
    for(int i = 0; i < count; ++i){
        int curTa = end[i] - arr[i];
        ta += curTa;
        wait += curTa - burst[i];
        resp += start[i] - arr[i];
    }

    printf("Shortest Remaining Time First\n");
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count, (float) wait / count);
}
