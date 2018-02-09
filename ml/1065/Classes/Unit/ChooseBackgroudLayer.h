
#ifndef __ChooseBackgroudLayer__
#define __ChooseBackgroudLayer__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>

USING_NS_CC;
using namespace cocos2d::ui;

class Door : public Node {
    
    
public:
    Door(){
        onDoorCallback = nullptr;
    }
    static Door* create(int index);
    virtual bool init(int index);
    
    void openDoor();
    void closeDoor();
    
    void setDoorTouchEnable(bool enable){
        m_pDoor->setEnabled(enable);
    }
    std::function<void(int)> onDoorCallback;
protected:
    
    void _onButtonCallback(Ref*,Widget::TouchEventType);
    
    Button* m_pDoor;
    Sprite* m_pLight;
    int m_nIndex;
};

class ChooseBackgroudLayer : public Layer
{
public:
    ChooseBackgroudLayer();
    ~ChooseBackgroudLayer();
    
    CREATE_FUNC(ChooseBackgroudLayer);
    
    virtual bool init();
    void onButtonCallback(Ref*,Widget::TouchEventType);
    
    std::function<void(int index)> onItemCallback;
protected:
    void _onDoorCallback(int index);
    void _dismiss();
    Button* m_pNext;
    int m_nIndex;
    Sprite* m_pWall;
    Vector<Door*> m_VectorDoor;
};

#endif