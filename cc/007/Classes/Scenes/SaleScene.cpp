
#include "SaleScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"
#include "SaleManager.h"

static std::string gsModels[] = {
    "round",
    "heart",
    "cat",
    "star",
    "ginger",
    "tree",
};
SaleScene::SaleScene()
{
    m_pFood = nullptr;
//    m_pPlate = nullptr;
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
    Sprite* pBg = Sprite::create("content/make/common/bg.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create("content/make/common/bg1.png");
    pTable->setPosition(visibleSize*0.5);
    this->addChildToContentLayer(pTable);
    pTable->setLocalZOrder(1);
    
    _showFood();
    
    this->runAction(Sequence::create(DelayTime::create(0.2),
                                     CallFunc::create([=](){
//        AudioHelp::getInstance()->playEffect("vo_serve_page.mp3");
    }),
                                     DelayTime::create(0.8),
                                     CallFunc::create([=](){
        _showGuest();
    }),  NULL));
//
    
    m_pGameUI->showShareUILayout();
    m_pGameUI->showBackLayout2();
    m_pGameUI->showNormalLayout();
    
    AudioHelp::getInstance()->playBackGroundMusic("music_home.mp3");
    return true;
}

void SaleScene::onEnter()
{
    ExtensionScene::onEnter();
    
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
    if (!m_bReadySale) {
        pDragNode->back(10);
    }else{
//        RoleModel* pGuest = SaleManager::getInstance()->getRole();
        if (worldPoint.y>400+CMVisibleRect::getDesignOffset().y) {
                _pickFood();
                pDragNode->setVisible(false);
                pDragNode->setTouchEnabled(false);
                pDragNode->back(10);
            
        }else{
            pDragNode->back(10);
        }
    }

}

void SaleScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void SaleScene::onButtonCallback(Button* btn)
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
        
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        DecorateManager::getInstance()->reset();
        AudioHelp::getInstance()->resetEffect();
        GameDataManager::getInstance()->m_bShowAds = true;
        ;
        this->stopAllActions();
        for(auto pChild:m_pUILayer->getChildren()){
            pChild->stopAllActions();
        }
        for(auto pChild:m_pContentLayer->getChildren()){
            pChild->stopAllActions();
        }
        
        RoleModel* pGuest = SaleManager::getInstance()->getRole();
        if (pGuest) {
            pGuest->getArmature()->getAnimation()->stop();
            pGuest->stopAllActions();
        }
        
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
            DecorateManager::getInstance()->reset();
            
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
    },"Cookie");
}
void SaleScene::_sendEmail()
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
void SaleScene::_showGuest()
{
    RoleModel* pGuest = SaleManager::getInstance()->produceGuest();
    this->addChildToBGLayer(pGuest);
    pGuest->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "na") ){
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
                if (m_nSaleCount == 1) {
                    m_pGameUI->showNextLayout2();
                }
            }
        }else if (MovementEventType::START==movementType) {
            if (0==std::strcmp(movementID.c_str(), "eat") ){
            }
        }
    };
    
//    int radom2 = arc4random()%10*0.1;
    pGuest->setScale(1);
    
    int radom = arc4random()%100;
    int flag = 1;
    if (radom>50) {
        flag = -1;
    }
    CMVisibleRect::setPosition(pGuest, 320+flag*visibleSize.width,250);
    SaleManager::getInstance()->guestCome();

    pGuest->setScaleX(flag*pGuest->getScaleX());
    pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(-flag*visibleSize.width, 0)),
                                       CallFunc::create([=](){
        SaleManager::getInstance()->guestStandby();
        
        if (m_nSaleCount==3) {
//            AudioHelp::getInstance()->playEffect("vo_serve_make_another.mp3");
        }else if(m_nSaleCount==0){
            m_pGuideLayer->showGuideMove(m_pFood->getPosition(), pGuest->getPosition()+Vec2(0,300));
        }
    }),
                                       DelayTime::create(0.5),
                                       CallFunc::create([=](){
        m_bReadySale = true;
    }),
                                       NULL));
    this->runAction(Sequence::create(DelayTime::create(1.5),
                                     CallFunc::create([=](){
//        if (m_nSaleCount<3){
            int index = SaleManager::getInstance()->getRoleIndex();
            AudioHelp::getInstance()->playEffect("vo_serve_hello"+std::to_string(index)+".mp3");
//        }
    }),
                                     DelayTime::create(0.5),
                                     CallFunc::create([=](){
        if(m_nSaleCount==0){
            AudioHelp::getInstance()->playEffect("vo_serve_drag_serve.mp3");
        }
    }), NULL));
}

