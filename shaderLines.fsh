uniform mediump vec3 outline;

void main(void)
{
    gl_FragColor = vec4(outline, 1.0);
}
