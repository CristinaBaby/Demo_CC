//
//  CookMakeScene.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/22.
//
//

#include "CookMakeScene.h"
#include "ParabolaTo.h"
#include "CircleBy.h"
#include　"MixMoldScene.h"


Vec2 potPos[5]={Vec2(162,261),Vec2(477,261),Vec2(319,212),Vec2(142,154),Vec2(501,154)};
Vec2 _potPos[5]={Vec2(-100,940),Vec2(-100,940),Vec2(480,940),Vec2(1060,940),Vec2(1060,940)};


int potZorder[5] = {10,20,30,40,50};

#define fruitTag          1001
#define cupTag            1002
#define gelatinTag        1003
#define switchTag         1004
#define potStateWater     1005
#define potStateGelatin   1006



CookMakeScene::CookMakeScene()
{
    oldPos = Vec2::ZERO;
    m_iZorder = -1;
    m_pHand = nullptr;
}
CookMakeScene::~CookMakeScene()
{
    
}
#pragma mark 初始化
bool CookMakeScene::init()
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
void CookMakeScene::onEnter()
{
    GameBaseScene::onEnter();
    
    this->scheduleOnce(schedule_selector(CookMakeScene::showStoven), 1.0);
    
}
void CookMakeScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void CookMakeScene::onExit()
{
    GameBaseScene::onExit();
}
void CookMakeScene::initUI()
{
    m_pBG = Sprite::create("images/cook_mold/make_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);

    m_pStoven = Sprite::create("images/cook_mold/stove.png");
    m_pStoven->setPosition(m_pBG->convertToWorldSpace(Vec2(576,294)));
    this->addChild(m_pStoven);

    auto stoven = Sprite::create("images/cook_mold/stove_off.png");
    stoven->setPosition(Vec2(321,38));
    m_pStoven->addChild(stoven);

    m_pSwitch = MySpriteButton::create("images/cook_mold/stove_on.png", std::bind(&CookMakeScene::switchBtn, this,std::placeholders::_1));
    m_pSwitch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pSwitch->setPosition(Vec2(27,33));
    m_pSwitch->setEnable(false);
    stoven->addChild(m_pSwitch);
    
    //初始化水杯
    m_pCup = ToolSprite::create("images/cook_mold/cup.png");
    m_pCup->setPosition(m_pBG->convertToWorldSpace(Vec2(204 - 1000,380)));
    this->addChild(m_pCup,10);
    m_pCup->setDelegate(this);
    m_pCup->setTag(cupTag);
    
    m_pGelatin= ToolSprite::create("images/cook_mold/gelatin.png");
    m_pGelatin->setPosition(m_pBG->convertToWorldSpace(Vec2(983 + 1000,369)));
    this->addChild(m_pGelatin,10);
    m_pGelatin->setDelegate(this);
    m_pGelatin->setTag(gelatinTag);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for(int i = 0 ;i<5; i++){
        //fruitPath
        auto pot = Sprite::create(__String::createWithFormat("images/cook_mold/pot_%s_down.png",fruitPath[i].c_str())->getCString());
        pot->setPosition(m_pStoven->convertToWorldSpace(potPos[i])+Vec2(0,pot->getContentSize().height/2-30));
        this->addChild(pot,potZorder[i]);
        pot->setTag(i);
        Rect rectPot = Rect(pot->getPosition().x-60,pot->getPosition().y + 44,119,32);
        
        m_vPot.pushBack(pot);
        
        auto potCover = Sprite::create(__String::createWithFormat("images/cook_mold/pot_%s_up.png",fruitPath[i].c_str())->getCString());
        potCover->setPosition(m_pStoven->convertToWorldSpace(potPos[i])+Vec2(0,pot->getContentSize().height/2-30));
        this->addChild(potCover,potZorder[i]+5);
        potCover->setTag(i);
        m_vPotCover.pushBack(potCover);
        
        auto fruit = ToolSprite::create(__String::createWithFormat("images/cook_mold/plate_%s.png",fruitPath[i].c_str())->getCString());
        this->addChild(fruit,i);
        fruit->setTag(i);
        fruit->setDelegate(this);
        fruit->cancelEvent(true);
        fruit->setPosition(Vec2(visibleSize.width/2+(i-2)*(fruit->getContentSize().width + 100),m_pBG->convertToWorldSpace(Vec2(0,600)).y));
        
        //添加移动工具的响应区域
        fruit->addRect(rectPot);
        m_pCup->addRect(rectPot);
        m_pGelatin->addRect(rectPot);
        m_vTool.pushBack(fruit);
        
        if (i<2) {
            fruit->setPositionX(fruit->getPositionX()-3000);
        }else if(i == 2){
        
            fruit->setPosition(fruit->getPosition()+Vec2(0,500));
        
        }else if(i>2){
        
            fruit->setPositionX(fruit->getPositionX()+1500);
        }
        
        auto plate = Sprite::create("images/cook_mold/plate.png");
        fruit->addChild(plate,-1);
        plate->setAnchorPoint(Vec2::ZERO);
        plate->setPosition(Vec2(-5,-5));
        
        fruit->addRectByNode(pot);
        pot->setPosition(_potPos[i]);
        potCover->setPosition(_potPos[i]);
        
//        //火
    }
    m_pStoven->setPositionX(m_pStoven->getPositionX()-1000);
}
#pragma mark 显示材料
void CookMakeScene::showStoven(float dt)
{
    AudioHelp::getInstance()->playEffect("slide.mp3");
    ActionUtils::moveByElasticOut(m_pStoven, Vec2(1000,0),1.0,CallFunc::create([=](){
    
        showPot(0);
    
    }));
}
void CookMakeScene::showPot(float dt)
{
    for (auto pot:m_vPot) {
        int tag = pot->getTag();
        pot->runAction(Sequence::create(DelayTime::create(0.2*tag),ParabolaTo::create(1.0,pot->getPosition(),m_pStoven->convertToWorldSpace(potPos[tag])+Vec2(0,pot->getContentSize().height/2-30 - 26)),CallFunc::create([=](){
        
            if (tag == 4) {
//                showPlate(0);
                showWaterCup(0);
//                showPotLid(0);
            }
        }),NULL));
    }
    for (auto pot:m_vPotCover) {
        
        int tag = pot->getTag();
        pot->runAction(Sequence::create(DelayTime::create(0.2*tag),CallFunc::create([=](){
        
            
        
        }),ParabolaTo::create(1.0,pot->getPosition(),m_pStoven->convertToWorldSpace(potPos[tag])+Vec2(0,pot->getContentSize().height/2-30 - 26)),CallFunc::create([=](){
            
        }),NULL));
        
        ActionUtils::delayTimeAction(this,0.2*tag + 1.0,CallFunc::create([=](){
        
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
        
        }));
        
    }
}
void CookMakeScene::showPlate(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for (auto plate:m_vTool) {
        
        int tag = plate->getTag();
        MoveTo* move = MoveTo::create(1.0, Vec2(visibleSize.width/2+(tag-2)*(plate->getContentSize().width + 30),m_pBG->convertToWorldSpace(Vec2(0,600)).y + 20));
        EaseElasticOut* eeo = EaseElasticOut::create(move, 1.0);
        plate->runAction(Sequence::create(DelayTime::create(0.2*tag),eeo,CallFunc::create([=](){
        
            plate->cancelEvent(false);
        
        }),NULL));
        
        ActionUtils::delayTimeAction(this,0.2*tag + 0.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
        }));
        
        
    }
    
    this->scheduleOnce([=](float dt){
    
        showHandFruitToPot(0);
    
    }, 2.0,"1");
    
    
}
void CookMakeScene::showWaterCup(float dt)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    ActionUtils::moveByElasticOut(m_pCup, Vec2(1000,0),1.0,CallFunc::create([=](){

        showHandAddWater(0);
        
    }));
}
void CookMakeScene::showGelatin(float dt)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    ActionUtils::moveByElasticOut(m_pGelatin, Vec2(-1000,0),1.0,CallFunc::create([=](){
        
        //showHandAddWater(0);
        showHandAddGelatin(0);
    }));
}
void CookMakeScene::showHandAddWater(float dt)
{
    if(!m_pHand){

        m_pHand = Sprite::create("images/finger1.png");
        m_pHand->setPosition(m_pCup->getPosition());
        this->addChild(m_pHand,100);
    
        ActionHelper::runBezierMoveHideAction(m_pHand,m_pHand->getPosition(),m_vPot.at(rand()%(int)m_vPot.size())->getPosition());
    
    }
}
void CookMakeScene::showHandAddGelatin(float dt)
{
    if(!m_pHand){
        
        m_pHand = Sprite::create("images/finger1.png");
        m_pHand->setPosition(m_pGelatin->getPosition());
        this->addChild(m_pHand,100);
        
        ActionHelper::runBezierMoveHideAction(m_pHand,m_pHand->getPosition(),m_vPot.at(rand()%(int)m_vPot.size())->getPosition());
        
    }
}
void CookMakeScene::showPotLid(float dt)
{
//    for (auto pot:m_vPotCover) {
//       
//        auto lid = MySpriteButton::create("images/cook_mold/pot_lid.png",std::bind(&CookMakeScene::potLidBtn, this,std::placeholders::_1));
////        lid->setPosition(Vec2(89,149));
//        lid->setPosition(Vec2(1000,1000));
//        lid->setTag(pot->getTag());
//        pot->addChild(lid);
//        m_vPotLid.pushBack(lid);
//        lid->setUserData((void*)potLidUserData);
//        lid->setEnable(false);
//        
//        MoveTo* move = MoveTo::create(0.5, Vec2(89 + 5,149 + 5));
//        EaseElasticOut* eeo = EaseElasticOut::create(move, 1.0);
//        lid->runAction(Sequence::create(DelayTime::create(0.1*pot->getTag()),eeo,CallFunc::create([=](){
//        
//            m_pSwitch->setEnable(true);
//        
//        
//        }), NULL));
//    }
}
void CookMakeScene::showPotStir(int index)
{
}
void CookMakeScene::showPotMix(float dt)
{
//    for(auto pot : m_vPot){
//        auto mix = Sprite::create(__String::createWithFormat("images/cook_mold/pot_%s0.png",fruitPath[pot->getTag()].c_str())->getCString());
//        mix->setAnchorPoint(Vec2::ZERO);
//        pot->addChild(mix,10);
//        mix->setTag(potMixTag);
//        mix->setUserData((void*)potMixUserData1);
//        mix->setOpacity(0);
//        mix->runAction(Sequence::create(FadeIn::create(0.25),CallFunc::create([=](){
//        
//            this->scheduleOnce(schedule_selector(CookMakeScene::showPotLid), 2.0);
//            
//        }), NULL));
//    }
}
void CookMakeScene::showHandFruitToPot(float dt)
{
    if(!m_pHand){
        
        int num = rand()%m_vTool.size();
        m_pHand = Sprite::create("images/finger1.png");
        m_pHand->setPosition(m_vTool.at(num)->getPosition() + Vec2(0,-50));
        this->addChild(m_pHand,100);
        
        ActionHelper::runBezierMoveHideAction(m_pHand,m_pHand->getPosition(),m_vPot.at(num)->getPosition());
//        m_vPot
//        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
//        
//            m_pHand->setVisible(true);
//            
//            
//        
//        }),MoveTo::create(3.0,m_vPot.at(num)->getPosition()),CallFunc::create([=](){
//        
//            m_pHand->setVisible(false);
//            m_pHand->setPosition(m_vTool.at(num)->getPosition());
//        
//        }), NULL)));
        
    }
}
#pragma mark 响应事件
void CookMakeScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    
    if(m_pHand){
    
        m_pHand->removeFromParent();
        m_pHand = nullptr;
    
    }
    
    oldPos = toolSprite->getPosition();
    //
    
    toolSprite->setLocalZOrder(100 + toolSprite->getTag());
}
void CookMakeScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->cancelEvent(true);
    
    AudioHelp::getInstance()->playEffect("mini game_miss.mp3");
    
    MoveTo* move = MoveTo::create(0.5, oldPos);
    EaseElasticOut* eeo = EaseElasticOut::create(move, 1.0);
    toolSprite->runAction(Sequence::create(eeo,CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
        toolSprite->setLocalZOrder(toolSprite->getTag());
        
        
    }),NULL));
}
void CookMakeScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    if(m_pHand){
        
        m_pHand->removeFromParent();
        m_pHand = nullptr;
        
    }
    int _tag = toolSprite->getTag();
    if (_tag < 6){
        _tag = fruitTag;
    }
