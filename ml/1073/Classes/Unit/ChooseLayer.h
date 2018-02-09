
#ifndef ChooseLayer_h
#define ChooseLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>
#include "LockItem.h"

USING_NS_CC;
class ChooseLayer : public Layer
{
public:
    ChooseLayer();
    ~ChooseLayer();
    
    CREATE_FUNC(ChooseLayer);
    
    virtual bool init();
    
    void showChooseBlender();
    
    std::function<void(int)> onItemCallback;
protected:
    void _onItemCallback(LockItem*);
    bool m_bSelected;
};
#endif /* ChooseLayer_h */
