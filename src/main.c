#define SOKOL_IMPL

#include <sokol_app.h>
#include <stdio.h>

void init() {}

void frame() {}

void cleanup() {}

void event(const sapp_event *ev) {}

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
