//
//  DecorationLayer.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#include "DecorationLayer.h"
#include "KSVisibleRect.h"
#include "GameMaster.h"
#include "IAPManager.h"
#include "Config.h"
#include "MyMakeConfig.h"
#include "ShopSelView.h"
#include "AdsLoadingLayer.h"
Scene* DecorationLayer::scene()
{
    Scene *pScene = Scene::create();
    pScene -> addChild(DecorationLayer::create());
    return pScene;
}

bool DecorationLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    BaseLayer::onEnter();
    nextBtn_ -> setVisible(true);
    isShopClicked_ = false;
    
    float yDistance = IAPManager::getInstance()->isShowAds()?100:0;
    
    typeScrollView_ = UITpyeSelView::create();
    typeScrollView_ -> setPosition(KSVisibleRect::getPosition(0, yDistance,kBorderNone,kBorderBottom));
    this -> addChild(typeScrollView_,999);
    typeScrollView_ -> typeSelected = CC_CALLBACK_1(DecorationLayer::showUnitView, this);
    typeScrollView_ -> typeUnSelected = CC_CALLBACK_0(DecorationLayer::removeUnitView, this);
    typeScrollView_ -> loadTypeListFromTo(0, 3);
    
    makeLayer_ = MakeLayer::create();
    makeLayer_ -> setPosition(KSVisibleRect::getPosition(0, yDistance));
    addChild(makeLayer_);
    makeLayer_ -> showDefaultDecorationUnit();
    makeLayer_ -> setDelegate(this);
    
    _preBtn = ui::Button::create("button/btn_back.png");
    _preBtn -> setAnchorPoint(Vec2(0, 0.5));
    _preBtn -> setPosition(KSVisibleRect::getPosition(10, 480,kBorderLeft,kBorderNone));
    _preBtn -> addTouchEventListener(CC_CALLBACK_2(BaseLayer::touchEvent, this));
    this -> addChild(_preBtn,1,kBtnPre);
    _preBtn -> setVisible(false);
    return true;
}

void DecorationLayer::preBtnClicked()
{
    BaseLayer::preBtnClicked();
    _preBtn -> setVisible(false);
    typeScrollView_ -> loadTypeListFromTo(0, 3);
    _nextCount = 0;

}


void DecorationLayer::showUnitView(std::string typeName)
{
    if(unitSelView_)
    {
        unitSelView_ -> removeFromParent();
        unitSelView_ = nullptr;
    }
    unitSelView_ = UIUnitScrollView::createWithTypeNameAndColumn(typeName,4);
    unitSelView_ -> setPosition(KSVisibleRect::getPosition(0, 0,kBorderNone,kBorderBottom));
    this -> addChild(unitSelView_,100);
    unitSelView_ -> lockedUnitClicked = CC_CALLBACK_0(DecorationLayer::shopClicked, this);
    unitSelView_ -> freeUnitClicked = CC_CALLBACK_2(DecorationLayer::unitSelected, this);
    
    if (makeLayer_)
    {
        makeLayer_ -> unitShow();
    }
    
//    nextBtn_ -> setVisible(false);
//    resetBtn_ -> setVisible(false);

    
}


void DecorationLayer::removeUnitView()
{
    if(unitSelView_)
    {
        unitSelView_ -> removeFromParent(); unitSelView_ = nullptr;
    }
    
//    if (makeLayer_)
//    {
//        makeLayer_ -> backToDecLayer();
//    }
//    nextBtn_ -> setVisible(true);
//    resetBtn_ -> setVisible(true);

}


void DecorationLayer::shopClicked()
{

    if (isShopClicked_)
    {
        return;
    }
    auto shop = dynamic_cast<ShopSelView*>(GameMaster::getInstance() -> goToShopLayer(this)) ;
    shop -> callback = CC_CALLBACK_0(DecorationLayer::checkUnlock, this);
    
    isShopClicked_ = true;
    
    this -> runAction(Sequence::create(DelayTime::create(1),
                                       CallFunc::create([&]{isShopClicked_ = false;})
                                       , NULL));
}

void DecorationLayer::checkUnlock()
{
    GameMaster::getInstance() -> showBannerAds();

    typeScrollView_ -> setSelectNone();
    this -> removeUnitView();
    float yDistance = IAPManager::getInstance()->isShowAds()?100:0;
    typeScrollView_ -> setPosition(KSVisibleRect::getPosition(0, yDistance,kBorderNone,kBorderBottom));
    makeLayer_ -> setPosition(KSVisibleRect::getPosition(0, yDistance));

}

void DecorationLayer::unitSelected(std::string pUnitImageName,int idx)
{
    
    auto typeName = MyMakeConfig::getInstance() -> typeName_;
    
    if (!strcmp(typeName.c_str(), "syrup"))
    {
        makeLayer_ -> updateUnit(idx);
    }
    else
    {
        makeLayer_ -> updateUnit(pUnitImageName);
    }
    
    this ->removeUnitView();
    
    typeScrollView_ -> setSelectNone();

}

void DecorationLayer::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();
    if(unitSelView_)
    {
        unitSelView_ -> removeFromParent(); unitSelView_ = nullptr;
    }

    nextBtn_ -> setVisible(true);
    _preBtn -> setVisible(true);
    if (_nextCount == 0)
    {
        typeScrollView_ -> loadTypeListFromTo(3, 6);
        _nextCount++;
        
    }
    else
    {
        
        if (IAPManager::getInstance()->isShowAds())
        {
            AdsLoadingLayer::showLoading<AdsLoadingLayer>(true);
            AdsLoadingLayer::loadingDoneCallback =[=]
            {
                GameMaster::getInstance() -> setMakeNode(makeLayer_);
                GameMaster::getInstance() -> goToShareLayer();
            };
        }
        else
        {
            GameMaster::getInstance() -> setMakeNode(makeLayer_);
            GameMaster::getInstance() -> goToShareLayer();
        }

    }
    

    
}



void DecorationLayer::resetBtnClicked()
{
    BaseLayer::resetBtnClicked();
    makeLayer_ -> reset();
    nextBtn_ -> setVisible(true);
}

void DecorationLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if(nextBtn_)
    {
        nextBtn_ -> setVisible(true);
    }
    
    if (makeLayer_)
    {
        makeLayer_ -> retain();
        makeLayer_ -> removeFromParent();
        this -> addChild(makeLayer_,-1);
        makeLayer_ -> backToDecLayer();
    }
    
    resetBtn_ -> setVisible(true);
    
    if (unitSelView_)
    {
        unitSelView_ -> checkUnlock();
    }
}

void DecorationLayer::onExit()
{
    BaseLayer::onExit();
}
