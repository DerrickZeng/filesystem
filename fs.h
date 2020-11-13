#ifndef __FS__H__
#define __FS__H__

#include "fs_def.h"
#include "device_io.h"

void fs_init(void);

enum {
    FS_TYPE_FUFS,
    FS_TYPE_TOTAL,
    FS_TYPE_NULL
};

struct dev_fsctrl_s{
    int fs_type;
    struct fs_operate_functions_s* opfuncs;
    device_handle_t device;
    fs_filesystem_t* fs_ctrl;
};

bool fs_mount(device_handle_t device, char drive_letter, int fs_type);
bool fs_dev_fs_ctrl(char drive_letter, struct dev_fsctrl_s* ctrl);


#define FS_ERROR -1
#define FS_SUCCESS 0

int fs_open(const char* path);
void fs_close(int fd);

int fs_read(int fd, char* buf, int count);
int fs_write(int fd, const char* buf, int count);
bool fs_ftruncate(int fd, fs_off_t size);
fs_off_t fs_lseek(int fd, fs_off_t off, int where);

int fs_mkdir(const char* path);
int fs_rmdir(const char* path);

int fs_link(const char* src_path, const char* new_path);
int fs_unlink(const char* path);

int fs_symlink(const char* src_path, const char* new_path);
int fs_readlink(const char *path, char *buf, size_t size);

int fs_stat(const char *path, struct fs_stat *buf);

struct _fs_dir_wrapper_s;
typedef struct _fs_dir_wrapper_s FS_DIR;

FS_DIR* fs_opendir(const char *path);
int fs_readdir(FS_DIR* dir, char* name);
void fs_closedir(FS_DIR* dir);

int fs_format(device_handle_t device, int sectors_per_block, int fs_type);
fs_size_t fs_filesystem_used_size(char drive_letter);
fs_size_t fs_filesystem_total_size(char drive_letter);


char* fs_getcwd(char* buffer, size_t size);
int fs_chdir(const char* path);

char* fs_abs_path(const char* path, char* abs_path, size_t size);
#endif /* __FS__H__ */
