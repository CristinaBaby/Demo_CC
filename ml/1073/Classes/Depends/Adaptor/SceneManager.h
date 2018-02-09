
#ifndef __SceneManager__
#define __SceneManager__

#include <iostream>
#include "cocos2d.h"

//要用的Scene头文件
#include "HomeScene.h"

#include "DecorateScene.h"
#include "ShareScene.h"
#include "ShopScene.h"
#include "LoadingScene.h"
#include "ChooseScene.h"

#include "SnowconeDisplayIngridentScene.h"
#include "SnowconeCrushIceScene.h"
#include "SnowconeAddSlushScene.h"
#include "SnowconeShapScene.h"
#include "SnowconeSculptScene.h"
#include "SnowconeColorScene.h"
#include "SnowconeEatScene.h"

#include "CottonDisplayIngridentScene.h"
#include "CottonChooseFlavorScene.h"
#include "CottonMakeScene.h"
#include "CottonChooseShapeScene.h"

#include "FunnelDisplayIngridentScene.h"
#include "FunnelMixSecne.h"
#include "FunnelPourScene.h"
#include "FunnelFryScene.h"
#include "FunnelDecorateScene.h"
#include "FunnelEatScene.h"

USING_NS_CC;

enum{
    eFade, //出去的场景淡出，屏幕变黑，进来的场淡入
    ePageForward, //向前翻页效果
    ePageBackward,  //向后翻页效果
    eSplitRows, //奇数行向左出，偶数行向右出
    eSplitCols,  //奇数行向下出，偶数行向上出
    eTurnOffTiles, //以随机的顺序关闭掉外出的场景的砖瓦
    eMoveInR, //从左边进入
    eMoveInT, //从上边进入
    eMoveInB,   //从下边进入
    SlideInL,   //从左边滑动着进入，滑动的过程中把第一个场景推出去了
    eSlideInR,   //从右边滑动进入
    eSlideInT,   //从上边滑动进入
    eSlideInB,   //从下边滑动进入
    eJumpZoom,   //跳动加缩放
    e,
};

class SceneManager
{
public:
template<typename T> static void replaceTheScene(bool trans = true,int type=-1) {
    LockUIManager::getInstance()->distroyRegister();
    Scene* targetScene = T::scene();
    int random = type;
    if (type==-1) {
        random = arc4random()%9;
    }
    random = 0;
    if (trans) {
        TransitionScene* pScene= createTransitionScene(targetScene,random);
        if (Director::getInstance()->getRunningScene()) {
            Director::getInstance()->replaceScene(pScene);
        }else {
            Director::getInstance()->runWithScene(pScene);
        }
    }else{
        if (Director::getInstance()->getRunningScene()) {
            Director::getInstance()->replaceScene(targetScene);
        }else {
            Director::getInstance()->runWithScene(targetScene);
        }
    }
}

template<typename T> static void pushTheScene(int type=0) {
    LockUIManager::getInstance()->distroyRegister();
    Scene* targetScene = T::scene();
    TransitionScene* pScene= createTransitionScene(targetScene,type);
    if (Director::getInstance()->getRunningScene()) {
        Director::getInstance()->pushScene(pScene);
    }else {
        Director::getInstance()->runWithScene(pScene);
    }
}
    static void popScene(){
        Director::getInstance()->popScene();
    }
    static void popToRootScene(){
        Director::getInstance()->popToRootScene();
    }
private:
    static TransitionScene* createTransitionScene(Scene* targetScene,int type){
        switch (type) {
            case eFade:
                return TransitionFade::create(1, targetScene,Color3B::WHITE);
                break;
            case ePageForward:
                return TransitionPageTurn::create(1, targetScene,false);
                break;
            case ePageBackward:
                return TransitionPageTurn::create(1, targetScene,true);
                break;
            case eSplitRows:
                return TransitionSplitRows::create(1, targetScene);
                break;
                
            case eSplitCols:
                return TransitionSplitCols::create(1, targetScene);
                break;
                
            case eTurnOffTiles:
                return TransitionTurnOffTiles::create(1, targetScene);
                break;
                
            case eMoveInR:
                return TransitionMoveInR::create(1, targetScene);
                break;
            case eMoveInT:
                return TransitionMoveInT::create(1, targetScene);
                break;
            case eMoveInB:
                return TransitionMoveInB::create(1, targetScene);
                break;
                
            default:
                return TransitionFade::create(1, targetScene);
                break;
        }
    }
};
#endif /* defined(__SceneManager__) */
