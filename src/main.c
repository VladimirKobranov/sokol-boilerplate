#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// sokol
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
// sokol debugger
#include "sokol_debugtext.h"
// glb import
#include "cgltf.h"
// matrix
#include <HandmadeMath.h>
#define mat4 HMM_Mat4
// shader
#include <shader_glsl.h>
// images
#include "stb_image.h"

/**
 * @brief Sokol struct
 */
static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
  sg_buffer vbuf;
  sg_buffer ibuf;
  sg_image tex;
  sg_view tex_view;
  sg_sampler smp;
  sg_index_type index_type;
} state;

/**
 * @brief Loaded file struct
 * Store all glb\gltf file information
 */
static struct {
  char name[64];     ///< Name of the model
  int num_indices;   ///< Number of indices
  int num_vertices;  ///< Number of vertices
  int num_triangles; ///< Number of triangles (calculated from num_vertices / 3)
} model;

static struct {
  char name[64];
  char mime_type[32];
  int width;
  int height;
  int channels;
  int decoded_channels;
  int encoded_size;
  int embedded;
} texture;

typedef struct {
  float position[3];
  float texcoord0[2];
} vertex_t;

/**
 * @brief Loads a 3D model from a file.
 *
 * @param filename Path to the .glb/.gltf file.
 *
 * @return 0 on success, otherwise an error code.
 */
void loadFile() {
  cgltf_options options = {0};
  cgltf_data *data = NULL;
  cgltf_parse_file(&options, "./scene.glb", &data);
  cgltf_load_buffers(&options, data, "./scene.glb");

  cgltf_primitive *prim = &data->meshes[0].primitives[0];
  snprintf(model.name, sizeof(model.name), "%s",
           data->meshes[0].name ? data->meshes[0].name : "unnamed");

  cgltf_accessor *pos = NULL;
  cgltf_accessor *texcoord0 = NULL;
  for (cgltf_size i = 0; i < prim->attributes_count; i++) {
    if (prim->attributes[i].type == cgltf_attribute_type_position) {
      pos = prim->attributes[i].data;
    }
    if (prim->attributes[i].type == cgltf_attribute_type_texcoord &&
        prim->attributes[i].index == 0) {
      texcoord0 = prim->attributes[i].data;
    }
  }

  vertex_t *verts = malloc(pos->count * sizeof(vertex_t));
  for (cgltf_size i = 0; i < pos->count; i++) {
    cgltf_accessor_read_float(pos, i, verts[i].position, 3);
    cgltf_accessor_read_float(texcoord0, i, verts[i].texcoord0, 2);
  }

  state.vbuf = sg_make_buffer(&(sg_buffer_desc){
      .size = pos->count * sizeof(vertex_t),
      .data = {.ptr = verts, .size = pos->count * sizeof(vertex_t)},
  });
  state.bind.vertex_buffers[0] = state.vbuf;
  free(verts);

  cgltf_texture_view *base_color =
      &prim->material->pbr_metallic_roughness.base_color_texture;
  cgltf_image *image = base_color->texture->image;
  cgltf_buffer_view *image_view = image->buffer_view;
  unsigned char *encoded =
      (unsigned char *)image_view->buffer->data + image_view->offset;

  int width = 0;
  int height = 0;
  int channels = 0;
  unsigned char *pixels = stbi_load_from_memory(encoded, (int)image_view->size,
                                                &width, &height, &channels, 4);
  printf("texture: %dx%d, channels: %d\n", width, height, channels);

  snprintf(texture.name, sizeof(texture.name), "%s",
           image->name ? image->name : "unnamed");
  snprintf(texture.mime_type, sizeof(texture.mime_type), "%s",
           image->mime_type ? image->mime_type : "unknown");
  texture.width = width;
  texture.height = height;
  texture.channels = channels;
  texture.decoded_channels = 4;
  texture.encoded_size = (int)image_view->size;
  texture.embedded = image->buffer_view != NULL;

  state.tex = sg_make_image(&(sg_image_desc){
      .width = width,
      .height = height,
      .pixel_format = SG_PIXELFORMAT_RGBA8,
      .data.mip_levels[0] = {.ptr = pixels,
                             .size = (size_t)(width * height * 4)},
  });
  state.tex_view = sg_make_view(&(sg_view_desc){.texture.image = state.tex});
  state.smp = sg_make_sampler(&(sg_sampler_desc){
      .min_filter = SG_FILTER_LINEAR,
      .mag_filter = SG_FILTER_LINEAR,
      .wrap_u = SG_WRAP_REPEAT,
      .wrap_v = SG_WRAP_REPEAT,
  });
  state.bind.views[VIEW_base_color_tex] = state.tex_view;
  state.bind.samplers[SMP_base_color_smp] = state.smp;
  stbi_image_free(pixels);

  cgltf_accessor *idx = prim->indices;
  unsigned char *ibase = (unsigned char *)idx->buffer_view->buffer->data;
  void *idx_data = ibase + idx->buffer_view->offset + idx->offset;
  cgltf_bool is_uint16 = idx->component_type == cgltf_component_type_r_16u;
  model.num_indices = (int)idx->count;

  model.num_vertices = (int)pos->count;
  model.num_indices = (int)idx->count;
  model.num_triangles = model.num_indices / 3;

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
  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = shd,
      .layout = {.buffers[0].stride = sizeof(vertex_t),
                 .attrs = {[ATTR_triangle_position] =
                               {.format = SG_VERTEXFORMAT_FLOAT3,
                                .offset = offsetof(vertex_t, position)},
                           [ATTR_triangle_texcoord0] =
                               {.format = SG_VERTEXFORMAT_FLOAT2,
                                .offset = offsetof(vertex_t, texcoord0)}}},
      .index_type = state.index_type,
      .face_winding = SG_FACEWINDING_CCW,
      .depth = {.compare = SG_COMPAREFUNC_LESS_EQUAL, .write_enabled = true},
      .label = "cube-pipeline"});

  state.pass_action = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {0.1f, 0.1f, 0.1f, 1.0f}},
      .depth = {.load_action = SG_LOADACTION_CLEAR, .clear_value = 1.0f},
  };

  // debugger
  sdtx_setup(&(sdtx_desc_t){
      .fonts[0] = sdtx_font_kc853(),
  });
}

