//
//  PopDialogLayer.cpp
//  COOK00321047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/11.
//
//

#include "PopDialogLayer.h"
#include "Global.h"
#include "SceneManager.h"
#include "AudioHelp.h"

PopDialogLayer::PopDialogLayer()
{
    
}
PopDialogLayer::~PopDialogLayer()
{
    
}
PopDialogLayer* PopDialogLayer::create(Color4B color)
{
    PopDialogLayer* pRet = new PopDialogLayer();
    if (pRet && pRet->init(color)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}
bool PopDialogLayer::init(Color4B color)
{
    if (!KSPopLayer::init(color)) {
        return false;
    }
    
    
    initUI();
    
    return true;
}


void PopDialogLayer::onEnter()
{
    KSPopLayer::onEnter();
    
    MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(DESIGN_WIDTH/2, DESIGN_HEIGHT/2)));
    EaseSineOut* esi = EaseSineOut::create(move);
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    Spawn* sp = Spawn::create(esi,scale, NULL);
    
    ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.2f,1.2f);
    
    AudioHelp::getInstance()->playEffect("pop_up_open.mp3");
    
    m_pDialog->runAction(Sequence::create(sp,s2,s1,s3,CallFunc::create([=](){
    
        
        showUI();
    
    }), NULL));
}
void PopDialogLayer::onExit()
{
    KSPopLayer::onExit();
}
void PopDialogLayer::onEnterTransitionDidFinish()
{
    KSPopLayer::onEnterTransitionDidFinish();
    
}
void PopDialogLayer::initUI()
{
    m_pDialog=Sprite::create("images/loading/doyou.png");
    kAdapterScreen->setExactPosition(m_pDialog, 0, 0);
    this->addChild(m_pDialog);
    m_pDialog->setScale(0);
    
    
    m_pOk = MySpriteButton::create("images/dialog/makemore-btn.png",std::bind(&PopDialogLayer::clickOk, this,std::placeholders::_1));
    m_pNo = MySpriteButton::create("images/dialog/eatit-btn.png",std::bind(&PopDialogLayer::clickNo, this,std::placeholders::_1));
    
    m_pOk->setPosition(Vec2(370+10,60));
    m_pNo->setPosition(Vec2(194-10,60));
    m_pOk->setOpacity(0);
    m_pNo->setOpacity(0);
    m_pDialog->addChild(m_pOk);
    m_pDialog->addChild(m_pNo);

    m_pMold = Sprite::create(__String::createWithFormat("images/loading/loading_%d.png",randNum())->getCString());
    m_pMold->setPosition(Vec2(196 + 40,171));
    m_pDialog->addChild(m_pMold);
    m_pMold->setScale(0);
    
    m_pQues = Sprite::create("images/loading/mark.png");
    m_pQues->setPosition(Vec2(383 - 40,169));
    m_pDialog->addChild(m_pQues);
    m_pQues->setScale(0);
}
void PopDialogLayer::showUI()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    m_pMold->runAction(Sequence::create(ScaleTo::create(0.5,1.2),EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0),nullptr));
    m_pQues->runAction(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.5,1.2),EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0),CallFunc::create([=](){
    
        m_pNo->runAction(FadeIn::create(0.25));
        m_pOk->runAction(FadeIn::create(0.25));
        
        m_pQues->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,10),RotateTo::create(0.25,-10),RotateTo::create(0.25,0),DelayTime::create(2.0),NULL)));
        
        
    }),nullptr));
}
void PopDialogLayer::clickOk(Ref* pSdener)
{
     AudioHelp::getInstance()->playEffect("button_general.mp3");
    if (btnOk) {
        btnOk();
    }
}
void PopDialogLayer::clickNo(Ref* pSdener)
{
     AudioHelp::getInstance()->playEffect("button_general.mp3");
    if (btnNo) {
        btnNo();
    }
}
int PopDialogLayer::randNum()
{
    int num = rand()%6;
    if (g_Mold_Sprite) {
        if (num == g_Mold_Sprite->getTag()) {
            
            return randNum();
        }
    }
    
    return num;
}
