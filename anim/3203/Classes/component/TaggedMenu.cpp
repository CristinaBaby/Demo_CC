

#include "TaggedMenu.h"
#include <algorithm>


TaggedMenu::TaggedMenu()
:m_pCurrent(nullptr)
,m_fTaggedChangeCallback(nullptr)
{
    
}

TaggedMenu *TaggedMenu::create()
{
    TaggedMenu *layer = new TaggedMenu();
    if (layer && layer->initWithTaggedMenu()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

TaggedMenu *TaggedMenu::create(SwitchButton *aButton, ...)
{
    va_list args;
    va_start(args,aButton);
    
    TaggedMenu *ret = TaggedMenu::createWithItems(aButton, args);
    
    va_end(args);
    
    return ret;
}

TaggedMenu *TaggedMenu::createWithItems(SwitchButton *firstButton, va_list args)
{
    Vector<SwitchButton*> items;
    if( firstButton )
    {
        items.pushBack(firstButton);
        SwitchButton *i = va_arg(args, SwitchButton*);
        while(i) {
            items.pushBack(i);
            i = va_arg(args, SwitchButton*);
        }
    }
    
    return TaggedMenu::createWithArray(items);
}

TaggedMenu* TaggedMenu::createWithArray(const Vector<SwitchButton*>& arrayOfItems)
{
    auto ret = new TaggedMenu();
    if (ret && ret->initWithArray(arrayOfItems)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool TaggedMenu::initWithArray(const Vector<SwitchButton*>& arrayOfItems)
{
    int z=0;
    
    for (auto& item : arrayOfItems)
    {
        this->addChild(item, z);
        item->m_fTouchCallback = CC_CALLBACK_1(TaggedMenu::onButtonCallback, this);
        m_vButtons.push_back(item);
        z++;
    }
    
    return true;
}

bool TaggedMenu::initWithTaggedMenu()
{
    if ( !Layer::init() ) {
        return false;
    }
    return true;
}

void TaggedMenu::setDefaultSelected(SwitchButton *aButton)
{
    setCurrentTagged(aButton);
}

void TaggedMenu::setCurrentTagged(SwitchButton *aButton)
{
    if (aButton == nullptr) {
        return;
    }
    
    if (aButton == m_pCurrent || std::find(m_vButtons.begin(), m_vButtons.end(), aButton) == m_vButtons.end()) {
        return;
    }
    
    m_pCurrent = aButton;
    
    m_pCurrent->changeStatus(SwitchButton::STATE_SELECTED);
    
    m_fTaggedChangeCallback(m_pCurrent->getTag());
}

void TaggedMenu::onButtonCallback(SwitchButton *pSender)
{
    setCurrentTagged(pSender);
    
    for (auto item : m_vButtons) {
        if (item != pSender && item->m_eStatus != SwitchButton::STATE_DISABLE) {
            item->changeStatus(SwitchButton::STATE_NORMAL);
        }
    }
}

TaggedMenu::~TaggedMenu()
{
    
}