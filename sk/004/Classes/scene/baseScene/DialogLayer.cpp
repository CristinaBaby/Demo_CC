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
    
    _dialogBg = Sprite::create("ui/popup/tips_bg.png");
    _dialogBg->setPosition(getContentSize()/2);
    layer->addChild(_dialogBg, 2);
    
    _messageLabel = Label::createWithTTF("", "ui/fonts/BRLNSDB.TTF", 20);
    _messageLabel->setAlignment(TextHAlignment::CENTER);
    _messageLabel->setMaxLineWidth(400);
    _messageLabel->setLineBreakWithoutSpace(true);
    _messageLabel->setTextColor(Color4B(250, 252, 246, 255));
    _messageLabel->setPosition(Vec2(_dialogBg->getContentSize().width/2, 270.0));
    _dialogBg->addChild(_messageLabel);
    
    auto okButton = Button::create("ui/popup/yes_btn.png");
    okButton->setPosition(Vec2(411,7));
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
        okButton->setPosition(Vec2(274,26));
        
        return true;
    }
    
    auto noButton = Button::create("ui/popup/no_btn.png");
    noButton->setPosition(Vec2(171, 14));
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
    
    TTFConfig ttfConfig("ui/fonts/BRLNSDB.TTF", fontSize, GlyphCollection::DYNAMIC);
    dialog->_messageLabel->setTTFConfig(ttfConfig);
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

