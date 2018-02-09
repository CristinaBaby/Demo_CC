//
// DressUpScene.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/25.
//
//

#include "DressUpScene.h"
#include "XCListButton.h"
#include "ShowScene.h"
#include "SPAScene.h"
#include "IAPManager.h"
#include "ShopLayer.h"
#include "AdsManager.h"

static const std::string sIconPath[] = {"cocoStudio/ui04_dressup/icon_clothes.png", "cocoStudio/ui04_dressup/icon_glasses.png", "cocoStudio/ui04_dressup/icon_hats.png", "cocoStudio/ui04_dressup/icon_necklace.png", "cocoStudio/ui04_dressup/icon_shoes.png"};

DressUpScene::DressUpScene()
:m_pAnimal(nullptr)
,m_pCategoryList(nullptr)
,m_pObjectsList(nullptr)
,m_pObjectsLayer(nullptr)
,m_pPopUp(nullptr)
{
    
}

void DressUpScene::onEnter()
{
    BaseScene::onEnter();
    if (!IAPManager::getInstance()->isAdRemoved()) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
}

DressUpScene* DressUpScene::create()
{
    auto pRet = new DressUpScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool DressUpScene::init()
{
    if ( !BaseScene::init("bg/dress_bg.jpg") )
    {
        return false;
    }
    
    setName("DressUpScene");
    
    m_pAnimal = AnimalModel::create(AnimalModel::Animal::kNone);
    m_pAnimal->setPosition(XCVisibleRect::getPosition(300, 480));
    m_pGameLayer->addChild(m_pAnimal, kGameZorder);
    
    auto *pUI = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/spa.json");
    m_pUILayer->addChild(pUI);
    
    auto pPrevious = Helper::seekWidgetByName(pUI, "public_btn_back");
    pPrevious->setTag(1);
    pPrevious->addTouchEventListener(CC_CALLBACK_2(DressUpScene::onButtonCallback, this));
    
    auto pNext = Helper::seekWidgetByName(pUI, "public_btn_next");
    pNext->setTag(2);
    pNext->addTouchEventListener(CC_CALLBACK_2(DressUpScene::onButtonCallback, this));
    
    m_pCategoryList = XCScrolledList1::create(Size(Director::getInstance()->getWinSize().width, 160));
    m_pCategoryList->setDirection(XCScrollView::Direction::HORIZONTAL);
    m_pCategoryList->setItemSize(Size(200, 160));
    m_pCategoryList->setPosition(Vec2(0, XCVisibleRect::getPositionY(100, kBottom)));
    m_pUILayer->addChild(m_pCategoryList);
    
    std::vector<Node *> vTemp;
    for (int i=0; i<5; ++i) {
        XCListButton *pButton = XCListButton::create(sIconPath[i], sIconPath[i]);
        pButton->setTag(i);
        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onCategoryCallback, this));
        vTemp.push_back(pButton);
    }
    m_pCategoryList->pushItems(vTemp);
    
    
    m_pObjectsLayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.8));
    m_pObjectsLayer->setVisible(false);
    m_pUILayer->addChild(m_pObjectsLayer, kUIZorder);
    
    auto touch = EventListenerTouchOneByOne::create();
    touch->setSwallowTouches(true);
    touch->onTouchBegan = [this](Touch *, Event *)->bool{
        if(m_pObjectsLayer->isVisible() || m_pPopUp) {
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch, m_pObjectsLayer);
    
    m_pObjectsList = XCScrolledList1::create(Size(580, Director::getInstance()->getWinSize().height - XCVisibleRect::getPositionX(130) - 100));
    m_pObjectsList->setDirection(XCScrollView::Direction::VERTICAL);
    m_pObjectsList->setMaxColumn(3);
    m_pObjectsList->setItemSize(Size(180, 150));
    m_pObjectsList->setPosition(Vec2((Director::getInstance()->getWinSize().width - 580) / 2, XCVisibleRect::getPositionX(130)));
    m_pObjectsLayer->addChild(m_pObjectsList, 1);
    
    Button *pClose = Button::create("cocoStudio/public/public_btn_close.png");
    pClose->setPosition(XCVisibleRect::getPosition(pClose->getContentSize().width * 0.5 + 20, pClose->getContentSize().height * 0.5 + 20, kRight, kTop));
    m_pObjectsLayer->addChild(pClose);
    pClose->addTouchEventListener([this](cocos2d::Ref *aButton, Widget::TouchEventType aType){
        if (aType != Widget::TouchEventType::ENDED) {
            return;
        }
        m_pObjectsLayer->setVisible(false);
        m_pObjectsList->cleanupScrollList();
    });
    
    if (Director::getInstance()->getRunningScene()->getName() == "SPAScene") {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/dress.mp3", true);
    }
    
    m_pCustom = _eventDispatcher->addCustomEventListener("ShopBuySuccess", [this](EventCustom *){
        if (IAPManager::getInstance()->isAdRemoved()) {
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    });
    
    return true;
}

void DressUpScene::onButtonCallback(cocos2d::Ref *aButton, Widget::TouchEventType aType)
{
    if (aType != Widget::TouchEventType::ENDED) {
        return;
    }
    
    switch (dynamic_cast<Button *>(aButton)->getTag()) {
        case 1:
        {
            
            m_pPopUp = GUIReader::getInstance()->widgetFromJsonFile("cocoStudio/popup.json");
            m_pUILayer->addChild(m_pPopUp, kPopZorder);
            
            auto pYes = Helper::seekWidgetByName(m_pPopUp, "popup_btn_yes");
            pYes->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
                Director::getInstance()->replaceScene(SPAScene::create(AnimalModel::create(AnimalModel::Animal::kNone)));
                m_pAnimal->clearCloth();
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            });
            auto pNo = Helper::seekWidgetByName(m_pPopUp, "popup_btn_quit");
            pNo->addTouchEventListener([this](Ref *, Widget::TouchEventType aType){
                m_pPopUp->removeFromParent();
                m_pPopUp = nullptr;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            });
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
        }
            break;
        case 2:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button.mp3");
            Director::getInstance()->replaceScene(ShowScene::create());
        }
            break;
        default:
            break;
    }
}

