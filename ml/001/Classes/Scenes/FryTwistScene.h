

#ifndef FryTwistScene_h
#define FryTwistScene_h

#include "ExtensionScene.h"

class FryTwistScene : public ExtensionScene {
    
public:
    FryTwistScene();
    ~FryTwistScene();
    
    MY_SCENE(FryTwistScene);
    CREATE_FUNC(FryTwistScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(FryTwistScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(FryTwistScene));}
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
protected:
    virtual void _initData();
    
    void _showPlate();
    void _doFry(DragNode* pNode);
    void _finishFry(DragNode* pNode);
    
    
    void _onProgressCallback(ProgressBarNew*,int);
    Sprite* m_pPot;
    Sprite* m_pPlateLeft;
    Sprite* m_pPlateRight;
    
    std::vector<DragNode*> m_DonutLeftVector;
    std::vector<DragNode*> m_DonutRightVector;
    
    std::map<int,DragNode*> m_DonutPotMap;
};
#endif /* FryTwistScene_h */
