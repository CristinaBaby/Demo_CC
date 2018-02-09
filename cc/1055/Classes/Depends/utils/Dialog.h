#ifndef __DIALOG_H__
#define __DIALOG_H__
#include "cocos2d.h"
#include "DialogCallback.h"
#include "Utils.h"
#include "extensions/cocos-ext.h"
#include "AudioHelp.h"

USING_NS_CC;
enum DialogDirection
{
    HORIZONTAL = 0,
    VERTICAL,
};
enum DialogFuncType {
    kDialogTypeNone = 100,
    kDialogReturnHome,
    kDialogReset,
    kDialogExit,
    kDialogNotice
};

class Dialog:public LayerColor {
public:
    Dialog();
    
    virtual ~Dialog();
    
    virtual bool init();
    
    static Dialog* create(void*, int, DialogDirection direction, float offsetX = 0, float offsetY = 0);
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
public:
    virtual void setContentText(const char*);
    
    virtual void setContentTextColor(const Color3B &color);

    virtual void setPositiveBtnText(const char*);
    
    virtual void setNegativeBtnText(const char*);
    
    void onClick(Ref*);
    
    void refreshPosition();
    
    void setCallback(DialogCallback* callback);
    
protected:
    void onBack();
    
    void dismiss();
    
    Layer* m_pContentLayer;
    
    Label* m_pContentLabel;
    
    Label* m_pPositiveLabel;
    
    Label* m_pNegativeLabel;
    
    SEL_MenuHandler selector;
 
public:
    //just one btn
    static const int DIALOG_TYPE_SINGLE = 1;
    
    static const int DIALOG_TYPE_NEGATIVE = 2;
    

private:
    Sprite* m_pBg;
    
    DialogCallback* callback;
    
    DialogDirection direction;
    
    int dialogType;
    
    void* args;
    
    Menu *mMenu;
    
    CC_SYNTHESIZE(DialogFuncType, m_FuncType, FuncType);
    
    float m_fOffsetX;
    float m_fOffsetY;
};
#endif