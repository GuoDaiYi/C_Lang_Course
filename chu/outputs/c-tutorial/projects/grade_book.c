/*
 * ============================================================================
 *  小项目 2：学生成绩管理器
 * ============================================================================
 *
 *  ▶ 这个项目是什么
 *  ---------------------------------------------------------------------------
 *    一个真正能用的成绩管理小工具。它把一个实用程序的完整结构走了一遍：
 *
 *        输入 → 处理 → 输出 → 存盘 → 再读回来
 *
 *    你可以添加学生、查看名单、看统计（平均分/最高分/最低分/及格率）、
 *    按成绩排序、查询某个学号，还能把数据存到文件里、下次打开还在。
 *
 *    这是本课程的**毕业设计**。能读懂它、也能改得动它，C 语言就算入门了。
 *
 *  ▶ 做这个项目之前，你需要已经会（前置知识）
 *  ---------------------------------------------------------------------------
 *    ✅ 第 3 课：会用 scanf 读输入，会检查 scanf 的返回值
 *    ✅ 第 5 课：会用 switch 做菜单式分支（这个项目的主循环就是 switch）
 *    ✅ 第 6 课：会用 for / while 循环
 *    ✅ 第 7 课：会定义函数、传参数、返回值；知道 C 的参数是「值传递」
 *    ✅ 第 8 课：会用数组，会用 sizeof 求数组长度
 *    ✅ 第 9 课：会用 char 数组存字符串，会用 strlen / strcpy / strcspn
 *    ✅ 第 10 课：会用指针（用指针把「人数」修改回调用者）
 *    ✅ 第 11 课：会用结构体、结构体数组、typedef
 *    ✅ 第 12 课：会用 fopen / fprintf / fscanf / fgets / fclose 读写文件
 *    🔁 忘了就看：第 11 课的结构体、第 12 课的文件读写
 *    ❓ 开课自测（能答上来再往下看，答案在文件最末尾）
 *       1) 结构体数组 list[i].score 里的 i 是从几开始的？
 *       2) 函数想修改调用者的 count，必须怎么传参数？
 *       3) fopen 的 "w" 和 "a" 有什么区别？
 *
 *  ▶ 本项目新出现的语法
 *  ---------------------------------------------------------------------------
 *    1) typedef struct { ... } Student;
 *       读作：把一个结构体类型取个短名字 Student
 *       作用：以后写 `Student s;` 就行，不用每次都写 `struct Student s;`。
 *       注意：这是给类型起别名，不是定义变量。
 *
 *    2) Student list[] 作为函数参数
 *       读作：一个 Student 类型的数组
 *       作用：把整份名单传给函数处理。
 *       注意：数组传进函数后，实际传的是「首元素的地址」，
 *             所以函数里能改到原数组的内容——这也是为什么排序函数不用返回值。
 *
 *    3) const Student list[]
 *       读作：常量 Student 数组
 *       作用：告诉编译器和读代码的人「这个函数只看不改这个数组」。
 *             写成 const 后，函数里如果试图改 list[i]，编译器会直接报错。
 *
 *    4) int *count 参数
 *       读作：指向整数的指针
 *       作用：函数要把「人数变了」这件事告诉调用者，
 *             就得拿到 count 的地址（因为 C 的参数是值传递）。
 *
 *    5) s->name 和 s.name 的区别
 *       读作：s 是指针，用箭头；s 是结构体变量，用点
 *       作用：访问结构体成员。
 *       注意：箭头 (->) 的 s 必须是「指针」，点的 s 必须是「结构体本身」。
 *
 *    6) %31[^\n]
 *       读作：最多读 31 个「非换行」字符
 *       作用：把一整行剩下的内容（包括空格）读进一个字符串，
 *             这样姓名里有空格也不会断。
 *       注意：31 是 NAME_LEN(32) 减 1，留一个位置给结尾的 '\0'。
 *
 *    7) 条件运算符 ?:
 *       读作：条件 ? 为真时的值 : 为假时的值
 *       作用：把 if/else 压成一行。`(list[i].score >= 60) ? "及格" : "不及格"`
 *
 *  ▶ 程序结构地图（先看懂这张地图，再看代码会轻松很多）
 *  ---------------------------------------------------------------------------
 *    数据流：
 *
 *      键盘输入
 *        │
 *        ▼
 *      ┌─────────────────────────────────────────┐
 *      │  main()  菜单循环（while + switch）        │
 *      │  持有：Student students[100]; int count;  │
 *      └───┬─────┬─────┬─────┬─────┬─────┬──────┘
 *          │     │     │     │     │     │
 *          ▼     ▼     ▼     ▼     ▼     ▼
 *        添加  显示  统计  排序  保存  读取
 *          │     │     │     │     │     │
 *          └─────┴─────┴─────┴─────┴─────┘
 *                        │
 *                        ▼
 *              gradebook.txt（存盘）
 *
 *    函数分三类：
 *      工具类：clear_input、read_int、print_menu
 *      功能类：add_student、list_students、show_stats、find_student、sort_by_score
 *      存盘类：save_to_file、load_from_file
 *
 *    注意 count 的传递方式：
 *      需要修改人数的函数 → 传 &count（add_student、load_from_file）
 *      只读不改人数的函数 → 传 count （list_students、show_stats …）
 *
 *  ▶ 怎么读这个文件
 *  ---------------------------------------------------------------------------
 *    1. 先读上面这几块，重点是「程序结构地图」；
 *    2. 阅读顺序建议：main → add_student → show_stats → sort_by_score →
 *       save_to_file → load_from_file，从上往下顺着菜单走一遍；
 *    3. 每个函数前都有「▶▶▶ 第 N 段」导读，讲清它干什么；
 *    4. 一定要亲手跑一遍：加两个人 → 看统计 → 排序 → 保存 → 读取 → 退出；
 *    5. 最后做「动手练习」。
 *
 *  运行方式（在本项目根目录下）：
 *    make run L=projects/grade_book
 *    或者手动：cc projects/grade_book.c -o grade && ./grade
 *
 *    数据会保存到当前目录的 gradebook.txt，下次运行可以读回来。
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

/* ▶▶▶ 第 1 段：用宏定义几个固定的常量
 *  做什么：把「最多多少人」「名字最长多长」「数据文件叫什么」集中管理。
 *  新语法：#define
 *  预期输出：不产生输出，只是让后面的代码好读、好改。
 */
