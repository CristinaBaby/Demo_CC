//
//  ShopLayer.cpp
//  FastFood
//
//  Created by huxixiang on 15/9/2.
//
//

#include "ShopLayer.h"
#include "IAPManager.h"
#include "Global.h"
#include "AdsManager.h"

enum ShopLayer_Tag {
    eUnlockAll_tag,
    eUnlockFlavors_tag,
    eUnlockDecorations_tag,
    eNoAds_tag,
    eRestoreBtn_tag,
    eBackBtn_tag
};

Scene* ShopLayer::scene()
{
    Scene* pScene = Scene::create();
    auto pLayer = ShopLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

ShopLayer::ShopLayer():m_pBg(nullptr),m_pGirl(nullptr),m_iCurrtTag(-1),isReturn(false)
{
    
}

ShopLayer::~ShopLayer()
{
    
}

bool ShopLayer::init()
{
    bool isInit = false;
    
    do{
        CC_BREAK_IF(!Layer::init());
        
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer());
        
        if (!g_bBgMusicType) {
            g_bBgMusicType = true;
            SoundPlayer::getInstance()->stopBackGroundMusic();
            SoundPlayer::getInstance()->playBackGroundMusic(bgMusick);
        }
        
        isInit = true;
        
    }while(0);
    
    return isInit;
}

bool ShopLayer::initData()
{
    this->setKeypadEnabled(true);
    
    return true;
}

bool ShopLayer::initLayer()
{
    kIAPManager->m_fnAfterPurchase = CC_CALLBACK_1(ShopLayer::purchaseCallback, this);
    kIAPManager->m_fnAfterRestore = CC_CALLBACK_2(ShopLayer::restoreCallback, this);
    
    // add bg
    m_pBg = Sprite::create("images/shop/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg, 480, 320);
    this->addChild(m_pBg);
    
    // awning
    auto pAwning = Sprite::create("images/shop/awning.png");
    kAdapterScreen->setExactPosition(pAwning, 480, 640 - 38, Vec2(0.5f, 0), kBorderNone, kBorderNone);
    this->addChild(pAwning, 100);

    // add back button
    auto pBackBtn = ToolSprite::create("images/ui/btn_pre.png");
    kAdapterScreen->setExactPosition(pBackBtn, 18 + pBackBtn->getContentSize().width / 2, 18 + pBackBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderLeft, kBorderTop);
    pBackBtn->setDelegate(this);
    pBackBtn->setShadeBtn(true);
    pBackBtn->setTag(eBackBtn_tag);
    this->addChild(pBackBtn, INT16_MAX);
    

    //////////////////////////
    this->addIapButton();
    
    return true;
}

void ShopLayer::addIapButton()
{
    // add shop item
    auto pUnlockAllBg = Sprite::create("images/shop/unlock_all.png");
    kAdapterScreen->setExactPosition(pUnlockAllBg, 675, 440);
    this->addChild(pUnlockAllBg, 3);
    
    auto pUnlockAll = ToolSprite::create("images/shop/buy_big.png");
    kAdapterScreen->setExactPosition(pUnlockAll, 675, 340);
    pUnlockAll->setDelegate(this);
    pUnlockAll->setShadeBtn(true);
    pUnlockAll->setTag(eUnlockAll_tag);
    this->addChild(pUnlockAll, 4);
//    if (kIAPManager->isPackagePurchased(0) || (kIAPManager->isPackagePurchased(1) && kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3))) {
//        pUnlockAll->setColor(Color3B(pUnlockAll->getColor().r / 2, pUnlockAll->getColor().g / 2, pUnlockAll->getColor().b / 2));
//    }
    
    auto pUnlockFlavors = ToolSprite::create("images/shop/buy_big.png");
    kAdapterScreen->setExactPosition(pUnlockFlavors, 380, 52);
    pUnlockFlavors->setDelegate(this);
    pUnlockFlavors->setShadeBtn(true);
    pUnlockFlavors->setTag(eUnlockFlavors_tag);
    this->addChild(pUnlockFlavors, 2);
//    if (kIAPManager->isPackagePurchased(0) || kIAPManager->isPackagePurchased(1)) {
//        pUnlockFlavors->setColor(Color3B(pUnlockFlavors->getColor().r / 2, pUnlockFlavors->getColor().g / 2, pUnlockFlavors->getColor().b / 2));
//    }
    
    auto pUnlockDecorations = ToolSprite::create("images/shop/buy_big.png");
    kAdapterScreen->setExactPosition(pUnlockDecorations, 822, 52);
    pUnlockDecorations->setDelegate(this);
    pUnlockDecorations->setShadeBtn(true);
    pUnlockDecorations->setTag(eUnlockDecorations_tag);
    this->addChild(pUnlockDecorations, 1);
//    if (kIAPManager->isPackagePurchased(0) || kIAPManager->isPackagePurchased(2)) {
//        pUnlockDecorations->setColor(Color3B(pUnlockDecorations->getColor().r / 2, pUnlockDecorations->getColor().g / 2, pUnlockDecorations->getColor().b / 2));
//    }
    
    m_pGirl = Sprite::create("images/shop/noads_girl.png");
    kAdapterScreen->setExactPosition(m_pGirl, 235, 405);
    this->addChild(m_pGirl, 1);
    
    auto pNoAds = ToolSprite::create("images/shop/noads_hand.png");
    kAdapterScreen->setExactPosition(pNoAds, 340, 360);
    pNoAds->setDelegate(this);
    pNoAds->setShadeBtn(true);
    pNoAds->setTag(eNoAds_tag);
    this->addChild(pNoAds, 2);
//    if (kIAPManager->isPackagePurchased(0) || kIAPManager->isPackagePurchased(3)) {
//        pNoAds->setColor(Color3B(pNoAds->getColor().r / 2, pNoAds->getColor().g / 2, pNoAds->getColor().b / 2));
//        m_pGirl->setColor(Color3B(m_pGirl->getColor().r / 2, m_pGirl->getColor().g / 2, m_pGirl->getColor().b / 2));
//    }
    
    // add restore button
    auto pRestoreBtn = ToolSprite::create("images/shop/restore.png");
    kAdapterScreen->setExactPosition(pRestoreBtn, 116, 320);
    pRestoreBtn->setDelegate(this);
    pRestoreBtn->setShadeBtn(true);
    pRestoreBtn->setTag(eRestoreBtn_tag);
    this->addChild(pRestoreBtn, 2);
}

void ShopLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eNoAds_tag) {
        m_pGirl->setColor(Color3B(m_pGirl->getColor().r / 2, m_pGirl->getColor().g / 2, m_pGirl->getColor().b / 2));
    }
}

