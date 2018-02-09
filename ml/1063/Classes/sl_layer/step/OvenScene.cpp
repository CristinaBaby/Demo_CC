//
//  OvenScene.cpp
//  CCATS016_CakeMaker
//
//  Created by wusonglin1 on 15/9/18.
//
//

#include "OvenScene.h"
#include "MainScene.h"
#include "ActionHelper.h"

REGIST_SECENE(OvenScene);
OvenScene::OvenScene():m_bOvenOver(false)
{
   
    m_pHand = nullptr;
    m_bOvenClose=true;
    m_bSitchClose = true;
    m_iOvenNum= 0;
    
    
}
OvenScene::~OvenScene()
{
    
}
bool OvenScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
    
}
void OvenScene::onEnter()
{
    GameBaseScene::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}
void OvenScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
//    showHand1();
    
    ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
        
        m_pOvenCover->setTexture(g_path + "oven/oven_open.png");
//        m_pOvenCover->cancelEvent(true);
        m_pOvenCover->cancelEvent(true);
        
        AudioHelp::getInstance()->playEffect("oven_open.mp3");
        
        
        showCake();
        
    }));
    
    
}
void OvenScene::onExit()
{
    GameBaseScene::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void OvenScene::initUI()
{
    m_pBG = Sprite::create(g_path + "oven/oven_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
    //    showNextButton(true);
//    showNextButton(true);
    
    auto oven = Sprite::create(g_path + "oven/oven.png");
    oven->setPosition(Vec2(384,626 - 90));
    oven->setTag(101);
    m_pBG->addChild(oven);

    m_pSwitch = ToolSprite::create((g_path + "oven/red.png").c_str());
    m_pSwitch->setDelegate(this);
    m_pSwitch->setIsMove(false);
    m_pSwitch->setTag(300);
    m_pSwitch->setPosition(Vec2(629,748));
    m_pSwitch->cancelEvent(true);
    m_pSwitch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    oven->addChild(m_pSwitch);

    
    m_pOvenCover = ToolSprite::create((g_path + "oven/oven_close.png").c_str());
    m_pOvenCover->setDelegate(this);
    m_pOvenCover->setIsMove(false);
    m_pOvenCover->setTag(100);
    m_pOvenCover->cancelEvent(true);
    m_pOvenCover->setAnchorPoint(Vec2::ZERO);
    oven->addChild(m_pOvenCover);
    m_pOvenCover->setLocalZOrder(100);
    
    
    
    Vec2 pos = m_pBG->convertToWorldSpace(oven->getPosition());
    
    
    m_rOverRect = Rect(pos.x-161,pos.x-41 + 250 - 180,250 + 100,160 + 100);
    
}
void OvenScene::showHand(Vec2 pos)
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(pos);
        this->addChild(m_pHand,100);
    
        if (this->m_pBackGroundLayer->getChildByTag(200)) {
            
            
            dynamic_cast<ToolSprite*>(this->m_pBackGroundLayer->getChildByTag(200))->cancelEvent(false);
            
        }
        
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
            
        }),MoveTo::create(1.0,Vec2(m_rOverRect.getMidX(),m_rOverRect.getMidY())),CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            m_pHand->setPosition(pos);
            
            
        }),DelayTime::create(2.0), NULL)));
        
    
    
    }
    

}
void OvenScene::showHand1()
{
    if (!m_pHand) {

        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(Vec2(324,374));
        m_pOvenCover->addChild(m_pHand,100);
        
        MoveBy* move1 = MoveBy::create(1.0, Vec2(0,-170));
        MoveBy* move2 = MoveBy::create(1.0, Vec2(0,-170));
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(move1,CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            
        }),move1->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),move2,CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            
        }),move2->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),NULL)));
        
        m_pOvenCover->cancelEvent(false);
    }
    
    
    
}
void OvenScene::showHand2()
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(Vec2(329,56));
        m_pOvenCover->addChild(m_pHand,100);
        
        m_pOvenCover->cancelEvent(false);
        
        //需要关门
        m_bOvenClose = false;
        
        MoveBy* move1 = MoveBy::create(1.0, Vec2(0,170));
        ScaleBy* scale1 = ScaleBy::create(1.0,0.7);
        Spawn* sp1 = Spawn::create(move1, scale1,NULL);
        
        
        MoveBy* move2 = MoveBy::create(1.0, Vec2(0,170));
        ScaleBy* scale2 = ScaleBy::create(1.0,0.7);
        Spawn* sp2 = Spawn::create(move2,scale2,NULL);
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(sp1,CallFunc::create([=](){
        
            m_pHand->setVisible(false);
        
        }),sp1->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),sp2,CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            
        }),sp2->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),NULL)));
        
        m_pOvenCover->cancelEvent(false);
    }

}
void OvenScene::showHand3()
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("cocostudio/ui/tips/finger_2.png");
        m_pHand->setAnchorPoint(Vec2(0.93,0.55));
        m_pHand->setPosition(Vec2(629,748));
        m_pOvenCover->addChild(m_pHand,100);
        m_pHand->setPositionX(m_pHand->getPositionX() - 40);
        
        MoveBy* move = MoveBy::create(0.5, Vec2(+40,0));
        ScaleBy* scale = ScaleBy::create(0.25, 0.9, 1.0);
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(move,scale,scale->reverse(),move->reverse(), NULL)));
        m_pSwitch->cancelEvent(false);
    }
}
void OvenScene::showHand4()
{
    if (!m_pHand) {
        
        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(Vec2(324,374));
        m_pOvenCover->addChild(m_pHand,100);
        
        
        MoveBy* move1 = MoveBy::create(1.0, Vec2(0,-170));
        MoveBy* move2 = MoveBy::create(1.0, Vec2(0,-170));
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(move1,CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            
        }),move1->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),move2,CallFunc::create([=](){
            
            m_pHand->setVisible(false);
            
        }),move2->reverse(),CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),NULL)));
        
        m_pOvenCover->cancelEvent(false);
        
        m_bOvenOver=true;
        m_bOvenClose=true;
    }

}
void OvenScene::showCake()
{
    auto mould = ToolSprite::create((g_path + "oven/mould0.png").c_str());
    kAdapterScreen->setExactPosition(mould, 0 - 400, 120,Vec2(1,0),kBorderRight,kBorderNone);
    this->addToBGLayer(mould);
    mould->setDelegate(this);
    mould->setTag(200);
    mould->addRect(m_rOverRect);
    mould->cancelEvent(true);
//    g_choose_waste = "strawberry";
    
    m_pCake = Sprite::create(__String::createWithFormat("%soven/bake_%s0.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
    m_pCake->setAnchorPoint(Vec2::ZERO);
    mould->addChild(m_pCake);
    
    auto mouldCover = Sprite::create(g_path + "oven/mould1.png");
    mouldCover->setAnchorPoint(Vec2::ZERO);
    mould->addChild(mouldCover,10);
    
    
    auto func = CallFunc::create([=](){
    
//        showHand2();
        ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
        
            showHand(mould->getPosition() + Vec2(-100,100));
        
        }));
        
    });
    
    AudioHelp::getInstance()->playEffect("swish.mp3");
    MoveBy* move =  MoveBy::create(0.5, Vec2(-400,0));
    mould->runAction(Sequence::create(move,func, NULL));
    
    
    ActionHelper::showBezier(mould, mould->getPosition(), ActionHelper::ShowDirection::show_from_right);
    
    
}
void OvenScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if(tag == 200){
    
    
        if (m_pHand) {
            m_pHand->stopAllActions();
            m_pHand->removeFromParent();
            m_pHand = nullptr;
        }
        
    
    }
    
    
    if (tag == 300) {
        
        if(m_bSitchClose){
        
            if (m_pHand) {
                m_pHand->stopAllActions();
                m_pHand->removeFromParent();
                m_pHand = nullptr;
            }
            //开关音效
            AudioHelp::getInstance()->playLoofEffect("baking.mp3");
            m_pSwitch->cancelEvent(true);
            m_pSwitch->setTexture(g_path + "oven/green.png");
            
            this->schedule(schedule_selector(OvenScene::startOven), 1.0);

            auto fireCover = Sprite::create("cocostudio/png/cake/oven/oven_close1.png");
            fireCover->setAnchorPoint(Vec2::ZERO);
            m_pOvenCover->addChild(fireCover,-1);
            fireCover->setTag(444);
            
            auto fire = Sprite::create(g_path + "oven/fire0.png");
//            fire->setPosition(Vec2(377,451 - 50));
            fire->setAnchorPoint(Vec2::ZERO);
            fire->setTag(333);
            m_pOvenCover->addChild(fire,2);
            
            auto pOvenAnimation = Animation::create();
            
            pOvenAnimation->addSpriteFrameWithFile(g_path + "oven/fire0.png");
            pOvenAnimation->addSpriteFrameWithFile(g_path + "oven/fire1.png");
            
            pOvenAnimation->setDelayPerUnit(0.5);
            pOvenAnimation->setLoops(-1);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            
            fire->runAction(pOvenAnimat);
            
        }else{
            m_pSwitch->cancelEvent(true);
            m_pSwitch->setTexture(g_path + "oven/red.png");
            
            
            if (m_pHand) {
                m_pHand->stopAllActions();
                m_pHand->removeFromParent();
                m_pHand = nullptr;
            }
            
            //打开门
            m_bOvenClose=false;
            m_pOvenCover->cancelEvent(false);
//            showHand4();
            
            m_pOvenCover->setTexture(g_path + "oven/oven_open.png");
            m_pOvenCover->cancelEvent(true);
            
            AudioHelp::getInstance()->playEffect("oven_open.mp3");
            
            dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(444))->stopAllActions();
            dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(444))->setVisible(false);
            
            auto func = CallFunc::create([=](){
                
                auto bowl = dynamic_cast<ToolSprite*>(m_pBG->getChildByTag(101)->getChildByTag(103));
                bowl->runAction(Sequence::create(Spawn::create(MoveTo::create(1.5,Vec2(389,739 - 200)),ScaleTo::create(1.5,1.2),nullptr),CallFunc::create([=](){
                    
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");
                    emitter1->setPosition(m_pCake->getContentSize()/2);
                    emitter1->setAutoRemoveOnFinish(true);
                    bowl->addChild(emitter1,100);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                }), DelayTime::create(2.0),CallFunc::create([=](){
                    SceneChangeManager->enterCream();
                }),NULL));
                //                        showNextButton(true);
            });
            
            
            ActionUtils::delayTimeAction(this,0.5,func);
        
        }
    }

}
void OvenScene::startOven(float dt)
{
    m_iOvenNum++;
    //开火音效
    
   
    
    if(m_iOvenNum == 6){
    
        //停止循环音效
        //烤制完成音效
        
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        
        AudioHelp::getInstance()->playEffect("baking ok.mp3");
        
        dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(333))->stopAllActions();
        dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(333))->setVisible(false);
        
        this->unschedule(schedule_selector(OvenScene::startOven));
        
        m_pCake->setTexture(__String::createWithFormat("%soven/bake_%s1.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
        
        //关火
        m_bSitchClose=false;
        showHand3();
        
        
       
    }
    
}
void OvenScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    toolSprite->cancelEvent(true);
    
    toolSprite->retain();
    toolSprite->removeFromParent();
    toolSprite->setAnchorPoint(Vec2(0.5,0.5));
    toolSprite->setPosition(Vec2(375,409));
    toolSprite->setScale(0.8);
    m_pBG->getChildByTag(101)->addChild(toolSprite);
    toolSprite->setTag(103);

