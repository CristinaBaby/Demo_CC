//
//  OutMoldScene.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/12.
//
//

#include "OutMoldScene.h"
#include "IAPManager.h"
#include "FillMaterialModel.h"
#include "EatLayer.h"
#include "CFSystemFunction.h"
#include "MoldMacLayer.h"
#include "ChangeLoading.h"
#include "PopDialogLayer.h"


enum decType{

    eSugar = 0,
    eToppings,
    eDecoration,
    eBg,
};


string outMoldPath[6]={"bear","s","coke","fish","hambuger","heart"};
string decBtnPath[4]={"bear","bear","bear","bear"};

//糖分颜色
Color4F colorDecSugar[15] ={
    Color4F((float)255/255,(float)255/255,(float)255/255,1.0),
    Color4F((float)156/255,(float)255/255,(float)0/255,1.0),
    Color4F((float)255/255,(float)180/255,(float)0/255,1.0),
    Color4F((float)144/255,(float)0/255,(float)255/255,1.0),
    Color4F((float)255/255,(float)0/255,(float)108/255,1.0),
    
    Color4F((float)255/255,(float)0/255,(float)204/255,1.0),
    Color4F((float)255/255,(float)123/255,(float)0/255,1.0),
    Color4F((float)0/255,(float)96/255,(float)255/255,1.0),
    Color4F((float)12/255,(float)255/255,(float)0/255,1.0),
    Color4F((float)0/255,(float)186/255,(float)255/255,1.0),
    
    Color4F((float)0/255,(float)246/255,(float)255/255,1.0),
    Color4F((float)249/255,(float)252/255,(float)0/255,1.0),
    Color4F((float)0/255,(float)255/255,(float)186/255,1.0),
    Color4F((float)255/255,(float)210/255,(float)0/255,1.0),
    Color4F((float)210/255,(float)0/255,(float)255/255,1.0),
    
};

