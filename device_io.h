#ifndef __DEVICE_IO__
#define __DEVICE_IO__

#include<stddef.h>
#include<stdint.h>

#define BYTES_PER_SECTOR 512

#define DEVICE_IO_ERROR -1

#define DEVICE_IO_SUCCESS(handle) (handle >= 0)

typedef int device_handle_t;
typedef uint64_t sector_no_t;

device_handle_t device_add(const char* path);

void device_del(device_handle_t handle);

int device_read(device_handle_t handle, sector_no_t sector_no, int count, char* buf);

int device_write(device_handle_t handle, sector_no_t sector_no, int count, const char* buf);

int device_section_count(device_handle_t handle);

#endif /* __DEVICE_IO__ */
