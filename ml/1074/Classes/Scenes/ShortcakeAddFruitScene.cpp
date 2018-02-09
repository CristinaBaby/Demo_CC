
#include "ShortcakeAddFruitScene.h"
#include "SceneManager.h"
#include "PolygonHelp.h"

static int gnFruitCutCount[] = {
    1,1,5,5,1,5,5,5,4,5,4,1
};

ShortcakeAddFruitScene::ShortcakeAddFruitScene()
{
    
}

ShortcakeAddFruitScene::~ShortcakeAddFruitScene()
{
    
}
bool ShortcakeAddFruitScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showFood();
    _showBowl();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
//        _showBowl();
    }), NULL));
    m_pGameUI->showNormalLayout();
    return true;
}

void ShortcakeAddFruitScene::onEnter()
{
    ExtensionScene::onEnter();
    Scene* scene = this->getScene();
    
//            scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,-98));
    scene->getPhysicsWorld()->setAutoStep(false);
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(ShortcakeAddFruitScene::updateStart), 2);
}

void ShortcakeAddFruitScene::updateStart(float delta)
{
    Scene* scene = this->getScene();
    scene->getPhysicsWorld()->setAutoStep(false);
    scheduleUpdate();
}
void ShortcakeAddFruitScene::update(float delta)

{
    
    // use fixed time and calculate 3 times per frame makes physics simulate more precisely.
    
    //这里表示先走3步瞧瞧  如果fps是1/60  三个setp就是1/180
    
    for (int i = 0; i < 3; ++i)
        
    {
        Scene* scene = this->getScene();
        scene->getPhysicsWorld()->step(1/180.0f);
        
    }
    
}

void ShortcakeAddFruitScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ShortcakeAddFruitScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void ShortcakeAddFruitScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void ShortcakeAddFruitScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if (name == "bowl") {
        if (worldPoint.y<visibleSize.height*0.6) {
            _pourFruit();
        }else{
            pDragNode->back();
        }
    }
}

void ShortcakeAddFruitScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ShortcakeAddFruitScene::_initData()
{
    setExPath("content/make/shortcake/make3/");
    m_nFlavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
}

