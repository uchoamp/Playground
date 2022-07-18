#include <stdio.h>
#include <string.h>

typedef long Align;
typedef union header Header;


union header {
    struct{
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

/* mymalloc: general-purpose storage allocator */
void *mymalloc(unsigned nbytes);
/* free: put block ap in free list */

void myfree(void *ap);

int main(int argc, char *argv[]){
    char *ca;
    
    ca = (char *)mymalloc(20);
    strcpy(ca, "Hello, World!!");

    while(*ca)
        putchar(*ca++);


    putchar('\n');
    return 0;
}

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */
#define NALLOC 1024 /* minimum #units to request */


/* free: put block ap in free list */
void myfree(void *ap) 
{
    Header *bp, *p;

    bp = (Header *)ap - 1;
    for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* freed block at start or end of arena */

    if(bp + bp->s.size == p->s.ptr){ /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;


    if(p + p->s.size == bp){ /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;   
    } else 
        p->s.ptr = bp;

    freep = p;

}

/* morecore: ask system for more memory */
static Header *morecore(unsigned nu){
    char *cp, *sbrk(int);
    Header *up;

    if(nu < NALLOC)
        nu = NALLOC;

    cp = sbrk(nu * sizeof(Header));
    if(cp == (char *) -1)
        return NULL;

    up = (Header *)cp;
    up->s.size = nu;

    myfree((void *)(up+1));

    return freep;
}

/* mymalloc: general-purpose storage allcoator */
void *mymalloc(unsigned nbytes)
{
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
    if((prevp = freep) == NULL){
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for(p = prevp->s.ptr; ;prevp = p, p = p->s.ptr){

        if(p->s.size >= nunits){
            if(p->s.size == nunits)
                prevp->s.ptr = p->s.ptr; 
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freep = prevp;
            return (void *)(p+1);
        }

        if(p == freep) /* wrapped around free list */
            if((p = morecore(nunits)) == NULL)
                return NULL;
    }

    return NULL;
}


