
#ifndef __DecorateConeScene__
#define __DecorateConeScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "CandlePosition.h"
#include "CandleNode.h"
#include "ScribbleShapeNode.h"

class DecorateConeScene : public ExtensionScene
{
public:
    DecorateConeScene();
    ~DecorateConeScene();
    
    MY_SCENE(DecorateConeScene);
    CREATE_FUNC(DecorateConeScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorationCallback(int index,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);

protected:
    virtual void _initData();
    void _reset();
    void _playEffectNice(float dt = 0.0f);
    
    Layer* m_pDecorateLayer;
    Sprite* m_pConeBack;
    ClippingNode* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pTexture;
    ScribbleNode* m_pColorScribble;
    ScribbleShapeNode* m_pScribble;
    Node* m_pSticker;
    Sprite* m_pExtra;
    
    Sprite* m_pBg;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    std::string m_sCurTypeStr;
    int m_nType;
    bool m_bShowShop;
    int m_nPenIndex;
    bool m_bCanTouch;
    bool m_bGuide[2];
    int m_nTextureCount;
    int m_nPenCount;
    int m_nColorCount;
};

#endif