cmake_minimum_required(VERSION 3.26)
project(CRT C)

set(CMAKE_C_STANDARD 11)

# 设置源文件
set(SOURCES
        g_entry.c
        g_stdio.c
        g_string.c
        g_malloc.c
)

# 编译源文件生成目标文件
foreach (source_file ${SOURCES})
    # 获取文件名（不包含扩展名）
    get_filename_component(file_name ${source_file} NAME_WE)
    # 生成目标文件的命令，使用 cl 编译器
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.obj
            COMMAND cl /c /DWIN32 /GS- ${CMAKE_CURRENT_SOURCE_DIR}/${source_file} /Fo${CMAKE_CURRENT_BINARY_DIR}/${file_name}.obj
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${source_file}
    )
    list(APPEND OBJECT_FILES ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.obj)
endforeach ()


# 将目标文件链接成静态库
add_library(gcrt STATIC ${OBJECT_FILES})
# 指定链接器
set(LINKER "link")
# 指定连接器语言为C
set_target_properties(gcrt PROPERTIES LINKER_LANGUAGE C)

# 添加编译目标：生成 main.c 的目标文件 main.obj
add_library(CRT_obj OBJECT main.c)
# 添加可执行文件 target，将 main.obj 链接到该可执行文件中
add_executable(CRT $<TARGET_OBJECTS:CRT_obj>)
# 链接 gcrt.lib 和 kernel32.lib
target_link_libraries(CRT gcrt kernel32)
# 指定链接参数
set_target_properties(CRT PROPERTIES LINK_FLAGS "/NODEFAULTLIB /entry:g_crt_entry")

# 利用一个假的目标启动Clion的代码审查功能
add_executable(fake
        gcrt.h
        g_malloc.c
        g_string.c
        g_stdio.c
        g_entry.c
        main.c
)