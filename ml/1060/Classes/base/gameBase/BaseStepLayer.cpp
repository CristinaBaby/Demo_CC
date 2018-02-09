#include "BaseStepLayer.h"
#include "KeyBackEvent.h"
using namespace cocos2d::ui;

 std::function<bool()> BaseStepLayer::keyBack = nullptr;

bool BaseStepLayer::onBack(){
    if(keyBack != nullptr)
        return keyBack();
    return false;
}

BaseStepLayer::BaseStepLayer(){
    _loopSound = -1;
    _operate = nullptr;
}
BaseStepLayer::~BaseStepLayer(){
    if(_loopSound != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(_loopSound);
        _loopSound = -1;
    }
}

bool BaseStepLayer::init(){
    if(Layer::init()) {
        _prompt = Sprite::create("hand.png");
        if(_prompt != nullptr){
            _prompt->setVisible(false);
            addChild(_prompt,300);
        }
        return true;
    }
    return false;
}

void BaseStepLayer::stopPrompt(){
    if(_prompt != nullptr){
        _prompt->stopAllActions();
        _prompt->setVisible(false);
        _prompt->setRotation(0);
    }
}

void BaseStepLayer::onEnterTransitionDidFinish() {
    Layer::onEnterTransitionDidFinish();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    KeyBackEvent::getInstance()->addBackEvent(std::bind(&BaseStepLayer::onBack, this), this);
    if(_operate != nullptr && _operate->_touchBtnCallback == nullptr)
        _operate->_touchBtnCallback = CC_CALLBACK_1(BaseStepLayer::touchEnd, this);
    if(_operate != nullptr){
        auto _next = _operate->getNodeByName("btn_next");
        if(_next != nullptr){
            _next->stopAllActions();
            _next->setRotation(0);
            _next->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),Spawn::create(JumpBy::create(1, Vec2::ZERO, 15, 2),RotateBy::create(1, 360), nullptr), nullptr)));
        }
    }
}

void BaseStepLayer::touchEnd(ui::Widget* widget) {
    
}
