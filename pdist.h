#define D(i, j) D[(i)*n + (j)]

void mypdist(const float *X, const float *Y, float *D, int m, int n, int k);
void mycopy_std(int m, int M, int K, int *idx, float *src, float *dst);
void mycopy_opt(int m, int M, int K, int *idx, float *src, float *dst);