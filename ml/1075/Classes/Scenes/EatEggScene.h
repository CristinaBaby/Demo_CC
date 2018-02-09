
#ifndef EatEggScene_h
#define EatEggScene_h

#include "ShareScene.h"

class EatEggScene : public ShareScene
{
public:
    EatEggScene();
    ~EatEggScene();
    
    MY_SCENE(EatEggScene);
    CREATE_FUNC(EatEggScene);
    
    virtual bool init();
    virtual void onEnter();
        
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    void onEatEndCallback(Vec2 location);
protected:
    virtual void _initData();
    
    void _reset();
    void _showEatScene();
    Sprite* m_pBg;
    Sprite* m_pPlate;
    Sprite* m_pOmelette;
    Node* m_pDecNode;
};
#endif /* EatEggScene_h */
