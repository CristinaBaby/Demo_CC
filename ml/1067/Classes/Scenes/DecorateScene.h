
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"

class DecorateScene : public ExtensionScene
{
public:
    DecorateScene();
    ~DecorateScene();
    
    MY_SCENE(DecorateScene);
    CREATE_FUNC(DecorateScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
   void TouchBegan(Touch *pTouch, Event *pEvent);
   void TouchEnded(Touch *pTouch, Event *pEvent);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _reset();
    
    void _playEffectNice(float dt = 0.0f);
    Sprite* m_pBg;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    TouchLayer* m_pTouchLayer;
    int m_nPackage;
    std::string m_sCurTypeStr;
    
    bool m_bCanTouch;
    int m_nIndex;
    bool m_bShowShop;
    bool m_bGuide[3];
};

#endif