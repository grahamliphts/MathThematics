#version 150

attribute vec3 a_position;
uniform mat4 projview_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = projview_matrix * model_matrix * vec4(a_position, 1.0f) ; 
}