//
//  ActionHelper.cpp
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/9/15.
//
//

#include "ActionHelper.h"
int ActionHelper::titleActionTag = 9112;
 void ActionHelper::showBackInOut(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd ,float time){
     setStartPostion(node, endPosiont,type);
     CallFunc * endCallback = nullptr;
     if(actionEnd != nullptr)
         endCallback = CallFunc::create(actionEnd);
     auto moveAction = Sequence::create(EaseBackInOut::create(MoveTo::create(time, endPosiont)),endCallback,nullptr);
     node->runAction(moveAction);
     
}

void ActionHelper::showBouce(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd,float time){
    setStartPostion(node, endPosiont,type);
    CallFunc * endCallback = nullptr;
    if(actionEnd != nullptr)
        endCallback = CallFunc::create(actionEnd);
    auto moveAction = Sequence::create(EaseBounceOut::create(MoveTo::create(time, endPosiont)),endCallback,NULL);
    node->runAction(moveAction);
}

void ActionHelper::setStartPostion(Node* node, Vec2 endPosiont,ShowDirection type){
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
    Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
    Vec2 _pos;
    switch (type) {
        case ShowDirection::show_from_bottom:
            _pos = Vec2(endPosiont.x,bottomRight.y-node->getContentSize().height*(1-node->getAnchorPoint().y));
     
            break;
        case ShowDirection::show_from_left:
            _pos = Vec2(topLeft.x -node->getContentSize().width*(1-node->getAnchorPoint().x), endPosiont.y);
            break;
        case ShowDirection::show_from_right:
            _pos = Vec2(bottomRight.x+node->getContentSize().width*node->getAnchorPoint().x, endPosiont.y);
            break;
        case ShowDirection::show_from_top:
            _pos = Vec2(endPosiont.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
            break;
    }
    auto _parent = node->getParent();
    if(_parent != nullptr)
        _pos = _parent->convertToNodeSpace(_pos);
    node->setPosition(_pos);
}

void ActionHelper::showRote(Node* node, Vec2 endPosiont,ShowDirection type, std::function<void()> actionEnd,float time) {
    show(node,endPosiont,type,[=](){
        auto p = node->convertToWorldSpace(Vec2(node->getContentSize().width*.5,0));
        node->setAnchorPoint(Vec2(0.5, 0));
        node->setPosition(node->getParent()->convertToNodeSpace(p));
        node->runAction(Sequence::create(EaseExponentialOut::create(RotateBy::create(0.1, -15)), EaseExponentialIn::create(RotateBy::create(0.5, 15)),nullptr));
        if(nullptr != actionEnd)
            actionEnd();
    },time,0.8);
}

void ActionHelper::show(Node* node, Vec2 endPosiont,ShowDirection type,std::function<void()> actionEnd,float time,float percent){
    setStartPostion(node, endPosiont,type);
    CallFunc * endCallback = nullptr;
    if(actionEnd != nullptr)
        endCallback = CallFunc::create(actionEnd);
    auto moveAction = Sequence::create(EaseElasticInOut::create(MoveTo::create(time, endPosiont),percent),endCallback,NULL);
    node->runAction(moveAction);
    
}

void ActionHelper::hide(Node* node,ShowDirection type, std::function<void()> actionEnd,bool _isRemove,float _time) {
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
    Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
    Vec2 curentPosition = node->getPosition();
    Vec2 endPosiont;
    
    switch (type) {
        case ShowDirection::show_from_bottom:
            endPosiont = Vec2(curentPosition.x,bottomRight.y-node->getContentSize().height*(1-node->getAnchorPoint().y));
            break;
        case ShowDirection::show_from_left:
            endPosiont = Vec2(topLeft.x -node->getContentSize().width*(1-node->getAnchorPoint().x), curentPosition.y);
            break;
        case ShowDirection::show_from_right:
            endPosiont = Vec2(bottomRight.x+node->getContentSize().width*node->getAnchorPoint().x, curentPosition.y);
            break;
        case ShowDirection::show_from_top:
            endPosiont = Vec2(curentPosition.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
            break;
    }
    if(node->getParent() != nullptr)
        endPosiont = node->getParent()->convertToWorldSpace(endPosiont);
    CallFunc * endCallback = nullptr;
    if(actionEnd != nullptr)
        endCallback = CallFunc::create(actionEnd);
    auto moveAction = Sequence::create(EaseExponentialIn::create(MoveTo::create(_time, endPosiont)), CallFunc::create([node,_isRemove] {
        if(_isRemove)
            node->removeFromParent();
    }),endCallback,NULL);
    node->runAction(moveAction);

}

void ActionHelper::changeBouce(Sprite* preSprite, Sprite* curentSprite) {
    auto bg2 = preSprite;
    bg2->stopAllActions();
    curentSprite->setVisible(false);
    curentSprite->setZOrder(preSprite->getZOrder());
    bg2->getParent()->addChild(curentSprite);
    
    ActionHelper::showBouce(curentSprite, preSprite->getPosition(), ActionHelper::ShowDirection::show_from_top,[bg2]{
        Director::getInstance()->getTextureCache()->removeTexture(bg2->getTexture());
         bg2->removeFromParent();
    });
    curentSprite->setVisible(true);
}

void ActionHelper::changeFade(Sprite* preSprite, Sprite* curentSprite) {
    if(preSprite->getTexture() == curentSprite->getTexture())
        return;
    auto tempPre = preSprite;
    tempPre->setCascadeOpacityEnabled(false);
    tempPre->setOpacity(0);
    tempPre->stopAllActions();
    tempPre->removeAllChildren();
    auto _s1 = Sprite::createWithSpriteFrame(tempPre->getSpriteFrame());
    _s1->setPosition(tempPre->getContentSize()*.5);
    tempPre->addChild(_s1);
    _s1->setCascadeOpacityEnabled(false);
    
    curentSprite->removeFromParent();
    curentSprite->setOpacity(0);
    curentSprite->setPosition(tempPre->getContentSize()*.5);
    tempPre->addChild(curentSprite);
    curentSprite->setCascadeOpacityEnabled(false);
    
    _s1->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=](){
        if(tempPre->getTexture() != curentSprite->getTexture())
            Director::getInstance()->getTextureCache()->removeTexture(tempPre->getTexture());
        tempPre->setSpriteFrame(curentSprite->getSpriteFrame());
        tempPre->removeAllChildren();
        tempPre->setOpacity(255);
    }), nullptr));
    curentSprite->runAction(FadeIn::create(0.5f));
}

