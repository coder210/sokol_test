cmake_minimum_required(VERSION 3.15)
project(sokol_triangle C)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 包含 sokol 头文件目录
include_directories(third_party/sokol)

# 定义可执行文件
add_executable(triangle src/main.c src/sokol_app.h  src/sokol_gfx.h)

# 平台特定链接库
if(EMSCRIPTEN)
    # WebAssembly 构建
    target_link_options(triangle PRIVATE
        -sUSE_GLFW=0          # 避免与 GLFW 冲突
        -sASYNCIFY            # 允许异步操作
        -sFULL_ES2            # 启用完整的 OpenGL ES 2.0 支持
        -sALLOW_MEMORY_GROWTH=1
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html  # 可选：自定义 HTML 模板
    )
    # 注意：如果使用 Emscripten，需要设置工具链文件：
    #   emcmake cmake -B build
    #   cmake --build build
elseif(WIN32)
    target_link_libraries(triangle
        opengl32
        gdi32
        user32
        ole32
        shell32
    )
elseif(APPLE)
    target_link_libraries(triangle
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
elseif(UNIX)
    target_link_libraries(triangle
        GL
        X11
        dl
        pthread
    )
endif()