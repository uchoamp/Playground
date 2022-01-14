#include <stdio.h>


int main(int argc, char *argv[])
{
    while (--argc > 0)
        printf("%x%s", *++argv, (argc > 1) ? " " : "");
    printf("\n");
    return 0;
}
