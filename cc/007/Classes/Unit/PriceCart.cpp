
#include "PriceCart.h"
#include "Global.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"


using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

PriceCart::PriceCart()
{
    onPriceButtonCallback = nullptr;
}

PriceCart::~PriceCart()
{
    
}
bool PriceCart::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    Node* pCocosCart = CSLoader::createNode("PriceCartNode.csb");
    
    m_pCart = Sprite::create("content/sale/bg.jpg");
    this->addChild(m_pCart);
    
    m_pShelfLeftBack = Sprite::create("content/sale/shelf2_1.png");
    this->addChild(m_pShelfLeftBack);
    m_pShelfLeftBack->setPosition(pCocosCart->getChildByName("shelf2_1")->getPosition());
    
    m_pShelfLeftFront = Sprite::create("content/sale/shelf2_2.png");
    this->addChild(m_pShelfLeftFront,3);
    m_pShelfLeftFront->setPosition(pCocosCart->getChildByName("shelf2_2")->getPosition());
    
    m_pShelfRightBack = Sprite::create("content/sale/shelf1_1.png");
    this->addChild(m_pShelfRightBack);
    m_pShelfRightBack->setPosition(pCocosCart->getChildByName("shelf1_1")->getPosition());
    
    m_pShelfRightFront = Sprite::create("content/sale/shelf1_2.png");
    this->addChild(m_pShelfRightFront,10);
    m_pShelfRightFront->setPosition(pCocosCart->getChildByName("shelf1_2")->getPosition());
    
    for (int i = 0; i<6; i++) {
        Vec2 pos = pCocosCart->getChildByName("candy"+to_string(i))->getPosition();
        m_CandyPositions.push_back(pos);
        
        Vec2 posButton = pCocosCart->getChildByName("price_tickets"+to_string(i))->getPosition();
        Button* pButton = Button::create("content/sale/price_tickets.png");
        this->addChild(pButton,6);
        pButton->setPosition(posButton);
        pButton->setTag(i);
        m_PriceButtons.pushBack(pButton);
        pButton->addTouchEventListener(CC_CALLBACK_2(PriceCart::_onButtonCallback, this));
        
        if (i>=3) {
            pButton->setLocalZOrder(11);
        }
        Label* pLable = Label::createWithTTF("", font_felt, 20,Size(60,35));
        pButton->addChild(pLable);
        pLable->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        pLable->setPosition(Vec2(42, 50));
        pLable->setColor(Color3B::BLACK);
        pLable->setName("priceLabel");
    }
    return true;
}
