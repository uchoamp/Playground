#include <fcntl.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>
#include "stdoi.h"

#define PERMS 0666 /* RW for owner, group, others */

FILE _iob[OPEN_MAX] = { /* stdin, stdout, stderr: */ 
    {0, (char *) 0, 0, _READ , 0},
    {0, (char *) 0, 0, _WRITE, 1},
    {0, (char *) 0, 0, _WRITE | _UNBUF, 2},
};

/* fopen: open file, return file ptr */
FILE *myfopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if(*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if((fp->flag & (_READ | _WRITE)) == 0)
            break; /* found free slot */

    if(fp >= _iob + OPEN_MAX)   /* no free slots */
        return NULL;

    if(*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if((fd = open(name, O_WRONLY)) == -1)
                fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else 
        fd = open(name, O_RDONLY, 0);

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if((fp->flag & (_READ|_EOF|_ERR)) != _READ)
        return EOF;

    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    if(fp->base == NULL)
        if((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);

    if(--fp->cnt < 0)
    {
        if(fp->cnt == -1)
            fp->flag |= _EOF;
        else
            fp->flag |= _ERR;
        return EOF;
    }

    return (unsigned char) *fp->ptr++;
}


int _flushbuf(int c, FILE *fp){
    int bufsize, n_written;

    if(((*fp).flag & (_WRITE | _EOF | _ERR)) != _WRITE)
        return EOF;

    bufsize = (fp->flag) & _UNBUF ? 1 : BUFSIZ;

    if(fp->base == NULL){
        if((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;
    }
    else {
        n_written = write(fp->fd, fp->base, bufsize - fp->cnt);
        if(n_written !=  (bufsize - fp->cnt))
            return EOF;
    }

    fp->cnt = bufsize - 1;
    fp->ptr = fp->base;
    *fp->ptr++ = (unsigned char)c;

    return n_written;
}
