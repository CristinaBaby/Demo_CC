
#include "BlenderCup.h"
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
BlenderCup::BlenderCup()
{
    m_fHeightDelta = 5;
    m_nFlavorIndex = -1;
    m_nStep = -1;
    m_pLid = nullptr;
    m_pShadow = nullptr;
    m_pWaterDrop = nullptr;
    m_pWater = nullptr;
    onFinishCrushIce = nullptr;
    onFinishPour = nullptr;
    onBeginCrushIce = nullptr;
    onLidOpen = nullptr;
    m_bTouchEnable = false;
}

BlenderCup::~BlenderCup()
{
}
bool BlenderCup::init(bool physics)
{
    if ( !Layer::init() )
    {
        return false;
    }
    m_bPhysics = physics;
    
    Node* pCocosNode = CSLoader::createNode("BlenderCupNode.csb");
    
//    m_pShadow = Sprite::create("content/make/common/blender/blender_cup_shadow.png");
//    this->addChild(m_pShadow);
//    m_pShadow->setPosition(pCocosNode->getChildByName("blender_cup_shadow")->getPosition());
    
    m_pCupBack = Sprite::create("content/make/common/blender/blender_cup_down.png");
    this->addChild(m_pCupBack);
    m_pCupBack->setPosition(pCocosNode->getChildByName("blender_cup_down")->getPosition());
    
    m_pCupFront = Sprite::create("content/make/common/blender/blender_cup_on.png");
    m_pCupFront->setLocalZOrder(10);
    this->addChild(m_pCupFront);
    m_pCupFront->setPosition(pCocosNode->getChildByName("blender_cup_on")->getPosition());
    
    if(m_bPhysics){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        
        PolygonHelp::getInstance()->addShapesWithFile("blender_cup", body,-m_pCupFront->getContentSize().width/2,-m_pCupFront->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pCupFront->setPhysicsBody(body);
    }
    
    Sprite*pNode = Sprite::create("content/make/common/blender/blender_cup_mask.png");
    
    m_pClippingNode = ClippingNode::create(pNode);
    this->addChild(m_pClippingNode);
    m_pClippingNode->setAlphaThreshold(0.5);
    m_pClippingNode->setPosition(pCocosNode->getChildByName("blender_cup_mask")->getPosition());
    
//    m_pWaterDrop = Sprite::create("content/make/common/blender/blender_water0.png");
//    m_pClippingNode->addChild(m_pWaterDrop,11);
//    m_pWaterDrop->setPosition(pCocosNode->getChildByName("blender_cup_mask")->getChildByName("juice")->getPosition());
    
    m_pLid = Sprite::create("content/make/common/blender/blender1_lid.png");
    this->addChild(m_pLid,9);
    Node* pCocosLid = pCocosNode->getChildByName("blender_lid");
    m_pLid->setScale(pCocosLid->getScaleX(), pNode->getScaleY());
    m_pLid->setPosition(pCocosLid->getPosition());
    
    if(m_bPhysics){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        PolygonHelp::getInstance()->addShapesWithFile("blender_lid", body,-m_pLid->getContentSize().width/2,-m_pLid->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        
        m_pLid->setPhysicsBody(body);
    }
    
    m_pIceNode = Node::create();
    m_pClippingNode->addChild(m_pIceNode,eZOrderWaterIce);
    m_pIceNode->setPosition(Vec2::ZERO);
    
    this->setContentSize(m_pCupFront->getContentSize());
    
    m_pWaterGrid = NodeGrid::create();
    m_pClippingNode->addChild(m_pWaterGrid,eZOrderWaterBack);
    m_pWaterGrid->setPosition(Vec2::ZERO);
    
    m_pWaterFrontGrid = NodeGrid::create();
    m_pClippingNode->addChild(m_pWaterFrontGrid,eZOrderWaterFront);
    m_pWaterFrontGrid->setPosition(Vec2::ZERO);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return m_bTouchEnable;
    };
    listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
        
        if(this->getLidShown()){
            Rect rect = getCupWorldRect();
            Vec2 pos = pTouch->getLocation();
            if(rect.containsPoint(pos)){
                this->showLid(false,true);
                this->onLidOpen();
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void BlenderCup::setBodyData()
{
    m_bPhysics = true;
    if(m_bPhysics){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        
        PolygonHelp::getInstance()->addShapesWithFile("blender_cup", body,-m_pCupFront->getContentSize().width/2,-m_pCupFront->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        m_pCupFront->setPhysicsBody(body);
    }
    
    if(m_bPhysics){
        auto body = PhysicsBody::create();
        body->setDynamic(false);
        PolygonHelp::getInstance()->addShapesWithFile("blender_lid", body,-m_pLid->getContentSize().width/2,-m_pLid->getContentSize().height/2);
        body->getFirstShape()->setMaterial(PhysicsMaterial(0.8f, 0.0f, 0.8f));
        
        m_pLid->setPhysicsBody(body);
    }
}
void BlenderCup::initWithIndex(int flavor)
{
    Node* pCocosNode = CSLoader::createNode("BlenderCupNode.csb");
    std::stringstream ostr2;
    ostr2<<"content/make/common/blender/juice"<<flavor<<"_0.png";
    m_pWaterFront = Sprite::create(ostr2.str());
    m_pWaterFrontGrid->addChild(m_pWaterFront);
    m_pWaterFront->setAnchorPoint(Vec2(0.5, 0));
    m_pWaterFront->setPosition(pCocosNode->getChildByName("blender_cup_mask")->getChildByName("juice")->getPosition()-m_pClippingNode->getStencil()->getAnchorPointInPoints()-Vec2(0,m_pWaterFront->getContentSize().height/2));
    m_pWaterFront->setOpacity(0);
    
    m_nFlavorIndex = flavor;
    waterStartPos = m_pWaterFront->getPosition();
    
    m_fCurHeight = m_pWaterFront->getContentSize().height;
}

void BlenderCup::addWater()
{
    Node* pCocosNode = CSLoader::createNode("BlenderCupNode.csb");
    
    std::stringstream ostr;
    ostr<<"content/make/common/blender/blender_water0.png";
    m_pWater = Sprite::create(ostr.str());
    m_pWaterGrid->addChild(m_pWater);
    m_pWater->setAnchorPoint(Vec2(0.5, 0));
    m_pWater->setPosition(pCocosNode->getChildByName("blender_cup_mask")->getChildByName("juice")->getPosition()-m_pClippingNode->getStencil()->getAnchorPointInPoints()-Vec2(0,m_pWater->getContentSize().height/2));
    m_pWater->setScale(1,0);
    m_pWater->runAction(ScaleTo::create(2, 1));
    
}
void BlenderCup::setStateEmpty()
{
    if (!m_pWater) {
        return;
    }
    m_pWater->setVisible(false);
    m_pWaterDrop->setVisible(false);
}

void BlenderCup::addFruits(int fruitType,int range,int count)
{
    m_nFruitType = fruitType;
    m_nFruitRange = range;
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<"content/make/common/blender/fruit"<<fruitType<<"_"<<i%range+1<<".png";
        
        Sprite* pIceIn = Sprite::create(ostr.str());
        m_FruitsVector.push_back(pIceIn);
        pIceIn->setVisible(false);
        m_pIceNode->addChild(pIceIn);
        float rotate = -40.0+20.0*(arc4random()%4);
        float rotateY = -40.0+20.0*(arc4random()%4);
        float rotateZ = -40.0+20.0*(arc4random()%4);
        pIceIn->setScale(0.5);
        pIceIn->setPosition(Vec2(0 ,-50));
        pIceIn->setRotation3D(Vec3(rotate, rotateY, rotateZ));
        
        auto body = PhysicsBody::createCircle(MIN(pIceIn->getContentSize().width, pIceIn->getContentSize().height)*0.1,PhysicsMaterial(0.8f, 0.1f, 0.8f));
        pIceIn->setPhysicsBody(body);
        pIceIn->setVisible(true);
    }
}

void BlenderCup::addIces(int iceType,int count)
{
    m_nIceType = iceType;
    for (int i = 0; i<count; i++) {
        std::stringstream ostr;
        ostr<<"content/make/common/ice/"<<iceType+1<<".png";
        
        Sprite* pIceIn = Sprite::create(ostr.str());
        m_IcesVector.push_back(pIceIn);
        pIceIn->setVisible(false);
        m_pIceNode->addChild(pIceIn);
        float rotate = -40.0+20.0*(arc4random()%4);
        float rotateY = -40.0+20.0*(arc4random()%4);
        float rotateZ = -40.0+20.0*(arc4random()%4);
        pIceIn->setScale(0.7);
        pIceIn->setPosition(Vec2(-100 ,-50));
        m_IcesVector.push_back(pIceIn);
        pIceIn->setRotation3D(Vec3(rotate, rotateY, rotateZ));
        
//        auto body = PhysicsBody::createBox(pIceIn->getContentSize()*0.3);
//        PhysicsBody* body = PhysicsBody::createBox(pIceIn->getContentSize()*0.4,PhysicsMaterial(0.8f, 0.1f, 0.8f),pIceIn->getContentSize()*0.2);
        auto body = PhysicsBody::createCircle(MAX(pIceIn->getContentSize().width, pIceIn->getContentSize().height)*0.2,PhysicsMaterial(0.8f, 0.1f, 0.8f));
//        body->setDynamic(false);
        pIceIn->setPhysicsBody(body);
        pIceIn->setVisible(true);
    }
}
void BlenderCup::addIce(int iceType,Vec2 posWorld)
{
    
    m_nIceType = iceType;
    std::stringstream ostr;
    ostr<<"content/make/common/ice/"<<iceType+1<<".png";
    Sprite* pIce = Sprite::create(ostr.str());
    this->addChild(pIce,8);
    pIce->setPosition(this->convertToNodeSpace(posWorld));
    pIce->setScale(0.7);
    
    Sprite* pIceIn = Sprite::createWithTexture(pIce->getTexture());
    m_IcesVector.push_back(pIceIn);
    pIceIn->setVisible(false);
    m_pIceNode->addChild(pIceIn);
    
    float rotate = -40.0+20.0*(arc4random()%4);
    float rotateY = -40.0+20.0*(arc4random()%4);
    float rotateZ = -40.0+20.0*(arc4random()%4);
    pIceIn->setScale(0.7);
    
    pIce->runAction(Sequence::create(JumpTo::create(0.5, Vec2(0, m_pCupBack->getContentSize().height*0.3), 30, 1),
                                     CallFunc::create([=](){
        auto body = PhysicsBody::createCircle(MIN(pIceIn->getContentSize().width, pIceIn->getContentSize().height)*0.2,PhysicsMaterial(0.8f, 0.1f, 0.8f));
        
        pIceIn->setPosition(pIce->getPosition());
        pIceIn->setPhysicsBody(body);
        pIceIn->setVisible(true);
        pIceIn->runAction(RotateBy::create(1, Vec3(rotate, rotateY, rotateZ)));
        pIce->removeFromParent();
    }), NULL));
}

void BlenderCup::pauceCrushIceManual(int step)
{
    m_pWaterGrid->pause();
    m_pWaterFrontGrid->pause();
    m_pWater->pause();
    m_pWater->pause();
    m_pWaterFront->pause();
    m_pIceNode->pause();
    AudioHelp::getInstance()->stopLoopEffect();
    
}
void BlenderCup::resumeCrushIceManual(int step)
{
    m_pWaterGrid->resume();
    m_pWaterFrontGrid->resume();
    m_pWater->resume();
    m_pWater->resume();
    m_pWaterFront->resume();
    m_pIceNode->resume();
    crushIceManual(step);
}

void BlenderCup::finishedCrush()
{
    pauceCrushIceManual(0);
    std::stringstream ostr;
    ostr<<"content/make/common/blender/juice"<<m_nFlavorIndex<<"_1.png";
    m_pWaterFront->setTexture(ostr.str());
}
void BlenderCup::crushIceManual(int step)
{
    AudioHelp::getInstance()->playLoopEffect("handeled_ice_crusher.mp3");
    if (m_nStep==step) {
        return;
    }
    m_nStep = step;
    if(step==0) {
        if (onBeginCrushIce) {
            onBeginCrushIce();
        }
        m_pWaterGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 5, 13)));
        m_pWaterFrontGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 5, 10)));
        m_pWater->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            m_pWater->setScaleX(-m_pWater->getScaleX());
        }),
                                                                   DelayTime::create(0.3), NULL)));
        m_pWaterFront->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            m_pWater->setScaleX(-m_pWater->getScaleX());
        }),
                                                                        DelayTime::create(0.3), NULL)));
        int k = 0;
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&k](Sprite*pIce){
            auto body = pIce->getPhysicsBody();
            float deltX = (-10.0+5.0*(arc4random()%4));
            float deltY = (20.0+4.0*(arc4random()%5));
            body->applyImpulse(Vec2(deltX*1000, deltY*1000));
            k++;
        });
    }else if(step==1){
        std::vector<Sprite*>tempVector;
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&tempVector](Sprite*pIce){
            std::stringstream ostr;
            ostr<<"content/make/common/ice/"<<m_nIceType+1;
            Sprite* pIce0 = Sprite::create(ostr.str()+"/2_1.png");
            m_pIceNode->addChild(pIce0);
            pIce0->setPosition(pIce->getPosition());
            pIce0->setTag(0);
            pIce0->setScale(0.8);
            auto body = PhysicsBody::createCircle(MIN(pIce0->getContentSize().width, pIce0->getContentSize().height)*0.2);
            body->addMass(20);
            pIce0->setPhysicsBody(body);
            
            Sprite* pIce1 = Sprite::create(ostr.str()+"/2_2.png");
            m_pIceNode->addChild(pIce1);
            pIce1->setPosition(pIce->getPosition());
            pIce1->setTag(1);
            pIce1->setScale(0.8);
            auto body2 = PhysicsBody::createCircle(MIN(pIce1->getContentSize().width, pIce1->getContentSize().height)*0.2);
            body2->addMass(20);
            pIce1->setPhysicsBody(body2);
            
            tempVector.push_back(pIce0);
            tempVector.push_back(pIce1);
            //            pIce->removeFromParent();
            pIce->setVisible(false);
        });
        m_IcesVector.clear();
        m_IcesVector.operator=(tempVector);
    }else if(step==2){
        std::vector<Sprite*>tempVector;
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&tempVector](Sprite*pIce){
            int tag = pIce->getTag();
            std::stringstream ostr;
            ostr<<"content/make/common/ice/"<<m_nIceType+1;
            Sprite* pIce0 = Sprite::create(ostr.str()+"/3_"+std::to_string(tag*2+1)+".png");
            m_pIceNode->addChild(pIce0);
            pIce0->setPosition(pIce->getPosition());
            Vec2 posDelt = Vec2((50.0-20.0*(arc4random()%5)), (40.0-8.0*(arc4random()%5)));
            pIce0->setTag(0);
            pIce0->setScale(0.8);
            auto body = PhysicsBody::createCircle(MIN(pIce0->getContentSize().width, pIce0->getContentSize().height)*0.2);
            body->addMass(20);
            pIce0->setPhysicsBody(body);
            
            Sprite* pIce1 = Sprite::create(ostr.str()+"/3_"+std::to_string(tag*2+2)+".png");
            m_pIceNode->addChild(pIce1);
            pIce1->setPosition(pIce->getPosition());
            pIce1->setTag(1);
            pIce1->setScale(0.8);
            auto body2 = PhysicsBody::createCircle(MIN(pIce1->getContentSize().width, pIce1->getContentSize().height)*0.2);
            body2->addMass(20);
            pIce1->setPhysicsBody(body2);
            
            tempVector.push_back(pIce0);
            tempVector.push_back(pIce1);
            //            pIce->removeFromParent();
            pIce->setVisible(false);
        });
        m_IcesVector.clear();
        m_IcesVector.operator=(tempVector);
    }
    else if(step==3){
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=](Sprite*pIce){
            pIce->runAction(Sequence::create(ScaleTo::create(2, 0.2),
                                             FadeOut::create(0.5),
                                             CallFunc::create([=](){
                m_pIceNode->stopAllActions();
                //                pIce->removeFromParent();
                pIce->setVisible(false);
            }), NULL));
        });
        m_pWater->runAction(ScaleTo::create(1, 1,1.1));
        m_pWaterFront->runAction(Sequence::create(ScaleTo::create(1, 1,1.1),
                                                  CallFunc::create([=](){
            ParticleSystemQuad *pParticle = ParticleSystemQuad::create("particle/crush_ice.plist");
            m_pClippingNode->addChild(pParticle,eZOrderWaterIce);
            pParticle->setPosition(Vec2(0,0));
            std::stringstream ostr;
            ostr<<"content/make/common/blender/juice_animation/"<<m_nFlavorIndex+1<<"_";
            m_pWater->setScale(1);
            m_pWaterFront->setScale(1);
            this->setWaterPercent(90);
            m_pWater->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 3,false,true,1));
            m_pWaterFront->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 3,false,true,1));
            m_pWaterFront->setOpacity(150);
            
            if(onFinishCrushIce){
                onFinishCrushIce();
            }
        }), NULL));
    }
    else if(step==4){
    }
    else if(step==5){
        
//        m_pWaterGrid->stopAllActions();
//        m_pWaterFrontGrid->stopAllActions();
//        m_pWater->stopAllActions();
//        m_pWater->stopAllActions();
//        m_pWaterFront->stopAllActions();
//        m_pIceNode->stopAllActions();
//        
//        if(onFinishCrushIce){
//            onFinishCrushIce();
//        }
//        AudioHelp::getInstance()->stopLoopEffect();
    }
    
    
}

