/*
 * ============================================================================
 *  练习 5 参考答案：数学计算器（几何 + 统计）
 * ============================================================================
 *
 *  ▶ 这个练习要练什么
 *  ---------------------------------------------------------------------------
 *    一个菜单式的数学计算器：平方根和乘方、勾股定理、海伦公式、
 *    平均数和标准差、角度弧度互转。对应第 13 课（math.h），
 *    另外用到了第 14 课（hypot）、第 16 课（标准差）、
 *    以及第 5 课（switch）、第 6 课（循环）、第 7 课（函数）、第 8 课（数组）。
 *
 *    这道题是前面所有东西的合流：
 *    菜单循环（练习 4）+ 数组（第 8 课）+ 数学函数（第 13 课）。
 *
 *  ▶ 前置知识（开始前先自测）
 *  ---------------------------------------------------------------------------
 *    ✅ 已经会：第 13 课：数学函数库 math.h（sqrt、pow、fabs、isnan、M_PI）
 *    ✅ 已经会：第 14 课：三角函数（hypot 勾股定理）
 *    ✅ 已经会：第 16 课：指数、对数与统计计算（平均数、方差、标准差）
 *    ✅ 已经会：第 5 课：多分支选择 switch
 *    ✅ 已经会：第 6 课：循环（while）、第 7 课：函数、第 8 课：数组
 *    🔁 忘了就看：第 13 课第 3 节「sqrt：平方根」、第 13 课第 5 节「M_PI（圆周率常量）」
 *    🔁 忘了就看：第 13 课第 7 节「isnan() / isinf() / NAN」
 *    🔁 忘了就看：第 14 课第 5 节「hypot(a, b)」
 *    🔁 忘了就看：第 16 课第 4 节「实战：统计计算」
 *    ❓ 自测（答案在文件末尾）
 *       1) 用 math.h 里的函数，为什么编译的时候要加 -lm？
 *       2) sqrt(-4) 的结果是什么？为什么不能直接把它打印出来？
 *       3) 海伦公式里那个 s 是什么？
 *
 *  ▶ 题目要求（做完练习再看）
 *  ---------------------------------------------------------------------------
 *    一直显示这个菜单，直到用户选 0 才结束：
 *        1. 平方根和乘方
 *        2. 勾股定理（求斜边）
 *        3. 三角形面积（海伦公式）
 *        4. 一组数据的统计（平均数 / 标准差）
 *        5. 角度与弧度互转
 *        0. 退出
 *    所有结果统一保留两位小数输出；开方负数、三边不成三角形、
 *    统计少于两个数，都要有提示。
 *
 *    验收样例（这是真的跑出来的输出，可以直接和你的程序对照）：
 *      输入 1  然后回车
 *      输入 9  然后回车
 *      输入 2  然后回车
 *      输入 0  然后回车
 *
 *      --------- 数学计算器 ---------
 *        1. 平方根和乘方
 *        2. 勾股定理（求斜边）
 *        3. 三角形面积（海伦公式）
 *        4. 一组数据的统计（平均数 / 标准差）
 *        5. 角度与弧度互转
 *        0. 退出
 *      -----------------------------
 *      请选择：
 *      请输入一个数：
 *      请输入指数：
 *      sqrt(9.00) = 3.00
 *      pow(9.00, 2.00) = 81.00
 *      --------------------------
 *
 *      --------- 数学计算器 ---------
 *        1. 平方根和乘方
 *        2. 勾股定理（求斜边）
 *        3. 三角形面积（海伦公式）
 *        4. 一组数据的统计（平均数 / 标准差）
 *        5. 角度与弧度互转
 *        0. 退出
 *      -----------------------------
 *      请选择：
 *
 *      再见！这次一共做了 1 次计算。
 *
 *  ▶ 需要用到的语法（速查）
 *  ---------------------------------------------------------------------------
 *    1) math.h 里的函数
 *       写法：sqrt(x)、pow(x, y)、hypot(a, b)、fabs(x)
 *       作用：平方根、乘方、勾股定理的斜边、绝对值。
 *       坑：★ 它们的参数和返回值全是 double，编译时要加 -lm。
 *
 *    2) isnan
 *       写法：if (isnan(p)) { ... }
 *       作用：判断一个浮点数是不是「算不出来」（NaN）。
 *             pow(-2, 0.5) 就会算出 NaN。
 *       坑：NaN 自己跟谁比都不相等（连 NaN == NaN 都是假），
 *            所以只能用 isnan 来判断，不能写 p != p。
 *
 *    3) M_PI（圆周率）
 *       写法：#ifndef M_PI / #define M_PI 3.14159... / #endif
 *       作用：M_PI 不是标准 C，各平台不一定给，自己定义最保险。
 *       坑：这个 #define 一定要放在用到 M_PI 的代码之前。
 *
 *    4) 数组
 *       写法：double data[MAX_DATA];  和  data[i]
 *       作用：存一组数，MAX_DATA 是文件里定义好的 100。
 *       坑：下标从 0 开始，最多能用到 data[MAX_DATA - 1]。
 *
 *    5) 标准差怎么算
 *       写法：标准差 = sqrt( 每个数与平均数之差的平方的平均 )
 *       作用：衡量这组数据「散得有多开」。
 *       坑：先算平均数，再拿平均数去减每一个数，顺序不能反。
 *
 *  ▶ 解法思路（先看思路，再看代码）
 *  ---------------------------------------------------------------------------
 *    1) 先搭骨架，这和练习 4 完全一样：
 *       while (1) 里做四件事 —— 打印菜单、读选择、switch 分发、打印分隔线；
 *       选 0 时 return 结束程序，顺便把「一共做了几次计算」报出来。
 *
 *    2) 每个 case 的结构也固定，就三步：
 *       读数据 → 检查合法性 → 算并打印。
 *       读数据这一步每个分支长得一模一样，
 *       所以把它们抽成 read_int / read_number 两个函数，
 *       main 里就只剩下「printf 提示 + 调函数 + 取 last_number」。
 *
 *    3) 检查分三类，想清楚了就不容易漏：
 *       数学上的限制：开方要拦负数（x < 0），
 *                    三角形要判断三边（a+b>c 且 a+c>b 且 b+c>a），
 *                    统计至少要两个数（n < 2）。
 *       输入的限制：读不到数（read_number 返回值不是 0）就跳过这次计算。
 *       结果的限制：pow 可能算出 NaN，要用 isnan 判断后再打印。
 *
 *    4) 第 3 项的海伦公式：
 *       先算半周长 s = (a + b + c) / 2，
 *       再算面积 = sqrt(s * (s - a) * (s - b) * (s - c))。
 *       注意要先判断三边能不能构成三角形，否则 s - a 可能是负数，
 *       sqrt 一个负数会得到 NaN。
 *
 *    5) 第 4 项统计：
 *       先读个数 n，检查 2 <= n <= MAX_DATA；
 *       再用一个 double 数组 data[MAX_DATA] 把这些数读进来；
 *       然后 average(data, n) 求平均数，
 *       standard_deviation(data, n, mean) 求标准差。
 *       注意：读数据的过程中途失败要跳出，不能拿半截数据去算。
 *
 *    6) 第 5 项角度弧度互转：
 *       角度转弧度 = 角度 × π ÷ 180，
 *       弧度转角度 = 弧度 × 180 ÷ π。
 *       这两个换算已经写成了 deg_to_rad / rad_to_deg 两个函数，直接调用。
 *
 *    提醒：这题最容易被写错的地方有三个。
 *          一是忘了加 -lm，链接阶段直接失败；
 *          二是没做定义域检查，sqrt(-4) 打出 nan；
 *          三是第 4 项读数据失败后还继续算，平均数和标准差全是错的。
 *
 *  ▶ 代码怎么读
 *  ---------------------------------------------------------------------------
 *    下面每段代码前面都有「▶▶▶ 第 N 段」，讲清这段在干嘛、跑起来会看到什么。
 *    建议顺序：先扫一眼每段的小标题 → 读代码 → 在脑子里预演输出 →
 *      最后把文件关掉，凭记忆重新写一遍，这样才真的学会。
 *
 *    第 1 段：常量（M_PI 的保险写法、MAX_DATA）。
 *    第 2 段：函数声明（原型）。
 *    第 3 段：全局变量 —— 最近一次读到的数。
 *    第 4 段：main 里的 while (1) 循环。
 *    第 5 段：读用户的选择。
 *    第 6 段：switch 的第 1 个分支 —— 平方根和乘方。
 *    第 7 段：switch 的第 2 个分支 —— 勾股定理。
 *    第 8 段：switch 的第 3 个分支 —— 三角形面积。
 *    第 9 段：switch 的第 4 个分支 —— 平均数 / 标准差。
 *    第 10 段：switch 的第 5 个分支 —— 角度弧度互转。
 *    第 11 段：case 0（退出）和 default（没有这个选项）。
 *    第 12 段：每轮结束打印分隔线。
 *    第 13 段往后：各个辅助函数。
 *
 *  运行方式（在本项目根目录下）：
 *    make run L=exercises/solutions/ex05_math
 *      （Makefile 已经自动带上 -lm，直接 make 就行。）
 *    手动编译一定要加 -lm，否则链接阶段会报错：
 *      cc -Wall -Wextra -Wpedantic -std=c11 exercises/solutions/ex05_math.c -o math -lm
 *
 *    参考答案在 exercises/solutions/ex05_math.c
 * ============================================================================
 */

