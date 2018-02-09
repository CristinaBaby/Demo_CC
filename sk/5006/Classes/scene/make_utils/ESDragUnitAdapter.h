//
//  ESDragUnitAdapter.hpp
//  ccats1053
//
//  Created by liji on 16/1/14.
//
//

#ifndef ESDragUnitAdapter_hpp
#define ESDragUnitAdapter_hpp

#include <stdio.h>
#include "ESUnitAdapter.h"
#include "ESScrollView.h"
using namespace std;

class ESDragUnitAdapter:public ESUnitAdapter
{
public:
    CreateAdapter(ESDragUnitAdapter);

    ESDragUnitAdapter();
    ~ESDragUnitAdapter();
    
    virtual ui::ImageView* getUnit(int idx = 0) override;
    
    function<void(ui::Widget* item,Touch* p)> _itemTouchMoved = nullptr;
    function<void(ui::Widget* item,Touch* p)> _itemTouchBegan = nullptr;
    function<void(ui::Widget* n,Touch* p)> _itemTouchEnded = nullptr;

    
    void setScroll(ESScrollView* s)
    {
        _scroll = s;
    };
    
private:
    
    void initListener();
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnd(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);

private:
    EventListenerTouchOneByOne* _listener;
    
    bool _checkMoving;
    bool _isMoving; 
    ESScrollView* _scroll;
    float _moveDisX;
    float _moveDisY;
    
};

#endif /* ESDragUnitAdapter_hpp */
