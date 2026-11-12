/*
 * ============================================================================
 *  第 9 课：字符串
 * ============================================================================
 *
 *  ▶ 这节课要解决什么问题
 *  ---------------------------------------------------------------------------
 *    上一课我们用数组存了一排数字，可现实里更常见的是「一段文字」：
 *    你的名字、一句问候语、一个命令……
 *
 *    C 语言里没有「字符串」这种单独的类型，它是用「字符数组」
 *    冒充的——而且在每段文字的末尾偷偷放了一个 '\0' 当结束标记，
 *    告诉所有函数「文字到这儿就没了」。
 *
 *    这节课你会搞明白：为什么 char a[] = "abc"; 的 sizeof 是 4 而不是 3、
 *    为什么比较两段文字不能用 ==、
 *    以及 <string.h> 里那一堆函数到底各自是干什么的。
 *
 *  ▶ 学这节课之前，你需要已经会（前置知识）
 *  ---------------------------------------------------------------------------
 *    ✅ 已经会：第 8 课：数组（声明、初始化、下标从 0 开始、sizeof 算元素个数）
 *    ✅ 已经会：第 3 课：让程序会问问题（scanf 怎么用）
 *    ✅ 已经会：第 5 课：多分支选择 switch
 *    ✅ 已经会：第 6 课：循环（for、while）
 *    🔁 忘了就看：第 8 课的数组初始化；第 3 课的 scanf 写法
 *    ❓ 开课自测（这 4 题能答上来就可以往下学，答案在文件最末尾）
 *       1) char 是什么类型？一个 char 能存几个字节？
 *       2) int a[5] 里有几个格子？合法的下标最大是几？
 *       3) 5 个 int 的数组用 sizeof 算出来是多少字节？
 *       4) printf 想打印一个整数用哪个格式符？想打印一个小数呢？
 *
 *  ▶ 本节课新出现的语法（先扫一眼这张表，看不懂先跳过，学到自然会懂）
 *  ---------------------------------------------------------------------------
 *    1) 用字符数组存一段文字
 *       写法：char name[20] = "Lucy";
 *       读作：字符型 数组 name【20 个】= "Lucy"
 *       作用：准备 20 个 char 格子，把文字 "Lucy" 放进去。
 *       注意：C 里没有「字符串」这个类型，字符串就是「字符数组」。
 *             而且文字末尾会自动补一个 '\0'，
 *             所以 20 个格子实际只能装 19 个字母 + 1 个结束标记。
 *
 *    2) '\0'（字符串的结束标记）
 *       写法：'\0'
 *       读作：反斜杠零
 *       作用：ASCII 值是 0 的一个特殊字符，标记「文字到此结束」。
 *             printf("%s")、strlen 这些函数全靠它判断文字在哪儿收尾。
 *       注意：它是一个真正的字符，会占掉一个格子的位置。
 *
 *    3) sizeof 为什么要多 1
 *       写法：char a[] = "abc";
 *       读作：字符型 数组 a（长度让编译器自己数）= "abc"
 *       作用：sizeof(a) 等于 4，不是 3！
 *             因为除了 a、b、c 三个字母，编译器还要腾一个格子放 '\0'。
 *             规则：字符串占的格子数 = 字母个数 + 1。
 *       注意：strlen("abc") 才是 3，因为它不把 '\0' 算进去。
 *             记住这个区别，面试和考试都常问。
 *
 *    4) %s 和 %c 这两个格式符
 *       写法：printf("%s", name);   printf("%c", name[0]);
 *       作用：%s 打印「一整段文字」（从开头一直打到 '\0' 为止）；
 *             %c 打印「一个字符」。
 *       注意：%s 要配 char 数组（或第 10 课的字符串指针），
 *             千万别用 %s 去打印一个 int，会打印出乱七八糟的地址。
 *
 *    5) #include <string.h>（字符串工具箱）
 *       作用：请进来一整套处理字符串的函数，不写这一行会报「函数没声明」。
 *       各函数分别干什么：
 *         strlen(s)      数长度：返回字符个数，不含 '\0'
 *         strcpy(dst, s) 复制：把 s 的内容抄到 dst 里（dst 要够大）
 *         strcat(dst, s) 拼接：把 s 接到 dst 后面（dst 要够大）
 *         strcmp(a, b)   比较：相等返回 0，a 小于 b 返回负数，反之返回正数
 *         strchr(s, c)   找字符：找到就返回「指向那个字符的地址」，
 *                        找不到返回 NULL
 *       注意：strcmp 返回负数还是正数由系统决定，本机上
 *             strcmp("Hello", "World") 恰好是 -1。
 *             千万不要写 if (strcmp(a, b) == 1)，
 *             正确的判断方式是 strcmp(a, b) == 0 才算相等。
 *
 *    6) #include <ctype.h>（字符工具箱）
 *       作用：判断和转换单个字符的工具。
 *       各函数分别干什么：
 *         isupper(c)  是大写字母吗？  islower(c) 是小写字母吗？
 *         isdigit(c)  是数字 0-9 吗？ isalpha(c) 是字母吗？
 *         toupper(c)  转成大写        tolower(c) 转成小写
 *       注意：这些函数只吃「一个字符」，而且只对英文有效，
 *             对中文汉字不起作用。
 *
 *    7) 字符串常量（用指针指向一段只读的文字）
 *       写法：const char *greeting = "Hello";
 *       读作：常量 字符指针 greeting = "Hello"
 *       作用：指向一段固定不变的文字。
 *       注意：和 char name[20] = "Lucy"; 的区别——
 *             前者是「一块可以随便改的数组」，后者是「指向只读的文字」。
 *             指针（那个 * 是什么、地址怎么算）留到第 10 课，
 *             这里只要知道它能用就行。
 *
 *    8) NULL 和 strchr 的返回值
 *       写法：char *pos = strchr(buffer, 'o');  if (pos != NULL) { ... }
 *       作用：strchr 找到字符时返回「指向它的地址」，找不到返回 NULL
 *             （一个表示「什么都没有」的特殊值）。
 *       注意：所以用它的结果之前一定要先判一下是不是 NULL，
 *             否则会拿一个空地址去做运算，直接崩溃。
 *
 *    9) 指针相减
 *       写法：(long)(pos - buffer)
 *       作用：两个地址相减，得到「中间隔了几个元素」。
 *             这里算出第一个 'o' 在第 4 个位置。
 *       注意：指针相减是第 10 课的内容，现在只要知道它能算距离就行。
 *
 *   10) scanf 的宽度限制
 *        写法：scanf("%19s", input)
 *        作用：%19s 意思是「最多读 19 个字符」。
 *              数组是 20 个格子，要留一个给 '\0'，
 *              所以最多只能读 19 个，不然就溢出了。
 *        注意：这是防止输入太长把数组撑爆的标准做法，务必写上。
 *
 *   11) 字符串数组
 *        写法：const char *commands[] = {"add", "del", "list"};
 *        作用：把几个字符串放进一个数组里存起来。
 *        注意：commands[i] 是一个字符串，commands[i][0] 就是它的第一个字符。
 *
 *  ▶ 怎么读这个文件
 *  ---------------------------------------------------------------------------
 *    1. 先读完上面三块（问题 / 前置 / 新语法），心里先有个框架；
 *    2. 往下看代码，每段代码前面都有「▶▶▶ 第 N 段」，讲清这段在干嘛、
 *       跑起来会看到什么；
 *    3. 边看边在脑子里预演输出，觉得「我大概知道这段会打印什么」再往下走；
 *    4. 最后做「动手练习」和「自测题」。
 *
 *  运行方式（在本项目根目录下）：
 *    make run L=lessons/09_strings
 *    或者手动：cc lessons/09_strings.c -o demo && ./demo
 *
 *  ★ 注意：第 6 段会等你敲键盘输入内容。
 *    运行后屏幕上出现「请输入 "yes" 试试」时，直接敲 yes 再按回车即可。
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>   /* strlen strcpy strcat strcmp strchr */
#include <ctype.h>    /* toupper tolower isdigit isalpha */

