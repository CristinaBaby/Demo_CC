//
//  MakeIceCreamStep02.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/19.
//
//

#include "MakeIceCreamStep02.hpp"
#define AGITATION_COUNT_BASE 35

enum spriteTag{
    eBowl_tag,
    eNodeGrid_tag,
    eHandle_tag,
    eEdgeShape_tag
};
MakeIceCreamStep02::MakeIceCreamStep02():m_pBg(nullptr),m_pMovableLayer(nullptr),m_pBarrel(nullptr),m_pBarrel_up(nullptr),m_pBarrel_up_temp(nullptr),m_pRing(nullptr),m_pAgitator_down(nullptr),m_pAgitator_up(nullptr),m_pRotor(nullptr),m_pIcecream(nullptr),isMoving(false),m_iStep(0),m_iIceCakeCount(0),m_pCover(nullptr),m_pCover_on(nullptr),m_pLid(nullptr),m_pHandle(nullptr),m_iTime(0),m_iIceCreamSoundId(-1),m_pListener(nullptr),m_pSnowParticle(nullptr),m_iMachineMixSoundId(-1),m_pProgressBg(nullptr),m_pProgressTimer(nullptr),m_pFlag(nullptr),m_pHandTip(nullptr)
{
    
}

MakeIceCreamStep02::~MakeIceCreamStep02()
{
    ArmatureDataManager::getInstance()->removeArmatureFileInfo("images/step/page4/parts/animation/NewAnimation.ExportJson");
}

Scene* MakeIceCreamStep02::scene()
{
    auto pScene = Scene::createWithPhysics();
//    pScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    pScene->getPhysicsWorld()->setGravity(Vec2(0, -150));
    auto pLayer = MakeIceCreamStep02::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool MakeIceCreamStep02::init()
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
        CC_BREAK_IF(!initLayer());
//        this->setNextBtnIsVisible(true);
        isInit = true;
    }while(0);
    
    return isInit;
}

bool MakeIceCreamStep02::initData()
{
    m_vSaveMoveLayerPos = Vec2::ZERO;

    return true;
}

