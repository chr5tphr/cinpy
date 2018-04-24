#include <stdlib.h>
#include <stdio.h>

#define VERSION "0.01"

void print_version(){
    printf("somelib ver. %s\n", VERSION);
}

double sum(unsigned int len, const double *data){
    double result = 0.;
    int i;
    for (i=0;i<len;i++){
        result += data[i];
    }
    return result;
}

void add(unsigned int len, const double *x, const double *y, double *out){
    int i;
    for (i=0;i<len;i++){
        out[i] = x[i] + y[i];
    }
}

void add_malloc(unsigned int len, const double *x, const double *y, double **out){
    *out = (double*)malloc(sizeof(double)*len);
    int i;
    for (i=0;i<len;i++){
        (*out)[i] = x[i] + y[i];
    }
}
