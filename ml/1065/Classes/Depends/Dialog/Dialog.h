#ifndef __DIALOG_H__
#define __DIALOG_H__
#include "cocos2d.h"
#include "DialogCallback.h"
#include "ui/CocosGUI.h"
#include "cocostudio/cocostudio.h"
#include "extensions/cocos-ext.h"



using namespace cocostudio;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
USING_NS_CC;

class Dialog:public LayerColor{
public:
	//just one btn
    static const int DIALOG_TYPE_SINGLE=1;
    static const int DIALOG_TYPE_NEGATIVE=2;
	Dialog();
	virtual ~Dialog();
	virtual bool init();
	static Dialog* create(cocos2d::Size size,void*,int);
	virtual void setContentText(const char* text);
	virtual void setPositiveBtnText(const char*);
    virtual void setNegativeBtnText(const char*);
    virtual void setSelectBtn(const char*,const char*);
	virtual void setTitleText(const char*);
    
    
	void refreshPosition();
    
    std::function<void(void*)> onNegativeClick;
    std::function<void(void*)> onPositiveClick;
protected:
    void _onUIButtonCallback(Ref* ref,Widget::TouchEventType type);
    
    void onClick(Ref*);
	void onBack();
    void dismiss();
    Scale9Sprite* bg;
	Label* title_label;
	Layer* contentLayer;
	Label* content_label;
	Label* positive_label;
	Label* negative_label;
	Node* parent;
	SEL_MenuHandler selector;
private:
	cocos2d::Size dialogSize;
	int dialogType;
	void* args;
    Menu* mMenu;
    
    bool m_bIsCocos;
    cocos2d::ui::Widget* m_pUL;
    
};
#endif