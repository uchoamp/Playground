#define NAME_MAX 14


typedef struct {
    long ino;
    char name[NAME_MAX+1];
} Dirent;


typedef struct {
    int fd;
    int Dirent;
} Dir;


Dir *opendir(char *dirname);
Dirent *readdir(Dir *dfd);
void closedir(Dir *dfd);
