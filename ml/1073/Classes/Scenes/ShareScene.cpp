
#include "ShareScene.h"
#include "SceneManager.h"

ShareScene::ShareScene()
{
    m_pTip = nullptr;
    m_bBack = false;
    m_bCanEat = false;
    m_pPlate = nullptr;
    //    AudioHelp::getInstance()->registerEffectScene(ClassString(ShareScene));
}

ShareScene::~ShareScene()
{
    //    AudioHelp::getInstance()->removeEffectScene(ClassString(ShareScene));
}
bool ShareScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    
    m_bRequestEmail = false;
    //    bg
    m_nBgIndex = 1;
    if (GameDataManager::getInstance()->m_nPackage == 0) {
        m_pBg = Sprite::create("content/make/cotton/bg/bg3.jpg");
    }else{
        m_pBg = Sprite::create("content/make/snowcone/bg/bg4.png");
    }
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
//    m_pTable = Sprite::create(localPath("bg/bg_table2.png"));
//    CMVisibleRect::setPositionAdapted(m_pTable, 320, 380,kBorderNone,kBorderBottom);
//    this->addChildToBGLayer(m_pTable);
//    m_pTable->setLocalZOrder(1);
    
    _showEatScene();
    
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    m_pGameUI->showShareUILayout(false);
    m_pGameUI->showBackLayout();
//    m_pGameUI->showNextLayout();
    m_pGameUI->showSSCMore();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    return true;
}

void ShareScene::onEnter()
{
    ExtensionScene::onEnter();
    //    decorate界面有放大缩下功能  避免截屏到放大缩小的ui被截图下来
    
//    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_EAT);
}

void ShareScene::onExit()
{
    ExtensionScene::onExit();
}
void ShareScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagFav==tag) {
        
        
    }else if (GameUILayoutLayer::eUIButtonTagPhoto==tag){
        btn->setEnabled(false);
        AudioHelp::getInstance()->playCameraEffect();
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        m_bRequestEmail = false;
        {
#if __cplusplus > 201100L
            RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
                onPermissionGrantedResult(requestcode, bgranted);
            };
#else
            RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
            //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
            int requestCode = 1;
            //调用权限申请的方法,根据需要申请敏感权限
            RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
        }
        
    }else if (GameUILayoutLayer::eUIButtonTagEmail==tag){
        btn->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        
        
        m_bRequestEmail = true;
        {
#if __cplusplus > 201100L
            RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
                onPermissionGrantedResult(requestcode, bgranted);
            };
#else
            RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
            //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
            int requestCode = 1;
            //调用权限申请的方法,根据需要申请敏感权限
            RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
        }

    }else if (GameUILayoutLayer::eUIButtonTagEatAgain==tag){
        btn->setVisible(false);
        btn->setEnabled(true);
        m_pDecoLayer->removeAllChildren();
        m_pDecManager->showFinishedFood(this->m_pDecoLayer);
        _showSpoon();
        
        m_pDecManager->m_nEatPercent = 98;
        if (m_nPackage==0) {
            m_bCanEat = true;
            m_pDecManager->setShapeTexture("content/eatShape0.png");
        }else if (m_nPackage==2) {
            if (GameDataManager::getInstance()->m_nSpoonIndex != -1) {
                m_pDecManager->setShapeTexture("content/eatShape2.png");
            }else{
                
                m_pDecManager->setShapeTexture("content/eatShape.png");
            }
        }
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        m_pDecManager->reset();
        AudioHelp::getInstance()->resetEffect();
        GameDataManager::getInstance()->m_bShowAds = true;
        ;
        
        RateUsManager::getInstance()->onRateusCallback = [=](int type) {
            SceneManager::replaceTheScene<ChooseScene>();
        };
        RateUsManager::getInstance()->showRateUs();
        return;
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            KeypadDispatchCenter::getInstance()->removeDelegate(this);
            btn->setEnabled(true);
            m_pDecManager->reset();
            
            AudioHelp::getInstance()->resetEffect();
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
            
        };
        btn->setEnabled(true);
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
        return;
    }else if (GameUILayoutLayer::eUIButtonTagBg==tag){
        btn->setEnabled(false);
        m_nBgIndex++;
        if (m_nBgIndex%2==0) {
            Sprite* pBg = Sprite::create(localPath("bg_summer.jpg"));
            pBg->setPosition(Vec2(visibleSize.width/2, visibleSize.height*1.5));
            this->addChildToBGLayer(pBg);
            pBg->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                            CallFunc::create([=]()
                                                             {
                                                                 
                                                                 btn->loadTextures("content/common/button/btn_winter.png","content/common/button/btn_winter.png","content/common/button/btn_winter.png");
                                                                 m_pBg->setTexture(localPath("bg_summer.jpg"));
                                                                 btn->setEnabled(true);
                                                                 
                                                             }), NULL));
        }else{
            Sprite* pBg = Sprite::create(localPath("bg_winter.jpg"));
            pBg->setPosition(Vec2(visibleSize.width/2, visibleSize.height*1.5));
            this->addChildToBGLayer(pBg);
            pBg->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                            CallFunc::create([=]()
                                                             {
                                                                 btn->loadTextures("content/common/button/btn_summer.png","content/common/button/btn_summer.png","content/common/button/btn_summer.png");
                                                                 m_pBg->setTexture(localPath("bg_winter.jpg"));
                                                                 btn->setEnabled(true);
                                                                 
                                                             }), NULL));
        }
        
        
    }
    ExtensionScene::onButtonCallback(btn);
}
void ShareScene::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            if(m_bRequestEmail){
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _sendEmail();
                }), NULL));
            }else{
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _savePhoto();
                }), NULL));
                
            }
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
            
            this->runAction(Sequence::create(DelayTime::create(0.01),
                                             CallFunc::create([=](){
                Dialog* dialog=Dialog::create(Size(504*1.2,360*1.2), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
                Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
                dialog->setPosition(Director::getInstance()->getVisibleOrigin());
            }), NULL));

        }
    }else{
        log("-------->anroid runtime permisson was not granted ,%d,requestcode = %d",bGranted,requestCode);
    }
}
void ShareScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    m_pGuideLayer->removeGuide();
}

void ShareScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void ShareScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if (name == "spoon") {
        Rect rect = Rect(100, 375, 482, 538);
        rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                  CallFunc::create([=](){
                pDragNode->setLocalZOrder(20);
                m_pDecManager->doEat(pDragNode->getWorldSpaceActionPoint());
                
                Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+ "eattemp.png");
                Sprite* pMask = Sprite::create("content/eatShape2.png");
                Size size = pMask->getContentSize();
                if(pFood){
                    Size size2 = pMask->getContentSize();
                    pFood->setAnchorPoint(Vec2::ZERO);
                    pFood->setPosition(Vec2(-worldPoint.x+size.width/2, -worldPoint.y+size.height/2));
                }
                log("======%f,%f",pFood->getContentSize().width,pFood->getContentSize().height);
                log("======%f,%f",worldPoint.x,worldPoint.y);
                RenderTexture* pRT = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                           GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
                if(pFood){
                    pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
                    pMask->setPosition(size*0.5);
                    pRT->begin();
                    pFood->visit();
                    pMask->visit();
                    pRT->end();
                    
                }
                Director::getInstance()->getRenderer()->render();
                
                __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
                __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
                
                
                Sprite* pSpoonIn = Sprite::createWithTexture(pRT->getSprite()->getTexture());
                pSpoonIn->setFlippedY(true);
                //            Sprite* pSpoonIn = pRT->getSprite();
                pDragNode->addChild(pSpoonIn);
                pSpoonIn->setName("in");
                pSpoonIn->setRotation(-40);
                //            pSpoonIn->setScale(0.7);
                pSpoonIn->setPosition(pDragNode->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                
                pDragNode->runAction(Sequence::create(DelayTime::create(1),
                                                      MoveTo::create(0.5, Vec2(visibleSize.width/2,visibleSize.height*0.4)),
                                                      CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("eat_cream.mp3");
//                    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setPosition(pDragNode->getWorldSpaceActionPoint());
                    
                    Node* pNode = pDragNode->getChildByName("in");
                    if (pNode) {
                        pNode->removeFromParent();
                    }
                    
                    if(!m_pDecManager->finishedEat()){
                        pDragNode->setTouchEnabled(true);
                    }
                    if (m_pDecManager->finishedEat() && !m_pGameUI->eatAgainIsShow()){
                        m_pGameUI->showEatAgainLayout();
                        m_pGameUI->showNextLayout2();
                        
                        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
                        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                        this->addChildToUILayer(pParticle);
                        pParticle->setLocalZOrder(10);
                        pParticle->setPosition(visibleSize*0.5);
                    }
                }), NULL));
            }),
                                                  RotateBy::create(0.5, -20), NULL));
            
            
        }
    }
}
void ShareScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}
#pragma mark - initData
void ShareScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/make/");
    
