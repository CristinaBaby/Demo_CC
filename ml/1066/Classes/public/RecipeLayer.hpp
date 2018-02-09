//
//  RecipeLayer.hpp
//  Muffin
//
//  Created by huxixiang on 15/11/27.
//
//

#ifndef RecipeLayer_hpp
#define RecipeLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
class RecipeLayer : public LayerColor{
    
public:
    RecipeLayer();
    virtual ~RecipeLayer();
    
    static RecipeLayer* create(const Color4B& color = Color4B(0, 0, 0, 0));
    bool initWithColor(const Color4B& color);
    
    bool initData();
    bool initUI();
    
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    void onButtontouchCallback(Ref *pSender, Widget::TouchEventType type);
    
private:
    Sprite* m_pBg;
    
    Button* m_pCloseBtn;
};

#endif /* RecipeLayer_hpp */
