/**
 * @author Alex Sangiuliano
 * @email alex22_7@hotmail.com
 * @create date 2021-12-26 10:56:52
 * @modify date 2021-12-26 10:56:52
 * @desc [description]
 */

#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct disk_stream *diskstreamer_new(int disk_id)
{
    struct disk *disk = disk_get(disk_id);

    if (!disk)
    {
        return 0; //NULL
    }
    
    struct disk_stream *streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}

int diskstreamer_seek(struct disk *disk, struct disk_stream *stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream *stream, void *out, int total)
{
    // calculate sector

    int sector = stream->pos / UROS_SECTOR_SIZE;
    int offset = stream->pos % UROS_SECTOR_SIZE;

    char buf[UROS_SECTOR_SIZE];

    int res = disk_read_block(stream->disk, sector, 1, buf);

    if (res < 0)
    {
        return res;
    }

    int total_to_read = total > UROS_SECTOR_SIZE ? UROS_SECTOR_SIZE : total;

    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset+i];
    }
    
    stream->pos += total_to_read;

    if (total > UROS_SECTOR_SIZE)
    {
        res = diskstreamer_read(stream, out, total-UROS_SECTOR_SIZE);
    }

    return res;
    
}

void diskstreamer_close(struct disk_stream *stream)
{
    kfree(stream);
}