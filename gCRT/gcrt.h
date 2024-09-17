#ifndef __G_CRT_H__
#define __G_CRT_H__
#endif
// NULL定义
#ifndef NULL
#define NULL ((void *)0)
#endif

// IO宏定义
typedef int FILE;
#define EOF (-1)
// FILE* 这个类型在Windows下实际上是内核句柄，要通过GetStdHandle的Windows API获得
#define stdin ((FILE *)(GetStdHandle(STD_INPUT_HANDLE)))
#define stdout ((FILE *)(GetStdHandle(STD_OUTPUT_HANDLE)))
#define stderr ((FILE *)(GetStdHandle(STD_ERROR_HANDLE)))

/*定义C++相关的函数，以使得函数的兼容性得到满足，但是依旧按照C的修饰规则来进行导出函数名修饰，即使用extern "C"*/
#ifdef __cplusplus
extern "C"
{
#endif

void _RTC_Shutdown();

void _RTC_InitBase();

void _RTC_CheckStackVars();

// 内存管理
void *g_memset(void *s, char c, unsigned int n);

// 内存分配
void g_free(void *ptr);

void *g_malloc(unsigned size);

void *g_calloc(unsigned int nmemb, unsigned int size);

int g_crt_init_heap();

// 字符串
char *g_reverse(char *s, unsigned int size);

char *g_itoa(int n, char *str, int radix);

int g_atoi(const char *nptr);

unsigned int g_strlen(const char *sr);

int g_strcmp(const char *src, const char *dst);

char *g_strcpy(char *dest, const char *src);

// IO
unsigned int g_fputc(unsigned int c, FILE *stream);

unsigned int g_fputs(const char *str, FILE *stream);

int g_fgetc(FILE *stream);

int g_fgetwc(FILE *stream);

unsigned int g_printf(const char *format, ...);

unsigned int g_fprintf(FILE *stream, const char *format, ...);

unsigned int g_vfprintf(FILE *stream, const char *format, char *arglist);

FILE *g_fopen(const char *filename, const char *mode);

int g_fread(const void *buffer, unsigned int size, unsigned int count, FILE *stream);

int g_fwrite(const void *buffer, unsigned int size, unsigned int count, FILE *stream);

int g_fclose(FILE *fp);

int g_fseek(FILE *fp, int offset, int set);

// ctype
// 检查字符是否为字母或数字
int g_isalnum(int c);

// 检查字符是否为字母
int g_isalpha(int c);

// 检查字符是否为 ASCII 字符
int g_isascii(int c);

// 检查字符是否为控制字符
int g_iscntrl(int c);

// 检查字符是否为数字
int g_isdigit(int c);

// 检查字符是否为可打印字符（不包括空格）
int g_isgraph(int c);

// 检查字符是否为小写字母
int g_islower(int c);

// 检查字符是否为可打印字符（包括空格）
int g_isprint(int c);

// 检查字符是否为空白字符
int g_isspace(int c);

// 检查字符是否为标点符号
int g_ispunct(int c);

// 检查字符是否为大写字母
int g_isupper(int c);

// 检查字符是否为十六进制数字
int g_isxdigit(int c);

#ifdef __cplusplus
}
#endif
