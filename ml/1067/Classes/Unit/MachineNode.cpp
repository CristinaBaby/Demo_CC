
#include "MachineNode.h"
#include "PolygonHelp.h"
#include "cocostudio/CCArmature.h"
#include "AudioHelp.h"

using namespace cocostudio;

MachineNode::MachineNode()
{
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
    
    m_pBody = Sprite::create("content/make/ice/machine/machine0_1.png");
    this->addChild(m_pBody);
    
    m_pBodyFront = Sprite::create("content/make/ice/machine/machine0_2.png");
    this->addChild(m_pBodyFront,10);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        if (m_bTouchEnable) {
            Vec2 location = pTouch->getLocation();
            Rect rect = m_pCover->getBoundingBox();
            rect.size.width +=80;
            rect.origin.x -= 40;
            rect.origin = this->convertToWorldSpace(rect.origin);
            rect.origin.y+=m_pCover->getContentSize().height/2;
            if (rect.containsPoint(location)) {
                ArmatureAnimation* lArmation = m_pCoverArmature->getAnimation();
                if (!m_bTouched) {
                    lArmation->playWithIndex(0,-1,1);
                    m_bTouched = true;
                    this->addUnit(5);
                    if(onBeginCallback){
                        onBeginCallback();
                    }

                }else{
                    lArmation->resume();
                }
                updateIce(0.1);
                this->schedule(schedule_selector(MachineNode::updateIce), 0.1);
                ParticleSystemQuad* pParticle = (ParticleSystemQuad*)getChildByName("ice");
                if (!pParticle) {
                    pParticle = ParticleSystemQuad::create("particle/ice.plist");
                    pParticle->setPosition(Vec2(0, 210));
                    this->addChild(pParticle,2);
                    pParticle->setName("ice");
                }else{
                    pParticle->resetSystem();
                }
                AudioHelp::getInstance()->playLoopEffect("machine_ice fall.mp3");
            }
        }
        return true;
    };
    listener->onTouchEnded = [=](Touch* pTouch,Event* event){
        if (!m_bTouchEnable) {
            return;
        }
        
        ArmatureAnimation* lArmation = m_pCoverArmature->getAnimation();
        lArmation->pause();
        ParticleSystemQuad* pParticle = (ParticleSystemQuad*)getChildByName("ice");
        if (pParticle) {
            pParticle->stopSystem();
        }
        this->unschedule(schedule_selector(MachineNode::updateIce));
        AudioHelp::getInstance()->stopLoopEffect();
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MachineNode::addUnit(int index)
{
    switch (index) {
        case 0:
        {
            
            m_pBody = Sprite::create("content/make/ice/machine/machine0_1.png");
            this->addChild(m_pBody);
            
            m_pBodyFront = Sprite::create("content/make/ice/machine/machine0_2.png");
            this->addChild(m_pBodyFront,10);
            
        }
            break;
        case 1:
        {
            
            m_pBowl = Sprite::create("content/make/ice/machine/machine1_1.png");
            this->addChild(m_pBowl);
            
            m_pBowlFront = Sprite::create("content/make/ice/machine/machine1_2.png");
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
            
            m_pCone = Sprite::create("content/make/ice/machine/cone1_0.png");
            this->addChild(m_pCone);
            
            m_pConeFront = Sprite::create("content/make/ice/machine/cone1_1.png");
            this->addChild(m_pConeFront,5);
            m_pCone->setPosition(Vec2(0, -20));
            m_pConeFront->setPosition(Vec2(0, -55));
        }
            break;
        case 3:
        {
            m_pStick = Sprite::create("content/make/ice/machine/machine2_2.png");
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
            

            
            m_pCover = Sprite::create("content/make/ice/machine/machine2_1.png");
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
                m_pMask = Sprite::create("content/make/ice/machine/ice3.png");
                m_pIceClipping = ClippingNode::create(m_pMask);
                m_pIceClipping->setAlphaThreshold(0.5);
                m_pIceClipping->setPosition(Vec2(0, 90));
                this->addChild(m_pIceClipping,1);
            }
            if (!m_pIce) {
                m_pIce = Sprite::create("content/make/ice/machine/ice3.png");
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
//        int opacity = m_pIce->getOpacity()+2;
//        if (opacity>=255) {
//            m_pIce->setOpacity(255);
//            this->unschedule(schedule_selector(MachineNode::updateIce));
//            if(onFinishedCallback){
//                onFinishedCallback();
//            }
//        }else{
//            m_pIce->setOpacity(opacity);
//        }
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
            this->unschedule(schedule_selector(MachineNode::updateIce));
        }else{
            m_pIce->setPosition(Vec2(0, posY));
        }
    }
}