void ShortcakeAddFruitScene::_showBowl()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pBowl = _createDrageNode(localPath("fruit_bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 780);
    m_pBowl->setTouchEnabled(false);
    m_pBowl->setName("bowl");
    m_pBowl->runAction(Sequence::create(DelayTime::create(1),
                                        MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                        CallFunc::create([=](){
        m_pBowl->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pBowl->getPosition(), visibleSize*0.5);
        m_pBowl->setTouchEnabled(true);
    }),
                                        DelayTime::create(0.2),
                                        CallFunc::create([=](){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        PolygonHelp::getInstance()->addShapesWithFile("fruit_bowl", body,-m_pBowl->getContentSize().width,-m_pBowl->getContentSize().height);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pBowl->setPhysicsBody(body);
        
        for(auto pSlice:m_Fruits) {
            auto body = PhysicsBody::createCircle(pSlice->getContentSize().width*0.2,PhysicsMaterial(1.0f, 0.0f, 1.0f));
            body->setDynamic(true);
            pSlice->setPhysicsBody(body);
        }
    }),NULL));
    m_pBowlFront = Sprite::create(localPath("fruit_bowl1.png"));
    m_pBowl->addChild(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
//    m_pBowlFront->setPosition(m_pBowl->getPosition());
//    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    
    int cutCount = gnFruitCutCount[m_nFlavorIndex];
    for (int k = 0; k<14; k++) {
        std::string Path;
        if (cutCount==1) {
            Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(k%2+cutCount+1)+".png";
        }else if (m_nFlavorIndex==10) {
            Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_6.png";
        }else if (m_nFlavorIndex == 8) {
            Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_5.png";
        }else{
            Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(cutCount)+".png";
        }
        Sprite* pSlice = Sprite::create(localPath(Path));
        m_pBowl->addChild(pSlice);
        pSlice->setPosition(Vec2(50-10.0*(arc4random()%10), -50+5.0*(arc4random()%10)));
        pSlice->setLocalZOrder(5);
        pSlice->setRotation(50.0-10.0*(arc4random()%10));
        
//        auto body = PhysicsBody::createCircle(pSlice->getContentSize().width*0.2,PhysicsMaterial(1.0f, 0.0f, 1.0f));
//        body->setDynamic(true);
//        pSlice->setPhysicsBody(body);
        
        m_Fruits.pushBack(pSlice);
    }
    
    
//    for(auto pSlice:m_Fruits) {
//        auto body = PhysicsBody::createCircle(pSlice->getContentSize().width*0.2,PhysicsMaterial(1.0f, 0.0f, 1.0f));
//        body->setDynamic(true);
//        pSlice->setPhysicsBody(body);
//    }
}

void ShortcakeAddFruitScene::_showFood()
{
    std::string strCake = GameDataManager::getInstance()->m_sFlavorCakeStr;
    std::string strCream = GameDataManager::getInstance()->m_sFlavorCreamStr;
    std::string strFruit = GameDataManager::getInstance()->m_sFlavorStr;
    std::vector<std::string> flavorCakes = StringHelp::split(strCake, ",");
    std::vector<std::string> flavorCreams = StringHelp::split(strCream, ",");
    std::vector<std::string> flavorFruits = StringHelp::split(strFruit, ",");
    
    int cakeCount = flavorCakes.size();
    m_nCakeCount = cakeCount;
    m_pPlate = Sprite::create(localRootPath("dec/cake_plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 300-visibleSize.height);
    
    m_pFood = Node::create();
    this->addChildToContentLayer(m_pFood);
    CMVisibleRect::setPositionAdapted(m_pFood, 320, 350-visibleSize.height);
    
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                         CallFunc::create([=](){
        if (cakeCount>3) {
            m_pPlate->runAction(MoveBy::create(0.5, Vec2(0, -80*(cakeCount-3))));
        }
    }), NULL));
    m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                        CallFunc::create([=](){
        if (cakeCount>3) {
            m_pFood->runAction(MoveBy::create(0.5, Vec2(0, -80*(cakeCount-3))));
        }
    }), NULL));
    
    for(int i = 0;i<cakeCount;i++) {
        Sprite* pCake = Sprite::create(localRootPath("dec/cake"+std::to_string(1+atoi(flavorCakes.at(i).c_str()))+".png"));
        m_pFood->addChild(pCake);
        pCake->setPosition(Vec2(0, 80*i));
        
        Sprite* pCream = Sprite::create(localRootPath("dec/cream"+std::to_string(1+atoi(flavorCreams.at(i).c_str()))+".png"));
        m_pFood->addChild(pCream);
        pCream->setPosition(Vec2(0, 80*i));
        
        if (i == cakeCount-1) {
            m_CakeSize = pCake->getContentSize();
        }else{
            TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"fruit"+std::to_string(i)+".png");
            Sprite* pFruit = Sprite::create(FileUtils::getInstance()->getWritablePath()+"fruit"+std::to_string(i)+".png");
            if (pFruit) {
                m_pFood->addChild(pFruit);
                pFruit->setPosition(Vec2(0, 80*i+40));
            }
        }
    }
    m_pFruit = Node::create();
    m_pFood->addChild(m_pFruit);
    m_pFruit->setPosition(Vec2(0, 80*m_nCakeCount-40));
}


void ShortcakeAddFruitScene::_pourFruit()
{
    m_pBowl->setTouchEnabled(false);
    m_pGuideLayer->removeGuide();
    
    m_pBowl->runAction(Sequence::create(MoveTo::create(0.5, m_pFood->getPosition()+Vec2(100, 80*(m_nCakeCount-1)+300)),
                                        RotateBy::create(0.5, -120),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pour_corn_kernal.mp3");
        
    }),
                                        DelayTime::create(2),
                                        CallFunc::create([=](){
        
        for(auto pSlice:m_Fruits) {
            pSlice->removeFromParent();
        }
        int cutCount = gnFruitCutCount[m_nFlavorIndex];
        
        for(int i=0;i<7;i++){
            std::string Path;
            float dX = -150+50*i;
            float dY = (1.0-pow(dX, 2)/pow(150, 2))*pow(50, 2);
            int count = 2;
            if(dY<=0.001f&&dY>=0.0010f){
                count = 1;
            }
            if (cutCount==1) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string((i+count)%2+cutCount+1)+".png";
            }else if (m_nFlavorIndex==10) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_6.png";
            }else if (m_nFlavorIndex == 8) {
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_5.png";
            }else{
                Path = "fruit"+std::to_string(m_nFlavorIndex+1)+"_"+std::to_string(cutCount)+".png";
            }
            for (int k = 0; k<count; k++) {
                Sprite* pSlice = Sprite::create(localPath(Path));
                m_pFruit->addChild(pSlice);
                float posY = sqrt(dY)*(k==0?1:-1);
                log("=========%f,%f",dX,posY);
                pSlice->setPosition(Vec2(dX, posY));
                pSlice->setLocalZOrder(5);
                pSlice->setRotation(50.0-10.0*(arc4random()%10));
            }
            
        }
    }),
                                        RotateTo::create(0.5, 0),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->_playEffectNiceWork();
        
    }),
                                        MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        _finish();
    }), NULL));
    
}