ActionInterval* ActionHelper::createShakeAction(float distance,float time ) {
      auto seq = Sequence::create(MoveBy::create(time, Vec2(-distance,-distance)), MoveBy::create(time*2, Vec2(distance*2,distance*2)),MoveBy::create(time*2, Vec2(-distance*2,-distance*2)),MoveBy::create(time, Vec2(distance,distance)),nullptr);
    return seq;
}



ActionInterval* ActionHelper::createRotePrompt() {
    auto a1 = RotateTo::create(0.05, 10);
    auto a2 = RotateTo::create(0.1, -10);
    auto a3 = RotateTo::create(0.05, 0);
    auto repet = Repeat::create(Sequence::create(a1,a2,a3, nullptr), 2);
    return RepeatForever::create(Sequence::create(repet,DelayTime::create(2), nullptr));
}

 ActionInterval* ActionHelper::createJumpScale(){
     return Spawn::create(JumpBy::create(0.8, Vec2(0, 0), 20, 2),Sequence::create(ScaleTo::create(0.4, -1,1),ScaleTo::create(0.4, 1,1),nullptr),nullptr);
}

ActionInterval* ActionHelper::createRoteAction(float rote,float time) {
    auto a1 = RotateTo::create(time, rote);
    auto a2 = RotateTo::create(time*2, -rote);
    auto a3 = RotateTo::create(time, 0);
    auto repeat = Repeat::create(Sequence::create(a1,a2,a3, nullptr), 2);
    return repeat;
}

ActionInterval* ActionHelper::createMovePrompt() {
    return Repeat::create(Sequence::create(MoveBy::create(0.25f, Vec2(-7,-7)), MoveBy::create(0.25f, Vec2(7,7)),nullptr),2);
}

ActionInterval* ActionHelper::createUpDownPrompt(){
    return RepeatForever::create(Sequence::create(MoveBy::create(0.35, Vec2(0,10)), MoveBy::create(0.35, Vec2(0,-10)),  nullptr));
}

