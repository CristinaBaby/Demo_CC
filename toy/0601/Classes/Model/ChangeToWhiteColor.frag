static const char* ChangeToWhiteColor_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n

varying vec2 v_texCoord;
                                       
void main()
{
    vec4 color = texture2D(CC_Texture0, v_texCoord);

    if (color.a > 0.000001)
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        gl_FragColor = color;
    }
}
);