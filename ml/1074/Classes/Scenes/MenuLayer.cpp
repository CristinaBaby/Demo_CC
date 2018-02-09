
#include "MenuLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"

bool MenuLayer::sIngredientVector[5][10] = {};
MenuLayer::MenuLayer()
{
    m_pMenu = nullptr;
    m_bCanHide = false;
    m_bCanTouch = false;
}

MenuLayer::~MenuLayer()
{
    
}

bool MenuLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    LayerColor* pColor = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(pColor);
    
    m_pFrame = Sprite::create("content/make/common/menu/menu_frame.png");
    this->addChild(m_pFrame);
    CMVisibleRect::setPosition(m_pFrame, 10, 10,kBorderLeft,kBorderTop);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* touch,Event*){
        return m_bCanTouch;
    };
    listener->onTouchEnded = [=](Touch* touch,Event*){
        if(m_bCanHide){
            hideMene();
        }
    };
    this->runAction(Sequence::create(DelayTime::create(5),
                                     CallFunc::create([=](){
        m_bCanHide = true;
    }), NULL));
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void MenuLayer::showMenu(int index,bool canTouch)
{
    m_bCanTouch = canTouch;
    m_bCanHide = canTouch;
    static std::string sPackName[] = {
        "ingredients_cupcake.png",
        "ingredients_cake.png",
        "ingredients_donut.png",
    };
    std::string name = sPackName[index];
    if (!m_pMenu) {
        m_pMenu = Sprite::create("content/make/common/menu/"+name);
        m_pFrame->addChild(m_pMenu);
    }else{
        m_pMenu->setTexture("content/make/common/menu/"+name);
    }
    
    m_pMenu->setPosition(m_pFrame->getAnchorPointInPoints());
//    m_pMenu->setScale(0.2);
//    m_pMenu->setOpacity(0);
    
    this->setVisible(true);
    
    CMVisibleRect::setPositionAdapted(m_pFrame, 50, 800,kBorderLeft,kBorderTop);
    m_pFrame->setScale(0.1);
    
//    m_pFrame->runAction(MoveTo::create(1, visibleSize*0.5));
//    m_pFrame->runAction(ScaleTo::create(1, 1));
//    m_pFrame->runAction(RotateBy::create(0.9, 360));
    m_pFrame->runAction(Sequence::create(DelayTime::create(0.5),
                                         Spawn::create(MoveTo::create(1, visibleSize*0.5),
                                                       ScaleTo::create(1, 1),
                                                       RotateBy::create(0.9, 360), NULL),
                                         NULL));
    this->runAction(Sequence::create(DelayTime::create(5),
                                     CallFunc::create([=](){
        m_bCanHide = false;
        hideMene();
    }), NULL));
    
    static Vec2 startPos = Vec2(230, 630);
    static float delt = 100;
    for (int i = 0; i<10; i++) {
        if (sIngredientVector[index][i]) {
            Sprite* pSprite = Sprite::create("content/make/common/menu/tick.png");
            m_pFrame->addChild(pSprite);
            
            pSprite->setPosition(startPos+Vec2(i%2*210, -delt*(i/2)));
            pSprite->setScale(0.2);
            pSprite->setOpacity(0);
            pSprite->runAction(Sequence::create(DelayTime::create(1),
                                                FadeIn::create(0.2),
                                                EaseBackOut::create(ScaleTo::create(0.5, 1)), NULL));
        }
    }
}

void MenuLayer::hideMene()
{
    m_pFrame->runAction(ScaleTo::create(0.5, 0.2));
    m_pFrame->runAction(Sequence::create(MoveTo::create(1, CMVisibleRect::getPosition(50, 200,kBorderLeft,kBorderTop)),
                                         CallFunc::create([=](){
        this->setVisible(false);
        this->removeFromParent();
    }), NULL));
}
