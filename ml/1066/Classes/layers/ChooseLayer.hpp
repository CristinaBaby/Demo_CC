//
//  ChooseLayer.hpp
//  MexicanFood
//
//  Created by huxixiang on 16/3/25.
//
//

#ifndef ChooseLayer_hpp
#define ChooseLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ShapeButton.h"
#include "UnitScrollView.h"
#include "MyPhysicsNode.hpp"
#include "ToolSprite.h"

USING_NS_CC;
class ChooseLayer: public BaseLayer, public ToolSpriteDelegate
{
public:
    ChooseLayer();
    ~ChooseLayer();
    
public:
    static Scene* scene();
    virtual bool init() override;
    CREATE_FUNC(ChooseLayer);
    
    bool initData();
    bool initLayer();
    void initBackground();
   
protected:
    void btnClickCallback(ShapeButton* sb);
    
    void playAnimation(float ft);
    ActionInterval* clickAnimation(Node* node);
    
    void addScrollView();
    void onListItemCallback(int tag,MenuItem* menuItem,int index);
    void onChooseItemCallback(int index, Touch *pTouch, Event *pEvent);
    void onTouchDownCallback(int index, Touch *pTouch, Event *pEvent);
    void onTouchMoveCallback(int index, Touch *pTouch, Event *pEvent);
    void onTouchEndCallback(int index, Touch *pTouch, Event *pEvent);
    
    virtual void update(float ft) override;
    void runHandTipAnimation();
    
    virtual void onNextCallback() override;
    void enterNextScene(float ft);

public:
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch) override;
    
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;

private:
    Layer* m_pUILayer;
    Layer* m_pContentLayer;
    
    Node* m_pBg;
    
    Sprite* m_pWafer;
    
    Layer* m_pShadeLayer;
    
    int m_iCount;
    
    UnitScrollView* m_pListView;

    int m_iCurrtIndex;
    
    int m_iCurrtTag;
    
    Vector<MyPhysicsNode*> m_vContianer;
    
    // 冰淇淋是否加载蛋筒上的判断区域
    Node* m_pResponseArea;

    ToolSprite* m_pNextBtn;
    bool m_bIsFlag;
    
    Vec2 m_oListViewOffset;
    
    ToolSprite* m_pHandTip;
};


#endif /* ChooseLayer_hpp */