void ShortcakeAddFruitScene::_saveFruit()
{
    Size candySize = m_CakeSize;
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 pos = m_pFruit->getPosition();
    m_pFruit->setPosition(Vec2(candySize.width/2, candySize.height/2));
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pFruit->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    m_pFruit->setPosition(pos);
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"fruit"+std::to_string(m_nCakeCount-1)+".png", false);
    pImage->autorelease();
    
}

void ShortcakeAddFruitScene::_saveCake()
{
    Size candySize = Size(m_CakeSize.width, m_CakeSize.height+80*m_nCakeCount);
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 pos = m_pFood->getPosition();
    m_pFood->setPosition(Vec2(candySize.width/2, m_CakeSize.height/2));
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pFood->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    m_pFood->setPosition(pos);
    Image* pImage = render->newImage();
    
    bool issuccess;
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"cake.png", false);
    
    pImage->autorelease();
}
void ShortcakeAddFruitScene::_finish()
{
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(0.8),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_make_more_layer.mp3");
    }),DelayTime::create(3),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_skip_button.mp3");
    }), NULL));
    Button* pSkip = Button::create("content/common/button/skip.png","content/common/button/skip.png","content/common/button/skip.png");
    this->addChildToUILayer(pSkip);
    CMVisibleRect::setPositionAdapted(pSkip, 480, 600+visibleSize.height);
    pSkip->setTouchEnabled(false);
    pSkip->runAction(Sequence::create(DelayTime::create(2),
                                      EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                      CallFunc::create([=](){
        pSkip->setTouchEnabled(true);
    }), NULL));
    
    Button* pMakeAgain = Button::create("content/common/button/more_layer.png","content/common/button/more_layer.png","content/common/button/more_layer.png");
    this->addChildToUILayer(pMakeAgain);
    CMVisibleRect::setPositionAdapted(pMakeAgain, 160, 600+visibleSize.height);
    pMakeAgain->setTouchEnabled(false);
    pMakeAgain->runAction(Sequence::create(DelayTime::create(2),
                                           EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                           CallFunc::create([=](){
        pMakeAgain->setTouchEnabled(true);
    }), NULL));
    
    pSkip->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if (type!=Widget::TouchEventType::ENDED) {
            return;
        }
        _saveCake();
        pSkip->runAction(Spawn::create(ScaleTo::create(0.5, 0.1,0.1),
                                       FadeOut::create(0.5), NULL));
        pMakeAgain->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.1,0.1),
                                                             FadeOut::create(0.5), NULL),
                                               CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("magic_effect.mp3");
        }),
                                               DelayTime::create(1),
                                               CallFunc::create([=](){
            SceneManager::replaceTheScene<DecorateScene>();
        }), NULL));
    });
    pMakeAgain->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if (type!=Widget::TouchEventType::ENDED) {
            return;
        }
        _saveFruit();
        pSkip->runAction(Spawn::create(ScaleTo::create(0.5, 0.1,0.1),
                                       FadeOut::create(0.5), NULL));
        pMakeAgain->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.1,0.1),
                                                             FadeOut::create(0.5), NULL),
                                               CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("magic_effect.mp3");
        }),
                                               DelayTime::create(1),
                                               CallFunc::create([=](){
            SceneManager::replaceTheScene<ShortcakeMixFlourScene>();
        }), NULL));
    });
}