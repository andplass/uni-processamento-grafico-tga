#version 410 core
in vec3 color_frag;
in vec2 texture_coord_frag;

out vec4 color_out;

// Texture sampler
uniform sampler2D texture;

void main()
{
    color_out = texture(texture, texture_coord_frag); // * vec4(color_frag, 1.0);
}