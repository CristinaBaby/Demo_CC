//
//  MarketScene.h
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/6.
//
//

#ifndef __MLABS030_IceCreamKids_CookingGames__MarketScene__
#define __MLABS030_IceCreamKids_CookingGames__MarketScene__

#include <stdio.h>
#include "GameBaseScene.h"
#include "DialogCallback.h"
#include "Dialog.h"

using namespace ui;
USING_NS_CC;

//步骤的基本模板
class MarketScene:public GameBaseScene,public ToolSpriteDelegate{
private:
    MarketScene();
    ~MarketScene();
public:
    virtual bool init();
    CREATE_FUNC(MarketScene);
    CREATE_SCENE(MarketScene);
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    virtual bool TouchBegan(Touch *touch, Event *unused_event);
    virtual void TouchEnded(Touch *touch, Event *unused_event);
    
     void touchEvent(Ref *obj , Widget::TouchEventType type);
public:
    void initUI();
    
    int checkTouchRect(Vec2 pos);
    bool checkNeedCartTag(int tag);
    bool checkCartTag(int tag);
    void checkUnTouch(float f);
    void cartAction(int tag,Vec2 pos);
    void addNeedCartTag();
    void showListBg(bool isShowCheck,int tag);
    void hideListBg();
    void showCheckIcon(int tag);
    
   
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    virtual void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
public:
    //下一步
    void nextStep();
public:
    Sprite* m_pBG;
    Sprite* m_pCart;
    
    bool m_bIsTouch;
    bool m_bIsDoubleTouch;
    //背景上的矩形区域
    vector<Rect> m_vRect;
    
    //需要选择的商品tag
    vector<int> m_vCartNeedTag;
    
    //已经选过的商品tag
    vector<int> m_vCartTag;
   
    
//    int m_iCarPosNum;
    
    int m_iCarItemNum;
    
    ToolSprite* m_pListBtn;
    ToolSprite* m_pListClose;
    Sprite* m_pListBg;
    LayerColor* m_pLayerColor;
};



#endif /* defined(__MLABS030_IceCreamKids_CookingGames__MarketScene__) */
