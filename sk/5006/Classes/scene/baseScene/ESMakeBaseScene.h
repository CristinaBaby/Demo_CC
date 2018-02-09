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
#include "ESScrollView.h"
#include "ESDragUnitAdapter.h"
#include "ESUnitAdapter.h"

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

typedef struct _DecorationConfig
{
public:
    _DecorationConfig()
    {};
    
    _DecorationConfig(
                      const std::string _moduleName,
                      const std::string _name,
                      const std::string _typeSelected,
                      const std::string _typeUnselected,
                      const std::string _iconFormat,
                      const std::string _fileFormat,
                      int _begin,
                      int _end,
                      int _zorder = 10)
    {
        name = _name;
        typeSelected = _typeSelected;
        typeUnselected = _typeUnselected;
        iconFormat = _iconFormat;
        fileFormat = _fileFormat;
        begin = _begin;
        end = _end;
        zorder = _zorder;
        moduleName = _moduleName;
    };
    
    /* 这个装饰的名字，对应IAP和Reward配置文件 */
    std::string moduleName;
    std::string name;
    
    /* icon 路径格式 */
    std::string iconFormat;//
    
    /* icon对应大图路径格式 */
    std::string fileFormat;
    
    /* 类别对应选中路径格式 */
    std::string typeSelected;
    
    /* 类别对应未选中路径格式 */
    std::string typeUnselected;
    
    
    /* 图片起始索引 */
    int begin;
    
    /* 图片结束索引 */
    int end;
    
    /* 装饰物品的zorder, 默认为10 */
    int zorder = 10;
}DecorationConfig;

typedef struct _ScrollViewConfig
{
public:
    _ScrollViewConfig()
    {};
    
    _ScrollViewConfig(const Size _scrollContentSize,
                      const std::string _bgName,
                      const std::string _selectedName,
                      const Vec2 &_pos,
                      const Vec2 &_posOut,
                      const float _margin
                      )
    {
        scrollContentSize = _scrollContentSize;
        bgName = _bgName;
        selectedName = _selectedName;
        pos = _pos;
        posOut = _posOut;
        margin = _margin;
    };
    
    Size scrollContentSize;
    std::string bgName;
    std::string selectedName;
    Vec2 pos;
    Vec2 posOut;
    float margin;
    
}ScrollViewConfig;

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
    
    void checkNextStep();
    void showNextStep();
    std::map<std::string,std::function<void()>> _steps;
    vector<std::string> _stepName;

    
    void showNextButton();
    void hideNextButton();
    void hideHomeButton();
    std::function<void()> onNextButtonTouchCallback = nullptr;
    std::function<void()> allStepOver = nullptr;
    std::function<void(ui::Button*)> _btnClicked = nullptr;

    bool isStepOver(); //根据_ingredientsCount来判定
    
    void registTouchEvent(ui::Widget*);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

    void createScrollView(DecorationConfig config,
                          const std::function<void(Widget *item)>& itemClickCallback,
                          ScrollViewConfig sConfig,
                          bool animated = false
                          );
    void markItemSelect(Widget *item);
    void removeScrollView(bool animated = false);
    
    void stepFinishEffect(Node* node, std::function<void()> callfuc,Vec2 pos);
    void showParticle(Node* node,Vec2 pos = Vec2::ZERO);
    
    void initTypeButton();
    void setTypeEnable(bool enable);

    void initProgressTimer();
    void removeProgressTimer();
    
    void showShop();
protected:

    int _stepCount = 0;
    Button* _nextButton;
    Button* _homeButton;
    bool isClicked;
    vector<Node*> _tempItemVec;
    int _ingredientsCount = 0;
    string _screenEventName = "";
    Widget* _selectWidget;
    Sprite* _selectBg;
    string _selectBgName;
    Node* _scrollNode;
    Vec2 _scrollPosOut;
    ESScrollView* _scrollView;
    string _iapIdentify= "";
    ProgressTimer *_progressTimer;
    Sprite *_progressBg;

    
private:
    void touchEvent(Ref *obj ,ui::Widget::TouchEventType type);
};


#endif /* ESMakeBaseScene_h */