bool MakeIceCreamStep02::initLayer()
{
    // 由于背景图片过大，在部分android机型上无法加载，所以切分成4张图拼接而成
    m_pBg = Node::create();
    m_pBg->setContentSize(Size(2345, 1703));
    m_pBg->ignoreAnchorPointForPosition(false);
    m_pBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pBg, 480, 320 - 250);
    this->addChild(m_pBg);
    
    // 左上
    auto pPart_LT = Sprite::create("images/step/bg/bg_1.jpg");
    pPart_LT->setAnchorPoint(Vec2(1, 0));
    pPart_LT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LT);
    
    // 右上
    auto pPart_RT = Sprite::create("images/step/bg/bg_2.jpg");
    pPart_RT->setAnchorPoint(Vec2(0, 0));
    pPart_RT->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RT);
    
    // 左下
    auto pPart_LD = Sprite::create("images/step/bg/bg_3.jpg");
    pPart_LD->setAnchorPoint(Vec2(1, 1));
    pPart_LD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_LD);
    
    // 右下
    auto pPart_RD = Sprite::create("images/step/bg/bg_4.jpg");
    pPart_RD->setAnchorPoint(Vec2(0, 1));
    pPart_RD->setPosition(m_pBg->getContentSize() / 2);
    m_pBg->addChild(pPart_RD);
    
    
    ////////////////////////////////////////////////////////
    // 由于动画需求，故桌子由两张图拼接而成
    m_pTable = Layer::create();
    m_pTable->setContentSize(Size(960, 640));
    m_pTable->ignoreAnchorPointForPosition(false);
    kAdapterScreen->setExactPosition(m_pTable, 480, 320);
    this->addChild(m_pTable, 3);
    
    auto pTable1 = Sprite::create("images/step/table_2.png");
    pTable1->setAnchorPoint(Vec2(0.5f, 0));
    pTable1->setPosition(Vec2(480, 69));
    m_pTable->addChild(pTable1, 1);
    
    auto pTable2 = Sprite::create("images/step/table_1.png");
    pTable2->setAnchorPoint(Vec2(0.5f, 1));
    pTable2->setPosition(Vec2(480, 69));
    m_pTable->addChild(pTable2, 2);
    
    // 播放进场动画
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(120 + kAdapterScreen->g_oOffset.x, 420 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(0 + kAdapterScreen->g_oOffset.x, 220 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(480 - 600 + kAdapterScreen->g_oOffset.x, 320 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(4.5f, oBezier);
    m_pBg->runAction(Sequence::create(DelayTime::create(0.6f), EaseSineOut::create(pBezierTo), nullptr));
    pTable1->runAction(Sequence::create(DelayTime::create(0.6f), ScaleTo::create(2.5f, 1.0f, 0.7f), ScaleTo::create(2, 1.0f), CallFunc::create([=](){
    
        m_pMovableLayer = Layer::create();
        m_pMovableLayer->setContentSize(Size(960, 640));
        m_pMovableLayer->ignoreAnchorPointForPosition(false);
        kAdapterScreen->setExactPosition(m_pMovableLayer, 480, 320);
        this->addChild(m_pMovableLayer, 5);
        
        // //////////////////////////////////////////
        // 添加机器零件
        // 木桶
        m_pBarrel = Sprite::create("images/step/page4/barrel_big_down.png");
        m_pBarrel->setPosition(650 + 650, 430);
        m_pMovableLayer->addChild(m_pBarrel, 2);
        
        m_pBarrel_up = Sprite::create("images/step/page4/barrel_bia_on.png");
        m_pBarrel_up->setPosition(650 + 650, 430);
        m_pMovableLayer->addChild(m_pBarrel_up, 20);
        
        m_pRing = Sprite::create("images/step/page4/parts/barrel_ring1.png");
        m_pRing->setAnchorPoint(Vec2(1, 1));
        m_pRing->setPosition(509 + 650, 564);
        m_pMovableLayer->addChild(m_pRing, 23);
        
        m_pResponseNode = LayerColor::create(Color4B(0, 0, 0, 0));
        m_pResponseNode->setContentSize(Size(380, 300));
        m_pResponseNode->ignoreAnchorPointForPosition(false);
        kAdapterScreen->setExactPosition(m_pResponseNode, 480, 340);
        this->addChild(m_pResponseNode, 30);
        
        // add shadow
        auto pShadow = Sprite::create("images/step/page4/barrel_shadow.png");
        pShadow->setPosition(145, 65);
        m_pBarrel->addChild(pShadow, -1);
        
        auto pAction = Sequence::create(EaseBackOut::create(MoveBy::create(1.1f, Vec2(-650, 0))), nullptr);
        m_pBarrel->runAction(pAction);
        m_pBarrel_up->runAction(pAction->clone());
        m_pRing->runAction(pAction->clone());
        
        ////////////////
        // 容器
        auto pAgitator = Sprite::create("images/step/page4/agitator1_1.png");
        kAdapterScreen->setExactPosition(pAgitator, 340 - 550, 420);
        this->addChild(pAgitator, 21);
        pAgitator->runAction(Sequence::create(DelayTime::create(0.6f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(550, 0))), nullptr));
        
        // 盖子
        auto pAgitatorCover = Sprite::create("images/step/page4/agitator3_1.png");
        kAdapterScreen->setExactPosition(pAgitatorCover, 220 - 500, 345);
        this->addChild(pAgitatorCover, 21);
        pAgitatorCover->runAction(Sequence::create(DelayTime::create(1.2f), MoveBy::create(0.7f, Vec2(500, 0)), nullptr));
        
        /////////////////
        // 转子
        auto pRotor = Sprite::create("images/step/page4/agitator2_1.png");
        kAdapterScreen->setExactPosition(pRotor, 750 + 500, 195);
        this->addChild(pRotor, 22);
        pRotor->runAction(Sequence::create(DelayTime::create(1.8f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-500, 0))), nullptr));
        
        /////////////////
        // 摇杆
        auto pRockers = Sprite::create("images/step/page4/agitator4_1.png");
        kAdapterScreen->setExactPosition(pRockers, 350 - 700, 190);
        this->addChild(pRockers, 22);
        pRockers->runAction(Sequence::create(DelayTime::create(2.4f), EaseBackOut::create(MoveBy::create(1.2f, Vec2(700, 0))), nullptr));
        
        this->runAction(Sequence::create(DelayTime::create(4.5f), CallFunc::create([=](){
            
            pRockers->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(-700, 0))), RemoveSelf::create(), nullptr));
            pRotor->runAction(Sequence::create(DelayTime::create(0.5f), EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(600, 0))), RemoveSelf::create(), nullptr));
            pAgitatorCover->runAction(Sequence::create(DelayTime::create(1.0f), EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(-600, 0))), RemoveSelf::create(), nullptr));
            pAgitator->runAction(Sequence::create(DelayTime::create(1.5f), EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(-600, 0))), RemoveSelf::create(), nullptr));
            
            // 移动桶和桶边缘的环
            auto pAction = Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.8f, Vec2(-170, -110)), nullptr);
            m_pRing->runAction(pAction);
            m_pBarrel->runAction(pAction->clone());
            m_pBarrel_up->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
                
                this->installAgitator1();
            }), nullptr));
            
        }), nullptr));
    }), nullptr));

    return true;
}

void MakeIceCreamStep02::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eBowl_tag) {
        if (isMoving || m_iStep != 0)
            return;
        if (m_pHandTip) {
            m_pHandTip->stopAllActions();
            m_pHandTip->setVisible(false);
        }
        
        toolSprite->setTexture("images/step/page3/bown2_down.png");
        m_vSaveMoveLayerPos = m_pMovableLayer->getPosition();
        m_pTable->stopAllActions();
        m_pMovableLayer->stopAllActions();
        // 向右移动木桶
        isMoving = true;
        m_iStep = 1;
        m_pMovableLayer->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(visibleSize.width / 2, m_pMovableLayer->getPositionY())), CallFunc::create([=](){
            m_iStep = 2;
            // 向下移动桌子和木桶
            m_pTable->runAction(MoveBy::create(0.2f, Vec2(0, -150)));
            m_pMovableLayer->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, -150)), CallFunc::create([=](){
                
                toolSprite->clearRect();
                toolSprite->addRectByNode(m_pResponseNode);
            }), nullptr));
            
        }), nullptr));
    }
}

void MakeIceCreamStep02::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
}

void MakeIceCreamStep02::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
}

