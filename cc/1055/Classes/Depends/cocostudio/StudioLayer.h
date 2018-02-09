//
//  StudioLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#ifndef StudioLayer_hpp
#define StudioLayer_hpp
#include"cocos2d.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
#include "DynBase.h"
USING_NS_CC;
using namespace cocostudio;
using namespace std;
class StudioLayer:public Layer,public DynObject{
    DECLARE_DYNCLASS(StudioLayer);
    CC_SYNTHESIZE(bool, isChange, IsChange);
public:
    StudioLayer();
    virtual ~StudioLayer();
    CREATE_FUNC(StudioLayer);
    static StudioLayer* create(string _filePath);
    static StudioLayer* createNoLayout(string _filePath);
    void playAction(string name,std::function<void()> _endFun = nullptr,bool _loop = false);
    timeline::ActionTimeline *getAction();
    template<class T> T getNodeByName(string _name){
        return static_cast<T>(ui::Helper::seekNodeByName(_root, _name));
    }
    Node* getNodeByName(string);
    void changeNode(string name,string path);
    void changeNode(string name,Texture2D* _tex);
    void changeNode(string name,SpriteFrame*);
    void changeNode(string name,Sprite*);
    Vec2 getPosinWord(string name);
    int playNodeSound(const string& nodeName);
      virtual void touchBtn(Ref*,ui::Widget::TouchEventType);
    
protected:
    virtual bool init();
    virtual void initWithFile(string _filePath);
    virtual void initWithFileNoLayout(string _filePath);
    
    virtual void clickBtn(ui::Widget*){}
    void registLisnter(Node*);
    void removeLesner(Node*);
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    std::function<void()> actionEndFuc;
protected:
    Node* _root;
    timeline::ActionTimeline *uiAction;
    std::string _fileName;
public:
    std::function<void(ui::Widget* widget)> _touchBtnCallback;
};

#endif /* StudioLayer_hpp */
