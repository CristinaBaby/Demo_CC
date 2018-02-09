#ifndef __DIALOG_H__
#define __DIALOG_H__
#include "cocos2d.h"

#include "extensions/cocos-ext.h"

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
    kDialogNotice,
    kDialogFavorite,
    kDialogAlbum,
    kDialogDelete,
    kDialogPurchased,
    kDialogRestored
};

class DialogCallback;
class Dialog:public LayerColor {
public:
    Dialog();
    
    virtual ~Dialog();
    
    virtual bool init();
    
    static Dialog* create(void*, int, DialogDirection direction, float scaleX = 0.7f, float scaleY = 0.3f, bool isImage = false, Sprite* image = nullptr);
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
public:
    virtual void setContentText(const char*);
    
    virtual void setContentTextColor(const Color3B &color);

    virtual void setPositiveBtnText(const char*);
    virtual void setNegativeBtnText(const char*);
    
    virtual void setPositiveBtnImage(const std::string &filename);
    virtual void setNegativeBtnImage(const std::string &filename);
    
    void onClick(Ref*);
    
    void refreshPosition();
    
    void setCallback(DialogCallback* callback);
    
    void setTitle(const std::string &title);
    
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
    
    MenuItemSprite* m_pPositive;
    MenuItemSprite* m_pNegative;
    
    DialogCallback* callback;
    
    DialogDirection direction;
    
    int dialogType;
    
    void* args;
    
    Menu *mMenu;
    
    CC_SYNTHESIZE(DialogFuncType, m_FuncType, FuncType);
    
    float m_fScaleX;
    float m_fScaleY;
    
    bool m_bIsImage;
    
    Sprite* m_pImage;
    
    bool m_bIsNeedTitle;
    Sprite* m_pTitle;
};


class DialogCallback{
public:
    
    virtual void onNegativeClick(void*) = 0;
    
    virtual void onPositiveClick(void*) = 0;
};
#endif