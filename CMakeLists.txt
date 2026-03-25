cmake_minimum_required(VERSION 3.15)
project(sokol_triangle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 源文件
add_executable(triangle src/main.c src/sokol_app.h  src/sokol_gfx.h)


# 平台特定链接库和编译定义
if(EMSCRIPTEN)
    # WebAssembly 构建
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_options(triangle PRIVATE
        -sUSE_GLFW=0
        -sASYNCIFY
        -sFULL_ES2
        -sALLOW_MEMORY_GROWTH=1
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html  # 可选，若无此文件请删除
    )
elseif(WIN32)
    # Windows 使用 OpenGL 后端（如需 D3D11 可添加选项）
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
    target_compile_definitions(triangle PRIVATE SOKOL_GLCORE)
    target_link_libraries(triangle
        GL
        X11
        dl
        pthread
    )
endif()