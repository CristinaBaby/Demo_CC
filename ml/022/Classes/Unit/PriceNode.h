
#ifndef __PriceNode__
#define __PriceNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PriceNode : public Node
{
public:
    PriceNode();
    ~PriceNode();
    
    CREATE_FUNC(PriceNode);
    
    virtual bool init();
    
    
    void initWithName(const std::string nomal,const std::string highlight,int price,bool locked = false);
    void unlock(bool unlock = true);
    bool isLocked(){return m_bLocked;}
    int getPrice(){return m_nPrice;}
protected:
    std::string m_sNormalPath;
    std::string m_sHighlightPath;
    bool m_bLocked;
    int m_nPrice;
    Sprite* m_pContentSrite;
    Sprite* m_pLock;
    Label *m_pPriceLable;
};

#endif