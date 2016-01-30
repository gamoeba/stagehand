attribute highp vec4 vertex;
attribute highp vec4 texCoord;
attribute mediump vec3 normal;
uniform mediump mat4 matrix;
varying highp vec4 texc;
varying mediump vec4 color;
varying highp float angle;

void main(void)
{
    vec3 toLight = normalize(vec3(0.0, 0.0, 0.8) - vertex.xyz);
    highp vec4 norm = (vec4(normal, 1.0));
    norm = matrix * norm;
    angle = max(dot(norm.xyz, toLight), 0.0);
    angle = min(angle,1.0);
    gl_Position = matrix * vertex;
    texc = texCoord;
}
