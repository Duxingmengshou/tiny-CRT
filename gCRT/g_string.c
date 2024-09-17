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
    for (unsigned int i = 0, j = size - 1; i < j; i++, j--) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
    return s; // 返回反转后的字符串
}


// 数字转字符串，注意此处写死了str为char[32]
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

// 字符串转数字，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，而再遇到非数字或字符串结束时('\0')才结束转换，并将结果返回。
int g_atoi(const char *nptr) {
    int result = 0;
    int sign = 1;
    // 跳过前导空格
    while (!g_isdigit(*nptr) && *nptr != '-' && *nptr != '+') {
        nptr++;
    }
    // 处理符号
    if (*nptr == '-') {
        sign = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }
    // 转换数字
    while (*nptr >= '0' && *nptr <= '9') {
        result = result * 10 + (*nptr - '0');
        nptr++;
    }
    return sign * result; // 返回结果
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