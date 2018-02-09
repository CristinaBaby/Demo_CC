//
//  MakeLayer.h
//  Make
//
//  Created by QAMAC01 on 15-2-12.
//
//

#ifndef __Make__MakeLayer__
#define __Make__MakeLayer__

#include <stdio.h>
#include "cocos2d.h"
//#include "EatNode.h"
#include "EatScribbleView.h"
#include "CandyScribbleView.h"
#include "Dialog.h"
#include "DialogCallback.h"
#include "FillMaterialModel.h"

USING_NS_CC;
class MakeLayerDelegate
{
public:
    virtual void unitUpdated(){};
    
};

class MakeLayer : public Layer , public EatScribbleViewDelegate,public DialogCallback
{
public:
    CREATE_FUNC(MakeLayer);
    bool init();
    void showDefaultDecorationUnit();
    void updateUnit(std::string unitImageName);
    void updateUnit(int idx);
    void reset();
    void resetScribble();
    void inDecLayer();
    void inShareLayer();
    void backToDecLayer();
    void unitShow();
    void setDelegate(MakeLayerDelegate* delegate){delegate_ = delegate;};
    void scribbleFinished();
    //share中重置snowcone
    void resetSnowconeInshare();
    
    //保存用于拍照
    void screenShotForShare();

    //禁止点击生成精灵
    void stopGenerate();

    
private:
    CC_SYNTHESIZE(EatScribbleView*, eatLayer_,EatLayer);
    CC_SYNTHESIZE(Layer*, plateLayer_,PlateLayer);
//    CC_SYNTHESIZE(EatScribbleView*, defaultFoodLayer_,DefaultFoodLayer);
    
    CC_SYNTHESIZE(Sprite*, background_,Background);
    CC_SYNTHESIZE(Sprite*, spoon_,Spoon);
    CC_SYNTHESIZE(Sprite*, extras_,Extras);


    float _yDistance = 0.f;
    
    Sprite* snowcone_;

    Vector<FillMaterialModel*> vec_movealbleItem;
    
    EventListenerTouchOneByOne* listener;
    
    //判定是否在share页面进行点击事件
    bool isInShare_ = false;
    MakeLayerDelegate* delegate_;
    
    Vector<SpriteFrame*> vec_stickerFrame;
    
    CandyScribbleView* scribbleLayer_;
    Sprite* cup_;
    //用于播放动画的cup
    Sprite* cupCover_;

    //通过点击位置生成
    std::string tapFoodString;
private:
    virtual void scribbleCleared();

    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){};

    void showDefaultBackground();
    void showDefaultUnit();
    
    void updateFood(std::string fullPath);
    void updateSpoon(std::string fullPath);
    void updateExtra(std::string fullPath);
    void updateCup(std::string unitImageName);
    //根据点击位置出现
    void generateFood(std::string unitImageName);

    void allowEat();
    void resetEatNode();
    void forbidMove();
    void allowMove();
    
    //从share返回后重置状态
    void resetDecStatus();
        
    //只保存scribble的截图
    void scribbleScreenShot();
    
    


};

#endif /* defined(__Make__MakeLayer__) */
