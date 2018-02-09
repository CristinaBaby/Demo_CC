//
//  ScribbleTouchNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#include "ScribbleTouchNode.h"
#include "ActionUtils.h"
#include "AudioHelp.h"

ScribbleTouchNode *ScribbleTouchNode::create(Size pCanvasSize,int type){
    
    
    ScribbleTouchNode *ret = new ScribbleTouchNode();
    if(ret && ret->init(pCanvasSize,type))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleTouchNode::ScribbleTouchNode(){
    _enableTouchBegan = true;
    _enableTouchMoved = true;
    _enableTouchEnded = true;
    m_bSound = true;
    m_iCount = 1;
}

ScribbleTouchNode::~ScribbleTouchNode(){
    this->getEventDispatcher()->removeEventListener(_touchListener);
    _touchListener->release();
}

bool ScribbleTouchNode::init(Size pCanvasSize,int type){
    if ( !ScribbleNode::init(pCanvasSize) ){
        return false;
    }
    m_iType = type;
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->retain();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = CC_CALLBACK_2(ScribbleTouchNode::touchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(ScribbleTouchNode::touchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(ScribbleTouchNode::touchEnded, this);
    
    if (m_iType == 1) {
        
        this->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener,1);
        
    }else {
        
        this->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, 1);
    
    }
    
    
    return true;
}

bool ScribbleTouchNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        if (_enableTouchBegan) {
            log("touch");
            
            __NotificationCenter::getInstance()->postNotification("ScribbleTouchBeagn");
            
            if (m_iType == 1) {
            
                AudioHelp::getInstance()->playEffect("glass.mp3");
                
            }else{
            
                AudioHelp::getInstance()->playEffect("eat.mp3");
                
                ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/biting.plist");
                emitter1->setDuration(0.2);
                //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
                emitter1->setAutoRemoveOnFinish(true);
                emitter1->setPosition(pTouch->getLocation());
                this->getParent()->addChild(emitter1, 30);
                
            }
            
            
            Point lTouchPoint = pTouch->getLocation();
            this->paint(lTouchPoint);
            
            
            
        }
        return true;
    }
    
    return false;
}
void ScribbleTouchNode::setEatSceneType()
{
    m_iType = 1 ;

}
void ScribbleTouchNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchMoved) {
        Point lTouchPoint = pTouch->getLocation();
        Point lPreTouchPoint = pTouch->getPreviousLocation();
        this->paint(lTouchPoint, lPreTouchPoint);
        
        if (m_iType == 1) {
            
            
            if (m_bSound) {
                m_bSound = false;
                AudioHelp::getInstance()->playEffect("glass.mp3");
                
                    auto func = CallFunc::create([&](){
                
                       m_bSound = true;
                        
                    });
                
                ActionUtils::delayTimeAction(this,1,func);
            }
            
            
        }
        
        
    }
}

void ScribbleTouchNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchEnded) {
        
        m_iCount++;
        
       
        if (m_iType == 1) {
            
//            AudioHelp::getInstance()->playLoofEffect("glass.mp3");
            CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
            
        }else{
            
            AudioHelp::getInstance()->playEffect("eat.mp3");
        }
        
       
        //消息者模式，发送擦拭完成的消息
        if(m_iType == 1){
            
            if(m_iCount%2  == 0){
                    
                if (getPecent() < 0.21) {
                    __NotificationCenter::getInstance()->postNotification("ScribbleOver");
                }
                    
            }
        }else if(m_iType == 0){
                   
        if(m_iCount%5  == 0){
                        
            if (getPecent() < 0.21) {
                    __NotificationCenter::getInstance()->postNotification("eatOver");
                }
                        
            }
        }
     
       
        
//        if (getPecent() < 0.11) {
//            //消息者模式，发送擦拭完成的消息
//            __NotificationCenter::getInstance()->postNotification("eatOver");
//        }
//        Point lTouchPoint = pTouch->getLocation();
//        this->paint(lTouchPoint);
    }
}
float ScribbleTouchNode::getPecent()
{
    return calcute(getRender()->newImage())/(restorePixes*1.0f);

}
bool ScribbleTouchNode::useTarget(string pTargetFile){
    auto image = new Image();
    image->initWithImageFile(pTargetFile);
    restorePixes = calcute(image);
    return ScribbleNode::useTarget(pTargetFile);
}
bool ScribbleTouchNode::useTarget(Sprite* sprite){

    
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
long ScribbleTouchNode::calcute(Image *pImage){
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
