#ifndef __FUFS__FILE_DIR__H__
#define __FUFS__FILE_DIR__H__

#include "base_file.h"
#include "filesystem.h"

#include "../fs_def.h"

typedef struct {
    base_file_t base_file;
}fufs_file_t;


fufs_file_t* fufs_open(device_handle_t device, fufs_filesystem_t* fs, const char* path);
void fufs_close(fufs_file_t* file);

int fufs_read(fufs_file_t* file, char* buf, int count);
int fufs_write(fufs_file_t* file, const char* buf, int count);
bool fufs_ftruncate(fufs_file_t* file, fs_off_t size);
fs_off_t fufs_lseek(fufs_file_t* file, fs_off_t off, int where);

bool fufs_mkdir(device_handle_t device, fufs_filesystem_t* fs, const char* path);
bool fufs_rmdir(device_handle_t device, fufs_filesystem_t* fs, const char* path);

bool fufs_link(device_handle_t device, fufs_filesystem_t* fs, const char* src_path, const char* new_path);
bool fufs_unlink(device_handle_t device, fufs_filesystem_t* fs, const char* path);

bool fufs_symlink(device_handle_t device, fufs_filesystem_t* fs, const char* src_path, const char* new_path);
bool fufs_readlink(device_handle_t device, fufs_filesystem_t* fs, const char *path, char *buf, size_t size);

bool fufs_stat(device_handle_t device, fufs_filesystem_t* fs, const char *path, struct fs_stat *buf);


typedef struct {
    base_file_t base_file;
}fufs_dir_t;

fufs_dir_t* fufs_opendir(device_handle_t device, fufs_filesystem_t* fs, const char *path);
bool fufs_readdir(fufs_dir_t* dir, char* name);
bool fufs_closedir(fufs_dir_t* dir);

#endif /* __FUFS__FILE_DIR__H__ */
