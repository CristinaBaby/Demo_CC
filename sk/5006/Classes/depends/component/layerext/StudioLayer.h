//
//  StudioLayer.h
//  CodeBase
//
//  Created by zhangguangzong1 on 12/1/14.
//
//

#ifndef CODEBASE_COMPONENT_LAYEREXT_STUDIOLAYER_H_
#define CODEBASE_COMPONENT_LAYEREXT_STUDIOLAYER_H_
#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "depends/component/layerext/CFLayer.h"


#define AUDIO_SUFFIX        "_Audio"

USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace cocos2d::ui;

class StudioLayer : public CFLayer{

public:
    /**
     * Create StudioLayer.
     *
     * @deprecated use `createWithCSB` instead
     * @param jsonFile  file created by Studio v1.6
     */
    CC_DEPRECATED_ATTRIBUTE static StudioLayer* create(const string& jsonFile);
    
    /**
     * init StudioLayer.
     *
     * @deprecated use `initWithCSBFile` instead
     * @param jsonFile  file created by Studio v1.6
     */
    CC_DEPRECATED_ATTRIBUTE bool initWithFile(const string& jsonFile);
    
    /**
     * Create StudioLayer.
     *
     * @param csbFile  file created by Studio v2.x
     */
    static StudioLayer* createWithCSB(const string& csbFile);
    
    /**
     * Init StudioLayer.
     *
     * @param csbFile  file created by Studio v2.x
     */
    bool initWithCSBFile(const string& csbFile);
    
    void setBackground(const string& path);
    /*
     * setClickCallback for Buttons
     * 不传第二个回调参数时,默认绑定StudioLayer自己回调方法，子类覆写可以实现处理事件,
     * 默认方法中播放了音效。
     */
    void setClickForBtns(Node* root,Widget::ccWidgetTouchCallback callback = nullptr);
    
    /*
     * setClickCallback for CheckBoxs
     * 不传第二个回调参数时,默认绑定StudioLayer自己回调方法，子类覆写可以实现处理事件,
     * 默认方法中播放了音效。
     */
    void setClickForCheckBoxs(Node* root,CheckBox::ccCheckBoxCallback callback = nullptr);
    
    /**
     * setClickCallback for music checkbox
     */
    void setMusicClick(const string& name);
    
    /*
     * 播放节点下挂载的音效,studio v2.x版本中制作的UI文件,直接把音效挂在节点下
     * 音效节点名称 = node->getName() + "_Audio"
     */
    void playEffectByNode(Node* node);
    
    virtual void onClick(Ref* ref,Widget::TouchEventType type) final;
    virtual void onCBClick(Ref*,CheckBox::EventType) final;

    /*继承StudioLayer实现自己Layer时，覆写下面两个方法可处理点击事件*/
    virtual void onButtonClick(Ref* ref,Widget::TouchEventType type);
    virtual void onCheckBoxClick(Ref*,CheckBox::EventType);

    /**
     * setClickCallback for music checkbox
     *
     * @deprecated use `setMusicClick(const string&)` instead
     */
    CC_DEPRECATED_ATTRIBUTE void setMusicClick(const string& name,const string& sound);
    
    /**
     * setClickCallback for Buttons
     *
     * @deprecated use `setClickForBtns` instead
     */
    CC_DEPRECATED_ATTRIBUTE void setBtnClickCall(Node* parent,Widget::ccWidgetTouchCallback callback);

    template <typename T>
    inline T findViewByName(Node* root, const std::string& name, Widget::ccWidgetTouchCallback callback = nullptr) const {
        auto a = dynamic_cast<T>(Helper::seekNodeByName(root, name));
        Widget* widget = dynamic_cast<Widget*>(a);
        if(widget && callback){
            widget->addTouchEventListener(callback);
        }
        return a;
    };
    
    void setButtonCallback(Widget::ccWidgetTouchCallback fnBtnCallback)
    {
        _fnBtnCallback = fnBtnCallback;
    }
    
    void setCheckBoxCallback(CheckBox::ccCheckBoxCallback fnCbCallback)
    {
        _fnCbCallback = fnCbCallback;
    }
    
    Node* getRootNode() const
    {
        return _rootNode;
    }
    
    Sprite* getBgSprite() const
    {
        return _bgSprite;
    }
    
    timeline::ActionTimeline* getActionTimeLine() const
    {
        return _uiAction;
    }
protected:
    StudioLayer();
    virtual ~StudioLayer();
    
    
    /*直接生成StudioLayer对象时,实现下面两个回调可处理点击事件*/
    Widget::ccWidgetTouchCallback   _fnBtnCallback = nullptr;
    CheckBox::ccCheckBoxCallback    _fnCbCallback = nullptr;
    
    Node* _rootNode;
    
    Sprite* _bgSprite;
    
    /*UI Action,created by cocos studio v2.x*/
    timeline::ActionTimeline* _uiAction;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(StudioLayer);
};

#endif /* defined(CODEBASE_COMPONENT_LAYEREXT_STUDIOLAYER_H_) */