void BlenderCup::crushIce()
{
    if (onBeginCrushIce) {
        onBeginCrushIce();
    }
    AudioHelp::getInstance()->playLoopEffect("automatic_ice_crusher.mp3");
    m_pWaterGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 5, 13)));
    m_pWaterFrontGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 5, 10)));
    m_pWater->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        m_pWater->setScaleX(-m_pWater->getScaleX());
    }),
                                                               DelayTime::create(0.3), NULL)));
    m_pWaterFront->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        m_pWater->setScaleX(-m_pWater->getScaleX());
    }),
                                                               DelayTime::create(0.3), NULL)));

    
    m_pIceNode->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        int k = 0;
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&k](Sprite*pIce){
            auto body = pIce->getPhysicsBody();
            float deltX = (-10.0+5.0*(arc4random()%4));
            float deltY = (20.0+4.0*(arc4random()%5));
            body->applyImpulse(Vec2(deltX*1000, deltY*1000));
            k++;
        });
        for_each(m_FruitsVector.begin(),m_FruitsVector.end(),[=,&k](Sprite*pIce){
            auto body = pIce->getPhysicsBody();
            float deltX = (-10.0+5.0*(arc4random()%4));
            float deltY = (20.0+8.0*(arc4random()%5));
            body->applyImpulse(Vec2(deltX*1000, deltY*1000));
            k++;
        });
    }),
                                                           DelayTime::create(0.5), NULL)));
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        
        for_each(m_FruitsVector.begin(),m_FruitsVector.end(),[=](Sprite*pFruit){
            pFruit->runAction(Sequence::create(DelayTime::create(1),
                                               FadeOut::create(2), NULL));
        });
