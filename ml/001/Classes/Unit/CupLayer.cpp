
#include "CupLayer.h"
CupLayer::CupLayer()
{
    m_fHeightDelta = 5;
    m_nFlavorIndex = -1;
    m_pLidBack = nullptr;
    m_pLidFront = nullptr;
    m_pStraw = nullptr;
    m_pWater = nullptr;
}

CupLayer::~CupLayer()
{
}
bool CupLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void CupLayer::initWithIndex(int index)
{
    m_pCupShadow  = Sprite::create("content/make/cup/cup_shadow.png");
    this->addChild(m_pCupShadow);
    
    m_pCupBack = Sprite::create("content/make/cup/cup_down.png");
    this->addChild(m_pCupBack,2);
    
    m_pCupFront = Sprite::create("content/make/cup/cup_on.png");
    this->addChild(m_pCupFront,10);
    
    m_pLidBack = Sprite::create("content/make/cup/cup_lio_down.png");
    this->addChild(m_pLidBack,1);
    
    m_pLidFront = Sprite::create("content/make/cup/cup_lio_on.png");
    this->addChild(m_pLidFront,11);
    
    std::stringstream ostrParttern;
    ostrParttern<<"content/make/cup/pattern/"<<index+1<<".png";
    m_pPattern = Sprite::create(ostrParttern.str());
    this->addChild(m_pPattern,12);
    
    
    this->setContentSize(m_pCupFront->getContentSize());
}

void CupLayer::showWater(int flavor)
{
    std::stringstream ostr;
    ostr<<"content/make/cup/Donut/"<<flavor+1<<".png";
    if (!m_pWater) {
        m_pWater = Sprite::create(ostr.str());
//        m_pWater->setAnchorPoint(Vec2(0.5, 0));
//        m_pWater->setPosition(Vec2(0, -m_pWater->getContentSize().height));
        
        Sprite*pNode = Sprite::create("content/make/cup/cup_mask.png");
        m_pSodaClippingNode = ClippingNode::create(pNode);
        m_pSodaClippingNode->addChild(m_pWater);
        this->addChild(m_pSodaClippingNode,5);
        m_pSodaClippingNode->setAlphaThreshold(0.5);
    }else{
        m_pWater->setTexture(ostr.str());
    }
    m_nFlavorIndex = flavor;
    waterStartPos = m_pWater->getPosition();
    m_fCurHeight = m_pWater->getContentSize().height;
}

void CupLayer::updatePattern(int index)
{
    std::stringstream ostrParttern;
    ostrParttern<<"content/make/cup/pattern/"<<index+1<<".png";
    if (!m_pPattern) {
        m_pPattern = Sprite::create(ostrParttern.str());
        this->addChild(m_pLidFront,12);
    }else{
        m_pPattern->setTexture(ostrParttern.str());
    }
    
}

void CupLayer::showFinished(bool strawDirectionLeft)
{
    if (!m_pLidBack) {
        m_pLidBack = Sprite::create("content/make/cup/cup_lio_down.png");
        this->addChild(m_pLidBack,1);
        
        m_pLidFront = Sprite::create("content/make/cup/cup_lio_on.png");
        this->addChild(m_pLidFront,11);
    }
    if (!m_pStraw) {
        m_pStraw = Sprite::create("content/make/cup/straw.png");
        this->addChild(m_pStraw);
        m_pStraw->setAnchorPoint(Vec2(0.95, 0));
        m_pStraw->setPosition(Vec2(0, 120));
    }
    if (!strawDirectionLeft) {
        m_pStraw->setScaleX(-1);
    }
    m_pStraw->setLocalZOrder(12);
//    ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pSodaClippingNode->getChildByName("bubble");
//    if (!pParticle) {
//        pParticle = ParticleSystemQuad::create("particle/soda.plist");
//        m_pSodaClippingNode->addChild(pParticle);
//        pParticle->setPosition(Vec2(0,-100));
//        pParticle->setName("bubble");
//    }
    setWaterPercent(100);
}

void CupLayer::updateWater(int index)
{
    if (index==m_nFlavorIndex) {
        return;
    }
    std::stringstream ostr;
    ostr<<"content/make/cup/Donut/"<<index+1<<".png";
    if (m_pWater) {
        Sprite* pCopyWater = Sprite::createWithTexture(m_pWater->getTexture());
        m_pSodaClippingNode->addChild(pCopyWater);
        pCopyWater->setAnchorPoint(m_pWater->getAnchorPoint());
        pCopyWater->setPosition(m_pWater->getPosition());
        pCopyWater->setScaleX(m_pWater->getScaleX());
        pCopyWater->setScaleY(m_pWater->getScaleY());
        
        pCopyWater->runAction(FadeTo::create(2, 50));
        m_pWater->setTexture(ostr.str());
    }else{
        m_pWater = Sprite::create(ostr.str());
        m_pWater->setAnchorPoint(Vec2(0.5, 0));
        m_pWater->setPosition(Vec2(0, -m_pWater->getContentSize().height/2));
        m_pSodaClippingNode->addChild(m_pWater);
    }
    m_nFlavorIndex = index;
    
}

