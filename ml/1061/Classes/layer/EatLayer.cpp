//
//  EatLayer.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/8.
//
//

#include "EatLayer.h"
#include "GummyConfig.h"
#include "ParabolaTo.h"
#include "MySpriteButton.h"
#include "AdapterScreen.h"
#include "AudioHelp.h"
#include "CircleBy.h"
#include "ActionUtils.h"
#include "ChangeLoading.h"
#include "IAPManager.h"
#include "AdsLoadingLayer.h"
#include "AdLoadingLayerBase.h"


Color4F colorMold[5]={Color4F(),Color4F(),Color4F(),Color4F(),Color4F()};
Scene* _scene;
EatLayer::EatLayer()
{
    m_iMoldInit = 0;
    m_bIsFirst = true;
    m_iEatNum = 0;
    m_bTapEatHide = true;
}
EatLayer::~EatLayer()
{

}
Scene* EatLayer::createScene()
{
    _scene = Scene::createWithPhysics();
    //    _scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    //    _scene->getPhysicsWorld()->setGravity(Vec2(0,-100));
    //    _scene->getPhysicsWorld()->setAutoStep(false);
    EatLayer* layer = EatLayer::create();
    _scene->addChild(layer);
    return _scene;
    
}
EatLayer* EatLayer::create()
{
    EatLayer* dec = new EatLayer();
    
    if(dec&&dec->init())
    {
        dec->autorelease();
        return dec;
    }
    CC_SAFE_DELETE(dec);
    return nullptr;
}
bool EatLayer::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!Layer::init());
        this->initData();
        this->initView();
