
#include "OvenNode.h"
#include "AudioHelp.h"
#include "AnimationHelp.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
OvenNode::OvenNode()
{
    onDoorCallback = nullptr;
    onBakeCallback = nullptr;
    m_bBaked = false;
    m_bIsBaking = false;
    m_bControlable = false;
    m_bTempReady = false;
    m_bTimeReady = false;
}

OvenNode::~OvenNode()
{
    
}
bool OvenNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    Node* pOVenNode = CSLoader::createNode("res/OvenNode.csb");
    ui::Helper::doLayout(pOVenNode);
    
    Node* pBody = pOVenNode->getChildByName("oven0");
    Node* pBodyDoor = pOVenNode->getChildByName("door1");
//    Node* BodyDoorOpen = pOVenNode->getChildByName("door4");
    Node* pFrame = pOVenNode->getChildByName("frame");
    Node* pSwitch = pFrame->getChildByName("k_temp");
    Node* pSwitchTime = pFrame->getChildByName("k_time");
    
    m_pBody = Sprite::create("content/make/common/bake/oven2.png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pBody->getPosition());
    
    
    m_pBodyDoor = Sprite::create("content/make/common/bake/door1.png");
    this->addChild(m_pBodyDoor,5);
    m_pBodyDoor->setAnchorPoint(pBodyDoor->getAnchorPoint());
    m_pBodyDoor->setPosition(pBodyDoor->getPosition());
    
//    m_pBodyDoorOpen = Sprite::create("content/make/common/bake/door4.png");
//    this->addChild(m_pBodyDoorOpen,5);
//    m_pBodyDoorOpen->setAnchorPoint(BodyDoorOpen->getAnchorPoint());
//    m_pBodyDoorOpen->setPosition(BodyDoorOpen->getPosition());
    
//    m_pSwitch = Button::create("content/make/common/bake/oven_button1.png","content/make/common/bake/oven_button1.png");
//    this->addChild(m_pSwitch);
//    m_pSwitch->setAnchorPoint(pSwitch->getAnchorPoint());
//    m_pSwitch->setPosition(pSwitch->getPosition());
//    m_pSwitch->setTouchEnabled(false);
//    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(OvenNode::_onButtonCallbak, this));
    
    m_pControlFrame = Node::create();
    this->addChild(m_pControlFrame,10);
    m_pControlFrame->setScale(pFrame->getScale());
    m_pControlFrame->setPosition(pFrame->getPosition());
    m_FramePos = m_pControlFrame->getPosition();
    m_fScale = m_pControlFrame->getScale();
    
    m_pFrame = Sprite::create("content/make/common/bake/frame.png");
    m_pFrame->setVisible(false);
    m_pControlFrame->addChild(m_pFrame);
//    m_pFrame->setScale(1.2, 1);
//    温度开关
    {
        Sprite* pMask = Sprite::create("content/make/common/bake/mask.png");
        m_pTempNode = Sprite::create("content/make/common/bake/k_temp350.png");
        m_pControlFrame->addChild(m_pTempNode,10);
        m_pTempNode->setScale(pSwitch->getScale());
        m_pTempNode->setPosition(pSwitch->getPosition());
        
        m_pTempClippingNode = ClippingNode::create(pMask);
        m_pTempClippingNode->setAlphaThreshold(0.5);
        m_pTempClippingNode->setPosition(m_pTempNode->getAnchorPointInPoints());
        m_pTempNode->addChild(m_pTempClippingNode);
        
        m_pTempColor = LayerGradient::create(Color4B(156, 255, 28, 255), Color4B(255, 0, 0, 255), Vec2(1, 0));
        m_pTempColor->setContentSize(pMask->getContentSize());
        m_pTempColor->setAnchorPoint(Vec2(1, 1));
        m_pTempColor->setCompressedInterpolation(true);
        m_pTempColor->setPosition(Vec2(-pMask->getContentSize().width*0.5, -pMask->getContentSize().height*0.5));
        
        m_pTempClippingNode->addChild(m_pTempColor);
        
        m_pTempMask = LayerColor::create(Color4B(140, 140, 140, 255));
        m_pTempMask->setContentSize(Size(pMask->getContentSize().width*1.4, pMask->getContentSize().height*0.5+70));
        m_pTempMask->setAnchorPoint(Vec2(0.5, 0));
        m_pTempMask->setPosition(Vec2(-pMask->getContentSize().width*0.7, -70));
        m_pTempClippingNode->addChild(m_pTempMask);
        
        
        m_pTempPoint = Sprite::create("content/make/common/bake/knob.png");
        m_pTempNode->addChild(m_pTempPoint);
        m_pTempPoint->setPosition(m_pTempNode->getAnchorPointInPoints()+Vec2(0, -9));
        m_pTempPoint->setRotation(-108);
        m_nTempRotation = m_pTempPoint->getRotation();
    }
    
//    时间开关
    {
        m_pTimeNode = Sprite::create("content/make/common/bake/k_time30.png");
        m_pControlFrame->addChild(m_pTimeNode,10);
        m_pTimeNode->setScale(pSwitchTime->getScale());
        m_pTimeNode->setPosition(pSwitchTime->getPosition());
        
        m_pTimePoint = Sprite::create("content/make/common/bake/knob.png");
        m_pTimeNode->addChild(m_pTimePoint);
        m_pTimePoint->setPosition(m_pTimeNode->getAnchorPointInPoints()+Vec2(0, -9));
        m_pTimePoint->setRotation(-102);
        m_nTimeRotation = m_pTimePoint->getRotation();
    }
    
//    m_pLightClipping = ClippingNode::create(Sprite::create("content/make/common/bake/oven_mask.png"));
//    m_pLightClipping->setAlphaThreshold(0.5);
//    this->addChild(m_pLightClipping,4);
    
    m_bIsOpen = false;
    m_bTouchEnable = false;
//    m_pBodyDoorOpen->setVisible(false);
    
    m_nTempPointer = 180;
    m_nTimePointer = 175;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(OvenNode::ccTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(OvenNode::ccTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(OvenNode::ccTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void OvenNode::setOvenSettings(int temp,int duration)
{
    if (temp == 250) {
        m_nTempPointer = 105;
        m_pTempNode->removeAllChildren();
        m_pTempNode->setTexture("content/make/common/bake/k_temp250.png");
        Sprite* pMask = Sprite::create("content/make/common/bake/mask250.png");
        
        m_pTempClippingNode = ClippingNode::create(pMask);
        m_pTempClippingNode->setAlphaThreshold(0.5);
        m_pTempClippingNode->setPosition(m_pTempNode->getAnchorPointInPoints());
        m_pTempNode->addChild(m_pTempClippingNode);
        
        m_pTempColor = LayerGradient::create(Color4B(156, 255, 28, 255), Color4B(255, 0, 0, 255), Vec2(1, 0.8));
        m_pTempColor->setContentSize(pMask->getContentSize());
        m_pTempColor->setAnchorPoint(Vec2(1, 1));
        m_pTempColor->setCompressedInterpolation(true);
        m_pTempColor->setPosition(Vec2(-pMask->getContentSize().width*0.5, -pMask->getContentSize().height*0.5));
        
        m_pTempClippingNode->addChild(m_pTempColor);
        
        m_pTempMask = LayerColor::create(Color4B(140, 140, 140, 255));
        m_pTempMask->setContentSize(Size(pMask->getContentSize().width*1.4, pMask->getContentSize().height*0.5+70));
        m_pTempMask->setAnchorPoint(Vec2(0.5, 0));
        m_pTempMask->setPosition(Vec2(-pMask->getContentSize().width*0.7, -70));
        m_pTempClippingNode->addChild(m_pTempMask);
        
        
        m_pTempPoint = Sprite::create("content/make/common/bake/knob.png");
        m_pTempNode->addChild(m_pTempPoint);
        m_pTempPoint->setPosition(m_pTempNode->getAnchorPointInPoints()+Vec2(0, -9));
        m_pTempPoint->setRotation(-108);
        m_nTempRotation = m_pTempPoint->getRotation();
        
    }else{
        m_nTempPointer = 175;
    }
    if (duration == 8) {
        m_nTimePointer = 170;
    }else{
//        m_nTimePointer = 175;
        m_nTimePointer = 140;
    }
}

void OvenNode::openDoor()
{
    m_bTouchEnable = false;
    AudioHelp::getInstance()->playEffect("open_close_door.mp3");
    m_bIsOpen = true;
    m_pBodyDoor->setRotation3D(Vec3(0, 0, 0));
    m_pBodyDoor->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/common/bake/door", 1, 4,false),
                                            DelayTime::create(0.5),
                                            CallFunc::create([=](){
        if (onDoorCallback) {
            onDoorCallback(this);
        }
        
    }), NULL));
}

void OvenNode::closeDoor()
{
    m_bTouchEnable = false;
    AudioHelp::getInstance()->playEffect("open_close_door.mp3");
    m_bIsOpen = false;
    m_pBodyDoor->setVisible(true);
    m_pBodyDoor->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/common/bake/door", 4, 1,false),
                                            DelayTime::create(0.5),
                                            CallFunc::create([=](){
        if (onDoorCallback) {
            onDoorCallback(this);
        }
        
    }), NULL));
}

