//
//  ScribbleLineFrameNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/14/15.
//
//

#include "ScribbleLineFrameNode.h"
#include "ScribbleUtil.h"

ScribbleLineFrameNode *ScribbleLineFrameNode::create(Size pCanvasSize){
    ScribbleLineFrameNode *ret = new ScribbleLineFrameNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleLineFrameNode::ScribbleLineFrameNode(){
    _lineFrameNode = nullptr;
    _patternNode = nullptr;
    _debugPattern = false;
    _delegate = nullptr;
}

bool ScribbleLineFrameNode::setPattern(string pLineFrameFile, bool pAutoFill/* = false*/, int pTolerance/* = 0*/){
    if (_patternNode) {
        _patternNode->removeFromParent();
        //        _patternNode->release();
    }
    Pattern *lPatternNode = Pattern::create(pLineFrameFile);
    lPatternNode->setBoundaryTolerance(pTolerance);
    _patternNode = lPatternNode;
    //    _patternNode->retain();
    this->addChild(lPatternNode, -1);
    lPatternNode->setAnchorPoint(Vec2(0.5, 0.5));
    //    lPatternNode->setPosition(pPosition);
    lPatternNode->getLineArt()->setPosition(this->getCanvasSize() / 2);
    lPatternNode->setVisible(false);
    lPatternNode->setDelegate(this);
    this->getScribble()->setPattern(lPatternNode->getLineArt());
    if(pAutoFill){
        lPatternNode->fillPatternAsync();
    }
    else{
        this->getDelegate()->loadEnded(this);
    }
    
    return true;
}


void ScribbleLineFrameNode::setLineFrame(Node *pLineFrame, Vec2 pPosition){
    if (nullptr != _lineFrameNode) {
        _lineFrameNode->removeFromParent();
    }
    _lineFrameNode = pLineFrame;
    if (nullptr != pLineFrame) {
        this->addChild(pLineFrame);
        pLineFrame->setPosition(pPosition);
        
        //    Texture2D::TexParams tp;
        //    tp.magFilter = GL_LINEAR;
        //    tp.minFilter = GL_NEAREST;
        //    tp.wrapS = GL_CLAMP_TO_EDGE;
        //    tp.wrapT = GL_CLAMP_TO_EDGE;
        //    _wireframe->getTexture()->setTexParameters(tp);
    }
}

void ScribbleLineFrameNode::paint(Sprite * lTarget, Point &pPosition)
{
    ScribbleNode::paint(lTarget, pPosition);
}

void ScribbleLineFrameNode::paint(Point pPosition){
    Vec2 lPointInPattern = _patternNode->convertToNodeSpace(pPosition);
    Rect lR = _patternNode->getLineArt()->getBoundingBox();
    if (!_patternNode->getLineArt()->getBoundingBox().containsPoint(lPointInPattern)) {
        return;
    }
    
    Color4B lColor = _patternNode->getPixelColor(lPointInPattern.x, _patternNode->getSize().height - lPointInPattern.y);//ScribbleUtil::getColor(, lPointInPattern);
    log("lTouchedColor %d %d %d %d", lColor.r, lColor.g, lColor.b, lColor.a);
    Color4F lTouchedColor(lColor);
    
    this->setPatternColor(lTouchedColor);
    
    ScribblePatternNode::paint(pPosition);
}

void ScribbleLineFrameNode::paint(Point pStartPosition, Point pEndPosition){
    ScribblePatternNode::paint(pStartPosition, pEndPosition);
}

void ScribbleLineFrameNode::debugPattern(bool pDebug/* = true*/){
    if (!pDebug) {
        return;
    }
    _debugPattern = true;
    if(nullptr != _lineFrameNode){
        _lineFrameNode->setVisible(false);
    }
    if(nullptr != _patternNode){
        _patternNode->setVisible(true);
    }
    if(_canvas){
        _canvas->clear(0.0, 0.0, 0.0, 0.0);
    }
}

void ScribbleLineFrameNode::loadPercent(Pattern *pNode, float pPercent){
    this->getDelegate()->loadPercent(this, pPercent);
}

void ScribbleLineFrameNode::loadEnded(Pattern *pNode){
    this->getDelegate()->loadEnded(this);
    if (_debugPattern) {
        this->debugPattern(true);
        string lPath = FileUtils::getInstance()->getWritablePath() + "pattern_debug.png";
        this->getPattern()->savePattern(lPath);
    }
}