void SaleScene::_pickFood()
{
    this->stopActionByTag(100);
    
    m_bReadySale = false;
//    std::string path = FileUtils::getInstance()->getWritablePath()+GameDataManager::getInstance()->getBoxName();
//    
//    Sprite* Sprite = Sprite::create(path);
    
    Size donutSize = m_pFood->getDragSprite()->getContentSize()*1.1;
    RenderTexture* render = RenderTexture::create(donutSize.width, donutSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pFood->setPosition(donutSize*0.5);
    render->begin();
    m_pFood->visit();
    render->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Sprite* pSkin = Sprite::createWithTexture(render->getSprite()->getTexture());
    pSkin->setFlippedY(-1);
    SaleManager::getInstance()->guestGetPizza(pSkin,0.3);
    pSkin->setPosition(Vec2(pSkin->getContentSize().width/2-80, pSkin->getContentSize().height/2-80));
//    pSkin->setPosition(Vec2(39, 10));
    
    if (m_nSaleCount == 0) {
        m_pGameUI->showCointLayout();
    }
    m_nSaleCount++;
}

void SaleScene::_giveCoint()
{
    {
        int index = SaleManager::getInstance()->getRoleIndex();
        AudioHelp::getInstance()->playEffect("vo_serve_cool"+std::to_string(index)+".mp3");
    }
    int count = 3;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = Vec2(500, 600);
    cfg.controlPoint_2 = Vec2(200, 300);
    cfg.endPosition = CMVisibleRect::getPositionAdapted(Vec2(320, 930),kBorderNone,kBorderTop);
    RoleModel* pGuest = SaleManager::getInstance()->getRole();
    for (int i = 0; i<count; i++) {
        Sprite* pCoint = Sprite::create("content/common/heart_coin.png");
        this->addChildToUILayer(pCoint);
        pCoint->setLocalZOrder(1000);
        Vec2 pos = Vec2(visibleSize.width/2-10.0+10.0*(arc4random()%10), 400.0+10.0*(arc4random()%20));
        pCoint->setPosition(pos);
        pCoint->setScale(0.8);
        AudioHelp::getInstance()->playEffect("coin_out.mp3");
        pCoint->runAction(Sequence::create(DelayTime::create(0.2*i+0.1),
                                           Spawn::create(ScaleTo::create(1, 0.5),
                                                         BezierTo::create(1, cfg), NULL),
                                           CallFunc::create([=](){
            m_pGameUI->updateCoint();
        }),
                                           MoveBy::create(0.5, Vec2(10, -30)),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("coin_in.mp3");
            
            if (i == count-1) {
                
                m_bReadySale = false;
                
                _showFood();
                _showGuest();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fallStarParticle.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setLocalZOrder(200);
                pParticle->setPosition(cfg.endPosition);
            }
            pCoint->removeFromParent();
        }), NULL));
    }
    float coint = GameDataManager::getInstance()->getCoint();
    GameDataManager::getInstance()->setCoint(coint+count);
    

}



void SaleScene::_showFood()
{
    if (m_pFood) {
        m_pFood->removeFromParent();
    }
    int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    int plateIndex = GameDataManager::getInstance()->m_nPlateIndex;
    if (flavorIndex == 5) {
        m_pFood = _createDrageNode("content/make/caramel/eat/plate.png");
        this->addChildToContentLayer(m_pFood);
        m_pFood->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height*0.7));
//        CMVisibleRect::setPositionAdapted(m_pFood, 350, 410-150-visibleSize.height,kBorderNone,kBorderBottom);
        CMVisibleRect::setPositionAdapted(m_pFood, 350, 410-150-visibleSize.height);
        m_pFood->setOrgPositionDefault();
        m_pFood->setTouchEnabled(false);
        m_pFood->setTag(10);
        m_pFood->setLocalZOrder(10);
        
        Sprite* pCookie = Sprite::create("content/make/caramel/eat/cookie.png");
        pCookie->setScaleX(-1);
        m_pFood->addChild(pCookie);
        m_pFood->setLocalZOrder(10);
        pCookie->setPosition(Vec2(0, 10));
        
        m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                            CallFunc::create([=](){
            m_pFood->setOrgPositionDefault();
            m_pFood->setTouchEnabled(true);
        }), NULL));
    }else{
        std::stringstream ostr;
        int shapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
        if (flavorIndex == 4) {
            ostr<<"content/make/classic/eat/chocolate_chip.png";
        }else{
            ostr<<"content/make/classic/eat/"<<gsModels[shapeIndex]<<"_"<<flavorIndex+1<<".png";
        }
        m_pFood = _createDrageNode("content/category/plate/"+std::to_string(plateIndex)+".png");
        this->addChildToContentLayer(m_pFood);
//        CMVisibleRect::setPositionAdapted(m_pFood, 350, 420-150-visibleSize.height,kBorderNone,kBorderBottom);
        CMVisibleRect::setPositionAdapted(m_pFood, 350, 420-150-visibleSize.height);
        m_pFood->getDragSprite()->setScale(0.95,0.7);
        m_pFood->setTouchEnabled(false);
        m_pFood->setTag(10);
        
        
        Sprite* pCookie = Sprite::create(ostr.str());
        pCookie->setScaleX(-1);
        m_pFood->addChild(pCookie);
        m_pFood->setLocalZOrder(10);
        pCookie->setPosition(Vec2(0, 0));
        
        TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"dec.png");
        Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"dec.png");
        pDec->setScale(0.95,0.72);
        m_pFood->addChild(pDec);
        m_pFood->setLocalZOrder(10);
        pDec->setPosition(Vec2(0, 30));
        if (flavorIndex != 4) {
            int shape = GameDataManager::getInstance()->m_nShapeIndex;
            if(shape == 2){
                pDec->setPosition(Vec2(0, 20));
            }else if(shape == 3){
                pDec->setPosition(Vec2(0, 10));
            }else if(shape == 4){
                pDec->setPosition(Vec2(0, 0));
            }else if(shape == 5){
                pDec->setPosition(Vec2(0, 0));
            }
        }
        m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                            CallFunc::create([=](){
            m_pFood->setOrgPositionDefault();
            m_pFood->setTouchEnabled(true);
        }), NULL));
    }
