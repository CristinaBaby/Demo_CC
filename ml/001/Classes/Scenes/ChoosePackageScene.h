
#ifndef __ChoosePackageScene__
#define __ChoosePackageScene__


#include "ExtensionScene.h"
#include "RoleModel.h"

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
    virtual void onExit();
    
    void onButtonCallback(Ref*,Widget::TouchEventType type);
    void onSelectCallback(int index);
protected:
    virtual void _initData();
    void _showDonutPackage();
    void _showTwistPackage();
    void _playEffect();
    
    Sprite* m_pDonutBubble0;
    Sprite* m_pDonutBubble1;
    Sprite* m_pTwistBubble0;
    Sprite* m_pTwistBubble1;
    Button* m_pDonut;
    Button* m_pTwist;
    
    RoleModel* m_pGirl;
    bool m_bDonutShowed;
    bool m_bTwistShowed;
};

#endif