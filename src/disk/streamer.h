/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:57:00
 * @modify date 2021-12-26 10:57:00
 * @desc [description]
 */

#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"

struct disk_stream
{
    int pos;
    struct disk *disk;
};

void diskstreamer_close(struct disk_stream *stream);
int diskstreamer_read(struct disk_stream *stream, void *out, int total);
int diskstreamer_seek(struct disk_stream *stream, int pos);
struct disk_stream *diskstreamer_new(int disk_id);

#endif