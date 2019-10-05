#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color_in;
layout (location = 2) in vec2 texture_coord_in;

out vec3 color_frag;
out vec2 texture_coord_frag;
  
uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(position, 1.0f);
	color_frag = color_in;
	texture_coord_frag = vec2(texture_coord_in.x, 1.0 - texture_coord_in.y);
}