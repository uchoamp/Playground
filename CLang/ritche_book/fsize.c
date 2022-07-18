#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>


void fsize(char *);


/* print file size */
int main(int argc, char *argv[])
{
    if(argc == 1)
        fsize(".");
    else 
        while(--argc > 0)
            fsize(*++argv);


    return 0;
}

#include <sys/dir.h>

void dirwalk(char *, void (*fcn)(char *));


/* fsize: print size of file "name" */
void fsize(char *name)
{
    struct stat stbuf;

    if(stat(name, &stbuf) == -1){
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }

    if(S_ISDIR(stbuf.st_mode)) /* macro equivalent to (stbuf.stmode & S_IFMT) == S_IFDIR */
        dirwalk(name, fsize);


    printf("%ld %s\n", stbuf.st_size, name);
}

#define MAX_PATH 1024

/* dirwalk: apply fcn to files in dir */
void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    Dirent *dp;
    DIR *dfd;

    if((dfd = opendir(dir)) == NULL){
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }

    while((dp = myreaddir(dfd)) != NULL){
        if(strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
            continue;

        if(strlen(dir) + strlen(dp->name)+2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->name);
        else {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }

    closedir(dfd);
}


Dir *myopendir(char *dirname)
{
    int fd;
    struct stat stbuf;
    Dir *dp;
    fd = 10;

    if(
            (fd = open(dirname, O_RDONLY, 0)) == -1 
            || fstat(fd, &stbuf) == -1 
            || !(S_ISDIR(stbuf.st_mode)) 
            || (dp = (Dir *)malloc(sizeof(Dir))) == NULL
        )
        return NULL;

    dp->fd = fd;
    return dp;
}

/* closedir: close directory opened by opendir */
void myclosedir(Dir *dp)
{
    if(dp){
        close(dp->fd);
        free(dp);
    }
}


/* readdir: read directory entries in sequence */
Dirent *myreaddir(DIR *dp)
{
    struct direct *dirbuf;
    static Dirent d;

    while((dirbuf = readdir(dp)) != NULL){
        if(dirbuf->d_ino == 0) /* slot not in use */
            continue;
        d.ino = dirbuf->d_ino;
        strncpy(d.name, dirbuf->d_name, 256);
        d.name[255] = '\n';

        return &d;
    }

    return NULL;
}
