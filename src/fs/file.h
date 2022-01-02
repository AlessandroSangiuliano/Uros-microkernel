/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-28 10:51:27
 * @modify date 2021-12-28 10:51:27
 * @desc [description]
 */

#ifndef FILE_H
#define FILE_H

#include "pparser.h"
#include <stdint.h>

struct disk;

typedef unsigned int FILE_SEEK_MODE;

enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};


typedef unsigned int FILE_MODE;
enum
{ 
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

typedef int (*fs_resolve_function)(struct disk* disk);
typedef void*(*fs_open_function)(struct disk* disk, struct path_part* path, FILE_MODE mode);

struct filesystem
{
    // Filesystem should return zero from resolve if the provided disk is using its filesystem
    fs_resolve_function resolve;
    fs_open_function open;
    char name[20];
};

struct file_descriptor
{
    // The descriptor index
    int index;
    struct filesystem* filesystem;

    // Private data for internal file descriptor
    void* private;

    // The disk that the file descriptor should be used on
    struct disk* disk;
};

void fs_init();
int fopen(const char* filename, const char* mode_str);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);

#endif