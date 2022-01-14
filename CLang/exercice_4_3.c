#include <stdio.h>
#include <stdlib.h> /*for atof()*/
#include <math.h>

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that number was found */

int getop(char[]);
void push(double);
double pop(void);

int main()
{
     int type;
     double op2, aux;
     char s[MAXOP];

     while ((type = getop(s)) != EOF)
     {

          switch (type)
          {
          case NUMBER:
               push(atof(s));
               break;
          case '+':
               push(pop() + pop());
               break;
          case '*':
               push(pop() * pop());
               break;
          case '-':
               op2 = pop();
               push(pop() - op2);
               break;
          case '/':
               op2 = pop();
               if (op2 != 0.0)
                    push(pop() / op2);
               else
                    printf("error: zero divisor\n");
               break;
          case '%':
               op2 = pop();
               if (op2 != 0.0)
                    push(fmod(pop(), op2));
               else
                    printf("error: zero divisor\n");

               break;
          case '\n':
               break;
          case 'r':
               printf("\t%.8g\n", pop());
               break;
          case 'p':
               op2 = pop();
               if (op2)
               {
                    push(op2);
                    printf("top element: %.8g\n", op2);
               }
               break;
          case 'd':
               op2 = pop();
               if (op2){
                    push(op2);
                    push(op2);
                    printf("%.8g was duplicated\n", op2);
               }
               break;
          case 's':
               op2 = pop();
               if(op2){
                    aux = pop();
                    if(aux){
                         push(op2);
                         push(aux);
                         printf("%.8g and %.8g swaped\n", aux, op2);
                    }
                    else
                         push(op2);
               }
               break;
               
          default:
               printf("error: unknow command %s\n", s);
          }
     }
     return 0;
}

#define MAXVAl 100  /* maximum depth of val stack */
int sp = 0;         /* next free stack position */
double val[MAXVAl]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
     if (sp < MAXVAl)
          val[sp++] = f;
     else
          printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
     if (sp > 0)
          return val[--sp];
     else
     {
          printf("error: stack empty\n");
          return 0.0;
     }
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
     int i, c;

     while ((s[0] = c = getch()) == ' ' || c == '\t')
          ;
     s[1] = '\0';

     if (!isdigit(c) && c != '.' && c != '-')
          return c; /* not a number */

     i = 0;
     if (isdigit(c) || c == '-') /* collect integer part with -*/
          while (isdigit(s[++i] = c = getch()))
               ;

     if (c == '.') /* collect fraction part */
          while (isdigit(s[++i] = c = getch()))
               ;

     s[i] = '\0';

     if (c != EOF)
          ungetch(c);
     if (s[0] == '-' && !s[1])
          return s[0];

     return NUMBER;
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
