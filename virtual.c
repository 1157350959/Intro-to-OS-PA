#include <stdio.h>
#include "oslabs.h"
#include <stdbool.h>

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == true)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    else
    {
        if(*frame_cnt > 0)
        {
            struct PTE new_pte = {true, frame_pool[--(*frame_cnt)], current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte;
            //*table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            (*table_cnt)++;
            return frame_pool[*frame_cnt];
        }
        else
        {
            int min_arrival_time = -1;
            int min_arrival_time_idx = -1;
            for(int i = 0; i < *table_cnt; i++)
            {
                if(page_table[i].is_valid == true)
                {
                    min_arrival_time = (min_arrival_time == -1)? page_table[i].arrival_timestamp : min_arrival_time;
                    min_arrival_time_idx = (min_arrival_time_idx == -1)? i : min_arrival_time_idx;
                    min_arrival_time = (0 <= page_table[i].arrival_timestamp || page_table[i].arrival_timestamp < min_arrival_time)? page_table[i].arrival_timestamp : min_arrival_time;
                    min_arrival_time_idx = (0 <= page_table[i].arrival_timestamp || page_table[i].arrival_timestamp < min_arrival_time)? i : min_arrival_time_idx;
                }
            }
            struct PTE new_pte = {true, page_table[min_arrival_time_idx].frame_number, current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte; 
            struct PTE replaced_pte = {false, -1, -1, -1, -1};
            page_table[min_arrival_time_idx] = replaced_pte;
            return page_table[page_number].frame_number;
        }
    }
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int ference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{

}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
{

}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{

}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
{

}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{

}

// int main()
// {
//     struct PTE pt[TABLEMAX] = {{0,-1,-1,-1,-1},{0,-1,-1,-1,-1},{1,10,3,3,1},{0,-1,-1,-1,-1},{0,-1,-1,-1,-1},{1,20,2,4,2},{0,-1,-1,-1,-1},{1,30,1,1,1}};
//     int tc = 8;
//     int pn = 0;
//     int fp[POOLMAX];
//     int fc = 0;
//     int ct = 12;
//     int fn = process_page_access_fifo(pt,&tc,pn,fp,&fc,ct);
//     for(int i = 0; i < tc; i++)
//     {
//         printf("%d,%d,%d,%d,%d\n",pt[i].is_valid,pt[i].frame_number,pt[i].arrival_timestamp,pt[i].last_access_timestamp,pt[i].reference_count);
//     }
//     printf("table_cnt: %d\n", tc);
//     printf("frame_cnt: %d\n", fc);
//     printf("frame_num: %d\n", fn);
//     return 0;
// }
