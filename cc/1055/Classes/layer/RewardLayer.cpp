//
//  RewardLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/15.
//
//

#include "RewardLayer.h"
#include "Shake.h"
//REGIST_SECENE(RewardLayer)

#define BtnOkTag  10
#define BtnNextTag  20
#define BtnEatTag  30

string rewardType[8]={"mold_all","dec","mold_heart","rolling","scoop","mix","newflavor","all"};

RewardLayer::RewardLayer()
{
    m_iTouchNum = 0;
    _pos = Vec2::ZERO;
    m_bIsFirst = false;
    m_bIsTouch = false;
}
RewardLayer::~RewardLayer()
{
    
}
void RewardLayer::showRewardLayer(RewardType type,std::function<void()> btnOk ,std::function<void()> btnNext,std::function<void()> btnEat )
{
    auto _scene = Scene::create();
    auto _layer = RewardLayer::create(type);
    _scene->addChild(_layer);
    
    _layer->btnOkClick = btnOk;
    _layer->btnNextClick = btnNext;
    _layer->btnEatClick = btnEat;
    
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(_scene);
}
RewardLayer* RewardLayer::create(RewardType type)
{
    RewardLayer* layer = new RewardLayer();
    if(layer && layer->init(type)){
    
        layer->autorelease();
        return layer;
    
    }
    CC_SAFE_DELETE(layer);
    return nullptr;

}
bool RewardLayer::init(RewardType type)
{
    if(!KSPopLayer::init(Color4B(0,0,0,0)))
        return false;
    
    _type = type;
    this->setAnchorPoint(Vec2::ZERO);
    
    
    _layerColor= LayerColor::create(Color4B(0,0,0,180));
    addChild(_layerColor);
    _layerColor->setVisible(false);
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("RewardScene.csb");
    addChild(_studioLayer);
    
    m_pBg = _studioLayer->getNodeByName<Sprite*>("bg");
    m_pBg->setOpacity(0);
    
    if(_studioLayer != nullptr && _studioLayer->_touchBtnCallback == nullptr)
        _studioLayer->_touchBtnCallback = CC_CALLBACK_1(RewardLayer::touchEnd, this);
    
    m_pGiftBox = _studioLayer->getNodeByName<Sprite*>("box");
    m_pRewards = _studioLayer->getNodeByName<Sprite*>("rewards");
    m_pLight1  = _studioLayer->getNodeByName<Sprite*>("light1");
    m_pLight2  = _studioLayer->getNodeByName<Sprite*>("light2");
    
    m_pGift = _studioLayer->getNodeByName<Sprite*>(rewardType[_type]);
    
    log("%s",rewardType[_type].c_str());
    
    m_pGift->setVisible(true);
    m_pGift->setScale(0);
    _pos = m_pGift->getPosition();
    
    auto lisenter = MoveLisenter::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pGiftBox);
    lisenter->setOrginOffset();
    
    lisenter->onTouchDown = CC_CALLBACK_2(RewardLayer::onTouchDown, this);
    lisenter->onTouchMoveInRect = CC_CALLBACK_3(RewardLayer::onTouchMoveInRect, this);
    lisenter->onTouchUp = CC_CALLBACK_2(RewardLayer::onTouchUp, this);
    
    m_pRewards->setScale(0);
    m_pGiftBox->setPosition(m_pGiftBox->getPosition()+Vec2(0,500));
    m_pGiftBox->setVisible(true);
    m_pLight1->setVisible(true);
    m_pLight1->setOpacity(0);
    m_pLight2->setVisible(true);
    m_pLight2->setOpacity(0);
    m_pLight2->setAnchorPoint(Vec2(0.50,0.51));
    _studioLayer->getNodeByName<Sprite*>("qiqiu")->setOpacity(0);
    
    m_pAnumation = Sprite::create("cocostudio/UI/rewards/animation/donhua_1.png");
    m_pAnumation->setPosition(this->getContentSize()/2);
    this->addChild(m_pAnumation);
    m_pAnumation->setPositionY(m_pAnumation->getPositionY()-1000);
    
    return true;
}
void RewardLayer::showAnumation(float dt)
{
    
    //根据不同的口味  选择不同的声音
    if(_type == typeMix || _type == typeDec || _type == typeRolling){
    
        ActionUtils::delayTimeAction(this,1.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("VoiceOver/nice,you've got a reward.mp3");
            
            
        }));
    
    }
    if(_type == typeScoop || _type == typeMoldAll || _type == typeNewFlavor){
        
        ActionUtils::delayTimeAction(this,1.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("VoiceOver/look,you've got something.mp3");
            
            
        }));
    }
    
    if(_type == typeMoldHeart){
        
        ActionUtils::delayTimeAction(this,1.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("VoiceOver/fantastic.mp3");
            
        }));
    }
    
    auto pOvenAnimation = Animation::create();
    for(int i = 1 ; i< 9 ; i++){
    
        pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("cocostudio/UI/rewards/animation/donhua_%d.png",i)->getCString());

    }
    pOvenAnimation->setDelayPerUnit(0.2);
    pOvenAnimation->setLoops(1);

    auto pOvenAnimat = Animate::create(pOvenAnimation);
    m_pAnumation->runAction(Sequence::create(MoveBy::create(0.5,Vec2(0,1000)),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("success.mp3");
    
    }),pOvenAnimat,FadeOut::create(0.25),CallFunc::create([=](){
        
        _layerColor->setVisible(true);
        m_pBg->runAction(Sequence::create(FadeIn::create(0.25),CallFunc::create([=](){
        
            m_pRewards->runAction(Sequence::create(ScaleTo::create(0.05,3.0),ScaleTo::create(0.5,1.0),ScaleTo::create(0.25,1.5),ScaleTo::create(0.25,1.0),DelayTime::create(0.225),CallFunc::create([=](){
                
                showGiftBox(0);
                
            }), NULL));
        
        }), NULL));
    }), NULL));
}
void RewardLayer::showGiftBox(float dt)
{
    m_pGiftBox1 = dynamic_cast<Sprite*>(m_pGiftBox->getChildByName("box1"));
    m_pGiftBox2 = dynamic_cast<Sprite*>(m_pGiftBox->getChildByName("box2"));

    AudioHelp::getInstance()->playEffect("present in.mp3");
    ActionUtils::moveByElasticOut(m_pGiftBox,Vec2(0,-500),1.0,CallFunc::create([=](){
        
        
        m_bIsTouch = true;
        
        _studioLayer->getNodeByName<Sprite*>("qiqiu")->runAction(FadeIn::create(0.2));
        m_pLight1->runAction(FadeIn::create(0.2));
        m_pGiftBox->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),RotateTo::create(0.15,-10),RotateTo::create(0.15,10),RotateTo::create(0.15,-20),RotateTo::create(0.15,20),RotateTo::create(0.25,0), DelayTime::create(0.5),NULL)));
        
        
    }));
}
void RewardLayer::showGift(float dt)
{
    
    AudioHelp::getInstance()->playEffect("present out.mp3");
    m_pGift->runAction(Sequence::create(Spawn::create(JumpBy::create(1.0,Vec2::ZERO,500,1),ScaleTo::create(1.0,1.0),RotateBy::create(1.0, 360), NULL),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("stamp.mp3");
        
        if(_type == typeRolling || _type == typeScoop){
            
            ActionUtils::delayTimeAction(this,0.1,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("VoiceOver/you've got new tools.mp3");
                
                
            }));
            
        }
        if(_type == typeNewFlavor){
            
            ActionUtils::delayTimeAction(this,0.1,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("VoiceOver/you've just unlock a new flavor as a reward.mp3");
                
            }));
        }
        if(_type == typeMoldAll){
            
            ActionUtils::delayTimeAction(this,0.1,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("VoiceOver/wow,you've got so many cutters.mp3");
                
                
            }));
            
        }
        if(_type == typeAllFlavor){
            
            ActionUtils::delayTimeAction(this,0.1,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("VoiceOver/it's so wonderful to unlock 2 flavors.mp3");
                
            }));
            
        }
        
        m_pLight2->runAction(Sequence::create(FadeIn::create(0.2),CallFunc::create([=](){
        
            showBtn(0);
        
        }),nullptr));
    
    }), NULL));

}
void RewardLayer::showBtn(float dt)
{
    m_pLight2->runAction(RepeatForever::create(RotateBy::create(0.5,30)));
    
    auto btnOk = _studioLayer->getNodeByName<Button*>("btn_ok");
    auto btnNext = _studioLayer->getNodeByName<Button*>("btn_next");
    auto btnEat = _studioLayer->getNodeByName<Button*>("btn_eat");
    btnOk->runAction(Sequence::create(DelayTime::create(1.0),EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0), NULL));
    btnNext->runAction(Sequence::create(DelayTime::create(1.0),EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0), NULL));
    btnEat->runAction(Sequence::create(DelayTime::create(1.0),EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0), NULL));
    
    if(_type == typeNewFlavor || _type == typeAllFlavor){
        btnNext->setVisible(true);
        btnEat->setVisible(true);
    }else{
        btnOk->setVisible(true);
    }
}
void RewardLayer::onEnter()
{
    KSPopLayer::onEnter();
    scheduleOnce(schedule_selector(RewardLayer::showAnumation), 0.05);
//    showAnumation(0);
//    showGiftBox(0);
}
void RewardLayer::onEnterTransitionDidFinish()
{
    KSPopLayer::onEnterTransitionDidFinish();
}
void RewardLayer::onExit()
{
    KSPopLayer::onExit();
}
void RewardLayer::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if(!m_bIsTouch)
        return;
    
    AudioHelp::getInstance()->playEffect("present tap.mp3");
    
    lisenter->setEnabled(false);
    
    if(!m_bIsFirst){
    
        m_bIsFirst = true;
        m_pGiftBox->stopAllActions();
        m_pGiftBox->setRotation(0);
        m_pGiftBox->setCascadeOpacityEnabled(false);
        m_pGiftBox->setOpacity(0);
        
    }
    
    
    
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    m_pGiftBox->runAction(Sequence::create(s1,s2,s3,CallFunc::create([=](){
    
        
    
    }),DelayTime::create(0.5),CallFunc::create([=](){
        
        
        m_pGiftBox2->runAction(MoveBy::create(0.25,Vec2(0,3)));
        //爆开
        m_pGiftBox2->runAction(Sequence::create(DelayTime::create(0.35),Spawn::create(MoveBy::create(1.0,Vec2(500,500)),RotateBy::create(1.0,360),NULL),CallFunc::create([=](){
            
        }), NULL));
        
        m_pGiftBox1->runAction(Sequence::create(ScaleTo::create(0.3,1.22,0.78),ScaleTo::create(0.2,1.12,1.13),CallFunc::create([=](){
            //            lisenter->setEnabled(true);
            showGift(0);
            
        }),FadeOut::create(0.2),NULL));
    
    }),nullptr));
}
void RewardLayer::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
}
void RewardLayer::onTouchUp(Node* node,MoveLisenter* lisenter)
{
}
void RewardLayer::touchEnd(ui::Widget* widget)
{
    Button* btn = (Button*)widget;
    int tag = btn->getTag();
    log("%d",tag);
    switch (tag) {
        case BtnOkTag:
        {
            func_bind(btnOkClick);
        }
            break;
        case BtnNextTag:
        {
            func_bind(btnNextClick);
        }
            break;
        case BtnEatTag:
        {
            func_bind(btnEatClick);
        }
            break;
        default:
            break;
    }
}