void MakeIceCreamStep02::onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    if (toolSprite->getTag() == eBowl_tag) {
        toolSprite->setSpriteEnabled(false);
        m_pIcecream->stopAllActions();
        
        m_pIcecream->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(0.6f, 1.12f, 0.9f), MoveBy::create(0.6f, Vec2(-22, -10)), RotateBy::create(0.6f, 12), nullptr) , nullptr));
        toolSprite->runAction(Sequence::create(MoveTo::create(0.2f, m_pResponseNode->getPosition() + Vec2(180, 110)), RotateBy::create(0.6f, -35), CallFunc::create([=](){
            
            toolSprite->setLocalZOrder(4);
            toolSprite->setAnchorPoint(Vec2(0.02f, 0.6f));
            toolSprite->setPosition(toolSprite->getPosition() + Vec2(-toolSprite->getContentSize().width * toolSprite->getScale() * 0.48f + 14, -toolSprite->getContentSize().height * toolSprite->getScale() * 0.1f - 35));
            
            // 倾斜碗
            toolSprite->runAction(Sequence::create(RotateBy::create(4.0f, -25), DelayTime::create(0.3f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(650, 0))), CallFunc::create([=](){
                
                // 添加盖子
                m_pCover = Sprite::create("images/step/page4/parts/barrel_lid_down.png");
                m_pCover->setPosition(490, 360 + 600);
                m_pMovableLayer->addChild(m_pCover, 6);
                
                m_pCover_on = Sprite::create("images/step/page4/parts/barrel_lid_on.png");
                m_pCover_on->setPosition(490, 360 + 600);
                m_pMovableLayer->addChild(m_pCover_on, 8);
                
                auto pAction = MoveBy::create(1.0f, Vec2(0, -600));
                m_pCover->runAction(pAction);
                m_pCover_on->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
                
                    this->addCoverEdge(true);
                    
                    // 设置搅拌器(容器)的边界
                    Vec2 pos[5]={
                        Vec2(-82.9, 21.0),
                        Vec2(-74.6, -155.6),
                        Vec2(75.9, -156.9),
                        Vec2(83.2, 21.7),
                        Vec2(-83.0, 20.3)
                    };
                    auto body = PhysicsBody::createEdgeChain(pos, 5);
                    auto edgeShape = Node::create();
                    edgeShape->setPhysicsBody(body);
                    edgeShape->setPosition(m_pAgitator_down->getContentSize() / 2 + Size(0, 0));
                    edgeShape->setTag(eEdgeShape_tag);
                    m_pAgitator_down->addChild(edgeShape);
                    
                    // 添加冰块
                    this->addIceCake();
                }), nullptr));
            }), RemoveSelf::create(), nullptr));
            
            m_iIceCreamSoundId = SoundPlayer::getInstance()->playEffectLoop(waterSound);
            // 碗里的冰淇淋减少
            m_pIcecream->runAction(Sequence::create(Spawn::create(ScaleTo::create(4.2f, 0.5f, 0.1f), RotateBy::create(4.2f, 22), MoveBy::create(4.2f, Vec2(-115, -18)), nullptr), CallFunc::create([=](){
                
                if (m_iIceCreamSoundId != -1) {
                    SoundPlayer::getInstance()->stopEffectLoop(m_iIceCreamSoundId);
                    m_iIceCreamSoundId = -1;
                }
            }), FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
            
            // 添加倒冰淇淋的粒子动画
            auto pParticle = ParticleSystemQuad::create("plists/pourIcecream.plist");
            pParticle->setPosition(m_pBarrel->getPosition() + Vec2(65, 245));
            m_pMovableLayer->addChild(pParticle, 10);
            pParticle->setAutoRemoveOnFinish(true);
            
        }), nullptr));
    }
}

void MakeIceCreamStep02::onTouchUpOutRect(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eBowl_tag) {
        // 向上移动桌子和木桶
        isMoving = true;
        toolSprite->clearRect();
        m_pTable->stopAllActions();
        m_pMovableLayer->stopAllActions();
        
        if (m_iStep == 1) {
            float time = (m_pMovableLayer->getPositionX() - m_vSaveMoveLayerPos.x) / 500.0f;
            toolSprite->setMovebackTime(time);
            m_pMovableLayer->runAction(Sequence::create(MoveTo::create(time, m_vSaveMoveLayerPos), CallFunc::create([=](){
            
                isMoving = false;
                m_iStep = 0;
            }), nullptr) );
        }else if(m_iStep == 2){
            toolSprite->setMovebackTime(0.4f);
            m_pTable->runAction(MoveTo::create(0.2f, Vec2(visibleSize.width / 2, 320 + kAdapterScreen->g_oOffset.y)));
            m_pMovableLayer->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(visibleSize.width / 2, m_vSaveMoveLayerPos.y)), MoveTo::create(0.2f, m_vSaveMoveLayerPos), CallFunc::create([=](){
                
                isMoving = false;
                m_iStep = 0;
            }), nullptr));
        }else{
            isMoving = false;
            m_iStep = 0;
        }
    }
}

void MakeIceCreamStep02::onMoveBackCallback(ToolSprite* toolSprite)
{
    if (toolSprite->getTag() == eBowl_tag) {
        this->runHandTipAnimation(1);
        toolSprite->setTexture("images/step/page3/bown2_down1.png");
        //液体效果,参数依次为：持续时间、网格大小、波纹数、振幅
        auto liquid1 = Liquid::create(1.5, Size(16, 12), 1, 3);
        auto liquid2 = Liquid::create(2.5f, Size(16, 12), 1, 1);
        toolSprite->getChildByTag(eNodeGrid_tag)->runAction(Sequence::create(liquid1, liquid2, nullptr));
    }
}