#define MAX_STUDENTS 100     /* 最多存 100 个学生 */
#define NAME_LEN     32      /* 姓名字段占 32 字节 */
#define DATA_FILE    "gradebook.txt"   /* 数据存到哪个文件 */

/* ▶▶▶ 第 2 段：定义「一个学生」这个数据结构
 *  做什么：把姓名、学号、成绩打包成一个整体，这样就能用数组一次存很多人。
 *  新语法：typedef struct { ... } 名字;
 *  预期输出：不产生输出，只是定义了数据的形状。
 */
typedef struct {
    char name[NAME_LEN];
    int  id;
    int  score;
} Student;

/* ▶▶▶ 第 3 段：函数声明（也叫「函数原型」）
 *  做什么：提前告诉编译器「后面会有这些函数，各长什么样」。
 *         不写这一段，编译器在 main 里看到 add_student(...) 时不认识它，
 *         会报 implicit declaration 错误。
 *
 *         注意 count 参数有两种写法：
 *           int *count → 函数会修改人数，所以传地址（第 10 课的指针）
 *           int count  → 函数只读不改，直接传值
 *
 *  预期输出：不产生输出。
 */
void  clear_input(void);
int   read_int(const char *prompt, int *out);
void  print_menu(void);
void  add_student(Student list[], int *count);
void  list_students(const Student list[], int count);
void  show_stats(const Student list[], int count);
void  find_student(const Student list[], int count);
void  sort_by_score(Student list[], int count);
void  save_to_file(const Student list[], int count);
void  load_from_file(Student list[], int *count);

/* ▶▶▶ 第 4 段：main —— 菜单循环（整个程序的总指挥）
 *  做什么：
 *    1) 准备一个能装 100 人的数组，和一个记录「当前有几个人」的 count；
 *    2) 死循环里反复：打印菜单 → 读用户选择 → switch 分发到对应功能；
 *    3) 任何一步读不到有效输入，都能安全退出或重来。
 *
 *  新语法：while (1)、switch/case/break、return
 *
 *  预期输出（刚启动时）：
 *      ============================================
 *                学 生 成 绩 管 理 器
 *      ============================================
 *      提示：如果之前保存过数据，可以先选 6 读取。
 *
 *      ---------------- 菜单 ----------------
 *        1. 添加学生
 *        2. 显示全部
 *        ...
 *      --------------------------------------
 *      请选择操作：
 */
