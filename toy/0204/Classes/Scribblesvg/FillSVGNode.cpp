//
//  FillSVGNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/11/3.
//
//

#include "FillSVGNode.h"

FillSVGNode *FillSVGNode::create(string pLineArtFile){
    FillSVGNode *ret = new FillSVGNode();
    if(ret && ret->init(pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FillSVGNode::init(string pLineArtFile){
    if (!ScribbleSVGLineFrameNode::init(pLineArtFile)) {
        return false;
    }
    this->_coloringSVGNode = new ColoringSVGNode();
    this->_coloringSVGNode->autorelease();
    this->addChild(this->_coloringSVGNode);
    this->_coloringSVGNode->debugPattern(false);
    this->_coloringSVGNode->setCanvas(this->_canvas);
    this->_coloringSVGNode->setContentSize(this->getContentSize());
    this->_coloringSVGNode->setPosition(Vec2(-this->_coloringSVGNode->getContentSize().width / 2, -this->_coloringSVGNode->getContentSize().height / 2));
    return true;
}

FillSVGNode::~FillSVGNode(){
}

bool FillSVGNode::setPattern(string pSVGFile, SVGFill::FillState pState/* = SVGFill::FillState::eAuto*/){
    ScribbleSVGLineFrameNode::setPattern(pSVGFile, pState);
    
    this->_coloringSVGNode->setPatternImage(this->_patternImage);
    this->_coloringSVGNode->setPattern(this->_patternNode);
    this->_coloringSVGNode->initRenderPara();
    this->_coloringSVGNode->updatePattern();
    this->_coloringSVGNode->setStartColor(Color4B(255, 0, 0, 255));
    this->_coloringSVGNode->setColoringMode(ColoringNode::ColoringMode::eFlat);
    
    return true;
}