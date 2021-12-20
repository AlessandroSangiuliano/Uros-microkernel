#include "heap.h"
#include "kernel.h"
#include <stdbool.h>
#include "status.h"
#include "memory/memory.h"

static int heap_validate_table(void *addr_ptr, void *end, struct heap_table *table)
{
    int res = 0;

    size_t table_size = (size_t)(end - addr_ptr);
    size_t total_blocks = table_size / HEAP_BLOCK_SIZE;

    if (table->total_entries != total_blocks)
    {
        res = -EINVARG;
        return res;
    }

    return res;
}

static int heap_validate_alignement(void *ptr)
{
    return ((unsigned int)ptr % HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap *heap, void *addr_ptr, void *end, struct heap_table *table)
{
    int res = 0;

    if (!heap_validate_alignement(addr_ptr) || !heap_validate_alignement(end))
    {
        res = -EINVARG;
        return res;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->start_address = addr_ptr;
    heap->table = table;

    res = heap_validate_table(addr_ptr, end, table);

    if (res < 0)
        return res;

    size_t table_size = sizeof(heap_table_entry) * table->total_entries;
    memset(table->entries, HEAP_BLOCK_FREE, table_size);

    return res;
}

static uint32_t heap_aling_value_to_upper(uint32_t value)
{
    if ((value % HEAP_BLOCK_SIZE) == 0)
    {
        return value;
    }

    value = (value - (value % HEAP_BLOCK_SIZE));
    value += HEAP_BLOCK_SIZE;

    return value;
}

static int heap_get_entry_type(heap_table_entry entry)
{
    return entry & 0x0f;
}

void *heap_block_to_address(struct heap *heap, int block)
{
    return heap->start_address + (block * HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(struct heap *heap, int start_block, int total_blocks)
{
    int end_block = (start_block + total_blocks) - 1;

    heap_table_entry entry = HEAP_BLOCK_TAKEN | HEAP_BLOCK_IS_FIRST;

    if (total_blocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (int i = start_block; i <= end_block; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TAKEN;

        if (i != end_block - 1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

int heap_get_start_block(struct heap *heap, int total_blocks)
{
    struct heap_table *table = heap->table;
    int current_block = 0;
    int block_start = -1;

    for (size_t i = 0; i < table->total_entries; i++)
    {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_FREE)
        {
            current_block = 0;
            block_start = -1;
            continue;
        }

        // first block?

        if (block_start == -1)
            block_start = i;
        

        current_block++;

        if (current_block == total_blocks)
            break;
        
    }

    if (block_start == -1)
        return -ENOMEM;
    
    
    return block_start;
}

void *heap_malloc_blocks(struct heap *heap, uint32_t total_blocks)
{
    void *address = 0;

    int start_block = heap_get_start_block(heap, total_blocks);

    if (start_block < 0)
    {
        return address;
    }

    address = heap_block_to_address(heap, start_block);

    // mark the block as taken

    heap_mark_blocks_taken(heap, start_block, total_blocks);
    return address;
}

void heap_mark_blocks_free(struct heap *heap, int starting_block)
{
    struct heap_table *table = heap->table;

    for (int i = starting_block; i < (int)table->total_entries; i++)
    {
        heap_table_entry entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_FREE;

        if (!(entry & HEAP_BLOCK_HAS_NEXT))
            break;
        
    }
    
    
}

int heap_address_to_block(struct heap *heap, void *address)
{
    return ((int) (address - heap->start_address)) / HEAP_BLOCK_SIZE;
}

void *heap_malloc(struct heap *heap, size_t size)
{
    size_t aligned_size = heap_aling_value_to_upper(size);
    uint32_t total_blocks = aligned_size / HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap *heap, void *ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}