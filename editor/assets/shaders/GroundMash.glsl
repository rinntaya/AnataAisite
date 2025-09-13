#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 vWorldPos;

uniform mat4 u_VP;

// 这里画的是一个超大 Quad 覆盖世界XZ平面
void main()
{
    vWorldPos = aPos;
    gl_Position = u_VP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 vWorldPos;
out vec4 FragColor;

uniform vec3 u_CamPos;   // 摄像机位置（用于淡出）
uniform vec3 u_GridColor1 = vec3(0.3, 0.3, 0.3);
uniform vec3 u_GridColor10 = vec3(0.6, 0.6, 0.6);

float gridFactor(vec2 coord, float scale)
{
    // 坐标缩放
    vec2 grid = abs(fract(coord * scale) - 0.5) / fwidth(coord * scale);
    float line = min(grid.x, grid.y);
    return 1.0 - min(line, 1.0); // 1=网格线，0=空
}

void main()
{
    // 取世界XZ坐标
    vec2 coord = vWorldPos.xz;

    // 细网格 (每1单位)
    float g1 = gridFactor(coord, 1.0);

    // 粗网格 (每10单位)
    float g10 = gridFactor(coord, 0.1);

    // 合并网格，粗网格线颜色更亮
    vec3 color = mix(u_GridColor1, u_GridColor10, g10);
    float gridLine = max(g1, g10);

    // 距离相机越远，越淡
    float dist = length(vWorldPos.xz - u_CamPos.xz);
    float fade = clamp(1.0 - dist / 1000.0, 0.0, 1.0);

    FragColor = vec4(color, gridLine * fade);
}
