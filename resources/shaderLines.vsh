attribute highp vec4 vertex;

uniform mediump mat4 viewMatrix;

void main(void)
{
    gl_Position = viewMatrix * vertex;
}
