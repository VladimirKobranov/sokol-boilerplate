#include <stdio.h>

// sokol
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
// glb import
#include "cgltf.h"
// matrix
#include <HandmadeMath.h>
// shader
#include <shader_glsl.h>

static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
  sg_buffer vbuf;
  sg_buffer ibuf;
  int num_indices;
  sg_index_type index_type;
} state;

void loadFile() {
  cgltf_options options = {0};
  cgltf_data *data = NULL;
  cgltf_parse_file(&options, "./scene.glb", &data);
  cgltf_load_buffers(&options, data, "./scene.glb");

  cgltf_primitive *prim = &data->meshes[0].primitives[0];
  cgltf_accessor *pos = NULL;
  for (cgltf_size i = 0; i < prim->attributes_count; i++)
    if (prim->attributes[i].type == cgltf_attribute_type_position)
      pos = prim->attributes[i].data;

  unsigned char *base = (unsigned char *)pos->buffer_view->buffer->data;
  float *verts = (float *)(base + pos->buffer_view->offset + pos->offset);
  state.vbuf = sg_make_buffer(&(sg_buffer_desc){
      .size = pos->count * pos->stride,
      .data = {.ptr = verts, .size = pos->count * pos->stride},
  });
  state.bind.vertex_buffers[0] = state.vbuf;

  cgltf_accessor *idx = prim->indices;
  unsigned char *ibase = (unsigned char *)idx->buffer_view->buffer->data;
  void *idx_data = ibase + idx->buffer_view->offset + idx->offset;
  cgltf_bool is_uint16 = idx->component_type == cgltf_component_type_r_16u;
  state.num_indices = (int)idx->count;
  state.index_type = is_uint16 ? SG_INDEXTYPE_UINT16 : SG_INDEXTYPE_UINT32;
  state.ibuf = sg_make_buffer(&(sg_buffer_desc){
      .usage = {.index_buffer = true},
      .size = idx->count * (is_uint16 ? 2 : 4),
      .data = {.ptr = idx_data, .size = idx->count * (is_uint16 ? 2 : 4)},
  });
  state.bind.index_buffer = state.ibuf;

  cgltf_free(data);
}

void init() {
  sg_setup(&(sg_desc){.environment = sglue_environment()});
  loadFile();

  sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));
  state.pip = sg_make_pipeline(
      &(sg_pipeline_desc){.shader = shd,
                          .layout = {.attrs = {[ATTR_triangle_position].format =
                                                   SG_VERTEXFORMAT_FLOAT3}},
                          .index_type = state.index_type,
                          .face_winding = SG_FACEWINDING_CCW,
                          .depth = {.compare = SG_COMPAREFUNC_LESS_EQUAL,
                                    .write_enabled = true},
                          .label = "cube-pipeline"});

  state.pass_action = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {0.1f, 0.1f, 0.1f, 1.0f}},
      .depth = {.load_action = SG_LOADACTION_CLEAR, .clear_value = 1.0f},
  };
}

void frame() {
  float angle = (float)sapp_frame_count() * 0.01f;
  float aspect = (float)sapp_width() / (float)sapp_height();

  HMM_Mat4 model = HMM_Rotate_RH(angle, HMM_V3(0.25, 0.5, 0));
  HMM_Mat4 view =
      HMM_LookAt_RH(HMM_V3(7, 0, 0), HMM_V3(0, 0, 0), HMM_V3(0, 1, 0));
  // ZO = Zero to One; clipping
  HMM_Mat4 proj =
      HMM_Perspective_RH_ZO(HMM_AngleDeg(50.0f), aspect, 0.1f, 100.0f);
  HMM_Mat4 mv = HMM_MulM4(view, model);
  HMM_Mat4 mvp = HMM_MulM4(proj, mv);
  vs_params_t vs_params = {.mvp = mvp, .mv = mv};

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);
  sg_range uniform_range = {.ptr = &vs_params, .size = sizeof(vs_params)};
  sg_apply_uniforms(UB_vs_params, &uniform_range);
  sg_draw(0, state.num_indices, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup() {
  sg_destroy_buffer(state.vbuf);
  sg_destroy_buffer(state.ibuf);
  sg_shutdown();
}

void event(const sapp_event *ev) {
  if (ev->type == SAPP_EVENTTYPE_KEY_DOWN &&
      ev->key_code == SAPP_KEYCODE_ESCAPE)
    sapp_quit();
}

sapp_desc sokol_main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return (sapp_desc){
      .width = 800,
      .height = 600,
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
  };
}
