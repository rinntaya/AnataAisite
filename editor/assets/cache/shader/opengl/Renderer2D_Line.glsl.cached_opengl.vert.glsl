#version 450

struct _8
{
    vec4 _m0;
};

layout(binding = 0, std140) uniform _30_32
{
    mat4 _m0;
} _32;

layout(location = 0) out _8 _10;
layout(location = 1) in vec4 _14;
layout(location = 1) flat out int _19;
layout(location = 2) in int _21;
layout(location = 0) in vec3 _38;

void main()
{
    _10._m0 = _14;
    _19 = _21;
    gl_Position = _32._m0 * vec4(_38, 1.0);
}

