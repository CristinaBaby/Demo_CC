
#ifndef __PriceEditBox__
#define __PriceEditBox__


#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIEditBox/UIEditBoxImpl.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PriceEditBox : public EditBox
{
public:
    PriceEditBox();
    ~PriceEditBox();
    static PriceEditBox* create(const Size& size,
                           Scale9Sprite* normalSprite,
                           Scale9Sprite* pressedSprite = nullptr,
                           Scale9Sprite* disabledSprite = nullptr);
    
    virtual bool init();
    
    void onEnter();
};


#endif