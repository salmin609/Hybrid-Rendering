#version 330 core
out vec4 fragColor;

in vec4 Vertex_UV;

uniform sampler2D screenTextures;
uniform float exposure = 10.0;

vec4 Normal()
{
	return texture(screenTextures, Vertex_UV.xy);
}
vec4 Bloom()
{
    vec4 c = texelFetch(screenTextures, ivec2(gl_FragCoord.xy), 0);
    c.rgb = vec3(1.0) - exp(-c.rgb * exposure);
    return c;
}

vec4 Inversion()
{
	return vec4(vec3(1.0 - texture(screenTextures, Vertex_UV.xy)), 1.0);
}
vec4 GrayScale()
{
	vec4 color = texture(screenTextures, Vertex_UV.xy);
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec4(average, average, average, 1.0);
}
const float offset = 1.0 / 300.0;

vec4 Kernal()
{
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 좌측 상단
        vec2( 0.0f,    offset), // 중앙 상단
        vec2( offset,  offset), // 우측 상단
        vec2(-offset,  0.0f),   // 좌측 중앙
        vec2( 0.0f,    0.0f),   // 정중앙
        vec2( offset,  0.0f),   // 우측 중앙
        vec2(-offset, -offset), // 좌측 하단
        vec2( 0.0f,   -offset), // 중앙 하단
        vec2( offset, -offset)  // 우측 하단   
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTextures, Vertex_UV.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

vec4 Blur()
{
    vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // 좌측 상단
            vec2( 0.0f,    offset), // 중앙 상단
            vec2( offset,  offset), // 우측 상단
            vec2(-offset,  0.0f),   // 좌측 중앙
            vec2( 0.0f,    0.0f),   // 정중앙
            vec2( offset,  0.0f),   // 우측 중앙
            vec2(-offset, -offset), // 좌측 하단
            vec2( 0.0f,   -offset), // 중앙 하단
            vec2( offset, -offset)  // 우측 하단   
        );

        float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
        );

        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTextures, Vertex_UV.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        return vec4(col, 1.0);
}


void main()
{
	fragColor = Normal();
}