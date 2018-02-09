//
//  MixMoldScene.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/6.
//
//

#include "MixMoldScene.h"
#include "CookMakeScene.h"
#include "CircleBy.h"
#include "VictoryLayer.h"
#include "ChangeLoading.h"

int  mixPotZorder[5] = {10,20,30,40,50};
Vec2 firePos[5]={Vec2(189 + 20,300),Vec2(537,300),Vec2(374,246),Vec2(193,182),Vec2(555,191)};

Vec2 mixPotPos[5]={Vec2(191,307),Vec2(558,304),Vec2(373,248),Vec2(166,179),Vec2(584,180)};
Vec2 _mixPotPos[5]={Vec2(-100,940),Vec2(-100,940),Vec2(480,940),Vec2(1060,940),Vec2(1060,940)};

int timeUpdate[3][5]={{2,5,8,11,14},{17,20,23,26,29},{31,33,35,37,39}};

#define potLidUserData    1007
#define potMixTag         1008
#define potMixUserData1   1009
#define potMixUserData2   1010
#define potMixUserData3   1011

//锅的煮沸三个阶段
#define potStepOne    1012
#define potStepTwo    1013
#define potStepThree  1014

#define potPoint      1015
#define potShadow      1016
MixMoldScene::MixMoldScene()
{
    m_iStep = 0;
    m_bSwitchOn = true;
    m_pHand = nullptr;
    m_bStirIsShow = false;
    m_iSuccessPot = 0;
    m_iFailPot = 0;
    g_vPotNum.clear();
    m_bIsShowHand1 = true;
    m_bIsShowHand2 = false;
}
MixMoldScene::~MixMoldScene()
{
    
}
bool MixMoldScene::init()
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
void MixMoldScene::onEnter()
{
    GameBaseScene::onEnter();
//    showPotMix(0);
    
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent("Flavor Page");
    
    ActionUtils::moveByElasticOut(m_pStoven,Vec2(0,10),3.0,CallFunc::create([=](){
    
        m_pStoven->setVisible(true);
        
    }));
    
    for(auto pot:m_vPot){
        ActionUtils::moveByElasticOut(pot,Vec2(0,10),3.0,CallFunc::create([=](){
            
            
            
        }));
    }
    for(auto potCover:m_vPotCover){
        ActionUtils::moveByElasticOut(potCover,Vec2(0,10),3.0,CallFunc::create([=](){
            
            
            
        }));
    }
}
void MixMoldScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void MixMoldScene::onExit()
{
    GameBaseScene::onExit();
    
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent("Flavor Page");
}
void MixMoldScene::initUI()
{
    m_pBG = Sprite::create("images/cook_mold/make_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    m_pStoven = Sprite::create("images/cook_mix/stove.png");
    m_pStoven->setPosition(m_pBG->convertToWorldSpace(Vec2(576,294)));
    this->addChild(m_pStoven);
//    m_pStoven->setVisible(false);
    
//    _pStoven= Sprite::create("images/cook_mix/stove.png");
//    _pStoven->setPosition(m_pBG->convertToWorldSpace(Vec2(576,294)));
//    this->addChild(_pStoven);
    
    
    auto stoven = Sprite::create("images/cook_mold/stove_off.png");
    stoven->setPosition(Vec2(372,46));
    m_pStoven->addChild(stoven);
    
    m_pSwitch = MySpriteButton::create("images/cook_mold/stove_on.png", std::bind(&MixMoldScene::switchBtn, this,std::placeholders::_1));
    m_pSwitch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pSwitch->setPosition(Vec2(27,33));
    m_pSwitch->setEnable(false);
    stoven->addChild(m_pSwitch);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for(int i = 0 ;i<5; i++){
        //fruitPath
        auto pot = Sprite::create(__String::createWithFormat("images/cook_mix/pot_%s_down.png",fruitPath[i].c_str())->getCString());
        pot->setPosition(m_pStoven->convertToWorldSpace(mixPotPos[i]) + Vec2(0,50));
        this->addChild(pot,mixPotZorder[i]);
        pot->setTag(i);
        Rect rectPot = Rect(pot->getPosition().x-60,pot->getPosition().y + 44,119,32);
        
        m_vPot.pushBack(pot);
        
        auto potCover =MySpriteButton::create(__String::createWithFormat("images/cook_mix/pot_%s_up.png",fruitPath[i].c_str())->getCString(), std::bind(&MixMoldScene::potCoverClick, this,std::placeholders::_1));
        potCover->setPosition(m_pStoven->convertToWorldSpace(mixPotPos[i])+ Vec2(0,50));
        this->addChild(potCover,mixPotZorder[i]+5);
        potCover->setTag(i);
        potCover->setEnable(false);
        m_vPotCover.pushBack(potCover);

    }
    showPotFire(0);
//    showPotLid(0);
    showPotMix(0);
//    m_pStoven->setPositionX(m_pStoven->getPositionX()-1000);
    
}
void MixMoldScene::showHand()
{
    if(!m_pHand){

        m_pHand = Sprite::create("images/finger2.png");
        this->addChild(m_pHand,20);
        m_pHand->setAnchorPoint(Vec2(0.78,0.12));
        m_pHand->setPosition(m_pStoven->convertToWorldSpace(Vec2(372,46))+Vec2(80,-85));
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5,-10),RotateTo::create(0,0),RotateTo::create(0.5,-10),CallFunc::create([=](){
           
            
        }), NULL)));
    }
}
void MixMoldScene::showHandClickPot(int index)
{
    if(!m_pHand){
        
        m_pHand = Sprite::create("images/finger2.png");
        m_pHand->setAnchorPoint(Vec2(0.78,0.12));
        m_vPotCover.at(index)->addChild(m_pHand);
        m_pHand->setPosition(Vec2(196,55));
        m_pHand->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5,-10),RotateTo::create(0,0),RotateTo::create(0.5,-10), NULL)));
        
    }

}
void MixMoldScene::showFont(float dt)
{
    auto font = Sprite::create("images/cook_mix/2.png");
    kAdapterScreen->setExactPosition(font, 480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(font,200);
    
    
    ActionUtils::moveByElasticOut(font,Vec2(0,-500));

}
void MixMoldScene::showPotMix(float dt)
{
    for(auto pot : m_vPot){
        auto mix = Sprite::create(__String::createWithFormat("images/cook_mix/pot_%s0.png",fruitPath[pot->getTag()].c_str())->getCString());
        mix->setAnchorPoint(Vec2::ZERO);
        pot->addChild(mix,10);
        mix->setTag(potMixTag);
        mix->setUserData((void*)potMixUserData1);
//        mix->setOpacity(0);
        mix->runAction(Sequence::create(CallFunc::create([=](){
            
            this->scheduleOnce(schedule_selector(MixMoldScene::showPotLid), 2.0);
            
        }), NULL));
    }
   

}
void MixMoldScene::showPotLid(float dt)
{
    for (auto pot:m_vPotCover) {
        
        auto lid = MySpriteButton::create("images/cook_mix/pot_lid.png",std::bind(&MixMoldScene::potLidBtn, this,std::placeholders::_1));
        //        lid->setPosition(Vec2(89,149));
        lid->setPosition(Vec2(1000,1000));
        lid->setTag(pot->getTag());
        pot->addChild(lid);
        m_vPotLid.pushBack(lid);
        lid->setUserData((void*)potLidUserData);
        lid->setEnable(false);
        m_vPotLidNum.push_back(pot->getTag());
        
        MoveTo* move = MoveTo::create(0.5, Vec2(108,179 + 5));
        EaseElasticOut* eeo = EaseElasticOut::create(move, 1.0);
        lid->runAction(Sequence::create(DelayTime::create(0.1*pot->getTag()),CallFunc::create([=](){
        
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
        
        
        }),eeo,CallFunc::create([=](){
            
            showHand();
            m_pSwitch->setEnable(true);
            
            
        }), NULL));
    }
}
void MixMoldScene::showPotFire(float dt)
{
    for (int i = 0;i<5;i++) {
        
        auto fire = Sprite::create("images/cook_mix/fire0.png");
        fire->setPosition(mixPotPos[i]);
        fire->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_pStoven->addChild(fire);
        fire->setVisible(false);
        m_vFire.pushBack(fire);
        fire->setScale(0.80);
    }
}
void MixMoldScene::potCoverClick(Ref *sender)
{
//    if (m_bStirIsShow) {
//        return;
//    }
    
    if(m_pHand){
        
        m_pHand->removeFromParent();
        m_pHand= nullptr;
        
    }
    
    auto btn = dynamic_cast<MySpriteButton*>(sender);
    btn->stopAllActions();
    size_t data = (size_t)btn->getUserData();
    if (data == potStepOne) {
        
        //进入搅拌
        btn->setEnable(false);
        
        auto lidBtn = dynamic_cast<MySpriteButton*>(m_vPotLid.at(btn->getTag()));
        lidBtn->stopAllActions();
        
        AudioHelp::getInstance()->playEffect("open the lid.mp3");
        
        lidBtn->runAction(Sequence::create(Spawn::create(RotateTo::create(0.2,-90),MoveBy::create(0.2, Vec2(-lidBtn->getContentSize().width/2,20)),nullptr), CallFunc::create([=](){
            
            showPotStir(btn->getTag());
            
        }),NULL));

        
        
    }else if(data == potStepTwo){
    
        //进入搅拌
        btn->setEnable(false);
        auto lidBtn = dynamic_cast<MySpriteButton*>(m_vPotLid.at(btn->getTag()));
        lidBtn->stopAllActions();
        
        AudioHelp::getInstance()->playEffect("open the lid.mp3");
        
        lidBtn->runAction(Sequence::create(Spawn::create(RotateTo::create(0.2,-90),MoveBy::create(0.2, Vec2(-lidBtn->getContentSize().width/2,20)),nullptr), CallFunc::create([=](){
            
            showPotStir(btn->getTag());
            
        }),NULL));
    }

}
void MixMoldScene::potActionStepOne(MySpriteButton* btn)
{
    btn->runAction(Sequence::create(DelayTime::create(timeUpdate[0][btn->getTag()]),CallFunc::create([=](){
       //出现汽泡
       auto lid = m_vPotLid.at(btn->getTag());
        lid->runAction(Sequence::create(MoveBy::create(0.2,Vec2(0,20)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),nullptr));
        
       for(int i = 0;i<10;i++){

            auto point = Sprite::create(__String::createWithFormat("images/cook_mix/%s_bubble.png",fruitPath[btn->getTag()].c_str())->getCString());
            point->setPosition(Vec2(44+rand()%140,145));
            m_vPot.at(btn->getTag())->addChild(point);
            point->setTag(potPoint);
            
            point->runAction(RepeatForever::create(Sequence::create(DelayTime::create(rand()%10*0.1),MoveBy::create(0.5,Vec2(0,60)),MoveBy::create(0.2,Vec2(0,-60)),NULL)));
           
        }
        AudioHelp::getInstance()->playLoofEffect("bubble.mp3");
    }),DelayTime::create(1.0),CallFunc::create([=](){
        //出现烟雾
        
        if(m_bIsShowHand1){
        
            m_bIsShowHand1=false;
            showHandClickPot(btn->getTag());
        }
        
        
        auto lid = m_vPotLid.at(btn->getTag());
        lid->stopAllActions();
        lid->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.25,rand()%20),RotateTo::create(0.25,-rand()%20),RotateTo::create(0.25,0),nullptr)));
        
        auto shadow = Sprite::create("images/cook_mix/steam0.png");
        m_vPot.at(btn->getTag())->addChild(shadow);
        shadow->setPosition(Vec2(106,174 + 10));
        shadow->setTag(potShadow);
        
        auto pOvenAnimation = Animation::create();
        for (int i = 0;i<8;i++) {
            
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/cook_mix/steam%d.png",i)->getCString());
            
            
        }
        pOvenAnimation->setDelayPerUnit(0.2);
        pOvenAnimation->setLoops(-1);
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        shadow->runAction(pOvenAnimat);

        btn->setEnable(true);
        //设置为第一阶段
        btn->setUserData((void*)potStepOne);
    
    }),DelayTime::create(5.0),CallFunc::create([=](){
    
        AudioHelp::getInstance()->stopLoofEffect();
        //如果超时不点,就直接糊了
        auto pot = dynamic_cast<Sprite*>(m_vPot.at(btn->getTag()));
        Vector<Node*> _nodeV = pot->getChildren();
        
        for(auto node : _nodeV)
        {
            
            if (node->getTag() == potPoint ||node->getTag() == potShadow ) {
                node->removeFromParent();
            }
            
        }
        m_vFire.at(btn->getTag())->removeFromParent();
        btn->setEnable(false);
        
        AudioHelp::getInstance()->playEffect("fire-put down.mp3");
        
        ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
        
            AudioHelp::getInstance()->playEffect("failed.mp3");
        
        
        }));
        
        
        auto smoke = Sprite::create("images/cook_mix/smoke.png");
        smoke->setAnchorPoint(Vec2(0.47,0.05));
        btn->addChild(smoke);
        smoke->setScale(0);
        smoke->setPosition(Vec2(110,143));
        
        smoke->runAction(Sequence::create(ScaleTo::create(1.0,1.0),FadeOut::create(0.1),nullptr));
        
        auto potMix = dynamic_cast<Sprite*>(m_vPot.at(btn->getTag()));
        if(potMix->getChildByTag(potMixTag)){
        
//            auto pot = dynamic_cast<Sprite*>(m_vPot.at(index));
            auto mix = dynamic_cast<Sprite*>(potMix->getChildByTag(potMixTag));
            
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s_burnt.png",fruitPath[btn->getTag()].c_str())->getCString());
        }
        
        
        
        auto lid = m_vPotLid.at(btn->getTag());
        RotateBy* rota = RotateBy::create(1.0,360);
        MoveTo* move = MoveTo::create(1.0,Vec2(1000,1000));
        Spawn* sp = Spawn::create(rota,move, NULL);
        lid->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create([=](){
        
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
        }),sp,CallFunc::create([=](){
            
            m_iFailPot++;
            if (m_iFailPot == 5) {
                
                //重新来
                SceneChangeManager->enterCookMakeScene();
                
            }else if (m_iFailPot + m_iSuccessPot == 5) {
                
                log("----->over");
                m_bSwitchOn = false;
                m_pSwitch->setEnable(true);
                showHand();
            }
        }), NULL));
    }),NULL));
}
void MixMoldScene::potActionStepTwo(MySpriteButton* btn)
{
    btn->runAction(Sequence::create(DelayTime::create(timeUpdate[1][btn->getTag()]-timeUpdate[0][btn->getTag()] - 4),CallFunc::create([=](){
        //出现汽泡
        auto lid = m_vPotLid.at(btn->getTag());
        lid->runAction(Sequence::create(MoveBy::create(0.2,Vec2(0,20)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),nullptr));
        for(int i = 0;i<10;i++){
            
            auto point = Sprite::create(__String::createWithFormat("images/cook_mix/%s_bubble.png",fruitPath[btn->getTag()].c_str())->getCString());
            point->setPosition(Vec2(44+rand()%140,145));
            m_vPot.at(btn->getTag())->addChild(point);
            point->setTag(potPoint);
            
            point->runAction(RepeatForever::create(Sequence::create(DelayTime::create(rand()%10*0.1),MoveBy::create(0.25,Vec2(0,60)),MoveBy::create(0,Vec2(0,-60)),NULL)));
            
            
        }
        AudioHelp::getInstance()->playLoofEffect("bubble.mp3");
    }),DelayTime::create(1.0),CallFunc::create([=](){
        //出现烟雾
        
        if (m_bIsShowHand2) {
            m_bIsShowHand2 = false;
            showHandClickPot(btn->getTag());
        }
        
        
        auto lid = m_vPotLid.at(btn->getTag());
        lid->stopAllActions();
        lid->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.25,rand()%20),RotateTo::create(0.25,-rand()%20),RotateTo::create(0.25,0),nullptr)));
        
        auto shadow = Sprite::create("images/cook_mix/steam0.png");
        m_vPot.at(btn->getTag())->addChild(shadow);
        shadow->setPosition(Vec2(106,174 + 10));
        shadow->setTag(potShadow);
        
        auto pOvenAnimation = Animation::create();
        for (int i = 0;i<8;i++) {
            
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/cook_mix/steam%d.png",i)->getCString());
            
            
        }
        pOvenAnimation->setDelayPerUnit(0.2);
        pOvenAnimation->setLoops(-1);
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        shadow->runAction(pOvenAnimat);
        
        btn->setEnable(true);
        //设置为第一阶段
        btn->setUserData((void*)potStepOne);
        
    }),DelayTime::create(5.0),CallFunc::create([=](){
        
        //如果超时不点,就直接糊了
        //如果超时不点,就直接糊了
        auto pot = dynamic_cast<Sprite*>(m_vPot.at(btn->getTag()));
        Vector<Node*> _nodeV = pot->getChildren();
        
        for(auto node : _nodeV)
        {
            
            if (node->getTag() == potPoint ||node->getTag() == potShadow ) {
                node->removeFromParent();
            }
            
        }
        
        AudioHelp::getInstance()->playEffect("fire-put down.mp3");
        
        ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("failed.mp3");
            
            
        }));
        
        m_vFire.at(btn->getTag())->removeFromParent();
        btn->setEnable(false);
        
        auto smoke = Sprite::create("images/cook_mix/smoke.png");
        smoke->setAnchorPoint(Vec2(0.47,0.05));
        btn->addChild(smoke);
        smoke->setScale(0);
        smoke->setPosition(Vec2(110,143));
        
        smoke->runAction(Sequence::create(ScaleTo::create(1.0,1.0),FadeOut::create(0.1),nullptr));
        
        auto potMix = dynamic_cast<Sprite*>(m_vPot.at(btn->getTag()));
        if(potMix->getChildByTag(potMixTag)){
            
            //            auto pot = dynamic_cast<Sprite*>(m_vPot.at(index));
            auto mix = dynamic_cast<Sprite*>(potMix->getChildByTag(potMixTag));
            
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s_burnt.png",fruitPath[btn->getTag()].c_str())->getCString());
        }
        
        
        
        auto lid = m_vPotLid.at(btn->getTag());
        RotateBy* rota = RotateBy::create(1.0,360);
        MoveTo* move = MoveTo::create(1.0,Vec2(1000,1000));
        Spawn* sp = Spawn::create(rota,move, NULL);
        lid->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
        }),sp,CallFunc::create([=](){
            
           m_iFailPot++;
            
            if (m_iFailPot == 5) {
                
                //重新来
                
                SceneChangeManager->enterCookMakeScene();
            }else  if (m_iFailPot + m_iSuccessPot == 5) {
                
                log("----->over");
                m_bSwitchOn = false;
                m_pSwitch->setEnable(true);
                showHand();
                
            }
            
            
        }), NULL));

        
    }),NULL));
    
    
}
void MixMoldScene::potActionStepThree(MySpriteButton* btn)
{
    
    
    
}
void MixMoldScene::showPotStir(int index)
{
    AudioHelp::getInstance()->stopLoofEffect();
    m_bStirIsShow = true;
    
    auto pot = dynamic_cast<Sprite*>(m_vPot.at(index));
    auto mix = dynamic_cast<Sprite*>(pot->getChildByTag(potMixTag));
    auto stir = Sprite::create("images/cook_mix/stir_mix.png");
    stir->setPosition(Vec2(1000,1000));
    pot->addChild(stir,10);
    
    size_t mixData = (size_t)mix->getUserData();
    
    MoveTo* move = MoveTo::create(0.5, Vec2(107,173));
    RotateBy* rota = RotateBy::create(0.5,360);
    Spawn* sp = Spawn::create(move,rota, NULL);
    int detale = rand()%5;
    CircleBy* circle1 = CircleBy::create(2.0,Vec2(0,10), 160, 360*(4 + detale));
    CircleBy* circle2 = CircleBy::create(2.0,Vec2(0,10), 160, 360*(4 + detale));
    
    
    
    ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
        
    
    }));
    
    stir->runAction(Sequence::create(sp,DelayTime::create(0.5),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("dough_mix.mp3");
        
    
    }),circle1,CallFunc::create([=](){
        
        Vector<Node*> _nodeV = pot->getChildren();
        
        for(auto node : _nodeV)
        {
            
            if (node->getTag() == potPoint ||node->getTag() == potShadow ) {
                node->removeFromParent();
            }
        
        }
        if (mixData == potMixUserData1) {
            
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s0.png",fruitPath[pot->getTag()].c_str())->getCString());
            
        }else if(mixData == potMixUserData2){
            
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s2.png",fruitPath[pot->getTag()].c_str())->getCString());
            
        }
    }),circle2,CallFunc::create([=](){
        if (mixData == potMixUserData1) {
            
            mix->setUserData((void*)potMixUserData2);
            
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s1.png",fruitPath[pot->getTag()].c_str())->getCString());
            
            
            
        }else if(mixData == potMixUserData2){
            
            mix->setUserData((void*)potMixUserData3);
            mix->setTexture(__String::createWithFormat("images/cook_mix/pot_%s3.png",fruitPath[pot->getTag()].c_str())->getCString());
            
        }
        
        m_bStirIsShow = false;
        
    }),FadeOut::create(0.15),CallFunc::create([=](){
        
        if((size_t)mix->getUserData() == potMixUserData3){
            
            //检测是否结束
            //结束 关火 下一步
            
            m_iSuccessPot++;
            
            auto lid = m_vPotLid.at(index);
            RotateBy* rota = RotateBy::create(1.0,360);
            MoveTo* move = MoveTo::create(1.0,Vec2(1000,1000));
            Spawn* sp = Spawn::create(rota,move, NULL);
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
            
            AudioHelp::getInstance()->playEffect("particle success.mp3");
            
            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/characterSelection_selectGirl.plist");
            emitter1->setPosition(Vec2(103,175 - 80));
            emitter1->setAutoRemoveOnFinish(true);
            m_vPotCover.at(index)->addChild(emitter1, 30);
            
            lid->runAction(Sequence::create(sp,CallFunc::create([=](){
                
                
                
                
                
            }), NULL));
            
            m_vOverPotIndex.push_back(index);
            g_vPotNum.push_back(index);
            
            log("----%d %d",m_iFailPot,m_iSuccessPot);
            if (m_iFailPot + m_iSuccessPot == 5) {
                
                m_bSwitchOn = false;
                m_pSwitch->setEnable(true);
                showHand();
                
            }
            
            
        }else{
            
            auto lidBtn = dynamic_cast<MySpriteButton*>(m_vPotLid.at(index));
            lidBtn->setRotation(0);
            lidBtn->runAction(Sequence::create(Spawn::create(RotateTo::create(0.2,0),MoveTo::create(0.2,Vec2(108,179 + 5)),nullptr), CallFunc::create([=](){
                
                m_vPotCover.at(index)->setUserData((void*)potStepTwo);
                potActionStepTwo(m_vPotCover.at(index));
                
            }),NULL));
            
        }
    }), NULL));
}
void MixMoldScene::potLidAction(float dt)
{
//    int num = rand()%m_vPotLid.size();
//    int num = randVectorNum();
//    auto lidBtn = dynamic_cast<MySpriteButton*>(m_vPotLid.at(num));
//    if((size_t)lidBtn->getUserData() == potLidUserData){
//        lidBtn->setRotation(0);
//        int deltal = rand()%5 + 5;
//        lidBtn->setEnable(true);
//        
//
//        lidBtn->runAction(Sequence::create(MoveBy::create(0.2,Vec2(0,20)),CallFunc::create([=](){
//        
//            //帧动画
////            m_vPot
//            
//            
//
//            
//        
//        }),Repeat::create(Sequence::create(RotateTo::create(0.15 + num*0.1,-deltal),RotateTo::create(0.15 + num*0.1,deltal), NULL),6),DelayTime::create(0.5),CallFunc::create([=](){
//        
//            shadow->removeFromParent();
//        
//        }),MoveBy::create(0.2,Vec2(0,-20)),RotateTo::create(0.15,0),CallFunc::create([=](){
//            
//            
//            potLidAction(0);
//            
//        }),nullptr));
//    }
}
void MixMoldScene::switchBtn(Ref *sender)
{
    if(m_pHand){
    
        m_pHand->removeFromParent();
        m_pHand= nullptr;
    
    }
    if(m_bSwitchOn){
    
        showFont(0);
        
        //开关打开
        m_pSwitch->setEnable(false);
        m_pSwitch->runAction(Sequence::create(ScaleTo::create(0.15,0.95),ScaleTo::create(0.15,1.0),RotateBy::create(0.5, 90),CallFunc::create([=](){
            for (auto fire:m_vFire) {
                
                fire->setVisible(true);
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile("images/cook_mix/fire0.png");
                pOvenAnimation->addSpriteFrameWithFile("images/cook_mix/fire1.png");
                pOvenAnimation->setDelayPerUnit(0.4);
                pOvenAnimation->setLoops(-1);
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                fire->runAction(pOvenAnimat);
                
            }
//            this->scheduleOnce(schedule_selector(MixMoldScene::potLidAction),2.0);
            
            for (auto potCover:m_vPotCover) {
                
                potActionStepOne(potCover);
                
            }
            
            
        }),NULL));
    
    
    }else{
        //开关关闭
        m_pSwitch->setEnable(false);
        m_pSwitch->runAction(Sequence::create(ScaleTo::create(0.15,0.95),ScaleTo::create(0.15,1.0),RotateBy::create(0.5, -90),CallFunc::create([=](){
            for (auto fire:m_vFire) {
                
                fire->setVisible(false);
                
            }
//            VictoryLayer* vic = VictoryLayer::create(kVictoryPot);
//            this->addChild(vic,100);
//            vic->btnNext=[=](){
//                
//                SceneChangeManager->enterMakeMoldScene();
//            };
//            vic->btnAgain=[=](){
//                
//                SceneChangeManager->enterCookMakeScene();
//                
//            };
            showParticle(0);
            
        }),NULL));
    
    
    }
    
}
void MixMoldScene::showParticle(float dt)
{
    AudioHelp::getInstance()->playEffect("cheer_star.mp3");
    for(int i=0;i<9;i++){
        
        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([=](){
            
            auto* effect = ParticleSystemQuad::create("particle/powerStar.plist");
            
            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
            this->addChild(effect,300);
            
            if(i == 8){
                
                VictoryLayer* vic = VictoryLayer::create(kVictoryPot);
                this->addChild(vic,300);
                vic->btnNext=[=](){
                    
                    
                    ChangeLoading::loading([](){
                        
                        SceneChangeManager->enterMakeMoldScene();
                        
                    });
                };
                vic->btnAgain=[=](){
                    
                    SceneChangeManager->enterCookMakeScene();
                    
                };
                
            }
            
        }), nullptr));
    }
}
void MixMoldScene::potLidBtn(Ref *sender)
{
//    auto lidBtn = dynamic_cast<MySpriteButton*>(sender);
//    lidBtn->setEnable(false);
//    size_t data = (size_t)lidBtn->getUserData();
//    if (data == potLidUserData) {
//        
//        for(vector<int>::iterator it  = m_vPotLidNum.begin(); it != m_vPotLidNum.end(); )
//        {
//            if (lidBtn->getTag() == *it) {
//                it = m_vPotLidNum.erase(it);
//            }else{
//            
//                ++it;
//            }
//        }
//        
//        lidBtn->setRotation(0);
//        lidBtn->setUserData(nullptr);
//    }
}
int MixMoldScene::randVectorNum()
{
//    if(m_vPotLidNum.size() == 0){
//        return 0;
//    }
//    int num = m_vPotLidNum.at(rand()%m_vPotLidNum.size());
//    
//    return num;
}
void MixMoldScene::nextStep()
{
    log("next");
    hideNextButton();
    
}