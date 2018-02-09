
#include "GuideLayer.h"
GuideLayer::GuideLayer()
{
    m_pGuide = nullptr;
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
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    
    m_pGuide->setPosition(begin);
    m_pGuide->setVisible(true);
    m_pGuide->setAnchorPoint(Vec2(1, 1));
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               MoveTo::create(1, end),
                                                               DelayTime::create(0.5f),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(false);
                                                                                    m_pGuide->setPosition(begin);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));

}
void GuideLayer::showGuideTap(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    m_pGuide->setAnchorPoint(Vec2(0.5, 1));
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(ScaleTo::create(1, 1.1)),
                                                               ScaleTo::create(1, 1),
                                                               EaseSineOut::create(ScaleTo::create(1, 1.1)),
                                                               ScaleTo::create(1, 1),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
}


void GuideLayer::showGuideTapDirection(Vec2 position,bool isLeft,float delayTime){
    showGuideTap(position,delayTime);
    if (!isLeft) {
        m_pGuide->setFlippedX(-1);
    }
}
void GuideLayer::showGuideTapVertical(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    m_pGuide->setAnchorPoint(Vec2(0.5, 1));
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(0, 50))),
                                                               MoveBy::create(1, Vec2(0, -50)),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(0, 50))),
                                                               MoveBy::create(1, Vec2(0, -50)),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
}
void GuideLayer::showGuideTapHorizonal(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    m_pGuide->setAnchorPoint(Vec2(0.5, 1));
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                               MoveBy::create(1, Vec2(-50, 0)),
                                                               EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                               MoveBy::create(1, Vec2(-50, 0)),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
    
}
void GuideLayer::showGuideShake(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/shake.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
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
                                                                                    m_pGuide->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
    
}
void GuideLayer::showGuideRotateRight(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               
                                                               RotateTo::create(1, 30),
                                                               DelayTime::create(1),
                                                               RotateTo::create(1, 0),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
}
    
void GuideLayer::showGuideRotateLeft(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/finger.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               
                                                               RotateTo::create(1, -30),
                                                               DelayTime::create(1),
                                                               RotateTo::create(1, 0),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
}

void GuideLayer::showGuideMix(Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create("content/common/tip/arrow2.png");
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               FadeIn::create(0.5),
                                                               ScaleTo::create(0.5, 0.9),
                                                               DelayTime::create(0.5),
                                                               ScaleTo::create(0.5, 1),
                                                               DelayTime::create(0.5),
                                                               ScaleTo::create(0.5, 0.9),
                                                               DelayTime::create(0.5),
                                                               ScaleTo::create(0.5, 1),
                                                               DelayTime::create(2),
                                                               FadeOut::create(0.1),
                                                               DelayTime::create(5),
                                                               NULL)));
}
void GuideLayer::showGuideScrollHorizonal(Vec2 position,float delayTime)
{
    Sprite* pLeft = (Sprite*)this->getChildByName("leftArraw");
    if (pLeft) {
        pLeft->removeFromParent();
    }
    Sprite* pRight = (Sprite*)this->getChildByName("rightArraw");
    if (pRight) {
        pRight->removeFromParent();
    }
    pLeft = Sprite::create("content/common/tip/arrow.png");
    this->addChild(pLeft);
    pLeft->setPosition(Vec2(position.x-30, position.y));
    pLeft->setName("leftArraw");
    pLeft->setAnchorPoint(Vec2(1, 0.5));
    
    pRight = Sprite::create("content/common/tip/arrow.png");
    this->addChild(pRight);
    pRight->setPosition(Vec2(position.x+30, position.y));
    pRight->setName("rightArraw");
    pRight->setAnchorPoint(Vec2(1, 0.5));
    pRight->setScaleX(-1);
    
    pLeft->setVisible(true);
    pLeft->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
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
                                                                                 pLeft->setVisible(false);
                                                                             }),
                                                            DelayTime::create(3),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 pLeft->setVisible(true);
                                                                             }),
                                                            NULL)));
    pRight->setVisible(true);
    pRight->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                             Spawn::create(EaseSineOut::create(MoveBy::create(1, Vec2(50, 0))),
                                                                           ScaleTo::create(1, -1.1,1.1), NULL),
                                                             Spawn::create(MoveBy::create(1, Vec2(-50, 0)),
                                                                           ScaleTo::create(1, -1,1), NULL),
                                                             Spawn::create(MoveBy::create(1, Vec2(50, 0)),
                                                                           ScaleTo::create(1, -1.1,1.1), NULL),
                                                             Spawn::create(MoveBy::create(1, Vec2(-50, 0)),
                                                                           ScaleTo::create(1, -1,1), NULL),
                                                             CallFunc::create([=]()
                                                                              {
                                                                                  pRight->setVisible(false);
                                                                              }),
                                                             DelayTime::create(3),
                                                             CallFunc::create([=]()
                                                                              {
                                                                                  pRight->setVisible(true);
                                                                              }),
                                                             NULL)));
    
}

void GuideLayer::showGuideArrow(Vec2 position,int direction,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    if (abs(direction)==1) {
        m_pGuide = Sprite::create("content/common/tip/arrows_0.png");
        if (direction<0) {
            m_pGuide->setFlippedX(-1);
            m_pGuide->setAnchorPoint(Vec2(1, 0.5));
        }else{
            m_pGuide->setAnchorPoint(Vec2(0, 0.5));
        }
    }else if (abs(direction)==2) {
        m_pGuide = Sprite::create("content/common/tip/btn_1.png");
        if (direction>0) {
            m_pGuide->setFlippedY(-1);
            m_pGuide->setAnchorPoint(Vec2(0.5, 0));
        }else{
            m_pGuide->setAnchorPoint(Vec2(0.5, 1));
        }
    }else{
        m_pGuide = Sprite::create("content/common/tip/btn_2.png");
        m_pGuide->setAnchorPoint(Vec2(1, 0.5));
    }
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    m_pGuide->setScale(0);
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               ScaleTo::create(0.5,1),
                                                               DelayTime::create(3.5),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setScale(0);
                                                                                }),
                                                               DelayTime::create(3),
                                                               NULL)));
    
}

void GuideLayer::showGuide(std::string path, cocos2d::Vec2 position,float delayTime)
{
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = Sprite::create(path);
    this->addChild(m_pGuide);
    m_pGuide->setPosition(position);
    m_pGuide->setVisible(true);
    
    m_pGuide->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delayTime),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    m_pGuide->setVisible(true);
                                                                                }),
                                                               NULL)));
}
void GuideLayer::removeGuide()
{
    Sprite* pLeft = (Sprite*)this->getChildByName("leftArraw");
    if (pLeft) {
        pLeft->removeFromParent();
    }
    Sprite* pRight = (Sprite*)this->getChildByName("rightArraw");
    if (pRight) {
        pRight->removeFromParent();
    }
    if (m_pGuide) {
        m_pGuide->removeFromParent();
    }
    m_pGuide = nullptr;
}