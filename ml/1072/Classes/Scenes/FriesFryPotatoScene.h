
#ifndef FriesFryPotatoScene_h
#define FriesFryPotatoScene_h

#include "ExtensionScene.h"

class FriesFryPotatoScene : public ExtensionScene
{
public:
    FriesFryPotatoScene();
    ~FriesFryPotatoScene();
    
    MY_PHYSIC_SCENE(FriesFryPotatoScene);
    CREATE_FUNC(FriesFryPotatoScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void updateStart(float delta);
    void update(float delta);
    void _onProgressCallback(ProgressBarNew* pBar,int segment);
protected:
    virtual void _initData();
    void _showThermomter();
    void _showPotatoPlate();
    void _showSalt();
    
    void _pourPotato();
    void _doFry();
    void _finish();
    
    Sprite* m_pBg;
    
    Sprite* m_pPot;
    Sprite* m_pColander;
//    Sprite* m_pColanderFront;
    DragNode* m_pColanderFront;
    
    DragNode* m_pPlate;
    DragNode* m_pSalt;
    std::vector<Sprite*> m_PotatoVector;
    ProgressBarNew* m_pBar;
};
#endif /* FriesFryPotatoScene_h */