int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;              /* 当前实际有多少个学生 */

    printf("============================================\n");
    printf("          学 生 成 绩 管 理 器\n");
    printf("============================================\n");
    printf("提示：如果之前保存过数据，可以先选 6 读取。\n");

    while (1) {                 /* 1 永远为真，靠 break / return 退出 */
        print_menu();

        int choice = 0;
        int rc = read_int("请选择操作：", &choice);

        if (rc == -2) {
            /* 输入结束了（比如按了 Ctrl+D），这时候必须退出，
             * 否则程序会一直重复提示，变成死循环。 */
            printf("\n输入已结束，程序退出。\n");
            return 0;
        }
        if (rc != 0) {
            printf("输入不合法，请输入数字。\n");
            continue;           /* 回到菜单重来 */
        }

        switch (choice) {
            case 1:
                add_student(students, &count);   /* 要改人数，所以传 &count */
                break;
            case 2:
                list_students(students, count);
                break;
            case 3:
                show_stats(students, count);
                break;
            case 4:
                sort_by_score(students, count);
                break;
            case 5:
                save_to_file(students, count);
                break;
            case 6:
                load_from_file(students, &count); /* 读回来也要改人数 */
                break;
            case 7:
                find_student(students, count);
                break;
            case 0:
                printf("\n感谢使用，再见！\n");
                return 0;
            default:
                printf("没有这个选项，请输入 0-7。\n");
                break;
        }
    }
}

/* ============================================================
 *  ▶▶▶ 第 5 段：小工具函数
 *  ---------------------------------------------------------------------------
 *  这一组函数不做业务，只解决「怎么和用户打交道」的问题。
 *  ============================================================ */

/* 把输入缓冲区里剩下的字符（包括换行）全部读掉 */
void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* 空循环：只读不处理 */
    }
}

/* 读一个整数：
 *   返回  0 -> 读成功
 *   返回 -1 -> 输入的不是整数
 *   返回 -2 -> 输入已经结束（Ctrl+D / 重定向的输入用完了）
 *
 * out 是指针：函数把读到的数字写进 *out，调用者那边就能拿到。 */
int read_int(const char *prompt, int *out) {
    printf("%s", prompt);
    int rc = scanf("%d", out);

    if (rc == EOF) {
        printf("\n");
        return -2;
    }
    if (rc != 1) {
        clear_input();
        return -1;
    }
    clear_input();      /* 数字后面的换行也要清掉，否则下次读字符串会读到空 */
    return 0;
}

/* 打印菜单 */
void print_menu(void) {
    printf("\n---------------- 菜单 ----------------\n");
    printf("  1. 添加学生\n");
    printf("  2. 显示全部\n");
    printf("  3. 统计信息\n");
    printf("  4. 按成绩排序（从高到低）\n");
    printf("  5. 保存到文件\n");
    printf("  6. 从文件读取\n");
    printf("  7. 按学号查询\n");
    printf("  0. 退出\n");
    printf("--------------------------------------\n");
}

/* ============================================================
 *  ▶▶▶ 第 6 段：各项功能
 *  ---------------------------------------------------------------------------
 *  这一组函数是真正的「业务逻辑」，每一个对应菜单里的一个选项。
 *  ============================================================ */

/* 添加一个学生 */
void add_student(Student list[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("名单已满（最多 %d 人），无法继续添加。\n", MAX_STUDENTS);
        return;
    }

    /* 新学生放在 list[*count]，也就是「当前人数的下一个空位」。
     * s 是指向这个空位的指针，用 -> 访问成员。 */
    Student *s = &list[*count];

    printf("请输入姓名（可以带空格）：");
    if (fgets(s->name, NAME_LEN, stdin) == NULL) {
        printf("读取姓名失败。\n");
        return;
    }
    s->name[strcspn(s->name, "\n")] = '\0';   /* 去掉末尾的换行 */

    if (s->name[0] == '\0') {
        printf("姓名不能为空，已取消。\n");
        return;
    }

    if (read_int("请输入学号：", &s->id) != 0) {
        printf("学号必须是数字，已取消。\n");
        return;
    }
    if (s->id <= 0) {
        printf("学号要大于 0，已取消。\n");
        return;
    }

    if (read_int("请输入成绩（0-100）：", &s->score) != 0) {
        printf("成绩必须是数字，已取消。\n");
        return;
    }
    if (s->score < 0 || s->score > 100) {
        printf("成绩要在 0 到 100 之间，已取消。\n");
        return;
    }

    (*count)++;      /* 人数加 1：先解引用，才能改到外面的 count */
    printf("已添加：%s（学号 %d，成绩 %d），当前共 %d 人。\n",
           s->name, s->id, s->score, *count);
}

