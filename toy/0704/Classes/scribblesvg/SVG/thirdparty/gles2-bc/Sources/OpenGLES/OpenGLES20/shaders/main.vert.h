unsigned char main_vert[] = "                       \n\
                                                    \n\
precision highp float;                              \n\
precision highp int;                                \n\
                                                    \n\
// Attributes                                       \n\
attribute vec4 a_position;                          \n\
attribute vec4 a_color;                             \n\
                                                    \n\
// Uniforms                                         \n\
uniform mat4 u_modelViewProjectionMatrix;           \n\
                                                    \n\
// Varyings                                         \n\
varying vec4 v_frontColor;                          \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    gl_Position = u_modelViewProjectionMatrix * a_position;                                 \n\
    v_frontColor = a_color / 255.0;                         \n\
}                                                   \n\
";
unsigned int main_vert_len = 6437;                  
