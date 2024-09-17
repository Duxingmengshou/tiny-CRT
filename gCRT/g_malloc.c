#include "gcrt.h"

#define ADDR_ADD(addr, offset) (((char *)(addr)) + offset)// 地址偏移量计算
#define HEADER_SIZE (sizeof(heap_header))// 堆头部大小

#include <Windows.h>


typedef struct heap_header {
    enum {
        HEAP_BLOCK_FREE = 0xABABABAB, // 空闲块的魔数
        HEAP_BLOCK_USED = 0xCDCDCDCD, // 占用快的魔数
    } type;//4B

    unsigned size; // 块的尺寸包括块的信息头，4B
    // 64位系统中指针的大小为64位，8B
    struct heap_header *next;
    struct heap_header *prev;
} heap_header;//24B

// 堆空间链表头
static heap_header *list_head = NULL;

// 向系统请求获取堆空间
int g_crt_init_heap() {
    void *base = NULL;
    heap_header *header = NULL;
    // 32MB heap size
    unsigned int heap_size = 1024 * 1024 * 32;

    // 以base为起点分配32MB的内存空间
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (base == NULL)
        return 0;

    header = (heap_header *) base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;

    list_head = header;
    // 分配失败
    if (header == NULL)
        return 0;
    return 1;
}

void *g_malloc(unsigned int size) {
    heap_header *header;

    if (size == 0)
        return NULL;

    header = list_head;
    while (header != NULL) {
        if (header->type == HEAP_BLOCK_USED) {
            header = header->next;
            continue;
        }

        // 刚好碰到一个空闲快，且其块的大小大于所需size加上一个信息头尺寸，但是小于所需size加上两个信息头尺寸，即剩余的内部碎片就算分离出来，也没有利用价值了，直接整个块都分配给used，等待整体释放
        if (header->size > size + HEADER_SIZE &&
            header->size <= size + HEADER_SIZE * 2) {
            g_printf("%s\n", "分配空间-直接分配块");
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        // 空闲块空间足够，且剩余的内部碎片分离出来还可以再使用
        if (header->size > size + HEADER_SIZE * 2) {
            // 分割空间
            g_printf("%s\n", "分配空间-分割块分配");
            heap_header *next = (heap_header *) ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size + HEADER_SIZE);
            if (header->next != NULL)
                header->next->prev = next;
            // 实际使用空间
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        };
        header = header->next;
    }
    return NULL;
}


void g_free(void *ptr) {
    // 块实际地址需要减去堆头部空间所占用地址
    heap_header *header = (heap_header *) ADDR_ADD(ptr, -HEADER_SIZE);
    if (header->type != HEAP_BLOCK_USED)
        return;

    header->type = HEAP_BLOCK_FREE;
    // 释放块的前一个块为空
    if (header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
        header->prev->next = header->next;
        if (header->next != NULL)
            header->next->prev = header->prev;
        header->prev->size += header->size;
        header = header->prev;
    }
    // 释放块的后一个块为空
    if (header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
        header->size += header->next->size;
        header->next = header->next->next;
    }
}


void *g_calloc(unsigned int nmemb, unsigned int size) {
    heap_header *header;

    size = nmemb * size;

    if (size == 0)
        return NULL;

    header = list_head;
    while (header != NULL) {
        if (header->type == HEAP_BLOCK_USED) {
            header = header->next;
            continue;
        }
        // 刚好碰到一个空闲快，且其块的大小大于所需size加上一个信息头尺寸，但是小于所需size加上两个信息头尺寸，即剩余的内部碎片就算分离出来，也没有利用价值了，直接整个块都分配给used，等待整体释放
        if (header->size > size + HEADER_SIZE &&
            header->size <= size + HEADER_SIZE * 2) {
            g_printf("%s\n", "分配空间-直接分配块");
            header->type = HEAP_BLOCK_USED;
            g_memset(ADDR_ADD(header, HEADER_SIZE), '\0', header->size - HEADER_SIZE);
            return ADDR_ADD(header, HEADER_SIZE);
        }
        // 空闲块空间足够，且剩余的内部碎片分离出来还可以再使用
        if (header->size > size + HEADER_SIZE * 2) {
            // 分割空间
            g_printf("%s\n", "分配空间-分割块分配");
            heap_header *next = (heap_header *) ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size + HEADER_SIZE);
            if (header->next != NULL)
                header->next->prev = next;
            // 实际使用空间
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;

            g_memset(ADDR_ADD(header, HEADER_SIZE), '\0', header->size - HEADER_SIZE);
            return ADDR_ADD(header, HEADER_SIZE);
        };
        header = header->next;
    }
    return NULL;
}