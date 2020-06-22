#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sched_algos.h"

void fillProcessArr(struct process ** processArr, int arrSize) { //Helper function to fill our array with randomly generated processes.
    int currIdx = 0;
    while(currIdx < arrSize) {
        processArr[currIdx] = malloc(sizeof(struct process));
        processArr[currIdx]->id = currIdx + 1;
        processArr[currIdx]->duration = rand() % 5 + 1;
        processArr[currIdx]->priorty = rand()%9+1;
        currIdx++;
    }
}

void outputProcess(struct process *process) { //Helper function to output a single process.
    printf("Process ID: %i, Duration: %i, Priority: %i\n", process->id, process->duration, process->priorty);
}

void outputProcesses(struct process ** processes, int arrSize) {
    int idx = 0;
    while(idx < arrSize) {
        outputProcess(processes[idx]);
        idx++;
    }
}

void fillPrioritiesArr(struct process ** priorities, int arrSize) {
    int priorIdx = 0;
    while(priorIdx < arrSize) {
        priorities[priorIdx] = malloc(sizeof(struct process));
        priorities[priorIdx]->priorty = rand() % 9 + 1;
        priorIdx++;
    }
}

void swap(struct process **processes, int a, int b){
//    printf("changed\n");
    struct process* temp;
    temp = processes[a];
    processes[a] = processes[b];
    processes[b] = temp;
}

void heapifypr(struct process ** arr, int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    
    
    if (l < n && arr[l]->priorty > arr[largest]->priorty)
        largest = l;
    
    
    if (r < n && arr[r]->priorty > arr[largest]->priorty)
        largest = r;
    
    
    if (largest != i)
    {

        swap(arr, i,largest);

        heapifypr(arr, n, largest);
    }
}


void heapSortpr(struct process ** arr, int n)
{
    
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifypr(arr, n, i);
    
    
    for (int i=n-1; i>=0; i--)
    {
        
        swap(arr,0,i);
        
        
        heapifypr(arr, i, 0);
    }
}

void heapifytime(struct process ** arr, int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    
    
    if (l < n && arr[l]->duration > arr[largest]->duration)
        largest = l;
    
    
    if (r < n && arr[r]->duration > arr[largest]->duration)
        largest = r;
    
    
    if (largest != i)
    {
        
        swap(arr, i,largest);
        
        heapifytime(arr, n, largest);
    }
}


void heapSorttime(struct process ** arr, int n)
{
    
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifytime(arr, n, i);
    
   
    for (int i=n-1; i>=0; i--)
    {
        
        swap(arr,0,i);
    
        heapifytime(arr, i, 0);
    }
}

int main(int argc, char ** argv) {
    srand(time(NULL));
    const int ARR_SIZE = 10;
    const int TIME_SLICE = 2;
    struct process ** processes = malloc(sizeof(struct process *) * ARR_SIZE);
    struct process ** lay = malloc(sizeof(struct process *) * ARR_SIZE);
    int ** priorities = malloc(sizeof(int *) * ARR_SIZE);
    
    fillProcessArr(processes, ARR_SIZE);
   
//    for(int i=0;i<ARR_SIZE;i++){
//        lay[i] = processes[i];
//        printf("%d %d %d\n", lay[i]->id, lay[i]->duration, lay[i]->priorty);
//    }
    
    printf("Outputting initial process array:\n");
    outputProcesses(processes, ARR_SIZE);
    printf("\n\n");
    
    
    sched_fcfs(processes, ARR_SIZE);
    sched_priority(processes, ARR_SIZE, priorities);
    sched_spn(processes, ARR_SIZE);
    sched_srt(processes, ARR_SIZE, TIME_SLICE);
    sched_round_robin(processes, ARR_SIZE, TIME_SLICE);

    
}

void sched_fcfs(struct process ** processes, int arrSize) {
    
    printf("First Come First Serve:\n");
    int currIdx = 0;
    int sum_time = 0;
    while(currIdx < arrSize){
        sum_time+=processes[currIdx]->duration;
        printf("Process ID: %i, Duration Sum Time: %i\n", processes[currIdx]->id, sum_time);
        currIdx++;
    }
    printf("\n\n");
}

void sched_priority(struct process ** processes, int arrSize, int ** priorities) {
    
    printf("Priority:\n");
    int currIdx = 0;
    int sum_time = 0;
    heapSortpr(processes, arrSize);
    while(currIdx < arrSize){
        sum_time+=processes[currIdx]->duration;

        printf("Process ID: %d, sum_time: %d Priorty: %d\n", processes[currIdx]->id, sum_time, processes[currIdx]->priorty);
        currIdx++;
    }
    printf("\n\n");
}

void sched_spn(struct process ** processes, int arrSize) {
    //First Sort all the processes increasing order ID and duration, apply FCFS algorithm.
    printf("Shortest Process Next:\n");
    int currIdx = 0;
    int sum_time = 0;
    heapSorttime(processes, arrSize);
    while(currIdx < arrSize){
        sum_time+=processes[currIdx]->duration;
        printf("Process ID: %i, Duration: %i, Duration Sum Time: %i\n", processes[currIdx]->id, processes[currIdx]->duration, sum_time);
        currIdx++;
    }
    
    printf("\n\n");
}

void sched_srt(struct process ** processes, int arrSize, int timeSlice) {
//    // First find timeslice is < 1, use time slice to finish remained project.
//    printf("Shortest Remaining Time:\n");
//    int currIdx = 0;
//    int sum_time = 0;
//    heapSorttime(processes, arrSize);
//    while(currIdx < arrSize) {
//        if (processes[currIdx]->duration <= timeSlice){
//            sum_time+= processes[currIdx]->duration;
//            processes[currIdx]->duration=0;
//            printf("Process ID: %i, Duration Sum Time: %i\n", processes[currIdx]->id, sum_time);
//            currIdx++;
//            continue;
//        }
//        sum_time += timeSlice;
//        processes[currIdx]->duration -= timeSlice;
//    }
//    printf("sum : %d\n", sum_time);
}

void sched_round_robin(struct process ** processes, int arrSize, int timeSlice) {
    // use time slice to finish by ID.
    printf("Round Robin:\n");
    int currIdx = 0;
    int sum_time = 0;
    int max = processes[0]->duration;
    printf("%d\n", max);
    for(int i=1;i<arrSize;i++){
        if(processes[i]->duration > max){
            max = processes[i]->duration;
        }
    }
    int max_loop = (max/2)+1;
    for(int i=0;i<max_loop;i++){
        for(currIdx =0; currIdx<arrSize; currIdx++){
            if(processes[currIdx]->duration == 0) continue;
            if(processes[currIdx]->duration <= timeSlice){
                sum_time+= processes[currIdx]->duration;
                processes[currIdx]->duration=0;
                printf("Process ID: %i, Duration Sum Time: %i\n", processes[currIdx]->id, sum_time);
            }
            else{
                sum_time+= timeSlice;
                processes[currIdx]->duration -= timeSlice;
            }
        }
    }
    printf("\n\n");
}
        


