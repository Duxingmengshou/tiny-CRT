#include "gcrt.h"

// msvc需要禁用优化，否则会自动替换为CRT中的memset函数，但是因为/NODEFAULTLIB禁用了CRT，所以会报错找不到memset标识符
void *g_memset(void *s, char c, unsigned int n) {
    g_printf("g_memset\n");
    char *ptr = (char *) s;
    for (unsigned int i = 0; i < n; i++) {
        *ptr = c;
        ptr++;
    }
    return s;
}