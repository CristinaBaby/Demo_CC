#include "TransitionScaleFade.h"

const unsigned int kSceneFade = 0xFADEFADE;

TransitionScaleFade * TransitionScaleFade::create(float duration, Scene *scene, const Color3B& color)
{
    TransitionScaleFade * transition = new TransitionScaleFade();
    transition->initWithDuration(duration, scene, color);
    transition->autorelease();
    return transition;
}

TransitionScaleFade* TransitionScaleFade::create(float duration,Scene* scene)
{
    return TransitionScaleFade::create(duration, scene, Color3B::BLACK);
}


void TransitionScaleFade::onEnter()
{
    TransitionScene::onEnter();
    
//    LayerColor* l = LayerColor::create(_color);
    _inScene->setVisible(false);
    
//    addChild(l, 2, kSceneFade);
    
//    ActionInterval* a = (ActionInterval *)Sequence::create
//    (
//     ScaleTo::create(1, 1.5),
//     FadeOut::create(2),
////     CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this)),
////     CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
//     nullptr
//     );
    
    _outScene->setCascadeColorEnabled(true);
    _outScene->getChildren().at(0)->setOpacity(0.5 * 255);
    //_outScene->runAction(a);
//    _outScene ->runAction(a);
}
