#include <stdio.h>
#include <math.h>

int main(void)
{
    double a = 100000.99999999999; /* = b + c - 1e-11 */
    double b = 100000.0;
    double c = 1.0;
    double s, naive, kahan;

    s = (a + b + c) / 2;
    naive = sqrt(s * (s - a) * (s - b) * (s - c));

    /* Kahan 稳定形式：把"减法"移到参数里，避免 (s-a) 这种相近数相减 */
    kahan = 0.25 * sqrt((a + (b + c)) * (c - (a - b)) *
                        (c + (a - b)) * (a + (b - c)));

    printf("s-a   = %.20g   (真实值 5e-12)\n", s - a);
    printf("naive = %.20g\n", naive);
    printf("kahan = %.20g\n", kahan);
    return 0;
}
