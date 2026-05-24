@ctype mat4 HMM_Mat4

@vs vs
layout(binding=0) uniform vs_params {
    mat4 mvp;
    mat4 mv;
};
in vec4 position;
out vec3 v_view_pos;

void main() {
    v_view_pos = (mv * position).xyz;
    gl_Position = mvp * position;
}
@end

@fs fs
in vec3 v_view_pos;
out vec4 frag_color;
void main() {
    vec3 n = normalize(cross(dFdx(v_view_pos), dFdy(v_view_pos)));
    float facing = abs(dot(n, normalize(-v_view_pos)));
    vec3 base = vec3(1.0, 0.5, 0.2);
    frag_color = vec4(base * (0.25 + 0.75 * facing), 1.0);
}
@end

@program triangle vs fs
