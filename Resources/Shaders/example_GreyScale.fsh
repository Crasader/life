#ifdef GL_ES
precision mediump float;
#endif

float  iGlobalTime = CC_Time[1];           // shader playback time (in seconds)
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main(void)
{
	vec4 c = texture2D(CC_Texture0, v_texCoord);
//	gl_FragColor.xyz = vec3(0.2126* sin(1.32 * iGlobalTime) *c.r + 0.7152* sin(1.32 * iGlobalTime)*c.g + 0.0722* sin(1.32 * iGlobalTime)*c.b);
////    v_fragmentColor.xyz = vec3(0.2126* sin(1.32 * iGlobalTime) *c.r + 0.7152* sin(1.32 * iGlobalTime)*c.g + 0.0722* sin(1.32 * iGlobalTime)*c.b);
////    v_fragmentColor.w = c.w;
////    gl_FragColor.xyz += v_fragmentColor.xyz;
//	gl_FragColor.w = c.w;
    
//    float gray = dot(c.rgb, vec3(0.2126, 0.7152, 0.0722)* sin(1.32 * iGlobalTime));
    gl_FragColor = v_fragmentColor * dot(c.rgb, vec3(0.2126, 0.7152, 0.0722)* sin(1.32 * iGlobalTime));
}

//#ifdef GL_ES
//precision lowp float;
//#endif
//
//varying vec4 v_fragmentColor;
//varying vec2 v_texCoord;
//uniform sampler2D u_texture;
//
//uniform float factor;
//uniform float width;
//uniform float offset;
//uniform vec3 color;
//void main()
//{
//    vec4 texColor = texture2D(u_texture, v_texCoord);
//    // line (x+y-offset=0) offset:[0, 2] (2->0)
//    //     |y
//    //     |
//    // ____|0__ 1__2____x
//    //     |       |
//    //     |_______|*
//    float distance = abs(v_texCoord[0]+v_texCoord[1]-offset)/1.414;
//    // linear gradient
//    // (1/width)x + y = 1
//    distance = 1.0-(1.0/width)*distance;
//    distance = max(distance, 0.0);
//    vec4 sample = vec4(0.0,0.0,0.0,0.0);
//    sample[0] = color[0] * distance;
//    sample[1] = color[1] * distance;
//    sample[2] = color[2] * distance;
//    sample[3] = distance;
//    
//    // blend additive
//    float alpha = sample[3]*texColor[3];
//    texColor[0] = texColor[0] + sample[0]*alpha*factor;
//    texColor[1] = texColor[1] + sample[1]*alpha*factor;
//    texColor[2] = texColor[2] + sample[2]*alpha*factor;
//    gl_FragColor = v_fragmentColor * texColor;
//}
