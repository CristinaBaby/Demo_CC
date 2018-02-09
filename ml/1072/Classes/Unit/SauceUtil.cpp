
#include "SauceUtil.h"
#include "MakeEatLayer.h"

SauceUtil::SauceUtil()
{
}

SauceUtil::~SauceUtil()
{
}
bool SauceUtil::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void SauceUtil::initWithIndex(int index)
{
    m_pPlateBack = Sprite::create("content/category/sauce/sauce_cup.png");
    this->addChild(m_pPlateBack);
    
    
       std::stringstream ostr;
    ostr<<"content/category/sauce/sauce"<<index<<".png";
    m_pSauce = Sprite::create(ostr.str());
    this->addChild(m_pSauce);

    this->setContentSize(m_pPlateBack->getContentSize());
}

Node* SauceUtil::getDecorateResult()
{
    Node* node = Node::create();
    MakeEatLayer* pLayer = MakeEatLayer::create();
    Size size = Size(m_pSauce->getContentSize().width, m_pSauce->getContentSize().height);
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    if (m_pSauce) {
        Vec2 orginPo = m_pSauce->getPosition();
        m_pSauce->setPosition(Vec2(size.width/2, size.height/2));
        m_pSauce->visit();
        m_pSauce->setPosition(orginPo);
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    pLayer->setRenderTexture(rt);
    pLayer->setPosition(Vec2(-size.width/2, -size.height/2));
    
    Texture2D* texture = m_pPlateBack->getTexture();
    if (texture) {
        Sprite*pPlate = Sprite::createWithTexture(m_pPlateBack->getTexture());
        
        node->addChild(pPlate);
    }
    pLayer->setName("eatLayer");
    node->addChild(pLayer);
    return node;
}