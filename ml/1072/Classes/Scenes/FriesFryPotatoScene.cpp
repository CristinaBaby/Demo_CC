
#include "FriesFryPotatoScene.h"
#include "PolygonHelp.h"
#include "SceneManager.h"

FriesFryPotatoScene::FriesFryPotatoScene()
{
}

FriesFryPotatoScene::~FriesFryPotatoScene()
{
    
}
bool FriesFryPotatoScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localRootPath("bg/bg_make1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320,500);
    this->addChildToBGLayer(pTable);
    
    m_pPot = Sprite::create(localPath("oil.png"));
    this->addChildToContentLayer(m_pPot);
    CMVisibleRect::setPositionAdapted(m_pPot, 320, 285+50);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fry_oil.plist");
    pParticle->setPosVar(Vec2(160, 70));
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(m_pPot->getPosition());
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_fry_fries0.mp3");
        _showThermomter();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_fry_fries1.mp3");
    }),
                                     DelayTime::create(15),
                                     CallFunc::create([=](){
        
        m_pColander = Sprite::create(localPath("colander_down1.png"));
        this->addChildToContentLayer(m_pColander);
        CMVisibleRect::setPositionAdapted(m_pColander, 320+visibleSize.width, 206+50);
        
        m_pColanderFront = _createDrageNode(localPath("colander_on3.png"));
        this->addChildToContentLayer(m_pColanderFront);
        m_pColanderFront->setLocalZOrder(10);
        m_pColanderFront->setPosition(m_pColander->getPosition());
        m_pColanderFront->setTouchEnabled(false);
        m_pColanderFront->setOrgPositionDefault();
        m_pColanderFront->index = 0;
        m_pColanderFront->setName("colander");
        m_pColander->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        m_pColanderFront->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                     CallFunc::create([=](){
            m_pColanderFront->setOrgPositionDefault();
        }), NULL));
        
        _showPotatoPlate();
        
    }), NULL));
    
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void FriesFryPotatoScene::onEnter()
{
    ExtensionScene::onEnter();
    
    Scene* scene = this->getScene();
    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
//    scene->getPhysicsWorld()->setAutoStep(false);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(FriesFryPotatoScene::updateStart), 2);
//    scene->getPhysicsWorld()->step(1/120.0f);
}
void FriesFryPotatoScene::updateStart(float delta)
{
    Scene* scene = this->getScene();
    scene->getPhysicsWorld()->setAutoStep(false);
    scheduleUpdate();
}
void FriesFryPotatoScene::update(float delta)

{
    
    // use fixed time and calculate 3 times per frame makes physics simulate more precisely.
    
    //这里表示先走3步瞧瞧  如果fps是1/60  三个setp就是1/180
    
    for (int i = 0; i < 3; ++i)
        
    {
        log("=======i %d");
        Scene* scene = this->getScene();
        scene->getPhysicsWorld()->step(1/180.0f);
        
    }
    
}

void FriesFryPotatoScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FriesFryPotatoScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    
    std::string name = pDragNode->getName();
    if (name=="colander") {
        int index = pDragNode->index;
        if (index==0) {
            pDragNode->getDragSprite()->setTexture(localPath("colander_on1.png"));
        }else if (index==1){
            m_pColander->setTexture(localPath("colander_down1.png"));
            pDragNode->getDragSprite()->setTexture(localPath("colander_on1.png"));
            m_pBar->pauseProgress();
        }
    }
}

void FriesFryPotatoScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="colander") {
        m_pColander->setPosition(pDragNode->getPosition());
    }
}

void FriesFryPotatoScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if (name=="plate") {
        Rect rect = m_pPot->getBoundingBox();
        rect.origin = m_pPot->getParent()->convertToWorldSpace(rect.origin);
        Rect plateRect = pDragNode->getDragSprite()->getBoundingBox();
        plateRect.origin = pDragNode->convertToWorldSpace(plateRect.origin);
        if (rect.intersectsRect(plateRect)) {
            m_pPlate->setTouchEnabled(false);
            m_pPlate->setPosition(m_pPot->getPosition()+Vec2(0, 300));
            
            auto body = PhysicsBody::create();
            body->setDynamic(false);
            
            PolygonHelp::getInstance()->addShapesWithFile("colander", body,-m_pColander->getContentSize().width/2,-m_pColander->getContentSize().height/2);
            body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
            m_pColander->setPhysicsBody(body);
            
            m_pPlate->runAction(Sequence::create(DelayTime::create(0.2),
                                                 RotateBy::create(0.5, -50),
                                                 CallFunc::create([=](){
                pDragNode->setLocalZOrder(0);
                _pourPotato();
            }),
                                                 DelayTime::create(4),
                                                 MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                m_pColanderFront->setTouchEnabled(true);
                m_pGuideLayer->showGuideMove(m_pColander->getPosition(),m_pColander->getPosition()+Vec2(0, -100));
            }),
                                                 NULL));
        }
    }else if (name=="salt") {
        Rect rect = m_pPot->getBoundingBox();
        rect.origin = m_pPot->getParent()->convertToWorldSpace(rect.origin);
        Rect plateRect = pDragNode->getDragSprite()->getBoundingBox();
        plateRect.origin = pDragNode->convertToWorldSpace(plateRect.origin);
        if (rect.intersectsRect(plateRect)){
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPot->getPosition()+Vec2(100, 200));
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, -60),
                                                  CallFunc::create([=](){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/salt.plist");
                pParticle->setScaleX(-1);
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
            }),
                                                  DelayTime::create(2),
                                                  CallFunc::create([=](){
                
                MenuLayer::addIngredient(MenuLayer::ePackageFries, MenuLayer::eFries_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }),
                                                  MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                  CallFunc::create([=](){
                m_pSalt->removeFromParent();
                m_pSalt = nullptr;
            }), NULL));
        }
        
    }else if (name=="colander") {
        int index = pDragNode->index;
        if (index==0) {
            pDragNode->setTouchEnabled(false);
            pDragNode->index = 1;
            pDragNode->setPosition(pDragNode->getOrgPosition()+Vec2(0,-20));
            m_pColander->setPosition(pDragNode->getPosition());
            m_pColander->setTexture(localPath("colander_down2.png"));
            pDragNode->getDragSprite()->setTexture(localPath("colander_on2.png"));
            _doFry();
        }else if (index==1){
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(pDragNode->getOrgPosition());
            m_pColander->setPosition(pDragNode->getPosition());
            m_pColander->setTexture(localPath("colander_down1.png"));
            pDragNode->getDragSprite()->setTexture(localPath("colander_on3.png"));
            _finish();
        }
    }
    m_pGuideLayer->removeGuide();
}

void FriesFryPotatoScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void FriesFryPotatoScene::_initData()
{
    setExPath("content/make/fries/2/");
}

