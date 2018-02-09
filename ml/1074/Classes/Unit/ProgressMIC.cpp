
#include "ProgressMIC.h"
#include "ActionHelper.h"

ProgressMIC::ProgressMIC()
{
    m_nPercent = 0;
    m_bFinished = false;
    onFinishCallback = nullptr;
}

bool ProgressMIC::init()
{
    if (!Node::init()) {
        return false;
    }
    m_pBarBg = Sprite::create("content/common/microphone0.png");
    this->addChild(m_pBarBg);
    
    lenght = m_pBarBg->getContentSize().height-35;
    m_nPercent = 0;
    
    m_pClipping = ClippingNode::create(Sprite::create("content/common/microphone_mask.png"));
    this->addChild(m_pClipping);
    m_pClipping->setPosition(Vec2(0, -lenght));
    
    m_pProgress = Sprite::create("content/common/microphone1.png");
    m_pClipping->addChild(m_pProgress);
    m_pProgress->setPosition(Vec2(0, lenght));
    
    return true;
}

void ProgressMIC::addPercent(int dt)
{
    updatePercent(m_nPercent+dt);
}

void ProgressMIC::updatePercent(int pecent)
{
    m_nPercent = pecent;
    if (m_nPercent>100) {
        m_nPercent = 100;
        if (onFinishCallback && !m_bFinished) {
            onFinishCallback();
            m_bFinished = true;
        }
        m_pClipping->setPosition(Vec2::ZERO);
        m_pProgress->setPosition(Vec2::ZERO);
        return;
    }
    m_pClipping->setPosition(Vec2(0, lenght*(pecent-100)/100));
    m_pProgress->setPosition(Vec2(0, lenght*(100-pecent)/100));
}