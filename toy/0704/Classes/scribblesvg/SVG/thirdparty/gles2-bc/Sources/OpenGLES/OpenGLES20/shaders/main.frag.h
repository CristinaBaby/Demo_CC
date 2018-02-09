unsigned char main_frag[] = "       \n\
#ifdef GL_FRAGMENT_PRECISION_HIGH   \n\
precision highp float;              \n\
#else                               \n\
precision mediump float;            \n\
#endif                              \n\
\n\
varying vec4 v_frontColor;          \n\
varying vec4 v_backColor;           \n\
void main()                         \n\
{                                   \n\
gl_FragColor = v_frontColor;//vec4(0.0, 1.0, 1.0, 1.0);                       \n\
}                                   \n\
";
unsigned int main_frag_len = 3144;