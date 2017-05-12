#version 150

attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 projview_matrix;
uniform mat4 model_matrix;

out vec3 pos;
out vec3 normals;
out vec3 viewVector;

void main()
{
	normals = mat3(model_matrix) * a_normal;
	vec4 viewspace = model_matrix * vec4(a_position, 1.0);
	viewVector = -viewspace.xyz;
	
    pos = vec3(projview_matrix * model_matrix * vec4(a_position, 1.0f)); 
    gl_Position = projview_matrix * model_matrix * vec4(a_position, 1.0f); 
}