//
//  DialogLayer.cpp
//
//
//  Created by maxiang on 2/23/16.
//
//

#include "DialogLayer.h"
#include "component/makeComponent/basicComponent/ComponentCycleScroll.h"

USING_NS_CC;
using namespace ui;

#define FontPath "ui/font/Chewy.ttf"
DialogLayer::~DialogLayer()
{
    
}

DialogLayer* DialogLayer::create(bool confirmOnly)
{
    DialogLayer* pRet = new DialogLayer();
    if (pRet && pRet->init(confirmOnly))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
};

bool DialogLayer::init(bool confirmOnly)
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layer = LayerColor::create(Color4B(0, 0, 0, 200));
    addChild(layer, 1);
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    
    _dialogBg = Sprite::create("ui/popup/bg.png");
    _dialogBg->setPosition(getContentSize()/2);
    layer->addChild(_dialogBg, 2);
    
    _messageLabel = Label::createWithTTF("", FontPath, 20);
    _messageLabel->setAlignment(TextHAlignment::CENTER);
    _messageLabel->setMaxLineWidth(_dialogBg->getContentSize().width-100);
    _messageLabel->setLineBreakWithoutSpace(true);
    _messageLabel->setTextColor(Color4B(250, 252, 246, 255));
    _messageLabel->setPosition(Vec2(_dialogBg->getContentSize().width/2, 370.0));
    _dialogBg->addChild(_messageLabel);
    
    auto okButton = Button::create("ui/popup/yes_btn.png");
    okButton->setPosition(Vec2(147,71));
    okButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if(type == Widget::TouchEventType::BEGAN){
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreTouchOnceNotification);
        }
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (_dismissAnimation)
            {
                _dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                    
                    if (_okCallback)
                        _okCallback();
                    removeFromParent();
                }), NULL));
            }
            else
            {
                if (_okCallback)
                    _okCallback();
                removeFromParent();
            }
        }
    });
    _dialogBg->addChild(okButton);
    
    if (confirmOnly)
    {
        okButton->loadTextureNormal("ui/popup/ok_btn.png");
        okButton->setPosition(Vec2(268,70));
        
        return true;
    }
    
    auto noButton = Button::create("ui/popup/no_btn.png");
    noButton->setPosition(Vec2(398,70));
    noButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if(type == Widget::TouchEventType::BEGAN){
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreTouchOnceNotification);
        }
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (_dismissAnimation)
            {
                _dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                    
                    if (_noCallback)
                        _noCallback();
                    removeFromParent();
                }), NULL));
            }
            else
            {
                if (_noCallback)
                    _noCallback();
                removeFromParent();
            }
        }
    });
    _dialogBg->addChild(noButton);
    
    return true;
}

DialogLayer* DialogLayer::showWithMessage(const std::string& message,
                                  const std::function<void()>& okCallback,
                                  const std::function<void()>& noCallback,
                                  Node *container,
                                  const float fontSize,
                                  bool dismissAnimation,
                                  bool confirmOnly)
{
    CCASSERT(container, "container must not be nil!");
    
    auto dialog = DialogLayer::create(confirmOnly);
    dialog->setupWithMessage(message, okCallback, noCallback);
    container->addChild(dialog, 100,129);
    dialog->_dismissAnimation = dismissAnimation;
    
    TTFConfig ttfConfig(FontPath, fontSize, GlyphCollection::DYNAMIC);
    dialog->_messageLabel->setTTFConfig(ttfConfig);
    dialog->_messageLabel->setTextColor(Color4B(119,60,40,255));
    dialog->show();
    
    return dialog;
}



void DialogLayer::show()
{
    _dialogBg->setScale(0.0);
    _dialogBg->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7));
}

void DialogLayer::setupWithMessage(const std::string& message,
                                   const std::function<void()>& okCallback,
                                   const std::function<void()>& noCallback)
{
    _messageLabel->setString(message);
    
    _okCallback = okCallback;
    _noCallback = noCallback;
}


void DialogLayer::updateBg(const std::string& file)
{
    _dialogBg->setTexture(file);
}