//        this->initMold();
        
        isInit=true;
    }while(0);
    
    return isInit;
}
#pragma mark 初始化
void EatLayer::initData()
{
    //从本地文件夹遍历
    FilePathUtil::getFilesPathByDir(GummyCaheDir, m_vMoldPath);
    //若小于20个，再由系统随机生产
    if(m_vMoldPath.size() < 20){
    
        int num = (int)m_vMoldPath.size();
        for(int i = 0;i<20 - num;i++){
        
            int fruitNum = rand()%5;
            int moldNum  = rand()%6;
            __String* path = __String::createWithFormat("images/make_mold/mold/pourmold_%s_%s.png",moldPath[moldNum].c_str(),fruitPath[fruitNum].c_str());
            m_vMoldPath.push_back(path->getCString());
            m_vMoldIndex.push_back(moldNum);
            
        }
        
        for (int i = 0;i<m_vMoldPath.size(); i++) {
            
            log("%s",m_vMoldPath.at(i).c_str());
            
        }
    }
}
void EatLayer::initView()
{
    m_pBg = Sprite::create("images/cook_mold/make_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg,480,320);
    this->addChild(m_pBg);

    auto border = PhysicsBody::create();
    border->addShape(PhysicsShapeEdgeChain::create(eatBgVerts1, 7,PHYSICSBODY_MATERIAL_DEFAULT,3));
    border->addShape(PhysicsShapeEdgeChain::create(eatBgVerts2, 4,PHYSICSBODY_MATERIAL_DEFAULT,3));
    m_pBg->setPhysicsBody(border);
    border->setDynamic(false);
    
    m_pGlass = Sprite::create("images/eat/jar_down.png");
    m_pGlass->setAnchorPoint(Vec2(0.47,0.13));
    m_pGlass->setPosition(m_pBg->convertToWorldSpace(Vec2(565,161)));
    this->addChild(m_pGlass,100);
    
    m_pGlassCover = Sprite::create("images/eat/jar_up.png");
    m_pGlassCover->setAnchorPoint(Vec2(0.47,0.13));
    m_pGlassCover->setPosition(m_pGlass->getPosition());
    this->addChild(m_pGlassCover,120);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //定义世界的边界
//    auto body = PhysicsBody::createEdgeBox(visibleSize,
//                                           PHYSICSBODY_MATERIAL_DEFAULT,5.0f);
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
//    edgeNode->setPhysicsBody(body);
//    this->addChild(edgeNode);

    auto borderUpper = PhysicsBody::createEdgeChain(glassVerts,36,PHYSICSBODY_MATERIAL_DEFAULT,3);
    m_pGlass->setPhysicsBody(borderUpper);
    
    this->schedule(schedule_selector(EatLayer::initMold), 0.1);
//    scheduleOnce(schedule_selector(EatLayer::updateStart), 0.1);
    scheduleUpdate();

    m_pNextGame = MySpriteButton::create("images/button/public_btn_next.png",std::bind(&EatLayer::nextGame, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(m_pNextGame,10, 10,Vec2(1,1),kBorderRight,kBorderTop);
    this->addChild(m_pNextGame);

    
    m_pReturnHome = MySpriteButton::create("images/button/public_btn_home.png",std::bind(&EatLayer::returnHome, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(m_pReturnHome,10, 10,Vec2(0,1),kBorderLeft,kBorderTop);
    this->addChild(m_pReturnHome);
    
    m_pEat = Sprite::create("images/eat/blow1.png");
    m_pEat->setPosition(m_pBg->convertToWorldSpace(Vec2(201,249 - 50)));
    this->addChild(m_pEat);
    m_pEat->setOpacity(0);
    
    
}
void EatLayer::nextGame(Ref* sender)
{
    ChangeLoading::loading([](){
        
//        SceneChangeManager->enterHomeScene();
        SceneChangeManager->enterPopFruitScene();
    });

}
void EatLayer::returnHome(Ref* sender)
{
    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setContentText("Are you sure you want to start over?");
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addChild(dialog, 9999);
}
void EatLayer::onNegativeClick(void* type)
{
    
}
void EatLayer::onPositiveClick(void* type)
{
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        //加广告
//        Director::getInstance()->popToRootScene();
        
//        ChangeLoading::loading([](){
//            
//            SceneChangeManager->enterHomeScene();
//        });
        
        
        if(kIAPManager->isShowAds())
        {
            AdsLoadingLayer::m_bIsFrom = false;
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            AdLoadingLayerBase::loadingDoneCallback = []()
            {
                Director::getInstance()->popToRootScene();
                SceneChangeManager->enterHomeScene();
            };
        }
        else
        {
            ChangeLoading::loading([](){
                
                Director::getInstance()->popToRootScene();
                SceneChangeManager->enterHomeScene();
                
            });
        }
        
        
        
//        if(kIAPManager->isShowAds())
//        {
//            
//            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
//            AdLoadingLayerBase::loadingDoneCallback = []()
//            {
//                Director::getInstance()->popToRootScene();
//                SceneChangeManager->enterHomeScene();
//            };
//        }
//        else
//        {
//            Director::getInstance()->popToRootScene();
//            SceneChangeManager->enterHomeScene();
//        }
    }
}
#pragma mark 初始化杯子里面的gummy
void EatLayer::initMold(float dt)
{
    if(m_iMoldInit == 20){
        this->unschedule(schedule_selector(EatLayer::initMold));
        
        AudioHelp::getInstance()->playEffect("particle success.mp3");
        
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/characterSelection_selectGirl.plist");
        emitter1->setPosition(m_pGlass->getContentSize()/2);
        emitter1->setAutoRemoveOnFinish(true);
        m_pGlass->addChild(emitter1, 30);
        
//        moldMove();
        if(m_bIsFirst){
            m_bIsFirst = false;
            this->scheduleOnce(schedule_selector(EatLayer::glassAction),2.0);
        
        }else{
            m_vMoldTouch.clear();
            readAction(0);
            this->unschedule(schedule_selector(EatLayer::initMold));
//            this->schedule(schedule_selector(EatLayer::moldAction), 2.0);
        }
        
        return;
    }

    string path = m_vMoldPath.at(m_iMoldInit);
        
    auto mold = Sprite::create(path.c_str());
    mold->setScale(0.25);
//    mold->setPosition(m_pGlass->convertToWorldSpace(Vec2(129 + rand()%100 - 50 ,411)));
    mold->setPosition(Vec2(129 + rand()%100 - 50 ,411 - 200));
    m_pGlass->addChild(mold,5);
    mold->setTag(m_iMoldInit);
    m_vMoldGlass.pushBack(mold);

    auto moldBorderUpper = PhysicsBody::createCircle(30);

    mold->setPhysicsBody(moldBorderUpper);
    moldBorderUpper->setDynamic(true);
    m_iMoldInit++;
    
}
void EatLayer::showTapToEat(float dt)
{
//    auto tap = Sprite::create("images/eat/tap-eat.png");
//    kAdapterScreen->setExactPosition(tap,480,420);
//    this->addChild(tap);
//    tap->setPositionX(tap->getPositionX()+1000);
//    
//    tap->runAction(Sequence::create(MoveBy::create(1.0,Vec2(-1000,0)),DelayTime::create(2.0),MoveBy::create(0.25, Vec2(-1000,0)),CallFunc::create([=](){
//    
//        readAction(0);
//    
//    }), NULL));
    
    auto font = Sprite::create("images/eat/tap-eat.png");
    kAdapterScreen->setExactPosition(font,480,50 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(font,20);
    font->setTag(1001);
    
    
    float detal = font->getPositionY() - Director::getInstance()->getVisibleSize().height/2;
    
    
    AudioHelp::getInstance()->playEffect("ingredients out.mp3");
    
    font->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5,Vec2(0,-detal)),1.0),DelayTime::create(1.0),CallFunc::create([=](){
        
        ActionUtils::delayTimeAction(this,1.0,CallFunc::create([=](){
            
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
        }));
        
    }),EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,detal-500)),1.0),DelayTime::create(1.5),CallFunc::create([=](){
    
        readAction(0);
    
    
    }), NULL));
    
    
    
}
void EatLayer::onEnter()
{
    Layer::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent("Eating stage");
}
void EatLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void EatLayer::onExit()
{
    Layer::onExit();
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent("Eating stage");
}
#pragma mark 设置物理引擎不同步
void EatLayer::updateStart(float dt)
{
    _scene->getPhysicsWorld()->setAutoStep(false);
//    initMold(0);
    
}
void EatLayer::update(float dt)
{
    _scene->getPhysicsWorld()->step(0.05f);

}
void EatLayer::readAction(float dt)
{
    auto ready = Sprite::create("images/eat/ready.png");
    kAdapterScreen->setExactPosition(ready, 480,320 + 1000);
    this->addChild(ready,20);
    
    AudioHelp::getInstance()->playEffect("ready.mp3");
    
    ready->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0, Vec2(0,-1000)), 1.0),RemoveSelf::create(), CallFunc::create([=](){
    
        goAction(0);
    
    }),NULL));
}
void EatLayer::goAction(float dt)
{
    auto go = Sprite::create("images/eat/go.png");
    kAdapterScreen->setExactPosition(go, 480,320);
    this->addChild(go,20);
    go->setOpacity(0);
    
    AudioHelp::getInstance()->playEffect("go.mp3");
    go->runAction(Sequence::create(FadeIn::create(0.2),RemoveSelf::create(), CallFunc::create([=](){
        
//        m_pEat->runAction(FadeIn::create(0.5));
//        m_pEat->setVisible(false);
//        this->schedule(schedule_selector(EatLayer::moldAction), 1.0);
//        this->scheduleOnce(schedule_selector(EatLayer::moldAction), rand()%10 * 0.1 + );
        moldAction(0);
    }), NULL));
}
void EatLayer::glassAction(float dt)
{
    MoveTo* move = MoveTo::create(1.0,m_pBg->convertToWorldSpace(Vec2(812,354 - 50)));
    ScaleTo* scale = ScaleTo::create(1.0,0.5);
    Spawn* sp = Spawn::create(move,scale, NULL);
    m_pGlass->runAction(Sequence::create(sp, CallFunc::create([=](){
    
    
//         readAction(0);
        //显示文字
        showTapToEat(0);
    
    }),NULL));
    MoveTo* move1 = MoveTo::create(1.0,m_pBg->convertToWorldSpace(Vec2(812,354 - 50)));
    ScaleTo* scale1 = ScaleTo::create(1.0,0.5);
    Spawn* sp1 = Spawn::create(move1,scale1, NULL);
    m_pGlassCover->runAction(Sequence::create(sp1, CallFunc::create([=](){
        
        
        
    }),NULL));
    
    
    
    
    m_vMoldTouch.clear();
    
    
    
   
    
}
void EatLayer::moldAction(float dt)
{
    if (m_iMoldInit == 0) {
//        this->unschedule(schedule_selector(EatLayer::moldAction));
//        showEatAgain(0);
       
        if(m_iEatNum > 17){
            m_iEatNum = 0;
//            showParticle(0);
            this->scheduleOnce(schedule_selector(EatLayer::showParticle), 2.0);
        
        }else{
            this->scheduleOnce(schedule_selector(EatLayer::showEatAgain), 2.0);
//            showEatAgain(0);
        
        }
        
        
//        showParticle(0);
        
        return;
    }
    int index = rand()%m_vMoldGlass.size();
    auto _mold = m_vMoldGlass.at(index);
    
    auto mold = MySpriteButton::createTexture(_mold->getTexture(),nullptr);
    mold->setPosition(Vec2(m_pGlass->convertToWorldSpace(Vec2(129,411))));
    this->addChild(mold,105);
    mold->setSelectorTouchBegan(std::bind(&EatLayer::clickMold, this,std::placeholders::_1));
    mold->setScale(0.25);
    m_vMoldTouch.pushBack(mold);
    
    _mold->removeFromParent();
    m_vMoldGlass.eraseObject(_mold);
    
    m_iMoldInit--;
    
    auto moldBorderUpper = PhysicsBody::createCircle(30);
    mold->setPhysicsBody(moldBorderUpper);
    moldBorderUpper->setDynamic(true);

    float posx =cocos2d::random(100, 700);
    float posy =cocos2d::random(500, 600);
    
    
    m_pGlass->runAction(Sequence::create(ScaleTo::create(0.5,0.55,0.45),EaseElasticOut::create(ScaleTo::create(0.2,0.5),1.0), NULL));
    m_pGlassCover->runAction(Sequence::create(ScaleTo::create(0.5,0.55,0.45),EaseElasticOut::create(ScaleTo::create(0.2,0.5),1.0), NULL));

    
    
    BezierTo* parabola = ParabolaTo::create(1.0 + (rand()%10 + 1) * 0.1,mold->getPosition(), Vec2(posx,posy));
    
//    this->scheduleOnce(schedule_selector(EatLayer::moldAction), rand()%10*1.0);
    
    ActionUtils::delayTimeAction(this,rand()%20*0.1,CallFunc::create([=](){
    
    
        moldAction(0);
    
    }));
    
    mold->runAction(Sequence::create(DelayTime::create(0.5),parabola, CallFunc::create([=](){
    
        mold->getPhysicsBody()->setVelocity(Vec2(-30,-50));
        
        
        
//        this->scheduleOnce([=](float dt){
//            
//            
//            
//            
//        }, (rand()%10 + 1) * 0.1,"once");
        
    }),DelayTime::create(5.0),CallFunc::create([=](){
    
        mold->runAction(Sequence::create(FadeOut::create(0.5),CallFunc::create([=](){
        
            mold->removeFromParent();
            m_vMoldTouch.eraseObject(mold);
            
            if (m_vMoldTouch.size() == 0) {
//                this->unschedule(schedule_selector(EatLayer::moldAction));
//                showEatAgain(0);
              
            }
        }), NULL));
    }),NULL));
}
void EatLayer::showParticle(float dt)
{
//    SoundPlayer::getInstance()->playEffect("sound/Sound/cheer_star.mp3");
    AudioHelp::getInstance()->playEffect("cheer_star.mp3");
    for(int i=0;i<9;i++){
       
        this->runAction(Sequence::create(DelayTime::create(0.4*i),CallFunc::create([=](){
            
            auto* effect = ParticleSystemQuad::create("particle/powerStar.plist");
            
            effect->setPosition(Vec2(60+rand()%820,200+rand()%485));
            this->addChild(effect);
            
            if(i == 8){
            
                showEatAgain(0);
            
            }
            
        }), nullptr));
    }
}
void EatLayer::showEatAgain(float dt)
{

    auto eatAgain = MySpriteButton::create("images/eat/eat_again.png",std::bind(&EatLayer::clickEatAgain, this,std::placeholders::_1));
    kAdapterScreen->setExactPosition(eatAgain,480,320);
    this->addChild(eatAgain,20);
    eatAgain->setPositionY(eatAgain->getPositionY()+1000);
    
    MoveBy* move = MoveBy::create(1.0, Vec2(0,-1000));
    EaseElasticOut* eeo = EaseElasticOut::create(move,1.0);
    eatAgain->runAction(Sequence::create(eeo,NULL));
     m_pEat->runAction(FadeOut::create(0.5));
}
void EatLayer::clickEatAgain(Ref* sender)
{
   
    auto btn = dynamic_cast<MySpriteButton*>(sender);
    btn->setEnable(false);
    
    btn->runAction(Sequence::create(FadeOut::create(0.5),CallFunc::create([=](){
    
        m_iMoldInit = 0;
        m_vMoldGlass.clear();
        
        this->schedule(schedule_selector(EatLayer::initMold), 0.1);
        btn->removeFromParent();
        
    
    }), NULL));
}
void EatLayer::clickMold(Ref* sender)
{
    if(m_bTapEatHide){
    
        m_bTapEatHide = false;
        if (this->getChildByTag(1001)) {
            
            ActionUtils::moveByElasticIn(this->getChildByTag(1001), Vec2(0,500));
            
        }
        
    
    }
    
    
    
    m_pEat->runAction(FadeIn::create(0.1));
    m_iEatNum++;
    auto moldBtn = dynamic_cast<MySpriteButton*>(sender);
    moldBtn->setEnable(false);
    moldBtn->getPhysicsBody()->setDynamic(true);
//    AudioHelp::getInstance()->playEffect("all star .mp3");
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    
    CircleBy* circle = CircleBy::create(0.5, Vec2(0,40), 360, 360*1.0);
    RotateBy* rota = RotateBy::create(0.5,360*1.5);
    Spawn* sp1 = Spawn::create(circle,rota, NULL);
    EaseSineIn* esi =EaseSineIn::create(sp1);
    moldBtn->runAction(Sequence::create(esi,CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("eat_gummy out.mp3");
        
    
    }),EaseSineIn::create(MoveTo::create(0.5,m_pEat->convertToWorldSpace(Vec2(173,165)))),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("eating.mp3");
        auto pOvenAnimation = Animation::create();
        pOvenAnimation->addSpriteFrameWithFile("images/eat/blow1.png");
        pOvenAnimation->addSpriteFrameWithFile("images/eat/blow2.png");
        pOvenAnimation->addSpriteFrameWithFile("images/eat/blow3.png");
        pOvenAnimation->addSpriteFrameWithFile("images/eat/blow4.png");
        pOvenAnimation->setDelayPerUnit(0.15);
        pOvenAnimation->setLoops(1);
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        m_pEat->runAction(pOvenAnimat);
        
        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/eatBits.plist");
        emitter1->setPosition(Vec2(121,124));
        emitter1->setAutoRemoveOnFinish(true);
//        emitter1->setScale(0.5);
        m_pEat->addChild(emitter1, 30);
    
    }),Spawn::create(MoveTo::create(0.2,m_pEat->convertToWorldSpace(Vec2(122,123))),ScaleTo::create(0.2, 0) ,NULL),CallFunc::create([=](){
    
        moldBtn->removeFromParent();
        
        m_vMoldTouch.eraseObject(moldBtn);
        
    
    }),NULL));
    
    
   
    
    
    
}