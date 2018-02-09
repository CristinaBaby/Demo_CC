
#include "HomeLogo.h"
#include "CMVisibleRect.h"
#include "Global.h"

HomeLogo::HomeLogo()
{
    
}

HomeLogo::~HomeLogo()
{
    
}


bool HomeLogo::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Sprite* pFrame = Sprite::create("content/start/logo/frame.png");
    this->addChild(pFrame);
    pFrame->setPosition(Vec2(0, 40));
    
    
    Sprite* pTomato = Sprite::create("content/start/logo/tomato.png");
    this->addChild(pTomato);
    pTomato->setPosition(Vec2(0, 80));
    
    Sprite* pTool = Sprite::create("content/start/logo/tool.png");
    this->addChild(pTool);
    pTool->setPosition(Vec2(-80, 100));
    
    Sprite* pLeaveLeft = Sprite::create("content/start/logo/leave3.png");
    this->addChild(pLeaveLeft);
    pLeaveLeft->setPosition(Vec2(-130, 70));
    
    Sprite* pLeaveRight = Sprite::create("content/start/logo/leave1.png");
    this->addChild(pLeaveRight);
    pLeaveRight->setPosition(Vec2(65, 73));
    
    Sprite* pLeaveRight2 = Sprite::create("content/start/logo/leave2.png");
    this->addChild(pLeaveRight2);
    pLeaveRight2->setPosition(Vec2(110, 70));
    
    Sprite* pText = Sprite::create("content/start/logo/text.png");
    this->addChild(pText);
    
    m_pStar = Sprite::create("content/start/logo/star.png");
    pText->addChild(m_pStar);
    m_pStar->setPosition(Vec2(117, 170));
    
    m_pFrame = pFrame;
    m_pTomato = pTomato;
    m_pTool = pTool;
    m_pLeaveLeft = pLeaveLeft;
    m_pLeaveRight = pLeaveRight;
    m_pLeaveRight2 = pLeaveRight2;
    m_pText = pText;
    
    return true;
}

void HomeLogo::onEnter()
{
    Layer::onEnter();
    
}

void HomeLogo::showAnimation()
{
    m_pStar->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.3),
                                                              DelayTime::create(1),
                                                              FadeOut::create(0.1),
                                                              DelayTime::create(3), NULL)));
//    m_pText->runAction(RepeatForever::create(Sequence::create(EaseBounceOut::create(ScaleTo::create(0.5, 1.1)),
//                                                               ScaleTo::create(3, 1),
//                                                               DelayTime::create(5),
//                                                               NULL)));
    m_pTomato->runAction(RepeatForever::create(Sequence::create(Spawn::create(JumpBy::create(1, Vec2::ZERO, 70, 1),
                                                                              RotateBy::create(1, -50),
                                                                              NULL),
                                                              DelayTime::create(4),
                                                              NULL)));
    m_pTool->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(-20, 20)),
                                                              MoveBy::create(0.1, Vec2(5, -5)),
                                                              MoveBy::create(0.1, Vec2(-5, 5)),
                                                              MoveBy::create(0.5, Vec2(20, -20)),
                                                              DelayTime::create(4),
                                                              NULL)));
    m_pLeaveLeft->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f, 5),
                                                                   RotateBy::create(0.2f, -10),
                                                                   RotateBy::create(0.1f, 5),
                                                                   DelayTime::create(1.8),
                                                                   NULL)));
    m_pLeaveRight->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f, 5),
                                                                   RotateBy::create(0.2f, -10),
                                                                   RotateBy::create(0.1f, 5),
                                                                   DelayTime::create(2.2),
                                                                   NULL)));
    m_pLeaveRight2->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f, 5),
                                                                   RotateBy::create(0.2f, -10),
                                                                   RotateBy::create(0.1f, 5),
                                                                   DelayTime::create(2),
                                                                   NULL)));
}