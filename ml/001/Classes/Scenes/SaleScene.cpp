
#include "SaleScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"
#include "SaleManager.h"

SaleScene::SaleScene()
{
    m_pDonut = nullptr;
    m_pPlate = nullptr;
    m_bReadySale = false;
    m_bBack = false;
    m_bRequestEmail = false;
    m_nSaleCount = 0;
    m_bButtonEnable = true;
}

SaleScene::~SaleScene()
{
    
}
bool SaleScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg_eat.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_eat_table.png"));
    CMVisibleRect::setPosition(pTable, 320, 320-100);
    this->addChildToBGLayer(pTable);
    pTable->setLocalZOrder(1);
    
    _showDonut();
    
    this->runAction(Sequence::create(DelayTime::create(0.2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_serve_page.mp3");
    }),
                                     DelayTime::create(0.8),
                                     CallFunc::create([=](){
        _showGuest();
    }),  NULL));
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showCointLayout();
    
    m_pGameUI->showBackLayout(true,false);
    m_pGameUI->showNextLayout();
    m_pGameUI->showServeUILayout();
//    m_pGameUI->showSSCMore();
    
    AudioHelp::getInstance()->playBackGroundMusic("happy.mp3");
    return true;
}

void SaleScene::onEnter()
{
    ExtensionScene::onEnter();
    
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_SALE);
}

void SaleScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    
}

void SaleScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
}

void SaleScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void SaleScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    int tag = pDragNode->getTag();
    if (!m_bReadySale || m_nSaleCount>=3) {
        pDragNode->back(tag);
    }else{
        RoleModel* pGuest = SaleManager::getInstance()->getRole();
        Rect rect = Rect(pGuest->getPositionX()-400, pGuest->getPositionY()-200, 800, 800);
        rect.origin = pGuest->getParent()->convertToWorldSpace(rect.origin);
//        rect = Rect(490,350,300,350);
        
        rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            int tag  = pDragNode->getTag();
            
                _pickDrink();
                pDragNode->setVisible(false);
                pDragNode->setTouchEnabled(false);
                pDragNode->back(tag);
            
        }else{
            pDragNode->back(tag);
        }
    }

}

void SaleScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void SaleScene::onButtonCallback(Button* btn)
{
    if (!m_bButtonEnable) {
        return;
    }
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
            RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kCamera | PERMISSION::kWriteExternalStorage);
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
        
        _showDonut();
        
        _showGuest();
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        DecorateManager::getInstance()->reset();
        GameDataManager::getInstance()->m_bShowAds = true;
        
        AudioHelp::getInstance()->playEffect("vo_best_party.mp3");
        
//        AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//        AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//            SceneManager::popToRootScene();
//            SceneManager::replaceTheScene<ChoosePackageScene>();
//        };
        RateUsManager::getInstance()->showRateUs();
        RateUsManager::getInstance()->onRateusCallback = [=](int type) {
            AdsManager::getInstance()->showAds(kTypeInterstitialAds);
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<ChoosePackageScene>();
        };
        
        return;
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new Donut?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            KeypadDispatchCenter::getInstance()->removeDelegate(this);
            btn->setEnabled(true);
            DecorateManager::getInstance()->reset();
            
            AudioHelp::getInstance()->resetEffect();
//            AdLoadingLayerBase::showLoading<AdsLoadingScene>(true);
//            AdLoadingLayerBase::s_currentInstance->loadingDoneCallback = [=](){
//                SceneManager::popToRootScene();
//                SceneManager::replaceTheScene<HomeScene>();
//            };
            AdsManager::getInstance()->showAds(kTypeInterstitialAds);
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
void SaleScene::onPermissionGrantedResult(int requestCode,bool bGranted){
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

void SaleScene::onKeyBackClicked()
{
    Node* pDialog = Director::getInstance()->getRunningScene()->getChildByTag(9999);
    if (pDialog) {
        pDialog->removeFromParent();
        return;
    }
    if (m_bBack) {
        return;
    }
    SceneManager::popScene();
}

RenderTexture* SaleScene::getResultRender()
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
#pragma mark - initData
void SaleScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagChooseIceShape;
}

