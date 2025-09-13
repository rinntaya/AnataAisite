#version 450

struct _9
{
    vec3 _m0;
    vec4 _m1;
    float _m2;
    float _m3;
};

layout(binding = 0, std140) uniform _47_49
{
    mat4 _m0;
} _49;

layout(location = 0) out _9 _11;
layout(location = 1) in vec3 _15;
layout(location = 2) in vec4 _21;
layout(location = 3) in float _27;
layout(location = 4) in float _32;
layout(location = 4) flat out int _36;
layout(location = 5) in int _38;
layout(location = 0) in vec3 _53;

void main()
{
    _11._m0 = _15;
    _11._m1 = _21;
    _11._m2 = _27;
    _11._m3 = _32;
    _36 = _38;
    gl_Position = _49._m0 * vec4(_53, 1.0);
}

