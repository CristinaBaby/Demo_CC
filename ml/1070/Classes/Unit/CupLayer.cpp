
#include "CupLayer.h"
CupLayer::CupLayer()
{
    m_fHeightDelta = 5;
    m_nFlavorIndex = -1;
    m_pCap = nullptr;
    m_pStraw = nullptr;
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

void CupLayer::initWithIndex(int index,int flavor)
{
    m_pCupBack = Sprite::create("content/make/soda/cup0.png");
    this->addChild(m_pCupBack);
    
    std::stringstream ostr;
    ostr<<"content/make/soda/cup/cup1_"<<index+1<<".png";
    m_pCupFront = Sprite::create(ostr.str());
    m_pCupFront->setLocalZOrder(10);
    this->addChild(m_pCupFront);
    
    Node* stencil = Node::create();
    Sprite*pNode = Sprite::create("content/make/soda/cup_mask.png");
    stencil->addChild(pNode);
    
    ostr.clear();
    ostr.str("");
    ostr<<"content/make/soda/soda/soda"<<flavor<<".png";
    m_pWater = Sprite::create(ostr.str());
    m_pWater->setAnchorPoint(Vec2(0.5, 0));
    m_pWater->setPosition(Vec2(0, -m_pWater->getContentSize().height/2));
    m_nFlavorIndex = flavor;
    
    m_pSodaClippingNode = ClippingNode::create(pNode);
    m_pSodaClippingNode->addChild(m_pWater);
    this->addChild(m_pSodaClippingNode);
    m_pSodaClippingNode->setAlphaThreshold(0.5);
    
    
    this->setContentSize(m_pCupFront->getContentSize());
    m_fCurHeight = m_pWater->getContentSize().height;
}

void CupLayer::showFinished(bool strawDirectionLeft)
{
    if (!m_pCap) {
        m_pCap = Sprite::create("content/make/soda/cup2.png");
        this->addChild(m_pCap);
        m_pCap->setPosition(Vec2(0, 0));
    }
    if (!m_pStraw) {
        m_pStraw = Sprite::create("content/make/soda/straw.png");
        this->addChild(m_pStraw);
        m_pStraw->setAnchorPoint(Vec2(0.95, 0));
        m_pStraw->setPosition(Vec2(0, 120));
    }
    if (!strawDirectionLeft) {
        m_pStraw->setScaleX(-1);
    }
    m_pCap->setLocalZOrder(11);
    m_pStraw->setLocalZOrder(12);
    ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pSodaClippingNode->getChildByName("bubble");
    if (!pParticle) {
        pParticle = ParticleSystemQuad::create("particle/soda.plist");
        m_pSodaClippingNode->addChild(pParticle);
        pParticle->setPosition(Vec2(0,-100));
        pParticle->setName("bubble");
    }
    setWaterPercent(100);
}

void CupLayer::updateWater(int index)
{
    if (index==m_nFlavorIndex) {
        return;
    }
    std::stringstream ostr;
    ostr<<"content/make/soda/soda/soda"<<index<<".png";
    if (m_pWater) {
        Sprite* pCopyWater = Sprite::createWithTexture(m_pWater->getTexture());
        m_pSodaClippingNode->addChild(pCopyWater);
        pCopyWater->setAnchorPoint(m_pWater->getAnchorPoint());
        pCopyWater->setPosition(m_pWater->getPosition());
        pCopyWater->setScaleX(m_pWater->getScaleX());
        pCopyWater->setScaleY(m_pWater->getScaleY());
//        pCopyWater->runAction(Sequence::create(FadeOut::create(1),
//                                               CallFunc::create([=]()
//                                                                {
//                                                                    pCopyWater->removeFromParent();
//                                                                }), NULL));
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
        m_pWater->setScaleY(0.1+1.0f*percent/100);
        m_fCurHeight = m_pWater->getContentSize().height*percent/100;
    }
}

void CupLayer::addWater(float height)
{
    m_fHeightDelta = height;
    scheduleUpdate();
//    resumeAddWater();
//    m_pCupFront->setOpacity(100);
}

void CupLayer::pauseAddWater()
{
//    _scheduler->pauseTarget(this);
    unscheduleUpdate();
}

void CupLayer::resumeAddWater()
{
//    _scheduler->resumeTarget(this);
    unscheduleUpdate();
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
        m_pWater->setScaleY(scale);
        
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
            ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pSodaClippingNode->getChildByName("bubble");
            if (!pParticle) {
                pParticle = ParticleSystemQuad::create("particle/soda.plist");
                m_pSodaClippingNode->addChild(pParticle);
                pParticle->setPosition(Vec2(0,-100));
                pParticle->setName("bubble");
            }
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