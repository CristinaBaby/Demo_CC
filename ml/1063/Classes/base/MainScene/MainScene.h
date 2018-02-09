//
//  MainScene.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/18.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"

#define MAIN_SCENE_DEBUG

#ifdef MAIN_SCENE_DEBUG
#define REGIST_SECENE(__TYPE__) \
static RegistHelp _registHelp##__TYPE__(#__TYPE__,std::bind(__TYPE__::create));
#else
#define REGIST_SECENE(__TYPE__)
#endif
class RegistHelp {
public:
    RegistHelp(const std::string & ,std::function<cocos2d::Layer*()>);
};

class MainScene:public cocos2d::Layer{
public:
    static void goToMainScene();
protected:
    CREATE_FUNC(MainScene);
    static cocos2d::Scene* createScene();
    static void registScene(const std::string&,std::function<cocos2d::Layer*()>);
    static void createMainSceneBtn();
protected:
    bool init();
    void touchFunc(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType);
protected:
    static std::map<std::string,std::function<cocos2d::Layer*()>> *sceneCreates;
    friend class RegistHelp;
    friend class CheckMainBtn;
};
#endif /* MainScene_hpp */