void SaleScene::_savePhoto()
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
    },"Donut");
}
void SaleScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Donut Maker!",
                                                              "<p>We are having donut party right now. Hurry to join us. There are so many tasty donuts here, You can make what you like. Come on! Let's Party!</p>  <p><a href=‘http://itunes.apple.com/app/id578210847’>http://itunes.apple.com/app/id578210847</a></p>",
                                                              (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Donut Maker!",
                                                              "We are having donut party right now. Hurry to join us. There are so many tasty donuts here, You can make what you like. Come on! Let's Party!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}

void SaleScene::_showGuest()
{
    RoleModel* pGuest = SaleManager::getInstance()->produceGuest();
    this->addChildToBGLayer(pGuest);
    pGuest->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "eat") ){
                _giveCoint();
                
                float scaleX = pGuest->getScaleX();
                int flag = scaleX>0?1:-1;
                SaleManager::getInstance()->guestBack();
                pGuest->runAction(Sequence::create(DelayTime::create(1),
                                                   MoveBy::create(3.5, Vec2(-flag*visibleSize.width, 0)),
                                                   CallFunc::create([=](){
                    pGuest->removeFromParent();
                }),
                                                   NULL));
            }
        }else if (MovementEventType::START==movementType) {
            if (0==std::strcmp(movementID.c_str(), "eat") ){
            }
        }
    };
    
    int radom2 = arc4random()%10*0.1;
    pGuest->setScale(0.4+radom2);
    
    int radom = arc4random()%100;
    int flag = 1;
    if (radom>50) {
        flag = -1;
    }
    CMVisibleRect::setPosition(pGuest, 320+flag*visibleSize.width, 550);
    SaleManager::getInstance()->guestCome();

    pGuest->setScaleX(flag*pGuest->getScaleX());
    pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(-flag*visibleSize.width, 0)),
                                       CallFunc::create([=](){
        SaleManager::getInstance()->guestStandby();
        
        if (m_nSaleCount==3) {
            AudioHelp::getInstance()->playEffect("vo_another_one.mp3");
            SaleManager::getInstance()->showDialog(_getTipDialog());
            
            RoleModel* pGirl = RoleModel::create();
            pGirl->setRole(localCocosPath("cooker0/"), "cooker0", 4);
            this->addChildToContentLayer(pGirl);
            pGirl->setScale(0.6);
            CMVisibleRect::setPositionAdapted(pGirl, 50, 300-visibleSize.height);
            pGirl->playArmation("Animation2", true);
            pGirl->runAction(Sequence::create(DelayTime::create(1),
                                              MoveBy::create(1, Vec2(100, visibleSize.height)),
                                              CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_make_another.mp3");
            }), NULL));
        }else if(m_nSaleCount==0){
            m_pGuideLayer->showGuideMove(m_pDonut->getPosition(), pGuest->getPosition()+Vec2(0,100));
        }
    }),
                                       DelayTime::create(0.5),
                                       CallFunc::create([=](){
        m_bReadySale = true;
    }),
                                       NULL));
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
        
        if (m_nSaleCount<3){
            int index = SaleManager::getInstance()->getRoleIndex();
            if (index==0) {
                AudioHelp::getInstance()->playEffect("vo_guest_boy1.mp3");
            }else if (index==1) {
                AudioHelp::getInstance()->playEffect("vo_guest_boy2.mp3");
            }else if (index==2) {
                AudioHelp::getInstance()->playEffect("vo_guest_girl.mp3");
            }
        }
    }), NULL));
}