void OvenNode::doBake()
{
    AudioHelp::getInstance()->playLoopEffect("baking.mp3");
    m_pBody->setTexture("content/make/common/bake/oven1.png");
    if (onBakeCallback) {
        onBakeCallback(this,0);
    }
    
    m_pBodyDoor->setTexture("content/make/common/bake/door0.png");
//    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/ovenlight.plist");
//    m_pLightClipping->addChild(pParticle);
//    pParticle->setPosition(Vec2(0, 200));
//    pParticle->setName("light");
    
//    m_pTempMask->runAction(RotateBy::create(8, 180));
    m_pTimePoint->runAction(Sequence::create(RotateBy::create(15, -m_nTimePointer),
                                             CallFunc::create([=](){
        stopBake();
    }), NULL));
}

void OvenNode::stopBake()
{
//    ParticleSystemQuad* pLight= (ParticleSystemQuad*)m_pLightClipping->getChildByName("light");
//    if (pLight) {
//        pLight->stopSystem();
//        pLight->removeFromParent();
//    }
    
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_great.mp3");
    this->stopAllActions();
    m_pBody->setTexture("content/make/common/bake/oven2.png");
    m_pBodyDoor->setTexture("content/make/common/bake/door1.png");
    m_bBaked = true;
    m_bIsBaking = false;
    if (onBakeCallback) {
        onBakeCallback(this,1);
    }
}

