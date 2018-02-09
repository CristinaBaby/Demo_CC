//
//  IceCreamScene.cpp
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/9.
//
//

#include "IceCreamScene.h"

string _toolPath[5] = {"cone0.png","handle0.png","switch0.png","light0.png","lid0.png"};

Vec2 _toolPos[5]={Vec2(368,275),Vec2(168,508),Vec2(366,527),Vec2(315,527),Vec2(239,625)};


enum toolIceType
{
    CONE = 0,
    HANDLE,
    SWITCH,
    LIGHT,
    LID,
    BoWL,
};

IceCreamScene::IceCreamScene()
{
    m_iCrushNum = 0;
    m_pCrush = nullptr;
}
IceCreamScene::~IceCreamScene()
{
    
}
bool IceCreamScene::init()
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
void IceCreamScene::onEnter()
{
    GameBaseScene::onEnter();
}
void IceCreamScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
    showHand1();
}
void IceCreamScene::onExit()
{
    GameBaseScene::onExit();
}
void IceCreamScene::initUI()
{
    m_pBG = Sprite::create("images/iceCream/icecream_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    m_pMachine = Sprite::create("images/iceCream/machine.png");
    m_pMachine->setAnchorPoint(Vec2(0.5,0));
    m_pBG->addChild(m_pMachine);
    m_pMachine->setPosition(Vec2(750,0));
    
    Vec2 pos = m_pBG->convertToWorldSpace(m_pMachine->getPosition());
    
    Rect coneRect = Rect(pos.x - 190,pos.y + 170,174,53);
    Rect bowlRect = Rect(pos.x - 200,pos.y + 609,174,53);
    
    for (int i=0;i<5; i++) {
        
        auto tool = ToolSprite::create(__String::createWithFormat("images/iceCream/%s",_toolPath[i].c_str())->getCString());
        tool->setPosition(_toolPos[i]);
        tool->setDelegate(this);
        tool->setIsMove(false);
        tool->cancelEvent(true);
        tool->setTag(i);
        m_pMachine->addChild(tool);
    }

    auto bowl = ToolSprite::create("images/iceCream/icecream_bowl0.png");
    bowl->setDelegate(this);
    bowl->setPosition(Vec2(268,245));
    bowl->setTag(BoWL);
    bowl->cancelEvent(true);
    bowl->addRect(bowlRect);
    m_pBG->addChild(bowl);
    
    //DEBUG
//    g_choose_waste = "chocolate";
    
    auto bowl_content = Sprite::create(__String::createWithFormat("images/iceCream/icecream_bowl0_%s.png",g_choose_waste.c_str())->getCString());
    bowl_content->setAnchorPoint(Vec2::ZERO);
    bowl->addChild(bowl_content);
    
    auto bowl_cover = Sprite::create("images/iceCream/icecream_bowl0_1.png");
    bowl_cover->setAnchorPoint(Vec2::ZERO);
    bowl->addChild(bowl_cover);
    
    
    

    
}
void IceCreamScene::showHand1()
{
    dynamic_cast<ToolSprite*>(m_pBG->getChildByTag(BoWL))->cancelEvent(false);
    
    auto hand = Sprite::create("images/iceCream/hand.png");
    hand->setPosition(m_pBG->convertToWorldSpace(Vec2(268,254)));
    hand->setTag(111);
    this->addToUILayer(hand);
    
    
    Vec2 pos = hand->getPosition();
    
    MoveTo* move = MoveTo::create(1.5,m_pBG->convertToWorldSpace(m_pMachine->getPosition()+Vec2(-200,609)));
   
    hand->runAction(RepeatForever::create(Sequence::create(move,CallFunc::create([=](){
        
        hand->setPosition(pos);
        
    }),Hide::create(),DelayTime::create(4.0),Show::create(),NULL)));
    
}
void IceCreamScene::showHand2(float dt)
{
    if(m_pMachine->getChildByTag(SWITCH)){
    
        dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(SWITCH))->cancelEvent(false);
    }
    
    auto hand = Sprite::create("images/iceCream/hand.png");
    hand->setPosition(m_pMachine->convertToWorldSpace(Vec2(368,524))+Vec2(61,-80));
    this->addToUILayer(hand);
    hand->setTag(112);
    
    Vec2 pos = hand->getPosition();
    
    hand->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,0.9),ScaleTo::create(0.5, 1.0), NULL)));
}
void IceCreamScene::showHand3()
{
    if(m_pMachine->getChildByTag(HANDLE)){
        
        dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(HANDLE))->cancelEvent(false);
    }
    
    
    auto hand = Sprite::create("images/iceCream/hand.png");
    hand->setPosition(m_pMachine->convertToWorldSpace(Vec2(156,504))+Vec2(61,-80));
    this->addToUILayer(hand);
    hand->setTag(113);
    
    hand->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,0.9),ScaleTo::create(0.5, 1.0), NULL)));
    

}
void IceCreamScene::crushAction(float dt)
{
    m_iCrushNum++;
    
    if (m_iCrushNum > 30) {
        
        this->unschedule(schedule_selector(IceCreamScene::crushAction));
        
        ToolSprite* tool = dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(HANDLE));
        tool->TouchEnded(nullptr, nullptr);
        tool->cancelEvent(true);
        
        showNextButton(true);
        
    }
    log("--->%d",m_iCrushNum);
    
    if (m_iCrushNum%10 == 0) {
        
        m_pCrush->setVisible(true);
        m_pCrush->setTexture(__String::createWithFormat("images/iceCream/cream_%s%d.png",g_choose_waste.c_str(),m_iCrushNum/10)->getCString());
        
    }
    
    
    
}
void IceCreamScene::coneAction(float dt)
{
    auto cone = Sprite::create("images/iceCream/cone1.png");
    cone->setPosition(m_pMachine->getChildByTag(CONE)->getPosition());
    m_pMachine->addChild(cone,0);
    
    
    
    auto cone_cover = Sprite::create("images/iceCream/cone1_1.png");
    cone_cover->setPosition(m_pMachine->getChildByTag(CONE)->getPosition());
    cone_cover->setTag(222);
    m_pMachine->addChild(cone_cover,100);
    
    auto func = CallFunc::create([=](){
    
        showHand3();
    
    });
    
    MoveTo* move = MoveTo::create(0.5, Vec2(150+10,220));
    cone->runAction(Sequence::create(move,func,nullptr));
    
    MoveTo* move1 = MoveTo::create(0.5, Vec2(150+10,220));
    cone_cover->runAction(Sequence::create(move1,nullptr));
    
    
    
}
void IceCreamScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag =toolSprite->getTag();
    oldPos = toolSprite->getPosition();
    switch (tag) {
        case BoWL:
            {
                if (this->m_pUILayer->getChildByTag(111)) {
                    this->m_pUILayer->getChildByTag(111)->stopAllActions();
                    this->m_pUILayer->getChildByTag(111)->removeFromParent();
                }
                
                
            }
            break;
        case SWITCH:
            {
                toolSprite->setTexture("images/iceCream/switch1.png");
                toolSprite->cancelEvent(true);
                dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(LIGHT))->setTexture("images/iceCream/light1.png");
                
                
                if (this->m_pUILayer->getChildByTag(112)) {
                    this->m_pUILayer->getChildByTag(112)->stopAllActions();
                    this->m_pUILayer->getChildByTag(112)->removeFromParent();
                }
                
                AudioHelp::getInstance()->playEffect("4.icecream machine1.mp3");
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("4.icecream machine1.mp3");
                });
                
                ActionUtils::delayTimeAction(this,2.0,func);
                
                this->scheduleOnce(schedule_selector(IceCreamScene::coneAction),4.0);
                
                
            }
            break;
        case HANDLE:
            {
                
                if (this->m_pUILayer->getChildByTag(113)) {
                    this->m_pUILayer->getChildByTag(113)->stopAllActions();
                    this->m_pUILayer->getChildByTag(113)->removeFromParent();
                }
                
//                toolSprite->cancelEvent(true);
                toolSprite->setTexture("images/iceCream/handle1.png");
                
                this->schedule(schedule_selector(IceCreamScene::crushAction), 0.1);
                
                
                auto crush = Sprite::create(__String::createWithFormat("images/iceCream/crush_%s0.png",g_choose_waste.c_str())->getCString());
                crush->setAnchorPoint(Vec2(0.5,1));
                crush->setPosition(Vec2(162,411));
                crush->setTag(102);
                m_pMachine->addChild(crush);
                
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/crush_%s0.png",g_choose_waste.c_str())->getCString());
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/crush_%s1.png",g_choose_waste.c_str())->getCString());
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/crush_%s2.png",g_choose_waste.c_str())->getCString());
               
                
                pOvenAnimation->setDelayPerUnit(0.75);
                pOvenAnimation->setLoops(-1);
                
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                crush->runAction(pOvenAnimat);
                
                if (!m_pCrush) {
                    
                    m_pCrush = Sprite::create(__String::createWithFormat("images/iceCream/cream_%s0.png",g_choose_waste.c_str())->getCString());
                    m_pCrush->setAnchorPoint(Vec2(0.5,0));
                    m_pCrush->setPosition(Vec2(45,70));
                    m_pCrush->setScale(0.9);
                    m_pCrush->setVisible(false);
                    m_pMachine->getChildByTag(222)->addChild(m_pCrush);
                }
                
            }
              break;
        default:
            break;
    }
    
    
    
    
}
void IceCreamScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    
    if (tag == BoWL) {
        toolSprite->cancelEvent(true);
        
        auto func = CallFunc::create([=](){
            
            if (m_pBG->getChildByTag(BoWL)) {
                
                dynamic_cast<ToolSprite*>(m_pBG->getChildByTag(BoWL))->cancelEvent(false);
            }
            
        });
        
        MoveTo* move = MoveTo::create(0.5, oldPos);
        toolSprite->runAction(Sequence::create(move,func, NULL));
        
    }else if (tag == HANDLE) {
        
        toolSprite->setTexture("images/iceCream/handle0.png");
        
        if (m_pMachine->getChildByTag(102)) {
            m_pMachine->getChildByTag(102)->stopAllActions();
            m_pMachine->getChildByTag(102)->removeFromParent();
        }
        this->unschedule(schedule_selector(IceCreamScene::crushAction));
    }
    
}
void IceCreamScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    int tag =toolSprite->getTag();
    switch (tag) {
        case BoWL:
        {
            toolSprite->cancelEvent(true);
            toolSprite->setVisible(false);
            
            dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(LID))->setTexture("images/iceCream/lid1.png");
            
            auto bowl = Sprite::create("images/iceCream/icecream_bowl1.png");
            bowl->setPosition(toolSprite->getPosition() + Vec2(0,20));
            m_pBG->addChild(bowl);
            
            bowl->setScale(0.5);
            
            
            auto bowl_content = Sprite::create(__String::createWithFormat("images/iceCream/icecream_bowl1_%s0.png",g_choose_waste.c_str())->getCString());
            bowl_content->setAnchorPoint(Vec2::ZERO);
            bowl->addChild(bowl_content);
            
            auto bowl_cover = Sprite::create("images/iceCream/icecream_bowl1_1.png");
            bowl_cover->setAnchorPoint(Vec2::ZERO);
            bowl->addChild(bowl_cover);
            
            auto pOvenAnimation = Animation::create();
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/icecream_bowl1_%s0.png",g_choose_waste.c_str())->getCString());
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/icecream_bowl1_%s1.png",g_choose_waste.c_str())->getCString());
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/iceCream/icecream_bowl1_%s2.png",g_choose_waste.c_str())->getCString());
            
            pOvenAnimation->setDelayPerUnit(0.75);
            pOvenAnimation->setLoops(1);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);

            auto func = CallFunc::create([=](){
            
                dynamic_cast<ToolSprite*>(m_pMachine->getChildByTag(LID))->setTexture("images/iceCream/lid0.png");
                bowl->runAction(FadeOut::create(0.5));
                bowl_cover->runAction(FadeOut::create(0.5));
                
                this->scheduleOnce(schedule_selector(IceCreamScene::showHand2),1.0);
            
            });
            
            
            
            bowl_content->runAction(Sequence::create(pOvenAnimat,RemoveSelf::create(),func, NULL));
            
        }
            break;
            
        default:
            break;
    }

}
void IceCreamScene::nextStep()
{
    log("next");
    hideNextButton();
    SceneChangeManager->enterDecScene();
}