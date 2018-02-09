//
//  PopDialogLayer.h
//  COOK00321047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/11.
//
//

#ifndef __COOK00321047_HotDogMaker__PopDialogLayer__
#define __COOK00321047_HotDogMaker__PopDialogLayer__

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

enum PopDialogType{

    newFlavor = 0,
    allFlavor,

};

class PopDialogLayer : public KSPopLayer{
    
public:
    PopDialogLayer();
    virtual ~PopDialogLayer();
    
public:
    static PopDialogLayer* create(PopDialogType type);
    virtual bool init(PopDialogType type);
private:
    void clickOk(Ref* pSdener);
    void clickNo(Ref* pSdener);
    
    void showUI();
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    
    std::function<void()> btnClose = nullptr;
public:
    void initUI();
    int randNum();
    
private:
    Sprite* m_pDialog;
    MySpriteButton* m_pOk;
    PopDialogType _type;
    Sprite* content;
//    MySpriteButton* m_pNo;
//    Sprite* m_pMold;
//    Sprite* m_pQues;
};
#endif /* defined(__COOK00321047_HotDogMaker__PopDialogLayer__) */
