//
//  MainScene.cpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/3/18.
//
//

#include "MainScene.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;
std::map<std::string,std::function<cocos2d::Layer*()>> *MainScene::sceneCreates = nullptr;

class CheckMainBtn : public Ref{
private:
    static CheckMainBtn* instance;
    CheckMainBtn() {
        auto _schedu = Director::getInstance()->getScheduler();
        _schedu->schedule([this](float){
            MainScene::createMainSceneBtn();
        }, this, 0.0, CC_REPEAT_FOREVER,0.7,false, "CheckMainBtn");
    }
public:
    static CheckMainBtn* getInstance() {
        if(instance == nullptr)
            instance = new CheckMainBtn();
        return instance;
        
    }
};
CheckMainBtn* CheckMainBtn::instance = nullptr;
RegistHelp::RegistHelp(const string & str,std::function<cocos2d::Layer*()> func){
#ifdef MAIN_SCENE_DEBUG
    if(func != nullptr){
        MainScene::registScene(str, func);
    }
#endif
}

void MainScene::goToMainScene(){
    CheckMainBtn::getInstance();
    auto scene = createScene();
    Director::getInstance()->popToRootScene();
    Director::getInstance()->replaceScene(scene);
}


Scene* MainScene::createScene() {
    Scene* _scene = Scene::create();
    _scene->addChild(MainScene::create());
    return _scene;
}

bool MainScene::init(){
    if(Layer::init()){
        if(sceneCreates != nullptr) {
            auto wordSize = Director::getInstance()->getVisibleSize();
            auto _listView = ui::ListView::create();
            _listView->setContentSize(wordSize);
            _listView->setDirection(ListView::Direction::VERTICAL);
            _listView->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
            _listView->setItemsMargin(20);
            this->addChild(_listView);
            
            for(auto itor = sceneCreates->begin();itor != sceneCreates->end();itor++){
                auto btn = Button::create();
                btn->setName(itor->first);
                btn->setTitleText(itor->first);
                btn->setTitleFontSize(30);
                _listView->pushBackCustomItem(btn);
                btn->addTouchEventListener(CC_CALLBACK_2(MainScene::touchFunc,this));
            }
            
        }
        
        return true;
    }
    return false;
}

void MainScene::registScene(const string& str,std::function<cocos2d::Layer*()> scene){
    do {
        CC_BREAK_IF(!scene);
        if(sceneCreates == nullptr)
            sceneCreates = new std::map<std::string,std::function<cocos2d::Layer*()>>();
        CC_BREAK_IF(sceneCreates->find(str) != sceneCreates->end());
        sceneCreates->insert(pair<std::string,function<cocos2d::Layer*()>>(str,scene));
    }while (false);
}

void  MainScene::createMainSceneBtn(){
    auto scene = Director::getInstance()->getRunningScene();
    if(scene->getChildByName("MainScene")!= nullptr)
        return;
    auto btn = Button::create();
    btn->setName("MainScene");
    btn->setTitleText("MainScene");
    btn->setTitleFontSize(30);
    btn->setTitleColor(Color3B::BLACK);
    auto size = Director::getInstance()->getVisibleSize();
    btn->setPosition(Vec2(btn->getContentSize().width*.5+20,size.height-btn->getContentSize().height*.5-20));
    btn->addTouchEventListener([](cocos2d::Ref* ref,ui::Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            goToMainScene();
        }
    });
    scene->addChild(btn,9999);
}

void MainScene::touchFunc(cocos2d::Ref* ref,ui::Widget::TouchEventType type){
    do {
        CC_BREAK_IF(type != Widget::TouchEventType::ENDED);
        CC_BREAK_IF(!sceneCreates);
        auto node = dynamic_cast<Widget*>(ref);
        CC_BREAK_IF(!node);
        auto itor = sceneCreates->find(node->getName());
        CC_BREAK_IF(itor == sceneCreates->end());
        auto scene = Scene::create();
        scene->addChild(itor->second());
        Director::getInstance()->popToRootScene();
        Director::getInstance()->replaceScene(scene);
    }while (false);
    
}