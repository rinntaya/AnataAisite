#version 450

struct _26
{
    vec4 _m0;
    vec2 _m1;
    float _m2;
};

layout(binding = 0) uniform sampler2D _16[32];

layout(location = 0) out vec4 _9;
layout(location = 3) flat in float _18;
layout(location = 0) in _26 _28;
layout(location = 1) out int _44;
layout(location = 4) flat in int _46;

void main()
{
    _9 = texture(_16[int(_18)], _28._m1 * _28._m2) * _28._m0;
    _44 = _46;
}

