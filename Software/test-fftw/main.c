#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fftw3.h>

#define N 1024
#define T 1

#define REAL 0
#define IMAG 1

double superpozicija(double t)
{
    return sin(2 * M_PI * 1 * t) + sin(2 * M_PI * 3 * t) + sin(2 * M_PI * 7 * t);
}

void generate_signal(fftw_complex *in)
{
    double dt = T / (double) N;

    for (int i = 0; i < N; i++) {
        double t = i * dt;
        in[i][REAL] = sin(2 * M_PI * 1 * t) + sin(2 * M_PI * 3 * t) + sin(2 * M_PI * 7 * t);
        in[i][IMAG] = 0.0; /*sin(2 * M_PI * 1 * t) + sin(2 * M_PI * 3 * t) + sin(2 * M_PI * 7 * t);*/
    }
}

void printMagnitudesInRange(fftw_complex *out, int n, double Fs, double minFreq, double maxFreq)
{
    int k;
    double freq;
    double magnitude;
    double deltaF = Fs / n;

    for (k = 0; k <= N / 2; ++k) { // We only need the first N/2 + 1 bins
        freq = k * deltaF;
        if (freq >= minFreq && freq <= maxFreq) {
            magnitude = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]);
            printf("Frequency: %f Hz, Magnitude: %f\n", freq, magnitude);
        }
    }
}

int main()
{
    int opcija = -1;

    // clock_t start, end;
    // double cpu_time;

    struct timespec start, end;

    long long elapsed_ns;

    bool exit = false;

    while (!exit) {
        printf("---IZBORNIK---\n"
               "1. R2R - 1D\n"
               "2. DFT - 1D\n"
               "0. Izlaz\n");

        scanf_s("%d", &opcija);

        switch (opcija) {
        case 0:
            exit = true;
            break;
        case 1: //R2R
        {
            double *in = (double *) malloc(sizeof(double) * N);
            double *out = (double *) malloc(sizeof(double) * N);

            fftw_plan p = fftw_plan_r2r_1d(N, in, out, FFTW_R2HC, FFTW_ESTIMATE);

            double dt = T / (double) N;

            for (int i = 0; i < N; i++) {
                double t = i * dt;
                in[i] = superpozicija(t);
            }

            //start = clock();

            clock_gettime(CLOCK_MONOTONIC, &start);

            fftw_execute(p);

            //end = clock();

            clock_gettime(CLOCK_MONOTONIC, &end);

            //cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

            elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            //printf("CPU vrijeme (ms): %f\n", cpu_time);

            printf("CPU vrijeme (ns): %lld\n", elapsed_ns);

            printf("Rezultati (zadnjih 10):\n");

            for (int i = N - 10; i < N; i++) {
                printf("%f\n", out[i]);
            }

            printf("Frekvencijski spektar: \n");

            printMagnitudesInRange((fftw_complex *) out, N, 44100.0, 20.0, 20000.0);

            fftw_destroy_plan(p);

            break;
        }
        case 2: //DFT
        {
            fftw_complex in[N];
            fftw_complex out[N];

            fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

            generate_signal(in);

            clock_gettime(CLOCK_MONOTONIC, &start);

            fftw_execute(p);

            //end = clock();

            clock_gettime(CLOCK_MONOTONIC, &end);

            //cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

            elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            //printf("CPU vrijeme (ms): %f\n", cpu_time);

            printf("CPU vrijeme (ns): %lld\n", elapsed_ns);

            printf("Rezultati (zadnjih 10):\n");

            for (int i = N - 10; i < N; i++) {
                printf("%f, %f\n", out[i][REAL], out[i][IMAG]);
            }

            printf("Frekvencijski spektar: \n");

            printMagnitudesInRange(out, N, 44100.0, 20.0, 20000.0);

            fftw_destroy_plan(p);

            break;
        }
        default:
            printf("Nepoznata opcija!\n");
            break;
        }
    }

    system("pause");

    return 0;
}
