cmake_minimum_required(VERSION 3.15)
project(sokol_triangle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 保持您原有的 add_executable，包含头文件
add_executable(triangle src/main.c src/sokol_app.h src/sokol_gfx.h)

# 将头文件标记为“仅头文件”，禁止编译
set_source_files_properties(src/sokol_app.h PROPERTIES HEADER_FILE_ONLY ON)
set_source_files_properties(src/sokol_gfx.h PROPERTIES HEADER_FILE_ONLY ON)

# 可选：确保 main.c 使用 C 语言（默认已是 C）
set_source_files_properties(src/main.c PROPERTIES LANGUAGE C)

if(APPLE)
    # 您原有的代码中曾将 main.c 设为 OBJC，但这里不再需要，我们保持 C
    # 如果确实需要 OBJC（例如使用了 Objective-C 语法），可取消注释，但 Sokol 无需
    # set_source_files_properties(src/main.c PROPERTIES LANGUAGE OBJC)
endif()

# 添加头文件搜索路径（假设 sokol 头文件就在 src/ 目录）
target_include_directories(triangle PRIVATE src)

if(EMSCRIPTEN)
    target_compile_definitions(triangle PRIVATE SOKOL_GLES3)
    target_link_options(triangle PRIVATE
        -sUSE_GLFW=0
        -sASYNCIFY
        -sFULL_ES3
        -sALLOW_MEMORY_GROWTH=1
        # --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html
    )
elseif(WIN32)
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        opengl32
        gdi32
        user32
        ole32
        shell32
    )
elseif(APPLE)
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
elseif(UNIX)
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE GL_GLEXT_PROTOTYPES)
    target_link_libraries(triangle
        GL
        X11
        Xi
        Xcursor
        Xrandr
        Xinerama
        Xfixes
        dl
        pthread
        m
    )
endif()