void FriesFryPotatoScene::_showThermomter()
{
    Sprite* pSprite = Sprite::create(localPath("thermometer_1.png"));
    this->addChildToContentLayer(pSprite);
    CMVisibleRect::setPositionAdapted(pSprite, 360+visibleSize.width, 520);
    pSprite->runAction(Sequence::create(DelayTime::create(0.5),
                                        MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        JumpBy::create(0.5, Vec2(0,-134), 20, 1),
                                        CallFunc::create([=](){
        pSprite->setTexture(localPath("thermometer_2.png"));
        Label* pLabel = Label::createWithTTF("0", font_felt, 30);
        pLabel->setPosition(Vec2(85, 287));
        pLabel->setTextColor(Color4B::BLACK);
        pSprite->addChild(pLabel);
        pLabel->setTag(0);
        pLabel->setRotation(-68);
        pLabel->runAction(Repeat::create(Sequence::create(DelayTime::create(0.03),
                                                          CallFunc::create([=](){
            pLabel->setString(std::to_string(pLabel->getTag()+1));
            pLabel->setTag(pLabel->getTag()+1);
        }), NULL), 370));
        
        MenuLayer::addIngredient(MenuLayer::ePackageFries, MenuLayer::eFries_oil);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        
    }),
                                        DelayTime::create(13),
                                        JumpBy::create(1,Vec2::ZERO,50,2),
                                        DelayTime::create(1),
                                        MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
    
}

void FriesFryPotatoScene::_showPotatoPlate()
{
    m_pPlate = _createDrageNode(localRootPath("1/plate.png"));
    this->addChildToContentLayer(m_pPlate);
    m_pPlate->setName("plate");
    CMVisibleRect::setPositionAdapted(m_pPlate, 500+visibleSize.width, 625);
    
    int count = 30;
    for (int i = 0; i<count; i++) {
        Sprite* pSprite = Sprite::create(localPath("fry/"+std::to_string(i%4+1)+"_1.png"));
        m_pPlate->addChild(pSprite,3-count/10);
        Vec2 pos = Vec2(-100.0+i/10*3+20.0*(i%10), 0);
        pSprite->setPosition(pos);
        pSprite->setRotation(-10.0+5.0*(arc4random()%4));
        pSprite->setTag(i);
        m_PotatoVector.push_back(pSprite);
    }
    m_pPlate->setTouchEnabled(false);
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pPlate->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pPlate->getPosition(), m_pColanderFront->getPosition());
        m_pPlate->setTouchEnabled(true);
    }), NULL));
}

void FriesFryPotatoScene::_showSalt()
{
    m_pSalt = _createDrageNode(localRootPath("0/salt.png"));
    this->addChildToContentLayer(m_pSalt);
    m_pSalt->setName("salt");
    CMVisibleRect::setPositionAdapted(m_pSalt, 500+visibleSize.width, 660);
    m_pSalt->setActionPoint(Vec2(63, 178));
    m_pSalt->setTouchEnabled(false);
    m_pSalt->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSalt->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pSalt->getPosition(), m_pColanderFront->getPosition());
        m_pSalt->setTouchEnabled(true);
    }), NULL));
}

void  FriesFryPotatoScene::_pourPotato()
{
    
    for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=](Sprite* pSprite){
        int tag = pSprite->getTag();
        this->runAction(Sequence::create(DelayTime::create(0.1+0.1*tag),
                                         CallFunc::create([=](){
            pSprite->runAction(RotateTo::create(0.5, 0));
            
            auto body = PhysicsBody::createBox(Size(pSprite->getContentSize().width/2,pSprite->getContentSize().height*0.3),PhysicsMaterial(1.0f, 0.1f, 100.8f));
            body->setDynamic(true);
            pSprite->setPhysicsBody(body);
        }), NULL));
        
        Vec2 pos = pSprite->getPosition();
        pos = pSprite->getParent()->convertToWorldSpace(pos);
        float rotation = pSprite->getRotation();
        
        pSprite->retain();
        pSprite->removeFromParent();
        m_pColanderFront->addChild(pSprite,-1);
        pSprite->release();
        pSprite->setPosition(m_pColanderFront->convertToNodeSpace(pos));
        pSprite->setRotation(m_pPlate->getRotation()+rotation);
    });
}


void  FriesFryPotatoScene::_doFry()
{
    AudioHelp::getInstance()->playLoopEffect("fry.mp3");
    AudioHelp::getInstance()->playEffect("vo_fry_fries2.mp3");
    this->runAction(Sequence::create(DelayTime::create(3),
                                     CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("vo_fry_fries4.mp3");
    }), NULL));