void ShopLayer::onTouchUp(ToolSprite* toolSprite, Touch *pTouch)
{
    if (toolSprite->getTag() == eNoAds_tag) {
        m_pGirl->setColor(Color3B(m_pGirl->getColor().r * 2, m_pGirl->getColor().g * 2, m_pGirl->getColor().b * 2));
    }

}

void ShopLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag())
    {
        case eBackBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            toolSprite->setSpriteEnabled(false);
            
            g_bIsNeedAds = false;
            g_bIsFromShop = true;

            Director::getInstance()->popScene();
        }
            break;
            
        case eUnlockAll_tag:
        {
            kIAPManager->printIapId(0);
            if (!kIAPManager->isPackagePurchased(0) && !(kIAPManager->isPackagePurchased(1) && kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3)))
            {
                SoundPlayer::getInstance()->playEffect(selectSound);
                m_iCurrtTag = eUnlockAll_tag;
                kIAPManager->purchase(0);
            }
            else
            {
                showDialog();
            }
        }
            break;
            
        case eUnlockFlavors_tag:
        {
            kIAPManager->printIapId(1);
            if (!kIAPManager->isPackagePurchased(0))
            {
                if (kIAPManager->isPackagePurchased(1))
                {
                    showDialog();
                }
                else
                {
                    SoundPlayer::getInstance()->playEffect(selectSound);
                    m_iCurrtTag = eUnlockFlavors_tag;
                    kIAPManager->purchase(1);
                }
            }
            else
            {
                showDialog();
            }
        }
            break;
            
        case eUnlockDecorations_tag:
        {
            kIAPManager->printIapId(2);
            if (!kIAPManager->isPackagePurchased(0))
            {
                if (kIAPManager->isPackagePurchased(2))
                {
                    showDialog();
                }
                else
                {
                    SoundPlayer::getInstance()->playEffect(selectSound);
                    m_iCurrtTag = eUnlockDecorations_tag;
                    kIAPManager->purchase(2);
                }
            }
            else
            {
                showDialog();
            }
        }
            break;
            
        case eNoAds_tag:
        {
            kIAPManager->printIapId(3);
            if (!kIAPManager->isPackagePurchased(0))
            {
                if (kIAPManager->isPackagePurchased(3))
                {
                    showDialog();
                }
                else
                {
                    SoundPlayer::getInstance()->playEffect(selectSound);
                    m_iCurrtTag = eNoAds_tag;
                    kIAPManager->purchase(3);
                }
            }
            else
            {
                showDialog();
            }
        }
            break;
            
        case eRestoreBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            
            kIAPManager->restore();
        }
            break;
    }
}

