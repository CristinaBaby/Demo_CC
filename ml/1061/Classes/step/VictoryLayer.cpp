//
//  VictoryLayer.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/14.
//
//

#include "VictoryLayer.h"
#include "ActionUtils.h"
#include "Shake.h"
#include "AudioHelp.h"
#include "Global.h"
#include "ParabolaTo.h"

string content[3]={"tips-fruit.png","tips-gummy.png","tips-pot.png"};
Vec2  pos[12]={Vec2(-100,1000),Vec2(-100,700),Vec2(-100,400),Vec2(-100,-100),Vec2(500,-100),Vec2(1000,-100),Vec2(1500,500),Vec2(1500,1000),Vec2(500,1500),Vec2(0,1500),Vec2(300,1500),Vec2(800,1500)};

Vec2 indexPos[5][5]={{Vec2(218,135),Vec2(0,0),Vec2(0,0),Vec2(0,0),Vec2(0,0)},{Vec2(133,135),Vec2(322,135),Vec2(0,0),Vec2(0,0),Vec2(0,0)},{Vec2(222,154),Vec2(112,98),Vec2(343,98),Vec2(0,0),Vec2(0,0)},{Vec2(137,170),Vec2(318,170),Vec2(137,95),Vec2(318,95),Vec2(0,0)},{Vec2(110,179),Vec2(329,175),Vec2(224,135),Vec2(111,88),Vec2(329,88)}};

VictoryLayer::VictoryLayer()
{
    m_iType = 0;
    m_sMoldPath = "";
    
}
VictoryLayer::~VictoryLayer()
{

}

VictoryLayer* VictoryLayer::create(int type,string path)
{
    VictoryLayer* vic = new VictoryLayer();
    if (vic && vic->init(type,path)) {
        
        vic->autorelease();
        return vic;
        
    }
    CC_SAFE_DELETE(vic);
    return nullptr;
    
}

bool VictoryLayer::init(int type,string path)
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,180))) {
        return false;
    }
    
    m_iType = type;
    
    m_sMoldPath = path;
    
    m_vIndex = g_vPotNum;
    
    m_pTitleBg = Sprite::create("images/victorytips/tips-bg.png");
    kAdapterScreen->setExactPosition(m_pTitleBg, 480,320 + 1000);
    this->addChild(m_pTitleBg,10);
    
    m_pBtnNext = MySpriteButton::create("images/victorytips/next-btn.png", std::bind(&VictoryLayer::btnNextClick, this,std::placeholders::_1));
    m_pBtnNext->setPosition(Vec2(325 + 10,-1));
    m_pTitleBg->addChild(m_pBtnNext);
    m_pBtnNext->setOpacity(0);
    
    m_pBtnAgain = MySpriteButton::create("images/victorytips/again-btn.png", std::bind(&VictoryLayer::btnAgainClick, this,std::placeholders::_1));
    m_pBtnAgain->setPosition(Vec2(120 + 10,-1));
    m_pTitleBg->addChild(m_pBtnAgain);
    m_pBtnAgain->setOpacity(0);
    
    
    
    for (int i = 0;i<3;i++) {
        auto star = Sprite::create("images/victorytips/star-bg.png");
        star->setPosition(Vec2(100 + 120*i + 10,322));
        m_pTitleBg->addChild(star);
        
        
        auto starUp = Sprite::create("images/victorytips/star.png");
        starUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        starUp->setPosition(star->getContentSize()/2);
        starUp->setTag(i);
        starUp->setScale(0);
        star->addChild(starUp);
        m_vStar.pushBack(starUp);
//        starUp->setPosition(Vec2(50,50));
        
//        Size visSize = Director::getInstance()->getVisibleSize();
//        starUp->setPosition(Vec2(visSize.width*i*0.5,visSize.height+200));
    }
    
    m_pVictory = Sprite::create("images/victorytips/victory1.png");
    m_pVictory->setPosition(Vec2(229,362 + 35 + 1000));
    m_pTitleBg->addChild(m_pVictory);
    
    m_pContent = Sprite::create(__String::createWithFormat("images/victorytips/%s",content[m_iType].c_str())->getCString());
    m_pContent->setPosition(Vec2(222,143 + 20));
    m_pTitleBg->addChild(m_pContent);
