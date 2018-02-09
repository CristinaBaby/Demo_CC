#include "TransitionFadeEx.h"

const unsigned int kSceneFade = 0xFADEFADE;

TransitionFadeEx * TransitionFadeEx::create(float duration, Scene *scene, const Color3B& color)
{
    TransitionFadeEx * transition = new TransitionFadeEx();
    transition->initWithDuration(duration, scene, color);
    transition->autorelease();
    return transition;
}

TransitionFadeEx* TransitionFadeEx::create(float duration,Scene* scene)
{
    return TransitionFadeEx::create(duration, scene, Color3B::BLACK);
}


void TransitionFadeEx :: onEnter()
{
    TransitionScene::onEnter();
    
    LayerColor* l = LayerColor::create(_color);
    _inScene->setVisible(false);
    
    addChild(l, 2, kSceneFade);
    Node* f = getChildByTag(kSceneFade);
    
    ActionInterval* a = (ActionInterval *)Sequence::create
    (
     FadeIn::create(1),
     CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this)),
     EaseExponentialIn::create(FadeOut::create(3)),
     CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
     nullptr
     );
    f->runAction(a);
}
