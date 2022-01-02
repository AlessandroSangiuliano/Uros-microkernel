/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-28 10:51:16
 * @modify date 2021-12-28 10:51:16
 * @desc [description]
 */

#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "kernel.h"
#include "fat/fat16.h"
#include "disk/disk.h"
#include "string/string.h"

struct filesystem *filesystems[UROS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[UROS_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem()
{
    int i = 0;
    for (i = 0; i < UROS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }

    return 0;
}

void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    fs = fs_get_free_filesystem();

    if (filesystem == 0)
    {
        kprintf("Must panic! fs_insert_filesystem\n"); // panic here instead print and infinite loop
        while(1) {}
    }
    

    if (!fs)
    {

        kprintf("Problem inserting filesystem\n"); // panic here instead print and infinite loop
        while(1) {}
    }

    *fs = filesystem;
}

static void fs_static_load()
{
    fs_insert_filesystem(fat16_init());
}

void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;
    for (int i = 0; i < UROS_MAX_FILE_DESCRIPTORS; i++)
    {
        if (file_descriptors[i] == 0)
        {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            // Descriptors start at 1
            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }
    }

    return res;
}

static struct file_descriptor* file_get_descriptor(int fd)
{
    if (fd <= 0 || fd >= UROS_MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }

    // Descriptors start at 1
    int index = fd - 1;
    return file_descriptors[index];
}

struct filesystem* fs_resolve(struct disk* disk)
{
    struct filesystem* fs = 0;
    for (int i = 0; i < UROS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}

FILE_MODE file_get_mode_by_string(const char *str)
{
    FILE_MODE mode = FILE_MODE_INVALID;

    if (strncmp(str, "r", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if (strncmp(str, "w", 1) == 0)
    {
        mode = FILE_MODE_WRITE;
    }
    else if (strncmp(str, "a", 1) == 0)
    {
        mode = FILE_MODE_APPEND;
    }
    
    return mode;
}

int fopen(const char* filename, const char* mode_str)
{
    int res = 0;

    struct path_root* root_path = pathparser_parse(filename, NULL);

    if (!root_path)
    {
        res = -EINVARG; //fopen should not return negative value. So setting the errors make sense only for debug purpose.
        return 0;
    }

    if (!root_path->first)
    {
        res = -EINVARG;
        return 0; 
    }
    
    struct disk* disk = disk_get(root_path->drive_no);

    if (!disk)
    {
        res = -EIO;
        return 0;
    }

    if (!disk->filesystem)
    {
        res = -EIO;
        return 0;
    }
    
    FILE_MODE mode = file_get_mode_by_string(mode_str);

    if (mode == FILE_MODE_INVALID)
    {
        res = -EINVARG;
        return 0;
    }
    
    void *descriptor_private_data = disk->filesystem->open(disk, root_path->first, mode);

    if (ISERR(descriptor_private_data))
    {
        res = ERROR_I(descriptor_private_data);
        return 0;
    }
    
    struct file_descriptor *desc = 0;
    res = file_new_descriptor(&desc);

    if (res < 0)
        return 0;

    desc->filesystem = disk->filesystem;
    desc->private = descriptor_private_data;
    desc->disk = disk;
    res = desc->index;
    
    return res;
}

