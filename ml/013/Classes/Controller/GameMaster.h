//
//  GameMaster.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-7-31.
//
//

#ifndef __MakeSnowMan__GameMaster__
#define __MakeSnowMan__GameMaster__

#include <iostream>
#include "cocos2d.h"
#include "MakeLayer.h"
USING_NS_CC;
class GameMaster
{
public:
    static GameMaster* getInstance();
    void init();
    void goToLoadingPage();
    void goToHomeLayer();
    void goToFavLayer();
    Node* goToShopLayer(Layer* );
  
    void goToMiniGameLayer();
    void goToMakeIceLayer();
    void goToChooseCupLayer();
    void goToGetSnowLayer();
    void goToShaveIceLayer();
    void goToChooseShapeLayer();
    void goToScribbleLayer();
    void goToDecorationLayer();
    void goToShareLayer();


    void goToMoreGame();
    void showBannerAds();

//    void goToPrivacy();
    
private:
    void goToScene(Scene *pScene);
    
private:

    CC_SYNTHESIZE(int, flavor_, Flavor);
    CC_SYNTHESIZE_RETAIN(__String*, cupName_, CupName );
    CC_SYNTHESIZE(std::string, shape_, Shape );
    CC_SYNTHESIZE(int, shapeIdx_, ShapeIdx);
    CC_SYNTHESIZE(bool, hidebanner_, Hidebanner);

    CC_SYNTHESIZE(RenderTexture*, renderTxt_, RenderTxt);
    CC_SYNTHESIZE(MakeLayer*, makeNode_, MakeNode);
};

#endif /* defined(__MakeSnowMan__GameMaster__) */
