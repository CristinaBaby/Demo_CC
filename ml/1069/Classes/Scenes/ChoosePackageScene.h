
#ifndef __ChoosePackageScene__
#define __ChoosePackageScene__


#include "ExtensionScene.h"

class ChoosePackageScene : public ExtensionScene
{
public:
    ChoosePackageScene();
    ~ChoosePackageScene();
    
    MY_SCENE(ChoosePackageScene);
    CREATE_FUNC(ChoosePackageScene);
    
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(ChoosePackageScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(ChoosePackageScene));}
    
    virtual bool init();
    void onPackageCallback(LockItem* item);
    void onButtonCallback(Ref*,Widget::TouchEventType type);
    void onSelectCallback(int index);
protected:
    virtual void _initData();
    Sprite* m_pBg;
    ExpandScrollView* m_pScrollView;
    
    ClippingNode* m_pGummyClipping;
    Sprite* m_pGummyDoor;
    
    ClippingNode* m_pNormalClipping;
    Sprite* m_pNormalDoor;
    
    bool m_bSelected;
    
    RoleModel* m_pGirl;
};

#endif