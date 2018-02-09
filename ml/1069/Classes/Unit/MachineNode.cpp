
#include "MachineNode.h"
#include "PolygonHelp.h"
#include "cocostudio/CCArmature.h"
#include "AudioHelp.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocostudio;

MachineNode::MachineNode()
{
    onStateCallback = nullptr;
    onBeginCallback = nullptr;
    onFinishedCallback = nullptr;
    m_bTouched = false;
    m_pIce = nullptr;
    m_pIceClipping = nullptr;
    m_bFinished = false;
}

MachineNode::~MachineNode()
{
    
}
bool MachineNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_bIsOpen = false;
    m_bButtonEnable = false;
    m_bTouchEnable = false;
    m_bIsOn = false;
    
    Node* pMachine = CSLoader::createNode("cocos/MachineNode.csb");
    ui::Helper::doLayout(pMachine);
    
    Node* pBody = pMachine->getChildByName("machine0");
    Node* pBodyFront = pMachine->getChildByName("machine1");
    Node* pBox = pMachine->getChildByName("box0");
    Node* pCap = pMachine->getChildByName("cap");
    Node* pButton = pMachine->getChildByName("btn_on");

    m_pBody = Sprite::create("content/make/snowcone/crush/machine0.png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pBody->getPosition());
    
    m_pBodyFront = Sprite::create("content/make/snowcone/crush/machine1.png");
    this->addChild(m_pBodyFront,10);
    m_pBodyFront->setPosition(pBodyFront->getPosition());
    
    m_pBoxNode = Node::create();
    this->addChild(m_pBoxNode);
    m_pBoxNode->setPosition(pBox->getPosition());
    
    m_pBox = Sprite::create("content/make/snowcone/crush/box0.png");
    m_pBoxNode->addChild(m_pBox);
    
    m_pBoxFront = Sprite::create("content/make/snowcone/crush/box1.png");
    m_pBoxNode->addChild(m_pBoxFront,5);
    
    m_pCap = Sprite::create("content/make/snowcone/crush/cap.png");
    m_pCap->setPosition(pCap->getPosition());
    m_pCap->setAnchorPoint(pCap->getAnchorPoint());
    m_pCap->setRotation(156);
    this->addChild(m_pCap,11);
    
    m_pMask = Sprite::create("content/make/snowcone/crush/box_mask.png");
    m_pIceClipping = ClippingNode::create(m_pMask);
    m_pIceClipping->setAlphaThreshold(0.5);
    m_pBoxNode->addChild(m_pIceClipping,1);
    
    m_pIce = Sprite::create("content/make/snowcone/crush/box_ice.png");
    m_pIceClipping->addChild(m_pIce);
    m_pIce->setPosition(Vec2(0, -m_pIce->getContentSize().height+50));
    
    m_pButton = Button::create("content/make/snowcone/crush/btn_on.png","content/make/snowcone/crush/btn_on.png","content/make/snowcone/crush/btn_on.png");
    this->addChild(m_pButton,12);
    m_pButton->setPosition(pButton->getPosition());
    m_pButton->addTouchEventListener(CC_CALLBACK_2(MachineNode::_onButtonCallback, this));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        if (m_bTouchEnable) {
            Vec2 location = pTouch->getLocation();
            Rect rect = m_pCap->getBoundingBox();
            rect.origin = this->convertToWorldSpace(rect.origin);
//            if (!m_bIsOpen) {
//                rect.origin.x += m_pCap->getContentSize().width;
//            }
            if (rect.containsPoint(location)) {
                
                return true;
            }
            return false;
        }
        return false;
    };
    listener->onTouchEnded = [=](Touch* pTouch,Event* event){
        if (!m_bTouchEnable) {
            return;
        }
//        m_bTouchEnable = false;
        m_bIsOpen = !m_bIsOpen;
        if (m_bIsOpen) {
            if (onStateCallback) {
                onStateCallback(eStateCapOpen);
            }
            m_pCap->runAction(RotateBy::create(0.5, -156));
        }else{
            if (onStateCallback) {
                onStateCallback(eStateCapClose);
            }
            m_pCap->runAction(RotateBy::create(0.5, 156));
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MachineNode::addIce(Sprite* pIce,Vec2 wolrdPos)
{
    this->addChild(pIce);
    pIce->setPosition(this->convertToNodeSpace(wolrdPos));
    pIce->runAction(Sequence::create(JumpTo::create(0.8, Vec2(90,350), 100, 1),
                                     MoveTo::create(0.8, Vec2(55,50)), NULL));
}


void MachineNode::setIcePercent(int percent)
{
    if(percent<0){
        percent = 0;
    }
    if (percent>100) {
        percent = 100;
    }
    m_pIce->setPosition(Vec2(0, -(100-percent)*m_pIce->getContentSize().height/100+50));
    
}

void MachineNode::addUnit(int index)
{
    switch (index) {
        case 0:
        {
            
            m_pBody = Sprite::create("content/make/snowcone/crush/machine0_1.png");
            this->addChild(m_pBody);
            
            m_pBodyFront = Sprite::create("content/make/snowcone/crush/machine0_2.png");
            this->addChild(m_pBodyFront,10);
            
        }
            break;
        case 1:
        {
            
            m_pBowl = Sprite::create("content/make/snowcone/crush/machine1_1.png");
            this->addChild(m_pBowl);
            
            m_pBowlFront = Sprite::create("content/make/snowcone/crush/machine1_2.png");
            this->addChild(m_pBowlFront);
            
            m_pBowl->setPosition(Vec2(0, 300+30));
            m_pBowlFront->setPosition(m_pBowl->getPosition());
            
//            auto body = PhysicsBody::create();
//            PolygonHelp::getInstance()->addShapesWithFile("machine_bowl", body,-m_pBowl->getContentSize().width/2,-m_pBowl->getContentSize().height/2);
//            body->setDynamic(false);
//            m_pBowl->setPhysicsBody(body);
            m_pBowlFront->setLocalZOrder(10);
        }
            break;
        case 2:
        {
            
            m_pCone = Sprite::create("content/make/snowcone/crush/cone1_0.png");
            this->addChild(m_pCone);
            
            m_pConeFront = Sprite::create("content/make/snowcone/crush/cone1_1.png");
            this->addChild(m_pConeFront,5);
            m_pCone->setPosition(Vec2(0, -20));
            m_pConeFront->setPosition(Vec2(0, -55));
        }
            break;
        case 3:
        {
            m_pStick = Sprite::create("content/make/snowcone/crush/machine2_2.png");
            this->addChild(m_pStick);
            m_pStick->setPosition(Vec2(0, 480+30));
            
//            auto body = PhysicsBody::create();
//            PolygonHelp::getInstance()->addShapesWithFile("machine2_2", body,-m_pStick->getContentSize().width/2,-m_pStick->getContentSize().height/2);
//            body->setDynamic(false);
//            m_pStick->setPhysicsBody(body);
            m_pStick->setLocalZOrder(5);
            
        }
            break;
        case 4:
        {
            ArmatureDataManager::getInstance()->addArmatureFileInfo("cocos/handle/HandleAnimation0.png", "cocos/handle/HandleAnimation0.plist", "cocos/handle/HandleAnimation.ExportJson");
        
            m_pCoverArmature = Armature::create("HandleAnimation");
            m_pCoverArmature->setPosition(Vec2(0, 530+30));
            m_pCoverArmature->runAction(MoveBy::create(1, Vec2(0, -150)));
            this->addChild(m_pCoverArmature);
            m_pCoverArmature->setLocalZOrder(11);
            

            
            m_pCover = Sprite::create("content/make/snowcone/crush/machine2_1.png");
            this->addChild(m_pCover);
            m_pCover->setPosition(Vec2(0, 530+30));
            m_pCover->runAction(MoveBy::create(1, Vec2(0, -150)));
//            auto body = PhysicsBody::create();
//            PolygonHelp::getInstance()->addShapesWithFile("machine2_1", body,-m_pCover->getContentSize().width/2,-m_pCover->getContentSize().height/2);
//            body->setDynamic(false);
//            m_pCover->setPhysicsBody(body);
            m_pCover->setLocalZOrder(10);
        }
            break;
        case 5:
        {
            if(!m_pIceClipping){
                m_pMask = Sprite::create("content/make/snowcone/crush/ice3.png");
                m_pIceClipping = ClippingNode::create(m_pMask);
                m_pIceClipping->setAlphaThreshold(0.5);
                m_pIceClipping->setPosition(Vec2(0, 90));
                this->addChild(m_pIceClipping,1);
            }
            if (!m_pIce) {
                m_pIce = Sprite::create("content/make/snowcone/crush/ice3.png");
                m_pIceClipping->addChild(m_pIce);
                m_pIce->setPosition(Vec2(0, -m_pIce->getContentSize().height));
//                m_pIce->setOpacity(0);
            }
        }
            break;
            
        default:
            break;
    }
}


void MachineNode::updateIce(float)
{
    if (m_pIce) {
        float posY = m_pIce->getPositionY();
        posY+=2;
        if (posY>=0) {
            m_pIce->setPosition(Vec2(0, 0));
            if (!m_bFinished) {
                m_bFinished = true;
                if(onFinishedCallback){
                    onFinishedCallback();
                }
            }
            _stopWork();
        }else{
            m_pIce->setPosition(Vec2(0, posY));
        }
    }
}


void MachineNode::getIce(Sprite* pSpoon,std::function<void()> callback)
{
    m_pBoxNode->addChild(pSpoon);
    pSpoon->setPosition(Vec2(m_pBox->getContentSize().width*0.3, m_pBox->getContentSize().height*0.2));
    pSpoon->runAction(Sequence::create(RotateBy::create(0.3, 20),
                                       RotateBy::create(0.3, -20),
                                       CallFunc::create([=](){
        if (callback) {
            callback();
        }
    }), NULL));
}

void MachineNode::separateBox()
{
    auto action = Sequence::create(DelayTime::create(1),
                                   MoveBy::create(0.5, Vec2(1000, 100)), NULL);
    Vector<Node*> pChildren = this->getChildren();
    for_each(pChildren.begin(),pChildren.end(),[=](Node* pNode){
        if (pNode!= m_pBoxNode) {
            pNode->runAction(action->clone());
        }
    });
    
    m_pBoxNode->runAction(Sequence::create(DelayTime::create(0.5),
                                           MoveBy::create(0.5, Vec2(0, -150)), NULL));
}

void MachineNode::_onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if(!m_bButtonEnable){
        return;
    }
    if (type==Widget::TouchEventType::ENDED) {
        m_bButtonEnable = false;
//        m_pButton->setTouchEnabled(false);
        m_bIsOn = !m_bIsOn;
        if (m_bIsOn) {
            if (onStateCallback) {
                onStateCallback(eStateTurnOn);
            }
            m_pButton->loadTextures("content/make/snowcone/crush/btn_on_h.png", "content/make/snowcone/crush/btn_on_h.png","content/make/snowcone/crush/btn_on_h.png");
            _doWork();
        }else{
            if (onStateCallback) {
                onStateCallback(eStateTurnOff);
            }
            _stopWork();
            m_pButton->loadTextures("content/make/snowcone/crush/btn_on.png", "content/make/snowcone/crush/btn_on.png","content/make/snowcone/crush/btn_on.png");
        }
    }
}

void MachineNode::_doWork()
{
    updateIce(0.1);
    AudioHelp::getInstance()->playLoopEffect("grind_ice.mp3");
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                           MoveBy::create(0.1, Vec2(-4, -4)),
                                                           MoveBy::create(0.1, Vec2(2, 2)),
                                                           NULL)));
    this->schedule(schedule_selector(MachineNode::updateIce), 0.1);
    ParticleSystemQuad* pParticle = (ParticleSystemQuad*)getChildByName("ice");
    if (!pParticle) {
        pParticle = ParticleSystemQuad::create("particle/ice.plist");
        pParticle->setPosition(Vec2(0, m_pBoxNode->getPositionY()+m_pBox->getContentSize().height/2+15));
        this->addChild(pParticle,2);
        pParticle->setName("ice");
    }else{
        pParticle->resetSystem();
    }
}

void MachineNode::_stopWork()
{
    this->stopAllActions();
    ParticleSystemQuad* pParticle = (ParticleSystemQuad*)getChildByName("ice");
    if (pParticle) {
        pParticle->stopSystem();
    }
    this->unschedule(schedule_selector(MachineNode::updateIce));
    AudioHelp::getInstance()->stopLoopEffect();
    if (onStateCallback) {
        onStateCallback(eStateFinish);
    }
    m_pButton->loadTextures("content/make/snowcone/crush/btn_on.png", "content/make/snowcone/crush/btn_on.png","content/make/snowcone/crush/btn_on.png");
}