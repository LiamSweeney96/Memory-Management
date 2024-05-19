
void* _malloc(int size);

typedef struct Block {
    struct Block *next;   // Points to next free block
    int size;             // Size of the current block
    int free;             // Determines whether memory has been freed (1 = no, 0 = yes)
} Block;

void _free(void *ptr);