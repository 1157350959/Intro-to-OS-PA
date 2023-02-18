#include <stdio.h>
#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
    else
    {
        if(*frame_cnt > 0)
        {
            *frame_cnt -= 1;
            struct PTE new_pte = {1, frame_pool[*frame_cnt], current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte;
            *table_cnt += 1;
            return frame_pool[*frame_cnt];
        }
        else
        {
            int min_arrival_time = -1;
            int min_arrival_time_idx = -1;
            for(int i = 0; i < *table_cnt; i++)
            {
                if(page_table[i].is_valid == 1)
                {
                    min_arrival_time = (min_arrival_time == -1)? page_table[i].arrival_timestamp : min_arrival_time;
                    min_arrival_time_idx = (min_arrival_time_idx == -1)? i : min_arrival_time_idx;
                    min_arrival_time = (0 <= page_table[i].arrival_timestamp || page_table[i].arrival_timestamp < min_arrival_time)? page_table[i].arrival_timestamp : min_arrival_time;
                    min_arrival_time_idx = (0 <= page_table[i].arrival_timestamp || page_table[i].arrival_timestamp < min_arrival_time)? i : min_arrival_time_idx;
                }
            }
            struct PTE new_pte = {1, page_table[min_arrival_time_idx].frame_number, current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte; 
            struct PTE replaced_pte = {0, -1, -1, -1, -1};
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