//    m_pContent->setOpacity(0);
    m_pContent->setScale(0);
    
    m_pGummys  = Sprite::create("images/victorytips/tips-gummy.png");
    m_pGummys->setPosition(Vec2(229,376 - 200));
    m_pTitleBg->addChild(m_pGummys,-1);
    
    m_pLight  = Sprite::create("images/victorytips/light.png");
    m_pLight->setPosition(m_pTitleBg->getContentSize()/2);
    m_pTitleBg->addChild(m_pLight,-10);
    m_pLight->setOpacity(0);
    m_pLight->setScale(1.2);
    
    
    for (int i = 0;i<12;i++) {
        
        int index = i%5;
        auto star = Sprite::create(__String::createWithFormat("images/victorytips/fireworks%d.png",index + 1)->getCString());
        kAdapterScreen->setExactPosition(star,480,320);
        this->addChild(star);
        star->setTag(i);
        m_vStarFly.pushBack(star);
        star->setVisible(false);
        
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/startFly.plist");
        emitter1->setPosition(star->getContentSize()/2);
        emitter1->setAutoRemoveOnFinish(true);
        emitter1->setDuration(-1);
        star->addChild(emitter1, 30);
      
        
    }
    
    if (type == kVictoryGummy && m_sMoldPath!="") {
        
        auto mold = Sprite::create(FileUtils::getInstance()->getWritablePath()+m_sMoldPath.c_str());
        m_pContent->addChild(mold);
        mold->setPosition(m_pContent->getContentSize()/2);
        mold->setScale(0.5);
        
    }
    
    if (type == kVictoryPot && m_vIndex.size()>0) {
        
        m_pContent->setVisible(false);
        
//        m_pTitleBg
        int num = 0;
        for (auto index:m_vIndex) {
            
            auto pot = Sprite::create(__String::createWithFormat("images/cook_mix/pot_%s_down.png",fruitPath[index].c_str())->getCString());
            pot->setAnchorPoint(Vec2(0.48,0.18));
            pot->setPosition(indexPos[m_vIndex.size()-1][num]);
            m_pTitleBg->addChild(pot);
            
            auto potCover = Sprite::create(__String::createWithFormat("images/cook_mix/pot_%s_up.png",fruitPath[index].c_str())->getCString());
            potCover->setAnchorPoint(Vec2::ZERO);
            pot->addChild(potCover,10);
            
            auto mix = Sprite::create(__String::createWithFormat("images/cook_mix/pot_%s3.png",fruitPath[index].c_str())->getCString());
            mix->setAnchorPoint(Vec2::ZERO);
            pot->addChild(mix);
            
            pot->setScale(0.5);
            num++;
        }
        m_vStar.at(0)->setVisible(false);
        m_vStar.at(1)->setVisible(false);
        m_vStar.at(2)->setVisible(false);
        
        int size = (int)m_vIndex.size();
        if(size == 5){
        
            m_vStar.at(0)->setVisible(true);
            m_vStar.at(1)->setVisible(true);
            m_vStar.at(2)->setVisible(true);
        }else
        if(size/2 <= 1){
        
            m_vStar.at(0)->setVisible(true);
        
        }else if(size/2 == 2){
            m_vStar.at(0)->setVisible(true);
            m_vStar.at(1)->setVisible(true);
        }
        
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/knifeParticl.plist");
        emitter1->setPosition(Vec2(213,138));
        emitter1->setAutoRemoveOnFinish(true);
        emitter1->setDuration(-1);
        m_pTitleBg->addChild(emitter1, 30);
    }

    m_pBearSuccess = Sprite::create(__String::createWithFormat("images/loading/loading_%d.png",rand()%3)->getCString());
