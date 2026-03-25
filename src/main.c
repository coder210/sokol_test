#define SOKOL_IMPL
//#define SOKOL_GLCORE
#define SOKOL_GLCORE

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include <stdio.h>

static struct {
        sg_pipeline pip;
        sg_bindings bind;
        sg_pass_action pass_action;
} state;

/* 顶点数据：位置 (x,y) + 颜色 (r,g,b,a) */
static const float vertices[] = {
        // 位置          // 颜色
         0.0f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,  // 上顶点 (红色)
        -0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f,  // 左下 (绿色)
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,  // 右下 (蓝色)
};


#ifdef SOKOL_GLCORE


/* 顶点着色器 GLSL (兼容 WebGL 和所有后端) */
static const char* vs_src =
"#version 100\n"
"attribute vec4 a_position;\n"
"attribute vec4 a_color;\n"
"varying vec4 v_color;\n"
"void main() {\n"
"    gl_Position = a_position;\n"
"    v_color = a_color;\n"
"}\n";

/* 片段着色器 GLSL */
static const char* fs_src =
"#version 100\n"
"precision mediump float;\n"
"varying vec4 v_color;\n"
"void main() {\n"
"    gl_FragColor = v_color;\n"
"}\n";


#endif // SOKOL_GLCORE


#ifdef SOKOL_D3D11
/* 顶点着色器 HLSL */
static const char* vs_src =
"struct VS_IN {\n"
"    float4 pos : POSITION;\n"
"    float4 color : COLOR;\n"
"};\n"
"struct VS_OUT {\n"
"    float4 pos : SV_POSITION;\n"
"    float4 color : COLOR;\n"
"};\n"
"VS_OUT main(VS_IN input) {\n"
"    VS_OUT output;\n"
"    output.pos = input.pos;\n"
"    output.color = input.color;\n"
"    return output;\n"
"}\n";

/* 片段着色器 HLSL */
static const char* fs_src =
"struct PS_IN {\n"
"    float4 pos : SV_POSITION;\n"
"    float4 color : COLOR;\n"
"};\n"
"float4 main(PS_IN input) : SV_Target {\n"
"    return input.color;\n"
"}\n";
#endif // SOKOL_D3D11


static int frame_count = 0;
static double last_time = 0.0;

/* 初始化回调 */
static void init(void) {
        printf("Window initialized\n");
        last_time = sapp_frame_duration();

        // 初始化 sokol_gfx
        sg_setup(&(sg_desc) {
                .environment = sglue_environment()   // 关键：传递 D3D11 设备环境
        });
        /* 创建着色器 */
        sg_shader_desc shd_desc = {
            .vertex_func = {
                .source = vs_src,
                .entry = "main",
            },
            .fragment_func = {
                .source = fs_src,
                .entry = "main",
            },
            .attrs = {
                [0] = {.glsl_name = "a_position", .hlsl_sem_name = "POSITION"},   // 对应顶点布局中的语义
                [1] = {.glsl_name = "a_color", .hlsl_sem_name = "COLOR"}
            }
        };

        sg_shader shd = sg_make_shader(&shd_desc);

        /* 创建顶点缓冲区 */
        sg_buffer_desc vbuf_desc = {
            .data = SG_RANGE(vertices),
            .label = "vertex-buffer"
        };
        sg_buffer vbuf = sg_make_buffer(&vbuf_desc);

        /* 绑定顶点缓冲区 */
        state.bind.vertex_buffers[0] = vbuf;

        /* 创建管线（顶点布局 + 着色器） */
        sg_pipeline_desc pip_desc = {
            .shader = shd,
            .layout = {
                .attrs = {
                    [0] = {.offset = 0, .format = SG_VERTEXFORMAT_FLOAT2},
                    [1] = {.offset = 8, .format = SG_VERTEXFORMAT_FLOAT4}
                }
            }
        };
        state.pip = sg_make_pipeline(&pip_desc);

        /* 设置清除颜色为深灰色 */
        state.pass_action.colors[0] = (sg_color_attachment_action){
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {0.2f, 0.2f, 0.2f, 1.0f}
        };
}

/* 每帧回调 */
static void frame(void) {
        // FPS 计数
        frame_count++;
        double now = sapp_frame_duration();
        if (now - last_time >= 1.0) {
                printf("FPS: %d\n", frame_count);
                frame_count = 0;
                last_time = now;
        }

        // 开始渲染
        sg_begin_pass(&(sg_pass) { .action = state.pass_action, .swapchain = sglue_swapchain() });
        sg_apply_pipeline(state.pip);
        sg_apply_bindings(&state.bind);
        sg_draw(0, 3, 1);   // 3个顶点（三角形）
        sg_end_pass();
        sg_commit();
}

/* 清理回调 */
static void cleanup(void) {
        sg_shutdown();
}

/* 事件回调（可选） */
static void event(const sapp_event* ev) {
        switch (ev->type) {
        case SAPP_EVENTTYPE_MOUSE_MOVE:
                printf("Mouse moved: x=%.1f, y=%.1f\n", ev->mouse_x, ev->mouse_y);
                break;
        case SAPP_EVENTTYPE_KEY_DOWN:
                printf("Key down: code=%d\n", ev->key_code);
                break;
        case SAPP_EVENTTYPE_KEY_UP:
                printf("Key up: code=%d\n", ev->key_code);
                break;
        default:
                break;
        }
}

/* sokol 主入口 */
sapp_desc sokol_main(int argc, char* argv[]) {
        return (sapp_desc) {
                .init_cb = init,
                        .frame_cb = frame,
                        .cleanup_cb = cleanup,
                        .event_cb = event,
                        .width = 800,
                        .height = 600,
                        .window_title = "Sokol Triangle + App",
                        .high_dpi = true,
        };
}