#include "gcrt.h"

int main() {
//    g_crt_init_heap();
    int *a = (int *) g_malloc(sizeof(int) * 1024);
    for (int i = 0; i < 1024; i++) {
        *(a + i) = i;
    }
    while (*a != 0xabababab && *a != 0xcdcdcdcd) {
        g_printf("%d\n", *a);
        a++;
    }
    FILE *
    g_free(a);

    return 0;
}
