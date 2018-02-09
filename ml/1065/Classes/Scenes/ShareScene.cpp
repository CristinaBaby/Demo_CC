
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
    m_bFired = false;
    m_bEatLayerShown = false;
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
    int bgIndex = GameDataManager::getInstance()->m_nBgIndex;
    ostringstream ostr;
    if (bgIndex<0) {
        ostr<<"content/make/dec/dec_bk.jpg";
    }else{
        
        ostr<<"content/category/bg/scene"<<bgIndex<<".jpg";
    }
    m_pBg = Sprite::create(ostr.str());
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    
    std::stringstream ostrTable;
    ostrTable<<"content/category/bg/table"<<bgIndex<<".png";
    m_pTable = Sprite::create(ostrTable.str());
    this->addChildToContentLayer(m_pTable);
    CMVisibleRect::setPosition(m_pTable, 480, 20);
    
    _showLightCandleScene();
//    _showEatScene();
    
    std::stringstream ostrKid;
    ostrKid<<"content/common/make/child0/";
    Sprite* pKid = Sprite::create(ostrKid.str()+"1.png");
    pKid->setTag(0);
    CMVisibleRect::setPosition(pKid, gKidPos[0]);
    this->addChildToContentLayer(pKid);
    pKid->setLocalZOrder(-1);
    
    Sprite* pEye = Sprite::create(ostrKid.str()+"wink1.png");
    pKid->addChild(pEye);
    pEye->setName("eye");
    pEye->setPosition(pKid->getContentSize()*0.5);
    pEye->setOpacity(0);
    pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                           DelayTime::create(0.3),
                                                           FadeOut::create(0.02),
                                                           DelayTime::create(3),
                                                           CallFunc::create([=]()
                                                                            {
                                                                                pEye->setTexture("content/common/make/child0/wink2.png");
                                                                            }),
                                                           FadeIn::create(0.02),
                                                           DelayTime::create(0.8),
                                                           FadeOut::create(0.02),
                                                           CallFunc::create([=]()
                                                                            {
                                                                                pEye->setTexture("content/common/make/child0/wink1.png");
                                                                            }),
                                                           DelayTime::create(3),
                                                           NULL)));

    
    for (int i = 3; i>0; i--) {
        std::stringstream ostr;
        ostr<<"content/common/make/child"<<i<<"/";
        Sprite* pKid = Sprite::create(ostr.str()+"2.png");
        pKid->setTag(i);
        CMVisibleRect::setPosition(pKid, gKidPos[i]);
        this->addChildToContentLayer(pKid);
        pKid->setLocalZOrder(-1);
        
        pKid->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate(ostr.str(), 2, 3,false,true,0.5),
                                                               DelayTime::create(0.5), NULL)));
        
        Sprite* pEye = Sprite::create(ostr.str()+"wink.png");
        pKid->addChild(pEye);
        pEye->setName("eye");
        pEye->setPosition(pKid->getContentSize()*0.5);
        pEye->setOpacity(0);
        pEye->runAction(Sequence::create(DelayTime::create(0.1+0.1*i),
                                         CallFunc::create([=]()
                                                          {
                                                              pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                                                                                     DelayTime::create(0.3),
                                                                                                                     FadeOut::create(0.02),
                                                                                                                     DelayTime::create(3+0.1*i), NULL)));
                                                          }), NULL));
      
        
    }
    

    
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    m_pGameUI->showShareUILayout(false);
    
    return true;
}

void ShareScene::onEnter()
{
    ExtensionScene::onEnter();
//    decorate界面有放大缩下功能  避免截屏到放大缩小的ui被截图下来
    
}

void ShareScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagFav==tag) {

        
    }else if (GameUILayoutLayer::eUIButtonTagPhoto==tag){
        Image* image = getResultRender()->newImage();
        image->autorelease();
        STSystemFunction st;
        btn->setEnabled(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]()
                                                          {
                                                              btn->setEnabled(true);
                                                          }), NULL));
        st.saveToAlbum(image, [=](bool ok){
            btn->setEnabled(true);
            if (ok) {
                Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                dialog->setContentText("Your image has been saved to your Camera Roll!");
                Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
                dialog->setPosition(Director::getInstance()->getVisibleOrigin());
            }else{
                Dialog* dialog=Dialog::create(Size(425*1.1,290*1.1), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
                dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
                Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
                dialog->setPosition(Director::getInstance()->getVisibleOrigin());
            }
        },"Cake");
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
        
        st.sendEmailAndFilePic("My Birthday party's coming! let's make a perfect cake for the crazy party!",
                               "<p>Check out the Cake I just made!  Download the app from the link below and see if you can make a better one!</p>  <p><a href=‘http://itunes.apple.com/app/id1105265720’>http://itunes.apple.com/app/id1105265720</a></p>",
                               (FileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
        Image* pImage = getResultRender()->newImage();
        pImage->saveToFile(FileUtility::getStoragePath()+"/shotscreen.png", false);
        pImage->autorelease();
        st.sendEmailAndFilePic("My Birthday party's coming! let's make a perfect cake for the crazy party!",
                               "Check out the Cake I just made!  Download the app from the link below and see if you can make a better one!",
                               (FileUtility::getStoragePath()+"/shotscreen.png").c_str());
#endif
    }else if (GameUILayoutLayer::eUIButtonTagEatAgain==tag){
        btn->setVisible(false);
        btn->setEnabled(true);
        m_pDecoLayer->removeAllChildren();
        m_pDecManager->showFinishedFood(this->m_pDecoLayer);
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        if (m_bEatLayerShown) {
            m_pDecManager->reset();
        }
        
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            KeypadDispatchCenter::getInstance()->removeDelegate(this);
            btn->setEnabled(true);
            if (m_bEatLayerShown) {
                m_pDecManager->reset();
            }
            

            if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
                AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
                AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
                    SceneManager::popToRootScene();
                    SceneManager::replaceTheScene<HomeScene>();
                };
            }else{
                SceneManager::popToRootScene();
                SceneManager::replaceTheScene<HomeScene>();
            }
            
        };
        btn->setEnabled(true);
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
    }else if (GameUILayoutLayer::eUIButtonTagBg==tag){btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to change the room?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            btn->setEnabled(true);
            
            ChooseBackgroudLayer* pLayer = ChooseBackgroudLayer::create();
            pLayer->onItemCallback = CC_CALLBACK_1(ShareScene::onBgCallback, this);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
        };
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };

        
    }
}
#pragma mark - initData
void ShareScene::_initData()
{
    m_nPackage = GameDataManager::getInstance()->m_nPackage;
    setExPath("content/"+gPackageName[m_nPackage]+"/");
    m_bDark = false;
}

void ShareScene::_showLightCandleScene()
{
    for (int i = 0; i<3; i++) {
        std::stringstream ostr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ostr<<"dec"<<i<<".png";
#else
        ostr<<"/dec"<<i<<".png";
#endif
        Sprite* pDec = Sprite::create(FileUtility::getStoragePath()+ostr.str());
        if (pDec) {
            this->addChildToContentLayer(pDec);
            pDec->setPosition(visibleSize.width/2,visibleSize.height/2);
            if (i==0) {
                pDec->setLocalZOrder(-1);
            }
        }
    }
    
    m_pCandleLayer = Layer::create();
    this->addChildToContentLayer(m_pCandleLayer);
    m_pCandleLayer->setLocalZOrder(3);
    m_pCandleLayer->setScale(0.6);
    m_pCandleLayer->setPosition(0, -visibleSize.height*0.1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"decorate.png");
#else
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"/decorate.png");
#endif
    m_pCandleLayer->addChild(m_pDecoratedCake);
    m_pDecoratedCake->setPosition(visibleSize.width/2,visibleSize.height*0.5);
    
    float delt = 100;
    std::vector<Vec3> candleVector = GameDataManager::getInstance()->m_VectorCandle;
    CandlePosition candlePos;
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320+delt, 260+20));
    candlePos.init(5, 5, 65, 48, pos);
    for_each(candleVector.begin(), candleVector.end(), [=,&candlePos](Vec3 vec){
        int index = vec.x;
        int type = vec.y;
        int tag = vec.z;
        Vec2 worldPos = candlePos.getPosition(index);
        Vec2 posCandle = m_pContentLayer->convertToNodeSpace(worldPos);
        CandleNode* pCandle = CandleNode::create();
        pCandle->initCandle(type, tag);
        pCandle->setFirePosition(CandleData::getCandleData(type).firePos);
        if (type==3) {
            if(tag==6){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+18,CandleData::getCandleData(type).firePos.y-11));
            }else if(tag==7){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+21,CandleData::getCandleData(type).firePos.y));
            }
        }
        pCandle->setPosition(Vec2(posCandle.x, posCandle.y-CandleData::getCandleData(type).holderHeight));
        m_pCandleLayer->addChild(pCandle);
        pCandle->setLocalZOrder(50-index);
        pCandle->setTag(0);
        m_VectorCandle.pushBack(pCandle);
    });
    m_pTool = _createDrageNode("content/make/dec/igniter.png");
    this->addChildToContentLayer(m_pTool);
    m_pTool->setLocalZOrder(50);
    CMVisibleRect::setPosition(m_pTool, 480+visibleSize.width, 320);
    m_pTool->setActionPoint(Vec2(8, 160));
    m_pTool->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    m_pTool->setTouchEnabled(false);
    m_pTool->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=]()
                                                         {
                                                             m_pTool->setTouchEnabled(true);
                                                         }), NULL));
    
    m_pCandleLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                               Spawn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height*0.1)),
                                                             ScaleTo::create(0.5, 1), NULL), NULL));

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
//    m_pDecoLayer->setScale(0.7);
//    m_pDecoLayer->setPosition(Vec2(0, -100));
    
    m_bEatLayerShown = true;
    m_pTip = Sprite::create("content/common/tte.png");
    
    this->addChildToUILayer(m_pTip);
    CMVisibleRect::setPosition(m_pTip, 480, 100,kBorderNone,kBorderTop);
    
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
    Dialog* dialog=Dialog::create(Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
    dialog->setContentText("Do you want to start over with a new food?");
    Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
    dialog->setPosition(Director::getInstance()->getVisibleOrigin());
    dialog->onPositiveClick = [=](void*){
        if (m_bEatLayerShown) {
            m_pDecManager->reset();
        }
        m_bBack = true;
        
        
        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
            AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
                SceneManager::popToRootScene();
                SceneManager::replaceTheScene<HomeScene>();
            };
        }else{
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
        }
        
    };
}



void ShareScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pTool) {
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fire.plist");
        m_pTool->addChild(pParticle);
        pParticle->setName("fire");
        pParticle->setPosition(m_pTool->convertToNodeSpace(worldPoint));
    }
}
void ShareScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pTool==pDragNode) {
        for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](CandleNode* pCandle){
            if (pCandle->getTag()==0) {
                Vec2 pos = pCandle->getFireWorldPos();
                Rect rect = Rect(pos.x-10, pos.y-10, 20, 20);
                if (rect.containsPoint(worldPoint)) {
                    stringstream ostr;
                    int candleType = pCandle->getCandleType();
                    int candleIndex = pCandle->getCandleIndex();
                    if (candleType==0) {
                        ostr<<"particle/firework"<<candleIndex<<".plist";
                    }else{
                        ostr<<"particle/fire.plist";
                    }
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create(ostr.str());
                    pCandle->addFire(pParticle);
                    pCandle->setTag(1);
                    m_bFired = true;
                }
            }
        });
        
    }
}
void ShareScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (m_pTool==pDragNode) {
        ParticleSystemQuad* pParticle = (ParticleSystemQuad*)pDragNode->getChildByName("fire");
        if(pParticle) {
            pParticle->removeFromParent();
        }
        if (m_bFired) {
            _lightCandle();
            m_pTool->setTouchEnabled(false);
            m_pTool->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 100)));
            Sprite* pDark = nullptr;
            if (!m_bDark) {
                m_bDark = true;
                pDark = Sprite::create("content/make/dec/black.png");
                this->addChildToContentLayer(pDark);
                pDark->setLocalZOrder(2);
                pDark->setPosition(visibleSize*0.5);
                pDark->setOpacity(0);
                pDark->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(FadeTo::create(1, 255)),
                                                                        EaseSineIn::create(FadeTo::create(1, 220)), NULL)));
            }
            this->runAction(Sequence::create(DelayTime::create(10),
                                             CallFunc::create(CC_CALLBACK_0(ShareScene::_dieoutCandle, this)),
                                             CallFunc::create([=]()
                                                              {
                                                                  if (pDark) {
                                                                      pDark->stopAllActions();
                                                                      pDark->runAction(Sequence::create(FadeOut::create(1),
                                                                                                        CallFunc::create([=](){
//                                                                          m_pGameUI->showNextLayout();
                                                                      }), NULL));
                                                                  }
                                                              }), NULL));
        }
    }
}
void ShareScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode, worldPoint);
}

void ShareScene::onBgCallback(int index)
{
    std::stringstream ostr;
    ostr<<"content/category/bg/scene"<<index<<".jpg";
    m_pBg->setTexture(ostr.str());
    
    std::stringstream ostrTable;
    ostrTable<<"content/category/bg/table"<<index<<".png";
    if (!m_pTable) {
        m_pTable = Sprite::create(ostrTable.str());
        this->addChildToContentLayer(m_pTable);
        CMVisibleRect::setPosition(m_pTable, 480, 20);
    }else{
        m_pTable->setTexture(ostrTable.str());
    }
    CMVisibleRect::setPosition(m_pTable, 480, 20);
    GameDataManager::getInstance()->m_nBgIndex = index;

    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
}
void ShareScene::_lightCandle()
{
    for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](CandleNode* pCandle){
        if (pCandle->getTag()==0) {
            stringstream ostr;
            int candleType = pCandle->getCandleType();
            int candleIndex = pCandle->getCandleIndex();
            if (candleType==0) {
                ostr<<"particle/firework"<<candleIndex<<".plist";
            }else{
                ostr<<"particle/fire.plist";
            }
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create(ostr.str());
            pCandle->addFire(pParticle);
            pCandle->setTag(1);
        }
    });
    AudioHelp::getInstance()->playBackGroundMusic("birthday.mp3");
    m_pCandleLayer->runAction(Spawn::create(ScaleTo::create(0.5, 0.7),
                                            MoveBy::create(0.5, Vec2(0, -90)), NULL));
}

