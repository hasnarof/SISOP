#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

char buffer[1000];

void encode(const char *filename)
{
    sprintf(buffer, "Jago_%s", filename);
}

void decode(const char *filename)
{
    const char *s = "/";
    memset(buffer, '\0', sizeof(buffer));

    char *token = strtok((char *)filename, s);
    while (token != NULL)
    {
        printf("token - %s\n", token);
        strcat(buffer, "/");
        if (!strncmp(token, "Jago_", 5))
            strcat(buffer, token + 5);
        else
            strcat(buffer, token);
        token = strtok(NULL, s);
    }
    printf("%s\n", buffer);
}

static const char *dirpath = "/home/almond/Downloads";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    decode(path);
    sprintf(fpath, "%s%s", dirpath, buffer);
    printf("%s\n", fpath);

    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        decode(path);
        sprintf(fpath, "%s%s", dirpath, buffer);
    }

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;

    dp = opendir(fpath);

    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
        {
            encode(de->d_name);
            res = (filler(buf, buffer, &st, 0));
        }
        else
            res = (filler(buf, de->d_name, &st, 0));

        if (res != 0)
            break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;

        sprintf(fpath, "%s", path);
    }
    else
    {
        decode(path);
        sprintf(fpath, "%s%s", dirpath, buffer);
    }

    printf("%s\n", fpath);

    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}