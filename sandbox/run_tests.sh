#!/bin/sh
# 编译（Linux/gcc 下需要 -lm，这里加上更保险）并跑几组典型输入
set -u
cd "$(dirname "$0")"

cc -std=c11 -Wall -Wextra -O2 triangle.c -o triangle -lm || exit 1
cc -std=c11 -Wall -Wextra -O2 triangle_fixed.c -o triangle_fixed -lm || exit 1

echo "=== 原版 triangle ==="
for t in "3 4 5" "1 1 1" "1 1 3" "1 2 3" "-1 2 2" "0 0 0" "abc 1 2" "1e200 1e200 1e200"; do
    printf '%-22s -> ' "$t"
    echo "$t" | ./triangle || true
done

echo
echo "=== 修正版 triangle_fixed ==="
for t in "3 4 5" "1 1 1" "1 1 3" "1 2 3" "-1 2 2" "0 0 0" "abc 1 2" "1e200 1e200 1e200"; do
    printf '%-22s -> ' "$t"
    echo "$t" | ./triangle_fixed 2>&1 || true
done
