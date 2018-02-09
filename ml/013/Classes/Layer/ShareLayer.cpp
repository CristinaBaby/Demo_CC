//
//  ShareLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#include "ShareLayer.h"
#include "KSVisibleRect.h"
#include "GameMaster.h"
#include "TypeSelView.h"
#include "UnitSelView.h"
#include "MakeConfig.h"
#include "FavManager.h"
#include "IAPManager.h"
#include "Config.h"
#include "AdsLoadingLayer.h"
Sprite* topView_;

Scene* ShareLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ShareLayer::create());
    return pScene;
}

bool ShareLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    
    log("1makeNode_ -> getReferenceCount() %d",GameMaster::getInstance() -> getMakeNode() -> getReferenceCount());

//    moreBtnInShare_ -> setVisible(true);
    
    makeLayer_ = dynamic_cast<MakeLayer*>(GameMaster::getInstance() -> getMakeNode()) ;
    if(makeLayer_ -> getParent())
        makeLayer_ -> removeFromParent();
    this -> addChild(makeLayer_);
    
    makeLayer_ ->  inShareLayer();

    
    topView_ = Sprite::create("button/tte.png");
    topView_ -> setPosition(KSVisibleRect::getPosition(320, 800));
    this -> addChild(topView_,2);

    
    this -> setTag(kTagReady);
 
    listener_ -> setSwallowTouches(false);
    
    listener_ -> onTouchBegan = [&](Touch* touch, Event* event)
    {
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
            if (target ->getTag() != kTagNone)
            {
                return true;
            }
        }
        return false;
    };
    
    
    listener_ -> onTouchEnded = [&](Touch* touch, Event* event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标

        if (target -> getTag() == kTagReady)
        {
            target -> setTag(kTagNone);
            
            if (topView_)
            {
                topView_ -> runAction(MoveBy::create(2, Vec2(-800, 0)));
            }
        }
    };
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_ -> clone(), this);

    newOneBtn_ -> setAnchorPoint(Vec2(1, 0));
    newOneBtn_ -> setPosition(KSVisibleRect::getPosition(50, 100,kBorderRight,kBorderBottom));
    
    favBtn_ -> setAnchorPoint(Vec2(0.5, 0));
    favBtn_ -> setPosition(KSVisibleRect::getPosition(320, 100,kBorderNone,kBorderBottom));

    backBtn_ -> setAnchorPoint(Vec2(0, 0));
    backBtn_ -> setPosition(KSVisibleRect::getPosition(50, 100,kBorderLeft,kBorderBottom));
    backBtn_ -> loadTextureNormal("button/btn_back_share.png");
    
    _reEatBtn = ui::Button::create("button/eat-again.png");
    _reEatBtn -> setAnchorPoint(Vec2(1, 1));
    _reEatBtn -> setPosition(KSVisibleRect::getPosition(30, 30,kBorderRight,kBorderTop));
    this -> addChild(_reEatBtn,10,kBtnReEat);
    _reEatBtn -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent,this));
    _reEatBtn -> setVisible(false);
    return true;
}

void ShareLayer::reeatClicked()
{
    BaseLayer::reeatClicked();
    
    makeLayer_ -> resetSnowconeInshare();
    _reEatBtn -> setVisible(false);
}

void ShareLayer::onEnterTransitionDidFinish()
{
    
    Layer::onEnterTransitionDidFinish();
    backBtn_ -> setVisible(true);
    favBtn_ -> setVisible(true);
    newOneBtn_ -> setVisible(true);
    
    __NotificationCenter::getInstance() -> addObserver(this, callfuncO_selector(ShareLayer::showEatAgain), "BeginEat", nullptr);
}

void ShareLayer::showEatAgain(Ref* obj)
{
    _reEatBtn -> setVisible(true);
}


void ShareLayer::favBtnClicked()
{
//    for (int i=0 ; i<10; i++)
//    {
//        FavManager::getInstance() -> addFavImage(screenShot());
//    }
    FavManager::getInstance() -> addFavImage(screenShot());
    
    Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogConfirm1, Dialog::DIALOG_TYPE_SINGLE);
    dialog->setContentText("Your Snow Cone has been added to your favorite!");
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    dialog -> setAnchorPoint(Vec2(0,0));
    dialog -> setPosition(KSVisibleRect::getPosition(0,0, kBorderLeft ,kBorderBottom));
    addChild(dialog, 9999,129);

}

RenderTexture* ShareLayer::screenShot()
{

    auto size = Director::getInstance() -> getVisibleSize();
    
    //clippingnode screenshot!!!
    RenderTexture* rtx = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    Scene* curScene = Director::getInstance()->getRunningScene();
    Point ancPos = curScene->getAnchorPoint();

    rtx -> begin();
//    makeLayer_ -> setScale(0.2f);
//    makeLayer_->setAnchorPoint(cocos2d::Point(0, 0));
    makeLayer_ -> visit();
    rtx -> end();
    Director::getInstance()->getRenderer()->render();
//    rtx->getSprite()->getTexture()->setAntiAliasTexParameters();
    rtx -> saveToFile("forShare");
//    makeLayer_ -> setScale(1.f);
    makeLayer_->setAnchorPoint(ancPos);
    
    return rtx;
}


void ShareLayer::newOneBtnClicked()
{
    BaseLayer::newOneBtnClicked();
    
    GameMaster::getInstance() -> getMakeNode() -> removeFromParent();
    GameMaster::getInstance() -> setMakeNode(nullptr);
    
    if (IAPManager::getInstance()->isShowAds())
    {
        AdsLoadingLayer::showLoading<AdsLoadingLayer>(true);
        AdsLoadingLayer::loadingDoneCallback =[=]
        {
            GameMaster::getInstance() -> goToHomeLayer();
        };
    }
    else
    {
        GameMaster::getInstance() -> goToHomeLayer();
    }
    
    makeLayer_  -> removeFromParent(); makeLayer_ = nullptr;
    
}

void ShareLayer::onExit()
{
    BaseLayer::onExit();
    __NotificationCenter::getInstance() -> removeObserver(this, "BeginEat");
}
