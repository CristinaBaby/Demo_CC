//
//  ScribbleSauce.cpp
//  MLABS1059
//
//  Created by wusonglin1 on 15/11/27.
//
//

#include "ScribbleSauce.h"
ScribbleSauce::ScribbleSauce()
{
    _drawing = false;
    m_tBrush = nullptr;
    callBack = nullptr;
    _drawColor = Color3B::WHITE;
    _brushScale = 1.0f;
    _touchBeginEnable = false;
}
ScribbleSauce::~ScribbleSauce()
{
    if (nullptr != m_rCanvas) {
        m_rCanvas->release();
    }
}

ScribbleSauce* ScribbleSauce::create(Size contentSize)
{
    ScribbleSauce *ret = new ScribbleSauce();
    if(ret && ret->init(contentSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
    
}
bool ScribbleSauce::init(Size contentSize)
{
    if (!Node::init()) {
        return false;
    }
    m_sContentSize = contentSize;
    
    this->setContentSize(m_sContentSize);
    m_rCanvas=RenderTexture::create(m_sContentSize.width,m_sContentSize.height);
    m_rCanvas->setPosition(m_sContentSize/2);
    this->addChild(m_rCanvas);
    m_rCanvas->retain();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan=[this](Touch *touch, Event *unusedEvent){
        
        if(callBack != nullptr)
            callBack(touch,touchType::touchBegin);
        if(_touchBeginEnable&&m_tBrush&&_drawing){
            vector<Vec2> _points;
            _points.push_back(touch->getLocation());
            paint(m_rCanvas,_points,m_tBrush);
        }
        return true;
    };
    listener->onTouchMoved=[this](Touch *touch, Event *unusedEvent){
        
        if(!_drawing)
            return;
        
        if (m_tBrush) {
            if(callBack != nullptr)
                callBack(touch,touchType::touchMove);
            vector<Point> lPositions = interpolate(touch->getPreviousLocation(),touch->getLocation(),1);
            this->paint(m_rCanvas, lPositions,m_tBrush);
        }
    };
    listener->onTouchEnded=[this](Touch *touch, Event *unusedEvent){
        if(callBack != nullptr)
            callBack(touch,touchType::touchEnd);
    };
    listener->onTouchCancelled=[this](Touch *touch, Event *unusedEvent){
        if(callBack != nullptr)
            callBack(touch,touchType::touchCancle);
    };
    listener->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    return true;
}
//线性插值
vector<Point> ScribbleSauce::interpolate(Point pStartPosition, Point pEndPosition, float pDelta)
{
    float lDistance = pStartPosition.getDistance(pEndPosition);
    int lDis = (int)lDistance;
    vector<Point> lPositions;
    for (int i = 0; i < lDis; i += pDelta) {
        float lDelta = float(i) / lDistance;
        float lDifX = pEndPosition.x - pStartPosition.x;
        float lDifY = pEndPosition.y - pStartPosition.y;
        
        Point lPosition(pStartPosition.x + (lDifX * lDelta), pStartPosition.y + (lDifY * lDelta));
        lPositions.push_back(lPosition);
    }
    return lPositions;
}
//绘制
void ScribbleSauce::paint(RenderTexture *pCanvas, vector<Point> pWorldPositions,Texture2D* texture)
{
    Sprite *lBrush = Sprite::createWithTexture(texture);
    lBrush->setColor(_drawColor);
    lBrush->setScale(_brushScale);
    //lBrush->setBlendFunc({GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA});
    //lBrush->setBlendFunc({GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA});
    for (int i = 0; i < pWorldPositions.size(); ++i) {
        Point lPosition = this->convertToNodeSpace(pWorldPositions.at(i));
        //        Point lPosition = pWorldPositions.at(i);
        lBrush->setPosition(lPosition);
        pCanvas->begin();
        
        lBrush->visit();
        
        pCanvas->end();
        Director::getInstance()->getRenderer()->render();
        pCanvas->getSprite()->getTexture()->setAntiAliasTexParameters();
        
    }
    
}
//设置画刷
void ScribbleSauce::setBrush(Sprite* brush)
{
    
    m_tBrush = brush->getTexture();
    
}
void ScribbleSauce::setBrush(Texture2D* brush)
{
    
    m_tBrush = brush;
    
}

void ScribbleSauce::paint(const Vec2& wordPos){
    
    if(m_rCanvas != nullptr&& m_tBrush != nullptr) {
        auto lBrush = Sprite::createWithTexture(m_tBrush);
           //lBrush->setColor(_drawColor);
        Point lPosition = this->convertToNodeSpace(wordPos);
        lBrush->setPosition(lPosition);
        m_rCanvas->begin();
        lBrush->visit();
        m_rCanvas->end();
        Director::getInstance()->getRenderer()->render();
        m_rCanvas->getSprite()->getTexture()->setAntiAliasTexParameters();
    }
    
}

void ScribbleSauce::reset(){
    if(m_rCanvas != nullptr)
        m_rCanvas->clear(0, 0, 0, 0);
}

