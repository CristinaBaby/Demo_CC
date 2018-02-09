//
// ViewPager.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#include "ViewPager.h"

ViewPager::ViewPager()
:m_nCount(1)
,m_vDots(std::vector<Sprite *>())
,m_pActiveDot(nullptr)
{
    
}

void ViewPager::onEnter()
{
    Node::onEnter();
    
}

ViewPager* ViewPager::create(const std::string &aNormal, const std::string &aActive)
{
    auto pRet = new ViewPager();
    if(pRet && pRet->init(aNormal, aActive))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

bool ViewPager::init(const std::string &aNormal, const std::string &aActive)
{
    if ( !Node::init() )
    {
        return false;
    }
    
    if (!aActive.empty()) {
        m_pActiveDot = Sprite::create(aActive);
        m_pActiveDot->setTag(0);
        addChild(m_pActiveDot);
    }
    
    if (!aNormal.empty()) {
        m_sNormalFilePath = aNormal;
        setDotCount(1);
    }
    
    return true;
}

void ViewPager::setDotCount(size_t aCount)
{
    if (aCount < 1) {
        return;
    }
    
    float  singleItemWidth = Sprite::create(m_sNormalFilePath)->getContentSize().width + 10;
    
    float width = aCount * singleItemWidth;
    
    float weightWidth = -(width * 0.5);
    
    for (auto dot : m_vDots) {
        dot->removeFromParent();
    }
    m_vDots.clear();
    
    float x = weightWidth;
    float y = 0.0;
    
    while (m_vDots.size() != aCount) {
        
        x += singleItemWidth * 0.5 ;
        Sprite *pNormalDot = Sprite::create(m_sNormalFilePath);
        pNormalDot->setPosition(x, y);
        addChild(pNormalDot);
        m_vDots.push_back(pNormalDot);
        x += singleItemWidth * 0.5;
        
    }
    
    setCurrentDot(0);
}

void ViewPager::setCurrentDot(size_t aIndex)
{
    
//    if (m_pActiveDot->getTag() == aIndex) {
//        return;
//    }
    
    m_vDots.at(m_pActiveDot->getTag())->setVisible(true);
    Sprite *pCurrentDot = m_vDots.at(aIndex);
    pCurrentDot->setVisible(false);
    m_pActiveDot->setPosition(pCurrentDot->getPosition());
    m_pActiveDot->setTag((int)aIndex);
}

void ViewPager::onExit()
{

    Node::onExit();
}
ViewPager::~ViewPager()
{
    
}