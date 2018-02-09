
#ifndef ChoosePackageLayer_h
#define ChoosePackageLayer_h

#include "ExtensionScene.h"

class ChoosePackageLayer : public Layer
{
public:
    ChoosePackageLayer();
    ~ChoosePackageLayer();
    
    MY_SCENE(ChoosePackageLayer);
    CREATE_FUNC(ChoosePackageLayer);
    
    virtual bool init();
    virtual void onEnter();
    
    std::function<void(int index)>onChooseCallback;
protected:
    void _onItemCallback(LockItem* pItem);
    
    Sprite* m_pBg;
    bool m_bSelected;
    Vector<LockItem*> m_Items;
};
#endif /* ChoosePackageLayer_h */
