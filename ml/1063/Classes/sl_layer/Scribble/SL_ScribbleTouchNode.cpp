//
//  SL_ScribbleTouchNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#include "SL_ScribbleTouchNode.h"
#include "ActionUtils.h"
#include "AudioHelp.h"

SL_ScribbleTouchNode *SL_ScribbleTouchNode::create(Size pCanvasSize){
    
    
    SL_ScribbleTouchNode *ret = new SL_ScribbleTouchNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

SL_ScribbleTouchNode::SL_ScribbleTouchNode(){
    _enableTouchBegan = true;
    _enableTouchMoved = true;
    _enableTouchEnded = true;
    m_pDelegate = nullptr;
    m_iCount = 1;
}

SL_ScribbleTouchNode::~SL_ScribbleTouchNode(){
    this->getEventDispatcher()->removeEventListener(_touchListener);
    _touchListener->release();
}

bool SL_ScribbleTouchNode::init(Size pCanvasSize){
    if ( !ScribbleNode::init(pCanvasSize) ){
        return false;
    }
  
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->retain();
    _touchListener->setSwallowTouches(false);
    _touchListener->onTouchBegan = CC_CALLBACK_2(SL_ScribbleTouchNode::touchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(SL_ScribbleTouchNode::touchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(SL_ScribbleTouchNode::touchEnded, this);
    
   
        
    this->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener,1);
        
   
    
    
    return true;
}

bool SL_ScribbleTouchNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        if (_enableTouchBegan) {
            log("touch");
            
            Point lTouchPoint = pTouch->getLocation();
            this->paint(lTouchPoint);
            
            //设置代理
            if (m_pDelegate) {
                m_pDelegate->onScribbleTouchBegan(pTouch,pEvent);
            }
        }
        return true;
    }
    
    return false;
}

void SL_ScribbleTouchNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchMoved) {
        Point lTouchPoint = pTouch->getLocation();
        Point lPreTouchPoint = pTouch->getPreviousLocation();
        this->paint(lTouchPoint, lPreTouchPoint);
        
        
        if (m_pDelegate) {
            m_pDelegate->onScribbleTouchMoved(pTouch,pEvent);
        }
    }
}

void SL_ScribbleTouchNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchEnded) {
        
        m_iCount++;
        
        if (m_pDelegate) {
            m_pDelegate->onScribbleTouchEnded(pTouch,pEvent);
        }
        
        if (m_pDelegate) {
            if (m_iCount%8 == 0) {
                
                //检测是擦除完成
                if (getPecent() < 0.21) {
                    //代理
                    if (m_pDelegate) {
                        
                        m_pDelegate->onScribbleTouchOver();
                    }
                    
                    
                }
                
            }

        }
        
        
                        

    }
}
void SL_ScribbleTouchNode::setDelegate(SL_ScribbleTouchDelegate* delegate)
{
    m_pDelegate = delegate;

}
float SL_ScribbleTouchNode::getPecent()
{
    return calcute(getRender()->newImage())/(restorePixes*1.0f);

}
bool SL_ScribbleTouchNode::useTarget(string pTargetFile){
    auto image = new Image();
    image->initWithImageFile(pTargetFile);
    restorePixes = calcute(image);
    return ScribbleNode::useTarget(pTargetFile);
}
bool SL_ScribbleTouchNode::useTarget(Sprite* sprite){

    
    RenderTexture* rt = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    rt->begin();
    
    sprite->visit();
    
    rt->end();
    Director::getInstance()->getRenderer()->render();
    auto image = rt->newImage();
    restorePixes = calcute(image);
    return ScribbleNode::useTarget(sprite);
}
//获取不透明像素点
long SL_ScribbleTouchNode::calcute(Image *pImage){
    long lCounter = 1;
    for (long i = 0; i < pImage->getHeight(); ++i) {
        for (long j = 0; j < pImage->getWidth(); ++j) {
            unsigned char *lData = pImage->getData();
            long lPixcelIndex = i * pImage->getWidth() + j;
            unsigned char lRed = lData[lPixcelIndex * 4];
            unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
            unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
            unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 10) {
                if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                    ++lCounter;
                }
            }
        }
    }
    delete pImage;
    return lCounter;
}
