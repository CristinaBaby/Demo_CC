
#include "ShareScene.h"
#include "SceneManager.h"

static std::string gPackageName[] = {
    "lemonade",
    "hotdog",
    "frenchfries",
    "snowcone",
    "churro",
    "burrito",
    "funnelcake",
    "frybread",
};
static  Vec2 gKidPos[] = {
    Vec2(170,225),
    Vec2(830,280),
    Vec2(640,340),
    Vec2(500,350),
};
ShareScene::ShareScene()
{
    m_pTip = nullptr;
    m_bBack = false;
}

ShareScene::~ShareScene()
{
    
}
bool ShareScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_nBgIndex = 1;
    m_pBg = Sprite::create("content/make/choose/bg_beach.png");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    NodeGrid* pGrid = NodeGrid::create();
    pGrid->setPosition(m_pBg->getContentSize()*0.5);
    m_pBg->addChild(pGrid);
    
    Sprite* pWave = Sprite::create("content/make/choose/bg_sea wave.png");
    pGrid->addChild(pWave);
    pWave->setPosition(Vec2::ZERO);
    
    pGrid->runAction(RepeatForever::create(Waves::create(3, Size(10, 10), 1, 5, true, true)));
    m_pGrid = pGrid;

    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    CMVisibleRect::setPosition(pTable, 320, 180);
    this->addChildToBGLayer(pTable);
    _showEatScene();
    
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    m_pGameUI->showShareUILayout(false);
    m_pGameUI->showBackLayout();
    m_pGameUI->showNextLayout();
    return true;
}

void ShareScene::onEnter()
{
    ExtensionScene::onEnter();
//    decorate界面有放大缩下功能  避免截屏到放大缩小的ui被截图下来
    
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_EAT_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_EAT_ICECREAM);
    }
}

void ShareScene::onExit()
{
    if (GameDataManager::getInstance()->m_nPackage==0){
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_EAT_SNOWCONE);
    }else{
        FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_EAT_ICECREAM);
    }
    ExtensionScene::onExit();
}
void ShareScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagFav==tag) {

        
    }else if (GameUILayoutLayer::eUIButtonTagPhoto==tag){
        Image* image = getResultRender()->newImage();
        STSystemFunction st;
        btn->setEnabled(false);
        AudioHelp::getInstance()->playCameraEffect();
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        
        st.saveToAlbum(image, [=](bool ok){
            btn->setEnabled(true);
            if (ok) {
                Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                dialog->setContentText("Your image has been saved to your Camera Roll!");
                Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
                dialog->setPosition(Director::getInstance()->getVisibleOrigin());
            }else{
                Dialog* dialog=Dialog::create(Size(497*1.3,355*1.3), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
                Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
                dialog->setPosition(Director::getInstance()->getVisibleOrigin());
            }
            delete image;
        },"Beach Food");
        
        
    }else if (GameUILayoutLayer::eUIButtonTagEmail==tag){
        STSystemFunction st;
        btn->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        Image* pImage = getResultRender()->newImage();
        pImage->saveToFile(FileUtility::getStoragePath()+"shotscreen.png", false);
        pImage->autorelease();
        st.sendEmailAndFilePic("Beach Food!",
                               "<p>Look at this Beach Food I just made. So yummy， tons of flavors and decorations to have fun. Let's play together.</p>  <p><a href=‘http://itunes.apple.com/app/id1134823961’>http://itunes.apple.com/app/id1134823961</a></p>",
                               (FileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
        Image* pImage = getResultRender()->newImage();
        pImage->saveToFile(FileUtility::getStoragePath()+"/shotscreen.png", false);
        pImage->autorelease();
        st.sendEmailAndFilePic("Beach Food!",
                               "Look at this Beach Food I just made. So yummy， tons of flavors and decorations to have fun. Let's play together.",
                               (FileUtility::getStoragePath()+"/shotscreen.png").c_str());
#endif
    }else if (GameUILayoutLayer::eUIButtonTagEatAgain==tag){
        btn->setVisible(false);
        btn->setEnabled(true);
        m_pDecoLayer->removeAllChildren();
        m_pDecManager->showFinishedFood(this->m_pDecoLayer);
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        m_pDecManager->reset();
        SceneManager::popToRootScene();
        SceneManager::replaceTheScene<ChoosePackageScene>();
        
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            KeypadDispatchCenter::getInstance()->removeDelegate(this);
            btn->setEnabled(true);
            m_pDecManager->reset();
            
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
            
        };
        btn->setEnabled(true);
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
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
}
#pragma mark - initData
void ShareScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/dec/");
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
    m_pEatLayer->setDrinkable(false);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
    if (GameDataManager::getInstance()->m_nPackage==0){
        m_pDecoLayer->setPosition(Vec2(-50, -visibleSize.height));
//        m_pEatLayer->setPosition(Vec2(-50,-visibleSize.height));
    }else{
        m_pDecoLayer->setPosition(Vec2(-50, -visibleSize.height));
//        m_pEatLayer->setPosition(Vec2(-50,-visibleSize.height));
    }
    m_pDecoLayer->runAction(action);
    m_pEatLayer->runAction(action->clone());
    
    m_pTip = Sprite::create("content/common/tte.png");
    this->addChildToUILayer(m_pTip);
    CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
    m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       DelayTime::create(2),
                                       CallFunc::create([=](){
        
        
        AudioHelp::getInstance()->playEffect("enjoy.mp3");
    }), NULL));
    
}

RenderTexture* ShareScene::getResultRender()
{
    RenderTexture* render = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
//    m_pBGLayer上添加了gridNode  截屏有问题
    Sprite* pBg = Sprite::create("content/make/choose/bg_beach2.png");
    pBg->setPosition(visibleSize*0.5);
    
    Sprite* pTable = Sprite::create(localPath("table.png"));
    CMVisibleRect::setPosition(pTable, 320, 180);
    
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
//    m_pBGLayer->visit();
    pBg->visit();
    pTable->visit();
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
    
    Sprite* pNode = (Sprite*)m_pDecoLayer->getChildByName("package");
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
    Dialog* dialog=Dialog::create(Size(497,355), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
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