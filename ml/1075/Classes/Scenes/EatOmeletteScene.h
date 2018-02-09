
#ifndef EatOmeletteScene_h
#define EatOmeletteScene_h

#include "ShareScene.h"

class EatOmeletteScene : public ShareScene
{
public:
    EatOmeletteScene();
    ~EatOmeletteScene();
    
    MY_SCENE(EatOmeletteScene);
    CREATE_FUNC(EatOmeletteScene);
    
    virtual bool init();
    virtual void onEnter();
        
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showEatScene();
    Sprite* m_pBg;
    Sprite* m_pPlate;
    Sprite* m_pOmelette;
    Node* m_pDecNode;
};
#endif /* EatOmeletteScene_h */
