//
//  EatLayer.cpp
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/29.
//
//

#include "EatLayer.h"
#include "Global.h"
#include "IAPManager.h"
#include "SSCInternalLibManager.h"
#include "SSCFileUtility.h"
#include "LockScreenLayer.h"
#include "AdsManager.h"
#include "SoundPlayer.h"
#include "AdsManager.h"
#include "RecipeLayer.hpp"
#include "EatSprite.h"
#include "RuntimePermissionManager.h"

#define FOOD_MARGIN 0

enum eEatTag{
    ePreBtn_tag,
    eCameraBtn_tag,
    eHomeBtn_tag,
    eEatAgainBtn_tag,
    eScribbleNode_tag
};

Scene* EatLayer::scene()
{
    Scene* pScene = Scene::create();
    auto pLayer = EatLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

EatLayer::EatLayer():m_pUILayer(nullptr),m_pContentLayer(nullptr),m_pStickerLayer(nullptr),m_pBg(nullptr)
{
    
}

EatLayer::~EatLayer()
{
}

bool EatLayer::init()
{
    bool isInit = false;
    
    do{
        CC_BREAK_IF(!BaseLayer::init());

        CC_BREAK_IF(!initUI());
        isInit = true;
        
    }while(0);
    
    return true;
}

bool EatLayer::initUI()
{
    m_pUILayer = Layer::create();
    this->addChild(m_pUILayer, 300);
    
    m_pContentLayer = Layer::create();
    this->addChild(m_pContentLayer, 200);
    
    m_pStickerLayer = Layer::create();
    this->addChild(m_pStickerLayer, 100);
    
    this->initBackground();

    // add title
    auto pTitle = Sprite::create("images/ui/tap_to_eat.png");
    kAdapterScreen->setExactPosition(pTitle, 480 + 900, 500);
    m_pUILayer->addChild(pTitle);
    // play sound
    pTitle->runAction(Sequence::create(DelayTime::create(0.65f), CallFunc::create([=](){
        
//        SoundPlayer::getInstance()->playEffect(swishSound);
    }), nullptr));
    pTitle->runAction(Sequence::create(DelayTime::create(0.6f), EaseSineIn::create(MoveBy::create(0.55f, Vec2(-900, 0))), DelayTime::create(4.0f), EaseSineOut::create(MoveBy::create(1.2f, Vec2(-900, 0))), RemoveSelf::create(), nullptr));

    
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    // add previous button
    auto pPreBtn = ToolSprite::create("images/ui/btn_pre.png");
    pPreBtn->setTag(ePreBtn_tag);
    pPreBtn->setDelegate(this);
    pPreBtn->setScaleBtn(true);
    pPreBtn->setShadeBtn(true);
    kAdapterScreen->setExactPosition(pPreBtn, BTN_MARGIN + pPreBtn->getContentSize().width / 2, BTN_MARGIN + pPreBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderLeft, kBorderTop);
    m_pUILayer->addChild(pPreBtn);

    // add home button
    auto pHomeBtn = ToolSprite::create("images/ui/btn_home.png");
    pHomeBtn->setTag(eHomeBtn_tag);
    pHomeBtn->setDelegate(this);
    pHomeBtn->setScaleBtn(true);
    pHomeBtn->setShadeBtn(true);
    kAdapterScreen->setExactPosition(pHomeBtn, BTN_MARGIN + pHomeBtn->getContentSize().width / 2, BTN_MARGIN + pHomeBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
    m_pUILayer->addChild(pHomeBtn);
    
    // add favorite button
    auto pCameraBtn = ToolSprite::create("images/ui/btn_photo.png");
    pCameraBtn->setTag(eCameraBtn_tag);
    pCameraBtn->setDelegate(this);
    pCameraBtn->setScaleBtn(true);
    pCameraBtn->setShadeBtn(true);
    kAdapterScreen->setExactPosition(pCameraBtn, BTN_MARGIN + pCameraBtn->getContentSize().width / 2 + 2, BTN_MARGIN + pCameraBtn->getContentSize().height / 2 + 105, Vec2(0.5f, 0.5f), kBorderRight, kBorderTop);
    m_pUILayer->addChild(pCameraBtn);

    
    ///////////////////////////////////////////////////
    this->initLayer();
    this->addTouchEatNode();
    
    return true;
}

void EatLayer::initBackground()
{
    // 由于背景图片过大，在部分android机型上无法加载，所以切分成4张图拼接而成
    m_pBg = Node::create();
    m_pBg->setContentSize(Size(2345, 1703));
    m_pBg->ignoreAnchorPointForPosition(false);
    m_pBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pBg, 480, 320);
    this->addChild(m_pBg);
    
    // 左上
    auto pPart_LT = Sprite::create("images/step/bg/bg_1.jpg");
    pPart_LT->setAnchorPoint(Vec2(1, 0));
    pPart_LT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LT);
    
    // 右上
    auto pPart_RT = Sprite::create("images/step/bg/bg_2.jpg");
    pPart_RT->setAnchorPoint(Vec2(0, 0));
    pPart_RT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RT);
    
    // 左下
    auto pPart_LD = Sprite::create("images/step/bg/bg_3.jpg");
    pPart_LD->setAnchorPoint(Vec2(1, 1));
    pPart_LD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LD);
    
    // 右下
    auto pPart_RD = Sprite::create("images/step/bg/bg_4.jpg");
    pPart_RD->setAnchorPoint(Vec2(0, 1));
    pPart_RD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RD);
    
    
    ////////////////////////////////////////////////////////
    auto pTable1 = Sprite::create("images/step/table_2.png");
    kAdapterScreen->setExactPosition(pTable1, 480, 40, Vec2(0.5f, 0));
    this->addChild(pTable1, 1);
    
    auto pTable2 = Sprite::create("images/step/table_1.png");
    kAdapterScreen->setExactPosition(pTable2, 480, 40, Vec2(0.5f, 1));
    this->addChild(pTable2, 2);
}

