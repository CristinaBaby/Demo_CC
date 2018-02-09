//
//  MoldChooseLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/20.
//
//

#include "MoldChooseLayer.h"

string moldPath[6]={"round","heart","cat","pe","star","tree"};
string moldChoosePath[6]={"round","heart","cat","ginger","star","tree"};

MoldChooseLayer::MoldChooseLayer()
{
    m_bIsShow = true;
    m_bIsFirst = false;
}
MoldChooseLayer::~MoldChooseLayer()
{
}
bool MoldChooseLayer::init()
{

    if(!Layer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeOtherCsb/MoldChoose.csb");
    addChild(_studioLayer);
    
    //设置接收事件
    if(_studioLayer != nullptr && _studioLayer->_touchBtnCallback == nullptr)
        _studioLayer->_touchBtnCallback = CC_CALLBACK_1(MoldChooseLayer::touchEnd, this);
    
    
    //加载碗，盘子一类的工具
    m_pBg = _studioLayer->getNodeByName<Sprite*>("bg");
    btnChoose = _studioLayer->getNodeByName<Button*>("btnchoose");
    
    for(int i =0;i<6;i++){
    
        auto mold = _studioLayer->getNodeByName<Sprite*>(moldPath[i]);
        mold->setTag(i);
        auto lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, mold);
        lisenter->setOrginOffset();
        lisenter->m_bIsMove = false;
        
        lisenter->onTouchDown = CC_CALLBACK_2(MoldChooseLayer::onTouchDown, this);
        m_vMoveLisenter.pushBack(lisenter);
        
        //第二种游戏状态，只显示两个模具
        
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        
        if(gameState == GameStateTwo){
        
            if (i>1)
                mold->setVisible(false);
            
            if (i == 1) {

                auto chooseParticle = ParticleSystemQuad::create("cocostudio/particle/knifeParticl.plist");
                chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                chooseParticle->setVisible(true);
                chooseParticle->setPosition(mold->getContentSize()*.5);
                chooseParticle->setScale(1.0);
                mold->addChild(chooseParticle,1);
                
            }
            
        }
        //第三种游戏状态，显示所有模具
        else if(gameState == GameStateThree){
        
            
            
            
        
        }
        
        
        
        
        
        
    }
    
    m_pBg->setPosition(btnChoose->getPosition());
    m_pBg->setScale(0);
    
    btnChoose->setPositionX(btnChoose->getPositionX()-500);
    btnChoose->setEnabled(false);
    return true;
}
void MoldChooseLayer::moveBtn()
{
    if(btnChoose){
    
        btnChoose->setEnabled(false);
        btnChoose->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(-500,0)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
            
            btnChoose = nullptr;
            
        }), NULL));
    
    }
}
void MoldChooseLayer::onEnter()
{
    Layer::onEnter();
    
    btnChoose->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(500,0)),1.0), CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("VoiceOver/choose your favorite shapes.mp3");
        showBg();
    
    }),NULL));
}
void MoldChooseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void MoldChooseLayer::onExit()
{
    Layer::onExit();
    
}
void MoldChooseLayer::showBg()
{
    if(m_pBg->getScale()<0.5){
    
        
       
        
        
        
        
        MoveTo* move = MoveTo::create(0.5,Director::getInstance()->getVisibleSize()/2);
        EaseSineOut* esi = EaseSineOut::create(move);
        ScaleTo* scale = ScaleTo::create(0.5, 1.0);
        Spawn* sp = Spawn::create(esi,scale, NULL);
        
        ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
        ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
        ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
        
       
        
        m_pBg->runAction(Sequence::create(sp,s2,s1,s3,CallFunc::create([=](){
        
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
            btnChoose->setEnabled(true);
            m_bIsShow = true;
        }), NULL));
        
    }
}
void MoldChooseLayer::hideBg()
{
    if (m_pBg->getScale() > 0.9) {
        MoveTo* move = MoveTo::create(0.5,btnChoose->getPosition());
        EaseSineOut* esi = EaseSineOut::create(move);
        ScaleTo* scale = ScaleTo::create(0.5, 0.0);
        Spawn* sp = Spawn::create(esi,scale, NULL);
        
        
        m_pBg->runAction(Sequence::create(sp,CallFunc::create([=](){
        
            m_bIsShow = false;
            btnChoose->setEnabled(true);
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
        }),NULL));
        
    }

}
void MoldChooseLayer::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    auto mold = dynamic_cast<Sprite*>(node);
    int tag = mold->getTag();

    mold->runAction(Sequence::create(JumpBy::create(1.0,Vec2::ZERO, 50,2),CallFunc::create([=](){
    
        hideBg();
        
//        if(GameDataInstance->getMold() != moldChoosePath[tag]){
//            
//            GameDataInstance->setMold(moldChoosePath[tag]);
//            
//            if(chooseMold){
//                
//                chooseMold();
//            }
//        }
        GameDataInstance->setMold(moldChoosePath[tag]);
        
        if(chooseMold){
            
            chooseMold();
        }
    
    }),DelayTime::create(1.0),CallFunc::create([=](){
    
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
    
    }), NULL));
}
void MoldChooseLayer::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
}
void MoldChooseLayer::onTouchUp(Node* node,MoveLisenter* lisenter)
{
}
void MoldChooseLayer::touchEnd(ui::Widget* widget)
{
    
    int tag = widget->getTag();
    if(tag == 100){
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        btnChoose->setEnabled(false);
        if (m_bIsShow) {
            hideBg();
        }else{
        
            showBg();
            func_bind(clickBtn);
        }
        
    }else if(tag == 200){
    
         Director::getInstance()->getEventDispatcher()->setEnabled(false);
        btnChoose->setEnabled(false);
        hideBg();
    
    }
    
}