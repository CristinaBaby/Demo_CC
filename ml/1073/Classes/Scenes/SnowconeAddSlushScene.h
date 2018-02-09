
#ifndef SnowconeAddSlushScene_h
#define SnowconeAddSlushScene_h

#include "ExtensionScene.h"

class SnowconeAddSlushScene : public ExtensionScene
{
public:
    SnowconeAddSlushScene();
    ~SnowconeAddSlushScene();
    
    MY_SCENE(SnowconeAddSlushScene);
    CREATE_FUNC(SnowconeAddSlushScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showBox();
    void _showCone(Vec2 posDelt = Vec2::ZERO);
    
    void _finish();
    
    Sprite* m_pBg;
    
    DragNode* m_pSpoon;
    
    Sprite* m_pBox;
    Sprite* m_pBoxFront;
    Sprite* m_pIcing;
    Sprite* m_pIcingFront;
    
    Node* m_pSnowConeNode;
    Sprite* m_pCone;
    Sprite* m_pConeFront;
    Sprite* m_pConeIce;
    int m_nSnowPutCount;
};
#endif /* SnowconeAddSlushScene_h */
