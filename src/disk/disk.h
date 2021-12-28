/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-25 17:49:43
 * @modify date 2021-12-25 17:49:43
 * @desc [description]
 */

#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

// this is a physical hard disk

#define DISK_TYPE_REAL 0


typedef unsigned int disk_type;

struct disk
{
    disk_type type;
    int sector_size;

    struct filesystem *filesystem;
};

struct disk* disk_get(int index);
void disk_search_and_init();
int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buf);

#endif