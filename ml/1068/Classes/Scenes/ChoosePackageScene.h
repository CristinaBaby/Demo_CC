
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
    
    virtual bool init();
    void onPackageCallback(Ref*,Widget::TouchEventType);
protected:
    virtual void _initData();
    
    ExpandScrollView* m_pScrollView;
    
};

#endif