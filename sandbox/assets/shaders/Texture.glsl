// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Postition;

void main()
{
    v_Postition = a_Position;
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core



// 可以直接在 GLSL 里给 uniform 一个默认值
uniform float time   = 0.0;
uniform vec2  resolution = vec2(800.0, 600.0);
uniform float scale  = 10.0;

// 简单 hash
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

// 2D 噪声函数
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    // 四个角的随机值
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    // 插值
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
    (c - a) * u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

// 生成随机梯度方向
vec2 random2(vec2 p) {
    // 类似 hash，输出范围 [-1,1] 的方向向量
    float x = fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
    float y = fract(sin(dot(p, vec2(269.5, 183.3))) * 43758.5453123);
    return normalize(vec2(x - 0.5, y - 0.5));
}

// 2D Perlin 噪声
float perlinNoise(vec2 p) {
    vec2 i = floor(p);   // 网格坐标
    vec2 f = fract(p);   // 小数部分（方格内的相对坐标）

    // 四个角点的梯度向量
    vec2 g00 = random2(i);
    vec2 g10 = random2(i + vec2(1.0, 0.0));
    vec2 g01 = random2(i + vec2(0.0, 1.0));
    vec2 g11 = random2(i + vec2(1.0, 1.0));

    // 点到角点的向量
    vec2 d00 = f - vec2(0.0, 0.0);
    vec2 d10 = f - vec2(1.0, 0.0);
    vec2 d01 = f - vec2(0.0, 1.0);
    vec2 d11 = f - vec2(1.0, 1.0);

    // 梯度点积（越接近角点方向，值越大）
    float v00 = dot(g00, d00);
    float v10 = dot(g10, d10);
    float v01 = dot(g01, d01);
    float v11 = dot(g11, d11);

    // 平滑插值权重
    vec2 u = f * f * (3.0 - 2.0 * f);

    // 插值
    return mix(
    mix(v00, v10, u.x),
    mix(v01, v11, u.x),
    u.y
    );
}


layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Postition;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv *= scale;

//    float n = noise(uv + time * 0.1);
//    float n = 0.5 + 0.5 * perlinNoise(v_Postition.xy * 5); * vec4(vec3(n), 1.0)
    color = texture(u_Texture, v_TexCoord * 10.0) * u_Color;
}