//        std::vector<Sprite*>tempVector;
//        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&tempVector](Sprite*pIce){
//            std::stringstream ostr;
//            ostr<<"content/make/common/ice/"<<m_nIceType+1;
//            Sprite* pIce0 = Sprite::create(ostr.str()+"/2_1.png");
//            m_pIceNode->addChild(pIce0);
//            pIce0->setPosition(pIce->getPosition());
//            pIce0->setTag(0);
//            pIce0->setScale(0.8);
//            auto body = PhysicsBody::createCircle(MIN(pIce0->getContentSize().width, pIce0->getContentSize().height)*0.2);
//            body->addMass(20);
//            pIce0->setPhysicsBody(body);
//            
//            Sprite* pIce1 = Sprite::create(ostr.str()+"/2_2.png");
//            m_pIceNode->addChild(pIce1);
//            pIce1->setPosition(pIce->getPosition());
//            pIce1->setTag(1);
//            pIce1->setScale(0.8);
//            auto body2 = PhysicsBody::createCircle(MIN(pIce1->getContentSize().width, pIce1->getContentSize().height)*0.2);
//            body2->addMass(20);
//            pIce1->setPhysicsBody(body2);
//            
//            tempVector.push_back(pIce0);
//            tempVector.push_back(pIce1);
////            pIce->removeFromParent();
//            pIce->setVisible(false);
//        });
//        m_IcesVector.clear();
//        m_IcesVector.operator=(tempVector);
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        Sprite* pFruit = Sprite::create("content/make/common/blender/fruit_crash"+std::to_string(m_nFlavorIndex)+".png");
        m_pWaterFrontGrid->addChild(pFruit);
        pFruit->setAnchorPoint(Vec2(0.5, 0));
        pFruit->setPosition(m_pWater->getPosition());
        pFruit->runAction(Sequence::create(DelayTime::create(2),
                                           FadeOut::create(1), NULL));
        
        std::vector<Sprite*>tempVector;
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=,&tempVector](Sprite*pIce){
            int tag = pIce->getTag();
            std::stringstream ostr;
            ostr<<"content/make/common/ice/"<<m_nIceType+1;
            Sprite* pIce0 = Sprite::create(ostr.str()+"/3_"+std::to_string(tag*2+1)+".png");
            m_pIceNode->addChild(pIce0);
            pIce0->setPosition(pIce->getPosition());
            Vec2 posDelt = Vec2((50.0-20.0*(arc4random()%5)), (40.0-8.0*(arc4random()%5)));
            pIce0->setTag(0);
            pIce0->setScale(0.8);
            auto body = PhysicsBody::createCircle(MIN(pIce0->getContentSize().width, pIce0->getContentSize().height)*0.2);
            body->addMass(20);
            pIce0->setPhysicsBody(body);
            
            Sprite* pIce1 = Sprite::create(ostr.str()+"/3_"+std::to_string(tag*2+2)+".png");
            m_pIceNode->addChild(pIce1);
            pIce1->setPosition(pIce->getPosition());
            pIce1->setTag(1);
            pIce1->setScale(0.8);
            auto body2 = PhysicsBody::createCircle(MIN(pIce1->getContentSize().width, pIce1->getContentSize().height)*0.2);
            body2->addMass(20);
            pIce1->setPhysicsBody(body2);
            
            tempVector.push_back(pIce0);
            tempVector.push_back(pIce1);
//            pIce->removeFromParent();
            pIce->setVisible(false);
        });
        m_IcesVector.clear();
        m_IcesVector.operator=(tempVector);
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        for_each(m_IcesVector.begin(),m_IcesVector.end(),[=](Sprite*pIce){
            pIce->runAction(Sequence::create(ScaleTo::create(2, 0.2),
                                             FadeOut::create(0.5),
                                             CallFunc::create([=](){
                m_pIceNode->stopAllActions();
//                pIce->removeFromParent();
                pIce->setVisible(false);
            }), NULL));
        });
        m_pWater->runAction(ScaleTo::create(1, 1,1.1));
        m_pWaterFront->runAction(ScaleTo::create(1, 1,1.1));
        AudioHelp::getInstance()->playEffect("vo_juice_ready_soon.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
//        ParticleSystemQuad *pParticle = ParticleSystemQuad::create("particle/crush_ice.plist");
//        m_pClippingNode->addChild(pParticle,eZOrderWaterIce);
//        pParticle->setPosition(Vec2(0,0));
        std::stringstream ostr;
        ostr<<"content/make/common/blender/juice_animation/"<<m_nFlavorIndex<<"_";
        m_pWater->setScale(1);
        m_pWaterFront->setScale(1);
//        this->setWaterPercent(90);
        m_pWaterFront->runAction(FadeIn::create(1));
        m_pWater->runAction(FadeOut::create(1));
//        m_pWater->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 3,false,true,1));
//        m_pWaterFront->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 3,false,true,1));
//        m_pWaterFront->setOpacity(150);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("ding.mp3");
        if(onFinishCrushIce){
            onFinishCrushIce();
        }
//        AudioHelp::getInstance()->stopLoopEffect();
//        m_pWaterGrid->stopAllActions();
//        m_pWaterFrontGrid->stopAllActions();
//        m_pWater->stopAllActions();
//        m_pWater->stopAllActions();
//        m_pWaterFront->stopAllActions();
//        m_pIceNode->stopAllActions();
        
    }),  NULL));
}

