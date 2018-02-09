
#include "CircleMove.h"

CircleMove* CircleMove::create(float duration, const cocos2d::Vec2& center, float endRaduis,float angle)
{
    CircleMove *pRet = new CircleMove();
    pRet->initWithDuration(duration, center, endRaduis,angle);
    pRet->autorelease();
    
    return pRet;
}

bool CircleMove::initWithDuration(float duration, const cocos2d::Vec2& center, float endRaduis,float angle)
{
    if (ActionInterval::initWithDuration(duration))
    {
        this->m_duration = duration;
        this->m_center = center;
        m_endRadius = endRaduis;
        
        /************************************************************************/
        /* 计算每次update调用时需要转动的弧度                                   */
        /************************************************************************/
        this->m_frameCnts = 0;
        
        this->m_anglePreFrame = angle / m_duration* Director::getInstance()->getAnimationInterval();
        m_debugColor = ccc4f(10*(arc4random()%25),10*(arc4random()%25),10*(arc4random()%25),150);
        return true;
    }
    return false;
}
CircleMove* CircleMove::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) CircleMove();
    a->initWithDuration(m_duration, m_center, m_endRadius, m_angle);
    a->autorelease();
    return a;
}

void CircleMove::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_initPos = target->getPosition();
    
    Vec2 offset = Vec2(m_initPos, m_center);
    m_beginRadius = sqrtf(powf(offset.x,2)+powf(offset.y,2));
    
    this->m_frameCnts = 0;
//    m_angleOrig =
    
    if (offset.x == 0.0)
        m_angleOrig = 90;
    else
        m_angleOrig = atan(fabs(offset.x / offset.y))*180/M_PI;
    
//    if ((xx < 0.0) && (yy >= 0.0))
//        angle_temp = PI - angle_temp;
//    else if ((xx < 0.0) && (yy < 0.0))
//        angle_temp = PI + angle_temp;
//    else if ((xx >= 0.0) && (yy < 0.0))
//        angle_temp = PI * 2.0 - angle_temp;
    
    m_deltRadius = (m_endRadius-m_beginRadius) / m_duration* Director::getInstance()->getAnimationInterval();
    
    m_debugColor = ccc4f(10*(arc4random()%25),10*(arc4random()%25),10*(arc4random()%25),150);
}

CircleMove* CircleMove::reverse() const
{
    CCASSERT(false, "reverse() not supported in CircleMove");
    return nullptr;
}

void CircleMove::stop(void)
{
    
    this->m_frameCnts = 0;
    m_initPos = _target->getPosition();
    
    m_debugColor = ccc4f(10*(arc4random()%25),10*(arc4random()%25),10*(arc4random()%25),150);
}
void CircleMove::update(float time)
{
    m_frameCnts++;
    float curRaduis = m_beginRadius+m_deltRadius*m_frameCnts;
    float angle = (m_angleOrig+m_frameCnts * m_anglePreFrame)*M_PI/180;
    log("=====angle==%f,%f,%d",m_frameCnts * m_anglePreFrame,angle,m_frameCnts);
    Vec2 pos = Vec2(curRaduis*cosf(angle), curRaduis*sinf(angle));
    pos = pos + m_center;
    _target->setPosition(pos);
    
    
    //debug
#if 0
    CCDrawNode *node = CCDrawNode::create();
    node->drawDot(pos,3,m_debugColor);
    _target->getParent()->addChild(node);
#endif
    
}