//    m_pBearSuccess->setPosition(Vec2());
    kAdapterScreen->setExactPosition(m_pBearSuccess,480,50);
    m_pBearSuccess->setAnchorPoint(Vec2(0.5,0));
    this->addChild(m_pBearSuccess);
    m_pBearSuccess->setScale(0);
    m_pBearSuccess->setPositionY(m_pBearSuccess->getPositionY()+400);
    m_pBearSuccess->setPositionX(m_pBearSuccess->getPositionX()-500);
    
   
    
    
    return true;
}
void VictoryLayer::showBear(float dt)
{
   
    
    m_pBearSuccess->runAction(Sequence::create(Spawn::create(ParabolaTo::create(1.5,m_pBearSuccess->getPosition(),kAdapterScreen->getExactPostion(Vec2(480,50))),ScaleTo::create(1.5,1),nullptr),CallFunc::create([=](){
    
        
        _showBear(0);
        
    
    }),nullptr));
  
    AudioHelp::getInstance()->playEffect("loading_1.mp3");
    
    
}
void VictoryLayer::_showBear(float dt)
{
    AudioHelp::getInstance()->playEffect("end_level_success.mp3");
    
    ScaleTo* s11 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s12 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s13 = ScaleTo::create(0.2f,1.0f,1.0f);
    RotateTo* rota1 = RotateTo::create(0.25,-15);
    Spawn* sp1 = Spawn::create(Sequence::create(s11,s12,s13, NULL),rota1, NULL);
    
    ScaleTo* s21 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s22 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s23 = ScaleTo::create(0.2f,1.0f,1.0f);
    RotateTo* rota2 = RotateTo::create(0.25,15);
    Spawn* sp2 = Spawn::create(Sequence::create(s21,s22,s23, NULL),rota2, NULL);
    
    
    m_pBearSuccess->runAction(RepeatForever::create(Sequence::create(sp1,sp2, NULL)));
    
    ActionUtils::delayTimeAction(this,3.0,CallFunc::create([=](){
    
        m_pBearSuccess->stopAllActions();
        m_pBearSuccess->runAction(Sequence::create(Spawn::create(RotateTo::create(0.2,0),ScaleTo::create(0.2f,1.0f,1.0f),NULL),FadeOut::create(0.5),CallFunc::create([=](){
        
            
            showTitleBg(0);
            
        
        }),nullptr));
    }));
    
}
void VictoryLayer::onEnter()
{
    LayerColor::onEnter();
//    showBear(0);
    showTitleBg(0);
}
void VictoryLayer::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
}
void VictoryLayer::onExit()
{
    LayerColor::onExit();
}
void VictoryLayer::showTitleBg(float dt)
{
    ActionUtils::moveByElasticOut(m_pTitleBg,Vec2(0,-1000),1.0,CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("game_victory.mp3");
        showGummy(0);
        showStarFly(0);
        showVictory(0);
    
    }));
//    showStarFly
    ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){

        showStarFly(0);
        
    }));
}

