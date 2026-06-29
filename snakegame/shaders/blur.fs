#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 resolution;

out vec4 finalColor;

void main()
{
    vec2 texelSize = 1.0 / resolution;

    // Blur radius in pixels. Bigger = blurrier but more samples per pixel.
    const float size = 3.0;

    vec4 color = vec4(0.0);
    float samples = 0.0;

    for (float x = -size; x <= size; x += 1.0)
    {
        for (float y = -size; y <= size; y += 1.0)
        {
            color += texture(texture0, fragTexCoord + vec2(x, y) * texelSize);
            samples += 1.0;
        }
    }

    color /= samples;

    finalColor = color * colDiffuse * fragColor;
}