void BlenderCup::showShadow(bool show)
{
    if (!m_pShadow) {
        return;
    }
    m_pShadow->setVisible(show);
}

void BlenderCup::showLid(bool show,bool animate)
{
    if (!m_pLid) {
        return;
    }
    m_bLidIsShow = show;
    if (show) {
        m_pLid->setVisible(true);
        if (animate) {
            m_pLid->runAction(Sequence::create(MoveBy::create(1, Vec2(-300, -1000)),
                                               CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("soda_cup.mp3");
            }), NULL));
        }else{
        }
    }else{
        if (animate) {
            AudioHelp::getInstance()->playEffect("soda_cup.mp3");
            m_pLid->runAction(Sequence::create(MoveBy::create(1, Vec2(300, 1000)),
                                               CallFunc::create([=](){
                m_pLid->setVisible(false);
            }), NULL));
        }else{
//            m_pLid->setPosition(m_pLid->getPosition()+Vec2(-300, -1000));
            m_pLid->setVisible(false);
        }
    }
}
void BlenderCup::showFinished(bool strawDirectionLeft)
{
    ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pClippingNode->getChildByName("bubble");
    if (!pParticle) {
//        pParticle = ParticleSystemQuad::create("particle/soda.plist");
//        m_pClippingNode->addChild(pParticle);
//        pParticle->setPosition(Vec2(0,-100));
//        pParticle->setName("bubble");
    }
    setWaterPercent(90);
    std::stringstream ostr;
    ostr<<"content/make/common/blender/juice/"<<m_nFlavorIndex<<"_1.png";
    m_pWater->setTexture(ostr.str());
    m_pWaterFront->setTexture(ostr.str());
    m_pWater->setScale(1);
    m_pWaterFront->setScale(1);
//    m_pWaterFront->setOpacity(150);
}

