
#include <unistd.h>

#include "memory_management.h"

#define struct_size sizeof(struct Block)

static Block list = {0,0};

void* _malloc(int size) 
{
    // Check if a valid size has been given
    if (size <= 0)
    {
        return NULL;
    }

    // Calculate total memory needed for allocation
    size = size+struct_size;

    // Declare block pointer for current block, and the next block in the list
    Block *current, **next = &(list.next);

    // Iterate through linked list
    for (current = list.next; current != NULL; current = current->next)
    {
        // If big enough block is found
        if (current->size >= size)
        {
            // Set next pointer to equal value at next node of linked list
            *next = current->next;
            // If current block is greater than the size required
            if (current->size != size)
            {
                // Use only the end of the memory block to avoid memory fragmentation
                current->size -= size; 
                current += current->size;
                current->size = size;
            }
            // Set allocated memory status to not freed
            current->free = 1;

            // Return pointer to memory block immediately after current
            return ((void *)current+1); 
        }
        // Store address of next node of linked list
        next = &(current->next);
    }

    // Request memory space from the OS if no large enough block found
    current = sbrk(size);

    // Check if sbrk was successful
    if (current == (void *)-1)
    {
        return NULL;
    }

    // Assign current block size to equal size required
    current->size = size;

    // Set allocated memory status to not freed
    current->free = 1;

    // Return pointer to memory block immediately after current
    return ((void *)current+1);
}

void _free(void *ptr)
{
    // If pointer is not NULL or already free
    if (ptr != NULL)
    {
        // Point to block rather than the start of the allocated memory
        Block *free_mem = (Block *)ptr-1; 
        
        if (free_mem->free == 1)
        {
            // Add free memory to free list
            free_mem->next = list.next;
            list.next = free_mem;

            // Clear up the memory by setting the pointer to NULL
            ptr = NULL;

            // Set status of memory block to be free
            free_mem->free = 0;
        }
    }

}