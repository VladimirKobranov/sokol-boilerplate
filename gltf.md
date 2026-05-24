# `cgltf_data` Structure (glTF 2.0)

Complete description of all types from `lib/cgltf.h` (v1.15)

---

## `cgltf_data` — root object

```
cgltf_data
├── file_type                    (cgltf_file_type) — glTF / GLB / invalid
├── file_data                    (void*) — raw file data
├── file_size                    (cgltf_size)
│
├── asset                        (cgltf_asset)
│   ├── copyright                (char*)
│   ├── generator                (char*)
│   ├── version                  (char*)
│   ├── min_version              (char*)
│   ├── extras                   (cgltf_extras)
│   ├── extensions_count         (cgltf_size)
│   └── extensions[]             (cgltf_extension*)
│
├── scenes[] / scenes_count      (cgltf_scene* / cgltf_size)
│   └── cgltf_scene
│       ├── name                 (char*)
│       ├── nodes[] / count      (cgltf_node** / cgltf_size)
│       ├── extras
│       └── extensions[] / count
│
├── scene                        (cgltf_scene*) — default scene
│
├── nodes[] / nodes_count        (cgltf_node* / cgltf_size)
│   └── cgltf_node
│       ├── name                 (char*)
│       ├── parent               (cgltf_node*)
│       ├── children[] / count   (cgltf_node** / cgltf_size)
│       ├── skin                 (cgltf_skin*)
│       ├── mesh                 (cgltf_mesh*)
│       ├── camera               (cgltf_camera*)
│       ├── light                (cgltf_light*)
│       ├── weights[] / count    (cgltf_float* / cgltf_size)
│       ├── has_translation      (cgltf_bool)
│       ├── translation[3]       (cgltf_float)
│       ├── has_rotation
│       ├── rotation[4]          (cgltf_float)
│       ├── has_scale
│       ├── scale[3]             (cgltf_float)
│       ├── has_matrix
│       ├── matrix[16]           (cgltf_float)
│       ├── extras
│       ├── has_mesh_gpu_instancing
│       ├── mesh_gpu_instancing  (cgltf_mesh_gpu_instancing)
│       │   ├── attributes[] / count
│       │   └── attributes_count
│       ├── extensions[] / count
│       └── extras
│
├── meshes[] / meshes_count      (cgltf_mesh* / cgltf_size)
│   └── cgltf_mesh
│       ├── name                 (char*)
│       ├── primitives[] / count (cgltf_primitive* / cgltf_size)
│       │   └── cgltf_primitive
│       │       ├── type         (cgltf_primitive_type)
│       │       │   triangles | triangle_strip | triangle_fan | ...
│       │       ├── indices      (cgltf_accessor*) — may be NULL
│       │       ├── material     (cgltf_material*)
│       │       ├── attributes[] / count
│       │       │   └── cgltf_attribute
│       │       │       ├── name  (char*) — "POSITION", "NORMAL", "TEXCOORD_0", etc.
│       │       │       ├── type  (cgltf_attribute_type)
│       │       │       │   position | normal | tangent | texcoord
│       │       │       │   | color | joints | weights | custom
│       │       │       ├── index (cgltf_int)
│       │       │       └── data  (cgltf_accessor*)
│       │       ├── targets[] / count     (cgltf_morph_target*)
│       │       │   └── cgltf_morph_target
│       │       │       ├── attributes[] / count
│       │       ├── extras
│       │       ├── has_draco_mesh_compression
│       │       ├── draco_mesh_compression
│       │       ├── mappings[] / count     (cgltf_material_mapping*)
│       │       └── extensions[] / count
│       ├── weights[] / count    (cgltf_float* / cgltf_size)
│       ├── target_names[] / count (char** / cgltf_size)
│       ├── extras
│       └── extensions[] / count
│
├── accessors[] / accessors_count  (cgltf_accessor* / cgltf_size)
│   └── cgltf_accessor
│       ├── name                 (char*)
│       ├── component_type       (cgltf_component_type)
│       │   r_8 | r_8u | r_16 | r_16u | r_32u | r_32f
│       ├── normalized           (cgltf_bool)
│       ├── type                 (cgltf_type)
│       │   scalar | vec2 | vec3 | vec4 | mat2 | mat3 | mat4
│       ├── offset               (cgltf_size) — offset into buffer_view
│       ├── count                (cgltf_size) — number of elements
│       ├── stride               (cgltf_size) — bytes between elements
│       ├── buffer_view          (cgltf_buffer_view*) — where data resides
│       ├── has_min / has_max    (cgltf_bool)
│       ├── min[16] / max[16]    (cgltf_float)
│       ├── is_sparse            (cgltf_bool)
│       ├── sparse               (cgltf_accessor_sparse)
│       │   ├── count
│       │   ├── indices_buffer_view
│       │   ├── indices_byte_offset
│       │   ├── indices_component_type
│       │   ├── values_buffer_view
│       │   └── values_byte_offset
│       ├── extras
│       └── extensions[] / count
│
├── buffer_views[] / buffer_views_count  (cgltf_buffer_view* / cgltf_size)
│   └── cgltf_buffer_view
│       ├── name                 (char*)
│       ├── buffer               (cgltf_buffer*)
│       ├── offset               (cgltf_size)
│       ├── size                 (cgltf_size)
│       ├── stride               (cgltf_size) — 0 = auto
│       ├── type                 (cgltf_buffer_view_type)
│       │   invalid | indices | vertices
│       ├── data                 (void*) — overrides buffer->data
│       ├── has_meshopt_compression
│       ├── meshopt_compression  (cgltf_meshopt_compression)
│       │   ├── buffer / offset / size / stride / count
│       │   ├── mode             (attributes | triangles | indices)
│       │   ├── filter           (octahedral | quaternion | exponential | color)
│       │   └── is_khr
│       ├── extras
│       └── extensions[] / count
│
├── buffers[] / buffers_count    (cgltf_buffer* / cgltf_size)
│   └── cgltf_buffer
│       ├── name                 (char*)
│       ├── size                 (cgltf_size)
│       ├── uri                  (char*) — NULL for GLB
│       ├── data                 (void*) — loaded by cgltf_load_buffers
│       ├── data_free_method     (cgltf_data_free_method)
│       │   none | file_release | memory_free
│       ├── extras
│       └── extensions[] / count
│
├── materials[] / materials_count  (cgltf_material* / cgltf_size)
│   └── cgltf_material
│       ├── name
│       ├── has_pbr_metallic_roughness
│       ├── pbr_metallic_roughness
│       │   ├── base_color_texture        (cgltf_texture_view)
│       │   ├── metallic_roughness_texture
│       │   ├── base_color_factor[4]
│       │   ├── metallic_factor
│       │   └── roughness_factor
│       ├── has_pbr_specular_glossiness
│       │   └── (diffuse/specular/glossiness)
│       ├── has_clearcoat
│       │   └── (clearcoat + roughness + normal textures + factors)
│       ├── has_transmission
│       │   └── (transmission_texture + factor)
│       ├── has_volume
│       │   └── (thickness_texture + factor + attenuation)
│       ├── has_ior / ior
│       ├── has_specular / specular
│       ├── has_sheen / sheen
│       ├── has_emissive_strength / emissive_strength
│       ├── has_iridescence / iridescence
│       ├── has_diffuse_transmission / diffuse_transmission
│       ├── has_anisotropy / anisotropy
│       ├── has_dispersion / dispersion
│       ├── normal_texture       (cgltf_texture_view)
│       ├── occlusion_texture    (cgltf_texture_view)
│       ├── emissive_texture     (cgltf_texture_view)
│       ├── emissive_factor[3]
│       ├── alpha_mode           (opaque | mask | blend)
│       ├── alpha_cutoff
│       ├── double_sided
│       ├── unlit
│       ├── extras
│       └── extensions[] / count
│
├── images[] / images_count      (cgltf_image* / cgltf_size)
│   └── cgltf_image
│       ├── name / uri / mime_type
│       ├── buffer_view          (cgltf_buffer_view*)
│       ├── extras
│       └── extensions[] / count
│
├── textures[] / textures_count  (cgltf_texture* / cgltf_size)
│   └── cgltf_texture
│       ├── name
│       ├── image / sampler
│       ├── has_basisu / basisu_image
│       ├── has_webp / webp_image
│       ├── extras
│       └── extensions[] / count
│
├── samplers[] / samplers_count  (cgltf_sampler* / cgltf_size)
│   └── cgltf_sampler
│       ├── name
│       ├── mag_filter / min_filter (cgltf_filter_type)
│       ├── wrap_s / wrap_t     (cgltf_wrap_mode)
│       ├── extras
│       └── extensions[] / count
│
├── skins[] / skins_count        (cgltf_skin* / cgltf_size)
│   └── cgltf_skin
│       ├── name
│       ├── joints[] / count     (cgltf_node**)
│       ├── skeleton             (cgltf_node*)
│       ├── inverse_bind_matrices (cgltf_accessor*)
│       ├── extras
│       └── extensions[] / count
│
├── cameras[] / cameras_count    (cgltf_camera* / cgltf_size)
│   └── cgltf_camera
│       ├── name
│       ├── type (perspective | orthographic)
│       └── data (union)
│           ├── perspective
│           │   ├── has_aspect_ratio / aspect_ratio
│           │   ├── yfov
│           │   ├── has_zfar / zfar
│           │   └── znear
│           └── orthographic
│               ├── xmag / ymag / zfar / znear
│
├── lights[] / lights_count      (cgltf_light* / cgltf_size)
│   └── cgltf_light
│       ├── name
│       ├── color[3] / intensity
│       ├── type (directional | point | spot)
│       ├── range
│       ├── spot_inner_cone_angle / spot_outer_cone_angle
│       └── extras
│
├── animations[] / animations_count  (cgltf_animation* / cgltf_size)
│   └── cgltf_animation
│       ├── name
│       ├── samplers[] / count
│       │   └── cgltf_animation_sampler
│       │       ├── input / output  (cgltf_accessor*)
│       │       └── interpolation   (linear | step | cubic_spline)
│       ├── channels[] / count
│       │   └── cgltf_animation_channel
│       │       ├── sampler         (cgltf_animation_sampler*)
│       │       ├── target_node     (cgltf_node*)
│       │       └── target_path     (translation | rotation | scale | weights)
│       ├── extras
│       └── extensions[] / count
│
├── variants[] / variants_count   (cgltf_material_variant* / cgltf_size)
│   └── cgltf_material_variant
│       ├── name
│       └── extras
│
├── extras                       (cgltf_extras)
│   └── data                     (char*) — raw JSON
│
├── data_extensions[] / count    (cgltf_extension* / cgltf_size)
├── extensions_used[] / count    (char** / cgltf_size)
├── extensions_required[] / count (char** / cgltf_size)
│
├── json                         (const char*) — raw JSON
├── json_size                    (cgltf_size)
├── bin                          (const void*) — raw GLB binary data
├── bin_size                     (cgltf_size)
│
├── memory                       (cgltf_memory_options) — alloc/free callbacks
└── file                         (cgltf_file_options) — file read callbacks
```