void ShopLayer::purchaseCallback(bool isSuc)
{
    if (isSuc) {
//        this->getChildByTag(m_iCurrtTag)->setColor(Color3B(this->getChildByTag(m_iCurrtTag)->getColor().r / 2, this->getChildByTag(m_iCurrtTag)->getColor().g / 2, this->getChildByTag(m_iCurrtTag)->getColor().b / 2));
//        this->getChildByTag(m_iCurrtTag)->setOpacity(254);
//        
//        if (m_iCurrtTag == eNoAds_tag) {
//            m_pGirl->setColor(Color3B(m_pGirl->getColor().r / 2, m_pGirl->getColor().g / 2, m_pGirl->getColor().b / 2));
//        }

        if (m_iCurrtTag == eUnlockAll_tag) {
            IAPManager::getInstance()->setPackageState(1, true);
            IAPManager::getInstance()->setPackageState(2, true);
            IAPManager::getInstance()->setPackageState(3, true);
//            if (this->getChildByTag(eUnlockFlavors_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockFlavors_tag)->setColor(Color3B(this->getChildByTag(eUnlockFlavors_tag)->getColor().r / 2, this->getChildByTag(eUnlockFlavors_tag)->getColor().g / 2, this->getChildByTag(eUnlockFlavors_tag)->getColor().b / 2));
//                this->getChildByTag(eUnlockFlavors_tag)->setOpacity(254);
//            }
//            
//            if (this->getChildByTag(eUnlockDecorations_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockDecorations_tag)->setColor(Color3B(this->getChildByTag(eUnlockDecorations_tag)->getColor().r / 2, this->getChildByTag(eUnlockDecorations_tag)->getColor().g / 2, this->getChildByTag(eUnlockDecorations_tag)->getColor().b / 2));
//                this->getChildByTag(eUnlockDecorations_tag)->setOpacity(254);
//            }
//            
//            if (this->getChildByTag(eNoAds_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eNoAds_tag)->setColor(Color3B(this->getChildByTag(eNoAds_tag)->getColor().r / 2, this->getChildByTag(eNoAds_tag)->getColor().g / 2, this->getChildByTag(eNoAds_tag)->getColor().b / 2));
//                m_pGirl->setColor(Color3B(m_pGirl->getColor().r / 2, m_pGirl->getColor().g / 2, m_pGirl->getColor().b / 2));
//                this->getChildByTag(eNoAds_tag)->setOpacity(254);
//            }
        }
        
//        if (kIAPManager->isPackagePurchased(1) && kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3)) {
//            if (this->getChildByTag(eUnlockAll_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockAll_tag)->setOpacity(254);
//                this->getChildByTag(eUnlockAll_tag)->setColor(Color3B(this->getChildByTag(eUnlockAll_tag)->getColor().r / 2, this->getChildByTag(eUnlockAll_tag)->getColor().g / 2, this->getChildByTag(eUnlockAll_tag)->getColor().b / 2));
//            }
//        }
    }
}

void ShopLayer::showDialog()
{
    if(!isReturn)
    {
        SoundPlayer::getInstance()->playEffect(buttonSound);
        isReturn = true;
        
        Dialog* dialog = Dialog::create((void*)kDialogTypeNone, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL, 0.78f, 0.4f);
        dialog->setContentText("You have already purchased this package!");
        dialog->setCallback(this);
        this->addChild(dialog, INT16_MAX);
    }
}

