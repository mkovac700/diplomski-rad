#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fftw3.h>

#define N 1024
#define T 1

double superpozicija(double t)
{
    return sin(2 * M_PI * 1 * t) + sin(2 * M_PI * 3 * t) + sin(2 * M_PI * 7 * t);
}

int main()
{
    clock_t start, end;
    double cpu_time;

    double *in = (double *) malloc(sizeof(double) * N);
    double *out = (double *) malloc(sizeof(double) * N);

    fftw_plan p = fftw_plan_r2r_1d(N, in, out, FFTW_R2HC, FFTW_ESTIMATE);

    double dt = T / (double) N;

    for (int i = 0; i < N; i++) {
        double t = i * dt;
        in[i] = superpozicija(t);
    }

    start = clock();

    fftw_execute(p);

    end = clock();

    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

    printf("CPU vrijeme (ms): %f\n", cpu_time);

    printf("Rezultati (zadnjih 10):\n");

    for (int i = N - 10; i < N; i++) {
        printf("%f\n", out[i]);
    }

    system("pause");

    return 0;
}
