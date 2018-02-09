//
//  ActionHelper.cpp
//
//  Created by luotianqiang1 on 3/9/15.
//
//

#include "component/actions/ActionHelper.h"
#include "SoundConfig.h"
//
void ActionHelper::moveInToSceneBounce(Node* node, Vec2 endPosiont,ActionDirection direction, std::function<void()> actionEndCall)
{
    putNodeToSceneOutBorder(node, endPosiont, direction);
    Vec2 pos;
    if(direction == ActionDirection::kActionDirectionLeft) {
        if(node->isIgnoreAnchorPointForPosition())
            pos = -Vec2(node->getContentSize());
        else
            pos  = -Vec2(node->getContentSize().width*(1-node->getAnchorPoint().x),node->getContentSize().height*(1-node->getAnchorPoint().y));
        
    } else {
        if(node->isIgnoreAnchorPointForPosition())
            pos = Vec2(node->getContentSize().width+Director::getInstance()->getVisibleSize().width, -node->getContentSize().height);
        else
            pos = Vec2(Director::getInstance()->getVisibleSize().width+node->getContentSize().width*node->getAnchorPoint().x,-node->getContentSize().height*(1-node->getAnchorPoint().y));
    }
    node->setPosition(pos);
    CallFunc * endCallback = nullptr;
    if(actionEndCall != nullptr)
        endCallback = CallFunc::create(actionEndCall);
    ccBezierConfig config;
    config.endPosition = endPosiont;
    config.controlPoint_1 = config.controlPoint_2 = Vec2((pos.x+endPosiont.x)*.5,endPosiont.y+80);
    
    auto moveAction = Sequence::create(BezierTo::create(0.8, config),endCallback,NULL);
    node->runAction(moveAction);
    kAudioUtil->playEffect(kSoundItemIn);
}

