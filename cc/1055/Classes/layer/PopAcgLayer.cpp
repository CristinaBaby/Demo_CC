//
//  PopAcgLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/25.
//
//

#include "PopAcgLayer.h"
#include "Global.h"

#include "AudioHelp.h"


PopAcgLayer::PopAcgLayer()
{
    _endfunc = nullptr;
}
PopAcgLayer::~PopAcgLayer()
{
    
}
void PopAcgLayer::loading(Node* node,std::function<void()> endFunc)
{
    PopAcgLayer* layer = PopAcgLayer::create();
    layer->setAnchorPoint(Vec2::ZERO);
    layer->_endfunc = endFunc;
    node->addChild(layer,100);
    layer->setTag(1001);
    
}
PopAcgLayer* PopAcgLayer::create()
{
    PopAcgLayer* pRet = new PopAcgLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}
bool PopAcgLayer::init()
{
    if (!KSPopLayer::init(Color4B(0,0,0,180))) {
        return false;
    }

    m_pDialog=Sprite::create("cocostudio/UI/story/story-bg.png");
    kAdapterScreen->setExactPosition(m_pDialog, 0, 0);
    this->addChild(m_pDialog);
    m_pDialog->setScale(0);
    
    content = Sprite::create("cocostudio/UI/story/acg0.png");
    content->setAnchorPoint(Vec2::ZERO);
    m_pDialog->addChild(content);
    
    m_pOk = MySpriteButton::create("cocostudio/UI/public/close-btn.png",std::bind(&PopAcgLayer::clickOk, this,std::placeholders::_1));
    m_pOk->setPosition(Vec2(575,464));
    m_pOk->setOpacity(0);
    m_pDialog->addChild(m_pOk);
    
    return true;
}
void PopAcgLayer::onEnter()
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
    
    m_pDialog->runAction(Sequence::create(DelayTime::create(0.5),sp,s2,s1,s3,CallFunc::create([=](){
        
        showUI();
        
    }), NULL));
}
void PopAcgLayer::onExit()
{
    KSPopLayer::onExit();
}
void PopAcgLayer::onEnterTransitionDidFinish()
{
    KSPopLayer::onEnterTransitionDidFinish();
    
}
void PopAcgLayer::initUI()
{
    
    
}
void PopAcgLayer::showUI()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    
//    AudioHelp::getInstance()->playEffect("VoiceOver/so many yummy cookies.mp3");
    this->runAction(Sequence::create(DelayTime::create(2.0),CallFunc::create([=](){

        content->setTexture("cocostudio/UI/story/acg1.png");
    
        AudioHelp::getInstance()->playEffect("VoiceOver/Let's make more cookies.mp3");
    }),DelayTime::create(2.0),CallFunc::create([=](){
    
    
    }),DelayTime::create(2.0),CallFunc::create([=](){
    
        
        if(_endfunc)
            _endfunc();
        
    
    }), NULL));
}
void PopAcgLayer::clickOk(Ref* pSdener)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    if (btnClose) {
        btnClose();
    }
}
void PopAcgLayer::clickNo(Ref* pSdener)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    //    if (btnNo) {
    //        btnNo();
    //    }
}
int PopAcgLayer::randNum()
{
    int num = rand()%6;
    //    if (g_Mold_Sprite) {
    //        if (num == g_Mold_Sprite->getTag()) {
    //
    //            return randNum();
    //        }
    //    }
    
    return num;
}
