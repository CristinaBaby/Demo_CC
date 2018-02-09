
#include "ShareScene.h"
#include "SceneManager.h"

static std::string gsModels[] = {
    "round",
    "heart",
    "cat",
    "star",
    "ginger",
    "tree",
};
ShareScene::ShareScene()
{
    m_pTip = nullptr;
    m_bBack = false;
    m_bCanEat = false;
    m_pPlate = nullptr;
    m_bButtonEnable = true;
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
    
    m_bRequestEmail = false;
    //    bg
    m_nBgIndex = 1;
    m_pBg = Sprite::create("content/make/common/bg2.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
//    m_pTable = Sprite::create("content/make/common/bg1.png");
//    CMVisibleRect::setPositionAdapted(m_pTable, 320, 0,kBorderNone,kBorderBottom);
//    this->addChildToBGLayer(m_pTable);
//    m_pTable->setAnchorPoint(Vec2(0.5,0));
//    m_pTable->setLocalZOrder(1);
    
    _showEatScene();
    
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    m_pGameUI->showShareUILayout(false);
    m_pGameUI->showBackLayout2();
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("music_home.mp3");
    return true;
}

void ShareScene::onEnter()
{
    ExtensionScene::onEnter();
    //    decorate界面有放大缩下功能  避免截屏到放大缩小的ui被截图下来
    
    //    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_EAT);
//    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_EAT);
}

void ShareScene::onExit()
{
    //    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_EAT);
    ExtensionScene::onExit();
}
void ShareScene::onButtonCallback(Button* btn)
{
    if(!m_bButtonEnable){
        return;
    }
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagFav==tag) {
        
        
    }else if (GameUILayoutLayer::eUIButtonTagPhoto==tag){
        btn->setEnabled(false);
        m_bButtonEnable = false;
        this->runAction(Sequence::create(DelayTime::create(2),
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
        AudioHelp::getInstance()->resetEffect();
        _reset();
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        m_pDecManager->reset();
        AudioHelp::getInstance()->resetEffect();
        GameDataManager::getInstance()->m_bShowAds = true;
        ;
        
        RateUsManager::getInstance()->onRateusCallback = [=](int type) {
            
            ChoosePackageLayer* pLayer = ChoosePackageLayer::create();
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(1000);
            pLayer->onChooseCallback = [=](int index){
                if (index == 0) {
                    SceneManager::replaceTheScene<ClassicChooseScene>();
                }else{
                    SceneManager::replaceTheScene<CaramelMeltSugarScene>();
                }
            };
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
            m_bButtonEnable = true;
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
#pragma mark - initData
void ShareScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/make/");
}
void ShareScene::_showEatScene()
{
    m_pDecoLayer = Layer::create();
    this->addChildToContentLayer(m_pDecoLayer);
    
    m_pDecManager = DecorateManager::getInstance();
//    m_pDecManager->showFinishedFood(m_pDecoLayer);
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(ShareScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(ShareScene::onEatEndCallback, this);
    m_pEatLayer->setDrinkable(false);
//    Rect rect = Rect(310, 710-100, 190, 160);
//    rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
//    m_pEatLayer->setDrinkRect(rect);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
//    if (GameDataManager::getInstance()->m_nPackage==0){
//        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
//    }else{
//        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
//    }
    m_pEatLayer->runAction(action);
//    m_pDecoLayer->runAction(action->clone());
    
    
//    _showCookie(false);
    _showCookie(false);
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
}

void ShareScene::onEatEndCallback(Vec2 location)
{
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
    
    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
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

void ShareScene::_showCookieDown(bool animation)
{
    if (!m_pDecoLayer) {
        m_pDecoLayer = Layer::create();
        this->addChildToContentLayer(m_pDecoLayer);
    }else{
        m_pDecoLayer->removeAllChildren();
    }
    m_pDecManager = DecorateManager::getInstance();
    
    if(animation){
        m_pDecoLayer->setPosition(Vec2(-visibleSize.width, 0));
        m_pDecoLayer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    }
    
    int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    if (flavorIndex == 5) {
        m_pDecManager->m_nEatPercent = 78;
        MakeEatLayer* pEatLayer = MakeEatLayer::create();
        Sprite* pCake = Sprite::create("content/make/caramel/dec/cookie1.png");
        
        m_pPlate = Sprite::create("content/make/caramel/dec/plate.png");
        //        this->addChildToContentLayer(m_pPlate);
        m_pDecoLayer->addChild(m_pPlate);
        CMVisibleRect::setPositionAdapted(m_pPlate, 350, 410-30);
        //        m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        
        Size size = m_pPlate->getContentSize();
        if (pCake) {
            size = pCake->getContentSize();
        }
        
        pCake->setPosition(Vec2(size.width/2, size.height/2));
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
        rt->setPosition(Vec2(0, 0));
        rt->beginWithClear(0, 0, 0, 0);
        pCake->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        pEatLayer->setRenderTexture(rt);
        
        MyRenderTexture* rt2 = createShadowRender(pCake);
        pEatLayer->setRenderTextureInner(rt2);
        
        CMVisibleRect::setPositionAdapted(rt, 350, 420-30);
        CMVisibleRect::setPositionAdapted(rt2, 350, 420-30);
        
        m_pDecoLayer->addChild(pEatLayer);
        m_pDecManager->setEatableLayer(pEatLayer);
    }else{
        m_pDecManager->m_nEatPercent = 97;
        m_pDecManager->showFinishedFood(m_pDecoLayer);
    }
    
    
    
    
    
}

void ShareScene::_showCookie(bool animation)
{
    if (!m_pDecoLayer) {
        m_pDecoLayer = Layer::create();
        this->addChildToContentLayer(m_pDecoLayer);
    }else{
        m_pDecoLayer->removeAllChildren();
    }
    m_pDecManager = DecorateManager::getInstance();
    
    if(animation){
        m_pDecoLayer->setPosition(Vec2(-visibleSize.width, 0));
        m_pDecoLayer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    }
    
    MakeEatLayer* pEatLayer = MakeEatLayer::create();
    
    int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    if (flavorIndex == 5) {
        Sprite* pCake = Sprite::create("content/make/caramel/dec/cookie1.png");
        
        m_pPlate = Sprite::create("content/make/caramel/dec/plate.png");
        //        this->addChildToContentLayer(m_pPlate);
        m_pDecoLayer->addChild(m_pPlate);
        CMVisibleRect::setPositionAdapted(m_pPlate, 350, 410-30,kBorderNone,kBorderBottom);
        //        m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        
        Size size = m_pPlate->getContentSize();
        if (pCake) {
            size = pCake->getContentSize();
        }
        
        pCake->setPosition(Vec2(size.width/2, size.height/2));
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
        rt->setPosition(Vec2(0, 0));
        rt->beginWithClear(0, 0, 0, 0);
        pCake->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        pEatLayer->setRenderTexture(rt);
        
        MyRenderTexture* rt2 = createShadowRender(pCake);
        pEatLayer->setRenderTextureInner(rt2);
        
        CMVisibleRect::setPositionAdapted(rt, 350, 420-30,kBorderNone,kBorderBottom);
        CMVisibleRect::setPositionAdapted(rt2, 350, 420-30,kBorderNone,kBorderBottom);
    }else{
        std::stringstream ostr;
        int shapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
        int plateIndex = GameDataManager::getInstance()->m_nPlateIndex;
        if (flavorIndex == 4) {
            ostr<<"content/make/classic/dec/chocolate_chip.png";
        }else{
            ostr<<"content/make/classic/dec/"<<gsModels[shapeIndex]<<"_"<<flavorIndex+1<<".png";
        }
//        Sprite* pCake = Sprite::create(ostr.str());
//        pCake->setFlippedX(true);
        
        Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"dec.png");
//        pDec->setScale(0.95,0.72);
        
        m_pPlate = Sprite::create("content/category/plate/"+std::to_string(plateIndex)+".png");
        m_pDecoLayer->addChild(m_pPlate);
        CMVisibleRect::setPositionAdapted(m_pPlate, 350, 420-30,kBorderNone,kBorderBottom);
//        m_pPlate->setScale(0.95,0.7);
        
        Size size = pDec->getContentSize();
//        if (pCake) {
//            size = pCake->getContentSize()*1.2;
//        }
        
//        pCake->setPosition(Vec2(size.width/2, size.height/2));
        pDec->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
        rt->setPosition(Vec2(0, 0));
        rt->beginWithClear(0, 0, 0, 0);
//        pCake->visit();
        pDec->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        pEatLayer->setRenderTexture(rt);
        
        MyRenderTexture* rt2 = createShadowRender(pDec);
        pEatLayer->setRenderTextureInner(rt2);
        
        CMVisibleRect::setPositionAdapted(rt, 350, 420-30,kBorderNone,kBorderBottom);
        CMVisibleRect::setPositionAdapted(rt2, 350, 420-30,kBorderNone,kBorderBottom);
    }
    
    
    
    
    
    m_pDecoLayer->addChild(pEatLayer);
    m_pDecManager->setEatableLayer(pEatLayer);
}

MyRenderTexture* ShareScene::createShadowRender(Sprite* pNode)
{
    Size size = pNode->getContentSize();
    MyRenderTexture* rt2 = MyRenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                   GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    rt2->setPosition(Vec2(size.width*0.5,size.height*0.5));
    
    Sprite* pSpriteTexture = Sprite::createWithTexture(pNode->getTexture());
    Sprite* pSprite = Sprite::createWithTexture(pNode->getTexture());
    pSpriteTexture->setPosition(Vec2(size.width*0.5, size.height*0.5));
    pSprite->setPosition(Vec2(size.width*0.5, size.height*0.5));
    
    pSprite->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    LayerColor* pColor = LayerColor::create(Color4B(180, 180, 180, 150));
    //    pColor->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pSpriteTexture->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    
    rt2->beginWithClear(0, 0, 0, 0);
    pSpriteTexture->visit();
    pColor->visit();
    pSprite->visit();
    
    rt2->end();
    
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt2, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt2, EVENT_COME_TO_FOREGROUND);
    return rt2;
}

void ShareScene::_reset()
{
//    if (GameDataManager::getInstance()->m_nFlavorIndex ==5) {
//        m_pDecManager->reset();
        _showCookie(false);
//    }else{
//        m_pDecoLayer->removeAllChildren();
//        m_pDecManager->showFinishedFood(this->m_pDecoLayer);
//        
//    }
}

void ShareScene::_savePhoto()
{
    Image* image = getResultRender()->newImage();
    AudioHelp::getInstance()->playCameraEffect();
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
        image->autorelease();
        m_bButtonEnable= true;
    },"Cookie");
}
void ShareScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Cookie!",
                                                              "<p>Look at the Cookie I just made.Isn't it so lovely? There are still tons of tools and decorations in Cookie you'll want to try!Stop hesitating! Download now!You are gonna love it!</p>  <p><a href=‘http://itunes.apple.com/app/id492034111’>http://itunes.apple.com/app/id492034111</a></p>",
                           (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Cookie!",
                                                              "Look at the Cookie I just made.Isn't it so lovely? There are still tons of tools and decorations in Cookie you'll want to try!Stop hesitating! Download now!You are gonna love it!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}