//#if __cplusplus > 201100L
//    RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int requestcode,bool bgranted){
//        onPermissionGrantedResult(requestcode, bgranted);
//    };
//#else
//    RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
//#endif
//    //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
//    int requestCode = 1;
//    //调用权限申请的方法,根据需要申请敏感权限
//    RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kCamera | PERMISSION::kWriteExternalStorage);
}
void ShareScene::_showEatScene()
{
    m_pDecoLayer = Layer::create();
    this->addChildToContentLayer(m_pDecoLayer);
    
    m_pDecManager = DecorateManager::getInstance();
    m_pDecManager->showFinishedFood(m_pDecoLayer);
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(ShareScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(ShareScene::onEatEndCallback, this);
    m_pEatLayer->onEatMoveCallback = CC_CALLBACK_1(ShareScene::onEatMoveCallback, this);
    m_pEatLayer->setDrinkable(true);
//    Rect rect = Rect(310, 710-100, 190, 160);
//    rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
//    m_pEatLayer->setDrinkRect(rect);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
    if (GameDataManager::getInstance()->m_nPackage==0){
        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
        m_pDecManager->setShapeTexture("content/eatShape0.png");
        m_pDecManager->saveFoodImage();
        m_bCanEat = true;
        //        m_pEatLayer->setPosition(Vec2(-50,-visibleSize.height));
    }else{
        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
        //        m_pEatLayer->setPosition(Vec2(-50,-visibleSize.height));
    }
    m_pDecoLayer->runAction(action);
    m_pEatLayer->runAction(action->clone());
    
    _showSpoon();
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                       CallFunc::create([=](){
        if (m_nPackage==0){
            AudioHelp::getInstance()->playEffect("vo_cotton_eat_page.mp3");
        }else{
            if(GameDataManager::getInstance()->m_nSpoonIndex != -1){
                AudioHelp::getInstance()->playEffect("vo_snowcone_eat_page.mp3");
            }else{                
                AudioHelp::getInstance()->playEffect("vo_enjoy_it.mp3");
            }
        }
    }), NULL));
    
    if(GameDataManager::getInstance()->m_nPackage == 2 && GameDataManager::getInstance()->m_nSpoonIndex != -1){
        return;
    }
    m_pTip = Sprite::create("content/common/tte.png");
    this->addChildToUILayer(m_pTip);
    CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
    m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       DelayTime::create(2),
                                       CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_tap_enjoy.mp3");
    }), NULL));
    
}