bool OvenNode::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
    return m_bControlable||m_bTouchEnable;
}
void OvenNode::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (m_bTouchEnable) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    
    {
        Rect rect = m_pTempColor->getBoundingBox();
        rect.origin = m_pTempColor->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(pos) && !m_bTempReady) {
            AudioHelp::getInstance()->playLoopEffect("clock_windup.mp3");
            int rotation = m_pTempPoint->getRotation();
            if (rotation+4-m_nTempRotation<=m_nTempPointer) {
//                m_pTempMask->setRotation(rotation+4);
                m_pTempPoint->setRotation(m_pTempPoint->getRotation()+4);
            }else{
                m_bTempReady = true;
                if (m_nTempPointer==105) {
                    m_pTempMask->runAction(RotateBy::create(1, m_nTempPointer-25));
                }else{
                    
                    m_pTempMask->runAction(RotateBy::create(1, m_nTempPointer-45));
                }
                Node* pNode = m_pControlFrame->getChildByName("guide_temp");
                if (pNode) {
                    pNode->removeFromParent();
                }
                AudioHelp::getInstance()->playEffect("ding.mp3");
                AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
            }
        }
    }
    {
        Rect rect = m_pTimeNode->getBoundingBox();
        rect.origin = m_pTimeNode->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(pos) && !m_bTimeReady) {
            AudioHelp::getInstance()->playLoopEffect("clock_windup.mp3");
            int rotation = m_pTimePoint->getRotation();
            if (rotation+4-m_nTimeRotation<=m_nTimePointer) {
                m_pTimePoint->setRotation(rotation+4);
            }else{
                AudioHelp::getInstance()->playEffect("ding.mp3");
                AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
                m_bTimeReady = true;
                Node* pNode = m_pControlFrame->getChildByName("guide_time");
                if (pNode) {
                    pNode->removeFromParent();
                }
            }
        }
    }
}
void OvenNode::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
    AudioHelp::getInstance()->stopLoopEffect();
    if (m_bControlable) {
        if (m_bTimeReady&&m_bTempReady) {
            m_bControlable = false;
            AudioHelp::getInstance()->playEffect(arc4random()%100>0?"vo_sweet.mp3":"vo_fabulous.mp3");
            _hideControlFrame();
            this->runAction(Sequence::create(DelayTime::create(0.5),
                                             CallFunc::create([=](){
                this->doBake();
            }), NULL));
        }
        return;
    }
    
    if (!m_bTouchEnable) {
        return;
    }
    Vec2 location = pTouch->getLocation();
    Rect rect = this->getDoorTouchWorldRect();
    if (m_bIsOpen) {
        if (rect.containsPoint(location)) {
            closeDoor();
        }
    }else{
        if (rect.containsPoint(location)) {
            openDoor();
        }
    }