void MakeIceCreamStep02::installAgitator1()
{
    // 组装搅拌机的容器
    m_pAgitator_down = Sprite::create("images/step/page4/parts/barrel_small_down.png");
    m_pAgitator_down->setPosition(490, 360 + 600);
    m_pMovableLayer->addChild(m_pAgitator_down, 5);
    
    m_pAgitator_up = Sprite::create("images/step/page4/parts/barrel_small_on.png");
    m_pAgitator_up->setPosition(490, 360 + 600);
    m_pMovableLayer->addChild(m_pAgitator_up, 15);
    
    SimpleAudioEngine::getInstance()->preloadEffect(iceCreamMachine);
    auto pAction = EaseExponentialOut::create(MoveBy::create(0.6f, Vec2(0, -600)));
    m_pAgitator_down->runAction(pAction);
    m_pAgitator_up->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
    
        // 组装搅拌器的转子
        m_pRotor = Sprite::create("images/step/page4/parts/barrel_gitator.png");
        m_pRotor->setPosition(490, 360 + 600);
        m_pMovableLayer->addChild(m_pRotor, 7);
        
        m_pRotor->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
        
            SoundPlayer::getInstance()->playEffect(iceCreamMachine);
        }), nullptr));
        m_pRotor->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0, -600)), CallFunc::create([=](){

            m_pMovableLayer->runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.8f, Vec2(-150, -30)), CallFunc::create([=](){
            
                // add icecream
                /////////////////////////////////////////////////////////
                // add bowl
                auto pBowl_down = ToolSprite::create("images/step/page3/bown2_down1.png");
                kAdapterScreen->setExactPosition(pBowl_down, 740 + 500, 280);
                pBowl_down->setDrag(true);
                pBowl_down->setDragBack(true);
                pBowl_down->setTag(eBowl_tag);
                this->addChild(pBowl_down, 10);
                pBowl_down->setMovebackTime(0.9f);
                pBowl_down->setScale(0.75f);
                
                auto pBowl_up = Sprite::create("images/step/page3/bown2_on.png");
                pBowl_up->setPosition(pBowl_down->getContentSize() / 2);
                pBowl_down->addChild(pBowl_up, 10);

                // 添加碗里的冰淇淋
                auto pNodeGrid = NodeGrid::create();
                m_pIcecream = Sprite::create("images/step/page3/batter1_4.png");
                m_pIcecream->setPosition(m_pIcecream->getContentSize() / 2);
                //这里注意是将精灵节点加到网格节点上
                pNodeGrid->addChild(m_pIcecream);
                pNodeGrid->setContentSize(m_pIcecream->getContentSize());
                pNodeGrid->ignoreAnchorPointForPosition(false);
                pNodeGrid->setAnchorPoint(Vec2(0.5f, 0.5f));
                pNodeGrid->setPosition(pBowl_down->getContentSize() / 2 + Size(5, 0));
                pNodeGrid->setTag(eNodeGrid_tag);
                pBowl_down->addChild(pNodeGrid, 9);
                
                //使用网格之前需要取消OpenGL的深度测试
                Director::getInstance()->setDepthTest(false);
                
                pBowl_down->runAction(Sequence::create(DelayTime::create(0.6f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-500, 0))), CallFunc::create([=](){
                    
                    pBowl_down->setOriginPoint(pBowl_down->getPosition());
                    pBowl_down->setDelegate(this);
                    
                    // 添加手指提示
                    m_pHandTip = ToolSprite::create("images/ui/fanger.png");
                    m_pHandTip->setAnchorPoint(Vec2(0, 1));
                    m_pHandTip->setPosition(pBowl_down->getPosition() + Vec2(0, 10));
                    this->addChild(m_pHandTip, INT16_MAX);
                    this->runHandTipAnimation(1);

                }), nullptr));
            }), nullptr));
 
        }), nullptr));
    }), nullptr));
}

void MakeIceCreamStep02::addIceCake()
{
    // 先将桶移动到屏幕中间
    auto pAction = MoveTo::create(0.8f, Vec2(visibleSize.width / 2, m_vSaveMoveLayerPos.y));
    m_pTable->runAction(pAction);
    m_pMovableLayer->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){

        // 初始化物理世界
        // 设置桶的内部边界
        Vec2 pos[9]={
            Vec2(-165, 158.5),
            Vec2(-133.9, 193.8),
            Vec2(-108.2, 178.7),
            Vec2(-94.3, -125.4),
            Vec2(6.9, -169.0),
            Vec2(112, -126.4),
            Vec2(123.7, 177.3),
            Vec2(147.5, 196.8),
            Vec2(185, 170)
        };
        auto body = PhysicsBody::createEdgeChain(pos, 9);
        auto edgeShape = Node::create();
        edgeShape->setPhysicsBody(body);
        body->setContactTestBitmask(0xFFFFFFFF);
        edgeShape->setPosition(m_pBarrel->getContentSize() / 2);
        m_pBarrel->addChild(edgeShape);
        
        // 在桶的外边缘设置一圈边界，防止冰块滚动后与桶的纹理相互嵌套
        Vec2 pos2[4]={
            Vec2(-130.3, -11.6),
            Vec2(-138.3, -194.0),
            Vec2(159.0, -195.8),
            Vec2(148.3, -8.1)
        };
        auto body2 = PhysicsBody::createEdgeChain(pos2, 4);
        auto edgeShape2 = Node::create();
        edgeShape2->setPhysicsBody(body2);
        body2->setContactTestBitmask(0xFFFFFFFF);
        edgeShape2->setPosition(m_pBarrel->getContentSize() / 2);
        m_pBarrel->addChild(edgeShape2);
        
        // 在桌子上添加一根线作为边界，防止个别遗漏的冰块掉出屏幕
        auto line = PhysicsBody::createEdgeSegment(Vec2(visibleOrigin.x, 100 + kAdapterScreen->g_oOffset.y), Vec2(visibleSize.width, 100 + kAdapterScreen->g_oOffset.y));
        auto edgeNode = Node::create();
        edgeNode->setPhysicsBody(line);
        line->setContactTestBitmask(0xFFFFFFFF);
        this->addChild(edgeNode);
   
        this->schedule(schedule_selector(MakeIceCreamStep02::createIceCake), 0.4f);
        
    }), nullptr));
}

