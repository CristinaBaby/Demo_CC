
#include "HomeCart.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

HomeCart::HomeCart()
{
    onCartClickCallback = nullptr;
}

HomeCart::~HomeCart()
{
    
}

bool HomeCart::init()
{
    if (!Node::init()) {
        return false;
    }
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        return true;
    };
    listener->onTouchEnded = [=](Touch* pTouch,Event* event){
        if (!m_bTouchEnable) {
            return;
        }
        Vec2 location = pTouch->getLocation();
        Rect rect = m_pBody->getBoundingBox();
        rect.origin = this->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(location)) {
            if (onCartClickCallback) {
                onCartClickCallback(this);
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void HomeCart::loadCartType(int type,bool left)
{
    m_bIsLeft = left;
    Node* pCocosNode = CSLoader::createNode("HomeCartNode.csb");
    std::string name = "cart"+std::to_string(type);
    if (!left) {
        name.append("_r");
    }
    Node* pCocosCart = pCocosNode->getChildByName(name);
    m_pBody = Sprite::create("content/start/"+name+".png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pCocosCart->getChildByName(name)->getPosition());
    
    int wheelCount = pCocosCart->getChildrenCount()-1;
    for (int i =0; i<wheelCount; i++) {
        Sprite* pWheel = Sprite::create("content/start/tyre"+std::to_string(type)+".png");
        this->addChild(pWheel);
        Node* pNode = pCocosCart->getChildByName("tyre"+std::to_string(type)+"_"+std::to_string(i));
        pWheel->setPosition(pNode->getPosition());
        pWheel->setScale(pNode->getScaleX());
        m_pWheelVector.pushBack(pWheel);
    }
    
}
void HomeCart::runCart()
{
    for_each(m_pWheelVector.begin(),m_pWheelVector.end(),[=](Sprite* pWheel){
        if(m_bIsLeft){
            pWheel->runAction(RepeatForever::create(RotateBy::create(1, -360)));
        }else{
            pWheel->runAction(RepeatForever::create(RotateBy::create(1, 360)));
        }
        
    });
}

void HomeCart::stopRunCart()
{
    for_each(m_pWheelVector.begin(),m_pWheelVector.end(),[=](Sprite* pWheel){
        pWheel->stopAllActions();
    });
}