
#include "CottonMachine.h"
#include "AnimationHelp.h"
#include "AudioHelp.h"

CottonMachine::CottonMachine()
{
    onSwitchTurnOn = nullptr;
}

CottonMachine::~CottonMachine()
{
    
}
bool CottonMachine::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_nFlavor = 0;
    m_pMaskMiddle = Sprite::create("content/make/make/middle_mask.png");
    m_pMaskOut = Sprite::create("content/make/make/mask2.png");
    
//    m_pShadow = Sprite::create("content/make/make/shadow.png");
//    this->addChild(m_pShadow, 0);
//    m_pShadow->setPosition(Vec2(-110, -90));
    
    m_pBodyBack = Sprite::create("content/make/make/machine_after.png");
    this->addChild(m_pBodyBack, 0);
    
    m_pBodyMiddle = Sprite::create("content/make/make/middle_animation1.png");
    this->addChild(m_pBodyMiddle, 1);
    m_pBodyMiddle->setPosition(Vec2(0, 95));
    
    m_pBodyFront = Sprite::create("content/make/make/machine_before.png");
    this->addChild(m_pBodyFront, 10);
    
    m_pSwitch = Button::create("content/make/make/off.png","content/make/make/off.png","content/make/make/off.png");
    this->addChild(m_pSwitch, 11);
    m_pSwitch->setPosition(Vec2(5, -230));
    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(CottonMachine::onButtonCallback, this));
    
    m_pMiddleClp = ClippingNode::create(m_pMaskMiddle);
    this->addChild(m_pMiddleClp, 1);
    m_pMiddleClp->setAlphaThreshold(0.5);
    m_pMiddleClp->setPosition(Vec2(0, 150));
    
    m_pOutClp = ClippingNode::create(m_pMaskOut);
    this->addChild(m_pOutClp, 6);
    m_pOutClp->setPosition(Vec2(0, 200+50));
    m_pOutClp->setAlphaThreshold(0.5);
    
    m_bIsOn = false;
    m_pSwitch->setEnabled(false);
    
    std::stringstream ostrFront ;
    ostrFront<<"content/make/make/before/before"<<m_nFlavor+1<<"_4.png";
    std::stringstream ostrBack ;
    ostrBack<<"content/make/make/after/after"<<m_nFlavor+1<<"_4.png";
    m_pSilkBack = Sprite::create(ostrBack.str());
    this->addChild(m_pSilkBack);
    m_pSilkBack->setPosition(Vec2(0, 130));
    
    m_pSilkFront = Sprite::create(ostrFront.str());
    this->addChild(m_pSilkFront);
    m_pSilkFront->setLocalZOrder(5);
    m_pSilkFront->setPosition(Vec2(0, 130));
    
    m_pSilkBack->setOpacity(0);
    m_pSilkFront->setOpacity(0);
    return true;
}
void CottonMachine::onExit()
{
    Node::onExit();
    AudioHelp::getInstance()->stopLoopEffect();
}

void CottonMachine::onButtonCallback(Ref*ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        m_bIsOn = !m_bIsOn;
        _turnOn(m_bIsOn);
    }
}

void CottonMachine::_turnOn(bool on)
{
    if (on) {
        AudioHelp::getInstance()->playLoopEffect("machine.mp3");
        m_pSwitch->loadTextures("content/make/make/on.png", "content/make/make/on.png", "content/make/make/on.png");
        m_pBodyMiddle->stopAllActions();
        m_pBodyMiddle->runAction(RepeatForever::create(AnimationHelp::createAnimate("content/make/make/middle_animation", 1, 2,false,true,0.01)));
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bombsugar2.plist");
        std::ostringstream ostr;
        ostr<<"content/make/flavor/cottoncandy_particle/particle"<<m_nFlavor+1<<".png";
        pParticle->setTexture(Sprite::create(ostr.str())->getTexture());
        this->addChild(pParticle,10);
        pParticle->setPosition(m_pMiddleClp->getPosition());
        
        _makeSilk();
    }else{
        AudioHelp::getInstance()->stopLoopEffect();
        m_pSwitch->loadTextures("content/make/make/off.png", "content/make/make/off.png");
        m_pBodyMiddle->stopAllActions();
    }
    if (onSwitchTurnOn) {
        onSwitchTurnOn(on);
    }
}

void CottonMachine::addSugar(int index,bool animate)
{
    m_nFlavor = index;
    std::stringstream ostr;
    ostr<<"content/make/flavor/taste/"<<index+1<<".png";
    Sprite* pSugar = Sprite::create(ostr.str());
    pSugar->setName("sugar");
    m_pMiddleClp->addChild(pSugar);
    if (animate) {
        m_pMiddleClp->setScale(0.1);
        m_pMiddleClp->runAction(Sequence::create(DelayTime::create(0.3),
                                                 EaseSineIn::create(ScaleTo::create(1, 1)), NULL));
    }
}

void CottonMachine::_makeSilk()
{
    m_pSilkBack->runAction(Sequence::create(FadeIn::create(1), NULL));
    m_pSilkFront->runAction(Sequence::create(FadeIn::create(1), NULL));
    std::stringstream ostrFront ;
    ostrFront<<"content/make/make/before/before"<<m_nFlavor+1<<"_";
    std::stringstream ostrBack ;
    ostrBack<<"content/make/make/after/after"<<m_nFlavor+1<<"_";
    m_pSilkBack->runAction(AnimationHelp::createAnimate(ostrBack.str(), 1, 4,false,true,0.8));
    m_pSilkFront->runAction(AnimationHelp::createAnimate(ostrFront.str(), 1, 4,false,true,1));
    
    m_pSilkBack->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        m_pSilkBack->setScaleX(-1);
    }),DelayTime::create(0.5),
                                                                  CallFunc::create([=](){
        m_pSilkBack->setScaleX(1);
    }),DelayTime::create(0.5), NULL)));
    m_pSilkFront->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        m_pSilkFront->setScaleX(-1);
    }),DelayTime::create(0.5),
                                                                  CallFunc::create([=](){
        m_pSilkFront->setScaleX(1);
    }),DelayTime::create(0.5), NULL)));
    
//    Node* pSugar = m_pMiddleClp->getChildByName("sugar");
//    if (pSugar) {
//        pSugar->runAction(ScaleTo::create(3, 0.3));
//        pSugar->runAction(MoveBy::create(4, Vec2(0, -50)));
//    }
    m_pMiddleClp->runAction(Sequence::create(ScaleTo::create(3, 0.3),
                                             CallFunc::create([=]()
                                                              {
                                                                  m_pMiddleClp->setVisible(false);
                                                              }), NULL));
    m_nCottonStep = 0;
}

bool CottonMachine::makeCotton()
{
    m_nCottonStep++;
    if (m_nCottonStep>=4) {
        return true;
    }
    std::stringstream ostrFront ;
    ostrFront<<"content/make/make/before/before"<<m_nFlavor+1<<"_"<<4-m_nCottonStep<<".png";
    std::stringstream ostrBack ;
    ostrBack<<"content/make/make/after/after"<<m_nFlavor+1<<"_"<<4-m_nCottonStep<<".png";
    m_pSilkBack->setTexture(ostrBack.str());
    m_pSilkFront->setTexture(ostrFront.str());
    if (m_nCottonStep>=3) {
        m_pSilkBack->runAction(FadeOut::create(1));
        m_pSilkFront->runAction(FadeOut::create(1));
        return true;
    }
    return false;
}