/* 显示全部学生 */
void list_students(const Student list[], int count) {
    if (count == 0) {
        printf("名单是空的，先选 1 添加学生吧。\n");
        return;
    }

    printf("\n%-4s %-16s %-8s %-6s %s\n", "序号", "姓名", "学号", "成绩", "是否及格");
    printf("------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-4d %-16s %-8d %-6d %s\n",
               i + 1,
               list[i].name,
               list[i].id,
               list[i].score,
               (list[i].score >= 60) ? "及格" : "不及格");
    }
    printf("------------------------------------------------\n");
    printf("共 %d 人\n", count);
}

/* 统计：平均分、最高分、最低分、及格率 */
void show_stats(const Student list[], int count) {
    if (count == 0) {
        printf("还没有数据，无法统计。\n");
        return;
    }

    int sum = 0;
    int max = list[0].score;
    int min = list[0].score;
    int pass = 0;

    /* 一趟循环同时算出 4 个结果 */
    for (int i = 0; i < count; i++) {
        sum += list[i].score;
        if (list[i].score > max) {
            max = list[i].score;
        }
        if (list[i].score < min) {
            min = list[i].score;
        }
        if (list[i].score >= 60) {
            pass++;
        }
    }

    printf("\n---------- 统计信息 ----------\n");
    printf("人数    : %d\n", count);
    printf("总分    : %d\n", sum);
    printf("平均分  : %.2f\n", (double)sum / count);
    printf("最高分  : %d\n", max);
    printf("最低分  : %d\n", min);
    printf("及格    : %d 人（%.1f%%）\n", pass, 100.0 * pass / count);
    printf("------------------------------\n");
}

/* 按学号查询 */
void find_student(const Student list[], int count) {
    if (count == 0) {
        printf("还没有数据。\n");
        return;
    }

    int id = 0;
    if (read_int("请输入要查询的学号：", &id) != 0) {
        printf("学号必须是数字。\n");
        return;
    }

    /* 找到就打印并立刻 return；找到最后一个都没找到，就打印「没有找到」 */
    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            printf("找到了：%s，学号 %d，成绩 %d\n",
                   list[i].name, list[i].id, list[i].score);
            return;
        }
    }
    printf("没有找到学号为 %d 的学生。\n", id);
}

/* 按成绩从高到低排序（冒泡排序） */
void sort_by_score(Student list[], int count) {
    if (count < 2) {
        printf("数据太少，不需要排序。\n");
        return;
    }

    /* 冒泡排序：反复比较相邻两个，把大的往前换。
     * 数据量小的时候完全够用，而且很好理解。
     * 外层 i 走 count-1 趟，每趟把当前最大的一个「冒」到末尾。
     * 内层 j 只走到 count-1-i，因为末尾已经排好了。 */
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (list[j].score < list[j + 1].score) {   /* 从高到低 */
                Student temp = list[j];                /* 结构体可以整体交换 */
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    printf("已按成绩从高到低排好序，选 2 可以查看。\n");
}

/* ============================================================
 *  ▶▶▶ 第 7 段：存盘和读盘
 *  ---------------------------------------------------------------------------
 *  这一组函数负责把内存里的名单写到文件、再从文件读回来。
 *  ============================================================ */

/* 保存到文件 */
void save_to_file(const Student list[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("无法写入 %s，请检查目录权限。\n", DATA_FILE);
        return;
    }

    for (int i = 0; i < count; i++) {
        /* 把姓名放在最后，因为姓名里可能有空格。
         * 这样读回来的时候可以用 %[^\n] 把「一整行剩下的内容」都当姓名。 */
        fprintf(fp, "%d %d %s\n", list[i].id, list[i].score, list[i].name);
    }

    fclose(fp);
    printf("已保存 %d 条记录到 %s\n", count, DATA_FILE);
}

/* 从文件读取 */
void load_from_file(Student list[], int *count) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("还没有 %s 文件（先选 5 保存一次吧）。\n", DATA_FILE);
        return;
    }

    *count = 0;         /* 重新加载，先清空 */

    char name[NAME_LEN];
    int id = 0;
    int score = 0;

    /* %31[^\n] 的意思是：最多读 31 个「不是换行」的字符，
     * 也就是把这一行剩下的内容全部读进 name，空格也能保留下来。 */
    while (*count < MAX_STUDENTS &&
           fscanf(fp, "%d %d %31[^\n]", &id, &score, name) == 3) {
        strcpy(list[*count].name, name);
        list[*count].id = id;
        list[*count].score = score;
        (*count)++;
    }

    fclose(fp);
    printf("已从 %s 读取 %d 条记录。\n", DATA_FILE, *count);
}

