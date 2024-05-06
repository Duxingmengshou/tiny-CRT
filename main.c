#include "gcrt.h"
int main() {
    g_crt_init_heap();
    char str[50] = {0};
    FILE *file = g_fopen("../abc.txt", "r+");
    g_fprintf(file, "%s", "你好世界!!!!!!!!!!!!!你好世界");
    g_fseek(file, 0l, 0);
    g_fread(str, sizeof(char), 50, file);
    g_printf(str);
    g_fclose(file);
    return 0;
}
