
#ifndef __DecorateGiftScene__
#define __DecorateGiftScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "CandlePosition.h"
#include "CandleNode.h"

class DecorateGiftScene : public ExtensionScene
{
public:
    DecorateGiftScene();
    ~DecorateGiftScene();
    
    MY_SCENE(DecorateGiftScene);
    CREATE_FUNC(DecorateGiftScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onDecorationCallback(int index,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _reset();
    void _showItems();
    
    Layer* m_pDecorateLayer;
    Layer* m_pCandleLayer;
    Sprite* m_pDecoratedCake;
    Vector<CandleNode*> m_VectorCandle;
    Sprite* m_pBg;
    ItemScrollView* m_pDecorationScrollView;
    std::string m_sCurTypeStr;
    int m_nType;
    bool m_bShowShop;
};

#endif