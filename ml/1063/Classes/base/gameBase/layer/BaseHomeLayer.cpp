//
//  BaseHomeLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#include "BaseHomeLayer.h"
#include "MyDialog.h"
#include "Analytics.h"
#include "AdLoadingLayerBase.h"
BaseHomeLayer::BaseHomeLayer():
_shopClass("ShopLayer")
,startClass("")
{
    startEffect = "sound/start.mp3";
}

bool BaseHomeLayer::onBack(){
    auto dialog = MyDialog::create("Are you sure you want to exit this game?", MyDialog::twoBtnPath);
    dialog->dialogBtnClick = [&](MyDialog * dialog,int index){
        if(index == 0) {
            Analytics::getInstance()->endSession();
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([]{
                std::exit(0);
            }),nullptr));
        }
    };
    getScene()->addChild(dialog);
    return true;
}

void BaseHomeLayer::touchEnd(ui::Widget* widget){
    if(widget->getName().compare("btn_more") == 0){
        _cfsys.showMoreGame();
    }
    else if(widget->getName().compare("btn_shop") == 0){
        auto _shop = dynamic_cast<Node*>(DynObject::createDynObject(_shopClass));
        if(_shop != nullptr)
            getScene()->addChild(_shop);
    } else if(widget->getName().compare("btn_play") == 0)
        playClick();
    else if(widget->getName().compare("btn_rate")==0)
        _cfsys.rateUs();
}

 void BaseHomeLayer::playClick(){
     auto _nextStep = dynamic_cast<BaseStepLayer*>(DynObject::createDynObject(startClass));
     Director::getInstance()->getEventDispatcher()->setEnabled(false);
     if(_nextStep != nullptr)
         _nextStep->retain();
     auto _callfunc = [_nextStep,this](){
         AdLoadingLayerBase::loadingDoneCallback = nullptr;
         SoundPlayer::getInstance()->playEffect(startEffect);
         if(_nextStep != nullptr){
             auto _scne = Scene::create();
             _scne->addChild(_nextStep);
             Director::getInstance()->replaceScene(_scne);
             _nextStep->release();
         }
         Director::getInstance()->getEventDispatcher()->setEnabled(true);
     };
     if(IAPManager::getInstance()->isShowAds()){
         AdLoadingLayerBase::loadingDoneCallback = _callfunc;
         showAdsCallBack();
     } else
         _callfunc();

}

void BaseHomeLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}