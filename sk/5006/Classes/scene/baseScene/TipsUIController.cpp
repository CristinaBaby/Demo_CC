//
//  TipsUIController.cpp
//  BHUG1062
//
//  Created by tanshoumei on 5/16/16.
//
//

#include "TipsUIController.h"
#include "component/actions/ActionHelper.h"

enum TipUITag
{
    kTipUITagClick,
    kTipUITagRotateDegree,
    kTipUITagAccelarate,
    kTipUITagMove
};

Node* TipsUIController::showClickTip(Node* parent, const Vec2& pointLocal)
{
    removeClickTip();
    Sprite* finger = Sprite::create(_rootPathRes + "finger1.png");
//    finger->setAnchorPoint(Vec2(0.25, 0.85));
    finger->setPosition(pointLocal);
    parent->addChild(finger,999);
    
//    auto aniSprite = Sprite::create(_rootPathRes + "chick.png");
//    finger->addChild(aniSprite, -1);
//    aniSprite->setPosition(Vec2(52,168));
    auto animate = ActionHelper::createAnimate(_rootPathRes + "finger2_%d.png", 0, 1,0.2);
//    aniSprite->runAction(ActionHelper::createScaleHint());
    finger->runAction(RepeatForever::create(animate));
    _mapTipsNode.insert(kTipUITagClick, finger);
    return finger;
}

void TipsUIController::removeClickTip()
{
    removeTipNode(kTipUITagClick);
}

Node* TipsUIController::showMoveTip(Node* parent, const Vec2& startPos , const Vec2& endPos)
{
    Sprite* finger = Sprite::create(_rootPathRes + "finger2_0.png");
    finger->setPosition(startPos);
    parent->addChild(finger,99);
    
    finger->runAction(RepeatForever::create(Sequence::create(Show::create(),
                                                             MoveTo::create(0.5, endPos),
                                                             DelayTime::create(0.5),
                                                             Place::create(startPos),
                                                             Hide::create(),
                                                             NULL)));
    _mapTipsNode.insert(kTipUITagMove, finger);
    return finger;

}

Node* TipsUIController::showMoveTipBezel(Node* parent, const Vec2& startPos , const Vec2& endPos)
{

    Sprite* finger = Sprite::create(_rootPathRes + "finger2_0.png");
    finger->setPosition(startPos);
    parent->addChild(finger,99);
    
    
    auto m = ActionHelper::moveToPostionByBezier(startPos,endPos, 1.2f);
    
    finger->runAction(RepeatForever::create(Sequence::create(Show::create(),
                                                             m,
                                                             Hide::create(),
                                                             Place::create(startPos),
                                                             DelayTime::create(1.2f),
                                                             NULL)));
    _mapTipsNode.insert(kTipUITagMove, finger);
    return finger;
}

void TipsUIController::removeMoveTip()
{
    removeTipNode(kTipUITagMove);
}

Node* TipsUIController::showAccelerateTip(cocos2d::Node *parent, const cocos2d::Vec2 &pointLocal, bool isRight)
{
    removeAccelerateTip();
    std::string file = _rootPathRes + "arrows1.png";
    auto animation = ActionHelper::createRotatePromptLeft();
    bool flippX = false;
    if (isRight)
    {
        file = _rootPathRes + "arrows1.png";
        animation = ActionHelper::createRotatePromptRight();
        flippX = true;
    }
    Sprite* spr = Sprite::create(file);
    spr->setPosition(pointLocal);
    parent->addChild(spr,INT_MAX);
    spr->setFlippedX(flippX);
    
    spr->runAction(animation);
    
    _mapTipsNode.insert(kTipUITagAccelarate, spr);
    return spr;
}

void TipsUIController::removeAccelerateTip()
{
    removeTipNode(kTipUITagAccelarate);
}

Node* TipsUIController::showMixTip90Degree(cocos2d::Node *parent, const cocos2d::Vec2 &pointLocal)
{
    if(!FileUtils::getInstance()->isFileExist(_rootPathRes + "arrows2.png"))
    {
        return nullptr;
    }
    
    Sprite* spr = Sprite::create(_rootPathRes + "arrows2.png");
    spr->setPosition(pointLocal);
    parent->addChild(spr,INT_MAX);
    spr->runAction(RepeatForever::create(RotateBy::create(1.2, 360)));
    _mapTipsNode.insert(kTipUITagRotateDegree, spr);
    return spr;
}

Node* TipsUIController::showMixTip45Degree(cocos2d::Node *parent, const cocos2d::Vec2 &pointLocal)
{
    auto aniSprite = Sprite::create(_rootPathRes + "rotate.png");
//    auto animate = ActionHelper::createAnimate(_rootPathRes + "mix_tips1_%d.png", 1, 5);
    parent->addChild(aniSprite,INT_MAX);
    aniSprite->setPosition(pointLocal);
//    aniSprite->runAction(RepeatForever::create(animate));
    _mapTipsNode.insert(kTipUITagRotateDegree, aniSprite);
    return aniSprite;
}

void TipsUIController::removeRotateTip()
{
    removeTipNode(kTipUITagRotateDegree);
}

Node* TipsUIController::showDownTip(Node* parent, const Vec2& pointLocal)
{
    removeAccelerateTip();
    std::string file = _rootPathRes + "down.png";
    Sprite* spr = Sprite::create(file);
    spr->setPosition(pointLocal);
    parent->addChild(spr,INT_MAX);
    
    spr->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.2f, Vec2(0, -60)),
                                                          DelayTime::create(0.4f),
                                                          Place::create(pointLocal),
                                                          NULL)));
    
    _mapTipsNode.insert(kTipUITagAccelarate, spr);
    return spr;
}

Node* TipsUIController::showPressTip(Node* parent,  const Vec2& startPos , const Vec2& endPos)
{
    Sprite* finger = Sprite::create(_rootPathRes + "finger1.png");
    finger->setPosition(startPos);
    parent->addChild(finger,99);
    finger->setRotation(-100);
    
    auto m1 = MoveTo::create(0.4f, endPos);
    auto s1 = ScaleTo::create(0.4f, 0.75f);
    auto s = Spawn::create(m1,s1,NULL);
    finger->runAction(RepeatForever::create(Sequence::create(s,
                                                           DelayTime::create(0.3f),
                                                           Place::create(startPos),
                                                           ScaleTo::create(0, 1),
                                                           NULL)));
    _mapTipsNode.insert(kTipUITagMove, finger);
    return finger;
}


void TipsUIController::removeTipNode(const int tag)
{
    auto node = _mapTipsNode.at(tag);
    if(node)
    {
        node->removeFromParent();
        _mapTipsNode.erase(tag);
    }
}

void TipsUIController::removeAllTips()
{
    for(auto itor: _mapTipsNode)
    {
        auto node = itor.second;
        if(node && node->getParent())
        {
            node->removeFromParent();
        }
    }
    _mapTipsNode.clear();
}
