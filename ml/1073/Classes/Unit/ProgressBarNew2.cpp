//
//  ProgressBarNew2.cpp
//  Make
//
//  Created by wuguiling on 15/10/21.
//
//

#include "ProgressBarNew2.h"


ProgressBarNew2::ProgressBarNew2()
{
    progressNewState = nullptr;
}

ProgressBarNew2* ProgressBarNew2::create(std::string barPath,std::string bgPath,std::string maskPath,Direction dir)
{
    ProgressBarNew2 *pRet = new ProgressBarNew2();
    if (pRet && pRet->init(barPath,bgPath,maskPath,dir))
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

bool ProgressBarNew2::init(std::string barPath,std::string bgPath,std::string maskPath,Direction dir)
{
    if ( !Node::init() )
    {
        return false;
    }
    
    m_eDirection = dir;
    
    m_pClipping = ClippingNode::create(Sprite::create(maskPath));
    m_pClipping->setOpacityModifyRGB(0.5);
    this->addChild(m_pClipping,2);
    m_pClipping->setAlphaThreshold(0.5);
    
    m_pBar = Sprite::create(barPath);
    m_pClipping->addChild(m_pBar);
    
    m_pBarBg = Sprite::create(bgPath);
    this->addChild(m_pBarBg);
    
    if (m_eDirection==Direction::eDirectionH) {
        m_pBar->setPosition(Vec2(m_pBar->getContentSize().width, m_pBar->getContentSize().height/2));
    }else{
        m_pBar->setPosition(Vec2(m_pBar->getContentSize().width/2,m_pBar->getContentSize().height));
    }
    m_pBar->setLocalZOrder(10);
    return true;
}

void ProgressBarNew2::beginProgress(float duration)
{
    Vec2 pos = Vec2(m_pBar->getPositionX()-m_fv, m_pBar->getPositionY());
    float length = m_pBar->getContentSize().width;
    if (m_eDirection==Direction::eDirectionH) {
        length = m_pBar->getContentSize().width;
        m_pClipping->setPosition(Vec2(-length, 0));
        m_pBar->setPosition(Vec2(m_pBar->getContentSize().width, 0));
    }else{
        length = m_pBar->getContentSize().height;
        m_pClipping->setPosition(Vec2(0, -length));
        m_pBar->setPosition(Vec2(0,m_pBar->getContentSize().height));
    }
    m_fv = length/duration*0.03;
    
    scheduleUpdate();
}

void ProgressBarNew2::setFlag(float ok,float bad,int segment0,int segment1,int segment2)
{
    m_fLineOK = ok;
    m_fLineBad = bad;
    m_nSegment_0 = segment0;
    m_nSegment_1 = segment1;
    m_nSegment_2 = segment2;
}

void ProgressBarNew2::pauseProgress()
{
    m_pBar->pause();
    unscheduleUpdate();
    this->setVisible(false);
}
void ProgressBarNew2::resumeProgress()
{
    m_pBar->pause();
    scheduleUpdate();
    this->setVisible(true);
}

void ProgressBarNew2::doSchedule()
{
    update(0.1f);
}
void ProgressBarNew2::update(float dt)
{
    Vec2 pos = Vec2(m_pBar->getPositionX()-m_fv, m_pBar->getPositionY());
    Vec2 posMask = Vec2(m_pClipping->getPositionX()+m_fv, m_pClipping->getPositionY());
    float length = m_pBar->getContentSize().width;
    float curLength = pos.x;
    if (m_eDirection==Direction::eDirectionH) {
        posMask = Vec2(m_pClipping->getPositionX()+m_fv, m_pClipping->getPositionY());
        pos = Vec2(m_pBar->getPositionX()-m_fv, m_pBar->getPositionY());
        length = m_pBar->getContentSize().width;
        curLength = m_pBar->getContentSize().width-pos.x;
    }else{
        posMask = Vec2(m_pClipping->getPositionX(), m_pClipping->getPositionY()+m_fv);
        pos = Vec2(m_pBar->getPositionX(), m_pBar->getPositionY()+m_fv);
        length = m_pBar->getContentSize().height;
        curLength = m_pBar->getContentSize().height-pos.y;
    }
    if (curLength>=length) {
        length = length;
        unscheduleUpdate();
    }
    if (curLength>=0 && curLength<=m_fLineOK) {
        float delt = m_fLineOK/m_nSegment_0;
        for (int i = 1; i<=m_nSegment_0; i++) {
            if (delt*i-m_fv<=curLength && curLength<=delt*i) {
                if (progressNewState) {
                    progressNewState(this, i);
                }
//                if(1==i){
//                    m_pBar->setColor(m_c0);
//                }else if (i==m_nSegment_0) {
//                    m_pBar->setColor(m_c1);
//                }
            }
        }
    }else if (curLength>m_fLineOK && curLength<=m_fLineBad) {
        float delt = (m_fLineBad-m_fLineOK)/m_nSegment_1;
        for (int i = 1; i<=m_nSegment_1; i++) {
            if (m_fLineOK+delt*i-m_fv<=curLength && curLength<=m_fLineOK+delt*i) {
                if (progressNewState) {
                    progressNewState(this, m_nSegment_0+i);
                }
//                if(1==i){
//                    m_pBar->setColor(m_c1);
//                }else if (i==m_nSegment_1) {
//                    m_pBar->setColor(m_c2);
//                }
            }
        }
    }else if (curLength>=m_fLineBad && curLength<=m_pBar->getContentSize().width+m_fv) {
        float delt = (length-m_fLineBad)/m_nSegment_2;
        for (int i = 1; i<=m_nSegment_2; i++) {
            if (m_fLineBad+delt*i-m_fv<=curLength && curLength<=m_fLineBad+delt*i) {
                if (progressNewState) {
                    progressNewState(this, m_nSegment_0+m_nSegment_1+i);
                }
//                if(1==i){
//                    m_pBar->setColor(m_c2);
//                }
            }
        }
    }
    if (curLength>=length) {
        stopProgress();
        this->removeFromParent();
        return;
    }
    m_pBar->setPosition(pos);
    
    m_pClipping->setPosition(posMask);
}