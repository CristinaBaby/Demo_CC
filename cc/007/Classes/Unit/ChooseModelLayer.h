
#ifndef ChooseModelLayer_h
#define ChooseModelLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>
#include "LockItem.h"

USING_NS_CC;
class ChooseModelLayer : public Layer
{
public:
    ChooseModelLayer();
    ~ChooseModelLayer();
    
    CREATE_FUNC(ChooseModelLayer);
    
    virtual bool init();
    
    void showChooseModel();
    
    std::function<void(int)> onItemCallback;
protected:
    void _onItemCallback(LockItem*);
    bool m_bSelected;
    Node* m_pRoot;
    Vector<LockItem*> m_Items;
};
#endif /* ChooseModelLayer_h */