/* ▶▶▶ 第 1 段：头文件与常量定义
 *  做什么：把要用到的库（stdio 负责输入输出、math 负责数学函数）请进来，
 *         再定义两个后面要反复用的常量。
 *  新语法：#include、#define、#ifndef / #endif
 *  预期输出：本段不产生输出，只是打好地基。
 */
#include <stdio.h>
#include <math.h>     /* sqrt pow hypot isnan */

/* M_PI 不是标准 C，各平台不一定给，自己定义最保险 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_DATA 100      /* 一次最多统计 100 个数 */

/* ▶▶▶ 第 2 段：函数声明（原型）
 *  做什么：告诉编译器「下面这些函数存在，叫什么名字、收什么参数、返回什么」。
 *          因为它们的定义写在 main 后面，所以必须先声明。
 */
void print_menu(void);
int  read_int(void);
int  read_number(void);
void clear_rest_of_line(void);
double deg_to_rad(double degree);
double rad_to_deg(double radian);
double average(const double data[], int count);
double standard_deviation(const double data[], int count, double mean);
int  is_valid_triangle(double a, double b, double c);

/* ▶▶▶ 第 3 段：全局变量 —— 最近一次读到的数
 *  为什么要有它：read_int / read_number 这两个函数要同时告诉你两件事 ——
 *  「读到的数是多少」和「读成功了没有」。C 的函数只能直接返回一样东西，
 *  所以用返回值表示成功没有，用这两个全局变量装读到的数。
 *  （另一种写法是用指针参数，但那是第 10 课的内容，这里先不碰。）
 *  注意：它们必须写在 main 前面，因为 main 里也要用到它们。
 */
