//
//  MakeOtherStep4.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#include "MakeOtherStep4.h"

REGIST_SECENE(MakeOtherStep4)

#define DataRectFull 1000

MakeOtherStep4::MakeOtherStep4()
{
    m_pMold = nullptr;
    m_pHand = nullptr;
    m_pTitle = nullptr;
}
MakeOtherStep4::~MakeOtherStep4()
{
    
}
bool MakeOtherStep4::init()
{
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeOtherCsb/MakeOtherStep4.csb");
    addChild(_studioLayer);

    m_sFlavor = GameDataInstance->getFlavor();
    m_pFlavor = _studioLayer->getNodeByName<Sprite*>(GameDataInstance->getFlavor());
    m_pFlavor->setVisible(true);
    
//    auto btn = _studioLayer->getNodeByName<ui::Button*>("btn");
//    ActionHelper::showBackInOut(btn, 0.5, ActionHelper::ShowDirection::show_from_left);
 
    if (!m_pTitle) {

        m_pTitle = Sprite::create("cocostudio/UI/tips/cut.png");
        m_pTitle->setAnchorPoint(Vec2(0.5,1));
        this->addChild(m_pTitle,100);
//        m_pTitle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height - 100 + 500));
        kAdapterScreen->setExactPosition(m_pTitle,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
        
    }
    
    return true;
}
void MakeOtherStep4::onEnter()
{
    BaseLayer::onEnter();
    
    
    
    _moldChooseLayer = MoldChooseLayer::create();
    this->addChild(_moldChooseLayer,20);
    _moldChooseLayer->clickBtn=[=](){
    
        if (m_pTitle) {
            kAdapterScreen->setExactPosition(m_pTitle,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
        }
    
    };
    _moldChooseLayer->chooseMold=[=](){
    
        m_sMold = GameDataInstance->getMold();
        m_vRect.clear();
        for(auto rect : m_vRect){
            rect->setUserData(nullptr);
        }
        
        if (m_pTitle) {
            
            kAdapterScreen->setExactPosition(m_pTitle,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
            m_pTitle->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5, Vec2(0,-500)), 1.0), NULL));
             AudioHelp::getInstance()->playEffect("VoiceOver/drag the cutter to cut 4 cookies.mp3");
        }
        
        if(m_pMold){
        
            m_pMold->runAction(Sequence::create(CallFunc::create([=](){
            
                if (m_pHand) {
                    m_pHand->removeFromParent();
                    m_pHand = nullptr;
                }
            }),EaseElasticIn::create(MoveBy::create(0.2,Vec2(500,0)),1.0),RemoveSelf::create(),CallFunc::create([=](){
            
                m_pMold = nullptr;
                
                m_pMold = Sprite::create(__String::createWithFormat("cocostudio/MakeOther4/mold_%s.png",m_sMold.c_str())->getCString());
                kAdapterScreen->setExactPosition(m_pMold,m_pMold->getContentSize().width/2 + 10,320 + 200,Vec2(0.5,0.5),kBorderRight,kBorderNone);
                this->addChild(m_pMold);
                
                _moldPos = m_pMold->getPosition();
                
                auto lisenter = MoveLisenter::create();
                
                Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pMold);
                lisenter->setOrginOffset();
                lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep4::onTouchDown, this);
                lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep4::onTouchMoveInRect, this);
                lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep4::onTouchUp, this);
                
                for(int i = 0;i<4;i++){
                    
                    auto rect = _studioLayer->getNodeByName<Sprite*>(__String::createWithFormat("rect%d",i)->getCString());
                    m_vRect.pushBack(rect);
                    lisenter->addRectByNode(rect);
                    
                }
                
                auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
                chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                chooseParticle->setVisible(true);
                chooseParticle->setPosition(m_pMold->getContentSize()*.5);
                chooseParticle->setScale(1.0);
                m_pMold->addChild(chooseParticle,1);
                chooseParticle->setTag(101);
                
                ActionHelper::showBackInOut(m_pMold, 0.5, ActionHelper::ShowDirection::show_from_right,[=](){
                
                    showHand();
                    
                });
            
            }), NULL));
        
        
        }else{
        
            m_pMold = Sprite::create(__String::createWithFormat("cocostudio/MakeOther4/mold_%s.png",m_sMold.c_str())->getCString());
            kAdapterScreen->setExactPosition(m_pMold,m_pMold->getContentSize().width/2 + 10,320 + 200,Vec2(0.5,0.5),kBorderRight,kBorderNone);
            this->addChild(m_pMold);
            
            _moldPos = m_pMold->getPosition();
            
            auto lisenter = MoveLisenter::create();
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pMold);
            lisenter->setOrginOffset();
            lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep4::onTouchDown, this);
            lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep4::onTouchMoveInRect, this);
            lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep4::onTouchUp, this);
            
            for(int i = 0;i<4;i++){
                
                auto rect = _studioLayer->getNodeByName<Sprite*>(__String::createWithFormat("rect%d",i)->getCString());
                m_vRect.pushBack(rect);
                lisenter->addRectByNode(rect);
                
            }
            
            auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
            chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            chooseParticle->setVisible(true);
            chooseParticle->setPosition(m_pMold->getContentSize()*.5);
            chooseParticle->setScale(1.0);
            m_pMold->addChild(chooseParticle,1);
            chooseParticle->setTag(101);
            
            ActionHelper::showBackInOut(m_pMold, 1.0, ActionHelper::ShowDirection::show_from_right,[=](){
                
                showHand();
                
            });
        }
    };
}
void MakeOtherStep4::showHand()
{
    if(!m_pHand){

        m_pHand=Sprite::create("cocostudio/UI/tips/finger.png");
        m_pHand->setPosition(_moldPos);
        this->addChild(m_pHand);
    
        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        
            m_pHand->setVisible(true);
            
        }),MoveBy::create(2.0, Vec2(-500,-300)),CallFunc::create([=](){
        
            m_pHand->setVisible(false);
            m_pHand->setPosition(_moldPos);
        
        }),DelayTime::create(1.0), NULL)));
    }
}
void MakeOtherStep4::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherStep4::onExit()
{
    BaseLayer::onExit();
}
void MakeOtherStep4::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if (m_pMold->getChildByTag(101)) {
        
        m_pMold->getChildByTag(101)->removeFromParent();
        
    }
    
    if (m_pTitle) {
        
        m_pTitle->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5, Vec2(0,500)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
            
            m_pTitle = nullptr;
            
        }), NULL));
    }
}
void MakeOtherStep4::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    _moldChooseLayer->moveBtn();
    if(m_pHand){
    
        m_pHand->removeFromParent();
        m_pHand = nullptr;
    
    }
    
    auto rect = m_vRect.at(index);
    size_t data = (size_t)rect->getUserData();
    if (data == DataRectFull)
        return;
    
    lisenter->setEnabled(false);
    
    node->runAction(Sequence::create(MoveTo::create(0.5,rect->getParent()->convertToWorldSpace(rect->getPosition()+Vec2(rect->getContentSize().width/2,rect->getContentSize().height/2))),ScaleTo::create(0.25,1.2),ScaleTo::create(0.25,0.9),ScaleTo::create(0.15,1.0),DelayTime::create(1.0),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("cut.mp3");
        
        rect->setUserData((void*)DataRectFull);
        
        auto rectCover = Sprite::create(__String::createWithFormat("cocostudio/MakeOther4/%s_trace_%s.png",m_sMold.c_str(),m_sFlavor.c_str())->getCString());
        rectCover->setPosition(rect->getContentSize()/2);
        rect->addChild(rectCover);
        
        auto rectSp= Sprite::create(__String::createWithFormat("cocostudio/MakeOther4/%s_paste_%s.png",m_sMold.c_str(),m_sFlavor.c_str())->getCString());
        rectSp->setPosition(rect->convertToWorldSpace(rect->getContentSize()/2));
        this->addChild(rectSp);
        rectSp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_vSprite.pushBack(rectSp);
        
        auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/fallStarParticle.plist");
        chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(rectSp->getContentSize()*.5);
        chooseParticle->setScale(1.0);
        rectSp->addChild(chooseParticle,1);
        
        AudioHelp::getInstance()->playEffect("all star .mp3");
        
     }), EaseElasticOut::create(MoveTo::create(1.0,_moldPos),1.0),CallFunc::create([=](){
    
        lisenter->setEnabled(true);
    
     }),DelayTime::create(1.0),CallFunc::create([=](){
         if (isOver()){
//             RewardLayer* layer = RewardLayer::create(RewardType::typeMoldAll);
//             this->addChild(layer,100);
//             layer->btnOkClick=[=](){
//                 
//                 func_bind(nextStep);
//                 
//             };
             Size visible = Director::getInstance()->getVisibleSize();
             Vec2 pos = Vec2(visible.width/2,visible.height/2);
             int i = 0;
             float num[4]={-2*( 187 + 10),-(187 + 10),0,187 + 50};
             
             m_pFlavor->runAction(Sequence::create(FadeOut::create(1.0),CallFunc::create([=](){}), NULL));
             
             for(auto rect:m_vRect){
             
                 rect->runAction(Sequence::create(FadeOut::create(1.0),CallFunc::create([=](){}), NULL));
             
             }
             
             ActionUtils::moveByElasticIn(m_pMold,Vec2(1000,0),1.0,CallFunc::create([=](){
                 
                 
             }));
             
             for (auto sp : m_vSprite) {
                 
                 sp->runAction(Sequence::create(/*EaseElasticInOut::create(MoveTo::create(1.0,Vec2(pos.x + 50 + num[i],pos.y)),1.0),*/DelayTime::create(1.5 + i*0.2),ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1), CallFunc::create([=](){
                     
                     if (i == 3) {
                         
                         string gameState = UserDefault::getInstance()->getStringForKey(GameState);
                         if (gameState == GameStateTwo) {
                             
                             RewardLayer* layer = RewardLayer::create(RewardType::typeMoldAll);
                             this->addChild(layer,1000);
                             layer->btnOkClick=[=](){
                                 
                                 func_bind(nextStep);
                             };
                             
                         }else{
                             
                             FlowerLayer::Loading(this,[=](){
                                 
                                 func_bind(nextStep);
                             });
                         }
                     }
                 
                 }),NULL));
                 i++;
             }
         }
    }),NULL));
}
bool MakeOtherStep4::isOver()
{

    bool over = false;
    int  num = 0;
    for(auto rect : m_vRect){
    
        size_t data = (size_t)rect->getUserData();
        if (data == DataRectFull)
            num++;
    }
    if (num == 4)
        over = true;
    
    return over;

}
void MakeOtherStep4::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    
    
}
void MakeOtherStep4::touchEnd(ui::Widget* widget)
{
     
    
    
}