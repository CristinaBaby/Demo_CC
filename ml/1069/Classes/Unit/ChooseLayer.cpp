
#include "ChooseLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"

ChooseLayer::ChooseLayer()
{
    onItemCallback = nullptr;
    m_bSelected =false;
}

ChooseLayer::~ChooseLayer()
{
    
}

bool ChooseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    LayerColor* pColor = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(pColor);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* touch,Event*){
        return true;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ChooseLayer::showChooseFrosing()
{
    Sprite* pTitle = Sprite::create("content/common/tip/font_frosting.png");
    this->addChild(pTitle);
    CMVisibleRect::setPosition(pTitle, 480, 500+600,kBorderNone,kBorderBottom);
    pTitle->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -600))));
    
    for (int i = 0; i<3; i++) {
        LockItem* item = LockItem::create();
        stringstream ostr;
        ostr<<"content/common/make/frosting_"<<i<<".png";
        item->setBg("content/common/make/frosting_box.png");
        item->initWithName(ostr.str(), "");
        this->addChild(item);
        item->onItemClicked = CC_CALLBACK_1(ChooseLayer::_onItemCallback, this);
        CMVisibleRect::setPosition(item, 200+273*i+visibleSize.width, 260);
        item->setItemTouchEnable(false);
        item->runAction(Sequence::create(DelayTime::create(0.5+0.3*i),
                                         EaseBounceOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              item->setItemTouchEnable();
                                                          }), NULL));
        item->setTag(i);
    }
}

void ChooseLayer::showChooseIcing()
{
    Sprite* pTitle = Sprite::create("content/common/tip/font_icing.png");
    this->addChild(pTitle);
    CMVisibleRect::setPosition(pTitle, 480, 600+600,kBorderNone,kBorderBottom);
    pTitle->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -600))));
    
    for (int i = 0; i<6; i++) {
        stringstream ostr;
        ostr<<"content/common/make/icing_"<<i<<".png";
        LockItem* item = LockItem::create();
        item->initWithName(ostr.str(), "");
        this->addChild(item);
        item->onItemClicked = CC_CALLBACK_1(ChooseLayer::_onItemCallback, this);
        int row = i/3;
        int col = i%3;
        CMVisibleRect::setPosition(item, 230+270*col+visibleSize.width, 400-row*200);
        item->setItemTouchEnable(false);
        item->runAction(Sequence::create(DelayTime::create(0.5+0.3*i),
                                         EaseBounceOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              item->setItemTouchEnable();
                                                          }), NULL));
        item->setTag(i);
    }
}

void ChooseLayer::_onItemCallback(LockItem* item)
{
    if (m_bSelected) {
        return;
    }
    m_bSelected = true;
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/click.plist");
    item->addChild(pParticle);
    pParticle->setPosition(Vec2::ZERO);
    this->runAction(Sequence::create(DelayTime::create(0.8),
                                     CallFunc::create([=]()
                                                      {
                                                          if (onItemCallback) {
                                                              onItemCallback(item->getTag());
                                                          }
                                                          this->removeFromParent();
                                                      }), NULL));
}