void ShareScene::_showSpoon()
{
    int spoonIndex = GameDataManager::getInstance()->m_nSpoonIndex;
    if(GameDataManager::getInstance()->m_nPackage == 2 && spoonIndex != -1){
        DragNode* pSpoon = _createDrageNode("content/category/spoon/"+std::to_string(spoonIndex)+".png");
        pSpoon->setTouchEnabled(false);
        pSpoon->setName("spoon");
        pSpoon->setActionPoint(Vec2(64, 52));
        m_pDecoLayer->addChild(pSpoon);
        CMVisibleRect::setPositionAdapted(pSpoon, 400, 500+visibleSize.height);
        pSpoon->runAction(Sequence::create(DelayTime::create(2),
                                           MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                           CallFunc::create([=](){
            pSpoon->setOrgPositionDefault();
            pSpoon->setTouchEnabled(true);
            m_pGuideLayer->showGuideMove(pSpoon->getPosition(), pSpoon->getPosition()+Vec2(40, 50));
        }), NULL));
        m_pDecManager->saveFoodImage();
        m_pDecManager->setShapeTexture("content/eatShape2.png");
    }
}
RenderTexture* ShareScene::getResultRender()
{
    RenderTexture* render = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pBGLayer->visit();
    m_pContentLayer->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;
}
void ShareScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
    if (drinkable) {
        m_pDecManager->doDrink();
        AudioHelp::getInstance()->playDrinkEffect();
    }
    if (m_nPackage == 0) {
        if (!m_bCanEat) {
            return;
        }
        m_bCanEat = false;
        m_CandyBeginPos = location;
//        Rect rect = Rect(320-243, 300-212, 486, 425);
//        rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
//        if (rect.containsPoint(location)) {
            m_pDecManager->doEat(location);
            Sprite* pFood = Sprite::create(FileUtils::getInstance()->getWritablePath()+ "eattemp.png");
            Sprite* pMask = Sprite::create("content/eatShape0.png");
            Size size = pMask->getContentSize();
            if(pFood){
                Size size2 = pMask->getContentSize();
                pFood->setAnchorPoint(Vec2::ZERO);
                pFood->setPosition(Vec2(-location.x+size.width/2, -location.y+size.height/2));
            }
            RenderTexture* pRT = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                       GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
            if(pFood){
                pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
                pMask->setPosition(size*0.5);
                pRT->beginWithClear(0, 0, 0, 0);
                pFood->visit();
                pMask->visit();
                pRT->end();
                
            }
            Director::getInstance()->getRenderer()->render();
            
            __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
            __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
            
            
            Sprite* pCandy = Sprite::createWithTexture(pRT->getSprite()->getTexture());
            pCandy->setFlippedY(true);
            pCandy->setScale(1.2);
            this->addChildToContentLayer(pCandy);
            pCandy->setName("cottoncandy");
            pCandy->setPosition(location);
            pCandy->setRotation(10);
        pCandy->setLocalZOrder(20);
        
        Sprite* pSilk = Sprite::create("content/make/cotton/2/candy/"+std::to_string(GameDataManager::getInstance()->m_nFlavorIndex+1)+"_1.png");
        pSilk->setScale(1.6);
        pSilk->setPosition(pCandy->getContentSize()*0.5);
//        pSilk->setOpacity(150);
        pCandy->addChild(pSilk,5);
        
//        }

    }
}

void ShareScene::onEatMoveCallback(Vec2 location)
{
    if (m_nPackage == 0){
        Node* pNode = m_pContentLayer->getChildByName("cottoncandy");
        if(pNode){
            pNode->setPosition(location);
            
            Vec2 offset = location-m_CandyBeginPos;
            float dt = 10;
            if(abs(offset.x)<dt && abs(offset.y) <dt){
                return;
            }
            
            Sprite* pSilk = (Sprite*)pNode->getChildByName("silk");
            if (!pSilk) {
                pSilk = Sprite::create("content/make/cotton/2/silk/"+std::to_string(GameDataManager::getInstance()->m_nFlavorIndex+1)+"_4.png");
                pSilk->setName("silk");
                pSilk->setPosition(pNode->getContentSize()*0.5);
                pSilk->setAnchorPoint(Vec2(0.5,0.8));
//                pSilk->setAnchorPoint(Vec2(220.0/pSilk->getContentSize().width, 230.0/pSilk->getContentSize().height));
                pNode->addChild(pSilk);
                if(abs(offset.x)>abs(offset.y)){
                    if (offset.x<0) {
                        pSilk->setRotation(-90-10);
                        log("====left");
                    }else{
                        pSilk->setRotation(90-10);
                        log("====right");
                    }
                }else{
                    if (offset.y>0) {
                        pSilk->setRotation(0-10);
                        log("====up");
                    }else{
                        pSilk->setRotation(-180-10);
                        log("====down");
                    }
                }
                pSilk->setScale(0.5);
                pSilk->setOpacity(0);
//                pSilk->runAction(ScaleTo::create(0.5, 1,0.5));
                pSilk->runAction(FadeIn::create(0.5));
            }
        }
    }
}

