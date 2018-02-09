//
//  CBPackScene.cpp
//  ColorBook
//
//  Created by maxiang on 4/22/15.
//
//

#include "CBPackScene.h"
#include "HomeScene.h"
#include "../Model/AppConfigs.h"
#include "../Model/PackManager.h"
#include "../Model/STVisibleRect.h"
#include "../Model/CBAppManager.h"
#include "../Model/CBUtils.h"
#include "AdLoadingLayerBase.h"
#include "AdsLoadingLayer.h"
#include "TimerManager.hpp"
#include "CBPictureScene.h"
#include "STSystemFunction.h"
#include "LockManager.h"
#include "TryLaterLayer.h"

#include "UICpp.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define TAG_PAGE_CONTROLLER  1234
#define RES_BACK_BUTTON      "public/back.png"
#define RES_BG               "pack/bg57.png"

#define RES_CONTROL_NORMAL   "pack/ui_page_bai.png"
#define RES_CONTROL_SELECT   "pack/ui_page_hui.png"

PackScene::~PackScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool PackScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        setName("PackScene");
        _isRewardAd = getNetWorkState();
        
        //back ground
        Sprite *background = Sprite::create(RES_BG);
        CC_BREAK_IF(!background);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //back
        auto winSize = Director::getInstance()->getWinSize();
        _back = ui::Button::create(RES_BACK_BUTTON);
        _back->addTouchEventListener(CC_CALLBACK_2(PackScene::touchDown, this));
        _back->setPosition(xApp->getBackPosition());
        addChild(_back,10);
        
        //listView
        _listView = ListView::create();
        _listView->setItemsMargin(50);
        _listView->setClippingEnabled(true);
        _listView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
        _listView->setContentSize(Size(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height/2 + 120));
        _listView->setBounceEnabled(false);
        _listView->setAnchorPoint(Vec2(0.5, 0.5));
        _listView->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,Director::getInstance()->getWinSize().height/2));
        _listView->setTouchEnabled(true);
//        _listView->setScrollBarEnabled(false);
        addChild(_listView,10);
        
        auto layout = ui::Layout::create();
        layout->setContentSize(Size(1,385));
        _listView->pushBackCustomItem(layout);
        
        reload();
        
        layout = ui::Layout::create();
        layout->setContentSize(Size(1,385));
        _listView->pushBackCustomItem(layout);

        xAds->setVisiable(kTypeBannerAds, true);
        
        Button * restore = Button::create("choose/restore.png");
        restore->setName("restore");
        restore->setPosition(Vec2(_listView->getPosition().x, (_listView->getPosition().y - _listView->getContentSize().height/2 - 110)/2 + 110));
        restore->setPressedActionEnabled(true);
        restore->setTouchEnabled(true);
        restore->addTouchEventListener(CC_CALLBACK_2(PackScene::touchDown, this));
        addChild(restore, 1);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PackScene::updatePacks(int tag)
{    
    auto sp = _listView->getItem(3 + tag + 1)->getChildByName("PurchasePack");
    if(sp)
    {
        sp->removeFromParent();
    }
}

void PackScene::turnToPage(int pageIndex)
{
    _pageControl->setSelectPage(pageIndex);
    _pageController->turnToPage(pageIndex, false);
}

#pragma mark- Action methods

void PackScene::backAction()
{
    _block = true;

    auto scene = HomeScene::create();
    scene->beBack();
    Director::getInstance()->replaceScene(scene);
}

void PackScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();

    if (!_beBack) {
        if (!xApp->isFirstLaunchApp())
        {
            xApp->requestCrossAd(this, 101);
        }
        else
        {
            xApp->setIsFirstLaunchApp(false);
            xAds->showAds(kTypeBannerAds);
        }
    }
}

void PackScene::touchDown(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    if (ui::Widget::TouchEventType::ENDED != type || _block) return;
    
    auto name = ((Node*)pSender)->getName();
    
    if (name == "restore") {
        xLock->restore();
    }
    else
    {
        _back->setTouchEnabled(false);
        backAction();
    }
}

void PackScene::onExit()
{
    Scene::onExit();
}

void PackScene::chooseLocalPack(const int index)
{
    PackInfo *pack = PackManager::getInstance()->getPackInfo(index);
    
    //if is server pack, check if is paid pack
    std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(pack);
    
    if (!purchaseIdentifier.empty())
    {
        if (xLock->isLocked(purchaseIdentifier))
        {
            if(!_isRewardAd)
            {
                xLock->purchase(xPack->getPackPurchasedIdentifier(index));
            }
            else
            {
                setName("PackScene");
                
                if (getChildByName("viewadslayer") == nullptr) {
                    xDialog->show("text1", "buy", "play_now", CC_CALLBACK_1(PackScene::onDialog, this));
                    xDialog->setUserData(index - 3);
                }
            }
            return;
        }
    }
    
    if (!_hasChoosed)
    {
        auto scene = PictureScene::createSceneWithPackIndex(index);
        Director::getInstance()->replaceScene(scene);
        _hasChoosed = true;
    }
}

