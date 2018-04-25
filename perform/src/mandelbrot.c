#include <complex.h>

void mandelbrot(unsigned int hei, unsigned int wid, unsigned int maxiter, unsigned int *out){
    unsigned int i, x, y;
    double complex z, c;

    for (i=0;i<(hei*wid);i++){
        x = i % wid;
        y = i / wid;

        z = 0.;
        c =   ((double)x)/((double)hei-1.)*2.0 - 1.5
          + ( ((double)y)/((double)wid-1.)*2.0 - 1.0 )*I;
        out[i] = 0;

        while ((cabs(z = z*z + c) < 2.) && (out[i] < maxiter)) {
            out[i]++;
        }
    }
}