void VictoryLayer::showGummy(float dt)
{
    ScaleTo* s1 = ScaleTo::create(0.05f,1.08f,0.96f);
    ScaleTo* s2 = ScaleTo::create(0.05f,1.0f,1.0f);
    ScaleTo* s3 = ScaleTo::create(0.05f,0.980,1.06f);
    ScaleTo* s4 = ScaleTo::create(0.1f,1.0f,1.0f);
    MoveBy* move1 = MoveBy::create(0.5,Vec2(0,200 + 50));
    MoveBy* move2 = MoveBy::create(0.1,Vec2(0,-50));
    m_pGummys->runAction(Sequence::create(move1,move2,s1,s2,s3,s4,CallFunc::create([=](){
        
//        showVictory(0);
        showContent(0);
        
    }), NULL));
}
void VictoryLayer::showVictory(float dt)
{
    
    MoveBy* move = MoveBy::create(0.5,Vec2(0,-1000));
    EaseElasticOut* eeo = EaseElasticOut::create(move,1.0);
    m_pVictory->runAction(Sequence::create(eeo,CallFunc::create([=](){
        
        
        
        
    }), NULL));
    
    
}
void VictoryLayer::showStarFly(float dt)
{
    ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
    
    
         AudioHelp::getInstance()->playEffect("fireworks2.mp3");
    
    
    }));
    
    for (auto star :m_vStarFly) {
        star->setVisible(true);
        
        star->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(3.5+2,pos[star->getTag()])),CallFunc::create([=](){
        
//            showlight(0);
//            showBtn(0);
//            if(star->getTag() == 1){
//                showStarNum(0);
//            }
            
            
        }),RemoveSelf::create(), NULL));
        
    }
    
    this->scheduleOnce(schedule_selector(VictoryLayer::showStarNum), 3);
    
}
void VictoryLayer::showStarNum(float dt)
{
    for (auto star:m_vStar) {
        
        log("star move");
//        MoveTo* move = MoveTo::create(0.5,Vec2(1,0));
//        EaseSineIn* eso = EaseSineIn::create(move);
//        EaseElasticOut* eeo = EaseElasticOut::create(eso,1.0);
        ScaleTo* scale1 = ScaleTo::create(0,0.4);
  
        
        star->runAction(Sequence::create(DelayTime::create(0.5*star->getTag()),scale1,EaseBackOut::create(ScaleTo::create(0.2, 1)),CallFunc::create([=](){
            
            if(star->isVisible()){
                
                ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/fallStarParticle.plist");
                emitter1->setPosition(star->getContentSize()/2);
                emitter1->setAutoRemoveOnFinish(true);
                emitter1->setDuration(0.5);
                star->addChild(emitter1, -1);
                AudioHelp::getInstance()->playEffect(__String::createWithFormat("star_%d.mp3",star->getTag()+1)->getCString());
            
            }

            
            
        }), CallFunc::create([=](){
        
            if (star->getTag() == 2) {
                
                showBtn(0);
                showlight(0);
                
            }
        
        }),NULL));
        
    }
    
}
void VictoryLayer::showBtn(float dt)
{
    m_pBtnNext->runAction(FadeIn::create(0.5));
    m_pBtnAgain->runAction(FadeIn::create(0.5));
}
void VictoryLayer::showContent(float dt)
{
    
//    m_pContent->runAction(Sequence::create(Spawn::create(FadeIn::create(0.5),MoveBy::create(0.5,Vec2(0,-30)),nullptr),CallFunc::create([=](){
//        
//        
//        
//    }), NULL));
    m_pContent->runAction(Sequence::create(ScaleTo::create(0,0.4),ScaleTo::create(1.0,1.2),ScaleTo::create(0.1,1.0),CallFunc::create([=](){
    
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/knifeParticl.plist");
        emitter1->setPosition(Vec2(213,138));
        emitter1->setAutoRemoveOnFinish(true);
        emitter1->setDuration(-1);
        m_pTitleBg->addChild(emitter1, 30);
    
    
    }), NULL));
}
void VictoryLayer::showlight(float dt)
{
    m_pLight->runAction(FadeIn::create(0));
    m_pLight->runAction(RepeatForever::create(RotateBy::create(5.0,360)));

}
void VictoryLayer::btnNextClick(Ref* pSender)
{
    auto btn = dynamic_cast<MySpriteButton*>(pSender);
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
    btn->runAction(Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6,CallFunc::create([=](){
    
    
        if(btnNext){
            
            btnNext();
        }
    
    
    }),NULL));

//    
//    if(btnNext){
//    
//        btnNext();
//    }
}
void VictoryLayer::btnAgainClick(Ref* pSender)
{
    auto btn = dynamic_cast<MySpriteButton*>(pSender);
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    float timeScale = 1.2;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
    btn->runAction(Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6,CallFunc::create([=](){
        
        
        if(btnAgain){
            
            btnAgain();
        }
    }),NULL));
    
}




