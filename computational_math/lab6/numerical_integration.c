#include <stdio.h>
#include <math.h>
#include <omp.h>

double func(double x)
{
    return 2 * pow(x, 3) + 9 * pow(x, 2) + 11;
}

int main()
{
    const double a = -2, b = 4, eps = 1E-10;
    double sq[2];
    int n0 = 10000000;
    #pragma omp parallel num_threads(1)
    {
        double delta = 1;
        int n = n0, k = 0;
        for (k = 0; delta > eps; n *= 2, k ^= 1) {
            double h = (b - a) / n, s = 0.0;
            sq[k] = 0;
            #pragma omp barrier
            #pragma omp for nowait
            for (int i = 0; i < n; i++)
                s += func(a + h * (i + 0.5));
            #pragma omp atomic
            sq[k] += s * h;
            #pragma omp barrier
            if (n > n0)
                delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }

    }
    printf("Rezult = %lf\n", (sq[0]) ? sq[0] : sq[1]);

    return 0;
}