void BlenderCup::pourJuice()
{
    m_pWaterGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 3, 13)));
    m_pWater->runAction(Sequence::create(MoveBy::create(4, Vec2(-m_pWater->getContentSize().width*0.9, 0)),
                                         CallFunc::create([=](){
        if(onFinishPour){
            onFinishPour();
        }
    }), NULL));
    m_pWaterFront->runAction(MoveBy::create(4, Vec2(-m_pWater->getContentSize().width*0.9, 0)));
}

void BlenderCup::updateWater(int index)
{
    if (index==m_nFlavorIndex) {
        return;
    }
    std::stringstream ostr;
    ostr<<"content/make/common/blender/cup_juice_down/"<<index+1<<".png";
    if (m_pWater) {
        Sprite* pCopyWater = Sprite::createWithTexture(m_pWater->getTexture());
        m_pClippingNode->addChild(pCopyWater);
        pCopyWater->setAnchorPoint(m_pWater->getAnchorPoint());
        pCopyWater->setPosition(m_pWater->getPosition());
        pCopyWater->setScaleX(m_pWater->getScaleX());
        pCopyWater->setScaleY(m_pWater->getScaleY());
//        pCopyWater->runAction(Sequence::create(FadeOut::create(1),
//                                               CallFunc::create([=]()
//                                                                {
//                                                                    pCopyWater->removeFromParent();
//                                                                }), NULL));
        pCopyWater->runAction(FadeTo::create(2, 50));
        m_pWater->setTexture(ostr.str());
    }else{
        m_pWater = Sprite::create(ostr.str());
        m_pWater->setAnchorPoint(Vec2(0.5, 0));
        m_pWater->setPosition(Vec2(0, -m_pWater->getContentSize().height/2));
        m_pClippingNode->addChild(m_pWater);
    }
    m_nFlavorIndex = index;
    
}

