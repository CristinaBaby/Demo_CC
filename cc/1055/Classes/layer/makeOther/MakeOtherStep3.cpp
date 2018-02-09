//
//  MakeOtherStep3.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#include "MakeOtherStep3.h"

REGIST_SECENE(MakeOtherStep3)

MakeOtherStep3::MakeOtherStep3()
{
    m_bIsRoll = false;
    m_iRollNum = 0;
    m_bIsSound = false;
}
MakeOtherStep3::~MakeOtherStep3()
{
}
bool MakeOtherStep3::init()
{
    
    if(!BaseLayer::init())
        return false;
    
    m_sFlovar = GameDataInstance->getFlavor();
    
    auto bg = Sprite::create("cocostudio/Make4/bg.jpg");
    kAdapterScreen->setExactPosition(bg,480,320);
    this->addChild(bg);
  
    m_pSpoon = Sprite::create("cocostudio/MakeOther1/rollingpin.png");
    kAdapterScreen->setExactPosition(m_pSpoon,680,520);
    this->addChild(m_pSpoon);
    
    m_pPaste = Sprite::create(__String::createWithFormat("cocostudio/MakeOther1/paste_%s0.png",m_sFlovar.c_str())->getCString());
    kAdapterScreen->setExactPosition(m_pPaste,480,320);
    this->addChild(m_pPaste);

    auto lisenter = MoveLisenter::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pSpoon);
    lisenter->setOrginOffset();
    lisenter->addRectByNode(m_pPaste);
    
    lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep3::onTouchDown, this);
    lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep3::onTouchMoveInRect, this);
    lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep3::onTouchUp, this);
    
    auto title = Sprite::create("cocostudio/UI/tips/Rolldough.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    
    AudioHelp::getInstance()->playEffect("present in.mp3");
    ActionHelper::showBackInOut(m_pPaste,0.5,ActionHelper::ShowDirection::show_from_left);
    ActionHelper::showBezier(m_pSpoon,m_pSpoon->getPosition(), ActionHelper::ShowDirection::show_from_right,1.0,[=](){
    
        
        if (this->getChildByTag(1001)) {
            
            this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
             AudioHelp::getInstance()->playEffect("VoiceOver/roll the dough.mp3");
        }
        auto tip = Sprite::create("cocostudio/UI/tips/arrows.png");
        tip->setRotation(90);
        m_pPaste->addChild(tip);
        tip->setPosition(m_pPaste->getContentSize()/2);
        tip->setTag(101);
        tip->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.0,1.5,1.0),ScaleTo::create(1.0,1.0,1.0),NULL)));
    });
    
    return true;
}
void MakeOtherStep3::onEnter()
{
    BaseLayer::onEnter();
}
void MakeOtherStep3::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherStep3::onExit()
{
    BaseLayer::onExit();
    
}
void MakeOtherStep3::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if (m_pPaste->getChildByTag(101)) {
        
        m_pPaste->getChildByTag(101)->removeFromParent();
        
    }
   
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
}
void MakeOtherStep3::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    if(!m_bIsSound){
    
        m_bIsSound = true;
        
        AudioHelp::getInstance()->playLoofEffect("roll the dough.mp3");
    
    }
    
    
    m_iRollNum++;
    
    if (!m_bIsRoll&&m_iRollNum%50==0) {
        m_bIsRoll = true;
        
        ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
        ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
        ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
        m_pPaste->runAction(Sequence::create(s1,s2,s3,DelayTime::create(0.5),CallFunc::create([=](){
            
            m_bIsRoll = false;
            
        }),nullptr));
    }
    
    if (m_iRollNum%100 == 0) {
        log("%d",m_iRollNum);
        int num = m_iRollNum/100;
        
        if(num>4){
        
            lisenter->setEnabled(false);
            m_bIsRoll = true;
            m_pPaste->stopAllActions();
            m_pPaste->runAction(ScaleTo::create(0.2,1.0));
            
            AudioHelp::getInstance()->stopLoofEffect();
            
            m_pSpoon->runAction(Spawn::create(RotateTo::create(1.0,0),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),CallFunc::create([=](){
            
                
//                RewardLayer* layer = RewardLayer::create(RewardType::typeMoldHeart);
//                this->addChild(layer,100);
//                layer->btnOkClick=[=](){
//                    
//                    func_bind(nextStep);
//                    
//                };

                string gameState = UserDefault::getInstance()->getStringForKey(GameState);
                if (gameState == GameStateTwo) {
                    
                    RewardLayer* layer = RewardLayer::create(RewardType::typeMoldHeart);
                    this->addChild(layer,1000);
                    layer->btnOkClick=[=](){
                        
                        func_bind(nextStep);
                        
                        
                    };
                    
                }else{
                    
                    FlowerLayer::Loading(this,[=](){
                        
                        func_bind(nextStep);
                    });
                    
                    
                }
                
                
           }),nullptr));
            
        }else{
            m_pPaste->setTexture(__String::createWithFormat("cocostudio/MakeOther1/paste_%s%d.png",m_sFlovar.c_str(),num)->getCString());
        }
    }
}
void MakeOtherStep3::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    if(m_bIsSound){
        AudioHelp::getInstance()->stopLoofEffect();
    }
}
void MakeOtherStep3::touchEnd(ui::Widget* widget)
{
    
    
}