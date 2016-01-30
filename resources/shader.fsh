varying highp vec4 texc;
uniform bool selected;
uniform mediump vec3 outline;

void main(void)
{
    highp vec2 t = texc.st;
    highp float fac = max(0.25, (1.0 - t.s) * t.t);
    fac = 0.5 + (fac * .5);
    if (selected)
        gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);
    else
        gl_FragColor = vec4(outline * fac, 0.1);
}