void BlenderCup::setWaterPercent(int percent)
{
    if (!m_pWater) {
        return;
    }
    m_pWater->setVisible(true);
    m_pWaterFront->setVisible(true);
    if (percent>100) {
        percent = 100;
    }else if(percent<0){
        percent = 0;
        ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pClippingNode->getChildByName("bubble");
        if (!pParticle){
            pParticle->removeFromParent();
        }
    }
    m_fPercent = percent;
    if (m_pWater) {
//        m_pWater->setScaleY(0.1+1.0f*percent/100);
        m_fCurHeight = m_pWaterFront->getContentSize().height*percent/100;
        m_pWaterFront->setPosition(waterStartPos+Vec2(0,-m_pWater->getContentSize().height+m_fCurHeight));
        m_pWater->setPosition(m_pWater->getPosition());
    }
}

void BlenderCup::addJuice(float height)
{
    m_fHeightDelta = height;
//    scheduleUpdate();
    resumeAddJuice();
//    m_pCupFront->setOpacity(100);
}

void BlenderCup::pauseAddJuice()
{
//    _scheduler->pauseTarget(this);
    unscheduleUpdate();
//    ParticleSystemQuad* pNode = (ParticleSystemQuad*)this->getChildByName("juice_stream");
//    if (pNode) {
//        pNode->stopSystem();
//        pNode->removeFromParent();
//    }
    Node* pNode = (ParticleSystemQuad*)this->getChildByName("juice_stream");
    if (pNode) {
        pNode->stopAllActions();
        pNode->removeFromParent();
    }
}

