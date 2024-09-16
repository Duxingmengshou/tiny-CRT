cmake_minimum_required(VERSION 3.26)
project(CRT C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(
        gCRT
)

add_executable(${PROJECT_NAME}
        main.c
)

# 链接 gcrt.lib 和 kernel32.lib
target_link_libraries(${PROJECT_NAME} gCRT kernel32)
# 指定链接参数
#/NODEFAULTLIB: 这个选项告诉链接器不要自动链接默认的库。这里用来禁止使用默认的CRT
#/ENTRY:g_crt_entry: 这个选项指定程序的入口点。默认情况下，MSVC 会使用 main 函数作为程序的入口点，/ENTRY:g_crt_entry 则告诉链接器使用 g_crt_entry 作为程序的起始执行点。
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB /ENTRY:g_crt_entry")