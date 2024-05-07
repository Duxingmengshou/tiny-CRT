#include "gcrt.h"

int main() {
    int *a = (int *) g_malloc(sizeof(int) * 1024);
    for (int i = 0; i < 1024; i++) {
        *(a + i) = i;
    }
    FILE *file = g_fopen("../abc.txt", "r+");
    while (*a != 0xabababab && *a != 0xcdcdcdcd) {
        g_fprintf(file,"%d\n", *a);
        g_printf("%d\n", *a);
        a++;
    }
    g_fclose(file);
    g_free(a);

    return 0;
}
