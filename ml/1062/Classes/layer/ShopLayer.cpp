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
    for(int i=0;i<=3;i++)
        _packgeName.push_back(StringUtils::format("%d",i));
 
}

bool ShopLayer::init(){
    if(BaseShopLayer::init()){
        _operate = StudioLayer::create("ShopLayer.csb");
        addChild(_operate);
        for(auto name:_packgeName)
            _operate->getNodeByName(name)->setVisible(false);
        for(int i=0;i<=5;i++) {
            auto particle = _operate->getNodeByname<ParticleSystemQuad*>(StringUtils::format("particle_%d",i));
            particle->setTexture(Director::getInstance()->getTextureCache()->addImage(StringUtils::format("png/shop/fireworks%d.png",i)));
            particle->stopSystem();
        }
        auto _EventPromotLayer = EventPromotLayer::create();
        _EventPromotLayer->setCallbakc([](){});
        addChild(_EventPromotLayer,50);
        auto _logo = _operate->getNodeByName("logo");
        if(_logo != nullptr)
            SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
            ActionHelper::showBackInOut(_logo, _logo->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                 SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                auto _dec = _operate->getNodeByName("2");
                ActionHelper::showBackInOut(_dec, _dec->getPosition(), ActionHelper::ShowDirection::show_from_left);
                _dec->setVisible(true);
                auto _noAds = _operate->getNodeByName("1");
                ActionHelper::showBackInOut(_noAds, _noAds->getPosition(), ActionHelper::ShowDirection::show_from_right,[this](){
                      SoundPlayer::getInstance()->playEffect("sound/general/ingredients_fly_in.mp3");
                    auto noAds = _operate->getNodeByName("3");
                    ActionHelper::showBackInOut(noAds, noAds->getPosition(), ActionHelper::ShowDirection::show_from_bottom);
                    noAds->setVisible(true);
                    _operate->getNodeByName("Sprite_1")->runAction(RepeatForever::create(Sequence::create(RotateTo::create(1, -30.45*2),RotateTo::create(1, 0), nullptr)));
                    
                    auto buyAll = _operate->getNodeByName("0");
                    ActionHelper::showBackInOut(buyAll, buyAll->getPosition(), ActionHelper::ShowDirection::show_from_top,[this](){
                        for(int i=0;i<=5;i++) {
                            auto particle = _operate->getNodeByname<ParticleSystemQuad*>(StringUtils::format("particle_%d",i));
                            particle->resetSystem();
                        }
                        ActionHelper::delayFunc(2, this, [this](){
                            for(int i=0;i<=5;i++) {
                                auto particle = _operate->getNodeByname<ParticleSystemQuad*>(StringUtils::format("particle_%d",i));
                                particle->stopSystem();
                            }
                        });
                    });
                    buyAll->setVisible(true);
                });
                _noAds->setVisible(true);
            });
        return true;
    }
    return false;
}