//    this->runAction(Sequence::create(DelayTime::create(1),
//                                     CallFunc::create([=]() {
//        if (onDoorCallback) {
//            onDoorCallback(this);
//        }
//    }), NULL));
}
void OvenNode::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}
void OvenNode::_onButtonCallbak(Ref* ref,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        m_pSwitch->setRotation(90);
        this->setSwitchEnable(false);
        this->setTouchEnable(false);
        doBake();
        m_bIsBaking = true;
        m_pSwitch->runAction(Sequence::create(RotateBy::create(5, -90),
                                              CallFunc::create([=](){
            stopBake();
        }), NULL));
    }
}
void OvenNode::_showControlFrame()
{
    AudioHelp::getInstance()->playEffect("window_popup.mp3");
    auto action = Sequence::create(DelayTime::create(0.5),
                                   Spawn::create(ScaleTo::create(1, 1),
                                                 MoveBy::create(1, Vec2(-m_pControlFrame->getPositionX(), -m_pControlFrame->getPositionY())),  NULL),
                                   NULL);
    m_pControlFrame->runAction(action);
    m_pFrame->setVisible(true);
    if(m_nTempPointer == 105){
        AudioHelp::getInstance()->playEffect("vo_bake_temprature_pizza.mp3");
    }else{
        AudioHelp::getInstance()->playEffect("vo_bake_temprature.mp3");
    }
    this->runAction(Sequence::create(DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_bake_8minuts.mp3");
    }), NULL));
    
    Sprite* pGuideTemp = Sprite::create("content/common/tip/arrow_arc.png");
    m_pControlFrame->addChild(pGuideTemp,100);
    pGuideTemp->setPosition(m_pTempNode->getPosition()+Vec2(0, 150));
    pGuideTemp->setOpacity(0);
    pGuideTemp->setName("guide_temp");
    pGuideTemp->setScaleX(-1);
    pGuideTemp->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               EaseSineOut::create(FadeTo::create(1, 200)),
                                                               FadeTo::create(1, 255),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    pGuideTemp->setVisible(false);
                                                                                }),
                                                               DelayTime::create(3),
                                                               CallFunc::create([=]()
                                                                                {
                                                                                    pGuideTemp->setVisible(true);
                                                                                }),
                                                               NULL)));
    
    Sprite* pGuideTime = Sprite::create("content/common/tip/arrow_arc.png");
    m_pControlFrame->addChild(pGuideTime,100);
    pGuideTime->setPosition(m_pTimeNode->getPosition()+Vec2(0, 150));
    pGuideTime->setOpacity(0);
    pGuideTime->setName("guide_time");
    pGuideTime->setScaleX(-1);
    pGuideTime->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1),
                                                                 EaseSineOut::create(FadeTo::create(1, 200)),
                                                                 FadeTo::create(1, 255),
                                                                 EaseSineOut::create(FadeTo::create(1, 200)),
                                                                 FadeTo::create(1, 255),
                                                                 CallFunc::create([=]()
                                                                                  {
                                                                                      pGuideTime->setVisible(false);
                                                                                  }),
                                                                 DelayTime::create(3),
                                                                 CallFunc::create([=]()
                                                                                  {
                                                                                      pGuideTime->setVisible(true);
                                                                                  }),
                                                                 NULL)));
}

void OvenNode::_hideControlFrame()
{
    auto action = Sequence::create(DelayTime::create(0.5),
                                   Spawn::create(ScaleTo::create(1, m_fScale),
                                                 MoveTo::create(1, m_FramePos), NULL),
                                   CallFunc::create([=](){
        m_pFrame->setVisible(false);
    }), NULL);
    m_pControlFrame->runAction(action);
}