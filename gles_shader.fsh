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

    if (false && drawTexture)
        gl_FragColor = texture2D(tex, texc.st) * fac;
        // the threshold constant defines the with of the lines
    else
        gl_FragColor = vec4(vec3(0.8, 1.0, 1.0) , 1.0);

}