int    last_int    = 0;
double last_number = 0.0;

int main(void) {
    /* ▶▶▶ 第 4 段：main 里的 while (1) 循环
     *  做什么：整个菜单流程都在这个循环里，一直转，直到用户选 0。
     *          operations 记着一共做了多少次计算。
     *  预期输出：一上来先打印一份菜单。
     */
    int operations = 0;    /* 一共做了多少次计算 */

    while (1) {
        print_menu();

        /* ▶▶▶ 第 5 段：读用户的选择
         *  做什么：read_int() 把读到的数放进全局变量 last_int，
         *          它自己返回「读成功了没有」。
         *  三种情况：
         *    返回 -2 → 输入已经结束（比如按了 Ctrl+D），程序退出；
         *    返回 -1 → 用户敲的不是数字，提示后 continue，回到菜单重来；
         *    返回  0 → 读成功，把 last_int 存进 choice。
         */
        int choice = 0;
        printf("请选择：");
        int rc = read_int();
        if (rc == -2) {
            printf("\n输入已结束，程序退出。\n");
            return 0;
        }
        if (rc != 0) {
            printf("\n输入的不是数字，请重新输入。\n");
            continue;
        }
        choice = last_int;

        switch (choice) {
            /* ▶▶▶ 第 6 段：第 1 项 —— 平方根和乘方
             *  做什么：读一个数 x 和一个指数 exponent，
             *          算出 sqrt(x) 和 pow(x, exponent)。
             *  两道检查：
             *    x < 0 → 负数没有平方根，直接跳过；
             *    pow 的结果可能是 NaN，用 isnan 判断一下再决定怎么打印。
             *  预期输出（输入 9 和 2）：
             *            sqrt(9.00) = 3.00
             *            pow(9.00, 2.00) = 81.00
             */
            case 1: {
                double x = 0.0;
                double exponent = 0.0;

                printf("请输入一个数：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                x = last_number;

                printf("请输入指数：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                exponent = last_number;

                if (x < 0.0) {
                    printf("%.2f 是负数，没有平方根。\n", x);
                    break;
                }

                printf("sqrt(%.2f) = %.2f\n", x, sqrt(x));

                double p = pow(x, exponent);
                if (isnan(p)) {
                    printf("pow(%.2f, %.2f) = nan（这个组合算不出结果）\n",
                           x, exponent);
                } else {
                    printf("pow(%.2f, %.2f) = %.2f\n", x, exponent, p);
                }
                operations++;
                break;
            }

            /* ▶▶▶ 第 7 段：第 2 项 —— 勾股定理求斜边
             *  做什么：读两条直角边 a 和 b，用 hypot(a, b) 求斜边。
             *  为什么要用 hypot：它内部做了防溢出的处理，
             *  比自己写 sqrt(a*a + b*b) 更稳（第 14 课）。
             *  预期输出（输入 3 和 4）：斜边长 = 5.00
             */
            case 2: {
                double a = 0.0;
                double b = 0.0;

                printf("请输入第一条直角边：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                a = last_number;

                printf("请输入第二条直角边：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                b = last_number;

                if (a <= 0.0 || b <= 0.0) {
                    printf("直角边必须大于 0。\n");
                    break;
                }

                printf("斜边长 = %.2f\n", hypot(a, b));
                operations++;
                break;
            }

            /* ▶▶▶ 第 8 段：第 3 项 —— 三角形面积（海伦公式）
             *  做什么：读三条边，先判断能不能构成三角形；
             *          能的话算半周长 s，再套海伦公式算面积。
             *  为什么先判断：三边不成三角形时 s - a 可能是负数，
             *                sqrt 一个负数会得到 NaN。
             *  预期输出（输入 3、4、5）：
             *            周长 = 12.00
             *            面积 = 6.00
             */
            case 3: {
                double a = 0.0;
                double b = 0.0;
                double c = 0.0;

                printf("请输入第一条边：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                a = last_number;

                printf("请输入第二条边：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                b = last_number;

                printf("请输入第三条边：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                c = last_number;

                if (!is_valid_triangle(a, b, c)) {
                    printf("这三条边构不成三角形。\n");
                    break;
                }

                double s = (a + b + c) / 2.0;          /* 半周长 */
                double area = sqrt(s * (s - a) * (s - b) * (s - c));

                printf("周长 = %.2f\n", a + b + c);
                printf("面积 = %.2f\n", area);
                operations++;
                break;
            }

            /* ▶▶▶ 第 9 段：第 4 项 —— 一组数据的统计
             *  做什么：先读要统计几个数 n（2 ~ MAX_DATA），
             *          再把 n 个数读进数组 data，
             *          最后输出平均数和标准差。
             *  为什么要有 broken 这个标记：
             *          读数据用的 for 循环里如果失败了，break 只跳出 for，
             *          所以外面还要一个标记，提示「输入无效」以后不要再算，
             *          否则会拿半截数据算出错误的平均数。
             *  预期输出（3 个数 10、20、30）：
             *            平均数 = 20.00
             *            标准差 = 8.16
             *            这组数据有正常波动。
             */
            case 4: {
                int n = 0;
                printf("要统计几个数（2~100）：");
                if (read_int() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                n = last_int;

                if (n < 2) {
                    printf("至少要两个数才能算标准差。\n");
                    break;
                }
                if (n > MAX_DATA) {
                    printf("一次最多统计 %d 个数。\n", MAX_DATA);
                    break;
                }

                double data[MAX_DATA];
                int broken = 0;
                for (int i = 0; i < n; i++) {
                    printf("请输入数据：");
                    if (read_number() != 0) {
                        broken = 1;
                        break;
                    }
                    data[i] = last_number;
                }
                if (broken) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }

                double mean = average(data, n);
                double std = standard_deviation(data, n, mean);

                printf("平均数 = %.2f\n", mean);
                printf("标准差 = %.2f\n", std);
                if (std < 5.0) {
                    printf("这组数据很集中。\n");
                } else if (std < 15.0) {
                    printf("这组数据有正常波动。\n");
                } else {
                    printf("这组数据差距很大。\n");
                }
                operations++;
                break;
            }

            /* ▶▶▶ 第 10 段：第 5 项 —— 角度与弧度互转
             *  做什么：读一个数值和一个方向，
             *          方向 1 表示角度转弧度，方向 2 表示弧度转角度。
             *  预期输出（输入 180，方向 1）：
             *            180.00 度 = 3.141593 弧度
             */
            case 5: {
                double value = 0.0;
                int direction = 0;

                printf("请输入数值：");
                if (read_number() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                value = last_number;

                printf("1 = 角度转弧度，2 = 弧度转角度：");
                if (read_int() != 0) {
                    printf("输入无效，这次计算跳过。\n");
                    break;
                }
                direction = last_int;

                if (direction == 1) {
                    printf("%.2f 度 = %.6f 弧度\n", value, deg_to_rad(value));
                } else if (direction == 2) {
                    printf("%.6f 弧度 = %.2f 度\n", value, rad_to_deg(value));
                } else {
                    printf("没有这个方向，只能选 1 或 2。\n");
                }
                operations++;
                break;
            }

            /* ▶▶▶ 第 11 段：case 0（退出）和 default（没有这个选项）
             *  做什么：选 0 就打印一共做了多少次计算，然后 return 结束程序；
             *          选了 1~5 以外的东西就提示一下，回到菜单重来。
             */
            case 0:
                printf("\n再见！这次一共做了 %d 次计算。\n", operations);
                return 0;      /* 直接退出程序，等价于 break 跳出循环 */
            default:
                printf("没有这个选项，请输入 0-5。\n");
                break;
        }

        /* ▶▶▶ 第 12 段：每轮结束打印一行分隔线
         *  做什么：让屏幕上的内容分成一段一段的，好看也好读。
         */
        printf("--------------------------\n");
    }
}

/* ▶▶▶ 第 13 段：打印菜单 */
void print_menu(void) {
    printf("\n--------- 数学计算器 ---------\n");
    printf("  1. 平方根和乘方\n");
    printf("  2. 勾股定理（求斜边）\n");
    printf("  3. 三角形面积（海伦公式）\n");
    printf("  4. 一组数据的统计（平均数 / 标准差）\n");
    printf("  5. 角度与弧度互转\n");
    printf("  0. 退出\n");
    printf("-----------------------------\n");
}

/* ▶▶▶ 第 14 段：读一个整数
 *  做什么：scanf("%d", &last_int) 去读，读到的数放在全局变量 last_int 里。
 *          函数自己的返回值告诉你「读成功了没有」：
 *            返回  0 -> 读成功
 *            返回 -1 -> 输入的不是数字
 *            返回 -2 -> 输入已经结束（Ctrl+D 或输入重定向用完了）
 *  为什么要调 clear_rest_of_line()：读失败的时候（比如用户敲了 abc），
 *          那三个字符还留在输入缓冲区里，不清掉的话下一次 scanf
 *          又会读到它们，程序就会一直说「输入无效」。
 */
int read_int(void) {
    int rc = scanf("%d", &last_int);

    if (rc == EOF) {
        printf("\n");
        return -2;
    }
    clear_rest_of_line();
    if (rc != 1) {
        return -1;
    }
    printf("\n");     /* 读完了，换一行，免得下面的提示挤在一起 */
    return 0;
}

/* ▶▶▶ 第 15 段：读一个小数
 *  做法和 read_int 一样，只是格式符从 %d 换成 %lf，
 *  读到的数放在 last_number 里。
 */
int read_number(void) {
    int rc = scanf("%lf", &last_number);

    if (rc == EOF) {
        printf("\n");
        return -2;
    }
    clear_rest_of_line();
    if (rc != 1) {
        return -1;
    }
    printf("\n");
    return 0;
}

/* ▶▶▶ 第 16 段：把输入行里剩下的字符全部读掉
 *  做什么：getchar() 一次读一个字符，直到读到换行符或者输入结束。
 *  为什么需要它：见上面 read_int 里的说明。
 */
void clear_rest_of_line(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* ▶▶▶ 第 17 段：角度转弧度
 *  公式：弧度 = 角度 × π ÷ 180
 */
double deg_to_rad(double degree) {
    return degree * M_PI / 180.0;
}

/* ▶▶▶ 第 18 段：弧度转角度
 *  公式：角度 = 弧度 × 180 ÷ π
 */
double rad_to_deg(double radian) {
    return radian * 180.0 / M_PI;
}

/* ▶▶▶ 第 19 段：平均数
 *  做法：把所有数加起来，再除以个数。
 */
double average(const double data[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / count;
}

/* ▶▶▶ 第 20 段：标准差 = 方差开根号，方差 = 偏差平方的平均
 *  做法：每个数先减去平均数，平方，累加，最后除以个数再开根号。
 *  关键点：必须先有平均数（mean），所以调用它的时候平均数要已经算好了。
 */
double standard_deviation(const double data[], int count, double mean) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = data[i] - mean;
        sum += diff * diff;
    }
    return sqrt(sum / count);
}

/* ▶▶▶ 第 21 段：能不能构成三角形
 *  条件：三条边都大于 0，而且任意两边之和都大于第三边。
 */
int is_valid_triangle(double a, double b, double c) {
    if (a <= 0.0 || b <= 0.0 || c <= 0.0) {
        return 0;
    }
    if (a + b <= c || a + c <= b || b + c <= a) {
        return 0;
    }
    return 1;
}

/* ============================================================================
 *  ▶ 常见报错速查
 *  ---------------------------------------------------------------------------
 *  undefined reference to `sqrt'
 *      → 忘了加 -lm。手动编译时写：
 *         cc -Wall -Wextra -Wpedantic -std=c11 exercises/solutions/ex05_math.c -o math -lm
 *
 *  error: implicit declaration of function 'isnan'
 *      → 忘了写 #include <math.h>。
 *
 *  'M_PI' undeclared
 *      → #define M_PI 那一段被删掉了，或者写在使用它的代码之后。
 *
 *  sqrt(负数) 打出来是 nan
 *      → 数学上就是没有平方根，要先用 if (x < 0.0) 拦住。
 *
 *  标准差算出来是 0.00
 *      → 算标准差前要先有平均数，而且要先减去平均数再平方。
 *
 *  统计那一项算出来的平均数不对
 *      → 读数据的过程中途失败了，却还拿着半截数据继续算。
 *        失败时要跳出这次计算。
 *
 *  ▶ 自测答案
 *  ---------------------------------------------------------------------------
 *  1) 因为 math.h 里的函数装在一个单独的数学库（libm）里，
 *     链接的时候要告诉编译器把它一起链进来，就是加 -lm。
 *     macOS 上不加也能跑，但 Linux 上一定要加。
 *
 *  2) NaN（不是数）。sqrt(-4) 在数学上没有意义，
 *     C 会算出一个特殊值 NaN，而且 NaN 自己跟谁比都不相等，
 *     所以只能用 isnan() 来判断。
 *
 *  3) s 是半周长，也就是周长的一半：s = (a + b + c) / 2。
 *     海伦公式是：面积 = sqrt(s * (s - a) * (s - b) * (s - c))。
 *
 *  ▶ 小结（能用自己的话说出来就算真的会了）
 *  ---------------------------------------------------------------------------
 *  · 「菜单 + 功能 + 校验」是工具型程序的通用骨架：
 *    while 管循环，switch 管分发，每个功能自己管输入检查。
 *  · 用数学函数前必须做「定义域检查」：开方要拦负数，
 *    求面积要判断能不能构成三角形，统计要够两个数。
 *  · 算出来的结果如果可能是 NaN，要用 isnan() 判断，不能直接拿去比较。
 *  · 用到 math.h 就一定要加 -lm，这是新手最常忘的一步。
 * ============================================================================ */
