#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"
#include <stdbool.h>
#include <limits.h>

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
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            return frame_pool[*frame_cnt];
        }
        else
        {
            int min_arrival_time = -1;
            int min_arrival_time_idx = -1;
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            for(int i = 0; i < *table_cnt; i++)
            {
                if(page_table[i].is_valid == true)
                {
                    if(min_arrival_time == -1)
                    {
                        min_arrival_time = page_table[i].arrival_timestamp;
                        min_arrival_time_idx = i;
                    }
                    if((0 <= page_table[i].arrival_timestamp) && (page_table[i].arrival_timestamp < min_arrival_time))
                    {
                        min_arrival_time = page_table[i].arrival_timestamp;
                        min_arrival_time_idx = i;
                    }
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

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int total_page_faults = 0;
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++)
    {
        time_stamp++;
        if(reference_string[i] >= table_cnt)
        {
            struct PTE new_pte = {false, -1, -1, -1, -1};
            int num_expansion = reference_string[i] - table_cnt + 1;
            for(int j = 0; j < num_expansion; j++)
            {
                page_table[table_cnt] = new_pte;
                table_cnt++;
            }
        }
        if(page_table[reference_string[i]].is_valid == true)
        {
            page_table[reference_string[i]].last_access_timestamp = time_stamp;
            page_table[reference_string[i]].reference_count++;
        }
        else
        {
            if(frame_cnt > 0)
            {
                struct PTE new_pte = {true, frame_pool[--frame_cnt], time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte;
                total_page_faults++;
            }
            else
            {
                int min_arrival_time = INT_MAX;
                int min_arrival_time_idx = TABLEMAX;
                for(int j = 0; j < table_cnt; j++)
                {
                    if((page_table[j].is_valid == true) && (0 < page_table[j].arrival_timestamp) && (page_table[j].arrival_timestamp < min_arrival_time))
                    {
                        min_arrival_time = page_table[j].arrival_timestamp;
                        min_arrival_time_idx = j;
                    }
                }
                struct PTE new_pte = {true, page_table[min_arrival_time_idx].frame_number, time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte; 
                struct PTE replaced_pte = {false, -1, -1, -1, -1};
                page_table[min_arrival_time_idx] = replaced_pte;
                total_page_faults++;
            }
        }
    }
    return total_page_faults;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
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
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            return frame_pool[*frame_cnt];
        }
        else
        {
            int min_last_access_time = -1;
            int min_last_access_time_idx = -1;
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            for(int i = 0; i < *table_cnt; i++)
            {
                if(page_table[i].is_valid == true)
                {
                    if(min_last_access_time == -1)
                    {
                        min_last_access_time = page_table[i].last_access_timestamp;
                        min_last_access_time_idx = i;
                    }
                    if((0 <= page_table[i].last_access_timestamp) && (page_table[i].last_access_timestamp < min_last_access_time))
                    {
                        min_last_access_time = page_table[i].last_access_timestamp;
                        min_last_access_time_idx = i;
                    }
                }
            }
            struct PTE new_pte = {true, page_table[min_last_access_time_idx].frame_number, current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte; 
            struct PTE replaced_pte = {false, -1, -1, -1, -1};
            page_table[min_last_access_time_idx] = replaced_pte;
            return page_table[page_number].frame_number;
        }
    }
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int total_page_faults = 0;
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++)
    {
        time_stamp++;
        if(reference_string[i] >= table_cnt)
        {
            struct PTE new_pte = {false, -1, -1, -1, -1};
            int num_expansion = reference_string[i] - table_cnt + 1;
            for(int j = 0; j < num_expansion; j++)
            {
                page_table[table_cnt] = new_pte;
                table_cnt++;
            }
        }
        if (page_table[reference_string[i]].is_valid == true)
        {
            page_table[reference_string[i]].last_access_timestamp = time_stamp;
            page_table[reference_string[i]].reference_count++;
        }
        else
        {
            if(frame_cnt > 0)
            {
                struct PTE new_pte = {true, frame_pool[--(frame_cnt)], time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte;
                total_page_faults++;
            }
            else
            {
                int min_last_access_time = INT_MAX;
                int min_last_access_time_idx = TABLEMAX;
                for(int i = 0; i < table_cnt; i++)
                {
                    if((page_table[i].is_valid == true) && (0 <= page_table[i].last_access_timestamp) && (page_table[i].last_access_timestamp < min_last_access_time))
                    {
                        min_last_access_time = page_table[i].last_access_timestamp;
                        min_last_access_time_idx = i;
                    }
                }
                struct PTE new_pte = {true, page_table[min_last_access_time_idx].frame_number, time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte; 
                struct PTE replaced_pte = {false, -1, -1, -1, -1};
                page_table[min_last_access_time_idx] = replaced_pte;
                total_page_faults++;
            }
        }
    }
    return total_page_faults;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int * table_cnt, int page_number, int frame_pool[POOLMAX], int * frame_cnt, int current_timestamp)
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
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            return frame_pool[*frame_cnt];
        }
        else
        {
            int min_reference_cnt = -1;
            int min_reference_cnt_idx = -1;
            *table_cnt = (page_number >= *table_cnt)? *table_cnt + 1 : *table_cnt;
            for(int i = 0; i < *table_cnt; i++)
            {
                if(page_table[i].is_valid == true)
                {
                    if(min_reference_cnt == -1)
                    {
                        min_reference_cnt = page_table[i].reference_count;
                        min_reference_cnt_idx = i;
                    }
                    if((0 <= page_table[i].reference_count) && (page_table[i].reference_count <= min_reference_cnt))
                    {
                        if((page_table[i].reference_count == min_reference_cnt) && (page_table[i].arrival_timestamp >= page_table[min_reference_cnt_idx].arrival_timestamp))
                        {
                            continue;
                        }
                        min_reference_cnt = page_table[i].reference_count;
                        min_reference_cnt_idx = i;
                    }
                }
            }
            struct PTE new_pte = {true, page_table[min_reference_cnt_idx].frame_number, current_timestamp, current_timestamp, 1};
            page_table[page_number] = new_pte; 
            struct PTE replaced_pte = {false, -1, -1, -1, -1};
            page_table[min_reference_cnt_idx] = replaced_pte;
            return page_table[page_number].frame_number;
        }
    }
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int total_page_faults = 0;
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++)
    {
        time_stamp++;
        if(reference_string[i] >= table_cnt)
        {
            struct PTE new_pte = {false, -1, -1, -1, -1};
            int num_expansion = reference_string[i] - table_cnt + 1;
            for(int j = 0; j < num_expansion; j++)
            {
                page_table[table_cnt] = new_pte;
                table_cnt++;
            }
        }
        if (page_table[reference_string[i]].is_valid == true)
        {
            page_table[reference_string[i]].last_access_timestamp = time_stamp;
            page_table[reference_string[i]].reference_count++;
        }
        else
        {
            if(frame_cnt > 0)
            {
                struct PTE new_pte = {true, frame_pool[--frame_cnt], time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte;
                total_page_faults++;
            }
            else
            {
                int min_reference_cnt = INT_MAX;
                int min_reference_cnt_idx = TABLEMAX;
                for(int i = 0; i < table_cnt; i++)
                {
                    if(page_table[i].is_valid == true)
                    {
                        if((0 <= page_table[i].reference_count) && (page_table[i].reference_count <= min_reference_cnt))
                        {
                            if((page_table[i].reference_count == min_reference_cnt) && (page_table[i].arrival_timestamp >= page_table[min_reference_cnt_idx].arrival_timestamp))
                            {
                                continue;
                            }
                            min_reference_cnt = page_table[i].reference_count;
                            min_reference_cnt_idx = i;
                        }
                    }
                }
                struct PTE new_pte = {true, page_table[min_reference_cnt_idx].frame_number, time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte; 
                struct PTE replaced_pte = {false, -1, -1, -1, -1};
                page_table[min_reference_cnt_idx] = replaced_pte;
                total_page_faults++;
            }
        }
    }
    return total_page_faults;
}
