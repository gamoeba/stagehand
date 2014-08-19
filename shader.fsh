varying highp vec4 texc;
int multiplicationFactor = 8;
uniform sampler2D tex;
uniform bool selected;
uniform bool screenshot;
uniform bool drawTexture;
uniform float xthreshold;
uniform float ythreshold;
varying highp float angle;

void main(void)
{
    highp float light = (0.5 + (angle * 0.5));
    highp vec2 t = texc.st;
    highp float fac = max(0.25, (1.0 - t.s) * t.t);
    fac = 0.5 + (fac * .5);

    if (drawTexture)
        gl_FragColor = texture2D(tex, texc.st) * fac;
        // the threshold constant defines the with of the lines
    else if (fract(t.s) < xthreshold  || fract(t.t) < ythreshold )
        gl_FragColor = vec4(0.8, 1.0, 1.0, 1.0);
    else if (fract(t.s) > (1.0 - xthreshold)  || fract(t.t) > (1.0 - ythreshold) )
        gl_FragColor = vec4(0.8, 1.0, 1.0, 1.0);
    else {
        if (selected) {
           gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);
        }else{
            if (!screenshot) {
                gl_FragColor = vec4(vec3(0.8, 1.0, 1.0) * fac, 0.1);
            } else {
               discard;
            }
        }
    }
}
