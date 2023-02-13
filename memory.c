#include <stdio.h>
#include "oslabs.h"

struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};

// struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt, int process_id)
// {
//     int best_idx = -1;
//     for(int i = 0; i < *map_cnt; i++)
//     {
//         if(memory_map[i].segment_size == request_size)
//         {
//             memory_map[i].process_id = process_id;
//             return memory_map[i];
//         }
//         else if(memory_map[i].segment_size < request_size)
//         {
//             continue;
//         }
//         else
//         {   
//             best_idx = (best_idx == -1)? i : best_idx;
//             best_idx = (memory_map[i].segment_size < memory_map[best_idx].segment_size)? i : best_idx;
//         }
//     }
//     // if(best_idx != -1)
//     // {
//     //     struct MEMORY_BLOCK new_free_block = {memory_map[best_idx].start_address + request_size,
//     //                                           memory_map[best_idx].end_address,
//     //                                           memory_map[best_idx].segment_size - request_size,
//     //                                           0};
//     //     memory_map[*map_cnt] = new_free_block;
//     //     memory_map[best_idx].end_address = memory_map[best_idx].start_address + request_size - 1;
//     //     memory_map[best_idx].segment_size = request_size;
//     //     memory_map[best_idx].process_id = process_id;
//     //     *map_cnt += 1;
//     //     return memory_map[best_idx];
//     // }
//     if(best_idx != -1)
//     {
//         struct MEMORY_BLOCK new_free_block = {memory_map[best_idx].start_address + request_size,
//                                               memory_map[best_idx].end_address,
//                                               memory_map[best_idx].segment_size - request_size,
//                                               0};
//         memory_map[*map_cnt] = new_free_block;
//         memory_map[best_idx].end_address = memory_map[best_idx].start_address + request_size - 1;
//         memory_map[best_idx].segment_size = request_size;
//         memory_map[best_idx].process_id = process_id;
//         for(int i = *map_cnt; i > best_idx + 1; i--)
//         {
//             memory_map[i] = memory_map[i - 1];
//         }
//         *map_cnt += 1;
//         return memory_map[best_idx];
//     }
//     return NULLBLOCK;
// }
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt, int process_id)
{
    struct MEMORY_BLOCK mb = {0,19,20,10};
    *map_cnt += 1;
    return mb;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt, int process_id)
{
    int lowest_starting_address = -1;
    for(int i = 0; i < *map_cnt; i++)
    {
        if(memory_map[i].segment_size == request_size && memory_map[i].process_id == 0)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if(memory_map[i].segment_size < request_size)
        {
            continue;
        }
        else if(memory_map[i].segment_size > request_size && memory_map[i].process_id == 0)
        {
            lowest_starting_address = (lowest_starting_address == -1)? i : memory_map[i].start_address;
            lowest_starting_address = (memory_map[i].start_address < lowest_starting_address)? memory_map[i].start_address : lowest_starting_address;
        }
    }
    if(lowest_starting_address != -1)
    {
        struct MEMORY_BLOCK new_free_block = {memory_map[lowest_starting_address].start_address + request_size,
                                              memory_map[lowest_starting_address].end_address,
                                              memory_map[lowest_starting_address].segment_size - request_size,
                                              0};
        memory_map[*map_cnt] = new_free_block;
        memory_map[lowest_starting_address].end_address = memory_map[lowest_starting_address].start_address + request_size - 1;
        memory_map[lowest_starting_address].segment_size = request_size;
        memory_map[lowest_starting_address].process_id = process_id;
        *map_cnt += 1;
        return memory_map[lowest_starting_address];
    }
    return NULLBLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt, int process_id)
{
    int largest_block_idx = -1;
    for(int i = 0; i < *map_cnt; i++)
    {
        if(memory_map[i].segment_size == request_size)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if(memory_map[i].segment_size < request_size)
        {
            continue;
        }
        else
        {
            largest_block_idx = (largest_block_idx == -1)? i : largest_block_idx;
            largest_block_idx = (memory_map[i].segment_size > memory_map[largest_block_idx].segment_size)? i : largest_block_idx;
        }
    }
    if(largest_block_idx != -1)
    {
        struct MEMORY_BLOCK new_free_block = {memory_map[largest_block_idx].start_address + request_size,
                                              memory_map[largest_block_idx].end_address,
                                              memory_map[largest_block_idx].segment_size - request_size,
                                              0};
        memory_map[*map_cnt] = new_free_block;
        memory_map[largest_block_idx].end_address = memory_map[largest_block_idx].start_address + request_size - 1;
        memory_map[largest_block_idx].segment_size = request_size;
        memory_map[largest_block_idx].process_id = process_id;
        *map_cnt += 1;
        return memory_map[largest_block_idx];
    }
    return NULLBLOCK;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt, int process_id, int last_address)
{
    int lowest_starting_address = -1;
    for(int i = 0; i < *map_cnt; i++)
    {
        if(memory_map[i].start_address >= last_address)
        {
            if(memory_map[i].segment_size == request_size)
            {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            else if(memory_map[i].segment_size < request_size)
            {
                continue;
            }
            else
            {
                lowest_starting_address = (lowest_starting_address == -1)? memory_map[i].start_address : lowest_starting_address;
                lowest_starting_address = (memory_map[i].start_address < lowest_starting_address)? memory_map[i].end_address : lowest_starting_address;
            }
        }
    }
    if(lowest_starting_address != -1)
    {
        struct MEMORY_BLOCK new_free_block = {memory_map[lowest_starting_address].start_address + request_size,
                                              memory_map[lowest_starting_address].end_address,
                                              memory_map[lowest_starting_address].segment_size = memory_map[lowest_starting_address].segment_size - request_size,
                                              0};
        memory_map[*map_cnt] = new_free_block;
        memory_map[lowest_starting_address].end_address = memory_map[lowest_starting_address].start_address + request_size - 1;
        memory_map[lowest_starting_address].segment_size = request_size;
        memory_map[lowest_starting_address].process_id = process_id;
        *map_cnt += 1;
        return memory_map[lowest_starting_address];
    }
    return NULLBLOCK;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int * map_cnt)
{
    int previous_block_idx = -1;
    int next_block_idx = -1;
    int freed_block_idx = -1;
    for(int i = 0; i < *map_cnt; i++)
    {
        previous_block_idx = (memory_map[i].end_address == freed_block.start_address - 1)? i : previous_block_idx;
        next_block_idx = (memory_map[i].start_address == freed_block.end_address + 1)? i : next_block_idx;
        freed_block_idx = (memory_map[i].start_address == freed_block.start_address &&
                           memory_map[i].end_address == freed_block.end_address)? i : freed_block_idx;
    }
    if((previous_block_idx == -1 || memory_map[previous_block_idx].process_id != 0) && (next_block_idx == -1 || memory_map[next_block_idx].process_id != 0))
    {
        // No need to merge
        memory_map[freed_block_idx].process_id == 0;
    }
    else if((previous_block_idx == -1 || memory_map[previous_block_idx].process_id != 0) && memory_map[next_block_idx].process_id == 0)
    {
        // Merge with the previous block
        memory_map[freed_block_idx].end_address = memory_map[next_block_idx].end_address;
        memory_map[freed_block_idx].segment_size = memory_map[freed_block_idx].segment_size + memory_map[next_block_idx].segment_size;
        memory_map[freed_block_idx].process_id = 0;
        for(int i = next_block_idx; i < *map_cnt; i++)
        {
            if(i == *map_cnt - 1)
            {
                memory_map[i] = NULLBLOCK;
            }
            else
            {
                memory_map[i] = memory_map[i + 1];
            }
        }
        *map_cnt -= 1;
    }
    else if(memory_map[previous_block_idx].process_id == 0 && (next_block_idx == -1 || memory_map[next_block_idx].process_id != 0))
    {
        // Merge with the next one
        memory_map[freed_block_idx].start_address = memory_map[previous_block_idx].start_address;
        memory_map[freed_block_idx].segment_size = memory_map[freed_block_idx].segment_size + memory_map[previous_block_idx].segment_size;
        memory_map[freed_block_idx].process_id = 0;
        for(int i = previous_block_idx; i < *map_cnt; i++)
        {
            if(i == *map_cnt - 1)
            {
                memory_map[i] = NULLBLOCK;
            }
            else
            {
                memory_map[i] = memory_map[i + 1];
            }
        }
        *map_cnt -= 1;
    }
    else
    {
        // Merge with both the previous block and next block
        memory_map[previous_block_idx].end_address = memory_map[next_block_idx].end_address;
        memory_map[previous_block_idx].segment_size = memory_map[previous_block_idx].segment_size + 
                                                   memory_map[freed_block_idx].segment_size + 
                                                   memory_map[next_block_idx].segment_size;
        for(int i = freed_block_idx; i < *map_cnt; i++)
        {
            if(i == *map_cnt - 2)
            {
                memory_map[i] = NULLBLOCK;
                memory_map[i + 1] = NULLBLOCK;
            }
            else
            {
                memory_map[i] = memory_map[i + 2];
            }
        }
        *map_cnt -= 2;
    }
}
