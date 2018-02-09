//
//  Coloring.frag
//
//
//  Created by renhong on 7/24/14.
//
//

const char* SVGPattern_frag = STRINGIFY(
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

void main(){
    vec4 lColor = texture2D(CC_Texture0, v_texCoord);
    
    if(vec3equal(lColor.rgb, kBlackColor.rgb)){//fragement in the lineframe around the shape
        gl_FragColor = besideColor(v_texCoord, CC_Texture0, kBlackColor);
    }
    else{
        gl_FragColor = lColor;
    }
}

vec4 besideColor(const vec2 pCoord, sampler2D pTexture, const vec4 pColor){
    vec4 lRes = vec4(0.0, 0.0, 0.0, 1.0);
    const int besideCoordsNumber = 8;
    vec2 besideCoords[besideCoordsNumber];
    
    besideCoords[0] = vec2(v_offset.x, 0.0);//+x轴
    besideCoords[1] = vec2(v_offset.x, v_offset.y);//一象限
    besideCoords[2] = vec2(0.0, v_offset.y);//+y轴
    besideCoords[3] = vec2(-v_offset.x, v_offset.y);//二象限
    besideCoords[4] = vec2(-v_offset.x, 0.0);//-x轴
    besideCoords[5] = vec2(-v_offset.x, -v_offset.y);//三象限
    besideCoords[6] = vec2(0.0, -v_offset.y);//-y轴
    besideCoords[7] = vec2(v_offset.x, -v_offset.y);//四象限
    
    for(int i = 0; i < besideCoordsNumber; ++i){
        vec2 lNearCoord = vec2(clamp(pCoord.x + besideCoords[i].x, 0.0, 1.0), clamp(pCoord.y + besideCoords[i].y, 0.0, 1.0));
        vec4 lBesideColor = texture2D(pTexture, lNearCoord);
        if(!vec3equal(lBesideColor.rgb, pColor.rgb)){
            lRes = lBesideColor;
            break;
        }
    }
    for(int i = 0; i < besideCoordsNumber; ++i){
        vec2 lNearCoord = vec2(clamp(pCoord.x + besideCoords[i].x * 2.0, 0.0, 1.0), clamp(pCoord.y + besideCoords[i].y * 2.0, 0.0, 1.0));
        vec4 lBesideColor = texture2D(pTexture, lNearCoord);
        if(!vec3equal(lBesideColor.rgb, pColor.rgb)){
            lRes = lBesideColor;
            break;
        }
    }
    
    return lRes;
}

bool vec3equal(vec3 pLeft, vec3 pRight){
    float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
    //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
    vec3 lTmp = pLeft - pRight;
    float lTmpMax = max(max(abs(lTmp.r), abs(lTmp.g)), abs(lTmp.b));
    if(lTmpMax <= EPSINON){
        return true;
    }
    return false;
}

bool vec4equal(vec4 pLeft, vec4 pRight){
    float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
    //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
    vec4 lTmp = pLeft - pRight;
    float lTmpMax1 = max(abs(lTmp.r), abs(lTmp.g));
    float lTmpMax2 = max(abs(lTmp.b), abs(lTmp.a));
    float lTmpMax = max(lTmpMax1, lTmpMax2);
    if(lTmpMax <= EPSINON){
        return true;
    }
    return false;
}
);