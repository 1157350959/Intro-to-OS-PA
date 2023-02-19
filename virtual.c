#include <stdio.h>
#include <stdlib.h>
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
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++, time_stamp++)
    {
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
            //printf("1. page: %d, faults: %d\n",reference_string[i],total_page_faults);
        }
        else
        {
            if(frame_cnt > 0)
            {
                struct PTE new_pte = {true, frame_pool[--frame_cnt], time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte;
                total_page_faults++;
                //printf("2. page: %d, faults: %d\nTable: \n",reference_string[i],total_page_faults);
                for(int j = 0; j < table_cnt; j++)
                {
                    //printf("valid: %d frame: %d\n",page_table[j].is_valid,page_table[j].frame_number);
                }
            }
            else
            {
                int min_arrival_time = -1;
                int min_arrival_time_idx = -1;
                for(int j = 0; j < table_cnt; j++)
                {
                    if(page_table[j].is_valid == true)
                    {
                        if(min_arrival_time == -1)
                        {
                            min_arrival_time = page_table[j].arrival_timestamp;
                            min_arrival_time_idx = j;
                        }
                        if((0 <= page_table[j].arrival_timestamp) && (page_table[j].arrival_timestamp < min_arrival_time))
                        {
                            min_arrival_time = page_table[j].arrival_timestamp;
                            min_arrival_time_idx = j;
                        }
                    }
                }
                struct PTE new_pte = {true, page_table[min_arrival_time_idx].frame_number, time_stamp, time_stamp, 1};
                page_table[reference_string[i]] = new_pte; 
                struct PTE replaced_pte = {false, -1, -1, -1, -1};
                page_table[min_arrival_time_idx] = replaced_pte;
                total_page_faults++;
                //printf("3. page: %d, faults: %d\n table_cnt: %d\n Table: ",reference_string[i],total_page_faults, table_cnt);
                for(int j = 0; j < table_cnt; j++)
                {
                    //printf("valid: %d frame: %d arrival_time: %d\n",page_table[j].is_valid,page_table[j].frame_number,page_table[j].arrival_timestamp);
                }
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
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++, time_stamp++)
    {
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
                int min_last_access_time = -1;
                int min_last_access_time_idx = -1;
                for(int i = 0; i < table_cnt; i++)
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
    for(int i = 0, time_stamp = 1; i < reference_cnt; i++, time_stamp++)
    {
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
                int min_reference_cnt = -1;
                int min_reference_cnt_idx = -1;
                for(int i = 0; i < table_cnt; i++)
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


// int main()
// {
//     struct PTE pt[TABLEMAX] = {{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{true,10,3,3,1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{true,20,2,4,2},{false,-1,-1,-1,-1},{true,30,1,1,1}};
//     int tc = 8;
//     int pn = 0;
//     int fp[POOLMAX];
//     int fc = 0;
//     int ct = 12;
//     int fn = process_page_access_lru(pt,&tc,pn,fp,&fc,ct);
//     for(int i = 0; i < tc; i++)
//     {
//         printf("%d,%d,%d,%d,%d\n",pt[i].is_valid,pt[i].frame_number,pt[i].arrival_timestamp,pt[i].last_access_timestamp,pt[i].reference_count);
//     }
//     printf("table_cnt: %d\n", tc);
//     printf("frame_cnt: %d\n", fc);
//     printf("frame_num: %d\n", fn);
//     return 0;
// }

// count
// int main()
// {
//     struct PTE page_table[TABLEMAX] = {{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1},{false,-1,-1,-1,-1}};
//     int tabel_cnt = 8;
//     //int reference_string[REFERENCEMAX] = {0,3,2,6,3,4,5,2,4,5,6};
//     int reference_string[REFERENCEMAX] = {0,6,1,8,0,3,3,9,8,8,7,4,9,8,9,4,9,0,2,5};
//     int reference_cnt = 20;
//     int frame_pool[POOLMAX] = {0,1,2};
//     int frame_cnt = 3;
//     int faults = count_page_faults_fifo(page_table,tabel_cnt,reference_string,reference_cnt,frame_pool,frame_cnt);
//     printf("%d",faults);
//     return 0;
// }