void MakeIceCreamStep02::createIceCake(float ft)
{
    // 随机生成许多冰块，掉落在桶里
    int count = 0;
    while (1) {
        auto type = rand() % 2 + 1;
        auto posX = rand() % 320 - 160;
        auto zoder = rand() % 2;
        if (zoder == 0) {
            zoder = 4;
        }else{
            zoder = 16;
        }
        auto rotate = rand() % 360;
        auto total = rand() % 3 + 2;
        auto pSprite = Sprite::create(__String::createWithFormat("images/step/page4/parts/ice_%d.png", type)->getCString());
        pSprite->setPosition(490 + posX, 780 + rand() % 10);
        auto body = PhysicsBody::createBox(Size(13, 13), PhysicsMaterial(1.5f, 0.5f, 0.5f));
        pSprite->setPhysicsBody(body);
        pSprite->setTag(m_iIceCakeCount + 100);
        m_pMovableLayer->addChild(pSprite, zoder);
        pSprite->setRotation(rotate);
        
        count++;
        m_iIceCakeCount++;
        if (m_iIceCakeCount % 3 == 0) {
            pSprite->runAction(Repeat::create(RotateBy::create(1.5f + (rand() % 10) / 9.0f, 360), 2));
        }
        
        if (m_iIceCakeCount < 17) {
            if (m_iIceCakeCount % 3 == 0) {
                body->setContactTestBitmask(0xFFFFFFFF);
            }
        }
        if (m_iIceCakeCount % 17 == 0) {
            body->setContactTestBitmask(0xFFFFFFFF);
        }
        
        if (m_iIceCakeCount <= 100) {
            if (m_iIceCakeCount % 2 == 0) {
                addCoverEdge(true);
            }else{
                addCoverEdge(false);
            }
        }else{
            addCoverEdge(false);
            addCoverEdge(true);
        }

        if (count >= total) {
            break;
        }
    }
    
    if (m_iIceCakeCount >= 170) {
        this->unschedule(schedule_selector(MakeIceCreamStep02::createIceCake));
        this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=](){
        
            Director::getInstance()->getEventDispatcher()->removeEventListener(m_pListener);
            
            auto pSprite = Sprite::create("images/step/page4/barrel_bia_on.png");
            pSprite->setPosition(m_pBarrel_up->getPosition());
            m_pBarrel_up->getParent()->addChild(pSprite, m_pBarrel_up->getLocalZOrder() - 2);
            
            m_pBarrel_up_temp = Sprite::create("images/step/page4/barrel_bia_on.png");
            m_pBarrel_up_temp->setPosition(m_pBarrel_up->getPosition());
            m_pBarrel_up->getParent()->addChild(m_pBarrel_up_temp, m_pBarrel_up->getLocalZOrder() - 1);
            m_pBarrel_up_temp->setOpacity(0);
            
            // 继续组装搅拌机
            this->installAgitator2();
            
        }), nullptr));
    }
}

void MakeIceCreamStep02::installAgitator2()
{
    // 添加机器的盖子
    m_pLid = Sprite::create("images/step/page4/parts/lid_1.png");
    m_pLid->setPosition(480, 517 + 300);
    m_pMovableLayer->addChild(m_pLid, 22);
    
    m_pLid->runAction(Sequence::create(MoveBy::create(0.8f, Vec2(0, -300)), CallFunc::create([=](){
        addCoverEdge(false);
        
        // 将桶边缘的环扣上
        m_pRing->runAction(Sequence::create(Spawn::create(RotateBy::create(1.0f, 170), MoveBy::create(1.0f, Vec2(-7, -3)), Sequence::create(ScaleTo::create(0.7f, 1.2f), ScaleTo::create(0.3f, 1.0f), nullptr), nullptr), CallFunc::create([=](){
        
            m_pRing->setTexture("images/step/page4/parts/barrel_ring2.png");
            m_pRing->setAnchorPoint(Vec2(0.5f, 0));
            m_pRing->setRotation(0);
   
            // 添加摇杆手柄
            addHandle();
            
        }), nullptr));
    }), nullptr));
    
}

