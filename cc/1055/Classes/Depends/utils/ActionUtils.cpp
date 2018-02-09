//
//  ActionUtils.cpp
//  MLABS1040_ItaliaFood
//
//  Created by wusonglin1 on 15-1-5.
//
//

#include "ActionUtils.h"
#include "CCImageColorSpace.h"


void ActionUtils::roll(Node* node,Vec2 vec2,float pTime,int circle,CallFunc* func)
{
    MoveBy* move = MoveBy::create(pTime, vec2);
    RotateBy* rotate = RotateBy::create(pTime, 360 * circle);
    
    Spawn* spawn = Spawn::create(move, rotate,NULL);
    node->runAction(Sequence::create(spawn,func, NULL));
}

void ActionUtils::dropOut(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    MoveBy* move = MoveBy::create(pTime, vec2);
    
    EaseElasticOut* eeo = EaseElasticOut::create(move,0.5);
    node->runAction(Sequence::create(eeo,func, NULL));
    
}
void ActionUtils::moveBy(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    
    MoveBy* move = MoveBy::create(pTime, vec2);
    
   
    node->runAction(Sequence::create(move,func,NULL));
    
    
}
void ActionUtils::moveTo(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    
    MoveTo* move = MoveTo::create(pTime, vec2);
    node->runAction(Sequence::create(move,func, NULL));
    
    
}
void ActionUtils::scaleTo(Node* node,float f,float pTime,CallFunc* func)
{
    ScaleTo* scale = ScaleTo::create(pTime, f);
    
    //动作由慢到快
    EaseSineIn* ease = EaseSineIn::create(scale);
    node->runAction(Sequence::create(ease,func, NULL));
    
}
void ActionUtils::moveByEaseBack(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    MoveBy* move = MoveBy::create(pTime, vec2);
    
    EaseBounceOut* ease = EaseBounceOut::create(move);
    node->runAction(Sequence::create(ease,func, NULL));


}
//终点弹性
void ActionUtils::moveByElasticOut(Node* node,Vec2 vec2,float pTime,CallFunc* func)

{
    MoveBy* move = MoveBy::create(pTime, vec2);
    
    EaseElasticOut* ease = EaseElasticOut::create(move,1.0);
    node->runAction(Sequence::create(ease,func, NULL));
    
    
}
//起点弹性
void ActionUtils::moveByElasticIn(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    MoveBy* move = MoveBy::create(pTime, vec2);
    
    EaseElasticIn* ease = EaseElasticIn::create(move,1.0);
    node->runAction(Sequence::create(ease,func, NULL));
    
    
}
//终点弹性
void ActionUtils::moveToElasticOut(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    MoveTo* move = MoveTo::create(pTime, vec2);
    
    EaseElasticOut* ease = EaseElasticOut::create(move,1.0);
    node->runAction(Sequence::create(ease,func, NULL));
    
    
}
//起点弹性
void ActionUtils::moveToElasticIn(Node* node,Vec2 vec2,float pTime,CallFunc* func)
{
    MoveTo* move = MoveTo::create(pTime, vec2);
    
    EaseElasticIn* ease = EaseElasticIn::create(move,1.0);
    node->runAction(Sequence::create(ease,func, NULL));
    
    
}
void ActionUtils::delayTimeAction(Node* node,float pTime,CallFunc* func)
{
    node->runAction(Sequence::create(DelayTime::create(pTime),func, NULL));
}
//show or hide
void ActionUtils::flashAction(Node *pNode,int lRepeatCount,CallFunc* func)
{
    auto flash = Sequence::create(Hide::create(),
                                  DelayTime::create(0.5),
                                  Show::create(),
                                  DelayTime::create(0.5),
                                  NULL);
    if (lRepeatCount == -1)
    {
        pNode -> runAction(RepeatForever::create(flash));
    }
    else
    {
        pNode -> runAction(Sequence::create
                           (
                                         Repeat::create(flash, lRepeatCount),
                                         Hide::create()
                                         , func,NULL));
    }


}
void ActionUtils::zoomAction(Node *pNode, int lRepeatCount,CallFunc* func)
{
    auto zoom = Sequence::create(ScaleTo::create(0.5, 0.8),
                                 ScaleTo::create(0.5, 1),
                                 NULL);
    if (lRepeatCount == -1)
    {
        pNode -> runAction(RepeatForever::create(zoom));
    }
    else
    {
        pNode -> runAction(Sequence::create(
                                         Repeat::create(zoom, lRepeatCount),
                                         Hide::create()
                                         , NULL));
    }


}
void ActionUtils::shakeNode(Node *pNode, float pScale, int lRepeatCount,CallFunc* func)
{
    {
        MoveBy *lMove1 = MoveBy::create(0.03 * pScale, Vec2(-10 * pScale, 0));
        MoveBy *lMove2 = MoveBy::create(0.06 * pScale, Vec2(20 * pScale, 0));
        MoveBy *lMove3 = MoveBy::create(0.03 * pScale, Vec2(-10 * pScale, 0));
        Sequence *lSequence = Sequence::create(lMove1, lMove2, lMove3, NULL);
        Repeat *lRepeat = Repeat::create(lSequence, lRepeatCount);
        
        pNode->runAction(Sequence::create(lRepeat,func, NULL));
    }
}
void ActionUtils::shakeCup(Node *pNode, float pScale, int lRepeatCount,CallFunc* func)
{
    RotateTo* rote = RotateTo::create(0.1, 10);
    RotateTo* rote1 = RotateTo::create(0.1, -10);
    Sequence *lSequence = Sequence::create(rote, rote1, NULL);
    
    Repeat *lRepeat = Repeat::create(lSequence, lRepeatCount);
    
    RotateTo* rote3 = RotateTo::create(0.1, 0);
    
    pNode->runAction(Sequence::create(lRepeat,rote3,func, NULL));
}



void ActionUtils::fadeOut(Node* node,float pTime,CallFunc* func)
{
    FadeOut* fade = FadeOut::create(pTime);
    
    
    node->runAction(Sequence::create(fade,func, NULL));

}
void ActionUtils::fadeIn(Node* node,float pTime ,CallFunc* func )
{
    FadeIn* fade = FadeIn::create(pTime);
    node->runAction(Sequence::create(fade,func, NULL));
}
Texture2D* ActionUtils::getShapeScribbleTexture(__String* filename,int h, int s, int v)
{
//    char* name = filename->getCString();
    auto orImg = new Image();
    orImg -> initWithImageFile(filename->getCString());
    
    auto img = CCImageColorSpace::imageWithHSB(orImg, h, s, v);
    Texture2D * tex = new Texture2D();
    tex->initWithImage(img);
    
    return tex;
}
void ActionUtils::jumpAction(Node *pNode, int lRepeatCount,CallFunc* func,float delayTime)
{
    auto jump = JumpBy::create(1.0, Vec2(0,0), 30, lRepeatCount);
    
   
    pNode -> runAction(Sequence::create(DelayTime::create(delayTime),Repeat::create(Sequence::create(jump,DelayTime::create(4.0),NULL),40), NULL));
}
void ActionUtils::jumpBall(Node *pNode, int lRepeatCount,CallFunc* func,float delayTime)
{
    
    auto jump = JumpBy::create(1.0, Vec2(0,0), 60, lRepeatCount);
    
    
    pNode -> runAction(Sequence::create(jump, NULL));
    

}


