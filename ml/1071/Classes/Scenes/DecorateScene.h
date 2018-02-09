
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "TouchLayer.h"
#include "DecTypeScrollView.h"
#include "ScribbleSauce.h"

class DecorateScene : public ExtensionScene
{
public:
    DecorateScene();
    ~DecorateScene();
    
    MY_SCENE(DecorateScene);
    CREATE_FUNC(DecorateScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onButtonCallback(Button* btn);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,int type,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _reset();
    void _saveFood();
    
    void _displayFood();
    
    void _disableMovableFood(bool disable = true,bool down = true);
    
    Sprite* m_pBg;
    Sprite* m_pTable;
    
    Layer* m_pDecorateLayer;
    
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    TouchLayer* m_pTouchLayer;
    ClippingNode* m_pClipping;
    ClippingNode* m_pClippingUp;
    ClippingNode* m_pClippingSause;
    Sprite* m_pPlate;
    Sprite* m_pFood;
    ScribbleSauce* m_pSauce;
    Sprite* m_pSauseBottle;
    
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    bool m_bCanTouch;
    int m_nIndex;
    Size m_CandySize;
    bool m_bPacked;
    
    bool m_bButtonEable;
    bool m_bGuide[3];
    bool m_bVOGuide[8];
};

#endif