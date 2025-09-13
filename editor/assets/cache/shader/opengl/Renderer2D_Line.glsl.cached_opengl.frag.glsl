#version 450

struct _10
{
    vec4 _m0;
};

layout(location = 0) out vec4 _9;
layout(location = 0) in _10 _12;
layout(location = 1) out int _19;
layout(location = 1) flat in int _21;

void main()
{
    _9 = _12._m0;
    _19 = _21;
}

