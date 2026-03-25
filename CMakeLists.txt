cmake_minimum_required(VERSION 3.15)
project(sokol_triangle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 源文件（注意：main.c 在 macOS 下会被当作 Objective-C 编译）
add_executable(triangle src/main.c src/sokol_app.h src/sokol_gfx.h)

# macOS：将 main.c 作为 Objective-C 编译，以支持 Cocoa 框架的语法
if(APPLE)
    set_source_files_properties(src/main.c PROPERTIES LANGUAGE OBJC)
endif()

# 平台特定编译定义和链接库
if(EMSCRIPTEN)
    # WebAssembly 构建：使用 OpenGL ES 3.0 (WebGL 2)
    target_compile_definitions(triangle PRIVATE SOKOL_GLES3)
    target_link_options(triangle PRIVATE
        -sUSE_GLFW=0           # 不使用 GLFW，sokol 自己管理窗口
        -sASYNCIFY             # 支持异步化，便于处理事件循环
        -sFULL_ES3             # 启用 WebGL 2
        -sALLOW_MEMORY_GROWTH=1
        # --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html   # 如有自定义 shell 文件，取消注释
    )
elseif(WIN32)
    # Windows 使用 OpenGL 核心
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        opengl32
        gdi32
        user32
        ole32
        shell32
    )
elseif(APPLE)
    # macOS 使用 OpenGL 核心 + Cocoa 框架
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
elseif(UNIX)
    # Linux 及其他 Unix-like 系统
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        GL
        X11
        dl
        pthread
    )
endif()