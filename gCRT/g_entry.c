#include "gcrt.h"
#include <Windows.h>

extern int main(int argc, char *argv[]); // 申明程序员编写的程序中的main函数
void g_exit(int);

static void crt_fatal_error(const char *msg) {
    g_printf("error:%s\n", msg);
    g_exit(1);
}

static void crt_fatal_info(const char *msg) {
    g_printf("info:%s\n", msg);
}

void g_crt_entry(void) {
    int ret;
    int flag = 0;
    int argc = 0;
    char *argv[16]; // 最多16个参数

    int sequentialSpaceNum = 0;   // 连续空格数目
    char *cl = GetCommandLineA(); // 对于windows系统提供GetCommandLine这个API来返回整体命令行参数字符串

    // 解析命令行
    argv[0] = cl;
    argc++;
    while (*cl) {
        if (*cl == '\"') { // 在字符串中的空格是有效的字符，所以需要设计标识位
            if (flag == 0)
                flag = 1;
            else
                flag = 0;
        }
            // 如果不是在字符串期间遇到空格，则需要剔除空格
        else if (*cl == ' ' && flag == 0) {
            sequentialSpaceNum = 1; // 空格数目为1

            while (*(cl + sequentialSpaceNum) == ' ') {
                sequentialSpaceNum++;
            }
            if (*(cl + sequentialSpaceNum)) {
                argv[argc] = cl + sequentialSpaceNum;
                argc++;
            }
            *cl = '\0';
            cl = cl + sequentialSpaceNum;
        }
        cl++;
    }
    if (!g_crt_init_heap())
        crt_fatal_error("heap initialize failed");
    crt_fatal_info("heap initialize ok");
    ret = main(argc, argv);
    g_exit(ret);
}

void g_exit(int exitCode) {
    ExitProcess(exitCode);
}