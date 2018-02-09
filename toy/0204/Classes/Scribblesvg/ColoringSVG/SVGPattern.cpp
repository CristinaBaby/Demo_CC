//
//  SVGPattern.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/10/29.
//
//

#include "SVGPattern.h"
#include "ScribbleUtil.h"
#define kSVGPatternShaderName "SVGPattern.frag"
#define STRINGIFY(A)  #A
#include kSVGPatternShaderName

static SVGPattern *_instance = nullptr;

SVGPattern *SVGPattern::getInstance(){
    if (nullptr == _instance) {
        _instance = new SVGPattern();
    }
    return _instance;
}

RenderTexture *SVGPattern::getPattern(Sprite *pTemplate){
    /*优化Pattern图片_templete的锯齿，以便线框图在多重采样抗锯齿之后，完全遮住Pattern图*/
    //renhong mark: 请进一步优化SVG绘制，以在GPU完成第一次绘制即完成本工作。
//    Sprite *lTemplate = Sprite::createWithTexture(lRT->getSprite()->getTexture());
    Size lSize = pTemplate->getContentSize();
    RenderTexture *lRT = RenderTexture::create(lSize.width, lSize.height);
    lRT->getSprite()->getTexture()->setAliasTexParameters();
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kSVGPatternShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert,SVGPattern_frag, lGLProgramKey);
    if (NULL != lProgram) {
        if (pTemplate->getGLProgram() != lProgram) {
            ScribbleUtil::getInstance()->setGLProgram(pTemplate, lProgram);
        }
    }
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(pTemplate->getGLProgram());
    lState->setUniformVec2("v_offset", Vec2(1.0 / pTemplate->getTexture()->getContentSize().width, 1.0 / pTemplate->getTexture()->getContentSize().height));
    
    pTemplate->setPosition(pTemplate->getContentSize() / 2);
    lRT->begin();
    pTemplate->visit();
    lRT->end();
    Director::getInstance()->getRenderer()->render();
    
    return lRT;
}