void SaleScene::_pickDrink()
{
    this->stopActionByTag(100);
    
    m_bReadySale = false;
    std::string path = FileUtils::getInstance()->getWritablePath()+GameDataManager::getInstance()->getBoxName();
    
    Sprite* Sprite = Sprite::create(path);
    
    Size donutSize = Sprite->getContentSize();
    RenderTexture* render = RenderTexture::create(donutSize.width, donutSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Sprite->setPosition(donutSize*0.5);
    render->begin();
    Sprite->visit();
    render->end();
    
    MakeEatLayer* pLayer = MakeEatLayer::create();
    pLayer->setContentSize(donutSize);
    pLayer->setRenderTexture(render);
    
//    pLayer->setPosition(Vec2(0,-donutSize.height*0.5));
//    Skin* pSkin = Skin::create();
//    pSkin->addChild(pLayer);
//    SaleManager::getInstance()->guestGetPizza(Sprite::create("content/dec/twist/donut2_1.png"),1,0);
    
    if (GameDataManager::getInstance()->m_nPackage==0) {
        SaleManager::getInstance()->guestGetPizza(pLayer,1,0);
        pLayer->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(-100, 90),false);
            pLayer->doEat(Vec2(0, 90),false);
            pLayer->doEat(Vec2(100, 90),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),
                                           DelayTime::create(0.9),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(-100, 0),false);
            pLayer->doEat(Vec2(0, 0),false);
            pLayer->doEat(Vec2(100, 0),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),
                                           DelayTime::create(0.9),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(-100, -90),false);
            pLayer->doEat(Vec2(0, -90),false);
            pLayer->doEat(Vec2(100, -90),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }), NULL));
    }else{
        SaleManager::getInstance()->guestGetPizza(pLayer,1,1);
//        pLayer->setRotation(100);
        pLayer->runAction(Sequence::create(DelayTime::create(1),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(60,30),false);
            pLayer->doEat(Vec2(60,-30),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),
                                           DelayTime::create(0.9),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(0,30),false);
            pLayer->doEat(Vec2(0,-30),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),
                                           DelayTime::create(0.9),
                                           CallFunc::create([=](){
            pLayer->doEat(Vec2(-60,30),false);
            pLayer->doEat(Vec2(-60,-30),false);
            AudioHelp::getInstance()->playEffect("basic/eat.mp3");
        }),NULL));
    }
    
    m_nSaleCount++;
}

void SaleScene::_giveCoint()
{
    {
        int index = SaleManager::getInstance()->getRoleIndex();
        if (index==0) {
            AudioHelp::getInstance()->playEffect("vo_thanks.mp3");
        }else if (index==1) {
            AudioHelp::getInstance()->playEffect("vo_thank_you.mp3");
        }else if (index==2) {
            AudioHelp::getInstance()->playEffect("vo_thank_you2.mp3");
        }
    }
    int count = 3;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = Vec2(500, 600);
    cfg.controlPoint_2 = Vec2(200, 300);
    cfg.endPosition = CMVisibleRect::getPosition(100, 20,kBorderLeft,kBorderTop);
    RoleModel* pGuest = SaleManager::getInstance()->getRole();
    for (int i = 0; i<count; i++) {
        Sprite* pCoint = Sprite::create("content/common/heart_coin.png");
        this->addChildToUILayer(pCoint);
        pCoint->setLocalZOrder(100);
//        SaleManager::getInstance()->guestPay(pCoint);
        Vec2 pos = Vec2(visibleSize.width/2-10.0+10.0*(arc4random()%10), 400.0+10.0*(arc4random()%20));
        pCoint->setPosition(pos);
        pCoint->setScale(0.5);
        pCoint->setLocalZOrder(count-i);
        AudioHelp::getInstance()->playEffect("coins.mp3");
        pCoint->runAction(Sequence::create(DelayTime::create(0.2*i+0.1),
                                           BezierTo::create(1, cfg),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("coin_in.mp3");

            if (i == count-1) {
                m_pGameUI->updateCoint();
                
                m_bReadySale = false;
                
                if(GameDataManager::getInstance()->m_nPackage==0){
                    _showDonut();
                }
                _showGuest();
            }
            pCoint->removeFromParent();
        }), NULL));
    }
    float coint = GameDataManager::getInstance()->getCoint();
    GameDataManager::getInstance()->setCoint(coint+count);
    

}



void SaleScene::_showDonut()
{
    if (m_pDonut) {
        m_pDonut->removeFromParent();
    }
    if(!m_pPlate) {
        if (GameDataManager::getInstance()->m_nPackage==0) {
            m_pPlate = Sprite::create("content/dec/donut/donut1_plate.png");
            m_pPlate->setScale(0.7);
        }else{
            m_pPlate = Sprite::create("content/dec/twist/donut2_plate.png");
//            m_pPlate->setScale(0.8,0.58);
        }
        this->addChildToContentLayer(m_pPlate);
//        m_pPlate->setRotation3D(Vec3(-70, 0, 0));
        CMVisibleRect::setPosition(m_pPlate, 400, 300+30-visibleSize.height);
        m_pPlate->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height)));
    }
    
    if(GameDataManager::getInstance()->m_nPackage==0){
        std::string name = GameDataManager::getInstance()->getBoxName();
        m_pDonut = _createDrageNode(FileUtils::getInstance()->getWritablePath()+name);
        this->addChildToContentLayer(m_pDonut);
        m_pDonut->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height*0.7));
        CMVisibleRect::setPosition(m_pDonut, 400, 300+50-visibleSize.height);
        m_pDonut->setOrgPositionDefault();
