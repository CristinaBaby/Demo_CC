
#ifndef __SauceUtil__
#define __SauceUtil__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SauceUtil : public Layer
{
public:
    SauceUtil();
    ~SauceUtil();
    
    CREATE_FUNC(SauceUtil);
    
    virtual bool init();
    
    void initWithIndex(int index);
    
    Node* getDecorateResult();
    
    Sprite* m_pPlateBack;
    Sprite *m_pSauce;
};

#endif