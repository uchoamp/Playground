#include <stdio.h>
#include <stdarg.h>

void minprintf(char *fmt, ...);

int main(int argc, char *argv[]){
    char c[10];
    scanf("%3c", c);
    printf("%s\n",c);
    return 0;
}

void minprintf(char *fmt, ...){
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);
    for(p = fmt; *p; p++){
        if(*p != '%'){
            putchar(*p);
            continue;
        }
        switch(*++p){
            case 'd':
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, int);
                printf("%f", dval);
                break;
            case 's':
                for(sval = va_arg(ap, char*); *sval; sval++)
                    putchar(*sval);
                break;
            default: 
                putchar(*p);
                break;
        }
    }
}




