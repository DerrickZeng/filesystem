#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "fs.h"

#include "memory/alloc.h"
#include "device_io.h"
#include "utils/sys.h"
#include "shell.h"

#define ERROR -1

int create(const char* path, size_t size);
int format(const char* path, const char* type, int block_size);

int enter(void);


int main(int argc, char* argv[]){
    fs_init();

    int arg_count = argc - 1;
    char** args = argv + 1;

    if (arg_count == 0) {
        return enter();
    }

    const char* sub_cmd = args[0];
    if (strcmp(sub_cmd, "create") == 0) {

        if (arg_count < 3) {
            fprintf(stderr, "command not found!\n");
            return ERROR;
        }

        char* end;
        size_t size = strtoumax(args[2], &end, 10);
        return create(args[1], size);

    }else if (strcmp(sub_cmd, "format") == 0) {

        if (arg_count < 4) {
            fprintf(stderr, "command not found!\n");
            return ERROR;
        }

        char* end;
        int block_size = strtol(args[3], &end, 10);
        return format(args[1], args[2], block_size);

    } else if (strcmp(sub_cmd, "enter") == 0) {

        return enter();

    } else if (strcmp(sub_cmd, "help") == 0) {

        printf("Help command:\n"
               "create <FilePath> <FileSize>  create a container file\n"
               "format <ContainerPath> <FileSystemType> <SectorSize> \n"
               "enter  enter file system shell\n"
               "help \n"
               );

    } else {
        fprintf(stderr, "Bad command!\n");
        return ERROR;
    }
}

int create(const char* path, size_t size)
{
    if (ft_create_bin_file(path, size)) {
        printf("Create container file %s successfully，file size %d\n", path, (int)size);
        return ERROR;
    } else {
        printf("Create container file %s，file size %d unsuccessfully\n", path, (int)size);
        return 0;
    }
}

int format(const char* path, const char* type, int block_size)
{
    int fs_type;
    if (strcmp(type, "fufs") == 0) {
        fs_type = FS_TYPE_FUFS;
    } else {
        printf("unkown file system type\n");
        return ERROR;
    }

    int device = device_add(path);
    if (!DEVICE_IO_SUCCESS(device)) {
        printf("mount failed\n");
        return ERROR;
    }

    int sectors_per_block = block_size / BYTES_PER_SECTOR;
    if (! (0 < sectors_per_block && sectors_per_block <= 16)) {
        printf("Sectors per block is limited from 1 to 16");
        device_del(device);
        return ERROR;
    }


    if (fs_format(device, sectors_per_block, fs_type) != FS_SUCCESS) {
        printf("The format did not complete successfully.\n");
        device_del(device);
        return ERROR;
    } else {
        printf("Format complete! Black siz: %d\n", sectors_per_block * BYTES_PER_SECTOR);
        device_del(device);
        return 0;
    }
}

int enter(void)
{
    const char* config_file = "config.txt";

    FILE* fp = fopen(config_file, "rb");
    if (fp == NULL) {
        printf("Failed to open the configuration file！Maybe no configuration file？\n");
        return ERROR;
    }

    /* mount a device */
    while (true) {
        char buf[32];
        char path[FS_MAX_FILE_PATH];
        int ret = fscanf(fp, "%s%s", buf, path);
        char drive_letter = buf[0];
        if (ret == EOF || ret == 0) {
            break;
        }

        if (ret != 2) {
            printf("Error parsing config file%s\n", config_file);
            fclose(fp);
            return ERROR;
        }

        int device = device_add(path);
        if (!DEVICE_IO_SUCCESS(device)) {
            printf("Failed to mount：number%c container file%s\n", drive_letter, path);
            continue;
        }

        if (!fs_mount(device, drive_letter, FS_TYPE_FUFS)) {
            printf("Failed to mount：number%c container file%s\n", drive_letter, path);
            continue;
        }

        printf("Mount successfully：number%c container file%s\n", drive_letter, path);
    }
    fclose(fp);



    printf("******** Welcome to use fufs file system shell！ ********\n");
    return shell_main();
}