void PackScene::onDialog(const string& name)
{
    if (name == "buy") {
        xLock->purchase(xPack->getPackPurchasedIdentifier(_index));
    }
    else if (name == "play_now")
    {
        if (getNetWorkState()) {
            xLock->rewardAd(xDialog->getUserData());
        }
        else
        {
            auto tryLater = TryLaterLayer::create();
            addChild(tryLater, 10);
        }
    }
}

void PackScene::reload()
{
    for (int index = 0; index < 5; index++) {
        PackInfo *pack = PackManager::getInstance()->getPackInfo(index);
        createLocalPackUI(pack, index);
    }
}

void PackScene::createLocalPackUI(PackInfo* pack, const int index)
{
    float interval = getContentSize().width / 3;
    float offsetX = 0;
    
    std::string logoPath = pack->logoName;
    
    //remove the last one
    removeChildByTag(index);
    
    //create logo sprite
    Sprite *logo = Sprite::create(logoPath);
    CCASSERT(logo, "Create pack logo failed!");
    logo->setTag(index);
    offsetX = ((interval - logo->getContentSize().width)/2) * (2 * index + 1) + logo->getContentSize().width * index;
    //logo->setPosition(Vec2(offsetX + logo->getContentSize().width/2, getContentSize().height/2));
    
    auto layout = Layout::create();
    layout->setContentSize(logo->getContentSize());
    layout->setTag(index);
    layout->setTouchEnabled(true);
    layout->addTouchEventListener(CC_CALLBACK_2(PackScene::selecteItemEvent, this));
    layout->addChild(logo);
    logo->setPosition(layout->getContentSize()/2);
    
    //显示图案
    auto path = xPack->getPackPicturePath(pack->packId, quickRandom(1, 6), false);
    auto thumbnail = Sprite::create(path, Rect(0, 0, 1536, 1775));
    thumbnail->setPosition(logo->getContentSize()/2);
    logo->addChild(thumbnail);
    
    float scaleX = (logo->getContentSize().width) / thumbnail->getContentSize().width;
    float scaleY = (logo->getContentSize().height) / thumbnail->getContentSize().height;
    float scale = scaleX > scaleY ? scaleY : scaleX;
    thumbnail->setScale(scale * 0.85);
    
    //6+ icon, stupid!!!!!
    Sprite *icon = Sprite::create("pack/ui03_icon_crayons.png");
    CCASSERT(icon, "Create pack 6+ icon failed!");
    icon->setPosition(Vec2(logo->getContentSize().width - 50, logo->getContentSize().height - 40));
    logo->addChild(icon);
    
    //check if paid pack?
    std::string purchaseIdentifier = PackManager::getInstance()->getPackPurchasedIdentifier(pack);
    
    //this is purchase pack
    if (!purchaseIdentifier.empty()) {
        
        if (xLock->isLocked(purchaseIdentifier)) {
            
            string filename = "game/lock_pack/IAP_" ;
            filename = _isRewardAd? filename+ "1.png" : filename + "2.png";
            
            auto purchase = ImageView::create(filename);
            purchase->setPosition(Vec2(layout->getContentSize().width/2,
                                       layout->getContentSize().height/3));
            purchase->setName("PurchasePack");
            layout->addChild(purchase);
        }
    }
    
    _listView->pushBackCustomItem(layout);
}

void PackScene::selecteItemEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(ui::Widget::TouchEventType::ENDED == type)
    {
        auto target = static_cast<Widget*>(pSender);
        _index = target->getTag();
        CCLOG("select child end index = %d", _index);
        chooseLocalPack(_index);
    }
}

void PackScene::networkChanged()
{    
    //只处理从"有网"变成"没有网络"
    runAction(CallFuncN::create([&](Node * node){
        if(xLock->getNetworkState() && getNetWorkState() == false)
        {
            _isRewardAd = false;
            for(int index = 4; index < 6; index++){
                auto lock = (ImageView*)(_listView->getItem(index)->getChildByName("PurchasePack"));
                if(lock != NULL)
                {
                    lock->loadTexture("game/lock_pack/IAP_2.png");
                }
            }
        }
    }));
}

bool PackScene::getNetWorkState() {
    STSystemFunction sf;
    return sf.checkNetworkAvailable();
}

