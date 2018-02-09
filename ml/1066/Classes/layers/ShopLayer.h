//
//  ShopLayer.h
//  FastFood
//
//  Created by huxixiang on 15/9/2.
//
//

#ifndef __FastFood__ShopLayer__
#define __FastFood__ShopLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"

USING_NS_CC;

class ShopLayer : public Layer, public ToolSpriteDelegate, public DialogCallback{
    
public:
    ShopLayer();
    ~ShopLayer();
    
    static Scene* scene();
    CREATE_FUNC(ShopLayer);
    virtual bool init() override;
    
    bool initData();
    bool initLayer();

protected:
    void addIapButton();
    void showDialog();
    
    void purchaseCallback(bool isSuc);
    void restoreCallback(bool isRestore, bool isPurchase);
    
public:
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    // 监听Android返回键事件
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event) override;
    virtual void onNegativeClick(void* type) override;
    virtual void onPositiveClick(void* type) override;
    
    virtual void onTouchDown(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onTouchUp(ToolSprite* toolSprite, Touch *pTouch) override;
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch) override;
    
private:
    Sprite* m_pBg;
    
    Sprite* m_pGirl;
    
    int m_iCurrtTag;
    
    bool isReturn;
  
};

#endif /* defined(__FastFood__ShopLayer__) */
