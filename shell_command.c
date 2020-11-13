#include "shell_command.h"

#include <string.h>
#include "utils/path.h"
#include "utils/sys.h"
/*
int cmd_pwd(int argc, char* argv[])
{
    char path[FS_MAX_FILE_PATH];
    fs_getcwd(path, FS_MAX_FILE_PATH);
    printf("%s\n", path);
    return 0;
}

int cmd_cd(int argc, char* argv[])
{
    if (argc <= 0) {
        printf("unrecognized command\n");
        return -1;
    }

    if (fs_chdir(argv[0]) == FS_SUCCESS) {
        return 0;
    } else {
        printf("Change failed，maybe catalog does not exist."\n");
        return -1;
    }

}

int cmd_ls(int argc, char* argv[])
{
    const char* path;
    char wd[FS_MAX_FILE_PATH];
    if (argc <= 0) {
        fs_getcwd(wd, FS_MAX_FILE_PATH);
        path = wd;
    } else {
        path = argv[0];
    }

    FS_DIR* dir = fs_opendir(path);
    if (dir == NULL) {
        printf("Failed to list catalog information.\n");
    }

    char name[32];
    do {
        fs_readdir(dir, name);
        printf("%s ", name);
    } while(name[0] != '\0');
    printf("\n");

    fs_closedir(dir);

    return 0;
}

int cmd_mkdir(int argc, char* argv[])
{
    if (argc <= 0) {
        printf("unrecognized command\n");
        return -1;
    }

    if (fs_mkdir(argv[0]) == FS_SUCCESS) {
        return 0;
    } else {
        printf("Failed.\n");
        return -1;
    }
}

*/

static bool tree(char* path, int* p_level)
{
    for (int i = 0; i < *p_level; i++) {
        printf("   ");
    }
    printf("%s\n", path_p_basename(path));

    struct fs_stat st;
    fs_stat(path, &st);
    if (st.st_mode != FS_S_IFDIR) {
        return true;
    }

    FS_DIR* dir = fs_opendir(path);
    char name[32];

    while ((fs_readdir(dir, name), name[0] != '\0')) {
        size_t tail = strlen(path);
        path_join(path, FS_MAX_FILE_PATH, name);

        (*p_level)++;
        tree(path, p_level);
        (*p_level)--;
        path[tail] = '\0';
    }

    fs_closedir(dir);
        return true;
}

int cmd_tree(int argc, char* argv[])
{
    char path[FS_MAX_FILE_PATH];
    if (argc <= 0) {
        fs_getcwd(path, FS_MAX_FILE_PATH);
    } else {
        strcpy(path, argv[0]);
    }

    int level = 0;
    tree(path, &level);
    return 0;
}


int cmd_createfile(int argc, char* argv[])
{
    if (argc != 2) {
        printf("The parameter is incorrect！\n");
        return -1;
    }

    int ret = fs_stat(argv[0], NULL);
    if(ret == FS_SUCCESS) {
        printf("There is already a file with the same name.\n");
        return -1;
    }

    int fd = fs_open(argv[0]);
    fs_write(fd, argv[1], strlen(argv[1]));
    fs_close(fd);

    return 0;
}

int cmd_cat(int argc, char* argv[])
{
    if (argc != 1) {
        printf("The parameter is incorrect！\n");
        return -1;
    }

    struct fs_stat st;
    int ret = fs_stat(argv[0], &st);
    if(ret != FS_SUCCESS) {
        printf("The file does not exist\n");
        return -1;
    }

    if (st.st_mode != FS_S_IFREG) {
        printf("This file is in a format we don't support！\n");
        return -1;
    }

    char buf[1024];
    int size = 1024;


    int fd = fs_open(argv[0]);
    while (true) {
        int count = fs_read(fd, buf, size - 1);
        buf[count] = '\0';
        printf("%s", buf);
        if (count != size - 1) {
            break;
        }
    }
    printf("\n");
    fs_close(fd);

    return 0;
}

int cmd_df(int argc, char* argv[])
{
    printf("Disk\tDeviceNum\tType\tAlreadyUsed\tTotal\n");
    for (int i = 0; i <= 'z' - 'a'; i++) {
        struct dev_fsctrl_s ctrl;
        if (fs_dev_fs_ctrl(i + 'a', &ctrl)) {
            printf("%c\t", i + 'A');
            printf("%d\t", ctrl.device);

            printf("%s\t", ctrl.fs_type == FS_TYPE_FUFS ? "fufs" : "unknown");

            size_t size;
            char sym = ft_human_size((size_t)fs_filesystem_used_size(i + 'a'), &size);
            printf("%d%c\t", (int)size, sym);

            sym = ft_human_size((size_t)fs_filesystem_total_size(i + 'a'), &size);
            printf("%d%c\t", (int)size, sym);

            printf("\n");
        }
    }
    return 0;
}

int cmd_pwd(int argc, char* argv[])
{
    char path[FS_MAX_FILE_PATH];
    fs_getcwd(path, FS_MAX_FILE_PATH);
    printf("%s\n", path);
    return 0;
}

int cmd_cd(int argc, char* argv[])
{
	if(argc !=1){
	   printf("Input was not in a correct format.");
	   return -1; 
   }
   int a = fs_chdir(argv[0]); 
   if(a == -1){
	   printf("Failed to change the path.\n");
	   return -1;
   }
    return 0;
}

