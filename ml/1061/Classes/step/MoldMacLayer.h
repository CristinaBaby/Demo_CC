//
//  MoldMacLayer.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 16/1/15.
//
//

#ifndef MoldMacLayer_hpp
#define MoldMacLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MySpriteButton.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "AdapterScreen.h"
#include "ActionUtils.h"
#include "ToolSprite.h"
#include "Global.h"
#include "MySpriteButton.h"

USING_NS_CC;
using namespace std;

class MoldMacLayer : public LayerColor,public ToolSpriteDelegate{
public:
    MoldMacLayer();
    ~MoldMacLayer();
    
    virtual bool init();
    CREATE_FUNC(MoldMacLayer);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    
    bool touchBegan(Touch *pTouch, Event *pEvent);
    void clickRight();
    void clickLeft();
    
    void onTouchDown(ToolSprite* toolSprite,Touch *pTouch);
    void onTouchUp(ToolSprite* toolSprite,Touch *pTouch);
public:
    std::function<void(int index)> pushBtnDown = nullptr;
    std::function<void()> pushBtnUp = nullptr;
    std::function<void()> firstBtnDown = nullptr;
    void hideMacCenter();
    void showMacCenter();
    
    void setMoldType(int i);
    void setIsOver();
private:
    Vector<Sprite*> m_vMacData;
    
    float m_fOffset;
    Size _visbileSize;
    bool m_bIsTouch;
    bool m_bIsFirst;
    Sprite* m_pMacCenter;
    ToolSprite* m_pMacPushBtn;
    int m_iMoldType;
    int m_iMoldShape;
    int m_bIsOver;
};

#endif /* MoldMacLayer_hpp */
