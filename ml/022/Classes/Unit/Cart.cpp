
#include "Cart.h"
#include "Global.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"


using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

Cart::Cart()
{
    onPriceButtonCallback = nullptr;
}

Cart::~Cart()
{
    
}
bool Cart::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    return true;
}

void Cart::setPriceTag(int index,std::string price)
{
    Button* pButton = m_PriceButtons.at(index);
    Label* pLable = (Label*)pButton->getChildByName("priceLabel");
    pLable->setString(price);
}

void Cart::_onButtonCallback(Ref* ref,Widget::TouchEventType type){
    if (Widget::TouchEventType::ENDED==type) {
        Button* pButton = (Button*)ref;
        if(onPriceButtonCallback){
            onPriceButtonCallback(pButton);
        }
    }
}
