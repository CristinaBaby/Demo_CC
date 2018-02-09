//
//  PopAcgLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/25.
//
//

#ifndef PopAcgLayer_hpp
#define PopAcgLayer_hpp

#include <stdio.h>


#include "cocos2d.h"
#include "KSPopLayer.h"
#include "ui/CocosGUI.h"
#include "ActionUtils.h"
#include "DialogCallback.h"
#include "Dialog.h"
#include "AdapterScreen.h"
#include "MySpriteButton.h"


USING_NS_CC;

using namespace std;
using namespace ui;

class PopAcgLayer : public KSPopLayer{
    
public:
    PopAcgLayer();
    virtual ~PopAcgLayer();
    
public:
    static void loading(Node* node,std::function<void()> endFunc = nullptr);
    static PopAcgLayer* create();
    virtual bool init();
private:
    void clickOk(Ref* pSdener);
    void clickNo(Ref* pSdener);
    
    void showUI();
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    
    std::function<void()> btnClose = nullptr;
    std::function<void()> _endfunc;
public:
    void initUI();
    int randNum();
    
private:
    Sprite* m_pDialog;
    MySpriteButton* m_pOk;
    Sprite* content;
};

#endif /* PopAcgLayer_hpp */
