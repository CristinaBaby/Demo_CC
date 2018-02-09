
#include "ChooseBackgroudLayer.h"
#include "CMVisibleRect.h"

Door *Door::create(int index)
{
    auto pRet = new Door();
    if (pRet && pRet->init(index)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = NULL;
    return NULL;
}

bool Door::init(int index)
{
    if (!Node::init()) {
        return false;
    }
    m_nIndex = index;
    m_pLight = Sprite::create("content/make/dec/choosebg/light.png");
    m_pLight->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(m_pLight);
    m_pLight->setOpacity(0);
//    m_pLight->setScale(0.8,0.1);
    
    std::stringstream ostr;
    ostr<<"content/make/dec/choosebg/room"<<index<<"_0.png";
    m_pDoor = Button::create(ostr.str(),ostr.str(),ostr.str());
    m_pDoor->setAnchorPoint(Vec2(0.5, 0));
    this->addChild(m_pDoor);
    m_pDoor->addTouchEventListener(CC_CALLBACK_2(Door::_onButtonCallback, this));
    return true;
}

void Door::openDoor()
{
    if (m_pLight) {
        m_pLight->runAction(FadeIn::create(0.5));
    }
    if (m_pDoor) {
        std::stringstream ostr;
        ostr<<"content/make/dec/choosebg/room"<<m_nIndex<<"_1.png";
        m_pDoor->loadTextures(ostr.str(), ostr.str(),ostr.str());
    }
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dooropen.plist");
    m_pDoor->addChild(pParticle);
    pParticle->setPosition(m_pDoor->getContentSize()*0.5);
    pParticle->setName("paritcle");
}

void Door::closeDoor()
{
    if (m_pLight) {
        m_pLight->runAction(FadeOut::create(0.5));
    }
    if (m_pDoor) {
        std::stringstream ostr;
        ostr<<"content/make/dec/choosebg/room"<<m_nIndex<<"_0.png";
        m_pDoor->loadTextures(ostr.str(), ostr.str(),ostr.str());
        Node*pNode = m_pDoor->getChildByName("paritcle");
        if (pNode) {
            pNode->removeFromParent();
        }
    }
}

void Door::_onButtonCallback(Ref*,Widget::TouchEventType type)
{
    
    if (type == Widget::TouchEventType::ENDED) {
        openDoor();
        if (onDoorCallback) {
            onDoorCallback(m_nIndex);
        }
    }
}
                                   
ChooseBackgroudLayer::ChooseBackgroudLayer()
{
    onItemCallback = nullptr;
}

ChooseBackgroudLayer::~ChooseBackgroudLayer()
{
    
}
bool ChooseBackgroudLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    LayerColor* pColor = LayerColor::create(Color4B(160, 160, 160, 160));
    this->addChild(pColor);
    
    Sprite* pWall = Sprite::create("content/make/dec/choosebg/wall.png");
    this->addChild(pWall);
    CMVisibleRect::setPosition(pWall, 480, 320);
    m_pWall = pWall;
    
    for (int i = 0; i<3; i++) {
        Door* pDoor = Door::create(i);
        pWall->addChild(pDoor,1);
        pDoor->setScale(0.1);
        pDoor->setVisible(false);
        pDoor->runAction(Sequence::create(DelayTime::create(0.5+0.1*i),
                                          CallFunc::create([=]()
                                                           {
                                                               pDoor->setVisible(true);
                                                           }),
                                          ScaleTo::create(0.3, 1), NULL));
        pDoor->setPosition(180+250*i, 90);
        pDoor->onDoorCallback = CC_CALLBACK_1(ChooseBackgroudLayer::_onDoorCallback, this);
        pDoor->setTag(i);
        m_VectorDoor.pushBack(pDoor);
    }
    
    m_pNext = cocos2d::ui::Button::create("content/common/button/btn_next.png");
    this->addChild(m_pNext);
    m_pNext->setVisible(false);
    m_pNext->setEnabled(false);
    m_pNext->addTouchEventListener(CC_CALLBACK_2(ChooseBackgroudLayer::onButtonCallback, this));
    CMVisibleRect::setPosition(m_pNext, 10, 10,kBorderRight,kBorderTop);
    
    pWall->setPosition(Vec2(pWall->getPositionX(), pWall->getPositionY()+1000));
    pWall->runAction(Sequence::create(DelayTime::create(0.5),
                                      EaseBackOut::create(MoveBy::create(1, Vec2(0, -1000))), NULL));
    pWall->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.5, 1,0.98),
                                                            ScaleTo::create(2, 0.98,1),
                                                            ScaleTo::create(2, 1),
                                                            DelayTime::create(1), NULL)));
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch*,Event*){
        return true;
    }
    ;
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void ChooseBackgroudLayer::onButtonCallback(Ref*,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (onItemCallback) {
            onItemCallback(m_nIndex);
        }
        m_pNext->setVisible(false);
        m_pNext->setEnabled(false);
        _dismiss();
    }
}

void ChooseBackgroudLayer::_onDoorCallback(int index)
{
    m_nIndex = index;
    std::for_each(m_VectorDoor.begin(), m_VectorDoor.end(), [=](Door*pDoor){
        if (pDoor->getTag()==m_nIndex) {
            pDoor->setDoorTouchEnable(false);
        }else{
            pDoor->setDoorTouchEnable(true);
            pDoor->closeDoor();
        }
    });
    m_pNext->setVisible(true);
    m_pNext->setEnabled(true);
}

void ChooseBackgroudLayer::_dismiss()
{
    m_pWall->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(1, Vec2(0, 1000))),
                                        CallFunc::create([=]()
                                                         {
                                                             this->removeFromParent();
                                                         }),
                                        NULL));

}