/**
 * @brief Main frame function
 * Works each frame
 *
 */
void frame() {
  float angle = (float)sapp_frame_count() * 0.01f;
  float aspect = (float)sapp_width() / (float)sapp_height();

  mat4 model_matrix = HMM_Rotate_RH(angle, HMM_V3(0.25, 0.5, 0));
  mat4 view = HMM_LookAt_RH(HMM_V3(7, 0, 0), HMM_V3(0, 0, 0), HMM_V3(0, 1, 0));
  // ZO = Zero to One; clipping
  mat4 proj = HMM_Perspective_RH_ZO(HMM_AngleDeg(50.0f), aspect, 0.1f, 100.0f);
  mat4 mv = HMM_MulM4(view, model_matrix);
  mat4 mvp = HMM_MulM4(proj, mv);
  vs_params_t vs_params = {.mvp = mvp, .mv = mv};

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);
  sg_range uniform_range = {.ptr = &vs_params, .size = sizeof(vs_params)};
  sg_apply_uniforms(UB_vs_params, &uniform_range);
  sg_draw(0, model.num_indices, 1);

  sdtx_canvas(sapp_widthf() * 0.5f, sapp_heightf() * 0.5f);
  sdtx_origin(1.0f, 1.0f);
  sdtx_color3f(1.0f, 1.0f, 1.0f);
  sdtx_puts("logs:\n");
  sdtx_printf("model: %s\n", model.name);
  sdtx_printf("indices: %d\n", model.num_indices);
  sdtx_printf("vertices: %i\n", model.num_vertices);
  sdtx_printf("triangles: %i\n", model.num_triangles);
  sdtx_printf("texture: %s\n", texture.name);
  sdtx_printf("texture mime: %s\n", texture.mime_type);
  sdtx_printf("texture size: %dx%dpx\n", texture.width, texture.height);
  sdtx_printf("texture channels: %d -> %d\n", texture.channels,
              texture.decoded_channels);
  sdtx_printf("texture encoded: %d bytes\n", texture.encoded_size);
  sdtx_printf("texture embedded: %s\n", texture.embedded ? "yes" : "no");

  sdtx_draw();
  sg_end_pass();
  sg_commit();
}

/**
 * @brief Cleanup of resources
 *
 */
void cleanup() {
  sdtx_shutdown();
  sg_destroy_buffer(state.vbuf);
  sg_destroy_buffer(state.ibuf);
  sg_destroy_sampler(state.smp);
  sg_destroy_view(state.tex_view);
  sg_destroy_image(state.tex);
  sg_shutdown();
}

/**
 * @brief Sokol events handler
 *
 */
void event(const sapp_event *ev) {
  if (ev->type == SAPP_EVENTTYPE_KEY_DOWN &&
      ev->key_code == SAPP_KEYCODE_ESCAPE)
    sapp_quit();
}

/**
 * @brief Sokol config function
 *
 */
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
