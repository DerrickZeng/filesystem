#ifndef __FUFS__FILESYSETM__H__
#define __FUFS__FILESYSETM__H__

#include <stdint.h>
#include "superblock.h"
#include "block.h"
#include "../device_io.h"
#include "../fs_def.h"


typedef enum {
    FUFS_SUCCESS,
    FUFS_FAIL

}fufs_errcode_t;

typedef struct {
    superblock_t sb;
}fufs_filesystem_t;


fufs_filesystem_t* fufs_filesystem_new(device_handle_t device);
bool fufs_filesystem_init(fufs_filesystem_t* fs, device_handle_t device);

fs_size_t fufs_filesystem_used_size(fufs_filesystem_t* fs);
fs_size_t fufs_filesystem_total_size(fufs_filesystem_t* fs);

bool fufs_format(device_handle_t device, int sectors_per_block);





#endif /* __FUFS__FILESYSETM__H__ */