int main(void) {

    /* ▶▶▶ 第 1 段：字符串就是「末尾带 \0 的字符数组」
     *  做什么：把 "Lucy" 放进一个 20 格子的字符数组，
     *          再对比 sizeof（占多少字节）和 strlen（几个字符）。
     *  新语法：char name[20] = "Lucy";、'\0'、%s、%zu
     *  预期输出：屏幕上出现
     *            name = Lucy
     *            sizeof(name) = 20   <-- 数组总大小
     *            strlen(name) = 4   <-- 实际字符数，不含 \0
     *           为什么 sizeof 是 20 而不是 5？因为你写的是 name[20]，
     *           数组开多大就是多大；strlen 是 4，因为 "Lucy" 有 4 个字母。
     */
    char name[20] = "Lucy";
    /* 内存里实际是： 'L' 'u' 'c' 'y' '\0' ...（后面都是空的）
     * '\0' 是隐藏的结束标记，ASCII 值 0。
     * 所有字符串函数都靠它判断「字符串到哪儿结束」。
     * 所以长度 20 的数组最多只能装 19 个字符 + 1 个 '\0'。 */

    printf("name = %s\n", name);
    printf("sizeof(name) = %zu   <-- 数组总大小\n", sizeof(name));
    printf("strlen(name) = %zu   <-- 实际字符数，不含 \\0\n", strlen(name));

    /* ▶▶▶ 第 2 段：亲眼看看那个多出来的 '\0'
     *  做什么：① 把 name 数组的每个格子都打印出来，看看第 5 个格子藏了什么；
     *          ② 再用一个刚好装下 "abc" 的数组，对比 sizeof 和 strlen。
     *  新语法：(int) 强制转换（把 char 变成 int，再用 %d 打印它的 ASCII 码）
     *  预期输出：屏幕上出现
     *            --- 那个多出来的 \0 ---
     *            name 数组里每个格子的内容：
     *              name[0] = 76
     *              name[1] = 117
     *              name[2] = 99
     *              name[3] = 121
     *              name[4] = 0   <-- 这就是隐藏的 '\0'，ASCII 码 0
     *            sizeof(abc) = 4   <-- 3 个字母 + 1 个 '\0'
     *            strlen(abc) = 3   <-- 只有 3 个字母，不算 '\0'
     */
    printf("\n--- 那个多出来的 \\0 ---\n");
    printf("name 数组里每个格子的内容：\n");
    for (int i = 0; i <= 4; i++) {          /* 故意多看一格：正好是 '\0' */
        printf("  name[%d] = %d", i, (int)name[i]);
        if (name[i] == '\0') {
            printf("   <-- 这就是隐藏的 '\\0'，ASCII 码 0");
        }
        printf("\n");
    }

    char abc[] = "abc";               /* 长度不写，让编译器自己数 */
    printf("sizeof(abc) = %zu   <-- 3 个字母 + 1 个 '\\0'\n", sizeof(abc));
    printf("strlen(abc) = %zu   <-- 只有 3 个字母，不算 '\\0'\n", strlen(abc));

    /* ▶▶▶ 第 3 段：用指针指向一段只读的文字
     *  做什么：声明一个字符串常量指针 greeting，让它指向 "Hello"。
     *  新语法：const char *greeting = "Hello";
     *  预期输出：屏幕上出现
     *            greeting = Hello
     */
    /* 也可以用指针方式指向一个字符串常量 */
    const char *greeting = "Hello";
    printf("greeting = %s\n", greeting);
    /* 区别：char name[20] 是一块可以修改的数组；
     *       const char *greeting 指向只读的文字，别去改它的内容。 */

    /* ▶▶▶ 第 4 段：逐字符访问与遍历
     *  做什么：先把 "Lucy" 的 4 个字母一个一个打出来，
     *          再倒着打一遍。
     *  新语法：%c 格式符；name[i] 取第 i 个字符
     *  预期输出：屏幕上出现
     *            --- 逐个字符打印 "Lucy" ---
     *            第 0 个字符: L
     *            第 1 个字符: u
     *            第 2 个字符: c
     *            第 3 个字符: y
     *            倒着打印: ycuL
     */
    int name_len = (int)strlen(name);  /* 长度只算一次，后面直接拿来用 */
    printf("\n--- 逐个字符打印 \"%s\" ---\n", name);
    for (int i = 0; i < name_len; i++) {
        printf("第 %d 个字符: %c\n", i, name[i]);
    }

    /* 也可以直接用 '\0' 作为结束条件 */
    printf("倒着打印: ");
    for (int i = name_len - 1; i >= 0; i--) {
        printf("%c", name[i]);
    }
    printf("\n");

    /* ▶▶▶ 第 5 段：常用字符串函数
     *  做什么：准备 first、second、buffer 三个字符数组，
     *          依次演示测长度、比较、复制、拼接、找字符。
     *  新语法：strlen / strcmp / strcpy / strcat / strchr、NULL
     *  预期输出：屏幕上出现
     *            --- string.h 常用函数 ---
     *            strlen("Hello") = 5
     *            strcmp("Hello", "World") = -1（负数说明 Hello 在前）
     *            strcmp("abc", "abc")   = 0（等于 0 说明相同）
     *            复制之后 buffer = Hello
     *            拼接之后 buffer = Hello World
     *            第一个 'o' 在位置 4（从 0 数）
     */
    char first[20] = "Hello";
    char second[20] = "World";
    /* buffer 要装下 "Hello" + 空格 + "World" = 11 个字符，
     * 末尾还要再留 1 个格子给 '\0'，一共要 12 格。
     * 开 50 格是为了留足余量——数组开小了，strcpy / strcat 会写坏内存。 */
    char buffer[50] = {0};      /* 留足够大的空间，并清空 */

    printf("\n--- string.h 常用函数 ---\n");
    printf("strlen(\"%s\") = %zu\n", first, strlen(first));

    /* 比较：相等返回 0，不能直接用 == */
    printf("strcmp(\"Hello\", \"World\") = %d（负数说明 Hello 在前）\n",
           strcmp("Hello", "World"));
    printf("strcmp(\"abc\", \"abc\")   = %d（等于 0 说明相同）\n",
           strcmp("abc", "abc"));

    /* 复制与拼接：目标数组一定要够大 */
    strcpy(buffer, first);              /* 把 first 复制进 buffer */
    printf("复制之后 buffer = %s\n", buffer);

    strcat(buffer, " ");                /* 在末尾追加一个空格 */
    strcat(buffer, second);             /* 再追加 second */
    printf("拼接之后 buffer = %s\n", buffer);

    /* 找字符：返回指针，找不到返回 NULL */
    char *pos = strchr(buffer, 'o');
    if (pos != NULL) {                  /* 用之前一定要先判 NULL */
        printf("第一个 'o' 在位置 %ld（从 0 数）\n", (long)(pos - buffer));
        /* 指针相减得到「相隔几个元素」，第 10 课会讲 */
    }

    /* ▶▶▶ 第 6 段：判断字符串是否相同的正确写法
     *  做什么：提示你输入一个词，程序读进来后判断它是不是 "yes"。
     *  新语法：scanf("%19s", input) != 1、strcmp(input, "yes") == 0
     *  预期输出：屏幕上出现
     *            请输入 "yes" 试试（输入其他内容会有不同结果）：你输入的是 yes，判断正确。
     *           （输入别的内容，比如 hello，则会打印
     *            你输入的是 "hello"，不等于 yes。）
     */
    /* input 开 20 个格子，所以 %19s 最多读 19 个字符，
     * 剩下的 1 个格子留给 '\0'，这样才装得下。 */
    char input[20] = {0};
    printf("\n请输入 \"yes\" 试试（输入其他内容会有不同结果）：");
    if (scanf("%19s", input) != 1) {    /* 限制最多读 19 个字符，防止溢出 */
        printf("没有读到内容\n");
        return 1;
    }

    if (strcmp(input, "yes") == 0) {
        printf("你输入的是 yes，判断正确。\n");
    } else {
        printf("你输入的是 \"%s\"，不等于 yes。\n", input);
    }
    /* 千万别写成 if (input == "yes")：那比较的是两个地址，永远为假。 */

    /* ▶▶▶ 第 7 段：自己造轮子——算长度、转大写
     *  做什么：不调用 strlen，自己数到 '\0' 为止算出长度；
     *          再用 toupper 把每个字母转成大写打出来。
     *  新语法：while (buffer[length] != '\0')；toupper
     *  预期输出：屏幕上出现
     *            手动数出 buffer 的长度是 11（和 strlen 一样）
     *            转成大写: HELLO WORLD
     *           "Hello World" 是 11 个字母（中间还有一个空格），
     *           所以转成大写之后是 HELLO WORLD。
     */
    int length = 0;
    while (buffer[length] != '\0') {    /* 一直数到结束标记 */
        length++;
    }
    printf("\n手动数出 buffer 的长度是 %d（和 strlen 一样）\n", length);

    printf("转成大写: ");
    for (int i = 0; buffer[i] != '\0'; i++) {
        printf("%c", toupper(buffer[i]));   /* ctype.h 提供的转换函数 */
    }
    printf("\n");

    /* ▶▶▶ 第 8 段：统计一段文字里的大写字母和数字
     *  做什么：遍历 "C Language 2024 from OpenAI!"，
     *          数出字母个数、大写字母个数、数字个数。
     *  新语法：isupper / isdigit / isalpha；(unsigned char) 强制转换
     *  预期输出：屏幕上出现
     *            文字：C Language 2024 from OpenAI!
     *            字母 19 个，其中大写 5 个；数字 4 个。
     *           5 个大写字母是 C、L、O、A、I；4 个数字是 2、0、2、4。
     */
    const char *text = "C Language 2024 from OpenAI!";
    int upper_count = 0;
    int digit_count = 0;
    int letter_count = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper((unsigned char)text[i])) {
            upper_count++;
        }
        if (isdigit((unsigned char)text[i])) {
            digit_count++;
        }
        if (isalpha((unsigned char)text[i])) {
            letter_count++;
        }
    }
    printf("\n文字：%s\n", text);
    printf("字母 %d 个，其中大写 %d 个；数字 %d 个。\n",
           letter_count, upper_count, digit_count);

    /* ▶▶▶ 第 9 段：用 switch 处理命令字符
     *  做什么：遍历三个命令 add / del / list，
     *          只看每个单词的首字母，用 switch 分发到对应的提示。
     *  新语法：字符串数组 const char *commands[]；commands[i][0] 取首字母
     *  预期输出：屏幕上出现
     *            --- 命令解释器 ---
     *            命令 add -> 新增一条记录
     *            命令 del -> 删除一条记录
     *            命令 list -> 列出全部记录
     */
    /* 很多命令行工具都是这样工作的：读到一个命令，
     * 用 switch 决定做什么（第 5 课的 switch 在这里派上用场）。 */
    printf("\n--- 命令解释器 ---\n");

    const char *commands[] = {"add", "del", "list"};   /* 字符串数组 */
    int command_count = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < command_count; i++) {
        printf("命令 %s -> ", commands[i]);
        switch (commands[i][0]) {      /* commands[i][0] 是这个单词的首字母 */
            case 'a':
                printf("新增一条记录\n");
                break;
            case 'd':
                printf("删除一条记录\n");
                break;
            case 'l':
                printf("列出全部记录\n");
                break;
            default:
                printf("不认识这个命令\n");
                break;
        }
    }

    /* ▶▶▶ 第 10 段：结束程序
     *  做什么：告诉操作系统「程序跑完了，一切正常」。
     *  预期输出：屏幕内容到此结束，进程退出。
     */
    return 0;
}