void DressUpScene::onCategoryCallback(cocos2d::Node *aSender)
{
    
    m_pObjectsList->cleanupScrollList();
    
    std::vector<Node *> vTemp;
    
    switch (aSender->getTag()) {
        case 0:
        {
            switch (m_pAnimal->getAnimalType()) {
                case AnimalModel::Animal::kRagDoll:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/cat/ragdoll/clothing/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/cat/ragdoll/clothing/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                case AnimalModel::Animal::kBeagle:
                {
                    for (int i=1; i<=20; ++i) {
                        std::string resourcePath = StringUtils::format("dress/dog/beagle/clothing/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/dog/beagle/clothing/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }

                    }
                }
                    break;
                case AnimalModel::Animal::kSchnauzer:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/dog/schnauzer/clothing/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/dog/schnauzer/clothing/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                case AnimalModel::Animal::kTabby:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/cat/tabby/clothing/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/cat/tabby/clothing/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case 1:
        {
            std::string rootPath = (m_pAnimal->getAnimalType() == AnimalModel::Animal::kRagDoll || m_pAnimal->getAnimalType() == AnimalModel::Animal::kTabby) ? "dress/cat" : "dress/dog";
            
            for (int i=1; i<=10; ++i) {
                std::string resourcePath = StringUtils::format("%s/glasses/%d.png", rootPath.c_str(), i);
                std::string iconPath = StringUtils::format("%s/glasses/icon/%d.png", rootPath.c_str(), i);
                XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                pButton->setName(resourcePath);
                pButton->setTag(aSender->getTag());
                pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                vTemp.push_back(pButton);
                
                if (i > 5 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                    Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                    pLock->setScale(0.5);
                    pButton->setTag(112);
                    pLock->setPosition(pButton->getContentSize() / 2);
                    pButton->addChild(pLock, 10);
                }
            }
            
        }
            break;
        case 2:
        {
            std::string rootPath = (m_pAnimal->getAnimalType() == AnimalModel::Animal::kRagDoll || m_pAnimal->getAnimalType() == AnimalModel::Animal::kTabby) ? "dress/cat" : "dress/dog";
            
            for (int i=1; i<=20; ++i) {
                
                if (i == 11) {
                    continue;
                }
                
                std::string resourcePath = StringUtils::format("%s/headwear/%d.png", rootPath.c_str(), i);
                std::string iconPath = StringUtils::format("%s/headwear/icon/%d.png", rootPath.c_str(), i);
                XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                pButton->setName(resourcePath);
                pButton->setTag(aSender->getTag());
                pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                vTemp.push_back(pButton);
                
                if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                    Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                    pLock->setScale(0.5);
                    pButton->setTag(112);
                    pLock->setPosition(pButton->getContentSize() / 2);
                    pButton->addChild(pLock, 10);
                }
            }
        }
            break;
        case 3:
        {
            std::string rootPath = (m_pAnimal->getAnimalType() == AnimalModel::Animal::kRagDoll || m_pAnimal->getAnimalType() == AnimalModel::Animal::kTabby) ? "dress/cat" : "dress/dog";
            
            for (int i=1; i<=20; ++i) {
                std::string resourcePath = StringUtils::format("%s/necklace/%d.png", rootPath.c_str(), i);
                std::string iconPath = StringUtils::format("%s/necklace/icon/%d.png", rootPath.c_str(), i);
                XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                pButton->setName(resourcePath);
                pButton->setTag(aSender->getTag());
                pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                vTemp.push_back(pButton);
                
                if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                    Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                    pLock->setScale(0.5);
                    pButton->setTag(112);
                    pLock->setPosition(pButton->getContentSize() / 2);
                    pButton->addChild(pLock, 10);
                }
            }
        }
            break;
        case 4:
        {
            switch (m_pAnimal->getAnimalType()) {
                case AnimalModel::Animal::kRagDoll:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/cat/ragdoll/shoe/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/cat/ragdoll/shoe/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                case AnimalModel::Animal::kBeagle:
                {
                    for (int i=1; i<=20; ++i) {
                        std::string resourcePath = StringUtils::format("dress/dog/beagle/shoe/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/dog/beagle/shoe/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                case AnimalModel::Animal::kSchnauzer:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/dog/schnauzer/shoe/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/dog/schnauzer/shoe/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                case AnimalModel::Animal::kTabby:
                {
                    for (int i=1; i<=20; ++i) {
                        
                        std::string resourcePath = StringUtils::format("dress/cat/tabby/shoe/%d.png", i);
                        std::string iconPath = StringUtils::format("dress/cat/tabby/shoe/icon/%d.png", i);
                        XCListButton *pButton = XCListButton::create(iconPath, iconPath);
                        pButton->setName(resourcePath);
                        pButton->setTag(aSender->getTag());
                        pButton->setTouchCallback(CC_CALLBACK_1(DressUpScene::onChangeClothCallback, this));
                        vTemp.push_back(pButton);
                        
                        if (i > 10 && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockDressUp) && !IAPManager::getInstance()->getItemIsBought(kIAPUnlockAll)) {
                            Sprite *pLock = Sprite::create("cocoStudio/public/public_lock.png");
                            pLock->setScale(0.5);
                            pButton->setTag(112);
                            pLock->setPosition(pButton->getContentSize() / 2);
                            pButton->addChild(pLock, 10);
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    m_pObjectsList->pushItems(vTemp);
    Scale9Sprite *pBg = Scale9Sprite::create("sroll_bar.png", Rect(0, 0, 20, 20));
    Scale9Sprite *pImage = Scale9Sprite::create("sroll.png", Rect(0, 0, 20, 20));
    m_pObjectsList->setBarImage(pBg, pImage);
    m_pObjectsLayer->setVisible(true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/button_dress.mp3");
}

void DressUpScene::onChangeClothCallback(Node *aButton)
{
    
    if (aButton->getTag() == 112) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/popup.mp3");
        m_pObjectsLayer->setVisible(false);
        m_pObjectsList->cleanupScrollList();
        m_pUILayer->addChild(ShopLayer::create(), kPopZorder);
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/UI/select_dress.mp3");
    std::string reousrcePath = aButton->getName();
    AnimalModel::ClothType type = (AnimalModel::ClothType)aButton->getTag();
    m_pAnimal->changeClothing(type, Sprite::create(reousrcePath));
    m_pObjectsLayer->setVisible(false);
    m_pObjectsList->cleanupScrollList();
}

void DressUpScene::onExit()
{
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    BaseScene::onExit();
}
DressUpScene::~DressUpScene()
{
    _eventDispatcher->removeEventListener(m_pCustom);
}