
#include "PriceEditBox.h"

PriceEditBox::PriceEditBox()
{
}

PriceEditBox::~PriceEditBox()
{
    
}

PriceEditBox* PriceEditBox::create(const cocos2d::Size &size, cocos2d::ui::Scale9Sprite *normalSprite, ui::Scale9Sprite *pressedSprite, Scale9Sprite* disabledSprite)
{
    PriceEditBox* pRet = new (std::nothrow) PriceEditBox();
    
    if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, normalSprite))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}
bool PriceEditBox::init()
{
    if ( !EditBox::init() )
    {
        return false;
    }
    return true;
}

void PriceEditBox::onEnter()
{
    EditBox::onEnter();
    if (_editBoxImpl) {
        this->runAction(Sequence::create(DelayTime::create(0.3),
                                         CallFunc::create([=](){
            _editBoxImpl->openKeyboard();
        }), NULL));
    }
}