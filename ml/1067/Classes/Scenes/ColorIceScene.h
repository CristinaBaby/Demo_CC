
#ifndef __ColorIceScene__
#define __ColorIceScene__


#include "ExtensionScene.h"
#include "ScribbleNode.h"

class ColorIceScene : public ExtensionScene
{
public:
    ColorIceScene();
    ~ColorIceScene();
    
    MY_SCENE(ColorIceScene);
    CREATE_FUNC(ColorIceScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    RenderTexture* getResultRender();
    void onButtonCallback(Button* btn);
    void onBottleClicked(Ref*,Widget::TouchEventType);
    void dragNodeTouchBegin(DragNode* dragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* dragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* dragNode,Point worldPoint);
    void dragNodeClicked(DragNode* dragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showColorBottle(bool unlock = false);
    void _showSnowCone();
    void _showAllBottleAnimation();
    
    void _showUnlockLayer();
    void _moveawayUnlockLayer();
    void _showTool(int index);
    void _moveawayTool(int index);
    void _resetColor();
    void _playEffectYummy(float);
    ExpandScrollView* m_pBottleList;
    Vector<DragNode*> m_ToolBox;
    Vector<Node*> m_Snows;
    Vector<Node*> m_SnowPos;
    Vector<Node*> m_Ices;
    Vector<Node*> m_IcePos;
    Vector<Node*> m_CrushedIces;
    Vector<Node*> m_IceArrows;
    Vector<Node*> m_SnowArrows;
    
    Node* m_pUnlockLayer;
    Node* m_pBoxBack;
    Node* m_pBoxFront;
    
    Sprite* m_pConeShape;
    ScribbleNode* m_pSnow;
//    MyRenderTexture* pTempRT;
    
    int m_nShapeIndex;
    int m_nFlavorIndex;
    bool m_bResetShown;
    bool m_bGuide[2];
};

#endif