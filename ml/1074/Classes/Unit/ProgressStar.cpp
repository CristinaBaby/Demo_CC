
#include "ProgressStar.h"
#include "ActionHelper.h"

ProgressStar::ProgressStar()
{
    m_nStarCount = 0;
}

bool ProgressStar::init()
{
    if (!Node::init()) {
        return false;
    }
    m_pBarBg = Sprite::create("content/common/progress/progress_bar.png");
    this->addChild(m_pBarBg);
    return true;
}

void ProgressStar::addStar()
{
    if (m_nStarCount>=4) {
        return;
    }
    m_pBarBg->runAction(ActionHelper::getJellyAction());
    Sprite* pStar = Sprite::create("content/common/progress/progress_star"+std::to_string(m_nStarCount+1)+".png");
    this->addChild(pStar);
    pStar->setPosition(Vec2(0, -100));
    pStar->setScale(1.5);
    pStar->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(-95+64*m_nStarCount, 6)),
                                                    ScaleTo::create(0.5, 1), NULL),
                                      ActionHelper::getJellyAction(), NULL));
    m_nStarCount++;
}