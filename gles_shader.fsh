precision mediump float;
precision mediump int;
varying highp vec4 texc;
int multiplicationFactor = 8;
uniform sampler2D tex;
uniform bool selected;
uniform bool screenshot;
uniform bool drawTexture;
uniform float xthreshold;
uniform float ythreshold;
varying highp float angle;


float calcIntensity(float min, float mid, float max, float val) {
    if (val <= mid)
        return smoothstep(min,mid,val);
    else
        return 1.0 - smoothstep(mid,max,val);
}

void main(void)
{
    highp float light = (0.5 + (angle * 0.5));
    highp vec2 t = texc.st;
    highp float fac = max(0.25, (1.0 - t.s) * t.t);
    fac = 0.5 + (fac * .5);

    if (drawTexture)
        gl_FragColor = texture2D(tex, texc.st) * fac;
        // the threshold constant defines the with of the lines
    else if (fract(t.s) < (xthreshold*2.0)  || fract(t.t) < (ythreshold*2.0) ) {
            highp float ax = calcIntensity(0.0, xthreshold, xthreshold*2.0, fract(t.s));
            highp float ay = calcIntensity(0.0, ythreshold, ythreshold*2.0, fract(t.t));
            gl_FragColor = vec4(0.8, 1.0, 1.0, sqrt(ax*ax + ay*ay));
    } else if (fract(t.s) > (1.0 - (xthreshold*2.0))  || fract(t.t) > (1.0 - (ythreshold*2.0) )) {
        highp float ax = calcIntensity(1.0-(xthreshold*2.0), 1.0 - xthreshold, 1.0, fract(t.s));
        highp float ay = calcIntensity(1.0-(ythreshold*2.0), 1.0 - ythreshold, 1.0, fract(t.t));
        gl_FragColor = vec4(0.8, 1.0, 1.0, sqrt(ax*ax + ay*ay));
    } else {
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
