/*
 * 
 * M. Burak BUGRUL - 150140015
 * K. Emre OTO     - 150140032
 * Ezgi YILMAZ     - 150150036
 * 
 * System Programming Project 3
 * Read Only HTML Tidy FUSE Project
 * 
 */

#define FUSE_USE_VERSION 26

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fuse.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <tidy.h>
#include <tidybuffio.h>

char *rw_path;
char *config_file;

int min( int a, int b ){
    
    if( a < b )
        return a;
    
    return b;
}

int ends_with( char *str, char *end ){

    int str_len = strlen(str);
    int end_len = strlen(end);

    if( strcmp(str + str_len - end_len, end) == 0 )
        return 1;
    
    return 0;
}

char* tidyfy( char * input){

    TidyBuffer output = {0};
    TidyDoc tdoc = tidyCreate();
    tidyLoadConfig(tdoc, config_file);
    char *ret;

    int flag = tidyOptSetBool(tdoc, TidyXhtmlOut, yes);

    if(!flag)
        return NULL;
    
    flag = tidyParseString( tdoc, input );

    if( flag < 0 )
        return NULL;

    flag = tidyCleanAndRepair( tdoc );

    if( flag < 0 )
        return NULL;

    flag = tidySaveBuffer( tdoc, &output );

    if( flag < 0 )
        return NULL;

    ret = strdup(output.bp);

    return ret;
}

static char* translate_path(const char* path){

    char *rPath = malloc(sizeof(char) * (strlen(path) + strlen(rw_path) + 1));

    strcpy(rPath,rw_path);
    
    if (rPath[strlen(rPath)-1] == '/')
        rPath[strlen(rPath)-1]='\0';
    
    strcat(rPath,path);
    return rPath;
}

static int tidy_rofs_getattr(const char *path, struct stat *st_data){

    int res;
    char *upath = translate_path(path);

    res = lstat(upath, st_data);
    

    if( ends_with(upath, ".html") ){

        int size = st_data->st_size;
        char *input = malloc(size);

        int fd = open(upath, O_RDONLY);
        res = pread(fd, input, size, 0);

        if( res == -1 ){
            close(fd);
            free(input);
            free(upath);
            return -errno;
        }

        char* tidied = tidyfy(input);

        if( tidied == NULL ){
            close(fd);
            free(input);
            free(upath);
            return -EAGAIN;
        }

        
        close(fd);
        free(input);

        st_data->st_size = strlen(tidied);
        free(tidied);
    }

    free(upath);
    
    if(res == -1)
        return -errno;
    
    return 0;
}

static int tidy_rofs_readlink(const char *path, char *buf, size_t size){

    int res;
    char *upath = translate_path(path);

    res = readlink(upath, buf, size - 1);
    free(upath);

    if(res == -1)
        return -errno;
    
    buf[res] = '\0';
    return 0;
}

