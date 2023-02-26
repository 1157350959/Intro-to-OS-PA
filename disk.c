#include "oslabs.h"
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

struct RCB NULLRCB = {0, 0, 0, 0, 0};

bool is_NULLRCB(struct RCB rcb)
{
    if(rcb.address == NULLRCB.address && 
        rcb.arrival_timestamp == NULLRCB.arrival_timestamp &&
        rcb.cylinder == NULLRCB.cylinder &&
        rcb.process_id == NULLRCB.process_id &&
        rcb.request_id == NULLRCB.request_id)
    {
        return true;
    }
    return false;
}

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int * queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if(is_NULLRCB(current_request))
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int * queue_cnt)
{
    if(*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int earliest_arrival_time = INT_MAX;
        int earliest_arrival_time_idx = QUEUEMAX;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if(request_queue[i].arrival_timestamp < earliest_arrival_time)
            {
                earliest_arrival_time = request_queue[i].arrival_timestamp;
                earliest_arrival_time_idx = i;
            }
        }
        struct RCB return_rcb = request_queue[earliest_arrival_time_idx];
        for(int i = earliest_arrival_time_idx; i < *queue_cnt - 1; i++)
        {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return return_rcb;
    }
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int * queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if(is_NULLRCB(current_request))
    {
        return new_request;
    }
    else
    {
       request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request; 
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int * queue_cnt, int current_cylinder)
{
    if(*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int closest_cylinder = INT_MIN;
        int closest_cylinder_idx = QUEUEMAX;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if((abs(request_queue[i].cylinder - current_cylinder) < abs(closest_cylinder - current_cylinder))
                ||
                ((abs(request_queue[i].cylinder - current_cylinder) == abs(closest_cylinder - current_cylinder)) && (request_queue[i].arrival_timestamp < request_queue[closest_cylinder_idx].arrival_timestamp)))
            {
                closest_cylinder = request_queue[i].cylinder;
                closest_cylinder_idx = i;
            }
        }
        struct RCB return_rcb = request_queue[closest_cylinder_idx];
        for(int i = closest_cylinder_idx; i < *queue_cnt - 1; i++)
        {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return return_rcb;
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int * queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if(is_NULLRCB(current_request))
    {
        return new_request;
    }
    else
    {
       request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request; 
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int * queue_cnt, int current_cylinder, int scan_direction)
{
    if(*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int earliest_arrival_time = INT_MAX;
        int idx = QUEUEMAX;
        int closest_cylinder = INT_MIN;
        bool equal_flag = false;
        bool large_cylinder_flag = false;
        bool small_cylinder_flag = false;
        for(int i = 0; i < *queue_cnt; i++)
        {
            if((request_queue[i].cylinder == current_cylinder) && (request_queue[i].arrival_timestamp < earliest_arrival_time))
            {
                equal_flag = true;
                earliest_arrival_time = request_queue[i].arrival_timestamp;
                idx = i;
            }
            else if((scan_direction == 1) && (!equal_flag))
            {
                if((request_queue[i].cylinder > current_cylinder) && (abs(request_queue[i].cylinder - current_cylinder) < abs(closest_cylinder - current_cylinder)))
                {
                    large_cylinder_flag = true;
                    closest_cylinder = request_queue[i].cylinder;
                    idx = i;
                }
                else if((abs(request_queue[i].cylinder - current_cylinder) < abs(closest_cylinder - current_cylinder)) && !large_cylinder_flag)
                {
                    closest_cylinder = request_queue[i].cylinder;
                    idx = i;
                }
            }
            else if((scan_direction == 0) && (!equal_flag))
            {
                if((request_queue[i].cylinder < current_cylinder) && (abs(request_queue[i].cylinder - current_cylinder) < abs(closest_cylinder - current_cylinder)))
                {
                    small_cylinder_flag = true;
                    closest_cylinder = request_queue[i].cylinder;
                    idx = i;
                }
                else if((abs(request_queue[i].cylinder - current_cylinder) < abs(closest_cylinder - current_cylinder)) && !small_cylinder_flag)
                {
                    closest_cylinder = request_queue[i].cylinder;
                    idx = i;
                }
            }
        }
        struct RCB return_rcb = request_queue[idx];
        for(int i = idx; i < *queue_cnt - 1; i++)
        {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return return_rcb;
    }
}
void print_Q(struct RCB rq[QUEUEMAX], int q_cnt)
{
    for(int i = 0; i < q_cnt; i++)
    {
        printf("%d %d %d %d %d\n",rq[i].request_id,rq[i].arrival_timestamp,rq[i].cylinder,rq[i].address,rq[i].process_id);
    }
    printf("q_cnt: %d\n",q_cnt);
}

void print_RCB(struct RCB rcb)
{
    printf("rcb: %d %d %d %d %d\n",rcb.request_id,rcb.arrival_timestamp,rcb.cylinder,rcb.address,rcb.process_id);
}

// int main()
// {
//     //arrival
//     struct RCB request_q[QUEUEMAX];
//     int q_cnt = 0;
//     struct RCB current = {51,1,53,53,51};
//     struct RCB new = {52,2,54,54,52};
//     int time = 2;
//     struct RCB rcb = handle_request_arrival_fcfs(request_q,&q_cnt,current,new,time);
//     print_Q(request_q, q_cnt);
//     print_RCB(rcb);
// }

// int main()
// {
//     //completion
//     struct RCB request_q[QUEUEMAX] = {{1,52,58,58,1},{2,51,58,58,2},{3,53,58,58,3}};
//     int q_cnt = 3;
//     int cylinder = 58;
//     int scan = 1;
//     struct RCB rcb = handle_request_completion_look(request_q, &q_cnt,cylinder,scan);
//     print_Q(request_q,q_cnt);
//     print_RCB(rcb);
// }