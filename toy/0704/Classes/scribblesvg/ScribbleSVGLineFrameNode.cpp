//
//  ScribbleSVGLineFrameNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/9/8.
//
//

#include "ScribbleSVGLineFrameNode.h"
#include "SVGPattern.h"
ScribbleSVGLineFrameNode *ScribbleSVGLineFrameNode::create(string pLineArtFile){
    ScribbleSVGLineFrameNode *ret = new ScribbleSVGLineFrameNode();
    if(ret && ret->init(pLineArtFile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleSVGLineFrameNode::ScribbleSVGLineFrameNode(){
    _lineFrameNode = nullptr;
    _patternNode = nullptr;
    _debugPattern = false;
}

ScribbleSVGLineFrameNode::~ScribbleSVGLineFrameNode(){
    Texture2D *lTexture = _patternNode->getTexture();
    CC_SAFE_RELEASE(_patternNode);
    Director::getInstance()->getTextureCache()->removeTexture(lTexture);
}

bool ScribbleSVGLineFrameNode::init(string pLineArtFile){
    //renhong mark: 请尝试优化void SVGSource::optimize()，进一步加快加载SVG速度。
    SVGSprite *lLineArt = SVGSprite::create(pLineArtFile, SVGFill::FillState::eAuto);
    
    Size lSize = lLineArt->getContentSize();
    this->setContentSize(lSize);
    //设置SVG画布大小为Scribble画布大小，以免SVG离线绘制变形
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    RenderTexture *lRT = RenderTexture::create(lSize.width, lSize.height);
    //    lRT->clear(1.0, 0.0, 0.0, 1.0);
    lRT->begin();
    lLineArt->visit();
    lRT->end();
    Director::getInstance()->getRenderer()->render();
    
    //创建Pattern像素图
    Image *lPatternImage = lRT->newImage();
    CC_SAFE_RELEASE(_patternImage);
    _patternImage = lPatternImage;
    
    //创建Pattern精灵及纹理
    CC_SAFE_RELEASE(_patternNode);
    _patternNode = Sprite::createWithTexture(lRT->getSprite()->getTexture());//lRT->getSprite();
    Size drawSize = _patternNode->getContentSize();
    _patternNode->setPosition(Vec2(drawSize.width/2, drawSize.height/2));
    _patternNode->retain();
    
    lSize = Director::getInstance()->getWinSize();
    //重置SVG画布大小
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    //重置cocos2d视口大小
    Director::getInstance()->setViewport();
    
    //构建并初始化画布
    if (!ScribblePatternNode::init(drawSize)) {
        return false;
    }
    _canvas->getSprite()->getTexture()->setAntiAliasTexParameters();
    return true;
}

bool ScribbleSVGLineFrameNode::setPattern(string pSVGFile, SVGFill::FillState pState/* = SVGFill::FillState::eAuto*/){
    Sprite *lTemplate = Sprite::createWithTexture(_patternNode->getTexture());//lRT->getSprite()
    RenderTexture *lRT = SVGPattern::getInstance()->getPattern(lTemplate);
    //创建Pattern精灵及纹理
    _patternNode->setTexture(lRT->getSprite()->getTexture());
    _patternNode->setPosition(this->getCanvasSize() / 2);
    //设置Scribble Pattern
    this->getScribble()->setPattern(_patternNode);
    if(_debugPattern){
        this->addChild(_patternNode, -1);//默认不显示Pattern精灵，只在调试状态显示
        string lPath = "scribble_svg_pattern_debug.png";
        if(lRT->saveToFile(lPath)){
            log("save pattern at %s", lPath.c_str());
        }
        Director::getInstance()->getRenderer()->render();
    }
    return true;
}

void ScribbleSVGLineFrameNode::paint(Point pPosition){
    Vec2 lPointInPattern = this->convertToNodeSpace(pPosition) + _patternNode->getContentSize() / 2;
    
    Color4B lColor = ScribbleUtil::getInstance()->getColor(_patternImage, Vec2(lPointInPattern.x,lPointInPattern.y));
    CCLOG("lTouchedColor %d %d %d %d at Pattern %f %f (World %f %f)", lColor.r, lColor.g, lColor.b, lColor.a, lPointInPattern.x, _patternNode->getContentSize().height - lPointInPattern.y, pPosition.x, pPosition.y);
    Color4F lTouchedColor(lColor);
    
    this->setPatternColor(lTouchedColor);
    
    ScribblePatternNode::paint(pPosition);
}

void ScribbleSVGLineFrameNode::paint(Point pStartPosition, Point pEndPosition){
    ScribblePatternNode::paint(pStartPosition, pEndPosition);
}

void ScribbleSVGLineFrameNode::setLineFrame(Node *pLineFrame, Vec2 pPosition/* = Vec2::ZERO*/){
    if (nullptr != _lineFrameNode) {
        _lineFrameNode->removeFromParent();
    }
    _lineFrameNode = pLineFrame;
    if (nullptr != pLineFrame) {
        this->addChild(pLineFrame, 10);
        pLineFrame->setPosition(pPosition);
        pLineFrame->setAnchorPoint(Vec2(0.5, 0.5));
    }
}

void ScribbleSVGLineFrameNode::debugPattern(bool pDebug/* = true*/){
    if (!pDebug) {
        return;
    }
    _debugPattern = true;
    if(nullptr != _lineFrameNode){
        _lineFrameNode->setVisible(false);
    }
    if(nullptr != _patternNode){
        
    }
    if(_canvas){
        _canvas->clear(0.0, 0.0, 0.0, 0.0);
    }
}