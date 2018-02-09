
#include "TipLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"

TipLayer::TipLayer()
{
    
}

TipLayer::~TipLayer()
{
    
}


bool TipLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setSwallowsTouches(true);
    this->setTouchEnabled(true);
    
    LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,150));
    this->addChild(pLayer);
    
    
    m_pBg = Sprite::create("content/common/frame.png");
    this->addChild(m_pBg);
    m_pBg->setAnchorPoint(Vec2(0, 0.5));
    CMVisibleRect::setPositionAdapted(m_pBg, 480-visibleSize.width, 200);
  
    m_pContentLabel = Label::createWithTTF("",font_felt.c_str(), fontsize(30),Size(335, 175),TextHAlignment::CENTER,TextVAlignment::CENTER);
    m_pContentLabel->setDimensions(335,175);
    
    
//    m_pContentLabel->setAnchorPoint(Vec2(0.5,1));
    m_pContentLabel->setColor(Color3B(230,56,154));
    m_pContentLabel->setPosition(Vec2(480, 155));
    m_pBg->addChild(m_pContentLabel);
    
    return true;
}

void TipLayer::onEnter()
{
    Layer::onEnter();
}

bool TipLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void TipLayer::showTip(std::string text,bool animate)
{
    m_pContentLabel->setString(text);
    if (!animate) {
        CMVisibleRect::setPositionAdapted(m_pBg, 110, 320);
        m_pBg->runAction(Sequence::create(DelayTime::create(0.5),
                                                    DelayTime::create(3),
                                                    CallFunc::create([=]()
                                                                     {
                                                                         this->removeFromParent();
                                                                     }),
                                                    NULL));
    }else{
        CMVisibleRect::setPositionAdapted(m_pBg, 110-visibleSize.width, 320);
        m_pBg->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(3, Vec2(visibleSize.width, 0))),
                                          DelayTime::create(1),
                                          CallFunc::create([=]()
        {
            this->removeFromParent();
        }),
                                          NULL));
    }
}