void MakeIceCreamStep02::addCoverEdge(bool isShow)
{
    if (isShow) {
        // 设置盖子的边界
        Vec2 pos1[6]={
            Vec2(-83.3, 114.4),
            Vec2(-62.5, 136.6),
            Vec2(2.3, 157.3),
            Vec2(61.9, 138.2),
            Vec2(81.7, 112.6),
            Vec2(-81.8, 114.2)
        };
        auto body1 = PhysicsBody::createEdgeChain(pos1, 6);
        auto edgeShape1 = Node::create();
        edgeShape1->setPhysicsBody(body1);
        body1->setContactTestBitmask(0xFFFFFFFF);
        edgeShape1->setPosition(m_pAgitator_down->getContentSize() / 2);
        edgeShape1->setTag(10);
        m_pAgitator_down->addChild(edgeShape1);
    }else{
        m_pAgitator_down->removeChildByTag(10);
    }
}

void MakeIceCreamStep02::addHandle()
{
    this->addProgress();
    ArmatureDataManager::getInstance()->addArmatureFileInfo("images/step/page4/parts/animation/NewAnimation.ExportJson");

    m_pHandle = Armature::create("NewAnimation");
    m_pHandle->ignoreAnchorPointForPosition(false);
    m_pHandle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pHandle, 727 + 500, 425);
    this->addChild(m_pHandle, 21);
    m_pHandle->getAnimation()->setIsPause(false);
    m_pHandle->runAction(Sequence::create(MoveBy::create(0.8f, Vec2(-500, 0)), CallFunc::create([=](){
    
        // 添加手指提示
        if (m_pHandTip) {
            m_pHandTip->setPosition(m_pHandle->getPosition() + Vec2(0, -30));
            this->runHandTipAnimation(2);
        }
        
        // 替摇杆注册touch事件
        auto pListener = EventListenerTouchOneByOne::create();
        pListener->setSwallowTouches(true);
        pListener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
            
            Vec2 touchPoint = pTouch->getLocation();
            Vec2 realPos = m_pHandle->getParent()->convertToNodeSpace(touchPoint);
            if (m_pHandle->getBoundingBox().containsPoint(realPos)){
                if (!m_pHandle->getAnimation()->isPlaying()) {
                    if (m_pHandle->getAnimation()->isPause()) {
                        m_pHandle->getAnimation()->resume();
                    }else{
                        m_pHandle->getAnimation()->play("Animation1");
                    }
                    
                    if (m_iMachineMixSoundId == -1) {
                        m_iMachineMixSoundId = SoundPlayer::getInstance()->playEffectLoop(MachineMixSound);
                    }

                    m_pProgressBg->runAction(FadeIn::create(0.1f));
                    m_pProgressTimer->runAction(FadeIn::create(0.1f));
                    m_pFlag->runAction(FadeIn::create(0.1f));
                    this->schedule(schedule_selector(MakeIceCreamStep02::makeIcecreamTime), 0.1f);
                    
                    // 背景飘雪
                    if (!m_pSnowParticle) {
                        m_pSnowParticle = ParticleSystemQuad::create("plists/snow.plist");
                        m_pSnowParticle->setPosition(Vec2(480, 640) + kAdapterScreen->g_oOffset);
                        this->addChild(m_pSnowParticle, 2);
                    }
                    
                    if (m_pHandTip) {
                        m_pHandTip->stopAllActions();
                        m_pHandTip->setVisible(false);
                    }
                }
                return true;
            }else{
                
                return false;
            }
        };
        pListener->onTouchMoved = [=](Touch *pTouch, Event *pEvent){
            
        };
        pListener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
            if (m_pHandle->getAnimation()->isPlaying()) {
                m_pHandle->getAnimation()->pause();

                this->unschedule(schedule_selector(MakeIceCreamStep02::makeIcecreamTime));
            }
            
            if (m_iMachineMixSoundId != -1) {
                SoundPlayer::getInstance()->stopEffectLoop(m_iMachineMixSoundId);
                m_iMachineMixSoundId = -1;
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, m_pHandle);
    }), nullptr));
}

void MakeIceCreamStep02::runHandTipAnimation(int step)
{
    if (m_pHandTip) {
        if (step == 1) {
            m_pHandTip->stopAllActions();
            m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
            m_pHandTip->setVisible(true);
            m_pHandTip->setOpacity(0);
            m_pHandTip->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                
                m_pHandTip->runAction(Repeat::create(Sequence::create(FadeIn::create(0.25f), MoveTo::create(1.0f, m_pBarrel->getPosition() + Vec2(-100, 120)), FadeOut::create(0.15f), CallFunc::create([=](){
                    
                    m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
                }), DelayTime::create(0.5f), nullptr), 2));
            }), nullptr));
        }else if(step == 2){
            m_pHandTip->setVisible(true);
            m_pHandTip->setOpacity(0);
            m_pHandTip->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                
                m_pHandTip->runAction(RepeatForever::create(Sequence::create(Sequence::create(FadeIn::create(0.25f), Repeat::create(Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), nullptr), 2), FadeOut::create(0.15f), nullptr), DelayTime::create(6.0f), nullptr)));
            }), nullptr));
        }
    }
}

