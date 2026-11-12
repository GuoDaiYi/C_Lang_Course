# 一个给新手用的简单 Makefile
# 常用命令：
#   make                              编译全部
#   make run L=lessons/01_hello_world 编译并运行指定文件（不用写 .c）
#   make list                         列出所有可以运行的程序
#   make clean                        清理

CC      ?= cc
CFLAGS  ?= -Wall -Wextra -Wpedantic -std=c11 -g
# math.h 里的函数（sqrt、pow...）在单独的数学库里，
# Linux 上链接时必须加上 -lm，macOS 上写不写都行。
LDLIBS  ?= -lm
BUILD   := build

# L 是要运行的文件（不含 .c），比如 lessons/05_loops
L ?= lessons/01_hello_world

LESSON_SRC  := $(wildcard lessons/*.c)
EX_SRC      := $(wildcard exercises/*.c exercises/solutions/*.c)
PROJECT_SRC := $(wildcard projects/*.c)
ALL_SRC     := $(LESSON_SRC) $(EX_SRC) $(PROJECT_SRC)
ALL_BIN     := $(patsubst %.c,$(BUILD)/%,$(ALL_SRC))

.PHONY: all run list clean help

# 默认目标：把所有源文件都编译一遍，提前暴露语法错误
all: $(ALL_BIN)

# 通用规则：build/xxx/yyy 由 xxx/yyy.c 编译而来
$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
	@echo "  编译成功 -> $@"

# 编译并运行：make run L=lessons/05_loops
run: $(BUILD)/$(L)
	@echo "===== 运行 $(L) ====="
	@$(BUILD)/$(L)

list:
	@echo "可运行的程序（用 make run L=... 运行，不要写 .c）："
	@for f in $(ALL_SRC); do echo "  $${f%.c}"; done

clean:
	@rm -rf $(BUILD)
	@echo "已清理 $(BUILD)/ 目录"

# ---------------------------------------------------------------------------
# 彻底清理：把「随时能重新生成的垃圾」全部清掉，源码和文档一律不动。
#
# 判断标准只有一条：删掉之后，跑一条命令能不能重新造出来？
#   能   -> 是产物，可以删
#   不能 -> 是你写的代码或文档，绝不能删
#
# 具体清四类：
#   1. build/            编译产物（make 就能重建）
#   2. 源码目录里的可执行文件（手动 cc 时忘了加 -o build/... 留下的）
#   3. *.dSYM/           macOS 调试符号包
#   4. .DS_Store         macOS 文件夹元数据
#   5. 运行时数据文件     students.txt / report.txt / gradebook.txt ...
# ---------------------------------------------------------------------------
tidy: clean
	@rm -rf lessons/*.dSYM exercises/*.dSYM projects/*.dSYM
	@find lessons exercises projects -type f \
	    ! -name '*.c' ! -name '*.md' ! -name '*.sh' -delete 2>/dev/null || true
	@find lessons exercises projects -depth -type d -empty -delete 2>/dev/null || true
	@find . -name '.DS_Store' -delete 2>/dev/null || true
	@rm -f students.txt report.txt gradebook.txt scores.txt
	@echo "已彻底清理：编译产物、散落的可执行文件、macOS 垃圾、运行时数据文件。"
	@echo "源码和文档一个都没动，随时用 make 重新编译。"

help:
	@echo "make                              编译全部"
	@echo "make run L=lessons/02_variables   编译并运行指定文件"
	@echo "make list                         列出所有程序"
	@echo "make clean                        只删 build/ 目录"
	@echo "make tidy                         彻底清理（编译产物 + macOS 垃圾 + 运行时数据）"
	@echo ""
	@echo "提示：数学函数（sqrt、pow...）需要链接数学库 -lm，"
	@echo "      本 Makefile 已经自动加上了，手动编译时要自己写。"