void BlenderCup::resumeAddJuice()
{
//    _scheduler->resumeTarget(this);
    scheduleUpdate();
    
    std::stringstream ostr;
//    ostr<<"content/make/common/machine/grain/"<<m_nFlavorIndex+1<<".png";
//    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/juice.plist");
//    pParticle->setTexture(Sprite::create(ostr.str())->getTexture());
//    this->addChild(pParticle);
//    pParticle->setPosition(waterStreamPos);
//    pParticle->setName("juice_stream");
    
    ostr<<"content/make/common/machine/juice_animation/"<<m_nFlavorIndex<<"_";
    Sprite* pStream = Sprite::create(ostr.str()+"1.png");
    this->addChild(pStream);
    pStream->setAnchorPoint(Vec2(0.5, 1));
    pStream->setPosition(waterStreamPos);
    pStream->setName("juice_stream");
    pStream->runAction(RepeatForever::create(AnimationHelp::createAnimate(ostr.str(), 1, 2)));
}

void BlenderCup::update(float dt)
{
    if (m_pWater) {
        m_fCurHeight += m_fHeightDelta;
        int percent = 0;
//        log("=====contentsize = %f,%f",m_pWater->getContentSize().width,m_pWater->getContentSize().height);
        if (m_fCurHeight>m_pWater->getContentSize().height) {
            m_fCurHeight = m_pWater->getContentSize().height;
        }else if(m_fCurHeight<0){
            m_fCurHeight = 0;
        }
        percent = m_fCurHeight*100/m_pWater->getContentSize().height;
        m_fPercent = percent;
        log("====percent===%d",m_fPercent);
        float scale =1.0f*percent/100;
        if (scale<0.2) {
            scale = 0.2;
        }
//        m_pWater->setScaleY(scale);
        
        m_pWater->setPosition(waterStartPos+Vec2(0,-m_pWater->getContentSize().height+m_fCurHeight));
        m_pWaterFront->setPosition(m_pWater->getPosition());
        
        ParticleSystemQuad*pParticle2 = (ParticleSystemQuad*)this->getChildByName("soda");
        if (pParticle2){
            Vec2 point = m_pWater->getPosition();
            if (m_fCurHeight<80) {
                point.y = 120-m_pWater->getContentSize().height/2;
            }else if (m_fCurHeight>m_pWater->getContentSize().height-20){
                point.y = m_pWater->getContentSize().height/2;
            }else{
                point.y = m_fCurHeight-m_pWater->getContentSize().height/2+50;
            }
            pParticle2->setPosition(point);
        }
        
        if (percent%10==0) {
            for_each(m_IcesVector.begin(),m_IcesVector.end(),[=](Sprite* pIce){
//                float rotateX = 10.0-5.0*(arc4random()%4);
//                float rotateY = 10.0-5.0*(arc4random()%4);
//                float rotateZ = 10.0-5.0*(arc4random()%4);
//                float poX = 10.0-5.0*(arc4random()%4);
//                pIce->runAction(EaseBackOut::create(MoveBy::create(0.3, Vec2(poX, 5))));
//                pIce->runAction(RotateBy::create(0.3, Vec3(rotateX, rotateY, rotateZ)));
                //                pIce->setPosition(pIce->getPosition()+Vec2(0, 5));
                
                float posX = 10.0-5.0*(arc4random()%4);
                auto body = pIce->getPhysicsBody();
                body->applyImpulse(Vec2(posX*1000, 25*1000));
            });
        }
        if (percent>=60){
            ParticleSystemQuad*pParticle = (ParticleSystemQuad*)m_pClippingNode->getChildByName("bubble");
            if (!pParticle) {
//                pParticle = ParticleSystemQuad::create("particle/soda.plist");
//                m_pClippingNode->addChild(pParticle);
//                pParticle->setPosition(Vec2(0,-100));
//                pParticle->setName("bubble");
            }
        }
        if (m_fCurHeight>=m_pWater->getContentSize().height || m_fCurHeight<=0) {
            unscheduleUpdate();
        }
    }
}

void BlenderCup::saveWaterImage()
{
    RenderTexture* rt = RenderTexture::create(m_pWater->getContentSize().width, m_pWater->getContentSize().height);
    //    rt->setPosition(visibleSize*0.5);
    Vector<Node*>pChildVector = m_pClippingNode->getChildren();
    
    rt->begin();
    for (int i = 0; i<pChildVector.size(); i++) {
        Node* pNode = pChildVector.at(i);
        if (pNode!=m_pClippingNode->getStencil()) {
            Vec2 position = pNode->getPosition();
            pNode->setPosition(Vec2(m_pWater->getContentSize().width/2, 0));
            pNode->visit();
            pNode->setPosition(position);
        }
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = rt->newImage();
    pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"water.png", false);
}
