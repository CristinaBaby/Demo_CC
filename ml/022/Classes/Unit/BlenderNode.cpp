
#include "BlenderNode.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"
#include "CircleMove.h"
#include "AnimationHelp.h"
#include "PolygonHelp.h"
#include "AudioHelp.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

enum{
    eZOrderWaterBack,
    eZOrderWaterIce,
    eZOrderWaterFront,
};
BlenderNode::BlenderNode()
{
    onBlenderWorking = nullptr;
    onBlenderStopWorking = nullptr;
    onBlenderSwitchTurnOff = nullptr;
    m_bReadyWork = false;
    m_bFinishCrush = false;
    m_nCount = 0;
    m_nStep = 0;
}

BlenderNode::~BlenderNode()
{
}

bool BlenderNode::init(int type)
{
    if ( !Layer::init() )
    {
        return false;
    }
    m_nType = type;
    Node* pCocosNode = CSLoader::createNode("BlenderNode"+std::to_string(type)+".csb");
    
    m_pBody = Sprite::create("content/make/blender/blender"+std::to_string(type+1)+".png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pCocosNode->getChildByName("blender")->getPosition());
    
    m_pCup = BlenderCup::create();
    this->addChild(m_pCup);
    m_pCup->setPosition(pCocosNode->getChildByName("cup")->getPosition());
    m_pCup->onFinishCrushIce = [=](){
        m_bReadyWork = false;
        if(onBlenderStopWorking){
            onBlenderStopWorking();
        }
        m_bFinishCrush = true;
        if (m_nType==1) {
            m_bReadyWork = true;
            m_pSwitch->loadTextures("content/make/blender/blender2_on.png", "content/make/blender/blender2_off.png","content/make/blender/blender2_off.png");
            m_pSwitch->setTouchEnabled(true);
            m_pSwitch->setEnabled(true);
        }
    };
    
    if (type==0) {
        m_pHandle = Sprite::create("content/make/blender/blender1_1.png");
        this->addChild(m_pHandle);
        Node* pHandle = pCocosNode->getChildByName("blender1_1");
        m_pHandle->setAnchorPoint(pHandle->getAnchorPoint());
        m_pHandle->setPosition(pHandle->getPosition());
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        listener->onTouchBegan = CC_CALLBACK_2(BlenderNode::TouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(BlenderNode::TouchMove, this);
        listener->onTouchEnded = CC_CALLBACK_2(BlenderNode::TouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }else if (type==1) {
        m_pSwitch = Button::create("content/make/blender/blender2_off.png","content/make/blender/blender2_off.png","content/make/blender/blender2_on.png");
        this->addChild(m_pSwitch);
        m_pSwitch->setPosition(pCocosNode->getChildByName("blender2_switch")->getPosition());
        m_pSwitch->setTouchEnabled(false);
        m_pSwitch->setEnabled(true);
        m_pSwitch->addTouchEventListener(CC_CALLBACK_2(BlenderNode::onButtonCallback, this));
    }
    return true;
}

bool BlenderNode::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_bReadyWork) {
        return false;
    }
    Point location = pTouch->getLocation();
    Point point = this->convertToWorldSpace(m_pHandle->getPosition());
    
    Size handleSize = m_pHandle->getContentSize();
    Rect rect = Rect(-handleSize.width*getScaleX()*0.5, -handleSize.height*getScaleY()*0.5, handleSize.width*getScaleX(), handleSize.height*getScaleY());
    rect = m_pHandle->getBoundingBox();
    Rect boundBox = RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
    boundBox.origin = this->getParent()->convertToWorldSpace(boundBox.origin);
    
    if (!boundBox.containsPoint(location)) {
        return false;
    }
    if (onBlenderWorking) {
        onBlenderWorking();
    }
    if (m_nStep==0) {
        m_pCup->crushIceManual(m_nStep);
        m_pCup->resumeCrushIceManual(m_nStep);
    }else{
        m_pCup->resumeCrushIceManual(m_nStep);
    }
    log("contain");
    return true;
}
void BlenderNode::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (!m_bReadyWork) {
        return;
    }
    Vec2 pos = pTouch->getPreviousLocation()-pTouch->getLocation();
    m_pHandle->setRotation(m_pHandle->getRotation()+MAX(abs(pos.x), abs(pos.y)));
    m_nCount++;
    if (m_nCount%120==0) {
        if (m_nStep==0) {
        }else{
            m_pCup->resumeCrushIceManual(m_nStep);
        }
        m_nStep++;
    }
    
}
void BlenderNode::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pCup->pauceCrushIceManual(m_nStep);
}

void BlenderNode::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (!m_bReadyWork) {
        return;
    }
    if(type==Widget::TouchEventType::ENDED){
        if (!m_bFinishCrush) {
            if (onBlenderWorking) {
                onBlenderWorking();
            }
            AudioHelp::getInstance()->playEffect("blender_button.mp3");
            m_pSwitch->setTouchEnabled(false);
            m_pSwitch->setEnabled(false);
            m_pCup->crushIce();
            m_bReadyWork = false;
        }else{
            m_pSwitch->setTouchEnabled(false);
            m_pSwitch->setEnabled(false);
            m_pCup->pauceCrushIceManual(5);
            if (onBlenderSwitchTurnOff) {
                onBlenderSwitchTurnOff();
            }
        }
    }
}