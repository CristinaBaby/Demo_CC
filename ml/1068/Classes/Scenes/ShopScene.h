
#ifndef __ShopScene__
#define __ShopScene__


#include "ExtensionScene.h"
#include "ExpandScrollView.h"
#include "IAPItem.h"

class ShopScene : public ExtensionScene
{
public:
    ShopScene();
    ~ShopScene();
    
    MY_SCENE(ShopScene);
    CREATE_FUNC(ShopScene);
    
    virtual bool init();
    virtual void onEnter();
    
    void onItemClicked(int);
    
protected:
    virtual void _initData();
    
    void _showScrollBottom();
    void _showScrollBottomBack();
    
    
    ExpandScrollView* m_pScrollView;
    
    Vector<IAPItem*> m_ItemVector;
    Point m_InnerPosition;
    bool m_bIsScrolling;

};

#endif