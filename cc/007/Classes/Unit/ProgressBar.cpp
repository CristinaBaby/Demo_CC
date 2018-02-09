//
//  ProgressBar.cpp
//  Make
//
//  Created by wuguiling on 15/10/21.
//
//

#include "ProgressBar.h"


ProgressBar::ProgressBar()
{
    progressNewState = nullptr;
    m_sBarPath = "";
    m_sBeginPath = "";
    m_sOkPath = "";
    m_s_BadPath = "";
    m_nBegin = 100;
    m_nOk = 0;
    m_nBad = 0;
}

ProgressBar* ProgressBar::create(std::string barPath, std::string beginPath, std::string okPath, std::string badPath,std::string maskPath,Direction dir)
{
    ProgressBar *pRet = new ProgressBar();
    if (pRet && pRet->init(barPath,beginPath,okPath,badPath,maskPath,dir))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool ProgressBar::init(std::string barPath, std::string beginPath, std::string okPath, std::string badPath,std::string maskPath,Direction dir)
{
    if ( !Node::init() )
    {
        return false;
    }
    m_sBarPath = barPath;
    m_sBeginPath = beginPath;
    m_sOkPath = okPath;
    m_s_BadPath = badPath;
    m_eDirection = dir;
    
    m_pBar = Sprite::create(barPath);
    this->addChild(m_pBar);
    
    m_pHead = Sprite::create(beginPath);
    this->addChild(m_pHead);
    if (m_eDirection==Direction::eDirectionH) {
        m_pHead->setPosition(Vec2(-m_pBar->getContentSize().width/2, 0));
    }else{
        m_pHead->setPosition(Vec2(0,-m_pBar->getContentSize().height/2));
    }
    m_pHead->setLocalZOrder(10);
    
    m_pClippingNode = ClippingNode::create(Sprite::create(maskPath));
    this->addChild(m_pClippingNode);
    m_pClippingNode->setAlphaThreshold(0.5);
    
//    if (m_eDirection==Direction::eDirectionH) {
//        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bubble_h.plist");
//        pParticle->setPosition(m_pHead->getPosition());
//        pParticle->setName("particle");
//        m_pClippingNode->addChild(pParticle);
//        
//    }else{
//        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bubble_v.plist");
//        pParticle->setPosition(m_pHead->getPosition());
//        pParticle->setName("particle");
//        m_pClippingNode->addChild(pParticle);
//    }
    return true;
}

void ProgressBar::beginProgress(float duration,bool AUTO)
{
    if (strcmp(m_sBeginPath.c_str(), "")!=0) {
        m_pHead->setTexture(m_sBeginPath);
    }
    Vec2 pos = Vec2(m_pHead->getPositionX()+m_fv, m_pHead->getPositionY());
    float length = m_pBar->getContentSize().width;
    
    if (m_eDirection==Direction::eDirectionH) {
        length = m_pBar->getContentSize().width;
        m_pHead->setPosition(Vec2(-m_pBar->getContentSize().width/2, 0));
    }else{
        length = m_pBar->getContentSize().height;
        m_pHead->setPosition(Vec2(0,-m_pBar->getContentSize().height/2));
    }
    m_fv = 1.0f*length/duration/60.0f;
    m_pHead->runAction(RepeatForever::create(Sequence::create(EaseBackOut::create(ScaleTo::create(1, 1.1)),
                                                              DelayTime::create(0.5),
                                                              ScaleTo::create(0.5, 1),
                                                              NULL)));
    if (AUTO) {
        scheduleUpdate();
    }
}

void ProgressBar::setPercentStage(int begin, int ok, int bad)
{
    
    m_nBegin = begin;
    m_nOk = ok;
    m_sBarPath = bad;
}

void ProgressBar::pauseProgress()
{
    m_pBar->pause();
    m_pHead->pause();
    unscheduleUpdate();
}
void ProgressBar::resumeProgress()
{
    m_pBar->pause();
    m_pHead->pause();
    scheduleUpdate();
}

void ProgressBar::doSchedule()
{
    update(0.1f);
}
void ProgressBar::setPercent(float percent)
{
    if (percent>100){
        if (m_fPercent!=100) {            
            if (progressNewState) {
                progressNewState(eStateOver);
            }
        }
        m_fPercent = 100;
        return;
    }
    m_fPercent = percent;
    Vec2 pos = Vec2(-m_pBar->getContentSize().width/2+m_pBar->getContentSize().width*percent/100, m_pHead->getPositionY());
    m_pHead->setPosition(pos);
    if (percent>=100) {
        m_fPercent = 100;
        if (progressNewState) {
            progressNewState(eStateOver);
        }
    }
}
void ProgressBar::update(float dt)
{
    Vec2 pos = Vec2(m_pHead->getPositionX()+m_fv, m_pHead->getPositionY());
    float length = m_pBar->getContentSize().width;
    float curLength = pos.x+m_pBar->getContentSize().width/2;
    if (m_eDirection==Direction::eDirectionH) {
        pos = Vec2(m_pHead->getPositionX()+m_fv, m_pHead->getPositionY());
        length = m_pBar->getContentSize().width;
        curLength = pos.x;
    }else{
        pos = Vec2(m_pHead->getPositionX(), m_pHead->getPositionY()+m_fv);
        length = m_pBar->getContentSize().height;
        curLength = pos.y+m_pBar->getContentSize().height/2;
    }
    if (curLength+m_pBar->getContentSize().width/2>=length) {
        unscheduleUpdate();
        return;
    }
    if (curLength>=0 && curLength<m_fv) {
        if (strcmp(m_sBeginPath.c_str(), "")!=0) {
            m_pHead->setTexture(m_sBeginPath);
        }
        if (progressNewState) {
            progressNewState(eStateNormal);
        }
    }
    if (curLength+m_pBar->getContentSize().width/2>=length*m_nBegin/100 && curLength+m_pBar->getContentSize().width/2<length*m_nBegin/100+m_fv) {
        if (strcmp(m_sOkPath.c_str(), "")!=0) {
            m_pHead->setTexture(m_sOkPath);
        }
        if (progressNewState) {
            progressNewState(eStateOK);
        }
        //        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/fangshe.plist");
        //        m_pBar->addChild(pParticle);
        //        pParticle->setPosition(m_pHead->getPosition());
    }
    if (curLength+m_pBar->getContentSize().width/2>=length*m_nOk/100 && curLength+m_pBar->getContentSize().width/2<length*m_nOk/100+m_fv) {
        if (strcmp(m_s_BadPath.c_str(), "")!=0) {
            m_pHead->setTexture(m_s_BadPath);
        }
        if (progressNewState) {
            progressNewState(eStateBad);
        }
        //        this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(1, 1)),
        //                                                                        MoveBy::create(0.01, Vec2(-2, -2)),
        //                                                                        MoveBy::create(0.01, Vec2(1, 1)),
        //                                                                        NULL)));
    }
    
    if (curLength+m_pBar->getContentSize().width/2+m_fv>=length) {
        stopProgress();
        if (progressNewState) {
            progressNewState(eStateOver);
        }
//        return;
    }
    m_pHead->setPosition(pos);
    Node* pNode = m_pClippingNode->getChildByName("particle");
    if (pNode) {
        pNode->setPosition(Vec2(0, pos.y));
    }
}