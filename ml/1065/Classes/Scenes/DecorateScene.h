
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
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
    virtual void onExit();
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _reset();
    
    Sprite* m_pBg;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    int m_nPackage;
    std::string m_sCurTypeStr;
    
    bool m_bShowShop;
};

#endif