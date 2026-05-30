@ctype mat4 HMM_Mat4

@vs vs
layout(binding=0) uniform vs_params {
    mat4 mvp;
    mat4 mv;
};
in vec4 position;
in vec2 texcoord0;
out vec3 v_view_pos;
out vec2 v_uv;

void main() {
    v_view_pos = (mv * position).xyz;
    v_uv = texcoord0;
    gl_Position = mvp * position;
}
@end

@fs fs
layout(binding=0) uniform texture2D base_color_tex;
layout(binding=0) uniform sampler base_color_smp;
in vec3 v_view_pos;
in vec2 v_uv;
out vec4 frag_color;
void main() {
    vec3 n = normalize(cross(dFdx(v_view_pos), dFdy(v_view_pos)));
    float facing = abs(dot(n, normalize(-v_view_pos)));
    vec4 base = texture(sampler2D(base_color_tex, base_color_smp), v_uv);
    frag_color = vec4(base.rgb * (0.25 + 0.75 * facing), base.a);
}
@end

@program triangle vs fs
