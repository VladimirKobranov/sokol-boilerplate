#include <stdio.h>

// deps
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>

// shader
#include <shader_glsl.h>

static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
} state;

void init() {
  printf("void init\n");

  sg_setup(&(sg_desc){.environment = sglue_environment()});

  float vertices[] = {0.0f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
                      0.5f,  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f};

  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(vertices), .label = "triangle_verices"});

  sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));

  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = shd,
      .layout =
          {
              .attrs = {[ATTR_triangle_position].format =
                            SG_VERTEXFORMAT_FLOAT3,
                        [ATTR_triangle_color0].format = SG_VERTEXFORMAT_FLOAT4},
          },
      .label = "triangle-pipeline"});

  state.pass_action = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {0.1f, 0.1f, 0.1f, 1.0f}},
  };
}

void frame() {
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);
  sg_draw(0, 3, 1);

  sg_end_pass();
  sg_commit();
}

void cleanup() { sg_shutdown(); }

void event(const sapp_event *ev) {
  if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    printf("key %d pressed\n", ev->key_code);
    if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
      printf("App quit\n");
      sapp_quit();
    }
    if (ev->key_code == SAPP_KEYCODE_ENTER) {
      printf("key enter pressed\n");
    }
  }
}

sapp_desc sokol_main(int argc, char *argv[]) {
  printf("Hello sokol! \n");

  return (sapp_desc){
      .width = 640,
      .height = 480,
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
  };
}