void MakeIceCreamStep02::makeIcecreamTime(float ft)
{
    m_iTime++;
    
    if (m_pProgressTimer->getPercentage() <= 100) {
        m_pProgressTimer->setPercentage(m_pProgressTimer->getPercentage() + 100.0f / (AGITATION_COUNT_BASE * 4 + 25));
        m_pFlag->setPosition(m_pFlag->getPosition() + Vec2(0, 440.0f / (AGITATION_COUNT_BASE * 4 + 25)));
    }
    
    
    if (m_iTime % AGITATION_COUNT_BASE == 0) {

        m_pBarrel_up_temp->setTexture(__String::createWithFormat("images/step/page4/parts/barrel_bia_on%d.png", m_iTime / AGITATION_COUNT_BASE)->getCString());
        m_pBarrel_up_temp->runAction(FadeIn::create(1.0f));
        m_pBarrel_up->runAction(FadeOut::create(2.0f));
        
        Sprite* pTemp = m_pBarrel_up_temp;
        m_pBarrel_up_temp = m_pBarrel_up;
        m_pBarrel_up = pTemp;
    }
    
    // 添加桶上面的水珠
    if (m_iTime % 32 == 0) {
        addDrip();
    }
    
    for (Node* node : m_pMovableLayer->getChildren()) {
        if (node->getTag() >= 100 && m_pBarrel->getBoundingBox().containsPoint(node->getPosition())) {

            if (rand() % 5 == 0) {
                node->runAction(JumpBy::create(0.1f, Vec2::ZERO, 5, 1));
            }
        }
    }

    // 搅拌结束
    if (m_iTime == AGITATION_COUNT_BASE * 4 + 25) {
        this->getEventDispatcher()->removeEventListenersForTarget(m_pHandle);
        if (m_pHandle->getAnimation()->isPlaying()) {
            if (m_iMachineMixSoundId != -1) {
                SoundPlayer::getInstance()->stopEffectLoop(m_iMachineMixSoundId);
                m_iMachineMixSoundId = -1;
            }
            // 移除进度条
            auto pAction = Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr);
            m_pProgressBg->runAction(pAction);
            m_pProgressTimer->runAction(pAction->clone());
            m_pFlag->runAction(pAction->clone());
            
            // 停止搅拌
            this->unschedule(schedule_selector(MakeIceCreamStep02::makeIcecreamTime));
            m_pHandle->getAnimation()->gotoAndPause(0);
            
            // 移除物理世界所有的刚体
            this->getScene()->getPhysicsWorld()->setGravity(Vec2::ZERO);
            
            // 搅拌结束后，先将机器向下移动一段距离，腾出上方操作空间
            auto pAction2 = MoveBy::create(0.8f, Vec2(0, -150));
            m_pHandle->runAction(pAction2);
            m_pTable->runAction(pAction2->clone());
            m_pMovableLayer->runAction(Sequence::create(pAction2->clone(), CallFunc::create([=](){
                
                finishMix();
                m_pSnowParticle->stopSystem();
            }), nullptr));
        }
    }
}

void MakeIceCreamStep02::addDrip()
{
    auto total = rand() % 5 + 5;

    for (int i = 0; i < total; i++) {
        auto posX = rand() % 200 - 100;
        auto posY = rand() % 200 - 100;
        auto type = rand() % 4 + 1;
        auto time = rand() % 10 / 3.0f + 1.0f;
        
        auto pDrip = Sprite::create(__String::createWithFormat("images/step/page4/parts/water%d.png", type)->getCString());
        pDrip->setAnchorPoint(Vec2(0.5f, 1));
        pDrip->setPosition(m_pBarrel_up->getContentSize() / 2 + Size(posX, posY + 20));
        m_pBarrel_up->addChild(pDrip);
        pDrip->setOpacity(0);
        pDrip->setScale(0);
        pDrip->runAction(Sequence::create(Spawn::create(ScaleTo::create(time, 1.0f), FadeIn::create(time), nullptr), ScaleTo::create(1.0f, 1.0f, 1.3f), EaseSineInOut::create(MoveBy::create(3.5f, Vec2(0, -50))), nullptr));
    }
}

