//
//  ColoringLineFrameNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/17/15.
//
//

#include "ColoringLineFrameNode.h"
#include "ScribbleUtil.h"

ColoringLineFrameNode::ColoringLineFrameNode(){
    _patternNode = nullptr;
    _delegate = nullptr;
    _debugPattern = false;
}

ColoringLineFrameNode::~ColoringLineFrameNode(){
    
}

bool ColoringLineFrameNode::init(string pLineArtFile){
    if (!ColoringNode::init(pLineArtFile, pLineArtFile)) {
        return false;
    }
    _debugPattern = false;
    return true;
}

void ColoringLineFrameNode::initPattern(string pPatternImage){
    //overwrite父类方法，以此避免在init方法中被调用执行ColoringNode::initPattern
}

void ColoringLineFrameNode::updatePattern(){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_template->getGLProgram());
    lState->setUniformTexture("s_texture_shape", _template->getTexture());
    lState->setUniformVec2("v_texture_shape_size", _template->getTexture()->getContentSize());
    CC_SAFE_RELEASE(_templateImage);
    _templateImage = _patternNode->getPatternImage();
    _templateImage->retain();
    CC_SAFE_RELEASE(_shapeBoundTexture);
    _shapeBoundTexture = this->getShapeBounds(_templateImage);
    _shapeBoundTexture->retain();
    lState->setUniformTexture("s_texture_shape_bounds", _shapeBoundTexture);
}

bool ColoringLineFrameNode::setPattern(string pLineFrameFile,int pTolerance/* = 0*/){
    if (_patternNode) {
        _patternNode->removeFromParent();
    }

    Pattern *lPatternNode = Pattern::create(pLineFrameFile);
    lPatternNode->setDelegate(this);
    lPatternNode->setBoundaryTolerance(pTolerance);
    _patternNode = lPatternNode;
    this->addChild(lPatternNode, -1);
//    lPatternNode->setAnchorPoint(Vec2(0.5, 0.5));
    lPatternNode->setVisible(false);
    _template->setTexture(_patternNode->getLineArt()->getTexture());
    _template->setContentSize(_patternNode->getLineArt()->getContentSize());
    lPatternNode->fillPatternAsync();
    
    return true;
}

void ColoringLineFrameNode::debugPattern(bool pDebug/* = true*/){
    if (!pDebug) {
        return;
    }
    _debugPattern = true;
    if(nullptr != _lineArt){
        _lineArt->setVisible(false);
    }
    if(nullptr != _patternNode){
        _patternNode->setVisible(true);
    }
    if(nullptr != _canvas){
        _canvas->clear(0.0, 0.0, 0.0, 0.0);
    }
}

void ColoringLineFrameNode::loadPercent(Pattern *pNode, float pPercent){
    this->getDelegate()->loadPercent(this, pPercent);
}

void ColoringLineFrameNode::loadEnded(Pattern *pNode){
    this->updatePattern();
    this->getDelegate()->loadEnded(this);
    if (_debugPattern) {
        this->debugPattern(true);
        string lPath = FileUtils::getInstance()->getWritablePath() + "pattern_debug.png";
        this->getPattern()->savePattern(lPath);
    }
}