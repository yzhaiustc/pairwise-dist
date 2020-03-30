#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mkl.h"

#include "utils.h"
#include "pdist.h"

int main(int argc, char* argv[])
{
    int m = 4000;
    int M = 2000;
    int K = 10;
    if (argc != 4) 
    {
        printf("./mypdist [m] [M] [K]\n");
        printf("Wrong input, exit.\n");
        exit(-1);
    }

    m = atoi(argv[1]);
    M = atoi(argv[2]);
    K = atoi(argv[3]);

    if (m == 0 || M == 0 || K == 0) {
        printf("matrix dims cannot be ZERO, return.\n");
        exit(-2);
    }

    printf("SRC: %d - by - %d\nDST: %d - by - %d\nIDX: %d - by - 1\n", M, K, m, K, m);

    float *src, *dst, *dst_cmp;
    float *flush_cache;
    int *idx;
    double t0, t1;

    src = (float *)malloc(sizeof(float) * M * K);
    dst = (float *)malloc(sizeof(float) * m * K);
    dst_cmp = (float *)malloc(sizeof(float) * m * K);
    flush_cache = (float *)malloc(sizeof(float) * 8192 * 8192);
    idx = (int *)malloc(sizeof(int) * m * 1);
    int i;
    for (i = 0; i < m; i++) idx[i] =  i*M/m;

    randomize_matrix(src, M, K);

    printf("\n################# FLUSHING CACHE #################\n");
    randomize_matrix(flush_cache, 8192, 8192);
    fflush(stdout);

    printf("\n################# Copy Standard #################\n");
    fflush(stdout);
    t0 = get_sec();
    mycopy_std(m, M, K, idx, src, dst_cmp);
    //mypdist(X, Y, D, m, n, k);
    t1 = get_sec();
    printf("Naive copy : Elapsed time: %8.6fs, Copied %8.6fG floats per second.\n", (t1-t0), \
        (m / 1000.) * (K / 1000.) * (1/1000.) / (t1 - t0));     
    fflush(stdout);

    printf("\n################# FLUSHING CACHE #################\n");
    randomize_matrix(flush_cache, 8192, 8192);
    fflush(stdout);

    printf("\n################# Copy Optimized #################\n");
    fflush(stdout);
    t0 = get_sec();
    mycopy_opt(m, M, K, idx, src, dst);
    //mypdist(X, Y, D, m, n, k);
    t1 = get_sec();
    printf("BLAS copy : Elapsed time: %8.6fs, Copied %8.6fG floats per second.\n", (t1-t0), \
        (m / 1000.) * (K / 1000.) * (1/1000.) / (t1 - t0));    
    fflush(stdout);

    verify_matrix(dst, dst_cmp, m * K);
    if (0)
    {
        printf("\n################# SRC #################\n");
        print_matrix(src, M, K);
        printf("\n################# DST #################\n");
        print_matrix(dst, m, K);
        printf("\n################# DST CMP #################\n");
        print_matrix(dst_cmp, m, K);   
        printf("\n################# IDX #################\n");
        for (i = 0; i < m; i++) printf(" %d ",idx[i]);
        printf("\n################# DONE #################\n");
    }


    free(src); src = NULL;
    free(dst); dst = NULL;
    free(idx); idx = NULL;
    free(flush_cache); flush_cache = NULL;

    return 0;
}
