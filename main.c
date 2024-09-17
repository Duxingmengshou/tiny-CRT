#include "GCRT/gcrt.h"

int main() {
    char *p = "abc+123456dfg";
    int k = g_atoi(p);
    g_printf("%d", k);
    return 0;
}
