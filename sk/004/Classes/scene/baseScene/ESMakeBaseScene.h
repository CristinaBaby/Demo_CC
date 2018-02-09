//
//  ESMakeBaseScene.h
//  BHUG1064
//
//  Created by liji on 16/5/26.
//
//

#ifndef ESMakeBaseScene_h
#define ESMakeBaseScene_h

#include <stdio.h>

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "depends/component/layerext/StudioLayer.h"
#include "CMVisibleRect.h"
#include "SoundConfig.h"
#include "TipsUIController.h"
#include "ActionHelper.h"
using namespace cocos2d;
using namespace ui;
using namespace cocostudio;

#define MY_SCENE(__TYPE__) \
static Scene* scene() \
{ \
Scene *scene = Scene::create();\
__TYPE__ *layer = __TYPE__::create();\
scene->addChild(layer);\
return scene;\
}


class ESMakeBaseScene : public StudioLayer
{
public:
    
    ESMakeBaseScene();
    ~ESMakeBaseScene(){};
    
    MY_SCENE(ESMakeBaseScene);
    CREATE_FUNC(ESMakeBaseScene);
    bool init() override;
    void onExit() override;
    void onEnterTransitionDidFinish() override;
    
    
    void showNextStep();
    std::map<std::string,std::function<void()>> _steps;
    vector<std::string> _stepName;

    
    void showNextButton();
    void hideNextButton();
    std::function<void()> onNextButtonTouchCallback = nullptr;
    std::function<void()> allStepOver = nullptr;
    std::function<void(ui::Button*)> _btnClicked = nullptr;

    bool isStepOver(); //根据_ingredientsCount来判定
    
    void registTouchEvent(ui::Widget*);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

    
protected:

    int _stepCount = 0;
    Button* _nextButton;
    Button* _homeButton;
    bool isClicked;
    vector<Node*> _tempItemVec;
    int _ingredientsCount = 0;
    string _screenEventName = "";
    
    
private:
    void touchEvent(Ref *obj ,ui::Widget::TouchEventType type);

};


#endif /* ESMakeBaseScene_h */