---

## Helper Types

### `cgltf_extras`

```
char* data — arbitrary JSON data
```

### `cgltf_extension`

```
char* name
char* data — extension JSON content
```

### `cgltf_texture_view`

```
cgltf_texture* texture
cgltf_int texcoord
cgltf_float scale
cgltf_bool has_transform
cgltf_texture_transform transform
```

### `cgltf_texture_transform`

```
cgltf_float offset[2]
cgltf_float rotation
cgltf_float scale[2]
cgltf_bool has_texcoord
cgltf_int texcoord
```

### `cgltf_options` (parsing options)

```
cgltf_file_type type       — invalid = auto-detect
cgltf_size json_token_count — 0 = auto
cgltf_memory_options memory — allocator
cgltf_file_options file     — file operations
```

---

## How to Read Vertices (short)

```c
cgltf_accessor *pos = mesh->primitives[0].attributes[0].data;
float *verts = (float*)((unsigned char*)pos->buffer_view->buffer->data + pos->offset);
// pos->count — vertex count
// pos->stride — bytes between vertices (usually 12 = 3×float for vec3)
for (cgltf_size i = 0; i < pos->count; i++)
    printf("%f %f %f\n", verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
```

## Main Enums

| Enum                        | Values                                                                                                                                     |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ |
| `cgltf_result`              | success, data_too_short, unknown_format, invalid_json, invalid_gltf, invalid_options, file_not_found, io_error, out_of_memory, legacy_gltf |
| `cgltf_attribute_type`      | position, normal, tangent, texcoord, color, joints, weights, custom                                                                        |
| `cgltf_component_type`      | r_8, r_8u, r_16, r_16u, r_32u, r_32f                                                                                                       |
| `cgltf_type`                | scalar, vec2, vec3, vec4, mat2, mat3, mat4                                                                                                 |
| `cgltf_primitive_type`      | points, lines, line_loop, line_strip, triangles, triangle_strip, triangle_fan                                                              |
| `cgltf_animation_path_type` | translation, rotation, scale, weights                                                                                                      |
| `cgltf_interpolation_type`  | linear, step, cubic_spline                                                                                                                 |
| `cgltf_camera_type`         | perspective, orthographic                                                                                                                  |
| `cgltf_light_type`          | directional, point, spot                                                                                                                   |
| `cgltf_buffer_view_type`    | indices, vertices                                                                                                                          |
| `cgltf_file_type`           | gltf, glb                                                                                                                                  |
