#include <stdio.h>
#include <math.h>

int main(void)
{
    double a, b, c, area;

    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        fprintf(stderr, "输入错误：请提供三个数字\n");
        return 1;
    }

    if (a <= 0 || b <= 0 || c <= 0) {
        fprintf(stderr, "输入错误：边长必须为正数\n");
        return 1;
    }

    /* 三角形不等式：两边之和必须严格大于第三边 */
    if (a + b <= c || a + c <= b || b + c <= a) {
        fprintf(stderr, "输入错误：%.6g %.6g %.6g 构不成三角形\n", a, b, c);
        return 1;
    }

    /* Kahan 稳定形式，避免 (s-a) 这类相近数相减造成的精度损失 */
    area = 0.25 * sqrt((a + (b + c)) * (c - (a - b)) *
                       (c + (a - b)) * (a + (b - c)));

    printf("%.6f\n", area);
    return 0;
}