void MakeIceCreamStep02::finishMix()
{
    //////////////////////////////////////////////////////////
    // 打开盖子，取出制作好的冰淇淋
    // 先打开桶边缘紧扣的环
    m_pRing->runAction(Sequence::create(Spawn::create(RotateBy::create(1.0f, -170), MoveBy::create(1.0f, Vec2(0, 2)), Sequence::create(ScaleTo::create(0.7f, 1.2f), ScaleTo::create(0.3f, 1.0f), nullptr), nullptr), CallFunc::create([=](){

        m_pRing->setTexture("images/step/page4/parts/barrel_ring1.png");
        m_pRing->setAnchorPoint(Vec2(0.5f, 1));
        m_pRing->setRotation(0);
        
        // 移除手柄，打开机器盖子
        m_pHandle->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.6, Vec2(600, 0))), RemoveSelf::create(), nullptr));
        
        m_pLid->runAction(Sequence::create(DelayTime::create(0.2f), MoveBy::create(0.2f, Vec2(0, 30)), EaseExponentialIn::create(MoveBy::create(0.8f, Vec2(-800, 0))), CallFunc::create([=](){
            
            // 添加制作好的冰淇淋在桶里
            auto pIcecreamInAgitator = Sprite::create("images/step/page4/parts/ice_cream.png");
            pIcecreamInAgitator->setPosition(85, 255);
            m_pAgitator_up->addChild(pIcecreamInAgitator, -2);
            
            // 打开容器的盖子
            auto pAction = Sequence::create(MoveBy::create(0.3f, Vec2(0, 80)), EaseExponentialIn::create(MoveBy::create(0.7f, Vec2(700, 0))), RemoveSelf::create(), nullptr);
            m_pCover->runAction(pAction);
            m_pCover_on->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
                
                this->getScene()->getPhysicsWorld()->setGravity(Vec2(0, -9.8f));
                
                // 取出容器里面的转头
                m_pRotor->setTexture("images/step/page4/parts/barrel_gitator_1.png");
                m_pRotor->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 340)), CallFunc::create([=](){
                    m_pRotor->setLocalZOrder(m_pAgitator_up->getLocalZOrder() - 3);
                    
                    // 取出转头额，转头上留下的残液滴下
                    for (int i = 0; i < 5; i++) {
                        auto type = rand() % 2 + 1;
                        auto posX = rand() % 100 - 50;
                        auto posY = rand() % 60;
                        auto time = rand() % 5 / 10.0f;
                        auto pSprite = Sprite::create(__String::createWithFormat("images/step/page4/parts/batter_%d.png", type)->getCString());
                        pSprite->setPosition(m_pAgitator_up->getContentSize() / 2 + Size(posX, 220 + posY));
                        m_pAgitator_up->addChild(pSprite, -1);
                        
                        pSprite->runAction(Sequence::create(DelayTime::create(time), Spawn::create(MoveBy::create((posY + 150) / 100.0f, Vec2(0, -posY - 110)), Sequence::create(DelayTime::create((posY + 150) / 100.0f - 0.5f), ScaleTo::create(0.5f, 2.0f, 0.4f), nullptr), nullptr), FadeOut::create(1.0f), nullptr));
                    }
                    
                    // 取出制作好的冰淇淋
                    auto pAction = Sequence::create(DelayTime::create(4.0f), MoveBy::create(0.6f, Vec2(0, 70)), nullptr);
                    m_pAgitator_down->getChildByTag(eEdgeShape_tag)->runAction(pAction->reverse());
                    m_pAgitator_down->runAction(pAction);
                    m_pAgitator_up->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
                        
                        for (Node* node : m_pMovableLayer->getChildren()) {
                            if (node->getTag() >= 100 && m_pBarrel->getBoundingBox().containsPoint(node->getPosition())) {
                                
                                node->setScale(node->getScale() + 0.1f);
                            }
                        }

                        auto pAction = MoveBy::create(0.5f, Vec2(0, 150));
                        m_pTable->runAction(pAction);
                        m_pMovableLayer->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
                        
                            SoundPlayer::getInstance()->playEffect(decSound);
                            auto par = ParticleSystemQuad::create("plists/decorate.plist");
                            par->setPosition(visibleSize / 2 + Size(0, 60));
                            this->addChild(par, INT16_MAX);
                            par->setAutoRemoveOnFinish(true);
                            
                            this->setNextBtnIsVisible(true);
                        }), nullptr));
                    }), nullptr));
                    
                }), DelayTime::create(2.0f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(700, 0))), RemoveSelf::create(), nullptr));
                
            }), nullptr));
            
        }), RemoveSelf::create(), nullptr));
        
    }), nullptr));
}

void MakeIceCreamStep02::addProgress()
{
    //////////////////////////////////////////////////////////////
    // 添加进度条
    m_pProgressBg = Sprite::create("images/ui/bar_box.png");
    kAdapterScreen->setExactPosition(m_pProgressBg, 15, 320, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    m_pProgressBg->setTag(160);
    this->addChild(m_pProgressBg, 99);
    m_pProgressBg->setOpacity(0);
    
    // add progressbar
    m_pProgressTimer = ProgressTimer::create(Sprite::create("images/ui/bar2.png"));
    kAdapterScreen->setExactPosition(m_pProgressTimer, 15, 320, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    this->addChild(m_pProgressTimer, 100);
    m_pProgressTimer->setOpacity(0);
    m_pProgressTimer->setPercentage(0);
    m_pProgressTimer->setType(ProgressTimer::Type::BAR);
    m_pProgressTimer->setMidpoint(Point(0.5f, 0));
    m_pProgressTimer->setBarChangeRate(Point(0, 1));
    
    // add flag
    m_pFlag = Sprite::create("images/ui/bar2_1.png");
    kAdapterScreen->setExactPosition(m_pFlag, 5, 320 - 210, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    this->addChild(m_pFlag, 101);
    m_pFlag->setScale(0.8f);
    m_pFlag->setOpacity(0);
}

void MakeIceCreamStep02::onNextCallback()
{
    SceneManager::getInstance()->enterChooseScene();
}

void MakeIceCreamStep02::onEnter()
{
    BaseLayer::onEnter();
    
    // 监听物理碰撞
    m_pListener = EventListenerPhysicsContact::create();
    m_pListener->onContactBegin = [=](PhysicsContact& contact){
        // 取消碰撞监测
        contact.getShapeA()->setContactTestBitmask(0x00000000);
        if (rand() % 2 == 0) {
            SoundPlayer::getInstance()->playEffect(ice1Sound);
        }else{
            SoundPlayer::getInstance()->playEffect(ice2Sound);
        }
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_pListener,1);
}

void MakeIceCreamStep02::onExit()
{
    BaseLayer::onExit();
    
    if (m_iIceCreamSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iIceCreamSoundId);
        m_iIceCreamSoundId = -1;
    }
    
    if (m_iMachineMixSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iMachineMixSoundId);
        m_iMachineMixSoundId = -1;
    }
}