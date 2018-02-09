
#include "BarrelFreezerNode.h"
#include "PolygonHelp.h"
#include "cocostudio/CCArmature.h"
#include "AudioHelp.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocostudio;

BarrelFreezerNode::BarrelFreezerNode()
{
    onFinishedCallback = nullptr;
    onHandleCallback = nullptr;
    m_bFinished = false;
    m_nFreezeIndex = 0;
}

BarrelFreezerNode::~BarrelFreezerNode()
{
    
}
bool BarrelFreezerNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_bTouchEnable = false;
    
    Node* pMachine = CSLoader::createNode("cocos/BarrelFreezerNode.csb");
    ui::Helper::doLayout(pMachine);

    this->addChild(pMachine);
    m_pInnerNode = pMachine->getChildByName("inner_barrel");
    m_pLidNode = pMachine->getChildByName("lid");
    m_pIceBack = pMachine->getChildByName("ice_back");
    m_pIceFront = pMachine->getChildByName("ice_front");
    
    m_pBodyFront = dynamic_cast<Sprite*>(pMachine->getChildByName("barrel_on"));
    m_pCover = dynamic_cast<Sprite*>(m_pInnerNode->getChildByName("lid_down"));
    m_pCoverFront = dynamic_cast<Sprite*>(m_pInnerNode->getChildByName("lid_on"));
    m_pRing = dynamic_cast<Sprite*>(pMachine->getChildByName("barrel_ring1"));
    m_pRingUp = dynamic_cast<Sprite*>(pMachine->getChildByName("barrel_ring2"));
    m_pBatter = dynamic_cast<Sprite*>(m_pInnerNode->getChildByName("batter_down"));
    m_pBatterFront = dynamic_cast<Sprite*>(m_pInnerNode->getChildByName("batter_on"));
    ArmatureDataManager::getInstance()->addArmatureFileInfo("NewAnimation/NewAnimation0.png", "NewAnimation/NewAnimation0.plist", "NewAnimation/NewAnimation.ExportJson");
    
    m_pHandleArmature = Armature::create("NewAnimation");
    m_pHandleArmature->setPosition(m_pLidNode->getChildByName("handle")->getPosition());
    m_pLidNode->addChild(m_pHandleArmature);

    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        if (m_bTouchEnable) {
            Vec2 location = pTouch->getLocation();
            Vec2 point = CocoStudioHelper::getBoneWorlPoint(m_pHandleArmature, "Layer18");
            Rect rect = Rect(point.x, point.y-50, 100, 100);
            if (rect.containsPoint(location)) {
                ArmatureAnimation* lArmation = m_pHandleArmature->getAnimation();
                lArmation->playWithIndex(0,-1,1);
                this->schedule(schedule_selector(BarrelFreezerNode::_freeze), 2);
                if (onHandleCallback) {
                    onHandleCallback();
                }
                AudioHelp::getInstance()->playLoopEffect("icemachinerolling.mp3");
                m_pIceFront->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, 0.99),
                                                                              ScaleTo::create(0.3, 1), NULL)));
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
        
        AudioHelp::getInstance()->stopLoopEffect();
        ArmatureAnimation* lArmation = m_pHandleArmature->getAnimation();
        lArmation->stop();
        
        m_pIceFront->stopAllActions();
        this->unschedule(schedule_selector(BarrelFreezerNode::_freeze));
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void BarrelFreezerNode::setState(int state)
{
    switch (state) {
        case eStateOpen:{
            m_pInnerNode->setPosition(Vec2(m_pInnerNode->getPositionX(), 100));
            m_pRingUp->setVisible(false);
            m_pLidNode->setVisible(false);
            m_pCover->setVisible(false);
            m_pCoverFront->setVisible(false);
            m_pBatter->setOpacity(0);
            m_pBatterFront->setOpacity(0);
        }
            break;
        case eStateAddBatter:{
            m_pBatter->runAction(FadeIn::create(2));
            m_pBatterFront->runAction(FadeIn::create(2));
        }
            break;
        case eStateCovor:{
            m_pCover->setPosition(Vec2(m_pCover->getPositionX()+1000, m_pCover->getPositionY()+500));
            m_pCoverFront->setPosition(Vec2(m_pCoverFront->getPositionX()+1000, m_pCoverFront->getPositionY()+500));
            m_pCover->setVisible(true);
            m_pCoverFront->setVisible(true);
            m_pCover->runAction(Sequence::create(MoveBy::create(1, Vec2(-1000, -400)),
                                                 CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ice2.mp3");
            }),
                                                 MoveBy::create(1, Vec2(0, -100)), NULL));
            m_pCoverFront->runAction(Sequence::create(MoveBy::create(1, Vec2(-1000, -400)),
                                                 MoveBy::create(1, Vec2(0, -100)), NULL));
            
            m_pInnerNode->runAction(Sequence::create(DelayTime::create(1),
                                                     CallFunc::create([=](){
                m_pIceFront->runAction(Sequence::create(ScaleTo::create(0.5, 0.9),
                                                        ScaleTo::create(0.5, 1), NULL));
            }),
                                                     MoveBy::create(1, Vec2(0, -100)),
                                                     CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("icemachine1.mp3");
            }), NULL));
            m_pLidNode->setPosition(Vec2(m_pLidNode->getPositionX()+1000, m_pLidNode->getPositionY()+500));
            m_pLidNode->setVisible(true);
            m_pLidNode->runAction(Sequence::create(DelayTime::create(1),
                                                   MoveBy::create(1, Vec2(-1000, -500)),
                                                   CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("ice1.mp3");
                m_pRing->runAction(Sequence::create(RotateBy::create(0.5, 150),
                                                    CallFunc::create([=](){
                    m_pRing->setVisible(false);
                    m_pRingUp->setVisible(true);
                    m_bTouchEnable = true;
                }), NULL));
            }), NULL));
        }
            break;
        case eStateFinish:
        {
            m_bTouchEnable = false;
            m_pRing->setRotation(150);
            m_pRingUp->setVisible(false);
            m_pRing->setVisible(true);
            m_pBatter->setTexture("content/make/icecream/freeze/ice_cream_down.png");
            m_pBatterFront->setTexture("content/make/icecream/freeze/ice_cream_on.png");
            m_pRing->runAction(Sequence::create(RotateBy::create(0.5, -150),
                                                CallFunc::create([=](){
                
            }), NULL));
            
            m_pLidNode->runAction(Sequence::create(DelayTime::create(1),
                                                   MoveBy::create(1, Vec2(1000, 500)), NULL));
            
            m_pCover->runAction(Sequence::create(DelayTime::create(1),
                                                 MoveBy::create(1, Vec2(0, 100)),
                                                 MoveBy::create(1, Vec2(1000, 400)),
                                                 NULL));
            m_pCoverFront->runAction(Sequence::create(DelayTime::create(1),
                                                      MoveBy::create(1, Vec2(0, 100)),
                                                      MoveBy::create(1, Vec2(1000, 400)),
                                                      NULL));
            m_pInnerNode->runAction(Sequence::create(DelayTime::create(1),
                                                     CallFunc::create([=](){
                
                m_pIceFront->runAction(Sequence::create(ScaleTo::create(0.5, 0.9),
                                                        ScaleTo::create(0.5, 1), NULL));
            }),
                                                     MoveBy::create(1, Vec2(0, 100)), NULL));
        }
            break;
        default:
            break;
    }
}
void BarrelFreezerNode::_freeze(float dt)
{
    if(m_nFreezeIndex>3){
        return;
    }
    m_nFreezeIndex++;
    std::stringstream ostr;
    ostr<<"content/make/icecream/freeze/barrel_on_"<<m_nFreezeIndex<<".png";
    m_pBodyFront->setTexture(ostr.str());
    
    if(m_nFreezeIndex>=3){
        this->unschedule(schedule_selector(BarrelFreezerNode::_freeze));
        AudioHelp::getInstance()->stopLoopEffect();
        m_pIceFront->stopAllActions();
        setState(eStateFinish);
        if (onFinishedCallback) {
            onFinishedCallback();
        }
    }
}