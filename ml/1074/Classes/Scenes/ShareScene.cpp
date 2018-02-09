
#include "ShareScene.h"
#include "SceneManager.h"
#include "CandlePosition.h"
#include "CandleNode.h"

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
    AudioHelp::getInstance()->playBackGroundMusic("music.mp3");
    
    //    bg
    m_nBgIndex = 1;
    m_pBg = Sprite::create(localPath("bg.jpg"));
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
    
    if(GameDataManager::getInstance()->m_nPackage == 1){
        std::vector<Vec3> vecs = GameDataManager::getInstance()->m_VectorCandle;
        if(vecs.size()>0){
            {
                m_nRequestTag = eRequesetTagAudio;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                MICAudioManager::getInstance()->onRequestPermisionCallback = [=](bool granted){
                    this->runAction(Sequence::create(DelayTime::create(0.01),
                                                     CallFunc::create([=](){
                        this->runAction(Sequence::create(DelayTime::create(1),
                                                        CallFunc::create([=](){
                            if (granted) {
                                _audioRecord();
                            }else{
                                _dieoutCandle();
                                m_bCanEat = true;
                            }
                        }), NULL));
                    }), NULL));
                };
                MICAudioManager::getInstance()->requestPermision();
#else
                
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
                RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kRecordAudio);
#endif
            }
        }
    }
    return true;
}

