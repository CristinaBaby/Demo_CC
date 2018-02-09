//
//  ShopLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/30.
//
//

#include "ShopLayer.hpp"
#include "EventPromotLayer.h"
IMPLEMENT_COCOS2DX_CLASS(ShopLayer);

ShopLayer::ShopLayer(){
    for(int i=0;i<=2;i++)
        _packgeName.push_back(StringUtils::format("%d",i));
 
}

bool ShopLayer::init(){
    if(BaseShopLayer::init()){
        _operate = StudioLayer::create("ShopLayer.csb");
        addChild(_operate);
        auto _EventPromotLayer = EventPromotLayer::create();
        _EventPromotLayer->setCallbakc([](){});
        addChild(_EventPromotLayer,50);
      auto  _light = _operate->getNodeByName("light");
        _light->setScale(0,1);
        auto _logo = _operate->getNodeByName("logo");
        if(_logo != nullptr)
            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            ActionHelper::showBackInOut(_logo, _logo->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                 SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                auto _dec = _operate->getNodeByName("decorations");
                ActionHelper::showBackInOut(_dec, _dec->getPosition(), ActionHelper::ShowDirection::show_from_left);
                _dec->setVisible(true);
                auto _noAds = _operate->getNodeByName("noAds");
                ActionHelper::showBackInOut(_noAds, _noAds->getPosition(), ActionHelper::ShowDirection::show_from_right,[this](){
                      SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                    auto buyAll = _operate->getNodeByName("buyAll");
                    ActionHelper::showBackInOut(buyAll, buyAll->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                        auto  _light = _operate->getNodeByName("light");
                         _light->runAction(ScaleTo::create(0.7, 1,1));
                    });
                    buyAll->setVisible(true);
                });
                _noAds->setVisible(true);
            });
        return true;
    }
    return false;
}






