//
//  PopDialogLayer.cpp
//  COOK00321047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/11.
//
//

#include "PopDialogLayer.h"
#include "Global.h"

#include "AudioHelp.h"

string pathContent[2] = {"newFlavor.png","unlockAll.png"};

PopDialogLayer::PopDialogLayer()
{
    _type = newFlavor;
}
PopDialogLayer::~PopDialogLayer()
{

}
PopDialogLayer* PopDialogLayer::create(PopDialogType type)
{
    PopDialogLayer* pRet = new PopDialogLayer();
    if (pRet && pRet->init(type)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}
bool PopDialogLayer::init(PopDialogType type)
{
    if (!KSPopLayer::init(Color4B(0,0,0,180))) {
        return false;
    }
    
    
    _type = type;
    
    m_pDialog=Sprite::create("cocostudio/UI/popup/unlock-tips-bg.png");
    kAdapterScreen->setExactPosition(m_pDialog, 0, 0);
    this->addChild(m_pDialog);
    m_pDialog->setPosition(kAdapterScreen->getExactPostion(Vec2(DESIGN_WIDTH/2, DESIGN_HEIGHT/2)));
    m_pDialog->setPositionY(m_pDialog->getPositionY()+1000);
                           
    __String* path = __String::createWithFormat("cocostudio/UI/popup/%s",pathContent[_type].c_str());
    
    content = Sprite::create(path->getCString());
    content->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    content->setPosition(m_pDialog->getContentSize()/2);
    content->setScale(0);
    m_pDialog->addChild(content);
    
    m_pOk = MySpriteButton::create("cocostudio/UI/public/close-btn.png",std::bind(&PopDialogLayer::clickOk, this,std::placeholders::_1));
    m_pOk->setPosition(Vec2(369,436));
    m_pOk->setOpacity(0);
    m_pDialog->addChild(m_pOk);
   
    
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
    
    AudioHelp::getInstance()->playEffect("award popup.mp3");
    
    m_pDialog->runAction(Sequence::create(DelayTime::create(0.5),sp,s2,s1,s3,CallFunc::create([=](){
    
        
        showUI();
        content->runAction(Sequence::create(ScaleTo::create(0.25,1.3),ScaleTo::create(0.15,1.0),CallFunc::create([=](){
        
            
            
            
            if(_type == allFlavor){
            
                AudioHelp::getInstance()->playEffect("VoiceOver/amazing,we can cook what we like.mp3");
            
            }else{
            
                AudioHelp::getInstance()->playEffect("VoiceOver/cool,let's make it.mp3");
            
            }
            
            auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/decorateParticle.plist");
            chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            chooseParticle->setVisible(true);
            chooseParticle->setPosition(content->getContentSize()*.5);
            chooseParticle->setScale(1.0);
            content->addChild(chooseParticle,1);

            
            AudioHelp::getInstance()->playEffect("all star .mp3");
            
        }), NULL));
    
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
    

}
void PopDialogLayer::showUI()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
   
    m_pOk->runAction(FadeIn::create(0.25));
}
void PopDialogLayer::clickOk(Ref* pSdener)
{
     AudioHelp::getInstance()->playEffect("button_general.mp3");
    if (btnClose) {
        btnClose();
    }
}
void PopDialogLayer::clickNo(Ref* pSdener)
{
     AudioHelp::getInstance()->playEffect("button_general.mp3");
//    if (btnNo) {
//        btnNo();
//    }
}
int PopDialogLayer::randNum()
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