//    int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
//    if(!m_pPlate) {
//        if (flavorIndex == 5) {
//            m_pPlate = Sprite::create("content/make/caramel/eat/plate.png");
//            this->addChildToContentLayer(m_pPlate);
//            m_pPlate->setLocalZOrder(5);
//            CMVisibleRect::setPositionAdapted(m_pPlate, 350, 410-150-visibleSize.height,kBorderNone,kBorderBottom);
//        }else{
//            int plateIndex = GameDataManager::getInstance()->m_nPlateIndex;
//            m_pPlate = Sprite::create("content/category/plate/"+std::to_string(plateIndex)+".png");
//            this->addChildToContentLayer(m_pPlate);
//            m_pPlate->setLocalZOrder(5);
//            CMVisibleRect::setPositionAdapted(m_pPlate, 350, 420-150-visibleSize.height,kBorderNone,kBorderBottom);
//            m_pPlate->setScale(0.95,0.7);
//        }
//        m_pPlate->runAction(MoveBy::create(1, Vec2(0, visibleSize.height)));
//    }
//    
//    if (flavorIndex == 5) {
//        m_pFood = _createDrageNode("content/make/caramel/eat/cookie.png");
//        this->addChildToContentLayer(m_pFood);
//        m_pFood->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height*0.7));
//        CMVisibleRect::setPositionAdapted(m_pFood, 350, 420-150-visibleSize.height,kBorderNone,kBorderBottom);
//        m_pFood->setOrgPositionDefault();
//        m_pFood->setTouchEnabled(false);
//        m_pFood->setTag(10);
//        m_pFood->setLocalZOrder(10);
//        m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
//                                            CallFunc::create([=](){
//            m_pFood->setOrgPositionDefault();
//            m_pFood->setTouchEnabled(true);
//        }), NULL));
//    }else{
//        std::stringstream ostr;
//        int shapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
//        if (flavorIndex == 4) {
//            ostr<<"content/make/classic/eat/chocolate_chip.png";
//        }else{
//            ostr<<"content/make/classic/eat/"<<gsModels[shapeIndex]<<"_"<<flavorIndex+1<<".png";
//        }
//        m_pFood = _createDrageNode(ostr.str());
//        this->addChildToContentLayer(m_pFood);
//        m_pFood->setScaleX(-1);
//        CMVisibleRect::setPositionAdapted(m_pFood, 350, 420-150-visibleSize.height,kBorderNone,kBorderBottom);
//        
//        Sprite* pDec = Sprite::create(FileUtils::getInstance()->getWritablePath()+"dec.png");
//        pDec->setScale(0.95,0.72);
//        pDec->setScaleX(-1);
//        m_pFood->addChild(pDec);
//        m_pFood->setLocalZOrder(10);
//        pDec->setPosition(Vec2(0, 30));
//        if (flavorIndex != 4) {
//            int shape = GameDataManager::getInstance()->m_nShapeIndex;
//            if(shape == 2){
//                pDec->setPosition(Vec2(0, 20));
//            }else if(shape == 3){
//                pDec->setPosition(Vec2(0, 10));
//            }else if(shape == 4){
//                pDec->setPosition(Vec2(0, 0));
//            }else if(shape == 5){
//                pDec->setPosition(Vec2(0, 0));
//            }
//        }
//        m_pFood->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
//                                            CallFunc::create([=](){
//            m_pFood->setOrgPositionDefault();
//            m_pFood->setTouchEnabled(true);
//        }), NULL));
//    }
}
