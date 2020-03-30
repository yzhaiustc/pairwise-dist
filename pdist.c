#include "pdist.h"
#include "mkl.h"
#include "utils.h"
#include <immintrin.h>
#include "math.h"

//#define TIME_COUNT 1

void mypdist(const float *X, const float *Y, float *D, int m, int n, int k)
{
#ifdef TIME_COUNT
    double t0, t1;
    double t_total = 0., t_gemm = 0.;
    t0 = get_sec();
#endif
    int i, mn = m * n, min_mn = m < n ? m : n;
    float *Vx = (float*)malloc(sizeof(float) * m);
    float *Vy = (float*)malloc(sizeof(float) * n);
    float *ones = (float*)malloc(sizeof(float) * n);
    float *ptr_x = X, *ptr_y = Y, *ptr_d = D;
    float tmp_x, tmp_y;

    for (i = 0; i < m; i++)
    {
        tmp_x = cblas_snrm2(k, ptr_x, 1);
        ptr_x += k;
        Vx[i] = tmp_x * tmp_x;
    }

    for (i = 0; i < n; i++)
    {
        tmp_y = cblas_snrm2(k, ptr_y, 1);
        ptr_y += k;
        Vy[i] = tmp_y * tmp_y;
        ones[i] = 1;
    }
#ifdef TIME_COUNT
    t1 = get_sec();
    t_total += (t1 - t0);
    t0 = t1;
#endif
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, k, -2., X, k, Y, k, 0., D, n);
#ifdef TIME_COUNT
    t1 = get_sec();
    t_total += (t1 - t0);
    t_gemm += (t1 - t0);
    t0 = t1;
#endif
    for (i = 0; i < m; i++)
    {
        tmp_x = Vx[i];
        cblas_saxpy(n, 1., Vy, 1, ptr_d, 1);
        cblas_saxpy(n, tmp_x, ones, 1, ptr_d, 1);
        ptr_d += n;
    }

    for (i = 0; i < mn; i += 8)
    {
        _mm256_storeu_ps(D+i, _mm256_sqrt_ps(_mm256_loadu_ps(D+i)));
    }

    if (i != mn)
    {
        for (; i < mn; i++)
        {
            D[i] = sqrtf(D[i]);
        }
    }

    for (i = 0; i < min_mn; i++)
    {
        D(i, i) = 0;
    }

    free(Vx); Vx = NULL;
    free(Vy); Vy = NULL;
    free(ones); ones = NULL;
#ifdef TIME_COUNT
    t1 = get_sec();
    t_total += (t1 - t0);

    printf("t-total : %f, t-gemm: %f, ratio: %f %%\n", t_total, t_gemm, t_gemm / t_total * 100);
    printf("potential improvement ratio by fusing: %f %%\n", 100. - t_gemm / t_total * 100);
#endif
    return;
}

void mycopy_opt(int m, int M, int K, int *idx, float *src, float *dst)
{
    int i,k;
    int K32 = K & -32, K8 = K & -8;
    float *dst_ptr = dst, *src_ptr;
    for (k = 0; k < m; k++)
    {
        src_ptr = src + idx[k] * K;
        for (i = 0; i < K32; i+=32) 
        {
            _mm_prefetch(dst_ptr+32, _MM_HINT_T0);
            _mm256_storeu_ps(dst_ptr+i, _mm256_loadu_ps(src_ptr+i));
            //_mm_prefetch(dst_ptr+40, _MM_HINT_T0);
            _mm256_storeu_ps(dst_ptr+i+8, _mm256_loadu_ps(src_ptr+i+8));
            //_mm_prefetch(dst_ptr+48, _MM_HINT_T0);
            _mm256_storeu_ps(dst_ptr+i+16, _mm256_loadu_ps(src_ptr+i+16));
            //_mm_prefetch(dst_ptr+56, _MM_HINT_T0);
            _mm256_storeu_ps(dst_ptr+i+24, _mm256_loadu_ps(src_ptr+i+24));
        }
        for (i = K32; i < K8; i++) _mm256_storeu_ps(dst_ptr+i, _mm256_loadu_ps(src_ptr+i));
        for (i = K8; i < K; i++) *(dst_ptr+i)=*(src_ptr+i);
        dst_ptr+=K;
    }
}

void mycopy_std(int m, int M, int K, int *idx, float *src, float *dst)
{
    int i,k,dst_cnt=0;
    int row_idx;
    for (k = 0; k < m; k++)
    {
        row_idx = idx[k];
        for (i = 0; i < K; i++) dst[dst_cnt*K+i]=src[row_idx*K+i];
        dst_cnt++;
    }
}