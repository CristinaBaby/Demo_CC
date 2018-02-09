
#ifndef __DecorateScene__
#define __DecorateScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"
#include "ScribbleShapeNode.h"

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
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(DecorateScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(DecorateScene));}
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,int type,DecorateConfigData Data);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMoved(Touch *pTouch, Event *pEvent);
   void TouchEnded(Touch *pTouch, Event *pEvent);
protected:
    virtual void _initData();
    void _initDefaultDecorate();
    void _reset();
    
    void _showBox();
    void _moveBox();
    void _pack();
    void _unpack();
    
    void _addDecoration(int index,DecorateConfigData decData);
    void _saveBox();
    void _savePizza();
    
    void _displayPizza();
    
    Sprite* m_pBg;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    TouchLayer* m_pTouchLayer;
    int m_nPackage;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    
    Layer* m_pDecorateLayer;
    
    Node* m_pBoxNode;
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    ScribbleShapeNode* m_pScribble;
    
    Node* m_pPizza;
    Node* m_pPlate;
    
    bool m_bCanTouch;
    int m_nIndex;
    bool m_bShowShop;
    bool m_bGuide[3];
    bool m_bVOGuide[4];
    bool m_bPacked;
    bool m_bCanDecorate;
    Size m_boxSize;
};

#endif