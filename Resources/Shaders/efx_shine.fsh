#ifdef GL_ES
precision mediump float;
#endif

//uniform sampler2D u_texture;
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
uniform float factor;
float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)


void main(void)
{
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    
    float timeFactor = abs(sin(1.32 * iGlobalTime * 3.5));
    if (timeFactor >0.45) {
//        timeFactor = max(0.8, timeFactor);
        timeFactor = 1.0;
    }else{
        timeFactor = 0.5;
    }
//    texColor[0] = texColor[0] * factor * max(0.65, abs(sin(1.32 * iGlobalTime)));
//    texColor[1] = texColor[1] * factor * max(0.65, abs(sin(1.32 * iGlobalTime)));
    texColor[0] = texColor[0] * factor * timeFactor;
    texColor[1] = texColor[1] * factor * max(0.5,timeFactor*0.8);
    texColor[2] = texColor[2] * 1.0;
    gl_FragColor = v_fragmentColor * texColor;
}