void ShareScene::update(float dt)
{
    float power = MICAudioManager::getInstance()->getPower();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (power>-28 && power<-10) {
        log("========power=====%f",power);
        m_pProgress->addPercent(5);
    }
#else
    if (power>2 && power<4) {
        log("========power=====%f",power);
        m_pProgress->addPercent(5);
    }
#endif
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
    AudioHelp::getInstance()->resumeBackGroundMusic();
    MICAudioManager::getInstance()->onRequestPermisionCallback = nullptr;
    if (isScheduled(schedule_selector(ShareScene::update))) {
        unschedule(schedule_selector(ShareScene::update));
        MICAudioManager::getInstance()->stopRecord();
    }
}
void ShareScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagFav==tag) {
        
        
    }else if (GameUILayoutLayer::eUIButtonTagPhoto==tag){
        btn->setEnabled(false);
        AudioHelp::getInstance()->playCameraEffect();
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        m_nRequestTag = eRequesetTagPhoto;
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
        
        
        m_nRequestTag = eRequesetTagEmail;
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
        if (m_nPackage==2) {
            _showDonut(false);
        }else{
            m_pDecManager->showFinishedFood(this->m_pDecoLayer);
            
            m_pDecManager->m_nEatPercent = 98;
        }
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
//        if(GameDataManager::getInstance()->m_nPackage != 2){
//            m_pDecManager->reset();
//        }
        DecorateManager::getInstance()->reset();
        AudioHelp::getInstance()->resetEffect();
        GameDataManager::getInstance()->m_bShowAds = true;
        ;
        
        RateUsManager::getInstance()->onRateusCallback = [=](int type) {
            SceneManager::popToRootScene();
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
            if(m_nRequestTag == eRequesetTagEmail){
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _sendEmail();
                }), NULL));
            }else if(m_nRequestTag == eRequesetTagPhoto){
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _savePhoto();
                }), NULL));
                
            }else if(m_nRequestTag == eRequesetTagAudio){
                this->runAction(Sequence::create(DelayTime::create(0.01),
                                                 CallFunc::create([=](){
                    _audioRecord();
                }), NULL));
            }
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
            this->runAction(Sequence::create(DelayTime::create(0.01),
                                             CallFunc::create([=](){
                Dialog* dialog=Dialog::create(Size(558*1.2,434*1.2), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                if(m_nRequestTag == eRequesetTagAudio){
                    dialog->setContentText("This app does not have access to your microphone.You can enable access in Privacy Setting.");
                    dialog->onPositiveClick = [=](void*){
                        _dieoutCandle();
                        m_bCanEat = true;
                    };
                }else{
                    dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
                }
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
    if (m_nPackage == 0){
        setExPath("content/make/cupcake/dec/");
    }else if (m_nPackage==1){
        setExPath("content/make/shortcake/dec/");
    }else if (m_nPackage==2){
        setExPath("content/make/donut/dec/");
    }
    
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
    
    if(GameDataManager::getInstance()->m_nPackage == 2){
        _showDonut(false);
    }else{
        m_pDecManager->showFinishedFood(m_pDecoLayer);
    }
    
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
    m_pDecoLayer->setPosition(Vec2(0, -visibleSize.height));
    
    m_pDecoLayer->runAction(action);
    m_pEatLayer->runAction(action->clone());
    
    
    if(GameDataManager::getInstance()->m_nPackage == 1){
//        static std::string candleType[] = {"let","num","sam"};
        std::vector<Vec3> vecs = GameDataManager::getInstance()->m_VectorCandle;
        for_each (vecs.begin(), vecs.end(), [=](Vec3 ret){
            int index = ret.z;
            Vec2 pos = Vec2(ret.x, ret.y);
//            Vec2 worldPos = candlePos.getPosition(index);
//            Vec2 posCandle = m_pContentLayer->convertToNodeSpace(worldPos);
            CandleNode* pCandle = CandleNode::create();
            pCandle->initCandle(index);
            m_pDecoLayer->addChild(pCandle);
            pCandle->setPosition(pos+Vec2(0,-pCandle->getContentSize().height/2));
            int type = pCandle->calculateType(index);
            int num = pCandle->calculateNum(index);
            pCandle->setFirePosition(CandleData::getCandleData(type).firePos);
            if (type==3) {
                if(num==6){
                    pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+18,CandleData::getCandleData(type).firePos.y-11));
                }else if(num==7){
                    pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+21,CandleData::getCandleData(type).firePos.y));
                }
            }
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fire.plist");
            pCandle->addFire(pParticle);
            m_Candles.push_back(pCandle);
        });
        
        if(vecs.size()>0){
            m_bCanEat = false;
        }else{
            m_bCanEat = true;
            m_pTip = Sprite::create("content/common/tte.png");
            this->addChildToUILayer(m_pTip);
            CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
            m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                               MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               DelayTime::create(2),
                                               CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_cant_wait.mp3");
            }), NULL));
        }
    }else{
        m_bCanEat = true;
        m_pTip = Sprite::create("content/common/tte.png");
        this->addChildToUILayer(m_pTip);
        CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
        m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           DelayTime::create(2),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_cant_wait.mp3");
        }), NULL));
    }
    
}
void ShareScene::_dieoutCandle()
{
    AudioHelp::getInstance()->playBackGroundMusic("music.mp3");
    int count = 0;
    for_each(m_Candles.begin(), m_Candles.end(), [=,&count](Node* pNode){
        CandleNode* pCandle  = (CandleNode*) pNode;
        pCandle->removeFire();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
        pCandle->addFire(pParticle);
        
        count++;
        pCandle->runAction(Sequence::create(DelayTime::create(2.0+0.1*count),
                                            CallFunc::create([=]()
                                                             {
                                                                 AudioHelp::getInstance()->playEffect("candlefly.mp3");
                                                             }),
                                            EaseSineIn::create(MoveBy::create(0.5, Vec2(100, visibleSize.height))),
                                            CallFunc::create([=](){
            if (count == m_Candles.size()) {
                m_bCanEat = true;
            }
                                                pCandle->removeFromParent();
                                            }), NULL));
    });
    if (m_Candles.size()==0) {
        m_bCanEat = true;
    }
    if (!m_pTip) {
        m_pTip = Sprite::create("content/common/tte.png");
        this->addChildToUILayer(m_pTip);
        CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
        m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           DelayTime::create(2),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_cant_wait.mp3");
        }), NULL));
    }
}


void ShareScene::_showDonut(bool animate)
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    m_pDecoLayer->addChild(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 450);
    
    {
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"donut.png");
        Sprite* pCake = Sprite::create(FileUtils::getInstance()->getWritablePath()+"donut.png");
        Size size = pCake->getContentSize();
        
        pCake->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
//        rt->setPosition(Vec2(0, 0));
        rt->beginWithClear(0, 0, 0, 0);
        pCake->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        
        m_pEatLayerDown = MakeEatLayer::create();
        m_pEatLayerDown->setRenderTexture(rt);
        
        CMVisibleRect::setPositionAdapted(rt, 320, 500);
//        CMVisibleRect::setPositionAdapted(rt, 320+pCake->getContentSize().width/2, 500+rt->getContentSize().height/2);
//        CMVisibleRect::setPositionAdapted(m_pEatLayerDown, 320-pCake->getContentSize().width/2, 500-pCake->getContentSize().height/2);
//        rt->setPosition(Vec2(-pCake->getContentSize().width/2, -pCake->getContentSize().height/2));
        m_pDecoLayer->addChild(m_pEatLayerDown);
    }
    
    {
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"food_dec.png");
        Sprite* pCake = Sprite::create(FileUtils::getInstance()->getWritablePath()+"food_dec.png");
        Size size = pCake->getContentSize();
        
        pCake->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
