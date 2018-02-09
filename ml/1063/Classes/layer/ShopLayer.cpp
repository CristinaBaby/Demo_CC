//
//  ShopLayer.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/30.
//
//

#include "ShopLayer.h"
#include "MainScene.h"
IMPLEMENT_COCOS2DX_CLASS(ShopLayer);
REGIST_SECENE(ShopLayer);
ShopLayer::ShopLayer(){
    for(int i=0;i<=3;i++)
        _packgeName.push_back(StringUtils::format("%d",i));
 
}

bool ShopLayer::init(){
    if(BaseShopLayer::init()){
        SoundPlayer::getInstance()->playEffect("sound/ingredients_fly_in.mp3",0.6);
        _operate = StudioLayer::create("ShopLayer.csb");
        addChild(_operate);
        _operate->getNodeByName<ui::ScrollView*>("ScrollView_2")->setScrollBarOpacity(0);
        return true;
    }
    return false;
}






