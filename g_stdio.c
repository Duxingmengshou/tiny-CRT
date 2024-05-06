#include <windows.h>
#include "gcrt.h"

// 输出字符到文件
unsigned int g_fputc(unsigned int c, FILE *stream) {
    if (g_fwrite(&c, 1, 1, stream) != 1)
        return EOF;
    else
        return c;
}

// 输出字符串到文件
unsigned int g_fputs(const char *str, FILE *stream) {
    unsigned int len = g_strlen(str);
    if (g_fwrite(str, sizeof(char), len, stream) != len)
        return EOF;
    else
        return len;
}

// 格式化输出到命令行
unsigned int g_printf(const char *format, ...) {
    va_list(arglist);
            va_start(arglist, format);
    return g_vfprintf(stdout, format, arglist);
}

// 格式化输出到文件
unsigned int g_fprintf(FILE *stream, const char *format, ...) {
    va_list(arglist);
            va_start(arglist, format);
    return g_vfprintf(stream, format, arglist);
}

// 格式化输出到文件的base函数
unsigned int g_vfprintf(FILE *stream, const char *format, char *arglist) {

    int translating = 0;
    unsigned int ret = 0; // 记录最终输出的字符个数
    const char *p = 0;

    for (p = format; *p && *p != '\0'; ++p) {
        switch (*p) {
            case '%':
                if (!translating)
                    translating = 1; // translating置为1，代表后面的字符需要解析
                else {
                    if (g_fputc('%', stream) < 0)
                        return EOF;
                    ++ret;
                    translating = 0;
                }
                break;
            case 'd':
                if (translating) //%d
                {
                    char buf[32] = {0};
                    translating = 0;
                    g_itoa(va_arg(arglist, int), buf, 10);
                    if (g_fputs(buf, stream) < 0)
                        return EOF;
                    ret += g_strlen(buf);
                    g_free(buf);
                } else if (g_fputc('d', stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
            case 's':
                if (translating) //%s
                {
                    const char *str = va_arg(arglist, const char *);
                    translating = 0;
                    if (g_fputs(str, stream) < 0)
                        return EOF;
                    ret += g_strlen(str);
                } else if (g_fputc('s', stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
            default:
                if (translating)
                    translating = 0;
                if (g_fputc(*p, stream) < 0)
                    return EOF;
                else
                    ++ret;
                break;
        }
    }
    return ret;
}

/*
Windows API的ReadFILE()
BOOL ReadFile(
    HANDLE  hFile,//hFile为要读取的文件句柄，对应的是fread函数中stream参数
    LPVOID  lpBuffer,//lpBuffer是存储缓冲区的其实地址，对应fread函数中的buffer
    DWORD  nNumberofBytesToRead,//nNumberofBytesToRead代表要读取字节总数，等于fread函数中count * elementSize
    LPDWORD  lpNumberofBytesRead,//lpNumberOfBytesRead代表一个指向DWORD类型的指针，用来表示读取了多少个字节
    LPOVERLAPPED  lpOverlapped//lpOverlapped没用
);

Windows API的SetFilePointer(
    HANDLE  hFile,//hFile为要读取的文件句柄，对应的是fread函数中stream参数
    LONG  IDistanceToMove, //偏移量（低位），指向64位偏移量的低32位
    PLONG lpDistanceToMoveHigh, //偏移量（高位），指向64位偏移量的高32位
    DWORD dwMoveMethod//基准位置,文件开始位置FILE_BEGIN/文件当前位置FILE_CURRENT/文件结束位置FILE_END
);
*/

// 打开文件，w表示创建文件，w只写，w+可写可读，r表示读取文件，r表示只读，r+表示可写可读
FILE *g_fopen(const char *filename, const char *mode) {
    HANDLE hFile;
    DWORD access = 0;
    DWORD creation = 0;
    if (g_strcmp(mode, "w") == 0) {
        access |= GENERIC_WRITE;
        creation |= CREATE_ALWAYS;
    }

    if (g_strcmp(mode, "w+") == 0) {
        access |= GENERIC_WRITE | GENERIC_READ;
        creation |= CREATE_ALWAYS;
    }

    if (g_strcmp(mode, "r") == 0) {
        access |= GENERIC_READ;
        creation |= OPEN_EXISTING;
    }

    if (g_strcmp(mode, "r+") == 0) {
        access |= GENERIC_WRITE | GENERIC_READ;
        creation |= OPEN_EXISTING;
    }

    hFile = CreateFileA(filename, // 文件路径
                        access,                    // 访问模式，此处为读取模式
                        0,                               // 共享模式，0 表示独占访问
                        NULL,                            // 安全描述符，使用默认值 NULL
                        creation,                   // 打开已存在的文件
                        FILE_ATTRIBUTE_NORMAL,           // 文件属性，此处为普通文件
                        NULL                             // 模板文件句柄，不使用模板文件
    );
    if (hFile == INVALID_HANDLE_VALUE)
        return 0;
    return (FILE *) hFile;
}

// 关闭文件
int g_fclose(FILE *fp) {
    return CloseHandle((HANDLE) fp);
}

// 定位文件的输入位置
int g_fseek(FILE *fp, int offset, int set) {
    return SetFilePointer((HANDLE) fp, offset, 0, set);
}

// 写入文件
int g_fwrite(const void *buffer, unsigned int size, unsigned int count, FILE *stream) {
    int written = 0;
    if (!WriteFile((HANDLE) stream, buffer, size * count, (LPDWORD) &written, 0))
        return 0;
    return written;
}

// 读取文件
int g_fread(void *buffer, int size, int count, FILE *stream) {
    int read = 0;
    BOOL success = ReadFile(
            stream,                        // 文件句柄
            buffer,                      // 缓冲区指针
            size * count,    // 缓冲区大小
            (LPDWORD) &read,  // 用于存储实际读取的字节数
            NULL                     // 用于重叠操作，一般为 NULL
    );
    if (!success)
        return 0;
    return read;
}
