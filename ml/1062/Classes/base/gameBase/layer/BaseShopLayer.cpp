//
//  BaseShopLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/30.
//
//

#include "BaseShopLayer.h"
#include "MyDialog.h"
BaseShopLayer::BaseShopLayer():
_callBack(nullptr)
,_restoreBtn("btn_restore")
,closeBtn("btn_close")
,_showAction("show")
,closeAction("out"){
    IAPManager::getInstance()->m_fnAfterPurchase = CC_CALLBACK_1(BaseShopLayer::purchaseCall, this);
    IAPManager::getInstance()->m_fnAfterRestore = CC_CALLBACK_2(BaseShopLayer::restoreCall, this);
    setLocalZOrder(500);
}
BaseShopLayer::~BaseShopLayer(){
    
}

bool BaseShopLayer::onBack(){
    close();
    return true;
}

void BaseShopLayer::touchEnd(ui::Widget* widget){
    auto _name = widget->getName();
    if(_name.compare(closeBtn) == 0)
        close();
    else if(_name.compare(_restoreBtn) == 0)
        IAPManager::getInstance()->restore();
    else {
        for(int i=0;i<_packgeName.size();i++){
            if(_name.compare(_packgeName.at(i)) == 0){
                purchse(i+1);
                break;
            }
        }
    }
}

void  BaseShopLayer::close(){
    if(_operate != nullptr){
        _operate->setIsChange(true);
        if(_operate->getAction()->IsAnimationInfoExists(closeAction)){
            _operate->playAction(closeAction,[&](){
                _operate->setIsChange(false);
                this->removeFromParent();
            });
        } else
            this->removeFromParent();
    }
}

void BaseShopLayer::onEnterTransitionDidFinish(){
    BaseStepLayer::onEnterTransitionDidFinish();
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    if(_operate != nullptr&&_operate->getAction()->IsAnimationInfoExists(_showAction)){
        _operate->setIsChange(true);
        _operate->playAction(_showAction,[&](){
            _operate->setIsChange(false);
        });
    }
}
void BaseShopLayer::onExitTransitionDidStart(){
    BaseStepLayer::onExitTransitionDidStart();
    if(_callBack != nullptr){
        _callBack();
        if (IAPManager::getInstance()->isShowAds()) {
            AdsManager::getInstance()->showAds(kTypeBannerAds);
        }
    }
}

void BaseShopLayer::restoreCall(bool isScucess, bool isHasData){
    if(isScucess && isHasData){
        auto dial = MyDialog::create("restore successfully!", MyDialog::oneBtnPath);
        getScene()->addChild(dial);
        if(!IAPManager::getInstance()->isShowAds()){
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    }
}

void BaseShopLayer::purchaseCall(bool flag){
    if(flag){
        if(!IAPManager::getInstance()->isShowAds()){
            AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
        }
    }
}

 void BaseShopLayer::purchse(int index){
     if(IAPManager::getInstance()->isPackagePurchased(index)){
         auto dia = MyDialog::create("you have already purchased this package!", MyDialog::oneBtnPath);
         getScene()->addChild(dia);
     }else
         IAPManager::getInstance()->purchase(index);
}