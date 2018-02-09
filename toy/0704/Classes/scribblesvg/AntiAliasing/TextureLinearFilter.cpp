//
//  TextureLinearFilter.cpp
//  TextureLinearFilter_3_2
//
//  Created by renhong on 15/10/29.
//
//

#include "TextureLinearFilter.h"

#include "ScribbleUtil.h"
#define kTextureLinearFilterShaderName "TextureLinearFilter.frag"
#define STRINGIFY(A)  #A
#include kTextureLinearFilterShaderName

static TextureLinearFilter *_instance = nullptr;

TextureLinearFilter *TextureLinearFilter::getInstance(){
    if (nullptr == _instance) {
        _instance = new TextureLinearFilter();
    }
    return _instance;
}

RenderTexture *TextureLinearFilter::linearTexture(Sprite *pTemplate){
    Size lSize = pTemplate->getContentSize() * pTemplate->getScale();
    RenderTexture *lRT = RenderTexture::create(lSize.width, lSize.height);
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kTextureLinearFilterShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert,TextureLinearFilter_frag, lGLProgramKey);
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