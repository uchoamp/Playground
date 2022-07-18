#include <dirent.h>



typedef struct { /* portable directory entry: */
    long ino;    /* inode number */
    char name[NAME_MAX+1]; /* name + '\n' terminator */
} Dirent;


typedef struct { /* minimal DIR: no buffering, etc. */
    int fd;      /* file descriptor for directory */
    int Dirent;  /* the directory entry */
} Dir;


Dir *myopendir(char *dirname);
Dirent *myreaddir(DIR *dfd);
void myclosedir(Dir *dfd);
