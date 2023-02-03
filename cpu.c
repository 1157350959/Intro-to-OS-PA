#include <stdio.h>
#include <stdbool.h>
#include "oslabs.h"

#define is_any_PCB_running is_equal_NULLPCB
#define is_ready_queue_empty is_any_PCB_running

struct PCB NULLPCB = {0, 0, 0, 0, 0, 0, 0};

/*
    @brief The method compares the input argument with NULLPCB
    @param pcb: PCB to be compared with NULLPCB
    @retval true if pcb == NULLPCB, false otherwise
*/
bool is_equal_NULLPCB(struct PCB pcb)
{
    if (pcb.process_id == NULLPCB.process_id &&
        pcb.arrival_timestamp == NULLPCB.arrival_timestamp &&
        pcb.total_bursttime == NULLPCB.total_bursttime &&
        pcb.execution_starttime == NULLPCB.execution_starttime &&
        pcb.execution_endtime == NULLPCB.execution_endtime &&
        pcb.remaining_bursttime == NULLPCB.remaining_bursttime &&
        pcb.process_priority == NULLPCB.process_priority)
    {
        return true;
    }
    return false;
}

/*
    @brief  The method compares two integer and returns the smaller one
    @param  int1: integer 1
    @param  int2: integer 2
    @retval the smaller integer
*/
int min(int int1, int int2)
{
    return (int1 < int2) ? int1 : int2;
}

/*
    @brief  This function implements the logic to handle the arrival of a new process in a Priority-based Preemptive Scheduler
    @param  ready_queue: the ready queue (an array of PCB structs)
    @param  queue_cnt : the number of items in the ready queue
    @param  current_process: the PCB of the currently running process
    @param  new_process: the PCB of the newly arriving process
    @param  timestamp: the current timestamp
    @retval   the PCB to be executed next
*/
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{ 
    if(is_any_PCB_running(current_process))
    {   
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else
    {
        if(new_process.process_priority >= current_process.process_priority)
        {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt += 1;
            return current_process;
        }
        else
        {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime -= timestamp - current_process.arrival_timestamp;
            ready_queue[*queue_cnt] = current_process;
            *queue_cnt += 1;
            return new_process;
        }
    }
}

/*
    @berif  This method implements the logic to handle the completion of execution of a process in a Priority-based Preemptive Scheduler
    @param ready_queue: the ready queue (an array of PCB structs)
    @param queue_cnt: the number of items in the ready queue
    @param timestamp: the current timestamp
    @retval the PCB to be executed next
*/
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, int timestamp)
{
    if(*queue_cnt == 0)
    {
        return NULLPCB;
    }
    else
    {
        int highest_priority = ready_queue[0].process_priority;
        struct PCB next_PCB = ready_queue[0];
        int next_PCB_idx = 0;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if(ready_queue[i].process_priority < highest_priority)
            {
                highest_priority = ready_queue[i].process_priority;
                next_PCB = ready_queue[i];
                next_PCB_idx = i;
            }
        }
        for(int i = next_PCB_idx; i < *queue_cnt; i++)
        {
            if(next_PCB_idx == QUEUEMAX)
            {
                ready_queue[next_PCB_idx] = NULLPCB;
            }
            else
            {
                ready_queue[i] = ready_queue[i + 1];
            }
        }
        *queue_cnt -= 1;
        next_PCB.execution_starttime = timestamp;
        next_PCB.execution_endtime = timestamp + next_PCB.remaining_bursttime;
        return next_PCB;
    }
}

/*
    @brief  This method implements the logic to handle the arrival of a new process in a Shortest-Remaining-Time-Next Preemptive Scheduler
    @param  ready_queue: the ready queue (an array of PCB structs)
    @param  queue_cnt: the number of items in the ready queue
    @param  current_process: the PCB of the currently-running process
    @param  new_process: the PCB of the newly-arriving process 
    @param  time_stamp: the current timestamp
    @retval the PCB to be executed next
*/
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp)
{
    if(is_any_PCB_running(current_process))
    {   
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }
    else
    {
        if(new_process.total_bursttime >= current_process.remaining_bursttime)
        {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt += 1;
            return current_process;
        }
        else
        {
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            current_process.execution_starttime = 0;
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime -= time_stamp - current_process.arrival_timestamp;
            ready_queue[*queue_cnt] = current_process;
            *queue_cnt += 1;
            return new_process;
        }
    }
}

/*
    @brief  This method implements the logic to handle the completion of execution of a process in a Shortest-Remaining-Time Preemptive Scheduler
    @param ready_queue: the ready queue (an array of PCB structs)
    @param queue_cnt: the number of items in the ready queue
    @param timestamp: the current timestamp
    @retval the PCB to be executed next
*/
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, int timestamp)
{
    if(*queue_cnt == 0)
    {
        return NULLPCB;
    }
    else
    {
        int min_remaining_time = ready_queue[0].remaining_bursttime;
        struct PCB next_PCB;
        int next_PCB_idx = 0;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if(ready_queue[i].remaining_bursttime < min_remaining_time)
            {
                min_remaining_time = ready_queue[i].remaining_bursttime;
                next_PCB = ready_queue[i];
                next_PCB_idx = i;
            }
        }
        for(int i = next_PCB_idx; i < *queue_cnt; i++)
        {
            if(i == QUEUEMAX)
            {
                ready_queue[next_PCB_idx] = NULLPCB;
            }
            else
            {
                ready_queue[i] = ready_queue[i + 1];
            }
        }
        *queue_cnt -= 1;
        next_PCB.execution_starttime = timestamp;
        next_PCB.execution_endtime = timestamp + next_PCB.remaining_bursttime;
        return next_PCB;
    }
}

