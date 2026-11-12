#include <stdio.h>
#include <math.h>

/* 先往栈上写一堆垃圾，再看 scanf 失败时 a,b,c 到底是什么 */
static void dirty_stack(void)
{
    volatile double junk[16];
    for (int i = 0; i < 16; i++) junk[i] = 1234.5 + i;
    (void)junk;
}

static void calc(void)
{
    double a, b, c, s, area;
    scanf("%lf %lf %lf", &a, &b, &c);
    s = (a + b + c) / 2;
    area = sqrt(s * (s - a) * (s - b) * (s - c));
    printf("a=%lf b=%lf c=%lf area=%lf\n", a, b, c, area);
}

int main(void)
{
    dirty_stack();
    calc();
    return 0;
}