int cmd_ls(int argc, char* argv[])
{
	if(argc == 0){
		char path[FS_MAX_FILE_PATH];
	    fs_getcwd(path,FS_MAX_FILE_PATH);
		FS_DIR* dir  =  fs_opendir(path);
    if (dir == NULL){
        printf("This is not in a valid path name.\n");
        return -1;
    }
		char name[100] = "no";
        for(int i = 0;name[0] != '\0';i++){
	       fs_readdir(dir, name);
           printf("%s ",name);
        }
        printf("\n");
		return -1;
	}
	if(argc > 1){
	   printf("Input was not in a correct format.\n");
	   return -1;
   }
   FS_DIR* dir  =  fs_opendir(argv[0]);
   if (dir == NULL){
	   printf("This is not in a valid path name.\n");
	   return -1;
   }
   char name[100]="no";
   for(int i = 0;name[0] != '\0';i++){
	  fs_readdir(dir, name);
      printf("%s ",name);	  
   }
   printf("\n");
    return 0;
}

int cmd_mkdir(int argc, char* argv[])
{
	int a = fs_mkdir(argv[0]);
	if(a == -1){
		printf("Input was not in a correct format.");
		return -1;
	}
    return 0;
}

int cmd_rmdir(int argc, char* argv[])
{
	struct fs_stat st;
	int ret = fs_stat(argv[0], &st);
	if(ret == -1){
		printf("Catalog does not exist.");
		return -1;
	}
	
	else if (st.st_mode == FS_S_IFDIR) {
		fs_rmdir(argv[0]);
	}
	else printf("Failed to delete the catalog.");
    return 0;
}

int cmd_stat(int argc, char* argv[])
{
    if (argc != 1) {
        printf("The parameter is incorrect\n");
        return -1;
    }

	struct fs_stat  st;
	int ret = fs_stat(argv[0], &st);
  if (ret != FS_SUCCESS) {
      printf("The path does not exist.\n");
      return -1;
  }
	if (st.st_mode == FS_S_IFDIR){
		printf("This file is catalog.\n");
	}
	else if (st.st_mode == FS_S_IFLNK){
		printf("This file is symbolic link.\n");
	}
	else if (st.st_mode == FS_S_IFREG){
		printf("此文件为普通文件\n");
	}
	printf("inode number：  %d\n", (int)st.st_ino);
	printf("hard link number：  %d\n", (int)st.st_nlink);
	printf("file size：    %d\n", (int)st.st_size);
	printf("block size：      %d\n", (int)st.st_blksize);
	printf("number of block：        %d\n", (int)st.st_blocks);

  char buf[32];
  size_t size = 32;
  strftime(buf, size, "%Y-%m-%d %H:%M:%S", localtime(&st.st_ctime));
	printf("date created：            %s\n", buf);

  strftime(buf, size, "%Y-%m-%d %H:%M:%S", localtime(&st.st_atime));
	printf("last visited time：    %s\n", buf);

  strftime(buf, size, "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));
	printf("last edited time：    %s\n", buf);
    return 0;
}

int cmd_cp(int argc, char* argv[])
{
    int sourceFile;
    int targetFile;
    if(argc == 2)
    {
        sourceFile = fs_open(argv[0]);
        if (sourceFile == FS_ERROR)
        {
            /* code */
            printf("Failed！\n");
            return 0;
        }
        targetFile = fs_open(argv[1]);
        if (targetFile == FS_ERROR)
        {
            /* code */
            printf("Failed！\n");
            return 0;
        }
        char buf[1024];
        int c = fs_read(sourceFile, buf, sizeof(buf) - 1);
        while(c > 0)
        {
            fs_write(targetFile, buf, c);
            c = fs_read(sourceFile, buf, sizeof(buf) - 1);
        }

        fs_close(sourceFile);
        fs_close(targetFile);
    }
    else
    {
        printf("Command format is incorrect！\n");
    }
    return 0;
}

int cmd_mv(int argc, char* argv[])
{
    if (argc == 2)
    {
        int isSuccessed;
        isSuccessed = fs_link(argv[0],argv[1]);
        if (isSuccessed == FS_ERROR)
        {
            printf("Failed to move！\n");
            return 0;
        }
        fs_unlink(argv[0]);
    }
    else
    {
       printf("Command format is incorrect！\n");
    }
    return 0;
}

int cmd_rm(int argc, char* argv[])
{
    int isSuccessed;
    for(int i =0;i<argc;i++)
    {
            isSuccessed =  fs_unlink(argv[i]);
            if(isSuccessed == FS_ERROR)
            {
                printf("%s:Failed to delete！\n",argv[i]);
            }
    }
    return 0;
}

int cmd_ln(int argc, char* argv[])
{
    int isSuccessed;
    if (argc == 2)
    {
        isSuccessed = fs_link(argv[0],argv[1]);
        if (isSuccessed == FS_ERROR)
        {
            /* code */
            printf("Failed！\n");
        }
    }
    else if (argc == 3)
    {
        /* code */
        if (argv[0][0] == '-' && argv[0][1] == 's' && argv[0][2] == '\0')
        {
            /* code */
            isSuccessed = fs_symlink(argv[1],argv[2]);
            if (isSuccessed == FS_ERROR)
            {
                /* code */
                printf("Failed！\n");
            }
        }
        else
        {
            printf("Command format is incorrect！\n");
        }
    }
    else
    {
        printf("Command format is incorrect！\n");
    }
    return 0;
}

