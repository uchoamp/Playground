#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAXTOKEN 100

enum {NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl_();

int getch(void);
void ungetch(int);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int main()/* convert declaration to words */
{
    while(gettoken() != EOF){ /* 1st token on line */
        strcpy(datatype, token);
        out[0] = '\n';
        dcl(); /* parse rest of line */
        if(tokentype != '\n')
            printf("syntax error\n");
        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}

int gettoken(void)
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while((c = getch()) == ' ' || c == '\t')
        ;

    if(c == '('){
        if((c = getch()) == ')'){
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c == '['){
        for (*p++ = c; (*p++ = getch()) != ']';)
            ;
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch());)
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else 
        return tokentype = c;
}


/* dcl : parse a declarato */
void dcl(void)
{
    int ns;

    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;
    dirdcl_();
    while(ns-- > 0)
        strcat(out, " pointer to");
}


/* dirdcl: parse a driect declarator */
void dirdcl_(void)
{
   int type;

   if(tokentype == '('){           /* ( dcl ) */
       dcl();
       if(tokentype != ')') 
           printf("error: missing )\n");
   }else if (tokentype == NAME)  /* variable name */
           strcpy(name, token);
    else
        printf("error: expeted name or (dcl)\n");
    while((type=gettoken()) == PARENS || type == BRACKETS)
        if(type == PARENS)
            strcat(out, " function returning");
        else{
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    

}


#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;

int getch(void) /* get a (possibly pushed bakc) character */
{
     return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
     if (bufp >= BUFSIZE)
          printf("ungech: too many chracters\n");
     else
          buf[bufp++] = c;
}
