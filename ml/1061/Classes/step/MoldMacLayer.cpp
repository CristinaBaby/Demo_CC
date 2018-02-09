//
//  MoldMacLayer.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/15.
//
//

#include "MoldMacLayer.h"
#include "AudioHelp.h"

#define WaterParticle 1000
Color4F colorMoldWater[5] ={Color4F((float)201/255,(float)120/255,(float)255/255,1.0),
    Color4F((float)120/255,(float)213/255,(float)34/255,1.0),
    Color4F((float)252/255,(float)229/255,(float)0/255,1.0),
    Color4F((float)240/255,(float)146/255,(float)0/255,1.0),
    Color4F((float)255/255,(float)65/255,(float)144/255,1.0),
  
};

MoldMacLayer::MoldMacLayer()
{
    m_bIsTouch = true;
    m_bIsFirst = true;
    m_iMoldType = -1;
    m_iMoldShape = -1;
    m_bIsOver= false;
}
MoldMacLayer::~MoldMacLayer()
{

}

bool MoldMacLayer::init()
{
    if (!LayerColor::init()) {
        return false;
    }
    
    _visbileSize  = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(_visbileSize.width,219));
    
//    m_fOffset = (_visbileSize.width - 4*253)/4;
    m_fOffset = 25;
    for (int i = 0;i<5;i++) {
        
        auto mac = Sprite::create(__String::createWithFormat("images/make_mold/mold/pourmachine_%s.png",fruitPath[i].c_str())->getCString());
//        mac->setPosition(Vec2((mac->getContentSize().width + m_fOffset)*i,219/2));
        mac->setPosition(Vec2(_visbileSize.width/2 + (mac->getContentSize().width + m_fOffset)*(i-2),219/2));
        this->addChild(mac);
        mac->setTag(i);
        m_vMacData.pushBack(mac);
        
        auto pushBtn = Sprite::create("images/make_mold/mold/push0.png");
        pushBtn->setTag(i);
        mac->addChild(pushBtn,10);
        pushBtn->setPosition(Vec2(108,48));
       
    }

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(MoldMacLayer::touchBegan, this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    m_pMacCenter = Sprite::createWithTexture(m_vMacData.at(2)->getTexture());
    m_pMacCenter->setPosition(Vec2(_visbileSize.width/2,219));
    this->addChild(m_pMacCenter,10);
    m_pMacCenter->setAnchorPoint(Vec2(0.5,1));
    m_pMacCenter->setScale(1.0);
    m_iMoldType = m_vMacData.at(2)->getTag();
//    m_pMacCenter->setOpacity(0);
//    m_pMacCenter->setVisible(false);
    
    m_pMacPushBtn = ToolSprite::create("images/make_mold/mold/push0.png");
    m_pMacPushBtn->setDelegate(this);
    m_pMacCenter->addChild(m_pMacPushBtn,10);
    m_pMacPushBtn->setPosition(Vec2(108,48));
    m_pMacPushBtn->setIsMove(false);
    
//    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/knifeParticl.plist");
//    emitter1->setPosition(Vec2(213,138));
//    emitter1->setAutoRemoveOnFinish(true);
//    emitter1->setDuration(-1);
////    emitter1->
//    emitter1->setContentSize(m_pMacPushBtn->getContentSize());
//    m_pMacPushBtn->addChild(emitter1, 30);
    
    m_vMacData.at(2)->setVisible(false);
    m_pMacPushBtn->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,0),DelayTime::create(4.0), NULL)));
    
    return true;
}
void MoldMacLayer::setMoldType(int i)
{

    m_iMoldShape = i;

}
void MoldMacLayer::showMacCenter()
{
    m_pMacCenter->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(1.0,1.2),1.0),CallFunc::create([=](){
        
        
        
        
    }), NULL));

}
bool MoldMacLayer::touchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 touch = pTouch->getLocation();
    
    if (m_bIsOver) {
        return true;
    }
    
    
    if (!m_bIsTouch) {
        return true;
    }
    
    if (m_bIsFirst && touch.y>_visbileSize.height-219) {
        m_bIsFirst = false;
        m_bIsTouch = false;
        if (firstBtnDown) {
            firstBtnDown();
        }
        m_bIsTouch = true;
        
        
         return true;
    }
    
    if (touch.x>_visbileSize.width/2 + 130 && touch.y>_visbileSize.height-219) {
    
        for (auto btn:m_vMacData) {
            btn->setVisible(true);
        }
        
        
        //点击右边
        m_bIsTouch = false;
        m_pMacPushBtn->cancelEvent(true);
        clickRight();
        
    }else if(touch.x<_visbileSize.width/2 - 130 && touch.y>_visbileSize.height-219){
    
        for (auto btn:m_vMacData) {
            btn->setVisible(true);
        }
        //点击左边
        m_bIsTouch = false;
        m_pMacPushBtn->cancelEvent(true);
        clickLeft();

    
    }
    return true;
}
void MoldMacLayer::setIsOver()
{
    m_bIsOver = true;
    

}
void MoldMacLayer::clickRight()
{
    log("right");
    
    auto mac = Sprite::createWithTexture(m_vMacData.at(0)->getTexture());
    mac->setPosition(Vec2(m_vMacData.at(m_vMacData.size()-1)->getPosition().x + mac->getContentSize().width+m_fOffset,219/2));
    this->addChild(mac);
    mac->setTag(m_vMacData.at(0)->getTag());
    m_vMacData.insert(m_vMacData.size(),mac);
    
    auto pushBtn = Sprite::create("images/make_mold/mold/push0.png");
    mac->addChild(pushBtn,10);
    pushBtn->setPosition(Vec2(108,48));
    int i = 0;
    
    for(Sprite* sp:m_vMacData)
    {
        i++;
        sp->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.3, Vec2(-mac->getContentSize().width-m_fOffset, 0))), CallFunc::create([=](){
        
            if (i == (int)m_vMacData.size()) {
                
               
                Texture2D* macTexture = nullptr;
//                for (auto mac:m_vMacData) {
//                    
//                    if (mac->getPosition().x == _visbileSize.width/2) {
//                        
//                        macTexture = mac->getTexture();
//                        m_iMoldType = mac->getTag();
//                    }
//                }
                m_vMacData.at(3)->setVisible(false);
                macTexture  = m_vMacData.at(3)->getTexture();
                m_iMoldType = m_vMacData.at(3)->getTag();
                m_vMacData.at(0)->removeFromParent();
                m_vMacData.erase(0);
                
                m_pMacCenter->runAction(Sequence::create(ScaleTo::create(0.1,1.0),CallFunc::create([=](){
                
                    m_pMacCenter->setTexture(macTexture);
                    
                }),ScaleTo::create(0.6, 1.2),CallFunc::create([=](){
                
                    m_bIsTouch = true;
                    m_pMacPushBtn->cancelEvent(false);
                
                }),nullptr));
                
                
            }
            
        
        }),NULL) );
        
    }
    
}
void MoldMacLayer::clickLeft()
{
    log("left");
    auto mac = Sprite::createWithTexture(m_vMacData.at(m_vMacData.size()-1)->getTexture());
    mac->setPosition(Vec2(m_vMacData.at(0)->getPosition().x-mac->getContentSize().width-m_fOffset,219/2));
    this->addChild(mac);
    mac->setTag(m_vMacData.at(m_vMacData.size()-1)->getTag());
    m_vMacData.insert(0,mac);
    
    auto pushBtn = Sprite::create("images/make_mold/mold/push0.png");
    mac->addChild(pushBtn,10);
    pushBtn->setPosition(Vec2(108,48));
    
    int i = 0;
    
    for(Sprite* sp:m_vMacData)
    {
        i++;
        sp->runAction(Sequence::create(EaseSineInOut::create(MoveBy::create(0.3, Vec2(mac->getContentSize().width+m_fOffset, 0))), CallFunc::create([=](){
            
            if (i == (int)m_vMacData.size()) {
                
                Texture2D* macTexture = nullptr;
//                for (auto mac:m_vMacData) {
//                    
//                    if (mac->getPosition().x == _visbileSize.width/2) {
//                        
//                        macTexture = mac->getTexture();
//                        m_iMoldType = mac->getTag();
//                    }
//                }
                m_vMacData.at(2)->setVisible(false);
                macTexture = m_vMacData.at(2)->getTexture();
                m_iMoldType = m_vMacData.at(2)->getTag();
                
                m_vMacData.at(m_vMacData.size()-1)->removeFromParent();
                m_vMacData.erase(m_vMacData.size()-1);
                m_bIsTouch = true;
               
                m_pMacCenter->runAction(Sequence::create(ScaleTo::create(0.1,1.0),CallFunc::create([=](){
                    
                    m_pMacCenter->setTexture(macTexture);
                    
                }),ScaleTo::create(0.6, 1.2),CallFunc::create([=](){
                    
                    m_bIsTouch = true;
                    m_pMacPushBtn->cancelEvent(false);
                    
                }),nullptr));
            }
            
            
        }),NULL) );
        
    }

}
void MoldMacLayer::hideMacCenter()
{
    for (auto btn:m_vMacData) {
        btn->setVisible(true);
    }
    m_pMacCenter->setCascadeOpacityEnabled(false);
    m_pMacCenter->runAction(FadeOut::create(0.5));
    m_pMacPushBtn->cancelEvent(true);
    m_pMacPushBtn->runAction(FadeOut::create(0.5));
}
void MoldMacLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->setTexture("images/make_mold/mold/push1.png");
    
    m_pMacPushBtn->stopAllActions();
    m_pMacPushBtn->setRotation(0);
    
    if (m_bIsFirst) {
        m_bIsFirst = false;
        
        if (firstBtnDown) {
            firstBtnDown();
        }
        m_bIsTouch = true;
//        return;
    }
    if(pushBtnDown){
    
        
        auto waterParticle = ParticleSystemQuad::create("particle/waterDown.plist");
        waterParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        waterParticle->setVisible(true);
        
        if(m_iMoldShape == 1){
        
             waterParticle->setLife(0.4);
        
        }
//        waterParticle->setRotation(180);
        waterParticle->setScaleY(0.7);
        waterParticle->setAnchorPoint(Vec2(0.5,0));
        waterParticle->setPosition(Vec2(110,10));
        
        
        waterParticle->setStartColor(colorMoldWater[m_iMoldType]);
        //    waterParticle->setStartColor(Color4F(255/255,0,0,255/255));
        waterParticle->setEndColor(colorMoldWater[m_iMoldType]);
        m_pMacCenter->addChild(waterParticle,-1);
        waterParticle->setTag(WaterParticle);
        
        pushBtnDown(m_iMoldType);
        
        AudioHelp::getInstance()->playLoofEffect("pour liquid.mp3");
    }
}
void MoldMacLayer::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
    m_pMacPushBtn->runAction(RepeatForever::create(Sequence::create(DelayTime::create(4.0),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,0),DelayTime::create(4.0), NULL)));
    
    toolSprite->setTexture("images/make_mold/mold/push0.png");
    if (pushBtnUp) {
        
        if (m_pMacCenter->getChildByTag(WaterParticle)) {
            
            m_pMacCenter->getChildByTag(WaterParticle)->removeFromParent();
            
        }
        AudioHelp::getInstance()->stopLoofEffect();
        
        pushBtnUp();
    }
}
void MoldMacLayer::onEnter()
{
    LayerColor::onEnter();
}
void MoldMacLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
}
void MoldMacLayer::onExit()
{
    LayerColor::onExit();
}