/* ============================================================================
 *  ▶ 动手练习（先自己做，卡住了再翻 CHEATSHEET.md）
 *  ---------------------------------------------------------------------------
 *  1) 让用户输入自己的名字，输出「你好，XXX！你的名字有 N 个字符」。
 *     提示：scanf("%19s", name) 读进来，再用 strlen 算长度。
 *     验收：输入「小明」输出「你好，小明！你的名字有 2 个字符」。
 *
 *  2) 写一个函数把字符串里的所有小写字母变成大写（用 toupper）。
 *     提示：照第 7 段的循环改一下，但要写进字符串里而不是直接打印。
 *     验收：传入 "hello" 之后字符串本身变成 "HELLO"。
 *
 *  3) 判断用户输入的字符串是不是回文（正着读反着读一样），
 *     比如 "level"。提示：用两个下标从两端往中间比较。
 *     验收：输入 level 输出「是回文」，输入 hello 输出「不是回文」。
 *
 *  4) 统计一个字符串里出现了多少个空格。
 *     提示：写个循环，遇到 ' ' 就把计数器加 1。
 *     验收："a b c" 应该统计出 2 个空格。
 *
 *  5) 用 sprintf 把数字格式化成字符串：
 *       char s[50];
 *       sprintf(s, "总分是 %d 分", 285);
 *       printf("%s\n", s);
 *     提示：sprintf 和 printf 长得像，区别是它把结果写进数组，
 *           而不是打印到屏幕上。头文件还是 stdio.h。
 *     验收：屏幕上打印出「总分是 285 分」。
 *
 *  6) 把上面那个命令解释器改成让用户输入一个命令，
 *     判断它是 add / del / list 还是别的命令。
 *     提示：先取首字母，再用第 5 课的 switch 分发。
 *     验收：输入 add 打印「新增一条记录」，输入 xxx 打印「不认识这个命令」。
 *
 *  ★ 三个必须记住的点：
 *     1) 字符串末尾有隐藏的 '\0'，数组要留出空间；
 *     2) 比较用 strcmp 而不是 ==；
 *     3) 复制、拼接前确认目标数组够大，否则会溢出破坏别的内存。
 *
 *  ▶ 本课常见报错速查
 *  ---------------------------------------------------------------------------
 *  warning: comparison with string literal results in unspecified behavior
 *      → 写了 if (input == "yes")。== 比较的是两个地址，
 *        而字符串文字的地址永远不相等，所以永远为假。
 *        正确写法：if (strcmp(input, "yes") == 0) { ... }
 *
 *  warning: implicit declaration of function 'strlen'
 *      → 忘了写 #include <string.h>。strlen、strcpy、strcat、strcmp、
 *        strchr 全在这个头文件里。
 *
 *  error: initializer-string for array of chars is too long
 *      → 文字比数组长。比如 char name[3] = "Lucy"; 装不下。
 *        数组要按「字母个数 + 1」来开。
 *
 *  程序一运行就 Segmentation fault（段错误）
 *      → 最常见的原因是数组太小：比如 char buf[4] = "Lucy";，
 *        4 个字母加上 '\0' 一共要 5 个格子，4 个格子根本不够，
 *        写进去的时候就已经踩到别的内存了。
 *
 *  ▶ 自测题答案
 *  ---------------------------------------------------------------------------
 *  1) char 是「字符」类型，一个 char 占 1 个字节（8 位），能表示 256 种取值。
 *
 *  2) int a[5] 有 5 个格子，合法下标是 0~4，最大下标 = 长度 - 1 = 4。
 *
 *  3) 20 字节。int 在常见的 64 位电脑上占 4 字节，5 × 4 = 20。
 *
 *  4) 整数用 %d，小数用 %f 或 %.2f（保留两位小数）。
 *
 *  ▶ 本课小结（能用自己的话说出来，就算真的会了）
 *  ---------------------------------------------------------------------------
 *  · C 里没有字符串类型，字符串 = 末尾带 '\0' 的字符数组
 *  · '\0' 是结束标记，所有字符串函数都靠它判断文字在哪儿收尾
 *  · ★ char a[] = "abc"; 的 sizeof 是 4（3 个字母 + 1 个 '\0'），
 *    strlen 才是 3 —— 数组占多少 vs 文字有多长，是两回事
 *  · 数组长度要留出 '\0' 的位置：char name[20] 最多装 19 个字母
 *  · %s 打印整段文字，%c 打印单个字符
 *  · <string.h>：strlen 数长度 / strcpy 复制 / strcat 拼接 /
 *    strcmp 比较（相等返回 0）/ strchr 找字符（找不到返回 NULL）
 *  · <ctype.h>：isupper、islower、isdigit、isalpha 判断，
 *    toupper、tolower 转换
 *  · ★ 比较两段文字用 strcmp(a, b) == 0，绝不能用 ==
 *  · strcpy / strcat 之前一定要确认目标数组够大
 *  · scanf("%19s", ...) 的宽度限制能防止输入过长撑爆数组
 *  · 用 strchr 的结果之前，先判一下是不是 NULL
 *
 *  ▶ 下一课预告
 *  ---------------------------------------------------------------------------
 *  第 10 课：指针 —— C 语言最有特色、也最让人头疼的东西。
 *  上一课那句「函数拿到的是一份拷贝」，正是指针能打破的规矩。
 * ============================================================================ */