OutMoldScene::OutMoldScene()
{
    m_pCurrentSprite = nullptr;
    m_pIndex=-1;
    m_bIsTouchToppings = false;
    m_iToppingsIndex = -1;
    m_pItemBg = nullptr;
    m_pSugar = nullptr;
    m_iMoldClickNum = -1;
    m_bIsShowBtn = false;
    m_pHand = nullptr;
    m_bIsTouchSugar = false;
    m_iSugarIndex = -1;
    m_pArrow = nullptr;
    m_bIsFirst = true;
    m_bTouchBg = true;
}
OutMoldScene::~OutMoldScene()
{
    g_Mold_Sprite->release();
    g_Mold_Sprite = nullptr;
}
bool OutMoldScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
}
void OutMoldScene::onEnter()
{
    GameBaseScene::onEnter();
//    showDecUIBtn();
    
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent("Decoration page");
    
    m_pUnitScrollView->setEnable(true);
    if(m_pIndex!=-1){
    
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
        m_pItemBg = nullptr;
    
    }
    
    if(m_bIsFirst){
        m_bIsFirst = false;
        if(g_Mold_Sprite){
            
            g_Mold_Sprite->setPosition(m_pBG->convertToWorldSpace(Vec2(550 + 150,290 + 100)));
            this->addChild(g_Mold_Sprite,1);
            
            m_pMoldShadow = MySpriteButton::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s.png",outMoldPath[g_Mold_Sprite->getTag()].c_str())->getCString(),nullptr);
            m_pMoldShadow->setPosition(g_Mold_Sprite->getContentSize()/2);
            g_Mold_Sprite->addChild(m_pMoldShadow);
            m_pMoldShadow->setSelectorTouchBegan(std::bind(&OutMoldScene::moldClickBtn, this,std::placeholders::_1));
            //手指提示
            g_Mold_Sprite->setOpacity(0);
            
            
            g_Mold_Sprite->runAction(ScaleTo::create(0,1.0));
            
            showHandClickMold(0);
            
            m_pDecoritionLayer = Layer::create();
            g_Mold_Sprite->addChild(m_pDecoritionLayer,5);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan=CC_CALLBACK_2(OutMoldScene::onTouchLayer,this);
            listener->setSwallowTouches(false);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,m_pDecoritionLayer);
            
            m_pDecoritionToppingLayer = Layer::create();
            m_pDecoritionLayer->addChild(m_pDecoritionToppingLayer);
   
            auto mold = Sprite::create(__String::createWithFormat("images/make_mold/mold/pourmold_%s_blueberry.png",outMoldPath[g_Mold_Sprite->getTag()].c_str())->getCString());
            mold->setPosition(m_pDecoritionLayer->convertToNodeSpace(g_Mold_Sprite->getPosition()));
            
            m_pClippingNode = ClippingNode::create();
            m_pClippingNode->setAlphaThreshold(0.05);
            m_pClippingNode->setStencil(mold);
            m_pDecoritionLayer->addChild(m_pClippingNode);
        }
    }
}
void OutMoldScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void OutMoldScene::onExit()
{
    GameBaseScene::onExit();
    
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent("Decoration page");
}
void OutMoldScene::initUI()
{
    
//    MoldMacLayer* mold = MoldMacLayer::create();
//    this->addChild(mold,100);
//    mold->ignoreAnchorPointForPosition(false);
//    mold->setAnchorPoint(Vec2(0,1));
//    mold->setPosition(Vec2(0,Director::getInstance()->getVisibleSize().height));
    
    m_pBG = Sprite::create("images/cook_mold/make_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    m_pDecBtnBg = Sprite::create("images/dec/ban.png");
    kAdapterScreen->setExactPosition(m_pDecBtnBg,10, 320,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addChild(m_pDecBtnBg,10);
    
    float detalX = (m_pDecBtnBg->getContentSize().height - 124*4)/5;
    for (int i = 0;i<4;i++) {

        auto decBtn = MySpriteButton::create(__String::createWithFormat("images/dec/%s.png",DEC_ITEM[i].c_str())->getCString(),nullptr,false);
        decBtn->setPosition(m_pDecBtnBg->convertToWorldSpace(Vec2(m_pDecBtnBg->getContentSize().width/2,detalX + 62 + (detalX + 124)*(3-i))));
        this->addChild(decBtn,10);
        decBtn->setEnable(true);
        decBtn->setTag(i);
        decBtn->setScale(0);
        m_vDecBtn.pushBack(decBtn);
        decBtn->setSelectorTouchBegan(std::bind(&OutMoldScene::clickBtn, this,std::placeholders::_1));
        
    }
    
    m_pDecBtnBg->setPositionY(m_pDecBtnBg->getPositionY()+1000);
    m_pDecBtnBg->setOpacity(0);
    m_pUnitScrollViewBg = Sprite::create("images/dec/board_b.png");
    kAdapterScreen->setExactPosition(m_pUnitScrollViewBg, m_pDecBtnBg->getContentSize().width,320,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addChild(m_pUnitScrollViewBg,10);

    m_pUnitScrollView = UnitScrollView::create(Size(140 + 50, 490));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(OutMoldScene::onUnitCallback, this));
    //    kAdapterScreen->setExactPosition(m_pUnitScrollView,320,
    //                                     80+105,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    //    this->addToUILayer(m_pUnitScrollView,20);
    m_pUnitScrollView->setAnchorPoint(Vec2::ZERO);
    m_pUnitScrollView->setPosition(Vec2(-5,15));
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
    
    m_pUnitScrollView->m_iCol = 1;
    m_pUnitScrollView->m_fVSpace = 10;
    
    m_pUnitScrollView->m_fMarginLeft=30;
    m_pUnitScrollView->m_fMarginTop = 20;
    m_pUnitScrollView->m_bNeedAdjust = true;
    m_pUnitScrollView->m_pScrollView->setBounceable(true);
    
    m_pUnitScrollViewBg->addChild(m_pUnitScrollView);
    m_pUnitScrollViewBg->setVisible(false);
    
    m_pReset = MySpriteButton::create("images/button/public_btn_reset.png",std::bind(&OutMoldScene::resetBtn, this,std::placeholders::_1));
    m_pNext  = MySpriteButton::create("images/button/public_btn_next.png",std::bind(&OutMoldScene::nextBtn, this,std::placeholders::_1));
    m_pShare = MySpriteButton::create("images/button/camera.png",std::bind(&OutMoldScene::shareBtn, this,std::placeholders::_1));
    
    kAdapterScreen->setExactPosition(m_pShare, 10  + m_pNext->getContentSize().width + 5, 10,Vec2(1,1),kBorderRight,kBorderTop);
    kAdapterScreen->setExactPosition(m_pNext, 10, 10,Vec2(1,1),kBorderRight,kBorderTop);
    kAdapterScreen->setExactPosition(m_pReset, 10, 10,Vec2(1,0),kBorderRight,kBorderBottom);

    this->addChild(m_pReset,8);
    this->addChild(m_pNext,8);
    this->addChild(m_pShare,8);

    m_pPhotoBg = Sprite::create("images/eat/fream.png");
    kAdapterScreen->setExactPosition(m_pPhotoBg,480,320);
    this->addChild(m_pPhotoBg,20);
    
    m_pLayerColor = LayerColor::create(Color4B(0,0,0,180));
    this->addChild(m_pLayerColor,19);
    m_pLayerColor->setVisible(false);

    m_pPhoto = MySpriteButton::create("images/eat/photo.png",std::bind(&OutMoldScene::photoClick, this,std::placeholders::_1));
    m_pEmail = MySpriteButton::create("images/eat/email.png",std::bind(&OutMoldScene::emailClick, this,std::placeholders::_1));
    m_pClose = MySpriteButton::create("images/eat/popup_btn_close.png",std::bind(&OutMoldScene::closeClick, this,std::placeholders::_1));
    
    m_pPhoto->setPosition(Vec2(222,17));
    m_pEmail->setPosition(Vec2(430,17));
    m_pClose->setPosition(Vec2(618,397));
    
    m_pPhotoBg->addChild(m_pPhoto);
    m_pPhotoBg->addChild(m_pClose);
    m_pPhotoBg->addChild(m_pEmail);
    m_pPhoto->setScale(1.5);
    m_pEmail->setScale(1.5);
    
    m_pPhotoBg->setPosition(m_pShare->getPosition());
    m_pPhotoBg->setScale(0);
    
    m_pReset->setVisible(false);
    m_pNext->setVisible(false);
    m_pShare->setVisible(false);
    
}
void OutMoldScene::showPhotoBg()
{
    m_pReset->setEnable(false);
    m_pNext->setEnable(false);
    m_pShare->setEnable(false);
    m_pLayerColor->setVisible(true);
    
    for(auto btn:m_vDecBtn){
    
        btn->setEnable(false);
    
    }
    
    
    MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(DESIGN_WIDTH/2, DESIGN_HEIGHT/2))+Vec2(0,50));
    EaseSineOut* esi = EaseSineOut::create(move);
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    Spawn* sp = Spawn::create(esi,scale, NULL);
    
    ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    
    AudioHelp::getInstance()->playEffect("pop_up_open.mp3");
    
    m_pPhotoBg->runAction(Sequence::create(sp,s2,s1,s3,NULL));
}
void OutMoldScene::hidePhotoBg()
{
    MoveTo* move = MoveTo::create(0.5, m_pShare->getPosition());
    ScaleTo* scale = ScaleTo::create(0.5, 0);
    Spawn* sp = Spawn::create(move,scale, NULL);
    AudioHelp::getInstance()->playEffect("pop_up_close.mp3");
    m_pPhotoBg->runAction(Sequence::create(sp,CallFunc::create([=](){
    
        m_pReset->setEnable(true);
        m_pNext->setEnable(true);
        m_pShare->setEnable(true);
        m_pLayerColor->setVisible(false);
        if (m_pPhotoBg->getChildByTag(101)) {
            m_pPhotoBg->removeChildByTag(101);
        }
        for(auto btn:m_vDecBtn){
            
            btn->setEnable(true);
            
        }
    
    }),NULL));
}
void OutMoldScene::showFont(float dt)
{

    auto font = Sprite::create("images/dec/4.png");
    kAdapterScreen->setExactPosition(font,480,50 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(font,4);
    font->setTag(1001);
    
    AudioHelp::getInstance()->playEffect("ingredients out.mp3");

    font->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5,Vec2(0,-500)),1.0),DelayTime::create(0.2),CallFunc::create([=](){
    
    
    }), NULL));
    
}
void OutMoldScene::moldClickBtn(Ref* pSender)
{
    
    m_pMoldShadow->setEnable(false);
    m_iMoldClickNum++;
    
    if(m_iMoldClickNum==1){
    
        
        if (m_pHand) {
            m_pHand->removeFromParent();
            m_pHand=nullptr;
        }
    
        if(m_pArrow){
            m_pArrow->removeFromParent();
            m_pArrow = nullptr;
        }
        
    }
    
    
//    m_pMoldShadow->stopAllActions();
    
    AudioHelp::getInstance()->playEffect("pull out the mold.mp3");
    
    g_Mold_Sprite->runAction(Sequence::create(ScaleTo::create(0.15,1.01,0.98),ScaleTo::create(0.5,1.0), NULL));
    
    m_pMoldShadow->runAction(Sequence::create(ScaleTo::create(0.15,1.01,0.98),ScaleTo::create(0.5,1.0),MoveBy::create(0.25,Vec2(0,15)),CallFunc::create([=](){
    
    
        m_pMoldShadow->setEnable(true);
        
    
    }), NULL));
    
    if (m_iMoldClickNum == 3) {
        m_pMoldShadow->setEnable(false);
        
        AudioHelp::getInstance()->playEffect("mold_out.mp3");
        
        m_pMoldShadow->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5,Vec2(0,500)),RotateBy::create(0.5,360*2),nullptr),FadeOut::create(0.2),CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("particle success.mp3");
            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/goodParticle.plist");
            emitter1->setPosition(g_Mold_Sprite->getContentSize()/2);
            emitter1->setAutoRemoveOnFinish(true);
            g_Mold_Sprite->addChild(emitter1, 30);

            g_Mold_Sprite->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.5,Vec2(-120,-40)),1.0),CallFunc::create([=](){
                showFont(0);
                
                showDecUIBtn();
                m_pReset->setVisible(true);
                m_pNext->setVisible(true);
                m_pShare->setVisible(true);
                
            
            }), NULL));
            
            
            
            
        }),NULL));

        
    }
    
}
void OutMoldScene::showHandClickMold(float dt)
{
    if (!m_pHand) {

        m_pHand = Sprite::create("images/finger2.png");
        m_pHand->setPosition(g_Mold_Sprite->getPosition());
        this->addChild(m_pHand,4);
        m_pHand->setRotation(-50);
        m_pHand->setPositionY(m_pHand->getPositionY()-200);
        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        
            m_pHand->setVisible(true);
        
        }),EaseSineIn::create(MoveBy::create(2.0,Vec2(0,300))),CallFunc::create([=](){
        
            m_pHand->setPositionY(m_pHand->getPositionY()-300);
            m_pHand->setVisible(false);
        
        }), NULL)));

        m_pArrow = Sprite::create("images/dec/up.png");
        m_pArrow->setPosition(m_pHand->getPosition()+Vec2(-100,200));
        this->addChild(m_pArrow,100);
        
        m_pArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0,Vec2(0,10)),MoveBy::create(1.0,Vec2(0,-10)), NULL)));
        
    }
    

}
void OutMoldScene::showHandAddToppings(float dt)
{
    if(!m_pHand){
        
        m_pHand = Sprite::create("images/finger2.png");
        this->addChild(m_pHand,20);
        m_pHand->setAnchorPoint(Vec2(0.78,0.12));
        m_pHand->setPosition(g_Mold_Sprite->getPosition());
        
        m_pHand->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5,-10),RotateTo::create(0,0),RotateTo::create(0.5,-10),CallFunc::create([=](){
            
            
        }), NULL)));
    }
    

}
void OutMoldScene::closeClick(Ref* pSender)
{
AudioHelp::getInstance()->playEffect("button_general.mp3");
    hidePhotoBg();

}
void OutMoldScene::resetBtn(Ref* pSender)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    m_bIsShowReturn = true;
    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setContentText("Do you want to reset your food?");
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addChild(dialog, 9999);
}
void OutMoldScene::photoClick(Ref* pSender)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    Image* image1 = m_pRenderTexture->newImage();
    
    CFSystemFunction sys;
    sys.saveToAlbum(image1, CC_CALLBACK_1(OutMoldScene::addFavoriteCallback, this),"gummyMake");
}
void OutMoldScene::emailClick(Ref* pSender)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+GummyCaheDir).c_str());
    FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+GummyCaheDir +"/"+ "share").c_str());
    string rootPath;
    rootPath = FileUtility::getStoragePath()+"/"+GummyCaheDir+ "/" + "share" + "/";
    
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long nowtime =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    m_sPath = StringUtils::format("%s%ld.png",rootPath.c_str(),nowtime);
    
    Image* image1 = m_pRenderTexture->newImage();
    image1->saveToFile(m_sPath.c_str());
    this->scheduleOnce(schedule_selector(OutMoldScene::emailOpen), 1.5f);
}
void OutMoldScene::emailOpen(float dt)
{
    CFSystemFunction sys;
    sys.sendEmailAndFilePic("Gummy Make Maker", "I just made this gummy in the application! Let's play together. ", m_sPath.c_str());
    hidePhotoBg();
}
void OutMoldScene::nextBtn(Ref* pSender)
{
//    AudioHelp::getInstance()->playEffect("button_general.mp3");
//    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
//    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
//    dialog->setContentText("Do you want to make another gummy?");
//    dialog->setPositiveBtnText("");
//    dialog->setNegativeBtnText("");
//    dialog->setCallback(this);
//    this->addChild(dialog, 9999);
    PopDialogLayer* pop = PopDialogLayer::create(Color4B(0,0,0,180));
    pop->setAnchorPoint(Vec2::ZERO);
    this->addChild(pop,100);
    pop->btnNo=[=](){
    
        
        if(kIAPManager->isShowAds())
        {
            AdsLoadingLayer::m_bIsFrom = false;
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            AdLoadingLayerBase::loadingDoneCallback = []()
            {
                auto scene = EatLayer::createScene();
                Director::getInstance()->runWithScene(scene);
            };
        }
        else
        {
            ChangeLoading::loading([](){
                
                auto scene = EatLayer::createScene();
                Director::getInstance()->runWithScene(scene);
                
            });
        }
    };
    pop->btnOk=[=](){
        
        ChangeLoading::loading([](){
            
           SceneChangeManager->enterMakeMoldScene();
            
        });
        
    };
    
    RenderTexture* texture = RenderTexture::create(g_Mold_Sprite->getContentSize().width, g_Mold_Sprite->getContentSize().height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    texture->retain();
    texture->beginWithClear(0, 0,0,0);
    g_Mold_Sprite->setPosition(Vec2::ZERO);
    g_Mold_Sprite->setAnchorPoint(Vec2::ZERO);
    g_Mold_Sprite->setOpacity(0);
    
    if(g_Mold_Sprite->getChildByTag(1000)){
        g_Mold_Sprite->getChildByTag(1000)->setOpacity(0);
    }
    
    g_Mold_Sprite->visit();
    texture->end();
    
    Director::getInstance()->getRenderer()->render();
    texture->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    string path = FilePathUtil::newCachePath(GummyCaheDir);
    bool success = texture->saveToFile(path.c_str());
    g_Mold_Sprite->setVisible(false);
    
}
void OutMoldScene::shareBtn(Ref* pSender)
{
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    AudioHelp::getInstance()->playEffect("button_camera.mp3");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_pRenderTexture = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    m_pRenderTexture->retain();
    m_pRenderTexture->begin();
    
    m_pBG->visit();
    g_Mold_Sprite->visit();
//    m_pDecoritionLayer->visit();
    m_pContentLayer->visit();
    
    m_pRenderTexture->end();
    Director::getInstance()->getRenderer()->render();
    m_pRenderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    
//    Texture2D* tex = ->getTexture();
    Sprite* texture = m_pRenderTexture->getSprite();
    texture->setScale(MAX(660/texture->getContentSize().width, 466/texture->getContentSize().height));
    
    auto sprite = Sprite::createWithTexture(addPhoto(texture));
//    sprite->setScale(615/sprite->getContentSize().width, 390/sprite->getContentSize().height);
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(m_pPhotoBg->getContentSize()/2);
//    sprite->setPosition(Vec2(5 + m_pPhotoBg->getContentSize().width/2,9+ m_pPhotoBg->getContentSize().height/2));
    sprite->setTag(101);
    sprite->setFlippedY(true);
    m_pPhotoBg->addChild(sprite,-1);
    
    
    
    showPhotoBg();
    
   
}
void OutMoldScene::addFavoriteCallback(bool isSuc)
{
    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE);
    
    dialog -> setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    
    if(isSuc)
    {
        
        dialog->setContentText(__String::create("Your photo has been saved to your album!")->getCString());
    }
    else
    {
        dialog->setContentText("Fail to save photo!");
    }
   
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    this->addChild(dialog, 9999);
}
void OutMoldScene::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
    int lType = (uintptr_t)type;
    if (lType == kDialogExit) {
        
        
        
        
    }

}
void OutMoldScene::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
//        if(kIAPManager->isShowAds())
//        {
//
//            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
//            AdLoadingLayerBase::loadingDoneCallback = []()
//            {
//                Director::getInstance()->popToRootScene();
//                SceneChangeManager->enterHomeScene();
//            };
//        }
//        else
//        {
//            Director::getInstance()->popToRootScene();
//            SceneChangeManager->enterHomeScene();
//        }
    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");
        
        if(m_pSugar){
        
            m_pSugar->removeFromParent();
            m_pSugar = nullptr;
        
        }
        
        g_dec_bg = -1;
        m_pBG->setTexture("images/cook_mold/make_bg.jpg");
        m_pDecoritionLayer->removeAllChildren();
        
    }
    if (lType == kDialogExit) {
        
        
        
    }

}
void OutMoldScene::showDecUIBtn()
{
    m_pDecBtnBg->setOpacity(255);
    ActionUtils::moveByElasticOut(m_pDecBtnBg, Vec2(0,-1000),2.0,CallFunc::create([=](){
        for (auto btn:m_vDecBtn) {
            
            btn->runAction(Sequence::create(ScaleTo::create(0.5,1.1),ScaleTo::create(0.1,1.0),CallFunc::create([=](){
            
                if (btn->getTag() == 3) {
                    
                    m_bIsShowBtn = true;
                    
                }
                
            }), NULL));
        }
    }));
}
void OutMoldScene::clickBtn(Ref* pSender)
{
    if (!m_bIsShowBtn) {
        
        return;
        
    }
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");

//    if (m_pItemBg) {
//        m_pItemBg->removeFromParent();
//        m_pItemBg = nullptr;
//    }
    
    if (m_pHand) {
        m_pHand->removeFromParent();
        m_pHand=nullptr;
    }
    m_bIsTouchToppings = false;
    m_iToppingsIndex = -1;
    
    m_bIsTouchSugar = false;
    m_iSugarIndex = -1;
    
    AudioHelp::getInstance()->playSelectEffect();
    MySpriteButton* item = dynamic_cast<MySpriteButton*>(pSender);
    int index = item->getTag();
    
    ActionInterval *ret = NULL;
    float timeScale = 0.5;
    ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
    ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
    ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
    ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
    item->runAction(Sequence::create(ScaleTo::create(0, 1), lScale1, lScale2, lScale3, lScale4, lScale5, lScale6,CallFunc::create([=](){
    
        
        if(m_pCurrentSprite&&m_pIndex!=-1)
        {
            m_pCurrentSprite->setTexture(__String::createWithFormat("images/dec/%s.png",
                                                                    DEC_ITEM[m_pIndex].c_str())->getCString());
            m_pUnitScrollViewBg->setVisible(false);
            m_pUnitScrollView->setVisible(false);
        }
        
        if(item!=m_pCurrentSprite)
        {
            item->setTexture(__String::createWithFormat("images/dec/%s_h.png",
                                                        DEC_ITEM[index].c_str())->getCString());
            
            m_pUnitScrollView->loadingUnitScrollViewWithType(index);
            m_pItemBg = nullptr;
            m_pUnitScrollView->setVisible(true);
            m_pUnitScrollViewBg->setVisible(true);
            m_pCurrentSprite = item;
            m_pIndex = index;
            
        }
        else
        {
            m_pCurrentSprite->setTexture(__String::createWithFormat("images/dec/%s.png",
                                                                    DEC_ITEM[m_pIndex].c_str())->getCString());
            m_pUnitScrollView->setVisible(false);
            m_pUnitScrollViewBg->setVisible(false);
            m_pCurrentSprite=nullptr;
        }

    
    
    }), NULL));
}
bool OutMoldScene::onTouchLayer(Touch *pTouch, Event *pEvent)
{
    //点击Toppings
    if(m_bIsTouchToppings && m_iToppingsIndex != -1){

        if (m_pHand) {
            m_pHand->removeFromParent();
            m_pHand=nullptr;
        }
        
        auto topping = Sprite::create(__String::createWithFormat("images/dec/toppings/toppings%d.png",m_iToppingsIndex)->getCString());
        m_pClippingNode->addChild(topping);
        topping->setPosition(m_pDecoritionLayer->convertToNodeSpace(pTouch->getLocation()));
//        topping->setPosition(pTouch->getLocation());
        
        AudioHelp::getInstance()->playEffect("tap_decoration.mp3");
        
        float rotateM = rand()%360;
        float scaleM  =  0.3 + rand()%20 * 0.01;
        topping->setRotation(rotateM);
        topping->setScale(0);
        topping->runAction(Sequence::create(ScaleTo::create(0.05,0.5),ScaleTo::create(0.05,scaleM), NULL));
    }
    
    if (m_bIsTouchSugar && m_iSugarIndex!=-1) {
        
        if (m_pHand) {
            m_pHand->removeFromParent();
            m_pHand=nullptr;
        }
        
        if(g_Mold_Sprite->getBoundingBox().containsPoint(pTouch->getLocation())){
        
            m_pUnitScrollView->setEnable(false);
            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/fallParticle.plist");
            emitter1->setPosition(g_Mold_Sprite->getPosition() + Vec2(0,200));
            emitter1->setAutoRemoveOnFinish(true);
            this->addChild(emitter1, 30);
            emitter1->setStartColor(colorDecSugar[m_iSugarIndex]);
            emitter1->setEndColor(colorDecSugar[m_iSugarIndex]);
            
            AudioHelp::getInstance()->playEffect("add powder.mp3");
            
            
            ActionUtils::delayTimeAction(this,2.0,CallFunc::create([=](){
                
                m_pUnitScrollView->setEnable(true);
                if(!m_pSugar&&g_Mold_Sprite&&m_iSugarIndex!=-1){
                    
                    m_pSugar = Sprite::createWithTexture(addSugar(m_iSugarIndex));
                    m_pSugar->setPosition(g_Mold_Sprite->getContentSize()/2);
                    m_pSugar->setFlippedY(true);
//                    m_pSugar->setScale(0.907);
                    g_Mold_Sprite->addChild(m_pSugar);
                    
                }else if(g_Mold_Sprite&&m_pSugar&&m_iSugarIndex!=-1){
                    
                    m_pSugar->setTexture(addSugar(m_iSugarIndex));
                    
                }
            }));
        
        
        }
        
       

        
    }
    
    
    return true;
}
void OutMoldScene::onUnitCallback(int tag,MenuItem* menuItem,int index)
{
    AudioHelp::getInstance()->playEffect("add.mp3");
    log("----->m_pIndex  %d",m_pIndex);
    if(m_pIndex==-1){
        return;
        
    }
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(Sequence::create(MoveBy::create(0.5,Vec2(0,500)),RemoveSelf::create(),nullptr));
        
    }
    if (m_pItemBg && m_pItemBg->getParent()) {
        m_pItemBg->removeFromParent();
        m_pItemBg = nullptr;
    }
    if (m_pHand) {
        m_pHand->removeFromParent();
        m_pHand=nullptr;
    }
    