//        _tag = fruitTag;
    switch (_tag) {
        case fruitTag:
        {
            int tag = toolSprite->getTag();
            toolSprite->cancelEvent(true);
            
            Vec2 pos = m_vPot.at(tag)->convertToWorldSpace(Vec2(175,210 + 50));
            
            AudioHelp::getInstance()->playEffect("add_powder.mp3");
            
            toolSprite->runAction(Sequence::create(MoveTo::create(0.5, pos),CallFunc::create([=](){
            
                toolSprite->setVisible(false);

                auto plate = Sprite::create("images/cook_mold/plate.png");
                plate->setPosition(toolSprite->getPosition());
                this->addChild(plate,potZorder[tag]+1);
                
                auto fruit = Sprite::create(__String::createWithFormat("images/cook_mold/plate_%s.png",fruitPath[tag].c_str())->getCString());
                fruit->setAnchorPoint(Vec2::ZERO);
                plate->addChild(fruit);
                
                
                
                plate->runAction(Sequence::create(RotateBy::create(0.56,-30),CallFunc::create([=](){
                    
                    fruit->runAction(Sequence::create(ScaleTo::create(1.0, 0), NULL));
                    
                    auto particle = ParticleSystemQuad::create("particle/pourFlavor.plist");
                    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                    particle->setTexture(Director::getInstance()->getTextureCache()->addImage(__String::createWithFormat("images/cook_mold/%s_fall.png",fruitPath[tag].c_str())->getCString()));
                    particle->setVisible(true);
                    particle->setStartSize(particle->getStartSize()/2);
                    particle->setEndSize(particle->getEndSize()/2);
                    particle->setPosition(Vec2(125,210));
                    m_vPot.at(tag)->addChild(particle,5);
                    
                    auto fruit = Sprite::create(__String::createWithFormat("images/cook_mold/pot_%s.png",fruitPath[tag].c_str())->getCString());
                    fruit->setAnchorPoint(Vec2::ZERO);
                    m_vPot.at(tag)->addChild(fruit);
                    fruit->setOpacity(0);
                    
                    fruit->runAction(Sequence::create(FadeIn::create(0.5),DelayTime::create(1.5),MoveBy::create(2.0,Vec2(0,-20)),NULL));
                    
                }),DelayTime::create(1.5),CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("ingredients out.mp3");
                    
                    
                }),EaseElasticIn::create(MoveBy::create(0.5,Vec2(1000,0)),1.0),CallFunc::create([=](){
                    
                    for (auto tool:m_vTool) {
                        
                        if (tool->isVisible()) {
                            return;
                        }
                    }
                    
                    
                    ActionUtils::delayTimeAction(this,3.0,CallFunc::create([=](){
                        
                        Scene* pScene = MixMoldScene::createScene();
                        //                    Director::getInstance()->replaceScene(TransitionPageTurn::create(1.0, pScene,false));
                        Director::getInstance()->replaceScene(pScene);
                        
                    }));
                    
                    static bool showpot = true;
                    if (showpot) {
                        showpot = false;
                        //next
                        log("---->next");
                        log("showPotLid(0);");
                        
                        
                        
                        
                        this->scheduleOnce(schedule_selector(CookMakeScene::showPotMix), 2.0);
                        //
                        //                    showPotLid(0);
                    }
                    //              showPotLid(0);
                    
                }),NULL));
            
            
            }), NULL));
            
            
            

        }
             break;
        case cupTag:
        {
            m_pCup->cancelEvent(true);
            addWaterToPot(index);
        }
            break;
        case gelatinTag:
        {
            m_pGelatin->cancelEvent(true);
            addGelatinToPot(index);
        }
            break;
            
        default:
            break;
    }
}
void CookMakeScene::switchBtn(Ref *sender)
{
    m_pSwitch->setEnable(false);
    m_pSwitch->runAction(Sequence::create(ScaleTo::create(0.15,0.95),ScaleTo::create(0.15,1.0),RotateBy::create(0.5, 90),CallFunc::create([=](){

        for (auto fire:m_vFire) {
            
            fire->setVisible(true);
            auto pOvenAnimation = Animation::create();
            pOvenAnimation->addSpriteFrameWithFile("images/cook_mold/fire0.png");
            pOvenAnimation->addSpriteFrameWithFile("images/cook_mold/fire1.png");
            pOvenAnimation->setDelayPerUnit(0.4);
            pOvenAnimation->setLoops(-1);
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            fire->runAction(pOvenAnimat);
        }
        
        this->scheduleOnce(schedule_selector(CookMakeScene::potLidAction),2.0);
    
    }),NULL));
}
#pragma mark 处理事件
void CookMakeScene::addWaterToPot(int index)
{
    auto pot = dynamic_cast<Sprite*>(m_vPot.at(index));
    pot->setUserData((void*)potStateWater);
    
//    this->reorderChild(m_pCup, potZorder[index]+1);
    
    MoveTo* move = MoveTo::create(0.25, pot->convertToWorldSpace(Vec2(180,245)));
    RotateBy* rota = RotateBy::create(0.5, -90);
    RotateBy* rota1 = RotateBy::create(0.5, 90);
    m_pCup->runAction(Sequence::create(move,rota,CallFunc::create([=](){

        AudioHelp::getInstance()->playEffect("add water.mp3");
        
        m_pCup->setTexture("images/cook_mold/cup_fall.png");
        auto water = Sprite::create("images/cook_mold/water.png");
        water->setAnchorPoint(Vec2(1,1));
        water->setPosition(m_pCup->convertToWorldSpace(Vec2(0 + 5,128 - 10)));
//        water->setRotation(90);
        this->addChild(water,potZorder[index]+1);
    
        water->runAction(Sequence::create(ScaleTo::create(0.25, 1,1.5),ScaleTo::create(0.25, 1.0,1.0),FadeOut::create(0.5), NULL));

        auto potWater = Sprite::create("images/cook_mold/pot_water.png");
        potWater->setAnchorPoint(Vec2::ZERO);
        pot->addChild(potWater);
        potWater->setPositionY(potWater->getPositionY()-40);
//      potWater->setOpacity(0);
        potWater->runAction(MoveBy::create(1.25, Vec2(0,40)));
        
      }),DelayTime::create(1.0),rota1,CallFunc::create([=](){
    
        m_pCup->removeAllChildren();
        m_pCup->setTexture("images/cook_mold/cup.png");
    
        int num = -1;
        
        for (auto _pot: m_vPot) {
            size_t data = (size_t)_pot->getUserData();
            if (data != potStateWater) {
                
                num = _pot->getTag();
                addWaterToPot(num);
                return;
            }
        }
        
          ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
          
              AudioHelp::getInstance()->playEffect("ingredients out.mp3");
          
          }));
          
          ActionUtils::moveByElasticIn(m_pCup,Vec2(1000,0),1.5,CallFunc::create([=](){
        
                showGelatin(0);
        
        }));
    }),NULL));
}
void CookMakeScene::addGelatinToPot(int index)
{
    auto pot = dynamic_cast<Sprite*>(m_vPot.at(index));
    pot->setUserData((void*)potStateGelatin);
    MoveTo* move = MoveTo::create(0.25, pot->convertToWorldSpace(Vec2(180,245)));
    RotateBy* rota = RotateBy::create(0.5, -90);
    RotateBy* rota1 = RotateBy::create(0.5, 90);
    m_pGelatin->runAction(Sequence::create(move,rota,CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("pour powder.mp3");
        
        m_pGelatin->setTexture("images/cook_mold/gelatin_fall.png");
        auto chooseParticle = ParticleSystemQuad::create("particle/salt.plist");
        chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        chooseParticle->setVisible(true);
        chooseParticle->setDuration(1.0);
        chooseParticle->setPosition(m_pGelatin->convertToWorldSpace(Vec2(0 + 5,128 - 10)));
        chooseParticle->setScale(1.0);
        this->addChild(chooseParticle,potZorder[index]+1);
        
        auto salt = Sprite::create("images/cook_mold/pot_gelatin.png");
        salt->setAnchorPoint(Vec2(0.5,0));
        salt->setPosition(Vec2(92,121));
        pot->addChild(salt);
        salt->setScale(0);
        
        MoveBy* move = MoveBy::create(2.0,Vec2(0,-40));
        FadeOut* fade = FadeOut::create(2.0);
        Spawn* sp = Spawn::create(move,fade, NULL);
        
        salt->runAction(Sequence::create(ScaleTo::create(1.25,1.0),CallFunc::create([=](){
            
            auto gelatin = Sprite::create("images/cook_mold/pot_gelatin_.png");
            gelatin->setAnchorPoint(Vec2::ZERO);
            pot->addChild(gelatin);
            gelatin->setOpacity(0);
            gelatin->runAction(FadeIn::create(0.5));
        }),sp,nullptr));
        
    }),DelayTime::create(1.0),rota1,CallFunc::create([=](){
        
        m_pGelatin->removeAllChildren();
        m_pGelatin->setTexture("images/cook_mold/gelatin.png");
        
        int num = -1;
        for (auto _pot: m_vPot) {
            size_t data = (size_t)_pot->getUserData();
            if (data != potStateGelatin) {
                num = _pot->getTag();
                addGelatinToPot(num);
                return;
            }
        }
        ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients out.mp3");
            
        }));
//        AudioHelp::getInstance()->playEffect("ingredients out.mp3");
        ActionUtils::moveByElasticIn(m_pGelatin,Vec2(1000,0),1.5,CallFunc::create([=](){
            showPlate(0);
        }));
        
    }),NULL));
}
void CookMakeScene::potLidAction(float dt)
{
}
void CookMakeScene::potLidBtn(Ref *sender)
{
}
void CookMakeScene::nextStep()
{
    log("next");
    hideNextButton();
    
}