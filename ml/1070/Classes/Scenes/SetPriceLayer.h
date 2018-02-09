
#ifndef __SetPriceLayer__
#define __SetPriceLayer__


#include "cocos2d.h"
#include "IAPItem.h"
#include "Heads.h"
#include "ScribbleShapeNode.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

USING_NS_CC;


class SetPriceLayer : public Layer, public EditBoxDelegate
{
public:
    SetPriceLayer();
    ~SetPriceLayer();
    
    CREATE_FUNC(SetPriceLayer);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void setTagWorldPosition(Vec2 pos){
        m_pos = pos;
    }
    void showPriceTag();
    
    void onButtonCallback(Ref*,Widget::TouchEventType type);
    std::function<void(float)> onPriceSetCallbak;
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::EditBox *editBox);
    void toExtensionsMainLayer(cocos2d::Ref *sender);
    void update(float);
protected:
    Vec2 m_pos;
    const string pattern = "\\d*\\.?\\d*";
    Sprite* m_pBg;
    Label* m_pExText;
    PriceEditBox* m_pText;
    Button* m_pBtnOK;
    Button* m_pBtnCancel;
    std::string currentTex;
};

#endif