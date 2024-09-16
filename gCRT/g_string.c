#include "gcrt.h"

// 字符串长度
unsigned int g_strlen(const char *str) {
    unsigned int cnt = 0;
    if (!str)
        return 0;
    // 对有结束符0以及没有结束符的都进行结束判断
    for (; (int) *str != (int) (char) 0xab &&
           (int) *str != (int) (char) 0xcd &&
           *str != '\0'; ++str) {
        ++cnt;
    }
    return cnt;
}

// 字符串反转，对前size个字符进行反转
char *g_reverse(char *s, unsigned int size) {
    if (size <= 1) { return s; }
    if (size % 2 == 0) {
        for (unsigned int i = 0, j = size - 1; i - 1 != j; i++, j--) {
            char tmp = s[i];
            s[i] = s[j];
            s[j] = tmp;
        }
    } else {
        for (unsigned int i = 0, j = size - 1; i != j && j > 0; i++, j--) {
            char tmp = s[i];
            s[i] = s[j];
            s[j] = tmp;
        }
    }
}

// 对2-35进制进行数字转字符串，注意此处写死了str为char[32]
char *g_itoa(int n, char *str, int radix) {
    char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (radix > 36 || radix < 2)
        return NULL;
    char *head = str;
    char *p = head;
    unsigned int size = 0;
    for (int i = 0; i < 32; i++) {
        if (n < 0 && i == 0) {
            *p = '-', p++;
            size++;
        } else if (n != 0) {
            *p = digit[n % radix];
            p++;
            n /= radix;
            size++;
        } else {
            *p = '\0', p++;
        }
    }
    g_reverse(str, size);

    return str;
}

// 字符串比较，-1：src<dst，1：src>dst，0：相等
int g_strcmp(const char *src, const char *dst) {
    if (g_strlen(src) < g_strlen(dst)) {
        return -1;
    } else if (g_strlen(src) > g_strlen(dst)) {
        return 1;
    } else {
        for (unsigned int i = 0; i < g_strlen(src); i++) {
            if (src[i] < dst[i])
                return 1;
            else if (src[i] > dst[i])
                return -1;
            else
                continue;
        }
        return 0;
    }
}
// 字符串复制
char *g_strcpy(char *dest, const char *src) {
    for (unsigned int i = 0; i < g_strlen(dest) && i < g_strlen(src); i++) {
        dest[i] = src[i];
    }
    return dest;
}