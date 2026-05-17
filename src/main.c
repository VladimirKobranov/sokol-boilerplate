#define SOKOL_IMPL

#include <sokol_app.h>
#include <stdio.h>

void init() {}

void frame() {}

void cleanup() {}

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