//        m_pDonut->setScale(m_pPlate->getScaleX());
//        m_pDonut->getDragSprite()->setRotation3D(Vec3(-70, 0, 0));
        m_pDonut->setTouchEnabled(false);
        m_pDonut->setTag(2);
        m_pDonut->setScale(0.7);
        m_pDonut->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                             CallFunc::create([=](){
            m_pDonut->setOrgPositionDefault();
            m_pDonut->setTouchEnabled(true);
        }), NULL));
    }else{
        for (int i = 0; i<4; i++) {
            std::string name = GameDataManager::getInstance()->getBoxName();
            DragNode* pDonut = _createDrageNode(FileUtils::getInstance()->getWritablePath()+name);
            this->addChildToContentLayer(pDonut);
            pDonut->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height*0.7));
            CMVisibleRect::setPosition(pDonut, 400, 430-50*i-visibleSize.height);
            pDonut->setOrgPositionDefault();
//            pDonut->setScale(m_pPlate->getScaleX());
//            pDonut->getDragSprite()->setRotation3D(Vec3(-70, 0, 0));
            pDonut->setScale(0.8);
            pDonut->setTouchEnabled(false);
            pDonut->setTag(i+2);
            pDonut->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, visibleSize.height)),
                                                 CallFunc::create([=](){
                pDonut->setOrgPositionDefault();
                pDonut->setTouchEnabled(true);
            }), NULL));
            m_pDonut = pDonut;
        }
    }
}

Node* SaleScene::_getTipDialog()
{
    int package = GameDataManager::getInstance()->m_nPackage;
    Node* pTip = Node::create();
    
    Sprite* pBubble0 = Sprite::create("content/choose/bubble0.png");
    CMVisibleRect::setPositionAdapted(pBubble0, 40, -260);
    pTip->addChild(pBubble0);
    
    Sprite* pBubble1 = Sprite::create("content/choose/bubble1.png");
    CMVisibleRect::setPositionAdapted(pBubble1, 60, -200);
    pTip->addChild(pBubble1);
    
    
    Button* pButton = Button::create("content/choose/bubble.png");
    pTip->addChild(pButton);
    pButton->addTouchEventListener([=](Ref*,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if (!m_bButtonEnable) {
                return;
            }
            DecorateManager::getInstance()->reset();
            AudioHelp::getInstance()->playEffect("vo_best_party.mp3");
            RateUsManager::getInstance()->showRateUs();
            RateUsManager::getInstance()->onRateusCallback = [=](int type) {
                AdsManager::getInstance()->showAds(kTypeInterstitialAds);
                if (package==0) {
                    SceneManager::replaceTheScene<TwistRiseDoughScene>();
                }else{
                    SceneManager::replaceTheScene<MixFlourScene>();
                }
            };
        }
    });
    
    Node* pFood = Node::create();
    pTip->addChild(pFood);
    Sprite* pDonut;
    if (package==0) {
        pDonut = Sprite::create("content/dec/twist/donut.png");
    }else{
        pDonut = Sprite::create("content/dec/donut/donut.png");
        Sprite* pDec = Sprite::create("content/category/icing/"+std::to_string(arc4random()%20+1)+".png");
        pFood->addChild(pDec,1);
        Sprite* pDec2 = Sprite::create("content/category/sauce/"+std::to_string(arc4random()%30)+".png");
        pFood->addChild(pDec2,2);
        pDec->runAction(Sequence::create(DelayTime::create(0.3),
                                         FadeIn::create(0.3), NULL));
        pDec2->runAction(Sequence::create(DelayTime::create(0.3),
                                         FadeIn::create(0.3), NULL));
    }
    pFood->addChild(pDonut,0);
    
    pBubble0->setOpacity(0);
    pBubble1->setOpacity(0);
    pFood->setScale(0.1);
    pButton->setVisible(false);
    pFood->setVisible(false);
    pButton->setTouchEnabled(false);
    pBubble0->runAction(Sequence::create(FadeIn::create(0.3), NULL));
    pBubble1->runAction(Sequence::create(DelayTime::create(0.3),
                                                FadeIn::create(0.3),
                                                CallFunc::create([=](){
        pButton->setVisible(true);
        pButton->setTouchEnabled(true);
        pFood->setVisible(true);
        pFood->runAction(ScaleTo::create(0.5, 0.3));
    }), NULL));
    return pTip;
}