//    if(m_pCurrentSprite){
//        
//        m_pCurrentSprite->setTexture((__String::createWithFormat("images/dec/%s.png",
//                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));
//        m_pCurrentSprite=nullptr;
//    }
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);

    if(!m_pItemBg){
    
        m_pItemBg = Sprite::create("images/dec/itemBg.png");
        m_pItemBg->setPosition(item->getContentSize()/2);
        item->addChild(m_pItemBg,-1);
        m_pItemBg->setScale(0.8);
    }
    
    
    if(kIAPManager->isFree(DEC_ITEM[m_pIndex].c_str(),index))
    {
        switch (m_pIndex) {
            case eSugar:{
                if (m_pHand) {
                    m_pHand->removeFromParent();
                    m_pHand=nullptr;
                }
                m_bIsTouchSugar = true;
                m_iSugarIndex = index;
                //                showHandClickMold(0);
                showHandAddToppings(0);
                
                
                
            }
                break;
            case eToppings:{
                if (m_pHand) {
                    m_pHand->removeFromParent();
                    m_pHand=nullptr;
                }
                m_bIsTouchToppings = true;
                m_iToppingsIndex = index;
//                showHandClickMold(0);
                showHandAddToppings(0);
                
            }
                break;
            case eDecoration:{
                m_pUnitScrollView->setEnable(false);
                int v_rand = rand()%4 * 45;
                
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                ice->setPosition(m_pDecoritionLayer->convertToNodeSpace(kAdapterScreen->getExactPostion(Vec2(280 + v_rand, 320))));
                m_pDecoritionLayer->addChild(ice,10);
                ice->setUserData((void*)1);
                ice->setScale(0);
                ice->setPositionX(m_pDecoritionLayer->convertToNodeSpace(m_pBG->convertToWorldSpace(Vec2(550,290 + 100))).x+v_rand);
                
                Rect rect=Rect(Director::getInstance()->getVisibleOrigin().x,Director::getInstance()->getVisibleOrigin().y + 100,Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height);
                ice->setMoveRect(rect);
                
                Vec2 v = ice->getPosition();
                auto func=CallFunc::create([=](){
                    
//                    m_pGirdView->setEnable(true);
                    m_pUnitScrollView->setEnable(true);
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    emitter1->setPosition(ice->getContentSize()/2);
                    emitter1->setAutoRemoveOnFinish(true);
                    ice->addChild(emitter1, 30);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                    
                });
                
                MoveTo* move = MoveTo::create(0.5, v);
                ScaleTo* scale = ScaleTo::create(0.5, 0.6);
                Spawn* spwn = Spawn::create(move,scale, NULL);
                ice->runAction(Sequence::create(MoveTo::create(0, m_pDecoritionLayer->convertToNodeSpace(item->getParent()->convertToWorldSpace(item->getPosition()))),spwn,func,NULL));
                
                
            }
                break;
            case eBg:{
                
                if (!m_bTouchBg) {
                    break;
                }
                m_bTouchBg = false;
//                m_pUnitScrollView->setEnable(false);
                    g_dec_bg = index;
//                    m_pBG->setOpacity(0);
//                    m_pBG->setTexture(__String::createWithFormat("images/dec/%s/%s%d.jpg",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
//                    m_pBG->runAction(FadeIn::create(0.2));
                
//                    Sprite* bg = m_pBG;
//                    m_pBG = nullptr;
                
                    auto bg = Sprite::create(__String::createWithFormat("images/dec/%s/%s%d.jpg",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                    kAdapterScreen->setExactPosition(bg,480,320);
                    this->addChild(bg);
                    bg->setPositionX(m_pBG->getPositionX()+bg->getContentSize().width);
                
                    m_pBG->runAction(Sequence::create(MoveBy::create(0.5,Vec2(-bg->getContentSize().width,0)), NULL));
                    bg->runAction(Sequence::create(MoveBy::create(0.5,Vec2(-bg->getContentSize().width,0)),CallFunc::create([=](){
                
//                        m_pUnitScrollView->setEnable(true);
                        m_bTouchBg = true;
                        ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                        kAdapterScreen->setExactPosition(emitter1, 480,320 + 150);
                        emitter1->setAutoRemoveOnFinish(true);
                        this->addChild(emitter1,0);
                        AudioHelp::getInstance()->playEffect("all star .mp3");
                        m_pBG->removeFromParent();
                        m_pBG = nullptr;
                        m_pBG = bg;
                
                    }), NULL));
            }
                break;
            default:
            {
            }
                break;
                
        }
        
        
    }else{
        
        m_pUnitScrollView->setEnable(false);
        SceneManager::getInstance()->enterShopScene();
    }
    
    
}
//增加糖粉
Texture2D* OutMoldScene::addSugar(int index)
{
    Sprite* brush = Sprite::create(__String::createWithFormat("images/dec/sugar/mask_%s.png",outMoldPath[g_Mold_Sprite->getTag()].c_str())->getCString());

//     Sprite* brush = Sprite::create(__String::createWithFormat("images/dec/sugar/mask_%s.png",outMoldPath[0].c_str())->getCString());
    
    __String* str = __String::createWithFormat("images/dec/sugar/sugar%d.png",index);
    Sprite* mouleBg = Sprite::create(str->getCString());
    
    RenderTexture *tasteTexture = RenderTexture::create(brush->getContentSize().width, brush->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    tasteTexture->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    BlendFunc ccbFunc = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    brush->setBlendFunc(ccbFunc);
    brush->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    tasteTexture->begin();
    
    mouleBg->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    mouleBg->visit();
    brush->visit();
    tasteTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    
    log("%f  %f",tasteTexture->getSprite()->getContentSize().width,tasteTexture->getSprite()->getContentSize().height);
    
    return tasteTexture->getSprite()->getTexture();

}
Texture2D* OutMoldScene::addPhoto(Sprite* sp)
{
    Sprite* brush = Sprite::create(__String::create("images/eat/fream_bg.png")->getCString());

    Sprite* mouleBg = sp;
    
    RenderTexture *tasteTexture = RenderTexture::create(brush->getContentSize().width, brush->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    tasteTexture->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    BlendFunc ccbFunc = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    brush->setBlendFunc(ccbFunc);
    brush->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    tasteTexture->begin();
    
    mouleBg->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
    
    mouleBg->visit();
    brush->visit();
    tasteTexture->end();
    tasteTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    
    return tasteTexture->getSprite()->getTexture();
}
void OutMoldScene::nextStep()
{
    log("next");
    hideNextButton();
    
}