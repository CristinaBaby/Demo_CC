//
//  Coloring.frag
//
//
//  Created by renhong on 7/24/14.
//
//

const char* TextureLinearFilter_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 v_offset;

const vec4 kBlackColor = vec4(0.0, 0.0, 0.0, 1.0);

vec4 besideColor(const vec2 pCoord, sampler2D pTexture, const vec4 pColor);
bool vec3equal(vec3 pLeft, vec3 pRight);
bool vec4equal(vec4 pLeft, vec4 pRight);

void main(){//gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);return;
    vec4 lColor = texture2D(CC_Texture0, v_texCoord);
    
    vec2 lRightCoord = vec2(clamp(v_texCoord.x + v_offset.x, 0.0, 1.0), v_texCoord.y);
    vec4 lRightColor = texture2D(CC_Texture0, lRightCoord);
    
    vec2 lLeftCoord = vec2(clamp(v_texCoord.x - v_offset.x, 0.0, 1.0), v_texCoord.y);
    vec4 lLeftColor = texture2D(CC_Texture0, lLeftCoord);
    
    vec2 lTopCoord = vec2(v_texCoord.x, clamp(v_texCoord.y + v_offset.y, 0.0, 1.0));
    vec4 lTopColor = texture2D(CC_Texture0, lTopCoord);
    
    vec2 lBottomCoord = vec2(v_texCoord.x, clamp(v_texCoord.y - v_offset.y, 0.0, 1.0));
    vec4 lBottomColor = texture2D(CC_Texture0, lBottomCoord);
    
    vec4 lRes = lColor * 0.2 + lLeftColor * 0.2 + lRightColor * 0.2 + lTopColor * 0.2 + lBottomColor * 0.2;
    lRes.a = 1.0;
    gl_FragColor = lRes;//lColor * 0.6 + (lLeftColor + lRightColor + lTopColor + lBottomColor) * 0.1;
}
);