void CupLayer::setWaterPercent(int percent)
{
    if (percent>100) {
        percent = 100;
    }else if(percent<0){
        percent = 0;
        ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pSodaClippingNode->getChildByName("bubble");
        if (!pParticle){
            pParticle->removeFromParent();
        }
    }
    m_fPercent = percent;
    if (m_pWater) {
        m_fCurHeight = m_pWater->getContentSize().height*percent/100;
        m_pWater->setPosition(waterStartPos+Vec2(0,-m_pWater->getContentSize().height+m_fCurHeight));
    }
}

void CupLayer::addWater(float height)
{
    m_fHeightDelta = height;
    resumeAddWater();
//    m_pCupFront->setOpacity(100);
}

void CupLayer::pauseAddWater()
{
//    _scheduler->pauseTarget(this);
    unscheduleUpdate();
    ParticleSystemQuad* pNode = (ParticleSystemQuad*)this->getChildByName("juice_stream");
    if (pNode) {
        pNode->stopSystem();
        pNode->removeFromParent();
    }
}

void CupLayer::resumeAddWater()
{
//    _scheduler->resumeTarget(this);
    scheduleUpdate();
    std::stringstream ostr;
    ostr<<"content/make/machine/grain/"<<m_nFlavorIndex+1<<".png";
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/juice2.plist");
    pParticle->setTexture(Sprite::create(ostr.str())->getTexture());
    this->addChild(pParticle,4);
    pParticle->setPosition(waterStreamPos);
    pParticle->setName("juice_stream");
}

void CupLayer::update(float dt)
{
    if (m_pWater) {
        m_fCurHeight += m_fHeightDelta;
        int percent = 0;
//        log("=====contentsize = %f,%f",m_pWater->getContentSize().width,m_pWater->getContentSize().height);
        if (m_fCurHeight>m_pWater->getContentSize().height) {
            m_fCurHeight = m_pWater->getContentSize().height;
        }else if(m_fCurHeight<0){
            m_fCurHeight = 0;
        }
        percent = m_fCurHeight*100/m_pWater->getContentSize().height;
        m_fPercent = percent;
        log("====percent===%d",m_fPercent);
        float scale =1.0f*percent/100;
        if (scale<0.2) {
            scale = 0.2;
        }
//        m_pWater->setScaleY(scale);
        
        m_pWater->setPosition(waterStartPos+Vec2(0,-m_pWater->getContentSize().height+m_fCurHeight));
        
        ParticleSystemQuad*pParticle2 = (ParticleSystemQuad*)this->getChildByName("soda");
        if (pParticle2){
            Vec2 point = m_pWater->getPosition();
            if (m_fCurHeight<80) {
                point.y = 120-m_pWater->getContentSize().height/2;
            }else if (m_fCurHeight>m_pWater->getContentSize().height-20){
                point.y = m_pWater->getContentSize().height/2;
            }else{
                point.y = m_fCurHeight-m_pWater->getContentSize().height/2+50;
            }
            pParticle2->setPosition(point);
        }
        
        if (percent>=60){
//            ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pSodaClippingNode->getChildByName("bubble");
//            if (!pParticle) {
//                pParticle = ParticleSystemQuad::create("particle/soda.plist");
//                m_pSodaClippingNode->addChild(pParticle);
//                pParticle->setPosition(Vec2(0,-100));
//                pParticle->setName("bubble");
//            }
        }
        if (m_fCurHeight>=m_pWater->getContentSize().height || m_fCurHeight<=0) {
            unscheduleUpdate();
        }
    }
}

void CupLayer::saveWaterImage()
{
    RenderTexture* rt = RenderTexture::create(m_pWater->getContentSize().width, m_pWater->getContentSize().height);
    //    rt->setPosition(visibleSize*0.5);
    Vector<Node*>pChildVector = m_pSodaClippingNode->getChildren();
    
    rt->begin();
    for (int i = 0; i<pChildVector.size(); i++) {
        Node* pNode = pChildVector.at(i);
        if (pNode!=m_pSodaClippingNode->getStencil()) {
            Vec2 position = pNode->getPosition();
            pNode->setPosition(Vec2(m_pWater->getContentSize().width/2, 0));
            pNode->visit();
            pNode->setPosition(position);
        }
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = rt->newImage();
    pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"water.png", false);
}

void CupLayer::showOpenLid(bool open)
{
    if(open){
        this->runAction(MoveBy::create(0.5, Vec2(-100, 0)));
        m_pLidBack->runAction(JumpBy::create(0.5, Vec2(250, -250), 100, 1));
        m_pLidFront->runAction(JumpBy::create(0.5, Vec2(250, -250), 100, 1));
    }else{
        this->runAction(MoveBy::create(0.5, Vec2(100, 0)));
        m_pLidBack->runAction(JumpBy::create(0.5, Vec2(-250, 250), 100, 1));
        m_pLidFront->runAction(JumpBy::create(0.5, Vec2(-250, 250), 100, 1));
    }
}