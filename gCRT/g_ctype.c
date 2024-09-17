#include "gcrt.h"

// 检查字符是否为字母或数字
int g_isalnum(int c) {
    return (g_isalpha(c) || g_isdigit(c));
}

// 检查字符是否为字母
int g_isalpha(int c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

// 检查字符是否为 ASCII 字符
int g_isascii(int c) {
    return (c >= 0 && c <= 127);
}

// 检查字符是否为控制字符
int g_iscntrl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
}

// 检查字符是否为数字
int g_isdigit(int c) {
    return (c >= '0' && c <= '9');
}

// 检查字符是否为可打印字符（不包括空格）
int g_isgraph(int c) {
    return (c >= 33 && c <= 126);
}

// 检查字符是否为小写字母
int g_islower(int c) {
    return (c >= 'a' && c <= 'z');
}

// 检查字符是否为可打印字符（包括空格）
int g_isprint(int c) {
    return (c >= 32 && c <= 126);
}

// 检查字符是否为空白字符
int g_isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

// 检查字符是否为标点符号
int g_ispunct(int c) {
    return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) ||
           (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

// 检查字符是否为大写字母
int g_isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

// 检查字符是否为十六进制数字
int g_isxdigit(int c) {
    return (g_isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}
