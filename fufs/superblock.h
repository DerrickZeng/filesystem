#ifndef __FUFS_SUPERBLOCK__H__
#define __FUFS_SUPERBLOCK__H__

#include <stdint.h>
#include <stddef.h>

#include "../device_io.h"
#include "block.h"
#include "def.h"


typedef uint64_t fssize_t;

typedef struct {
    uint16_t sectors_per_block; 
    sector_no_t sectors;
    uint64_t total_size;
    inode_no_t root_dir;
    block_no_t inode_table_block; 
    inode_no_t inode_total_count;
    block_no_t data_block;
    block_no_t data_block_free_stack; 

    inode_no_t used_inode_count; 
    block_no_t used_data_block_count; 
}superblock_t;

size_t superblock_bin_size(void);

void superblock_dump_to_bin(superblock_t* sb, char* bin, size_t bin_size);
void superblock_load_from_bin(const char* bin, superblock_t* sb);

void superblock_create(superblock_t* sb,  sector_no_t sectors, int sectors_per_block,
                       block_no_t inode_table, inode_no_t inode_count, block_no_t data_block, block_no_t data_block_free_stack, inode_no_t root_inode);


bool superblock_load(device_handle_t device, superblock_t* sb);
bool superblock_dump(device_handle_t device, superblock_t* sb);

block_no_t superblock_block_count(const superblock_t* sb);
size_t superblock_block_size(const superblock_t* sb);
int superblock_sectors_per_block(const superblock_t *sb);

uint64_t superblock_total_size(const superblock_t* sb); 
uint64_t superblock_used_size(const superblock_t* sb); 
uint64_t superblock_free_size(const superblock_t* sb); 
void superblock_set_used_size(superblock_t* sb, uint64_t new_size); 
inode_no_t superblock_root_dir_inode(const superblock_t* sb);


block_no_t superblock_inode_table_start(const superblock_t* sb);
block_no_t superblock_inode_table_blocksize(const superblock_t* sb); 
block_no_t superblock_data_block_start(const superblock_t* sb);
block_no_t superblock_data_block_size(const superblock_t* sb); 

block_no_t superblock_data_block_free_stack(const superblock_t* sb); 

void superblock_data_block_free_stack_set(superblock_t* sb, block_no_t new_stack);


#endif /* __FUFS_SUPERBLOCK__H__ */
