/* Promise of Originality
I, Kris Olson,  promise that this source code file has, in its entirety, been
written by myself or a member of my group and by no other person or
persons. If at any time an exact copy of this source code is found to
be used by another person outside my group in this term, I understand
that all members of my group and the members of the group that
submitted the copy will receive a zero on this assignment.
*/

#include <stdio.h>
#include <stdlib.h>
#include "assn5.h"

#define MAX_PROCS (100)
#define TIME_Q (100)

void FCFS(int *arr, int *burst, int count) {
    int start[MAX_PROCS]; //start time array
    int end[MAX_PROCS];  //end time array
    int ready[MAX_PROCS];  //ready queue
    int clock = 0; //clock time counter
    /*turn around time, wait time, response time*/
    int ta = 0, wait = 0, resp = 0;
    /*indices for next job, current job and insert into RQ, count of jobs, numbers from file*/
    int next = 0, curr = 0, insert = 0;

    for (int i = 0; i < count; ++i) {
        //if no jobs in queue, need to put one in
        if (curr == insert) {
            ready[insert++] = next;  //put next job in ready queue
            clock = arr[next];  //if next job hasn't arrived yet, jump clock to arrival time
            next++;
        }

        //run the next job in the RQ
        start[curr] = clock;
        clock += burst[curr];
        end[curr] = clock;
        ta += end[curr] - arr[curr];
        wait += start[curr] - arr[curr];
        resp += start[curr] - arr[curr];

        //check to see if new jobs have arrived
        while (next < count && arr[next] < clock) {
            //if yes, put them on the RQ
            ready[insert++] = next++;
        }
        curr++;  //go to next job
    }

    printf("First Come, First Served\n");

    //calculate stats
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count,
           (float) wait / count);
}


//void SRTF(int *arr, int *burst, int count) {
//    int ready[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS], remain[MAX_PROCS];
//    int clock = 0;
//    int ta = 0, wait = 0, resp = 0;
//    int next = 0, cur = 0, insert = 0, inQ = 0;
//
//    for (int i = 0; i < count; ++i) { //loop to handle all of the count jobs
//        if (cur == insert) { //no jobs in queue
//            ready[insert++] = next;  //add next one to ready and inc insert
//            remain[next] = burst[next];  //set the job in the queue remaining time as full burst
//            clock = arr[next];  //move the clock to the beginning of the arrivals
//            start[next] = -1;  //set the start time to -1 so we can record when it actually starts
//            ++inQ;  //counter for what's in the queue inc
//            ++next;  //inc next job
//        }
//
//        if (start[ready[cur]] < 0) {start[ready[cur]] = clock;}  //save the first start time
//
//        if (clock + remain[ready[cur]] <= arr[next]){ //if job finishes before next arrives
//			printf("here\n");
//            end[ready[cur]] = clock + remain[ready[cur]];  //record end of job
//            start[next] = -1; //put arrived job on queue
//            ++inQ;  //inc #jobs on queue
//            remain[next] = burst[next]; //still putting on queue
//            clock = arr[next]; //moving clock to next job (might be a gap here)
//        }
//        else {  //job is running when next arrives this is pausing to see if the new job is shorter
//            remain[ready[cur]] -= arr[next] - clock;  //subtract the time it ran before new job arrived
//            clock = arr[next];  //now we're at the new job arrival time
//
//            ready[insert++] = next;
//            ++inQ;
//            for(int i = cur; i < inQ; ++i) {
//                for (int j = i + 1; j < inQ-1; ++j){
//                    if(remain[ready[i]] > remain[ready[j]]) {
//                        int temp = ready[i];
//                        ready[i] = ready[j];
//                        ready[j] = temp;
//                    }
//                }
//            }
//        }
//		++next;
//        ++cur;
//    }
//
//    for(int i = 0; i < count; ++i){
//        ta += end[i] - arr[i];
//        wait += end[i] - arr[i] - burst[i];
//        resp += start[i] - arr[i];
//    }
//
//    printf("Ready Q\n");
//    for (int k = 0; k < count; ++k) {
//        printf("%d\n");
//    }
//    printf("Shortest Remaining Time First\n");
//    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count, (float) wait / count);
//}


void RoundRobin(int *arr, int *burst, int count) {

    int ready[MAX_PROCS], remain[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS];
    int clock = 0;
    int ta = 0, wait = 0, resp = 0;  //turnaround, wait and response time
    int next = 0, curr = 0, insert = 0;

    while (next < count || curr != insert) { //still within arrays & not full
        /*fill empty ready queue*/
        if (curr == insert) { //no jobs in ready queue, need to put one in
            ready[insert] = next;  //ready contains pointer to which job
            remain[next] = burst[next];
            clock = arr[next];
            start[next] = -1;
            next++;
            insert++;
        }

        //run the next job in the RQ
        if (start[ready[curr]] < 0) { start[ready[curr]] = clock; }

        /*if it will have time left*/
        if (remain[ready[curr]] > TIME_Q) {
            clock += TIME_Q;
            remain[ready[curr]] -= TIME_Q;
        } else { /*if it is done*/
            clock += remain[ready[curr]];
            remain[ready[curr]] -= remain[ready[curr]];
            end[ready[curr]] = clock;
            wait += end[ready[curr]] - burst[ready[curr]] - arr[ready[curr]];
            ta += end[ready[curr]] - arr[ready[curr]];
            resp += start[ready[curr]] - arr[ready[curr]];
        }

        /*move arrived bursts to ready queue*/
        while (next < count && arr[next] <= clock) {
            ready[insert] = next;
            remain[next] = burst[next];
            start[next] = -1;
            next++;
            insert++;
        }

        if (remain[ready[curr]] > 0) {
            /*burst still has time, add to bottom of queue*/
            ready[insert] = ready[curr];
            insert++;
        }

        /*increment to next job in ready queue*/
        curr++;
    }
    printf("Round Robin with Time Quantum of %d\n", TIME_Q);

    //calculate stats
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count,
           (float) wait / count);
}

int main(int argc, char *argv[]) {
    printf("\nFCFS & Round Robin by Kris Olson\n");
    printf("SJF & SRTF by Joseph Jacobson\n");
    printf("Project Team: Kris Olson, Joseph Jacobson\n\n");

    FILE *fp;
    int arr[MAX_PROCS];  //arrival time array
    int burst[MAX_PROCS];  //burst time array
    int count = 0;  //count of jobs

    //read args
    if (argc != 2) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (NULL == fp) {
            printf("File not found.\n");
            return -1;
        }
    }

    /*read from file into arrays*/
    while (fscanf(fp, "%d %d", &arr[count], &burst[count]) == 2) {
        count++; //count of jobs
    }

    FCFS(arr, burst, count);
    SJF(arr, burst, count);
    SRTF(arr, burst, count);
    RoundRobin(arr, burst, count);

    return 0;
}
