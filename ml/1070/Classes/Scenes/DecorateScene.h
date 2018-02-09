
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "TouchLayer.h"
#include "DecTypeScrollView.h"

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
    void _saveCottonCandy();
    void _showPlate();
    void _removePlate();
    void _displayCottonCandy();
    
    Sprite* m_pBg;
    Sprite* m_pTable;
    Sprite* m_pShed;
    
    Sprite* m_pDecBg;
    Layer* m_pDecorateLayer;
    
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    TouchLayer* m_pTouchLayer;
    ClippingNode* m_pClipping;
    Sprite* m_pCandy;
    Sprite* m_pStick;
    Sprite* m_pBag;
    Sprite* m_pPlateShadow;
    Sprite* m_pPlateBack;
    Sprite* m_pPlateFront;
    Node* m_pDessert;
    Node* m_pDessert2;
    
    int m_nPackage;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    bool m_bCanTouch;
    int m_nIndex;
    Size m_CandySize;
    bool m_bPacked;
    
    bool m_bGuide[3];
    bool m_bVOGuide[8];
};

#endif