void ActionHelper::putNodeToSceneOutBorder(Node* node, Vec2 endPosiont,ActionDirection direction)
{
    if(node->getParent() == nullptr)
        return;
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
    Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
    Vec2 endPostionWorld = node->getParent()->convertToWorldSpace(endPosiont);
    Vec2 positionWorld = node->getPosition();
    switch (direction) {
        case ActionDirection::kActionDirectionBottom:
            positionWorld = Vec2(endPostionWorld.x, bottomRight.y-node->getContentSize().height*(1-node->getAnchorPoint().y));
            break;
        case ActionDirection::kActionDirectionLeft:
            positionWorld = Vec2(topLeft.x -node->getContentSize().width*(1-node->getAnchorPoint().x), endPostionWorld.y);
            break;
        case ActionDirection::kActionDirectionRight:
            positionWorld = Vec2(bottomRight.x+node->getContentSize().width*node->getAnchorPoint().x, endPostionWorld.y);
            node->setPosition(bottomRight.x+node->getContentSize().width*node->getAnchorPoint().x, endPosiont.y);
            break;
        case ActionDirection::kActionDirectionTop:
            positionWorld = Vec2(endPostionWorld.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
            node->setPosition(endPosiont.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
            break;
        default:
            break;
    }
    
    node->setPosition(node->getParent()->convertToNodeSpace(positionWorld));
}

void ActionHelper::moveInToSceneExponentialOut(Node* node, Vec2 endPosiont,ActionDirection type,std::function<void()> actionEndCall)
{
    putNodeToSceneOutBorder(node, endPosiont,type);
    Vec2 pos;
    if(type == ActionDirection::kActionDirectionLeft) {
        if(node->isIgnoreAnchorPointForPosition())
            pos = -Vec2(node->getContentSize());
        else
            pos  = -Vec2(node->getContentSize().width*(1-node->getAnchorPoint().x),node->getContentSize().height*(1-node->getAnchorPoint().y));
        
    } else {
        if(node->isIgnoreAnchorPointForPosition())
            pos = Vec2(node->getContentSize().width+Director::getInstance()->getVisibleSize().width, -node->getContentSize().height);
        else
            pos = Vec2(Director::getInstance()->getVisibleSize().width+node->getContentSize().width*node->getAnchorPoint().x,-node->getContentSize().height*(1-node->getAnchorPoint().y));
    }
    node->setPosition(pos);
    CallFunc * endCallback = nullptr;
    if(actionEndCall != nullptr)
        endCallback = CallFunc::create(actionEndCall);
    ccBezierConfig config;
    config.endPosition = endPosiont;
    config.controlPoint_1 = config.controlPoint_2 = Vec2((pos.x+endPosiont.x)*.5,endPosiont.y+80);
    
    auto moveAction = Sequence::create(BezierTo::create(0.8, config),endCallback,NULL);
    node->runAction(moveAction);
    kAudioUtil->playEffect(kSoundItemIn);
}

void ActionHelper::moveOutofSceneAndRemoveSelf(Node* node, ActionDirection type, std::function<void()> actionEndCall)
{
    auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    Vec2 topLeft =   rect.origin + Vec2(0,rect.size.height);
    Vec2 bottomRight = rect.origin + Vec2(rect.size.width,0);
    Vec2 curentPosition = node->getPosition();
    Vec2 endPosiont;
    
    switch (type) {
        case ActionDirection::kActionDirectionBottom:
            endPosiont = Vec2(curentPosition.x,bottomRight.y-node->getContentSize().height*(1-node->getAnchorPoint().y));
            break;
        case ActionDirection::kActionDirectionLeft:
            endPosiont = Vec2(topLeft.x -node->getContentSize().width*(1-node->getAnchorPoint().x), curentPosition.y);
            break;
        case ActionDirection::kActionDirectionRight:
            endPosiont = Vec2(bottomRight.x+node->getContentSize().width*node->getAnchorPoint().x, curentPosition.y);
            break;
        case ActionDirection::kActionDirectionTop:
            endPosiont = Vec2(curentPosition.x, topLeft.y +node->getContentSize().width*(node->getAnchorPoint().y));
            break;
        default:
            break;
    }
    
    CallFunc * endCallback = nullptr;
    if(actionEndCall != nullptr)
        endCallback = CallFunc::create(actionEndCall);
    auto moveAction = Sequence::create(
                                       MoveTo::create(0.5, endPosiont),
                                       CallFunc::create([node] {
                                            node->removeFromParent();
                                       }),
                                       endCallback, nullptr);
    node->runAction(moveAction);
}

Animate* ActionHelper::createAnimate(const std::string &pathFormat,int startIdx,int endIdx, float interval , bool frameRevise)
{
    
    Animation* animation = Animation::create();
    
    if (endIdx > startIdx)
    {
        for (int i = startIdx; i<=endIdx; i++)
        {
            animation->addSpriteFrameWithFile(StringUtils::format(pathFormat.c_str(),i));
        }
        if (frameRevise)
        {
            for (int i = endIdx; i>=startIdx; i--)
            {
                animation->addSpriteFrameWithFile(StringUtils::format(pathFormat.c_str(),i));
            }
        }
        animation->setDelayPerUnit(interval);
    }
    else
    {
        for (int i = startIdx; i>=endIdx; i--)
        {
            animation->addSpriteFrameWithFile(StringUtils::format(pathFormat.c_str(),i));
        }
        if (frameRevise)
        {
            for (int i = endIdx+1; i<startIdx; i++)
            {
                animation->addSpriteFrameWithFile(StringUtils::format(pathFormat.c_str(),i));
            }
        }
        animation->setDelayPerUnit(interval);
        
    }
    
    return Animate::create(animation);
}



void ActionHelper::changeFade(Sprite* preSprite, Sprite* curentSprite)
{
    preSprite->stopAllActions();
    preSprite->setOpacity(255);
    curentSprite->setPosition(preSprite->getPosition());
    curentSprite->setOpacity(0);
    curentSprite->setZOrder(preSprite->getZOrder());
    preSprite->getParent()->addChild(curentSprite);
    preSprite->runAction(Sequence::create(FadeOut::create(0.5f),
                                    CallFunc::create([preSprite](){
                                        preSprite->removeFromParent();
                                        Director::getInstance()->getTextureCache()->removeTexture(preSprite->getTexture());
                                    }),
                                    nullptr));
    curentSprite->runAction(FadeIn::create(0.5f));
}


ActionInterval* ActionHelper::createFloatAction(float duration)
{
    auto move1 = MoveBy::create(duration, Vec2(0, 10));
    auto move2 = MoveBy::create(duration, Vec2(0, -15));
    auto move3 = MoveBy::create(duration, Vec2(0, 5));
    auto seq = Sequence::create(move1,move2, move3, NULL);
    
    return RepeatForever::create(seq);
}


ActionInterval* ActionHelper::createShakeAction(float distance) {
    auto seq = Sequence::create(
                                MoveBy::create(0.05f, Vec2(-distance,-distance)),
                                MoveBy::create(0.1f, Vec2(distance*2,distance*2)),
                                MoveBy::create(0.1, Vec2(-distance*2,-distance*2)),
                                MoveBy::create(0.05f, Vec2(distance,distance)),nullptr);
    return seq;
}

ActionInterval* ActionHelper::createRotatePromptLeft() {
    auto a1 = RotateTo::create(0.05, 10);
    auto a2 = RotateTo::create(0.1, -10);
    auto a3 = RotateTo::create(0.05, 0);
    auto repet = Repeat::create(Sequence::create(a1,a2,a3, nullptr), 2);
    return RepeatForever::create(Sequence::create(repet,DelayTime::create(1.5f), nullptr));
}

ActionInterval* ActionHelper::createRotatePromptRight() {
    auto a1 = RotateTo::create(0.05, -10);
    auto a2 = RotateTo::create(0.1, 10);
    auto a3 = RotateTo::create(0.05, 0);
    auto repet = Repeat::create(Sequence::create(a1,a2,a3, nullptr), 2);
    return RepeatForever::create(Sequence::create(repet,DelayTime::create(1.5f), nullptr));
}

ActionInterval* ActionHelper::createRoteAction(float rote) {
    auto a1 = RotateTo::create(0.05, rote);
    auto a2 = RotateTo::create(0.1, -rote);
    auto a3 = RotateTo::create(0.05, 0);
    auto repeat = Repeat::create(Sequence::create(a1,a2,a3, nullptr), 2);
    return repeat;
}

ActionInterval* ActionHelper::createMovePrompt() {
    return Repeat::create(Sequence::create(MoveBy::create(0.25f, Vec2(-7,-7)), MoveBy::create(0.25f, Vec2(7,7)),nullptr),2);
}

ActionInterval* ActionHelper::moveToPostionByBezier(Vec2 startPos,Vec2 endPo,float duration)
{
    int delta = 1;
    delta = startPos.x>endPo.x?-1:1;
    
    auto config = ccBezierConfig();
    config.endPosition = endPo;
    config.controlPoint_1 = config.controlPoint_2 = Vec2(startPos.x+200*delta,startPos.y+250);
    
    return BezierTo::create(duration, config);
}


void ActionHelper::runMoveHideAction2(Node* node) {
    if(nullptr != node) {
        auto action = Sequence::create(CallFunc::create([node](){node->setVisible(true);}),createMovePrompt(),CallFunc::create([node](){node->setVisible(false);}),DelayTime::create(3) ,nullptr);
        node->runAction(RepeatForever::create(action));
    }
}

void ActionHelper::runMoveHideAction(Node* node,Vec2 startPos,Vec2 endPo,float time) {
    auto move = MoveTo::create(time,endPo);
    auto se = Sequence::create(
                               CallFunc::create([node,startPos](){node->setPosition(startPos); node->setVisible(true);}),
                               move,
                               CallFunc::create([node](){node->setVisible(false);}),
                               DelayTime::create(2.7f),nullptr);
    node->runAction(RepeatForever::create(se));
}




ActionInterval* ActionHelper::createRollJelly()
{
    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.2f,1.01f,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
    return Sequence::create(s1,s2,s3, nullptr);
}

ActionInterval* ActionHelper::createScalePop()
{

//    ScaleTo* s1 = ScaleTo::create(0,0);
    ScaleTo* s2 = ScaleTo::create(0.2f,1.03f);
    ScaleTo* s3 = ScaleTo::create(0.2f,0.98f);
    ScaleTo* s4 = ScaleTo::create(0.2f,1.f);
    return Sequence::create(s2,s3,s4, nullptr);
}

ActionInterval* ActionHelper::createScaleHint()
{
    return RepeatForever::create(Sequence::create(ScaleTo::create(0.4, 1.2), ScaleTo::create(0.4, 1.0), NULL));
}


