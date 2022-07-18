#define EOF (-1)
#define BUFSIZ 1 
#define OPEN_MAX 20 /* max #files at once */

/*
typedef struct _flag_field 
{
    unsigned int f_read : 1;
    unsigned int f_write: 1;
    unsigned int f_unbuf: 1;
    unsigned int f_eof: 1;
    unsigned int f_err: 1;
} _flag;*/

typedef struct _iof {
    int cnt; /* charactes left */
    char *ptr; /* next character position */
    char *base; /* location of buffer */
    int flag; /* mode of file access */
    int fd; /* file descriptor */
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum _flags{
    _READ =  0b00001,    /* file open for reading */
    _WRITE = 0b00010,   /* file open for writing */
    _UNBUF = 0b00100,   /* file is unbuffered */
    _EOF =   0b01000,    /* EOF has occured on this file */
    _ERR =   0b10000/* error occurred on this file */
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 \
        ? (unsigned char) *(p)->ptr++ : _fillbuf(p))

#define putc(x, p) (--(p)->cnt >= 0 \
    ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)