//    Sprite* pSprite = Sprite::create(localPath("make3/oil.png"));
//    this->addChildToContentLayer(pSprite);
//    pSprite->setName("bubble");
//    pSprite->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
//        pSprite->setScaleX(-pSprite->getScaleX());
//    }),
//                                                              DelayTime::create(0.4), NULL)));
    
    
    for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=](Sprite* pSprite){
        pSprite->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                                       MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
    });
    
    ProgressBarNew* pBar = ProgressBarNew::create("content/common/progress/progress.png", "content/common/progress/progress_bar.png", "content/common/progress/p_mask.png");
    pBar->setFlag(70, 104, 5, 2, 2);
    pBar->setColorSegment(Color3B(255, 180, 0), Color3B(50, 228, 71), Color3B(228, 71, 50));
    
    pBar->progressNewState = CC_CALLBACK_2(FriesFryPotatoScene::_onProgressCallback, this);
    this->addChildToContentLayer(pBar);
    pBar->setVisible(false);
    
    pBar->beginProgress(50);
    m_pBar = pBar;
    _showSalt();
}

void FriesFryPotatoScene::_onProgressCallback(ProgressBarNew* pBar,int segment)
{
    if (segment<6) {
        int index = 0;
        for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=,&index](Sprite* pSprite){
            int tag = pSprite->getTag();
            std::stringstream ostr;
            ostr<<"fry/"<<index%4+1<<"_"<<segment+1<<".png";
            pSprite->setTexture(localPath(ostr.str()));
            index++;
        });
        if (segment==5) {
            m_pColanderFront->index = 1;
            m_pColanderFront->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(m_pColander->getPosition()+Vec2(0,-100),m_pColander->getPosition());
            AudioHelp::getInstance()->playEffect("fry_ok.mp3");
        }
    }else if (segment>6 && segment<9) {
        int index = 0;
        for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=,&index](Sprite* pSprite){
            int tag = pSprite->getTag();
            std::stringstream ostr;
            ostr<<"fry/"<<index%4+1<<"_"<<segment-1<<".png";
            pSprite->setTexture(localPath(ostr.str()));
            index++;
        });
        if (segment==7) {
            m_pColanderFront->index = 1;
            m_pColanderFront->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            bool isPlay = AudioHelp::getInstance()->playFryBurnedEffect();
            if (isPlay) {
                this->runAction(Sequence::create(DelayTime::create(4),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->stopFryBurnedEffect();
                }), NULL));
            }
        }
        if (m_pSalt) {
            m_pSalt->setTouchEnabled(false);
            m_pSalt->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                                CallFunc::create([=](){
                m_pSalt->removeFromParent();
                m_pSalt = nullptr;
            }), NULL));
        }
    }else if (segment == 9) {
        //        AudioHelp::getInstance()->playEffect("fry_failed.mp3");
        CMVisibleRect::setPositionAdapted(m_pColander, 320, 206);
        CMVisibleRect::setPositionAdapted(m_pColanderFront, 320, 206);
        m_pColanderFront->index = 0;
        m_pColanderFront->setTouchEnabled(false);
        m_pColanderFront->getDragSprite()->setTexture(localPath("colander_on3.png"));
        int index = 0;
        for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=,&index](Sprite* pSprite){
            pSprite->runAction(FadeOut::create(0.5));
        });
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke2.plist");
        this->addChildToContentLayer(pParticle);
        pParticle->setLocalZOrder(9);
        pParticle->setPosition(m_pPot->getPosition());
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(9);
            pParticle->setPosition(m_pPot->getPosition()-Vec2(0, 30));
            
            
            int index = 0;
            for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=,&index](Sprite* pSprite){
                pSprite->removeFromParent();
            });
            m_PotatoVector.clear();
            _showPotatoPlate();
//            Node* pBubble = pDragNode->getChildByName("bubble");
//            if (pBubble) {
//                pBubble->removeFromParent();
//            }
            AudioHelp::getInstance()->stopLoopEffect();
            for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=](Sprite* pSprite){
                pSprite->stopAllActions();
            });
        }), NULL));
    }
}

void FriesFryPotatoScene::_finish(){
    
    AudioHelp::getInstance()->stopLoopEffect();
    for_each(m_PotatoVector.begin(), m_PotatoVector.end(), [=](Sprite* pSprite){
        pSprite->stopAllActions();
    });
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<FriesDecorateScene>();
    }), NULL));
}