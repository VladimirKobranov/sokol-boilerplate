@ctype mat4 HMM_Mat4

@vs vs
layout(binding=0) uniform vs_params {
    mat4 mvp;
};
in vec4 position;

void main() {
    gl_Position = mvp * position;
}
@end

@fs fs
out vec4 frag_color;
void main() {
    frag_color = vec4(1.0, 0.5, 0.2, 1.0);
}
@end

@program triangle vs fs