//    showHand2();
    
    ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("oven_closedoor.mp3");
        m_pOvenCover->setTexture(g_path + "oven/oven_close.png");
        
        showHand3();
    }));
    

}
void OvenScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if (tag == 100) {
        
        if(m_bOvenClose){
        
            if (pTouch->getDelta().y < - 25) {
                
                if (m_pHand) {
                    m_pHand->stopAllActions();
                    m_pHand->removeFromParent();
                    m_pHand = nullptr;
                }
                
                toolSprite->setTexture(g_path + "oven/oven_open.png");
                m_pOvenCover->cancelEvent(true);
                
                AudioHelp::getInstance()->playEffect("oven_open.mp3");
                
                if(!m_bOvenOver){
                    
                    showCake();
                }else{
                    
                    dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(444))->stopAllActions();
                    dynamic_cast<Sprite*>(m_pOvenCover->getChildByTag(444))->setVisible(false);
                    
                    auto func = CallFunc::create([=](){
             
                        auto bowl = dynamic_cast<ToolSprite*>(m_pBG->getChildByTag(101)->getChildByTag(103));
                        bowl->runAction(Sequence::create(Spawn::create(EaseElasticInOut::create(MoveTo::create(1.5,Vec2(389,739 - 200)),1.0),ScaleTo::create(1.5,1.2),nullptr),CallFunc::create([=](){
                        
                        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");
                            emitter1->setPosition(m_pCake->getContentSize()/2);
                            emitter1->setAutoRemoveOnFinish(true);
                            bowl->addChild(emitter1,100);
                            AudioHelp::getInstance()->playEffect("all star .mp3");
                        }), DelayTime::create(2.0),CallFunc::create([=](){
                            SceneChangeManager->enterCream();
                        }),NULL));
//                        showNextButton(true);
                    });
                    
                    
                    ActionUtils::delayTimeAction(this,0.5,func);
                    
                    
                }
                
                
            }
        }else{
            
            if (pTouch->getDelta().y > 25) {
                
                if (m_pHand) {
                    m_pHand->stopAllActions();
                    m_pHand->removeFromParent();
                    m_pHand = nullptr;
                }
                AudioHelp::getInstance()->playEffect("oven_closedoor.mp3");
                toolSprite->setTexture(g_path + "oven/oven_close.png");
                m_pOvenCover->cancelEvent(true);
                
                showHand3();
            }
        }
    }
}
void OvenScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
    

}


void OvenScene::nextStep()
{
    log("next");
    hideNextButton();
    SceneChangeManager->enterCream();
}