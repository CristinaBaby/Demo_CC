//
//  EatLayer.h
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/29.
//
//

#ifndef __LunchFoodMaker__EatLayer__
#define __LunchFoodMaker__EatLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"

USING_NS_CC;
class EatLayer: public BaseLayer, public ToolSpriteDelegate
{
public:
    EatLayer();
    ~EatLayer();
    
public:
    static Scene* scene();
    virtual bool init();
    CREATE_FUNC(EatLayer);
    
    bool initData();
    bool initUI();
    void initLayer();
    void initBackground();
    
public:
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch);
    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
public:
    void addTouchEatNode();

    void saveToAlbumCallback(bool isSuc);
    
    void addEatAgainBtn();

private:
    Layer* m_pUILayer;
    Layer* m_pContentLayer;
    Layer* m_pStickerLayer;
    
    Node* m_pBg;
};

#endif /* defined(__LunchFoodMaker__EatLayer__) */
