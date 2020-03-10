#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mkl.h"

#include "utils.h"
#include "pdist.h"

int main(int argc, char* argv[])
{
    int m = 4000;
    int n = 2000;
    int k = 10;
    if (argc != 4) 
    {
        printf("./mypdist [m] [n] [k]\n");
        printf("Wrong input, exit.\n");
        exit(-1);
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);

    if (m == 0 || n == 0 || k == 0) {
        printf("matrix dims cannot be ZERO, return.\n");
        exit(-2);
    }

    printf("X: %d - by - %d\nY: %d - by - %d\nDist: %d - by - %d\n", m, k, n, k, m, n);

    float *X, *Y, *D;
    double t0, t1;

    X = (float *)malloc(sizeof(float) * m * k);
    Y = (float *)malloc(sizeof(float) * n * k);
    D = (float *)malloc(sizeof(float) * m * n);

    randomize_matrix(X, m, k);
    randomize_matrix(Y, n, k);


    printf("\n################# My PDIST #################\n");
    fflush(stdout);

    t0 = get_sec();
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1., X, k, Y, n, 1., D, n);
    t1 = get_sec();
    printf("CBLAS : Elapsed time: %8.6fs, Perf: %8.6f \n", t1-t0, \
        2 * (m / 1000.) * (n / 1000.) * (k / 1000.) / (t1 - t0));    
    fflush(stdout);
    

    free(X); X = NULL;
    free(Y); Y = NULL;
    free(D); D = NULL;

    return 0;
}