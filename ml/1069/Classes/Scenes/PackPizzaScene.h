
#ifndef __PackPizzaScene__
#define __PackPizzaScene__


#include "ExtensionScene.h"
#include "ItemScrollView.h"
#include "DecTypeScrollView.h"
#include "TouchLayer.h"

class PackPizzaScene : public ExtensionScene
{
public:
    PackPizzaScene();
    ~PackPizzaScene();
    
    MY_SCENE(PackPizzaScene);
    CREATE_FUNC(PackPizzaScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onButtonCallback(Button* btn);
    RenderTexture* getResultRender();
    
    void onTypeCallback(int index,DecorateTypeConfigData,bool selected);
    void onDecorateOK();
    void onDecorationCallback(int index,int type,DecorateConfigData decData);
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
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
    
    Sprite* m_pBg;
    DecTypeScrollView* m_pTypeScrollView;
    ItemScrollView* m_pDecorationScrollView;
    
    TouchLayer* m_pTouchLayer;
    int m_nPackage;
    std::string m_sCurTypeStr;
    std::string m_sCurTypePath;
    
    Node* m_pPizza;
    Node* m_pBoxNode;
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    
    int m_nIndex;
    
    bool m_bPacked;
    bool m_bCanDecorate;
    
    Size m_boxSize;
};

#endif