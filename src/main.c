#include <stdio.h>

#define SOKOL_IMPL
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>

static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
} state;

void init() {
  printf("void init\n");

  sg_setup(&(sg_desc){.environment = sglue_environment()});

  state.pass_action = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {1.0f, 0.1f, 0.1f, 1.0f}},
  };
}

void frame() {
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

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
