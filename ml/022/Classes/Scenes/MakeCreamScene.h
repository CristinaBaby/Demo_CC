
#ifndef __MakeCreamScene__
#define __MakeCreamScene__


#include "ExtensionScene.h"
#include "BlenderNode.h"
#include "ChooseLayer.h"

class MakeCreamScene : public ExtensionScene
{
public:
    MakeCreamScene();
    ~MakeCreamScene();
    
    MY_PHYSIC_SCENE(MakeCreamScene);
    CREATE_FUNC(MakeCreamScene);
    enum{
        eDragTagFlavorBottle,
        eDragTagStick,
    };
    virtual bool init();
    virtual void onEnter();
    void update(float dt = 0.0f);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    
    void _showChooseBlenderUI();
    void _beginCrush();
    void _finish();
    
    ChooseLayer* m_pBlenderLayer;
    BlenderNode* m_pBlender;
    BlenderCup* m_pCup;
};

#endif