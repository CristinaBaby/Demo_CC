//
//  ShopLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-3-4.
//
//

#include "ShopLayer.h"
#include "TweenScrollView.h"
#include "KSVisibleRect.h"
#include "Config.h"
#include "IAPManager.h"
Scene* ShopLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(ShopLayer::create());
    return pScene;
}


bool ShopLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    BaseLayer::onEnter();
//    AdvertiseAdapter::hideBannerAd();
    
    this -> setBackground("shop/shop_bg.png");
    
    backBtn_ -> setVisible(true);
    backBtn_ -> loadTextureNormal("shop/btn_close.png");
    backBtn_ -> setAnchorPoint(Vec2(1, 1));
    backBtn_ -> setPosition(KSVisibleRect::getPosition(10, 10,kBorderRight,kBorderTop));

    auto title = Sprite::create("shop/btn_shop.png");
    title -> setAnchorPoint(Vec2(0.5, 1));
    title -> setPosition(KSVisibleRect::getPosition(320, 10,kBorderNone,kBorderTop));
    addChild(title,2);

    
    //    Vector<__String* > vec_Iap;
//    for(int i = 0 ;i<4;i++)
//    {
//        __String* str = __String::createWithFormat("iap/shop_%d.png",i);
//        vec_Iap.pushBack(str);
//    }
    shouldShowCross_ = false;
    
//    TweenScrollView* scroll = TweenScrollView::createScrollViewWithVec(vec_Iap);
//    scroll -> setPosition(KSVisibleRect::getPosition(-40, 0));
//    scroll -> setDelegate(this);
//    this -> addChild(scroll);
    
//    buyBtn_  = ui::Button::create("start/btn_buy.png");
//    buyBtn_ -> setPosition(KSVisibleRect::getPosition(470-25, 310));
//    buyBtn_ -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
//    this -> addChild(buyBtn_,999,kBtnIap);
    
    
//    ShopSelView* scroll = ShopSelView::create();
//    scroll -> setPosition(KSVisibleRect::getPosition(0, 0));
//    this -> addChild(scroll,2);
//    scroll -> setDelegate(this);
    
    ui::Button* masterBtn  = ui::Button::create("shop/btn_masterpack.png");
    masterBtn -> setPosition(KSVisibleRect::getPosition(320, 746,kBorderNone,kBorderNone));
    masterBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    masterBtn -> setPressedActionEnabled(false);
    this -> addChild(masterBtn,999,0);

    ui::Button* decorationBtn  = ui::Button::create("shop/btn_decoration-pack.png");
    decorationBtn -> setAnchorPoint(Vec2(0, 0));
    decorationBtn -> setPosition(KSVisibleRect::getPosition(20, 280+KSVisibleRect::getDesignOffset().y,kBorderLeft,kBorderBottom));
    decorationBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    this -> addChild(decorationBtn,999,1);
    decorationBtn -> setScale(0.85);
    
    ui::Button* flavorBtn  = ui::Button::create("shop/btn_moreflavors.png");
    flavorBtn -> setAnchorPoint(Vec2(1, 0));
    flavorBtn -> setPosition(KSVisibleRect::getPosition(20, 280+KSVisibleRect::getDesignOffset().y,kBorderRight,kBorderBottom));
    flavorBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    this -> addChild(flavorBtn,999,2);
    flavorBtn -> setScale(0.85);

    ui::Button* noadsBtn  = ui::Button::create("shop/btn_noads.png");
    noadsBtn -> setAnchorPoint(Vec2(0, 0));
    noadsBtn -> setPosition(KSVisibleRect::getPosition(20, 40+KSVisibleRect::getDesignOffset().y/2,kBorderLeft,kBorderBottom));
    noadsBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    this -> addChild(noadsBtn,999,3);
    noadsBtn -> setScale(0.85);

    ui::Button* restoreBtn  = ui::Button::create("shop/btn_restore.png");
    restoreBtn -> setAnchorPoint(Vec2(1, 0));
    restoreBtn -> setPosition(KSVisibleRect::getPosition(20, 40+KSVisibleRect::getDesignOffset().y/2,kBorderRight,kBorderBottom));
    restoreBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    this -> addChild(restoreBtn,999,kBtnRestore);
    restoreBtn -> setScale(0.85);

    return true;
}

void ShopLayer::unitSelected(int idx)
{
    
    
//    IAPManager::getInstance() -> UnlockItem(idx);

}


//void ShopLayer::scrollEndWithIdx(int idx)
//{
//    buyBtn_ -> setVisible(true);
//    curIdx_ = idx;
//}
//
//void ShopLayer::scrollBegan()
//{
//    buyBtn_ -> setVisible(false);
//
//}

void ShopLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            
            switch (btn -> getTag())
            {
                case kBtnIap:
                {
//                    IAPManager::getInstance() -> purchase(curIdx_);
                    break;
                }
                case kBtnRestore:
                {
//                    IAPManager::getInstance() -> restore();
                    break;
                }
                    
                default:
                    this -> unitSelected(btn -> getTag());
                    break;
            }
            break;
        case Widget::TouchEventType::CANCELED:

            break;
    }
}