/*
    @brief  This method implements the logic to handle the arrival of a new process in a Round-Robin Scheduler
    @param  ready_queue: the ready queue (an array of PCB structs)
    @param  queue_cnt: the number of items in the ready queue
    @param  current_process: the PCB of the currently-running process
    @param  new_process: the PCB of the newly-arriving process 
    @param  time_stamp: the current timestamp
    @param  time_quantum: the time quantum
    @retval the PCB to be executed next
*/
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{
    if(is_any_PCB_running(current_process))
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + min(time_quantum, new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else
    {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
}

/*
    @brief  This method implements the logic to handle the completion of execution of a process in a Round-Robin Scheduler

*/
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int * queue_cnt, int time_stamp, int time_quantum)
{
    if(*queue_cnt == 0) 
    {
        return NULLPCB;
    }
    else
    {
        int earliest_arrival_time = ready_queue[0].arrival_timestamp;
        struct PCB next_PCB = ready_queue[0];
        int next_PCB_idx = 0;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if(ready_queue[i].arrival_timestamp < earliest_arrival_time)
            {
                earliest_arrival_time = ready_queue[i].arrival_timestamp;
                next_PCB = ready_queue[i];
                next_PCB_idx = i;
            }
        }
        for(int i = next_PCB_idx; i < *queue_cnt; i++)
        {
            if(i == QUEUEMAX)
            {
                ready_queue[i] = NULLPCB;
            }
            else
            {
                ready_queue[i] = ready_queue[i + 1];
            }
        }
        *queue_cnt -= 1;
        next_PCB.execution_starttime = time_stamp;
        next_PCB.execution_endtime = time_stamp + min(time_quantum, next_PCB.remaining_bursttime);
        for(int i = 0; i < *queue_cnt; i++)
        {
            printf("%d%d%d%d%d%d%d\n",ready_queue[i].process_id,ready_queue[i].arrival_timestamp,ready_queue[i].total_bursttime,ready_queue[i].execution_starttime,ready_queue[i].execution_endtime,ready_queue[i].remaining_bursttime,ready_queue[i].process_priority);
        }
        printf("%d\n",*queue_cnt);
        printf("%d%d%d%d%d%d%d\n",next_PCB.process_id,next_PCB.arrival_timestamp,next_PCB.total_bursttime,next_PCB.execution_starttime,next_PCB.execution_endtime,next_PCB.remaining_bursttime,next_PCB.process_priority);
        return next_PCB;
    }
}

int main()
{   
    //------------------------------------completion rr--------------------
    struct PCB rq[] = {{1,22,8,0,0,8,0},{2,21,8,0,0,8,0},{3,23,8,0,0,8,0}};
    int qc = 3;
    int ts = 24;
    int tq = 10;
    struct PCB rpcb = handle_process_completion_rr(rq,&qc,ts,tq);

    //-------------------------------arrival rr----------------------------
    // struct PCB rq[] = {0};
    // int qc = 0;
    // struct PCB cp = {1,1,8,1,9,8,0};
    // struct PCB np = {2,2,8,0,0,8,0};
    // int ts = 2;
    // int tq = 6;
    // struct PCB rpcb = handle_process_arrival_rr(rq,&qc,cp,np,ts,tq);

    //-----------------------------completion srtp----------------------
    // struct PCB ready_queue[QUEUEMAX] = {{1,1,23,0,0,23,0},{2,1,22,0,0,22,0},{3,1,24,0,0,24,0}};
    // int qc = 3;
    // int ts = 2;
    // struct PCB rpcb = handle_process_completion_srtp(ready_queue,&qc,ts);


    //------------------------------arrival srtp-------------------------
    // struct PCB ready_queue[QUEUEMAX] = {0};
    // int queue_cnt = 0;
    // struct PCB cp = {1,1,8,1,9,8,0};
    // struct PCB np = {2,2,6,0,0,6,0};
    // int ts = 2;

    // struct PCB rpcb = handle_process_arrival_srtp(ready_queue, &queue_cnt, cp, np, ts);

    //--------------------------------completion pp-------------------------
    // struct PCB ready_queue[QUEUEMAX] = {{1,1,4,0,0,4,23},{2,1,4,0,0,4,22},{3,1,4,0,0,4,24}};
    // int qc = 3;
    // int ts = 2;
    // struct PCB rpcb = handle_process_completion_pp(ready_queue,&qc,ts);



    //---------------------------------------arrival pp-----------------------
    // struct PCB ready_queue[QUEUEMAX]={0};
    // int qc = 0;
    // struct PCB cp = {1,1,4,1,5,4,8};
    // struct PCB np = {2,2,3,0,0,3,6};
    // int ts = 2;
    // struct PCB rpcb = handle_process_arrival_pp(ready_queue, &qc, cp, np, ts);
    // printf("%d",rpcb.process_id);
    // printf("%d",qc);
    // struct PCB {
    //     int process_id;
    //     int arrival_timestamp;
    //     int total_bursttime;
    //     int execution_starttime;
    //     int execution_endtime;
    //     int remaining_bursttime;
    //     int process_priority;
    // };

    // return 0;
}