/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:57:20
 * @modify date 2021-12-26 10:57:20
 * @desc [description]
 */

#include "pparser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"

static int path_parser_valid_format(const char *filename)
{
    int len = strnlen(filename, UROS_MAX_PATH);

    return (len >= 3 && is_digit(filename[0]) && memcmp((void*)&filename[1], ":/", 2) == 0);
}

static int pathparser_get_drive_by_path(const char **path)
{
    if (!path_parser_valid_format(*path))
    {
        return -EBADPATH;
    }

    int drive_no = to_numeric_digit(*path[0]);

    // Add 3 bytes to skip drive number; eg: 0:/

    *path += 3;

    return drive_no;
}

static struct path_root* pathparser_create_root(int drive_number)
{
    struct path_root *path_r = kzalloc(sizeof(struct path_root));
    path_r->drive_no = drive_number;
    path_r->first = 0;

    return path_r;
}

static const char *pathparser_get_path_part(const char **path)
{
    char *result_path_part = kzalloc(UROS_MAX_PATH);
    int i = 0;

    while (**path != '/' && **path != 0x00)
    {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/')
    {
        *path += 1;
    }
    
    if (i == 0)
    {
        kfree(result_path_part);
        result_path_part = 0;
    }
    
    return result_path_part;
}

struct path_part *pathparser_parse_path_part(struct path_part *last_part, const char **path)
{
    const char *path_part_str = pathparser_get_path_part(path);

    if (!path_part_str)
    {
        return 0;
    }
    
    struct path_part *part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0x00;

    if (last_part)
    {
        last_part->next = part;
    }
    
    return part;
}

void pathparser_free(struct path_root *root)
{
    struct path_part *part = root->first;

    while (part)
    {
        struct path_part *next_part = part->next;
        kfree((void*)part->part);
        kfree(part);
        part = next_part;
    }
    
    kfree(root);
}

struct path_root *pathparser_parse(const char* path, const char *current_directory_path)
{
    int res = 0;
    const char *tmp_path = path;
    struct path_root *path_root = 0;

    if (strlen(path) > UROS_MAX_PATH)
    {
        return 0;
    }
    
    res = pathparser_get_drive_by_path(&tmp_path);

    if (res < 0)
    {
        return 0;
    }
    
    path_root = pathparser_create_root(res);

    if (!path_root)
    {
        return 0;
    }
    
    struct path_part *first_path = pathparser_parse_path_part(NULL, &tmp_path);

    if (!first_path)
    {
        return 0;
    }
   
    
    path_root->first = first_path;
    struct path_part *part = pathparser_parse_path_part(first_path, &tmp_path);

    while (part)
    {
        part = pathparser_parse_path_part(part, &tmp_path);
    }
    
    return path_root;
}