void ShareScene::_dieoutCandle()
{
    int count = 0;
    for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=,&count](CandleNode* pCandle){
        pCandle->removeFire();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
        pCandle->addFire(pParticle);
        
        count++;
        pCandle->runAction(Sequence::create(DelayTime::create(2.0+0.1*count),
                                            CallFunc::create([=]()
                                                             {
                                                                 AudioHelp::getInstance()->playEffect("candlefly.mp3");
                                                             }),
                                            EaseSineIn::create(MoveBy::create(0.5, Vec2(100, visibleSize.height))), NULL));
    });
    this->runAction(Sequence::create(DelayTime::create(2.5+0.1*count),
                                     CallFunc::create([=]()
                                                      {
                                                          AudioHelp::getInstance()->playBackGroundMusic("bg_theme.mp3");
                                                          m_pCandleLayer->removeAllChildren();
                                                          this->_showGift();
                                                          this->_showEatScene();
                                                      }), NULL));
}

void ShareScene::_showGift()
{
    
    DecorateConfigData decorationData = ConfigManager::getInstance()->getDecorateFile("boxgift");
    int random = arc4random()%decorationData.totalCount;
    std::stringstream ostr;
    ostr<<"content/category/bg/boxgift"<<"/"<<random<<".png";
    Sprite* m_pGift = Sprite::create(ostr.str());
    this->addChildToContentLayer(m_pGift);
    m_pGift->setLocalZOrder(10);
    m_pGift->setOpacity(0);
    
    Sprite* pBox = Sprite::create("content/category/bg/boxgift/box0.png");
    this->addChildToContentLayer(pBox);
    CMVisibleRect::setPositionAdapted(pBox, 200-visibleSize.width/2, 150);
    pBox->setLocalZOrder(5);

    AudioHelp::getInstance()->playEffect("happy.mp3");
    pBox->runAction(Sequence::create(JumpBy::create(1, Vec2(visibleSize.width/2, 0), 100, 1),
                                     ScaleTo::create(0.5, 1,0.8),
                                     ScaleTo::create(0.1, 0.9,1.1),
                                     CallFunc::create([=]()
                                                      {
                                                          pBox->setScale(1);
                                                          pBox->setTexture("content/category/bg/boxgift/box1_0.png");
                                                          Sprite* pBoxFront = Sprite::create("content/category/bg/boxgift/box_1.png");
                                                          pBoxFront->setPosition(pBox->getPosition());
                                                          pBoxFront->setLocalZOrder(20);
                                                          this->addChildToContentLayer(pBoxFront);
                                                          
                                                          Sprite* pBoxCover = Sprite::create("content/category/bg/boxgift/box2.png");
                                                          pBoxCover->setPosition(Vec2(pBox->getPosition().x,pBox->getPosition().y+200));
                                                          pBoxCover->setLocalZOrder(20);
                                                          this->addChildToContentLayer(pBoxCover);
                                                          
                                                          m_pGift->setPosition(pBox->getPosition());
                                                          m_pGift->setOpacity(255);
                                                          m_pGift->setScale(0.5);
                                                          m_pGift->runAction(Spawn::create(JumpBy::create(2, Vec2(0, -50), 220, 1),
                                                                                           ScaleTo::create(1.5, 1), NULL));
                                                          
                                                          pBoxCover->runAction(MoveBy::create(0.5, Vec2(-100, visibleSize.height)));
                                                          
                                                          pBox->runAction(Sequence::create(DelayTime::create(1),
                                                                                           MoveBy::create(1, Vec2(-visibleSize.width, -100)), NULL));
                                                          pBoxFront->runAction(Sequence::create(DelayTime::create(1),
                                                                                                MoveBy::create(1, Vec2(-visibleSize.width, -100)), NULL));
                                                          ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugarjump.plist");
                                                          pParticle->setPosition(pBox->getPosition());
                                                          pParticle->setLocalZOrder(10);
                                                          this->addChildToContentLayer(pParticle);
                                                      }), NULL));
}