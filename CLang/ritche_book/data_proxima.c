#include <stdio.h>
#include <limits.h>

typedef struct year Year;
int new_date(int year, int mounth, int day);

typedef struct {
} Mounth;

struct year {
    int year;
    Year *next_year;
    Year *previous_year;
};

typedef struct {

} Calendar;


int main(int argc, char *argv[]){
    printf("%d\n",  -1 >> 31);
}