ActionInterval* ActionHelper::createScaleAction() {
    return RepeatForever::create(Sequence::create(ScaleTo::create(0.8f, 1.1f), ScaleTo::create(0.8f, 0.9f),nullptr));
}

 void ActionHelper::runMoveHideAction2(Node* node) {
     if(nullptr != node) {
        auto action = Sequence::create(CallFunc::create([node](){node->setVisible(true);}),createMovePrompt(),CallFunc::create([node](){node->setVisible(false);}),DelayTime::create(3) ,nullptr);
         node->runAction(RepeatForever::create(action));
     }
}

void ActionHelper::runMoveHideAction(Node* node,Vec2 startPos,Vec2 endPo,float time) {
    auto move = MoveTo::create(time,endPo);
    auto se = Sequence::create(CallFunc::create([node,startPos](){node->setPosition(startPos); node->setVisible(true);}), move,CallFunc::create([node](){node->setVisible(false);}), DelayTime::create(2.7f),nullptr);
    node->runAction(RepeatForever::create(se));
}

void ActionHelper::runBezierMoveHideAction(Node* node,Vec2 startPos,Vec2 endPo,float time) {
    node->setVisible(true);
    ccBezierConfig _config;
    _config.endPosition = endPo;
    _config.controlPoint_1 = _config.controlPoint_2 = Vec2((startPos.x+ endPo.x)*.5,std::max(startPos.y, endPo.y)+160);
    auto move = BezierTo::create(time, _config);
    auto se = Sequence::create(MoveTo::create(0, startPos),FadeTo::create(0, 255), move,FadeTo::create(0, 0), DelayTime::create(2.7f),nullptr);
    node->runAction(RepeatForever::create(se));
}

 void ActionHelper::runRoteHideAction(Node* node, float _endRote,float time){
 
     auto _rote = RotateTo::create(time,_endRote);
     auto se = Sequence::create(CallFunc::create([node](){ node->setRotation(0); node->setVisible(true);}), _rote,RotateTo::create(0.1, 0), _rote,CallFunc::create([node](){node->setVisible(false);}), DelayTime::create(2.7f),nullptr);
     node->runAction(RepeatForever::create(se));
}

ActionInterval *ActionHelper::getJellyAction()
{
    ActionInterval *ret = NULL;
    
    ScaleTo *lScale1 = ScaleTo::create(0.13, 1.0, 0.90);
    ScaleTo *lScale2 = ScaleTo::create(0.11, 0.92, 1.0);
    ScaleTo *lScale3 = ScaleTo::create(0.10, 1.0, 0.96);
    ScaleTo *lScale4 = ScaleTo::create(0.09, 0.98, 1.0);
    ScaleTo *lScale5 = ScaleTo::create(0.08, 1.0, 0.99);
    ScaleTo *lScale6 = ScaleTo::create(0.07, 0.99, 1.0);
    
    ret = Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5, lScale6, NULL);
    
    return ret;
}

ActionInterval* ActionHelper::createJumpForever(){
    return RepeatForever::create(Sequence::create(DelayTime::create(2), JumpBy::create(0.8, Vec2(0, 0), 20, 2), nullptr));
}

ActionInterval *ActionHelper::createRollJelly(){
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    return RepeatForever::create( Sequence::create(s1,s2,s3, nullptr));
}

void ActionHelper::delayFunc(float _time,Node* _node,std::function<void()> _func){
    _node->runAction(Sequence::create(DelayTime::create(_time), CallFunc::create(_func), nullptr));
}

void ActionHelper::showTitle(Node* _node){
    _node->stopActionByTag(titleActionTag);
    showBackInOut(_node, _node->getPosition(), ActionHelper::ShowDirection::show_from_left,[_node](){
        auto _action = RepeatForever::create(Sequence::create(DelayTime::create(2), createShakeAction(), createShakeAction(),nullptr));
        _action->setTag(titleActionTag);
        _node->runAction(_action);
    });
    _node->setVisible(true);
}

void ActionHelper::showScaleArrow(Node* _node){
    auto _x = _node->getScaleX();
    auto _y = _node->getScaleY();
    _node->runAction(RepeatForever::create(Sequence::create( ScaleTo::create(0.4, 1,1), ScaleTo::create(0.4, _x,_y), nullptr)));
//    _node->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0, _x,_y), FadeTo::create(0, 255),Repeat::create(Sequence::create( ScaleTo::create(0.4, 1,1), ScaleTo::create(0.4, _x,_y),nullptr),2),FadeTo::create(0, 0),DelayTime::create(2), nullptr)));
    _node->setVisible(true);
}