void EatLayer::initLayer()
{
    auto offset = 0;
    if (g_iIceCreamCount == 2){
        offset = -70;
    }else if (g_iIceCreamCount == 3){
        offset = -130;
    }
    // 如果有贴纸，则添加贴纸
    if (g_iStickerIndex != -1) {
        auto pSticker = Sprite::create(__String::createWithFormat("images/dec/decCone/sticker/sticker%d.png", g_iStickerIndex)->getCString());
        kAdapterScreen->setExactPosition(pSticker, 508, 221 + offset);
        m_pStickerLayer->addChild(pSticker);
    }
}

void EatLayer::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            //add your code....
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
            SoundPlayer::getInstance()->playEffect(cameraSound);
            
            string str = SSCFileUtility::getStoragePath() + "IceCream.png";
            Director::getInstance()->getTextureCache()->removeTextureForKey(str);
            
            auto pSaveImage = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
            pSaveImage->begin();
            m_pBg->visit();
            m_pStickerLayer->visit();
            m_pContentLayer->visit();
            pSaveImage->end();
            Director::getInstance()->getRenderer()->render();
            pSaveImage->getSprite()->getTexture()->setAntiAliasTexParameters();
            
            //            CFSystemFunction album;
            SSCInternalLibManager::getInstance()->saveToAlbum(pSaveImage->newImage(), [=](bool save)
                                                              {
                                                                  saveToAlbumCallback(save);
                                                                  
                                                              }, "IceCream");
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
            this->runAction(Sequence::create(DelayTime::create(0.01),
                                             CallFunc::create([=](){
                Dialog* dialog = Dialog::create((void*)kDialogAlbum, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
                dialog->setContentText("This app does not have access to your photos.You can enable access in Privacy Setting.");
                dialog->setCallback(this);
                this->addChild(dialog, INT16_MAX);
            }), NULL));
            
        }
    }else{
        log("-------->anroid runtime permisson was not granted ,%d,requestcode = %d",bGranted,requestCode);
    }
}
void EatLayer::addTouchEatNode()
{
    ////////////////////////////////////////////////////////////
    // add icecream
    string str = SSCFileUtility::getStoragePath() + "food.png";
    Director::getInstance()->getTextureCache()->removeTextureForKey(str);

    auto eatSpirte = EatSprite::create();
    eatSpirte->_eatMaskPath = "images/ui/eat_mask.png";
    m_pContentLayer->addChild(eatSpirte);
    auto food = Sprite::create(str);
    food->setPosition(visibleSize*.5 + Size(-FOOD_MARGIN, 0));
    eatSpirte->addSprite(food);
    eatSpirte->CloneSprite();
    eatSpirte->setCheckEnable(true);
    eatSpirte->eatPercentageCallback = [=](float percent){
        if (g_isDebugMode) {
            log("percent===========%f", percent);
        }

        if (percent >= 0.85f) {
            eatSpirte->stopCheckPercentage();
            this->addEatAgainBtn();
        }
    };
}

void EatLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag())
    {
        case ePreBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(preSound);
            this->addLockScreenLayer(INT16_MAX);
            
            g_bEatSoundPlaying = false;
            g_bIsFlag = true;
            SceneManager::getInstance()->popScene();
        }
            break;
            
        case eCameraBtn_tag:
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
            break;
            
        case eHomeBtn_tag:
        {
            if(!isReturn)
            {
                SoundPlayer::getInstance()->playEffect(buttonSound);
                isReturn = true;
                
                Dialog* dialog = Dialog::create((void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
                dialog->setContentText("Do you want to start over with a new Ice Cream?");
                dialog->setCallback(this);
                this->addChild(dialog, INT16_MAX);
            }
        }
            break;
            
        case eEatAgainBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(resetSound);
            toolSprite->setSpriteEnabled(false);
            
            m_pContentLayer->removeAllChildrenWithCleanup(true);
            this->addTouchEatNode();
            
            m_pUILayer->getChildByTag(eCameraBtn_tag)->stopAllActions();
            m_pUILayer->getChildByTag(eCameraBtn_tag)->setScale(1.0f);
            toolSprite->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(0, -800))), RemoveSelf::create(), nullptr));
        }
            break;
    }
}

void EatLayer::saveToAlbumCallback(bool isSuc)
{
    if(!isReturn)
    {
        isReturn = true;
        
        Dialog* dialog = Dialog::create((void*)kDialogAlbum, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
        if(isSuc){
            dialog->setContentText("Your photo has been saved successfully!");
        }else{
            dialog->setContentText("Save to ablum failed!");
        }
        dialog->setCallback(this);
        this->addChild(dialog, INT16_MAX);
    }
}

void EatLayer::addEatAgainBtn()
{
    // add eatAgain button
    auto pEatAgainBtn = ToolSprite::create("images/ui/eat_again.png");
    pEatAgainBtn->setTag(eEatAgainBtn_tag);
    pEatAgainBtn->setDelegate(this);
    pEatAgainBtn->setScaleBtn(true);
    pEatAgainBtn->setShadeBtn(true);
    kAdapterScreen->setExactPosition(pEatAgainBtn, 480, 320 + 500);
    m_pUILayer->addChild(pEatAgainBtn);
    pEatAgainBtn->setSpriteEnabled(false);
    
    pEatAgainBtn->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(0, -500))), CallFunc::create([=](){
        
        pEatAgainBtn->setSpriteEnabled(true);
        
        auto pScale = ScaleBy::create(1.25f, 1.15f);
        pEatAgainBtn->runAction(RepeatForever::create(Sequence::create(pScale, pScale->reverse(), nullptr)));
        
        m_pUILayer->getChildByTag(eCameraBtn_tag)->runAction(RepeatForever::create(Sequence::create(pScale->clone(), pScale->clone()->reverse(), nullptr)));
    }), nullptr));
}

void EatLayer::onNegativeClick(void* type)
{
    isReturn = false;
}

void EatLayer::onPositiveClick(void* type)
{
    BaseLayer::onPositiveClick(type);
}

void EatLayer::onEnter()
{
    BaseLayer::onEnter();

    if (!kIAPManager->isShowAds())
        AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    else
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
}

void EatLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void EatLayer::onExit()
{
    BaseLayer::onExit();
}