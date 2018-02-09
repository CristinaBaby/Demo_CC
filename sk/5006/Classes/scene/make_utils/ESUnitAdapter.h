//
//  ESUnitAdapter.h
//  Make
//
//  Created by liji on 15/9/22.
//
//

#ifndef __Make__ESUnitAdapter__
#define __Make__ESUnitAdapter__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"
USING_NS_CC;

#define CreateAdapter(__TYPE__) \
static __TYPE__* create(std::string pathFormat, int startIndex, int endIndex) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->_pathFormat = pathFormat;\
pRet->_startIndex = startIndex;\
pRet->_endIndex = endIndex;\
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}


class ESUnitAdapter : public Ref
{
public:
    
    CreateAdapter(ESUnitAdapter);
    
    int getUnitCount();
    
    virtual ui::Widget* getUnit(int idx = 0);
    
    virtual Size getItemSize(int idx = 0);
    
    void registTouchEvent(ui::Button* obj);
        
    std::function<void(ui::Widget*)> unitTouchEnded ;
    
    virtual void btnTouchMoved(Ref*){};

    ESUnitAdapter();
    ~ESUnitAdapter(){};
    
    
    
protected:
    std::string _pathFormat;
    std::string _bgName;
    
    
    CC_SYNTHESIZE(int, _startIndex, StartIndex);
    CC_SYNTHESIZE(int, _endIndex, EndIndex);

private:
    void touchEvent(Ref*,ui::Widget::TouchEventType);

};

#endif /* defined(__Make__ESUnitAdapter__) */
