
#include "CongratulationLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"
#include "AudioHelp.h"

CongratulationLayer::CongratulationLayer()
{
    
}

CongratulationLayer::~CongratulationLayer()
{
    
}


bool CongratulationLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setSwallowsTouches(true);
    this->setTouchEnabled(true);
    
    LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,150));
    this->addChild(pLayer);
    
    
    Sprite *pLight = Sprite::create("content/award/light.png");;
    this->addChild(pLight);
    CMVisibleRect::setPositionAdapted(pLight, 480, 300);
    
    Sprite*pAward = Sprite::create("content/award/trophy0.png");
    this->addChild(pAward);
    pAward->setAnchorPoint(Vec2(0.5, 0));
    CMVisibleRect::setPositionAdapted(pAward, 480, 200);
    
    Sprite*pLetter = Sprite::create("content/award/text_c.png");
    this->addChild(pLetter);
    CMVisibleRect::setPositionAdapted(pLetter, 480, 450+visibleSize.height);
    
    Sprite*pBar = Sprite::create("content/award/bar.png");
    this->addChild(pBar);
    CMVisibleRect::setPositionAdapted(pBar, 480, 160);
    
    pLight->setOpacity(0);
    pAward->setOpacity(0);
//    pBar->setOpacity(0);
    
    pBar->setOpacity(0);
    pBar->setScale(0.1, 0.9);
    pAward->setScale(0.1);
    pBar->runAction(Sequence::create(Spawn::create(FadeIn::create(0.05),
                                                      ScaleTo::create(0.1, 1), NULL),
                                        CallFunc::create([=]()
                                                         {
                                                             AudioHelp::getInstance()->playEffect("congratulations.mp3");
                                                             pAward->runAction(Sequence::create(Spawn::create(FadeIn::create(0.1),
                                                                                                              EaseBackOut::create(ScaleTo::create(0.5, 1)), NULL),
                                                                                                CallFunc::create([=]()
                                                                                                                 {
                                                                                                                     pLight->runAction(RepeatForever::create(Sequence::create(EaseBounceOut::create(FadeTo::create(0.5, 255)),
                                                                                                                                                                              FadeTo::create(0.5, 180), NULL)));
                                                                                                                     pLetter->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(2, Vec2(0, -visibleSize.height))),
                                                                                                                                                      CallFunc::create([=]()
                                                                                                                                                                       {
                                                                                                                                                                           
                                                                                                                                                                           Sprite*pRibbon = Sprite::create("content/award/ribbon.png");
                                                                                                                                                                           this->addChild(pRibbon);
                                                                                                                                                                           CMVisibleRect::setPositionAdapted(pRibbon, 480, 350);
                                                                                                                                                                           pRibbon->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]()
                                                                                                                                                                                                                                                      {
                                                                                                                                                                                                                                                          pRibbon->setScale(-1);
                                                                                                                                                                                                                                                      }),
                                                                                                                                                                                                                                     DelayTime::create(0.3),
                                                                                                                                                                                                                                     CallFunc::create([=]()
                                                                                                                                                                                                                                                      {
                                                                                                                                                                                                                                                          pRibbon->setScale(1);
                                                                                                                                                                                                                                                      }),
                                                                                                                                                                                                                                     DelayTime::create(0.3), NULL)));
                                                                                                                                                                           ParticleSystemQuad*pParticle = ParticleSystemQuad::create("particle/ribbon0.plist");
                                                                                                                                                                           this->addChild(pParticle);
                                                                                                                                                                           CMVisibleRect::setPositionAdapted(pParticle, 500, 450);
//
//                                                                                                                                                                           ParticleSystemQuad*pParticle2 = ParticleSystemQuad::create("particle/ribbon1.plist");
//                                                                                                                                                                           this->addChild(pParticle2);
//                                                                                                                                                                           CMVisibleRect::setPositionAdapted(pParticle2, 480, 450);
                                                                                                                                                                           
                                                                                                                                                                       }),
                                                                                                                                                      DelayTime::create(3),
                                                                                                                                                      CallFunc::create([=]()
                                                                                                                                                                       {
                                                                                                                                                                           this->removeFromParent();
                                                                                                                                                                       }), NULL));
                                                                                                                 }), NULL));
                                                         }), NULL));
    
    return true;
}

void CongratulationLayer::onEnter()
{
    Layer::onEnter();
}

bool CongratulationLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