static int tidy_rofs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi){

    DIR *dp;
    int res;
    struct dirent *de;

    char *upath = translate_path(path);

    dp = opendir(upath);
    free(upath);

    if(dp == NULL)
        return res = -errno;

    while( de = readdir(dp) ){

        struct stat st;
        memset(&st, 0, sizeof(st));
        
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int tidy_rofs_mknod(const char *path, mode_t mode, dev_t rdev){ return -EROFS; }

static int tidy_rofs_mkdir(const char *path, mode_t mode){ return -EROFS; }

static int tidy_rofs_unlink(const char *path){ return -EROFS; }

static int tidy_rofs_rmdir(const char *path){ return -EROFS; }

static int tidy_rofs_symlink(const char *from, const char *to){ return -EROFS; }

static int tidy_rofs_rename(const char *from, const char *to){ return -EROFS; }

static int tidy_rofs_link(const char *from, const char *to){ return -EROFS; }

static int tidy_rofs_chmod(const char *path, mode_t mode){ return -EROFS; }

static int tidy_rofs_chown(const char *path, uid_t uid, gid_t gid){ return -EROFS; }

static int tidy_rofs_truncate(const char *path, off_t size){ return -EROFS; }

static int tidy_rofs_utime(const char *path, struct utimbuf *buf){ return -EROFS; }

static int tidy_rofs_open(const char *path, struct fuse_file_info *finfo){
    
    int res;
    int flags = finfo->flags;

    if ((flags & O_WRONLY) || (flags & O_RDWR) || (flags & O_CREAT) || (flags & O_EXCL) || (flags & O_TRUNC) || (flags & O_APPEND))
        return -EROFS;

    char *upath = translate_path(path);

    res = open(upath, flags);
    free(upath);

    if(res == -1)
        return -errno;
    
    close(res);
    return 0;
}

static int tidy_rofs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *finfo){

    int fd;
    int res;
    int is_html = 0;

    char *upath = translate_path(path);

    if( ends_with(upath, ".html") )
        is_html = 1;

    fd = open(upath, O_RDONLY);
    free(upath);

    if(fd == -1)
        return res = -errno;
    
    if( is_html ){

        char *input = malloc(size);

        res = pread(fd, input, size, offset);

        if( res == -1 ){
            close(fd);
            free(input);
            return -errno;
        }

        char* tidied = tidyfy(input);

        if( tidied == NULL ){
            close(fd);
            free(input);
            return -EAGAIN;
        }
        
        close(fd);
        free(input);

        strcpy(buf, tidied);
        return strlen(tidied);
    }
    else
        res = pread(fd, buf, size, offset);

    if(res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int tidy_rofs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo){ return -EROFS; }

static int tidy_rofs_statfs(const char *path, struct statvfs *st_buf){

    int res;
    char *upath = translate_path(path);

    res = statvfs(upath, st_buf);
    free(upath);

    if (res == -1)
        return -errno;
    
    return 0;
}

static int tidy_rofs_release(const char *path, struct fuse_file_info *finfo){ return 0; }

static int tidy_rofs_fsync(const char *path, int crap, struct fuse_file_info *finfo){ return 0; }

static int tidy_rofs_access(const char *path, int mode){

    int res;
    char *upath = translate_path(path);

    if (mode & W_OK)
        return -EROFS;

    res = access(upath, mode);
    free(upath);

    if (res == -1)
        return -errno;
    
    return res;
}

static int tidy_rofs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags){ return -EROFS; }

static int tidy_rofs_getxattr(const char *path, const char *name, char *value, size_t size){

    int res;
    char *upath = translate_path(path);

    res = lgetxattr(upath, name, value, size);
    free(upath);

    if(res == -1)
        return -errno;
    
    return res;
}

static int tidy_rofs_listxattr(const char *path, char *list, size_t size){
    
    int res;
    char *upath = translate_path(path);

    res = llistxattr(upath, list, size);
    free(upath);

    if(res == -1)
        return -errno;
    
    return res;
}

static int tidy_rofs_removexattr(const char *path, const char *name){ return -EROFS; }

struct fuse_operations rofs_oper = {
    .getattr     = tidy_rofs_getattr,
    .readlink    = tidy_rofs_readlink,
    .readdir     = tidy_rofs_readdir,
    .mknod       = tidy_rofs_mknod,
    .mkdir       = tidy_rofs_mkdir,
    .symlink     = tidy_rofs_symlink,
    .unlink      = tidy_rofs_unlink,
    .rmdir       = tidy_rofs_rmdir,
    .rename      = tidy_rofs_rename,
    .link        = tidy_rofs_link,
    .chmod       = tidy_rofs_chmod,
    .chown       = tidy_rofs_chown,
    .truncate    = tidy_rofs_truncate,
    .utime       = tidy_rofs_utime,
    .open        = tidy_rofs_open,
    .read        = tidy_rofs_read,
    .write       = tidy_rofs_write,
    .statfs      = tidy_rofs_statfs,
    .release     = tidy_rofs_release,
    .fsync       = tidy_rofs_fsync,
    .access      = tidy_rofs_access,
    .setxattr    = tidy_rofs_setxattr,
    .getxattr    = tidy_rofs_getxattr,
    .listxattr   = tidy_rofs_listxattr,
    .removexattr = tidy_rofs_removexattr
};

static struct fuse_opt tidy_rofs_opts[] = {
    FUSE_OPT_END
};

static int tidy_rofs_parse_opt(void *data, const char *arg, int key,
                          struct fuse_args *outargs){
    
    if(key == FUSE_OPT_KEY_OPT )
        return 1;
    else if( key == FUSE_OPT_KEY_NONOPT ){
        if( rw_path )
            return 1;
        else if( config_file ){
            rw_path = strdup(arg);
            return 0;
        }
        else{
            config_file = strdup(arg);
            return 0;
        }
    }

    fprintf(stderr, "Missing paremater(s)\n");
    exit(1);
}

int main( int argc, char **argv ){

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    int res;

    res = fuse_opt_parse(&args, &rw_path, tidy_rofs_opts, tidy_rofs_parse_opt);

    printf("%s %s\n", rw_path, config_file);

    if (rw_path == 0){
        fprintf(stderr, "Missing readwritepath\n");
        exit(1);
    }

    fuse_main(args.argc, args.argv, &rofs_oper, NULL);

    return 0;
}