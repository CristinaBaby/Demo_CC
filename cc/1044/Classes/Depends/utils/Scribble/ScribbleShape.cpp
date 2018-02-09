//
//  ScribbleShape.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#include "ScribbleShape.h"
#define STRINGIFY(A)  #A
#include "ScribbleShapeShader.frag"

ScribbleShape::ScribbleShape(Sprite *pBrush) : Scribble(pBrush){
}

void ScribbleShape::setBrushShader(){
    GLProgram *lProgram = this->loadShader(ScribbleShapeShader_frag, kScribbleShaderName);
    if (NULL != lProgram) {
        if (this->getBrush()->getGLProgram() != lProgram) {
            setShaderPrograms(this->getBrush(), lProgram);
            
            this->setBrushAlphaTestValue(0.0);
            this->setTargetAlphaTestValue(0.0);
            this->setTargetSolid(true);
            this->setBrushTexture();
        }
    }
}

void ScribbleShape::setBrushType(Scribble::BrushType pType){
    Scribble::setBrushType(pType);
    Sprite *lBrush = this->getBrush();
    switch (pType) {
        case Scribble::eBrush:{
            BlendFunc f = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
            lBrush->setBlendFunc(f);
            break;
        }
        default:
            break;
    }
}

void ScribbleShape::setShape(Sprite *pShape){
    //CCLOG("Warning: Dot use this function frequently");
    if (nullptr != _shape && pShape->getTexture() == _shape->getTexture()) {
        return;
    }
    if (nullptr == _shape) {
        _shape = pShape;
        _shape->retain();
    }
    else{
        pShape->retain();
        _shape->release();
        _shape = pShape;
    }
    
    this->setShapeTexture(pShape);
}

void ScribbleShape::setShapeTexture(Sprite *pTarget){
    if (nullptr == pTarget) {
        return;
    }
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec2("v_texSize_shape", Vec2(pTarget->getContentSize().width, pTarget->getContentSize().height));
    lState->setUniformTexture("s_texture_shape", pTarget->getTexture());
}

void ScribbleShape::setTarget(Sprite *pTarget){
    Scribble::setTarget(pTarget);
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformInt("v_taret_type_color", 0);
}

void ScribbleShape::setTarget(Color4F pColor){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec4("v_target_color", Vec4(pColor.r, pColor.g, pColor.b, pColor.a));
    lState->setUniformInt("v_taret_type_color", 1);
}