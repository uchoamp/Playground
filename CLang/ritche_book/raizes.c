#include <stdio.h>
#include <math.h>

int raizes(double a, double b, double c, double *px1, double *px2);


int main(int argc, char *argv[]){
    double px1, px2;
    //raizes(2.0, 7.0, 5.0, &px1, &px2);
    raizes(4.0, -4.0, 1.0, &px1, &px2);
    printf("x1: %.2f\n", px1);
    printf("x2: %.2f\n", px2);
}

int raizes(double a, double b, double c, double *px1, double *px2){

    double delta;
    
    if((delta = (pow(b, 2)) - (4 * a * c)) >= 0){
        *px1 = (-b + sqrt(delta))/(2*a);
        *px2 = (-b - sqrt(delta))/(2*a);
        return 1;
    } else 
        return 0;
}
