cmake_minimum_required(VERSION 3.15)
project(sokol_triangle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(triangle src/main.c src/sokol_app.h src/sokol_gfx.h)

if(APPLE)
    set_source_files_properties(src/main.c PROPERTIES LANGUAGE OBJC)
endif()

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