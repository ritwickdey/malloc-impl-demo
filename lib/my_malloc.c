#define _BSD_SOURCE
#include <unistd.h>
#include <errno.h>
#include "debug_log.h"

#define PAGE_SIZE sysconf(_SC_PAGESIZE)
#define MEM_ALLOC_LOT_SIZE (1 * PAGE_SIZE)
#define MEM_DEALLOC_LOT_SIZE (2 * PAGE_SIZE)
#define HEADER_SIZE (sizeof(m_block)) // 32 bytes
#define WRITABLE_AREA(p) (((m_block *)p) + 1)
#define HEADER_AREA(p) (((m_block *)p) - 1)

#define MAX(X, Y) ((X > Y) ? X : Y)

typedef enum
{
    FALSE,
    TRUE
} Boolean;

typedef struct m_block
{
    size_t size;
    Boolean free;
    struct m_block *prev;
    struct m_block *next;

} m_block;

m_block *g_mempool = NULL;


void *to_writable_mem(void *mem)
{
    return ((m_block *)mem) + 1;
}

void add_mem_to_pool(m_block *mem)
{
    if (g_mempool == NULL)
    {
        g_mempool = mem;
        g_mempool->next = NULL;
        g_mempool->prev = NULL;
        return;
    }

    m_block *parent = g_mempool;
    while (parent->next != NULL)
        parent = parent->next;

    parent->next = mem;
    mem->prev = parent;
    mem->next = NULL;
}

void *scan_mem_pool(size_t size)
{
    m_block *ptr = g_mempool;

    while (ptr != NULL)
    {

        if (ptr->free == TRUE && ptr->size >= size)
        {
            debug_log("Free %d bytes mem block found from mem pool.\n", ptr->size);
            return ptr;
        }

        ptr = ptr->next;
    }

    debug_log("No memory found in mem pool with block size %ld.\n", size);
    return NULL;
}

void resize_mem(m_block *mem, size_t size)
{

    if (mem->size - size <= HEADER_SIZE)
        return;
    size_t originalSize = mem->size;
    m_block *newMem = (m_block *)((char *)mem + size); // because, char size is 1 bytes

    newMem->size = mem->size - size;
    newMem->free = TRUE;
    newMem->next = mem->next;
    newMem->prev = mem;

    mem->size = size;
    mem->next = newMem;

    debug_log("Resized %ld bytes mem block into %ld and %ld\n", originalSize, mem->size, newMem->size);
}

void *allocate_mem(size_t size)
{
    debug_log("before allocate, program break = %10p\n", sbrk(0));
    void *mem = sbrk(size);

    if (mem == (void *)-1)
    {
        return NULL; // don't need to change errno
    }

    debug_log("Allocated %ld bytes memory using sbrk.\n", size);
    debug_log("after allocate, program break = %10p\n", sbrk(0));

    m_block *mem_block = mem;

    mem_block->size = size;
    mem_block->free = TRUE;
    mem_block->next = NULL;
    mem_block->prev = NULL;

    return mem_block;
}

void merge_free_blocks()
{
    m_block *ptr = g_mempool;

    if (ptr == NULL)
    {
        return;
    }

    while (ptr && ptr->next)
    {

        if (ptr->free == TRUE && ptr->next->free == TRUE)
        {
            ptr->size += ptr->next->size;
            ptr->next = ptr->next->next;
            if (ptr->next != NULL)
                ptr->next->prev = ptr;
        }

        ptr = ptr->next;
    }
}

void release_memory_from_last()
{

    m_block *ptr = g_mempool;
    m_block *free_area_start = NULL;
    size_t totalFreeSpace = 0;

    while (ptr)
    {
        if (ptr->free)
        {
            free_area_start = ptr;
            totalFreeSpace += ptr->size;
        }
        else
        {
            free_area_start = NULL;
            totalFreeSpace = 0;
        }
        ptr = ptr->next;
    }

    if (free_area_start == NULL || totalFreeSpace < MEM_DEALLOC_LOT_SIZE)
    {
        return;
    }

    void *reset = NULL;

    if (free_area_start == g_mempool)
    {
        reset = g_mempool;
        g_mempool = NULL;
    }
    else
    {
        reset = free_area_start;
        free_area_start->prev->next = NULL;
    }
    debug_log("before release, program break = %10p\n", sbrk(0));
    brk(reset);
    debug_log("Released %ld bytes mem from process\n", totalFreeSpace);
    debug_log("after release, program break = %10p\n", sbrk(0));
}

void *my_malloc(size_t size)
{
    m_block *mem = NULL;
    size_t msize = size + HEADER_SIZE;

    mem = scan_mem_pool(msize);

    if (size == 0)
        return mem;

    if (mem == NULL)
    {
        size_t allocate_size = MAX(msize, MEM_ALLOC_LOT_SIZE);

        if ((mem = allocate_mem(allocate_size)) == NULL)
        {
            errno = ENOMEM;
            return NULL;
        }

        add_mem_to_pool(mem);
    }

    resize_mem(mem, msize);
    mem->free = FALSE;
    return WRITABLE_AREA(mem);
}

void my_free(void *mem)
{
    if (mem == NULL)
        return;

    m_block *block = HEADER_AREA(mem);
    block->free = TRUE;
    debug_log("Freed %d bytes mem blocks.\n", block->size);

    merge_free_blocks();
    release_memory_from_last();
}