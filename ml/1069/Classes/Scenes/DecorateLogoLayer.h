
#ifndef __DecorateLogoLayer__
#define __DecorateLogoLayer__


#include "cocos2d.h"
#include "IAPItem.h"
#include "Heads.h"
#include "ScribbleShapeNode.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "GuideLayer.h"

USING_NS_CC;


class DecorateLogoLayer : public Layer
{
public:
    DecorateLogoLayer();
    ~DecorateLogoLayer();
    
    CREATE_FUNC(DecorateLogoLayer);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    std::function<void()> onLogoDecoratedCallback;
    void onButtonCallback(Ref*,Widget::TouchEventType type);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    void _showDecorationBanner();
    void _onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void _onDecorationCallback(int index,int type,DecorateConfigData decData);
    
    void _reset();
    bool _saveLogo();
    
    void _setStickerTouchEnable(bool enable);
    
    GuideLayer* m_pGuideLayer;
    Node* m_pRootNode;
    
    Sprite* m_pBg;
    
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    Button* m_pBtnOK;
    Button* m_pBtnCancel;
    
    ClippingNode* m_pLogoClipping;
//    Sprite* m_pColor;
    Sprite* m_pPattern;
    ScribbleShapeNode* m_pScribble;
    Node* m_pSticker;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    int m_nIndex;
    bool m_bCanTouch;
    DecorateConfigData m_curDecData;
    bool m_bGuide[4];
    bool m_bVOGuide[4];
};

#endif