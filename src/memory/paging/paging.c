#include "paging.h"
#include "memory/heap/kheap.h"

static uint32_t *current_directory = 0;
void paging_load_directory(uint32_t *directory);

struct paging_4gb_chunck *paging_new_4gb(uint8_t flags)
{
    int offset = 0;
    // directory is an address that points to a 4Kb memory area where entries must be set
    uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        // an entry (directory entry) is an address that points ta a 4kb memory area. these are the page tables
        uint32_t *entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

        for (int y = 0; y < PAGING_TOTAL_ENTRIES_PER_TABLE; y++)
        {
            entry[y] = (offset + (y * PAGING_PAGE_SIZE)) | flags; //phys mem address (page frames)
        }
        
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t) entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunck *chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunck));
    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}

void paging_switch(uint32_t *directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}

bool paging_is_aligned(void *address)
{
    return ((uint32_t) address % PAGING_PAGE_SIZE) == 0;
}

uint32_t *paging_4gb_chunck_get_directory(struct paging_4gb_chunck *chunk)
{
    return chunk->directory_entry;
}

int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out)
{
    int res = 0;

    if (!paging_is_aligned(virtual_address))
    {
        res = -EINVARG;
        return res;
    }

    *directory_index_out = ((uint32_t) virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *table_index_out = ((uint32_t) virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

    return res;
    
}

int paging_set(uint32_t *directory, void *virtual_address, uint32_t val)
{
    int res = 0;

    if (!paging_is_aligned(virtual_address))
    {
        res = -EINVARG;
        return res;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    res = paging_get_indexes(virtual_address, &directory_index, &table_index);

    if (res < 0)
    {
        return res;
    }

    // entry contains the pointer to the page table
    uint32_t entry = directory[directory_index];

    uint32_t *table = (uint32_t*) (entry & 0xfffff000); //bitwise and to get only the address of the entry

    table[table_index] = val; 

    // return zero for now, just a stub
    return 0;
    
}