#version 450

struct _12
{
    vec3 _m0;
    vec4 _m1;
    float _m2;
    float _m3;
};

layout(location = 0) in _12 _14;
layout(location = 0) out vec4 _49;
layout(location = 1) out int _63;
layout(location = 4) flat in int _65;

void main()
{
    float _21 = 1.0 - length(_14._m0);
    float _41 = smoothstep(0.0, _14._m3, _21) * smoothstep(_14._m2 + _14._m3, _14._m2, _21);
    if (_41 == 0.0)
    {
        discard;
    }
    _49 = _14._m1;
    _49.w *= _41;
    _63 = _65;
}

