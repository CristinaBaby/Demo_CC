
#ifndef __MapScene__
#define __MapScene__


#include "ExtensionScene.h"

class MapScene : public ExtensionScene
{
public:
    MapScene();
    ~MapScene();
    
    MY_SCENE(MapScene);
    CREATE_FUNC(MapScene);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void onButtonCallback(Ref*,Widget::TouchEventType);
protected:
    virtual void _initData();
    void _showGirl();
    void _winterMagic();
    
    void _showHouse();
    void _showShopHouse();
    ExpandScrollView* m_pScrollView;
    Node* m_MapLayer;
    Vector<Node*> m_Houses;
    Node* m_pHouseShop;
    Armature* m_pGirlArmature;
    bool m_bCanTouch;
};

#endif