/* ============================================================================
 *  ▶ 动手练习（这个程序还能怎么改进？）
 *  ---------------------------------------------------------------------------
 *  1) 加「删除学生」：找到后把它后面的元素全部往前挪一位，人数减 1。
 *     提示：数组中间删除元素的标准做法是「依次往前覆盖」。
 *
 *  2) 加「修改成绩」功能。
 *     提示：先按学号找到人，再改 list[i].score。
 *
 *  3) 加一个「导出 CSV」功能：用逗号分隔字段，方便用 Excel 打开。
 *     提示：新建一个文件，用 fprintf 写 "%s,%d,%d\n" 这样的格式。
 *
 *  4) 用 malloc 动态申请空间，替代固定大小的数组（进阶内容）。
 *     提示：这就要用到第 7 课讲过的指针，第 12 课之后的进阶内容。
 *
 *  5) 拆成 grade_book.h / grade_book.c / main.c 三个文件，
 *     练习多文件工程。
 *
 *  ▶ 本项目常见报错速查
 *  ---------------------------------------------------------------------------
 *  菜单重复刷屏，停不下来
 *      → 死循环。多半是 read_int 的 -2（输入结束）分支没处理。
 *
 *  添加学生后人数没变
 *      → 忘了 (*count)++，或者写成了 count++（那只是改了一个副本）。
 *
 *  读回来的姓名少了后半截
 *      → %31[^\n] 里的 31 写错了，或者 name 数组不够大。
 *        数字 31 必须 = 数组长度 - 1。
 *
 *  保存时中文变成了乱码
 *      → 文件是按字节写的，没写编码信息。用文本编辑器打开 gradebook.txt
 *        应该能看到正常中文；如果是乱码，检查一下编辑器显示编码。
 *
 *  排序之后顺序没变
 *      → 比较条件写反了。从高到低应该是 list[j].score < list[j+1].score。
 *
 *  ▶ 自测题答案
 *  ---------------------------------------------------------------------------
 *  1) 从 0 开始。所以 list[0] 是第一个人，list[count-1] 是最后一个人。
 *
 *  2) 传地址：int *count，调用时写 &count，函数里用 *count 改。
 *     因为 C 的参数是值传递，传值的话函数改的是副本，外面看不到变化。
 *
 *  3) "w" 是写入（write），会清空原文件再写；"a" 是追加（append），
 *     在文件末尾接着写，不会动原来的内容。
 *
 *  ▶ 本项目小结（能用自己的话说出来就算真的会了）
 *  ---------------------------------------------------------------------------
 *  · 一个实用的命令行程序 = 菜单循环 + 一堆功能函数 + 存盘读盘
 *  · 需要修改调用者数据的功能，参数要传指针（&count）
 *  · 只读不改的参数加 const，既安全又让读代码的人一眼看懂意图
 *  · 所有用户输入都要校验，所有文件操作都要判 NULL
 *  · 数据要能存下来才算数——这就是第 12 课文件读写的意义
 *
 *  ▶ 学完之后往哪走
 *  ---------------------------------------------------------------------------
 *  · 动态内存：malloc / free，替代固定大小的数组
 *  · 多文件工程：拆成 .h 和 .c，这是写大程序的第一步
 *  · 数据结构：链表、栈、队列
 *  · 实用工具：gdb（调试器）、valgrind（查内存泄漏）、git（版本管理）
 * ============================================================================ */
