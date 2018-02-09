
#include "GuideLayer.h"
GuideLayer::GuideLayer()
{
    m_pFinger = nullptr;
}

GuideLayer::~GuideLayer()
{
}
bool GuideLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void GuideLayer::showGuideMove(Vec2 begin,Vec2 end,float delayTime)
{
//    removeGuide();
    m_pFinger = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pFinger);
    
    m_pFinger->setPosition(begin);
    m_pFinger->setVisible(true);
    m_pFinger->setAnchorPoint(Vec2(0.1, 0.96));
    
    Vec2 offset = begin- end;
    float max = MAX(fabs(offset.x), fabs(offset.y));
    float dt = 0.5+0.001*max;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = cfg.controlPoint_2 = end*.5 + begin*.5;
    cfg.endPosition = end;
    m_pFinger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
//                                                               MoveTo::create(dt, end),
                                                               BezierTo::create(dt, cfg),
                                                               DelayTime::create(0.5f),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(false);
                                                                                    m_pFinger->setPosition(begin);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
    
}
void GuideLayer::showGuideTap(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pFinger = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pFinger);
    m_pFinger->setPosition(position);
    m_pFinger->setVisible(true);
    m_pFinger->setAnchorPoint(Vec2(0.1, 0.9));
    
    m_pPoint = Sprite::create("content/common/tip/click.png");
    this->addChild(m_pPoint,-1);
    m_pPoint->setPosition(m_pFinger->getPosition());
    
    m_pPoint->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(ScaleTo::create(1, 1.3)),
                                                               ScaleTo::create(1, 1),
                                                               EaseSineOut::create(ScaleTo::create(1, 1.3)),
                                                               ScaleTo::create(1, 1),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    this->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    this->setVisible(true);
                                                                                }),
                                                               NULL)));
}


void GuideLayer::showGuideTapDirection(Vec2 position,bool isLeft,float delayTime){
    showGuideTap(position,delayTime);
    if (!isLeft) {
        m_pFinger->setFlippedX(-1);
    }
}
void GuideLayer::showGuideTapVertical(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pFinger = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pFinger);
    m_pFinger->setPosition(position);
    m_pFinger->setVisible(true);
    m_pFinger->setAnchorPoint(Vec2(0.1, 0.96));
    m_pFinger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(0, 50))),
                                                               MoveBy::create(1, Vec2(0, -50)),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(0, 50))),
                                                               MoveBy::create(1, Vec2(0, -50)),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
}
void GuideLayer::showGuideTapHorizonal(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pFinger = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pFinger);
    m_pFinger->setPosition(position);
    m_pFinger->setVisible(true);
    m_pFinger->setAnchorPoint(Vec2(0.1, 0.96));
    m_pFinger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                               MoveBy::create(1, Vec2(-50, 0)),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                               MoveBy::create(1, Vec2(-50, 0)),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
    
}
void GuideLayer::showGuideShake(Vec2 position,float delayTime)
{
    if (m_pFinger) {
        m_pFinger->removeFromParent();
    }
    Node* pNode = this->getChildByName("kick");
    if (pNode) {
        pNode->removeFromParent();
    }
    m_pFinger = Sprite::create("content/common/tip/shake.png");
    this->addChild(m_pFinger);
    m_pFinger->setPosition(position);
    m_pFinger->setVisible(true);
    
    m_pFinger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               Spawn::create(EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                                             RotateTo::create(1, 10),
                                                                             NULL),
                                                               Spawn::create(MoveBy::create(1, Vec2(-100, 0)),
                                                                             RotateTo::create(1, -10),
                                                                             NULL),
                                                               Spawn::create(MoveBy::create(1, Vec2(50, 0)),
                                                                             RotateTo::create(1, 0),
                                                                             NULL),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
    
}
void GuideLayer::showGuideRotateRight(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pArrow = Sprite::create("content/common/tip/jiantou.png");
    this->addChild(m_pArrow);
    m_pArrow->setPosition(position);
    m_pArrow->setVisible(true);
    m_pArrow->setFlippedX(true);
    
    m_pArrow->setRotation(30);
    m_pArrow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               
                                                               RotateTo::create(1, -30),
                                                               DelayTime::create(2),
                                                               Hide::create(),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pArrow->setRotation(30);
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
}

