
#include "SugarBottle.h"
#include "Global.h"
#include "AudioHelp.h"

SugarBottle::SugarBottle()
{
    m_pMask = nullptr;
    m_pSugar = nullptr;
}

SugarBottle::~SugarBottle()
{
//    CC_SAFE_RELEASE_NULL(m_pMask);
//    CC_SAFE_RELEASE_NULL(m_pSugar);
}
bool SugarBottle::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_pShadow = Sprite::create("content/make/cotton/1/bottle_shadow.png");
    this->addChild(m_pShadow);
    
    m_pBottleBack = Sprite::create("content/make/cotton/1/bottle_down.png");
    this->addChild(m_pBottleBack);
    
//    m_pClippingNode = ClippingNode::create(Sprite::create("content/make/cotton/1/taste_brand3/mask.png"));
//    this->addChild(m_pClippingNode);
//    m_pClippingNode->setPosition(Vec2(0, -21));
//    m_pClippingNode->setAlphaThreshold(0.5);
    
    m_pMask = Sprite::create("content/make/cotton/1/taste_brand3/mask.png");
    m_pMask->retain();
    m_pMask->setBlendFunc((ccBlendFunc){GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA});
    m_pRender = RenderTexture::create(m_pMask->getContentSize().width, m_pMask->getContentSize().height);
    this->addChild(m_pRender);
//    m_pRender->setPosition(Vec2(0, -21));
    
    m_pLid = Sprite::create("content/make/cotton/1/bottle1.png");
    this->addChild(m_pLid, 4);
//    m_pLid->setPosition(Vec2(0.5, 20));
    
    m_pBottleFront = Sprite::create("content/make/cotton/1/bottle_on.png");
    this->addChild(m_pBottleFront,5);
    
    return true;
}

void SugarBottle::initSugar(int index)
{
    m_nFlavor = index;
    std::stringstream ostr;
    ostr<<"content/make/cotton/1/taste_brand3/"<<index+1<<".png";
    m_pSugar = Sprite::create(ostr.str());
//    m_pClippingNode->addChild(m_pSugar);
//    m_pSugar->setPosition(Vec2(3, -1));
    m_pSugar->setPosition(m_pMask->getContentSize()*0.5);
    m_pMask->setPosition(m_pMask->getContentSize()*0.5);
    m_pSugar->retain();
    m_pRender->begin();
    m_pSugar->visit();
    m_pMask->visit();
    m_pRender->end();
    
    
    Sprite* pTagBg = Sprite::create("content/make/cotton/1/taste_brand2/taste_brand.png");
    this->addChild(pTagBg,6);
    pTagBg->setPosition(Vec2(12, -12));
    
    std::stringstream ostrTag;
    ostrTag<<"content/make/cotton/1/taste_brand2/"<<index+1<<".png";
    m_pTag = Sprite::create(ostrTag.str());
    this->addChild(m_pTag,6);
    m_pTag->setPosition(Vec2(12, -12));
    
    std::stringstream ostrLabel;
    ostrLabel<<"content/make/cotton/1/taste_brand1/"<<index+1<<".png";
    m_pLabel = Sprite::create(ostrLabel.str());
    m_pLid->addChild(m_pLabel);
    m_pLabel->setPosition(Vec2(95, 198));
}

void SugarBottle::openLid()
{
    m_pLid->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(2, visibleSize)),
                                       CallFunc::create([=](){
        m_pLid->setVisible(false);
    }), NULL));
}

void SugarBottle::CoverLid()
{
    m_pLid->setVisible(true);
    m_pLid->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(2, Vec2(-visibleSize.width, -visibleSize.height))), NULL));
}

void SugarBottle::graySugar()
{
    if (m_pSugar) {
        Sprite* pSprite = Sprite::createWithTexture(m_pSugar->getTexture());
        pSprite->setColor(Color3B::GRAY);
        pSprite->setPosition(m_pSugar->getPosition());
        m_pRender->begin();
        pSprite->visit();
        m_pMask->visit();
        m_pRender->end();
    }
}

void SugarBottle::pourSugar()
{
    this->openLid();
    this->runAction(Sequence::create(RotateBy::create(0.5, 120),
                                     CallFunc::create([=]()
                                                      {
                                                          ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/poursugar.plist");
                                                          this->getParent()->addChild(pParticle);
                                                          std::ostringstream ostr;
                                                          ostr<<"content/make/cotton/1/particle/"<<m_nFlavor+1<<".png";
                                                          Vec2 pos = Vec2(97, 193);
                                                          pos = this->getParent()->convertToNodeSpace(m_pBottleBack->convertToWorldSpace(pos));
                                                          pParticle->setTexture(Sprite::create(ostr.str())->getTexture());
                                                          pParticle->setPosition(pos);
                                                          m_pRender->runAction(ScaleTo::create(2.5, 0.6,0.0));
                                                          m_pRender->runAction(MoveBy::create(2.5, Vec2(0, 50)));
//                                                          m_pSugar->runAction(ScaleTo::create(1, 0.6,0.5));
//                                                          m_pSugar->runAction(MoveBy::create(1, Vec2(0, 40)));
                                                          AudioHelp::getInstance()->playPourPowderEffect();
                                                      }),
                                     DelayTime::create(3),
                                     RotateBy::create(0.3, -120),
                                     CallFunc::create([=]()
                                                      {
                                                          this->CoverLid();
                                                      }), NULL));
}