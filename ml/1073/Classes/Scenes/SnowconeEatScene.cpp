
#include "SnowconeEatScene.h"
#include "SceneManager.h"

SnowconeEatScene::SnowconeEatScene()
{
    m_nRequestType = -1;
    m_nEatCount = 0;
}

SnowconeEatScene::~SnowconeEatScene()
{
    
}
bool SnowconeEatScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/Snowcones_bg5.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
//    Sprite* pTable = Sprite::create(localPath("Snowcones_bg4.png"));
//    CMVisibleRect::setPositionAdapted(pTable, 320, 320);
//    this->addChildToBGLayer(pTable);
    
    Sprite* pSnowcones = Sprite::create(FileUtils::getInstance()->getWritablePath()+"Snowcones.png");
    if (pSnowcones) {
//        m_pFood->addChild(pSnowcones);
    }
    
    AudioHelp::getInstance()->playEffect("vo_Snowcone_eat.mp3");
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_Snowcone_eat1.mp3");
    }), NULL));
    
    _showSnowcones();
    _showSpoon();
    
    m_pGameUI->showShareUILayout(false);
//    m_pGameUI->showBackLayout();
//    m_pGameUI->showNextLayout2();
    m_pGameUI->showSSCMore();
    m_pGameUI->showMenuUILayout();
    return true;
}

void SnowconeEatScene::onEnter()
{
    ExtensionScene::onEnter();
//    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_EAT);
}

void SnowconeEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void SnowconeEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void SnowconeEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="spoon"){
        Rect rect = m_pPlate->getBoundingBox();
        rect.origin = m_pPlate->getParent()->convertToWorldSpace(rect.origin);
        int count = 3;
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlate->getPosition()+Vec2(100-100*(m_nEatCount/count), 80-70*(m_nEatCount%count)));
            
            pDragNode->setLocalZOrder(1);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 20),
                                                  RotateBy::create(0.5, -20),
                                                  CallFunc::create([=](){
                pDragNode->setLocalZOrder(20);
                m_pSnowcones->doEat(pDragNode->getWorldSpaceActionPoint());
                
                Sprite* pSnowcones = Sprite::create(FileUtils::getInstance()->getWritablePath()+"Snowcones_dec.png");
                Sprite* pMask = Sprite::create("content/eatShape2.png");
                Size size = pMask->getContentSize();
                if(pSnowcones){
                    Size size2 = pMask->getContentSize();
                    pSnowcones->setPosition(Vec2(size.width*0.5-size2.width/count-30+110*(m_nEatCount/count), size.height*0.5-size2.height/count-20+70*(m_nEatCount%count)));
                }
                
                RenderTexture* pRT = RenderTexture::create(size.width, size.height,Texture2D::PixelFormat::RGBA8888,
                                                           GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
                if(pSnowcones){
                    pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
                    pMask->setPosition(size*0.5);
                    pRT->begin();
                    pSnowcones->visit();
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
                //            pSpoonIn->setScale(0.7);
                pSpoonIn->setPosition(m_pSpoon->convertToNodeSpace(m_pSpoon->getWorldSpaceActionPoint()));
                
                m_nEatCount++;
                pDragNode->runAction(Sequence::create(DelayTime::create(1),
                                                      MoveTo::create(0.5, Vec2(visibleSize.width/2,visibleSize.height*0.2)),
                                                      CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
                    this->addChildToUILayer(pParticle);
                    pParticle->setPosition(pDragNode->getWorldSpaceActionPoint());
                    
                    Node* pNode = pDragNode->getChildByName("in");
                    if (pNode) {
                        pNode->removeFromParent();
                    }
                    
                    if(m_nEatCount<count*count){
                        pDragNode->setTouchEnabled(true);
                    }
                    if (m_nEatCount>=count*count && !m_pGameUI->eatAgainIsShow()) {
                        m_pGameUI->showEatAgainLayout();
                        m_pGameUI->showNextLayout2();
                        
                        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
                        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                        this->addChildToUILayer(pParticle);
                        pParticle->setLocalZOrder(10);
                        pParticle->setPosition(visibleSize*0.5);
                    }
                }), NULL));
            }), NULL));
            
        }
        
    }
}

void SnowconeEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void SnowconeEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void SnowconeEatScene::onButtonCallback(Button* btn)
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
        m_nRequestType = 1;
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
            RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode,  PERMISSION::kWriteExternalStorage);
        }
        
    }else if (GameUILayoutLayer::eUIButtonTagEmail==tag){
        btn->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        
        
        m_nRequestType = 2;
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
        GameDataManager::getInstance()->m_bShowAds = true;
        
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
            
            AudioHelp::getInstance()->resetEffect();
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
            
        };
        btn->setEnabled(true);
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
void SnowconeEatScene::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            if(m_nRequestType==2){
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _sendEmail();
                }), NULL));
            }else if(m_nRequestType==1){
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

#pragma mark - initData
void SnowconeEatScene::_initData()
{
    setExPath("content/make/Snowcones/1/");
}

void SnowconeEatScene::_showSnowcones()
{
    m_pPlate = Sprite::create(localPath("pan2_1.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 320+60);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    
    m_pPanFront = Sprite::create(localPath("pan2_2.png"));
    this->addChildToContentLayer(m_pPanFront);
    CMVisibleRect::setPositionAdapted(m_pPanFront, 320-visibleSize.width, 320+60);
    m_pPanFront->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pPanFront->setLocalZOrder(8);
    
    
    Sprite* pSnowcones = Sprite::create(FileUtils::getInstance()->getWritablePath()+"Snowcones_dec.png");
    Size size = m_pPlate->getContentSize();
    if (pSnowcones) {
        size = pSnowcones->getContentSize();
    }
    m_pSnowcones = MakeEatLayer::create();
    
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    if (pSnowcones) {
        pSnowcones->setPosition(Vec2(size.width/2, size.height/2));
        pSnowcones->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    m_pSnowcones->setRenderTexture(rt);
    m_pSnowcones->setPosition(Vec2(-size.width/2, -size.height/2));
    m_pSnowcones->setShapeTexture(Sprite::create("content/eatShape2.png")->getTexture());
    this->addChildToContentLayer(m_pSnowcones);
    CMVisibleRect::setPositionAdapted(m_pSnowcones, 320-size.width/2-visibleSize.width, 350-size.height/2+60);
    m_pSnowcones->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pSnowcones->setLocalZOrder(5);
}

void SnowconeEatScene::_showSpoon()
{
    m_pSpoon = _createDrageNode(localPath("spoon_eat.png"));
    this->addChildToContentLayer(m_pSpoon);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 350+visibleSize.width, 400);
    
    m_pSpoon->setName("spoon");
    m_pSpoon->setActionPoint(Vec2(70, 38));
    m_pSpoon->getDragSprite()->setAnchorPoint(Vec2(70.0/m_pSpoon->getContentSize().width, 38.0/m_pSpoon->getContentSize().width));
    m_pSpoon->setTouchEnabled(false);
    m_pSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pSpoon->setOrgPositionDefault();
        m_pSpoon->setTouchEnabled(true);
    }), NULL));
    m_pSpoon->setLocalZOrder(20);
}

void SnowconeEatScene::_reset()
{
    m_nEatCount = 0;
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pPlate->removeFromParent();
    }), NULL));
    m_pPanFront->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pPanFront->removeFromParent();
    }), NULL));
    m_pSnowcones->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSnowcones->removeFromParent();
    }), NULL));
    m_pSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pSpoon->removeFromParent();
    }), NULL));
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showSnowcones();
        _showSpoon();
    }), NULL));
}

void SnowconeEatScene::_showEatScene()
{
    m_pDecoLayer = Layer::create();
    this->addChildToContentLayer(m_pDecoLayer);
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(SnowconeEatScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(SnowconeEatScene::onEatEndCallback, this);
    m_pEatLayer->setDrinkable(true);
    Rect rect = Rect(310, 710-100, 190, 160);
    rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
    m_pEatLayer->setDrinkRect(rect);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   MoveBy::create(1, Vec2(0, visibleSize.height)), NULL);
    if (GameDataManager::getInstance()->m_nPackage==0){
        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
        //        m_pEatLayer->setPosition(Vec2(-50,-visibleSize.height));
    }else{
        m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
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
        AudioHelp::getInstance()->playEffect("vo_tap_enjoy.mp3");
    }), NULL));
    
}

RenderTexture* SnowconeEatScene::getResultRender()
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
void SnowconeEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
    
}

void SnowconeEatScene::onEatEndCallback(Vec2 location)
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
    
    
    
    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(location);
//    m_pDecManager->doEat(location);
//    m_pDecManager->stopDrink();
    
    if (//m_pDecManager->finishedEat() &&
        !m_pGameUI->eatAgainIsShow()) {
        m_pGameUI->showEatAgainLayout();
        m_pGameUI->showNextLayout2();
        
        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }
}

void SnowconeEatScene::onKeyBackClicked()
{
    Node* pDialog = Director::getInstance()->getRunningScene()->getChildByTag(9999);
    if (pDialog) {
        pDialog->removeFromParent();
        return;
    }
    if (m_bBack) {
        return;
    }
    log("========SnowconeEatScene::onKeyBackClicked");
    //    AudioHelp::getInstance()->playPreviousEffect();
    //    SceneManager::popScene();
    Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog->setContentText("Do you want to start over with a new food?");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
//        m_pDecManager->reset();
        m_bBack = true;
        SceneManager::popToRootScene();
        SceneManager::replaceTheScene<HomeScene>();
        
    };
}

void SnowconeEatScene::onBgCallback(int index)
{
    m_pBg->setTexture(localPath("bg_winter.jpg"));
    
    GameDataManager::getInstance()->m_nBgIndex = index;
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
}

void SnowconeEatScene::_savePhoto()
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
void SnowconeEatScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Snowcones!",
                                                              "<p>Look at the Snowcones I just made.Isn't it so lovely? There are still tons of tools and decorations in Snowcones you'll want to try!Stop hesitating! Download now!You are gonna love it!</p>  <p><a href=‘http://itunes.apple.com/app/id1188565450’>http://itunes.apple.com/app/id1188565450</a></p>",
                                                              (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Snowcones!",
                                                              "Look at the Snowcones I just made.Isn't it so lovely? There are still tons of tools and decorations in Snowcones you'll want to try!Stop hesitating! Download now!You are gonna love it!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}