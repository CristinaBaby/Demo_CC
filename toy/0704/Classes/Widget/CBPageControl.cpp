//
//  CBPageControl.cpp
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#include "CBPageControl.h"
USING_NS_CC;

#define INTERVAL 10

PageControl* PageControl::create(const std::string& normalImage,
                                 const std::string& selectImage)
{
    PageControl *ref = new (std::nothrow)(PageControl);
    if (ref && ref->init(normalImage, selectImage))
    {
        
        ref->autorelease();
        return ref;
    }
    else
    {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
    
}

bool PageControl::init(const std::string& normalImage, const std::string& selectImage)
{
    if (!Layer::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {
        
        _normalImageName = normalImage;
        _selectImageName = selectImage;
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void PageControl::setPagesCount(const int pagesCount)
{
    _pagesCount = pagesCount;
    float offset = 0;
    for (int i = 0; i < _pagesCount; ++i)
    {
        Sprite *normal = Sprite::create(_normalImageName);
        normal->setPosition(Vec2(offset+normal->getContentSize().height/2, normal->getContentSize().height/2));
        offset += normal->getContentSize().width + INTERVAL;
        addChild(normal);
    }
    
    _selectedControl = Sprite::create(_selectImageName);
    _selectedControl->setPosition(Vec2(_selectedControl->getContentSize().width/2, _selectedControl->getContentSize().height/2));
    addChild(_selectedControl);

    //caculate size
    setContentSize(Size(_pagesCount * (_selectedControl->getContentSize().width + INTERVAL), _selectedControl->getContentSize().height));
}


void PageControl::setSelectPage(const int index)
{
    _selectedControl->setPosition(index * _selectedControl->getContentSize().width + INTERVAL * (index + 1), _selectedControl->getPositionY());
}
