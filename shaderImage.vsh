attribute highp vec4 vertex;
attribute highp vec4 texCoord;

uniform mediump mat4 viewMatrix;
varying highp vec4 texc;

void main(void)
{
    gl_Position = viewMatrix * vertex;
    texc = texCoord;
}
