/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-28 11:10:42
 * @modify date 2021-12-28 11:10:42
 * @desc [description]
 */

#include "fat16.h"
#include "disk/disk.h"
#include "status.h"
#include "string/string.h"

int fat16_resolve(struct disk *disk);
void *fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode);

struct filesystem fat16_fs =
{
    .resolve = fat16_resolve,
    .open = fat16_open
};

int fat16_resolve(struct disk *disk)
{
    return -EIO;
}

void *fat16_open(struct disk* disk, struct path_part* path, FILE_MODE mode)
{
    return 0;
}

struct filesystem *fat16_init()
{
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}