void GuideLayer::showGuideRotateLeft(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pArrow = Sprite::create("content/common/tip/jiantou.png");
    this->addChild(m_pArrow);
    m_pArrow->setPosition(position);
    m_pArrow->setVisible(true);
    
    m_pArrow->setRotation(30);
    m_pArrow->runAction(Sequence::create(DelayTime::create(delayTime),
                                         
                                         RotateTo::create(1, -30),
//                                         DelayTime::create(2),
//                                         Hide::create(),
                                         NULL));
//    m_pArrow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
//                                                               
//                                                               RotateTo::create(1, -30),
//                                                               DelayTime::create(2),
//                                                               Hide::create(),
//                                                               DelayTime::create(3),
//                                                               CallFunc::create([=]()
//                                                                                {
//                                                                                    m_pArrow->setRotation(30);
//                                                                                    m_pArrow->setVisible(true);
//                                                                                }),
//                                                               NULL)));
}

void GuideLayer::showGuideMix(Vec2 position,float delayTime)
{
//    removeGuide();
    m_pArrow = Sprite::create("content/common/tip/xuanzhuan.png");
    this->addChild(m_pArrow);
    m_pArrow->setPosition(position);
    m_pArrow->setVisible(true);
    
    m_pArrow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               FadeIn::create(0.5),
                                                               RotateBy::create(3, 360),
                                                               DelayTime::create(2),
                                                               FadeOut::create(0.1),
                                                               DelayTime::create(5),
                                                               NULL)));
}
void GuideLayer::showGuideScrollHorizonal(Vec2 position,float delayTime)
{
    
//    removeGuide();
    m_pArrow = Sprite::create("content/common/tip/arrow.png");
    this->addChild(m_pArrow);
    
    
    m_pArrow->setVisible(true);
    m_pArrow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                            Spawn::create(EaseSineOut::create(MoveBy::create(1, Vec2(-50, 0))),
                                                                          ScaleTo::create(1, 1.1), NULL),
                                                            Spawn::create(MoveBy::create(1, Vec2(50, 0)),
                                                                          ScaleTo::create(1, 1), NULL),
                                                            Spawn::create(MoveBy::create(1, Vec2(-50, 0)),
                                                                          ScaleTo::create(1, 1.1), NULL),
                                                            Spawn::create(MoveBy::create(1, Vec2(50, 0)),
                                                                          ScaleTo::create(1, 1), NULL),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 m_pArrow->setVisible(false);
                                                                             }),
                                                            DelayTime::create(3),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 m_pArrow->setVisible(true);
                                                                             }),
                                                            NULL)));
    
}

void GuideLayer::showGuideArrow(Vec2 position,int direction,float delayTime)
{
//    removeGuide();
    
    if (abs(direction)==1) {
        m_pArrow->setAnchorPoint(Vec2(0.5, 1));
        if (direction<0) {
            m_pArrow = Sprite::create("content/common/tip/down.png");
            m_pArrow->setRotation(90);
        }else{
            m_pArrow = Sprite::create("content/common/tip/down.png");
            m_pArrow->setRotation(-90);
        }
    }else if (abs(direction)==2) {
        m_pArrow = Sprite::create("content/common/tip/down.png");
        m_pArrow->setAnchorPoint(Vec2(0.5, 1));
        if (direction>0) {
            m_pArrow->setRotation(180);
        }else{
        }
    }else{
        if (direction<0) {
            m_pArrow = Sprite::create("content/common/tip/arrows.png");
        }else{
            m_pArrow = Sprite::create("content/common/tip/arrows.png");
            m_pArrow->setRotation(-90);
        }
    }
    this->addChild(m_pArrow);
    m_pArrow->setPosition(position);
    m_pArrow->setVisible(true);
    m_pArrow->setScale(0);
    m_pArrow->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               ScaleTo::create(0.5,1),
                                                               DelayTime::create(3.5),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pArrow->setScale(0);
                                                                                }),
                                                               DelayTime::create(3),
                                                               NULL)));
    
}

void GuideLayer::showGuide(std::string path, cocos2d::Vec2 position,float delayTime)
{
//    removeGuide();
    
    m_pFinger = Sprite::create(path);
    this->addChild(m_pFinger);
    m_pFinger->setPosition(position);
    m_pFinger->setVisible(true);
    
    m_pFinger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pFinger->setVisible(true);
                                                                                }),
                                                               NULL)));
}
void GuideLayer::removeGuide()
{
    this->removeAllChildren();
    m_pFinger = nullptr;
    m_pArrow = nullptr;
    m_pPoint = nullptr;
}