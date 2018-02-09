
#ifndef __Cart__
#define __Cart__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DragNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class Cart : public Node
{
public:
    Cart();
    ~Cart();
    
    CREATE_FUNC(Cart);
    
    virtual bool init();
    
    void setPriceTag(int index,std::string price);
    Vec2 getCandyPosition(int index){
        return m_CandyPositions.at(index);
    }
    Button* getPriceButton(int index) {
        return m_PriceButtons.at(index);
    }
    std::function<void(Button*)> onPriceButtonCallback;
protected:
    void _onButtonCallback(Ref*,Widget::TouchEventType);
    
    Sprite* m_pCart;
    Vector<Button*> m_PriceButtons;
    Vector<Node*> m_Candys;
    std::vector<Vec2> m_CandyPositions;
};


#endif