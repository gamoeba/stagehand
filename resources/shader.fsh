varying highp vec4 texc;
uniform bool selected;
uniform mediump vec3 outline;

void main(void)
{
    highp float fac =  max(0.5 * ((1.0 - texc.s) * texc.t), 0.2);

    if (selected)
        gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);
    else
        gl_FragColor = vec4(outline * fac, 0.1);
}
