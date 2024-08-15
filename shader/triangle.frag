#version 460

out vec4 fragColor;

in VertexOut
{
	vec4 color;
} vertexData;

void main()
{
	fragColor = vertexData.color;
}
