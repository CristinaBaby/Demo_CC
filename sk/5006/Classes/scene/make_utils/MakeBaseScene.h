//
//  MakeBaseScene.h
//  BHUG1062
//
//  Created by maxiang on 5/5/16.
//
//

#ifndef MakeBaseScene_h
#define MakeBaseScene_h

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Utils.h"

using namespace cocos2d;
using namespace ui;

using namespace cocostudio;

class MakeBaseScene : public Layer
{
public:
    virtual bool init() override;
    
    void showNextButton();
    void hideNextButton();
    std::function<void()> onNextButtonTouchCallback = nullptr;
    std::function<void()> onResetButtonTouchCallback = nullptr;
    
    CC_SYNTHESIZE_READONLY(float, _designOffsetX, DesignOffsetX);
    CC_SYNTHESIZE_READONLY(float, _designOffsetY, DesignOffsetY);
    
    void setBackground(const std::string& file);
    void addNode(Node *node, float x, float y, int zorder = 2);
    
    void addChildToContentLayer(Node *node, int zorder = -1);
    Node *getChildInContentLayer(const std::string& name);
    Node *getChildInContentLayer(const int tag);
    void removeAllChidrenInContentLayer();
    void removeChidrenInContentLayerExcept(std::vector<Node*> nodes);
protected:
    MakeBaseScene():
    _nextButton(nullptr),
    _isAnimation(false),
    _bgSprite(nullptr),
    _contentLayer(nullptr),
    _resetButton(nullptr)
    {};
    virtual ~MakeBaseScene();
    
protected:
    Layer *_contentLayer;
    Button *_nextButton;
    Button *_resetButton;
    Sprite *_bgSprite;
    bool _isAnimation;
    bool _isShowMenu;
};

#endif /* MakeBaseScene_h */
