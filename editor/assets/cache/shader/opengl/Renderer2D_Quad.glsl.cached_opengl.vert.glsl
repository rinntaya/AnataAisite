#version 450

struct _9
{
    vec4 _m0;
    vec2 _m1;
    float _m2;
};

layout(binding = 0, std140) uniform _46_48
{
    mat4 _m0;
} _48;

layout(location = 0) out _9 _11;
layout(location = 1) in vec4 _15;
layout(location = 2) in vec2 _21;
layout(location = 4) in float _27;
layout(location = 3) flat out float _31;
layout(location = 3) in float _32;
layout(location = 4) flat out int _35;
layout(location = 5) in int _37;
layout(location = 0) in vec3 _54;

void main()
{
    _11._m0 = _15;
    _11._m1 = _21;
    _11._m2 = _27;
    _31 = _32;
    _35 = _37;
    gl_Position = _48._m0 * vec4(_54, 1.0);
}

