
#include "SaleScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"
#include "SaleManager.h"

SaleScene::SaleScene()
{
    m_bReadySale = false;
    m_bBack = false;
    m_bRequestEmail = false;
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
    Sprite* pBg = Sprite::create(localPath("bg/bg_2.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg/bg_table2.png"));
    CMVisibleRect::setPosition(pTable, 320, 320-100);
    this->addChildToBGLayer(pTable);
    pTable->setLocalZOrder(1);
    
    std::string name = GameDataManager::getInstance()->getBoxName();
    m_pDrink = _createDrageNode(FileUtils::getInstance()->getWritablePath()+name);
    this->addChildToContentLayer(m_pDrink);
    CMVisibleRect::setPosition(m_pDrink, 300, 300+30);
    m_pDrink->setOrgPositionDefault();
    
    _showGuest();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGameUI->showCointLayout();
    
    m_pGameUI->showBackLayout(true,false);
    m_pGameUI->showNextLayout();
    m_pGameUI->showShareUILayout();
//    m_pGameUI->showSSCMore();
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
    
}

void SaleScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void SaleScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    int tag = pDragNode->getTag();
    if (!m_bReadySale) {
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
        
        std::string name = GameDataManager::getInstance()->getBoxName();
        m_pDrink = _createDrageNode(FileUtils::getInstance()->getWritablePath()+name);
        this->addChildToContentLayer(m_pDrink);
        CMVisibleRect::setPosition(m_pDrink, 300, 300+30);
        m_pDrink->setOrgPositionDefault();
        
        _showGuest();
    }else if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        DecorateManager::getInstance()->reset();
        AudioHelp::getInstance()->resetEffect();
        GameDataManager::getInstance()->m_bShowAds = true;
        
        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
        
        SceneManager::popToRootScene();
        SceneManager::replaceTheScene<ChooseIceShapeScene>();
        return;
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new Slushy?");
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
    },"Slushy");
}
void SaleScene::_sendEmail()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Slushy!",
                                                              "<p>Look at the slushy I just made.Isn't it so lovely? There are still tons of tools and decorations in Slushy you'll want to try!Stop hesitating! Download now!You are gonna love it!</p>  <p><a href=‘http://itunes.apple.com/app/id682415372’>http://itunes.apple.com/app/id682415372</a></p>",
                                                              (SSCFileUtility::getStoragePath()+"shotscreen.png").c_str());
#else
    //set request runtime permission callback
    
    Image* pImage = getResultRender()->newImage();
    pImage->saveToFile(SSCFileUtility::getStoragePath()+"/shotscreen.png", false);
    pImage->autorelease();
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic("Slushy!",
                                                              "Look at the slushy I just made.Isn't it so lovely? There are still tons of tools and decorations in Slushy you'll want to try!Stop hesitating! Download now!You are gonna love it!",
                                                              (SSCFileUtility::getStoragePath()+"/shotscreen.png").c_str());
    
#endif
}

void SaleScene::_showGuest()
{
    RoleModel* pGuest = SaleManager::getInstance()->produceGuest();
    this->addChildToBGLayer(pGuest);
    pGuest->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "buy") ){
                _giveCoint();
//                SaleManager::getInstance()->guestStandbyPay();
            }else if (0==std::strcmp(movementID.c_str(), "sad")) {
                this->runAction(Sequence::create(DelayTime::create(3),
                                                 CallFunc::create([=](){
                    log("======PriceScene  sad===");
//                    if(m_bReadySale){
//                        SaleManager::getInstance()->guestSad();
//                        SaleManager::getInstance()->guestStandby2();
//                    }
                    m_bReadySale = false;
                    float scaleX = pGuest->getScaleX();
                    int flag = scaleX>0?1:-1;
                    pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(flag*visibleSize.width, 0)),
                                                       CallFunc::create([=](){
                        _showGuest();
                        pGuest->removeFromParent();
                    }),
                                                       NULL));
                }), NULL));
            }
        }else if (MovementEventType::START==movementType) {
            if (0==std::strcmp(movementID.c_str(), "go") ){
                this->runAction(Sequence::create(DelayTime::create(1),
                                                 CallFunc::create([=](){
                    
                    _giveCoint();
                }), NULL));
                float scaleX = pGuest->getScaleX();
                int flag = scaleX>0?1:-1;
                pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(-flag*visibleSize.width, 0)),
                                                   CallFunc::create([=](){
                    pGuest->removeFromParent();
                }),
                                                   NULL));
            }
        }
    };
    
    int radom2 = arc4random()%10*0.1;
    pGuest->setScale(0.6+radom2);
    
    int radom = arc4random()%100;
    int flag = 1;
    if (radom>50) {
        flag = -1;
    }
    CMVisibleRect::setPosition(pGuest, 320+flag*visibleSize.width, 400);
    SaleManager::getInstance()->guestCome();

    pGuest->setScaleX(flag*pGuest->getScaleX());
    pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(-flag*visibleSize.width, 0)),
                                       CallFunc::create([=](){
        SaleManager::getInstance()->guestSad();
        SaleManager::getInstance()->guestStandby2();
        
        
        m_pGuideLayer->showGuideMove(m_pDrink->getPosition(), pGuest->getPosition()+Vec2(0,100));
        static int count = 0;
        if (count==0) {
            AudioHelp::getInstance()->playEffect("vo_hi.mp3");
        }else if (count==1) {
            AudioHelp::getInstance()->playEffect("vo_hello.mp3");
        }else if (count==3) {
            AudioHelp::getInstance()->playEffect("vo_hi2.mp3");
        }
        count++;
    }),
                                       DelayTime::create(0.5),
                                       CallFunc::create([=](){
        m_bReadySale = true;
    }),
                                       NULL));
    
//    auto action = Sequence::create(DelayTime::create(15),
//                                   CallFunc::create([=](){
//        
//        SaleManager::getInstance()->guestSad();
//        SaleManager::getInstance()->guestStandby2();
//    }), NULL);
//    action->setTag(100);
//    this->runAction(action);
}

void SaleScene::_pickDrink()
{
    this->stopActionByTag(100);
    
    AudioHelp::getInstance()->playEffect("vo_nice_work_sold.mp3");
    m_bReadySale = false;
    std::string path = FileUtils::getInstance()->getWritablePath()+GameDataManager::getInstance()->getBoxName();
    
    Skin* skin = Skin::create(path);
//    skin->setAnchorPoint(Vec2(0.5, 0.15));
    SaleManager::getInstance()->guestGetPizza(skin,0.6);
    
    static int count = 0;
    if (count==0) {
        AudioHelp::getInstance()->playEffect("vo_thanks.mp3");
    }else if (count==1) {
        AudioHelp::getInstance()->playEffect("vo_not_bad.mp3");
    }else if (count==3) {
        AudioHelp::getInstance()->playEffect("vo_thank_you.mp3");
    }
    count++;
    
}

void SaleScene::_giveCoint()
{
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
//                SaleManager::getInstance()->guestBack();
                
//                if (m_nSaleCount<6) {
//                    _produceGuese();
//                }else{
//                    m_pGameUI->hideNext();
//                    this->runAction(Sequence::create(DelayTime::create(1),
//                                                     CallFunc::create([=](){
//                        _finish();
//                    }), NULL));
//                }
                m_bReadySale = false;
                
                m_pGameUI->showSaleAgainLayout();
                
                AudioHelp::getInstance()->playEffect("drink.mp3");
            }
            pCoint->removeFromParent();
        }), NULL));
    }
    float coint = GameDataManager::getInstance()->getCoint();
    GameDataManager::getInstance()->setCoint(coint+count);
    

}


