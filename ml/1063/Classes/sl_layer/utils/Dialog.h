#ifndef __DIALOG_H__
#define __DIALOG_H__
#include "cocos2d.h"
#include "DialogCallback.h"
#include "Utils.h"
#include "extensions/cocos-ext.h"


//USING_NS_CC;
//USING_NS_CC_EXT;

//#include "GameManager.h"

//USING_NS_CC_EXT;
USING_NS_CC;
enum DialogFuncType {
    kDialogTypeNone = 100,
    kDialogReturnHome,
    kDialogReset,
    kDialogExit,
    kDialogNotice
};

class Dialog:public LayerColor{
public:
	//just one btn
    static const int DIALOG_TYPE_SINGLE=1;
    static const int DIALOG_TYPE_NEGATIVE=2;
	Dialog();
	virtual ~Dialog();
	virtual bool init();
	static Dialog* create(Size ,void*,int);
	virtual void setContentText(const char* text);
	virtual void setPositiveBtnText(const char*);
	virtual void setNegativeBtnText(const char*);
	virtual void setTitleText(const char*);
	void onClick(Ref*);
	void refreshPosition();
    void setCallback(DialogCallback* callback);
    
    
    virtual void onEnter();
    
    virtual void onEnterTransitionDidFinish();

    virtual void onExit();
    
protected:
	void onBack();
	void dismiss();
	Label* title_label;
	Layer* contentLayer;
	Label* content_label;
	Label* positive_label;
	Label* negative_label;
	Node* parent;
	SEL_MenuHandler selector;
private:
    DialogCallback* callback;
	Size dialogSize;
	int dialogType;
	void* args;
    Menu *mMenu;
    Sprite* m_pBg;
    
    CC_SYNTHESIZE(DialogFuncType, m_FuncType, FuncType);
};
#endif