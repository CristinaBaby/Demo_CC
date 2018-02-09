//
//  TransitionSceneEx.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "TransitionFadeEx.h"

const unsigned int kSceneFade = 0xFADEFADE;

void TransitionFadeEx::onEnter()
{
    TransitionScene::onEnter();
    
    LayerColor* l = LayerColor::create(_color);
    _inScene->setVisible(false);
    
    addChild(l, 2, kSceneFade);
    Node* f = getChildByTag(kSceneFade);
    
    ActionInterval* a = (ActionInterval *)Sequence::create
    (
     FadeIn::create(_duration/2),
     CallFunc::create(CC_CALLBACK_0(TransitionFadeEx::myHideOutShowIn,this)),
     FadeOut::create(_duration/2),
     CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
     
     nullptr
     );
    f->runAction(a);
}

TransitionFadeEx * TransitionFadeEx::create(float duration, Scene *scene, const Color3B& color)
{
    TransitionFadeEx * transition = new TransitionFadeEx();
    transition->initWithDuration(duration, scene, color);
    transition->autorelease();
    return transition;
}

void TransitionFadeEx::myHideOutShowIn()
{
    TransitionScene::hideOutShowIn();
    EventCustom event("abc");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("abc");
}
