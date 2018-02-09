
#ifndef __ShareScene__
#define __ShareScene__


#include "ExtensionScene.h"
#include "EatLayer.h"
#include "ChooseBackgroudLayer.h"

class ShareScene : public ExtensionScene
{
public:
    ShareScene();
    ~ShareScene();
    
    MY_SCENE(ShareScene);
    CREATE_FUNC(ShareScene);
    
    virtual bool init();
    virtual void onEnter();
    void onExit();
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onKeyBackClicked();
    
    void onEatBeinCallback(Vec2 location,bool drinkable);
    void onEatEndCallback(Vec2 location);
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    void _showEatScene();
    
    RenderTexture* getResultRender();
    
    int m_nPackage;
    EatLayer* m_pEatLayer;
    Layer* m_pDecoLayer;
    DecorateManager* m_pDecManager;
    Sprite* m_pBg;
    Sprite* m_pTip;
    
    NodeGrid* m_pGrid;
    
    bool m_bBack;
    int m_nBgIndex;
};

#endif