//        rt->setPosition(Vec2(0, 0));
        rt->beginWithClear(0, 0, 0, 0);
        pCake->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        
        m_pEatLayerTop = MakeEatLayer::create();
        m_pEatLayerTop->setRenderTexture(rt);
        
//        CMVisibleRect::setPositionAdapted(rt, 320+pCake->getContentSize().width/2, 500+pCake->getContentSize().height/2);
        CMVisibleRect::setPositionAdapted(rt, 320, 500);
//        CMVisibleRect::setPositionAdapted(m_pEatLayerTop, 320-pCake->getContentSize().width/2, 500-pCake->getContentSize().height/2);
//        rt->setPosition(Vec2(-pCake->getContentSize().width/2, -pCake->getContentSize().height/2));
        
        m_pDecoLayer->addChild(m_pEatLayerTop);
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
    if (!m_bCanEat) {
        return;
    }
    if (drinkable) {
        m_pDecManager->doDrink();
        AudioHelp::getInstance()->playDrinkEffect();
    }
}

void ShareScene::onEatMoveCallback(Vec2 location)
{
    if (!m_bCanEat) {
        return;
    }
}

void ShareScene::onEatEndCallback(Vec2 location)
{
    if (!m_bCanEat) {
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
    
    
    AudioHelp::getInstance()->playEffect("basic/bite.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(location);
    if(GameDataManager::getInstance()->m_nPackage == 2){
        m_pEatLayerTop->doEat(location);
        if (m_pEatLayerTop->getPercent()>=82) {
            m_pEatLayerDown->doEat(location);
        }
        if (m_pEatLayerDown->getPercent()>=78 && !m_pGameUI->eatAgainIsShow()) {
            m_pEatLayerDown->doEat(location);
            m_pGameUI->showEatAgainLayout();
            m_pGameUI->showNextLayout2();
            
            AudioHelp::getInstance()->playEffect("cheer_star.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
        }
    }else{
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
        image->autorelease();
    },"Rainbow Cake");
}
void ShareScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Rainbow Cake!",
                                                              "<p>Look at the Rainbow Cake I just made.Isn't it so lovely? There are still tons of tools and decorations in Rainbow Cake you'll want to try!Stop hesitating! Download now!You are gonna love it!</p>  <p><a href=‘http://itunes.apple.com/app/id1222206960’>http://itunes.apple.com/app/id1222206960</a></p>",
                                                              (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Rainbow Cake!",
                                                              "Look at the Rainbow Cake I just made.Isn't it so lovely? There are still tons of tools and decorations in Rainbow Cake you'll want to try!Stop hesitating! Download now!You are gonna love it!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}

void ShareScene::_audioRecord()
{
    bool deviceReady = MICAudioManager::getInstance()->deviceReady();
    if (!deviceReady) {
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            m_pTip->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            _dieoutCandle();
        }), NULL));
    }else{
        
        m_pProgress = ProgressMIC::create();
        this->addChildToUILayer(m_pProgress);
        CMVisibleRect::setPositionAdapted(m_pProgress, 550, 200-400,kBorderRight,kBorderBottom);
        m_pProgress->onFinishCallback = [=](){
            MICAudioManager::getInstance()->stopRecord();
            unschedule(schedule_selector(ShareScene::update));
            
            m_pTip->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            m_pTip = nullptr;
            _dieoutCandle();
            m_pProgress->runAction(MoveBy::create(0.5, Vec2(500, 0)));
            AudioHelp::getInstance()->resumeBackGroundMusic();
        };
        m_pProgress->runAction(Sequence::create(DelayTime::create(0.5),
                                                MoveBy::create(1, Vec2(0, 400)),
                                                CallFunc::create([=](){
            MICAudioManager::getInstance()->startRecord();
            schedule(schedule_selector(ShareScene::update), 0.02);
            AudioHelp::getInstance()->pauseBackGroundMusic();
        }), NULL));
    }
    if (!m_pTip) {
        m_pTip = Sprite::create("content/common/lbo.png");
        this->addChildToUILayer(m_pTip);
        CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
        m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           DelayTime::create(2), NULL));
    }
    
}