void ShareScene::onEatEndCallback(Vec2 location)
{
    if (m_nPackage == 0) {
        Rect rect = Rect(320-243, 300-212, 486, 425);
        rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
        Vec2 pos = location;
//        if (rect.containsPoint(location)) {
//            pos = Vec2(visibleSize.width, visibleSize.height*0.3);
//        }
        Node* pNode = m_pContentLayer->getChildByName("cottoncandy");
        if(pNode){
            pNode->setPosition(pos);
            pNode->runAction(Sequence::create(ScaleTo::create(0.3,0.5),
                                              CallFunc::create([=](){
                m_bCanEat = true;
                pNode->removeFromParent();
            }), NULL));
        }
        
        AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setPosition(pos);
        
        
        if (m_pDecManager->finishedEat() && !m_pGameUI->eatAgainIsShow()){
            m_pGameUI->showEatAgainLayout();
            m_pGameUI->showNextLayout2();
            
            AudioHelp::getInstance()->playEffect("cheer_star.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
        }
//        m_pDecManager->saveFoodImage();
        
        if (m_pTip) {
            m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               CallFuncN::create([=](Node* node)
                                                                 {
                                                                     node->removeFromParent();
                                                                 }),
                                               NULL));
            m_pTip = nullptr;
        }
        return;
    }
    if(GameDataManager::getInstance()->m_nPackage == 2 &&
       GameDataManager::getInstance()->m_nSpoonIndex != -1){
        return;
    }
    if (m_pTip) {
        m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFuncN::create([=](Node* node)
                                                             {
                                                                 node->removeFromParent();
                                                             }),
                                           NULL));
        m_pTip = nullptr;
    }
    
    Sprite* pNode = (Sprite*)m_pDecoLayer->getChildByName("bag");
    if (pNode) {
        pNode->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                          ScaleTo::create(0.5, 1),
                                          JumpBy::create(0.5, Vec2(visibleSize.width, 100), 300, 1),
                                          CallFuncN::create([=](Node* node)
                                                            {
                                                                node->removeFromParent();
                                                            }),
                                          NULL));
        return;
    }
    
    if (m_nPackage == 2){
        AudioHelp::getInstance()->playEffect("eat_cream.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("basic/eat.mp3");
    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(location);
    m_pDecManager->doEat(location);
    m_pDecManager->stopDrink();
    AudioHelp::getInstance()->stopDrinkEffect();
    if (m_pDecManager->finishedEat() && !m_pGameUI->eatAgainIsShow()) {
        //        if (GameDataManager::getInstance()->m_nPackage==ePackageLemonade) {
        //            m_pGameUI->showEatAgainLayout(true);
        //        }else{
        //            m_pGameUI->showEatAgainLayout();
        //        }
        m_pGameUI->showEatAgainLayout();
        m_pGameUI->showNextLayout2();
        
        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }
}

void ShareScene::onKeyBackClicked()
{
    Node* pDialog = Director::getInstance()->getRunningScene()->getChildByTag(9999);
    if (pDialog) {
        pDialog->removeFromParent();
        return;
    }
    if (m_bBack) {
        return;
    }
    log("========ShareScene::onKeyBackClicked");
    //    AudioHelp::getInstance()->playPreviousEffect();
    //    SceneManager::popScene();
    Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog->setContentText("Do you want to start over with a new food?");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
        m_pDecManager->reset();
        m_bBack = true;
        
        
        //        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
        //            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
        //            AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
        //                SceneManager::popToRootScene();
        //                SceneManager::replaceTheScene<HomeScene>();
        //            };
        //        }else{
        //            SceneManager::popToRootScene();
        //            SceneManager::replaceTheScene<HomeScene>();
        //        }
        SceneManager::popToRootScene();
        SceneManager::replaceTheScene<HomeScene>();
        
    };
}

void ShareScene::onBgCallback(int index)
{
    m_pBg->setTexture(localPath("bg_winter.jpg"));
    
    GameDataManager::getInstance()->m_nBgIndex = index;
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
}

void ShareScene::_savePhoto()
{
    Image* image = getResultRender()->newImage();
    SSCInternalLibManager::getInstance()->saveToAlbum(image, [=](bool ok){
        if (ok) {
            Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
            dialog->setContentText("Your image has been saved to your Camera Roll!");
            Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
            dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        }else{
            Dialog* dialog=Dialog::create(Size(504*1.2,360*1.2), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
            dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
            Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
            dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        }
        delete image;
    },"Fair Food");
}
void ShareScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Fair Food!",
                                                              "<p>Look at the Fair Food I just made.Isn't it so lovely? There are still tons of tools and decorations in Fair Food you'll want to try!Stop hesitating! Download now!You are gonna love it!</p>  <p><a href=‘http://itunes.apple.com/app/id682415372’>http://itunes.apple.com/app/id682415372</a></p>",
                           (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Fair Food!",
                                                              "Look at the Fair Food I just made.Isn't it so lovely? There are still tons of tools and decorations in Fair Food you'll want to try!Stop hesitating! Download now!You are gonna love it!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}