void ShopLayer::restoreCallback(bool isRestore, bool isPurchase)
{
    if(!isReturn)
    {
        isReturn = true;
        
        Dialog* dialog = Dialog::create((void*)kDialogRestored, Dialog::DIALOG_TYPE_SINGLE, DialogDirection::HORIZONTAL, 0.83f, 0.45f);
        if(isRestore)
        {
            if(isPurchase)
            {
                dialog->setContentText("Your content has been restored!");
            }
            else
            {
                dialog->setContentText("Sorry! It looks like you haven't purchased anything yet.");
            }
        }
        else
        {
            dialog->setContentText("Sorry, restore transaction failed!");
        }
        dialog->setCallback(this);
        this->addChild(dialog, INT16_MAX);
        
        
//        if (kIAPManager->isPackagePurchased(0)) {
//            if (this->getChildByTag(eUnlockAll_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockAll_tag)->setOpacity(254);
//                this->getChildByTag(eUnlockAll_tag)->setColor(Color3B(this->getChildByTag(eUnlockAll_tag)->getColor().r / 2, this->getChildByTag(eUnlockAll_tag)->getColor().g / 2, this->getChildByTag(eUnlockAll_tag)->getColor().b / 2));
//            }
//        }
//        
//        if (kIAPManager->isPackagePurchased(1) || (!kIAPManager->isPackagePurchased(1) && kIAPManager->isPackagePurchased(0))) {
//            if (this->getChildByTag(eUnlockFlavors_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockFlavors_tag)->setOpacity(254);
//                this->getChildByTag(eUnlockFlavors_tag)->setColor(Color3B(this->getChildByTag(eUnlockFlavors_tag)->getColor().r / 2, this->getChildByTag(eUnlockFlavors_tag)->getColor().g / 2, this->getChildByTag(eUnlockFlavors_tag)->getColor().b / 2));
//            }
//        }
//        
//        if (kIAPManager->isPackagePurchased(2) || (!kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(0))) {
//            if (this->getChildByTag(eUnlockDecorations_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockDecorations_tag)->setOpacity(254);
//                this->getChildByTag(eUnlockDecorations_tag)->setColor(Color3B(this->getChildByTag(eUnlockDecorations_tag)->getColor().r / 2, this->getChildByTag(eUnlockDecorations_tag)->getColor().g / 2, this->getChildByTag(eUnlockDecorations_tag)->getColor().b / 2));
//            }
//        }
//        
//        if (kIAPManager->isPackagePurchased(3) || (!kIAPManager->isPackagePurchased(3) && kIAPManager->isPackagePurchased(0))) {
//            if (this->getChildByTag(eNoAds_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eNoAds_tag)->setOpacity(254);
//                this->getChildByTag(eNoAds_tag)->setColor(Color3B(this->getChildByTag(eNoAds_tag)->getColor().r / 2, this->getChildByTag(eNoAds_tag)->getColor().g / 2, this->getChildByTag(eNoAds_tag)->getColor().b / 2));
//                m_pGirl->setColor(Color3B(m_pGirl->getColor().r / 2, m_pGirl->getColor().g / 2, m_pGirl->getColor().b / 2));
//            }
//        }
//        
//        if (kIAPManager->isPackagePurchased(1) && kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3)) {
//            if (this->getChildByTag(eUnlockAll_tag)->getOpacity() >= 255) {
//                this->getChildByTag(eUnlockAll_tag)->setOpacity(254);
//                this->getChildByTag(eUnlockAll_tag)->setColor(Color3B(this->getChildByTag(eUnlockAll_tag)->getColor().r / 2, this->getChildByTag(eUnlockAll_tag)->getColor().g / 2, this->getChildByTag(eUnlockAll_tag)->getColor().b / 2));
//            }
//        }
    }
}

void ShopLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            isReturn = true;

            auto pLockLayer = LockScreenLayer::create();
            this->addChild(pLockLayer, INT16_MAX);
            
            g_bIsNeedAds = false;
            g_bIsFromShop = true;
            
            Director::getInstance()->popScene();
        }
    }
}

void ShopLayer::onNegativeClick(void* type)
{
    isReturn = false;
}

void ShopLayer::onPositiveClick(void* type)
{
    isReturn = false;
}

void ShopLayer::onEnter()
{
    Layer::onEnter();
    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}

void ShopLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}