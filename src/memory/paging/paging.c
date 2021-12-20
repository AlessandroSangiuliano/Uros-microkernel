#include "paging.h"
#include "memory/heap/kheap.h"

static uint32_t *current_directory = 0;
void paging_load_directory(uint32_t *directory);

struct paging_4gb_chunck *paging_new_4gb(uint8_t flags)
{
    int offset = 0;
    uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t *entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

        for (int y = 0; y < PAGING_TOTAL_ENTRIES_PER_TABLE; y++)
        {
            entry[y] = (offset + (y * PAGING_PAGE_SIZE)) | flags;
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

uint32_t *paging_4gb_chunck_get_directory(struct paging_4gb_chunck *chunk)
{
    return chunk->directory_entry;
}