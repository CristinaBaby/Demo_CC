//
//  MakeConeStep01.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#include "MakeConeStep01.hpp"
#include "GameData.h"

#define MIXCOUNT_BASE  50

enum Sprite_1_Tag{
    eFlourBag_tag,
    eSugerBowl_tag,
    eButterPlate_tag,
    eBlender_tag,
    eEgg1_tag,
    eEgg2_tag,
    eShadow_tag,
    eEgg1InBowl_tag,
    eEgg2InBowl_tag,
    eNodeGrid_tag,
    eBowlUp_tag
};

// 鸡蛋拖到碗里的时候放置到碗边的位置
Vec2 pos[8] = {
    Vec2(132, 370), // 左
    Vec2(528, 370), // 右
    Vec2(330, 478), // 上
    Vec2(330, 262), // 下
    
    Vec2(205, 452), // 左上
    Vec2(452, 452), // 右上
    Vec2(205, 285), // 左下
    Vec2(452, 285), // 右下
};

MakeConeStep01::MakeConeStep01():m_pBg(nullptr),m_pBowl(nullptr),m_pMixLayer(nullptr),m_pFlour(nullptr),m_pButterNode(nullptr),m_pSuger(nullptr),m_pEgg1(nullptr),m_pEgg2(nullptr),m_bEgg1AllowClick(false),m_bEgg2AllowClick(false),m_iAddEggCount(0),m_bIsMix(false),m_pStick(nullptr),m_iMixCount(0),m_pIngredient(nullptr),m_pIngredient_down(nullptr),m_iSugerSoundId(-1),m_iMixSoundId(-1),m_pHandTip(nullptr)
{

}

MakeConeStep01::~MakeConeStep01()
{

}

Scene* MakeConeStep01::scene()
{
    auto pScene = Scene::createWithPhysics();
//    pScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    pScene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto pLayer = MakeConeStep01::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool MakeConeStep01::init()
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
//        this->setNextBtnIsVisible(true);
        if (!g_bBgMusicType) {
            g_bBgMusicType = true;
            SoundPlayer::getInstance()->stopBackGroundMusic();
            SoundPlayer::getInstance()->playBackGroundMusic(bgMusick);
        }
        CC_BREAK_IF(!initLayer());
        
        isInit = true;
    }while(0);
    
    return isInit;
}

bool MakeConeStep01::initData()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    unsigned rand_seed = (unsigned)(now.tv_sec*1000 + now.tv_usec/1000);    //都转化为毫秒
    srand(rand_seed);

    return true;
}

bool MakeConeStep01::initLayer()
{
    // 由于背景图片过大，在部分android机型上无法加载，所以切分成4张图拼接而成
    m_pBg = Node::create();
    m_pBg->setContentSize(Size(2345, 1703));
    m_pBg->ignoreAnchorPointForPosition(false);
    m_pBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pBg, 480 + 200, 320 - 150);
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
    auto pTable1 = Sprite::create("images/step/table_2.png");
    kAdapterScreen->setExactPosition(pTable1, 480, 69, Vec2(0.5f, 0));
    this->addChild(pTable1, 1);
    
    auto pTable2 = Sprite::create("images/step/table_1.png");
    kAdapterScreen->setExactPosition(pTable2, 480, 69, Vec2(0.5f, 1));
    this->addChild(pTable2, 2);
    
    // 播放进场动画
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(180 + kAdapterScreen->g_oOffset.x, 420 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(980 + kAdapterScreen->g_oOffset.x, 220 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(480 + 600 + kAdapterScreen->g_oOffset.x, 320 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(3.5f, oBezier);
    m_pBg->runAction(Sequence::create(DelayTime::create(0.6f), MoveBy::create(2.0f, Vec2(0, 300)), pBezierTo, nullptr));
    pTable1->runAction(Sequence::create(DelayTime::create(0.6f), ScaleTo::create(2.0f, 1.0f, 0.7f), ScaleTo::create(3.5f, 1.0f), CallFunc::create([=](){
    
        /////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////
        // add bowl
        m_pBowl = Sprite::create("images/step/page1/bown_down.png");
        kAdapterScreen->setExactPosition(m_pBowl, 330 - 650, 280);
        this->addChild(m_pBowl, 3);
        
        auto pBowl_up = Sprite::create("images/step/page1/bowl_on.png");
        kAdapterScreen->setExactPosition(pBowl_up, 330 - 650, 280);
        pBowl_up->setTag(eBowlUp_tag);
        this->addChild(pBowl_up, 10);
        
        auto pAction = EaseBackOut::create(MoveBy::create(1.1f, Vec2(650, 0)));
        m_pBowl->runAction(pAction);
        pBowl_up->runAction(pAction->clone());
        
        
        // 添加搅拌层，所有材料均加到搅拌层上面统一管理(为简化逻辑，黄油和鸡蛋除外)
        m_pMixLayer = LayerColor::create(Color4B(0, 0, 0, 0));
        m_pMixLayer->setContentSize(m_pBowl->getContentSize());
        m_pMixLayer->ignoreAnchorPointForPosition(false);
        m_pMixLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_pMixLayer->setPosition(m_pBowl->getContentSize() / 2);
        m_pBowl->addChild(m_pMixLayer, 2);
        
        /////////////////////////////////////////////////////////
        auto pNodeGrid = NodeGrid::create();
        m_pIngredient = Sprite::create("images/step/page1/batter1.png");
        kAdapterScreen->setExactPosition(m_pIngredient, 330, 280);
        //这里注意是将精灵节点加到网格节点上
        pNodeGrid->addChild(m_pIngredient, 2);
        m_pIngredient_down = Sprite::create("images/step/page1/batter1.png");
        kAdapterScreen->setExactPosition(m_pIngredient_down, 330, 280);
        pNodeGrid->addChild(m_pIngredient_down);
        m_pIngredient_down->setOpacity(0);
        
        pNodeGrid->setTag(eNodeGrid_tag);
        this->addChild(pNodeGrid, 4);
        pNodeGrid->setVisible(false);
        
        //使用网格之前需要取消OpenGL的深度测试
        Director::getInstance()->setDepthTest(false);
        
        
        ////////////////////////////
        // 添加面粉袋子
        addFlour();
    
    }), nullptr));

    return true;
}

void MakeConeStep01::addFlour()
{
    // add flour
    auto pBag = ToolSprite::create("images/step/page1/flour_1.png");
    kAdapterScreen->setExactPosition(pBag, 750 + 460, 290);
    pBag->setDrag(true);
    pBag->setDragBack(true);
    pBag->setTag(eFlourBag_tag);
    this->addChild(pBag, 11);
    
    m_pFlour = Sprite::create("images/step/page1/flour.png");
    m_pFlour->setAnchorPoint(Vec2(0, 0));
    m_pFlour->setPosition(154 - m_pFlour->getContentSize().width / 2, 206 - m_pFlour->getContentSize().height / 2);
    pBag->addChild(m_pFlour);
    
    // 袋子的上层，为了遮住面粉而设计
    auto pBag_on = Sprite::create("images/step/page1/flour_on.png");
    pBag_on->setPosition(pBag->getContentSize() / 2);
    pBag->addChild(pBag_on, 10);
    
    // 添加阴影
    auto pBagShadow = Sprite::create("images/step/page1/flour_shadow.png");
    pBagShadow->setPosition(pBag->getContentSize() / 2);
    pBag->addChild(pBagShadow, -1);
    pBagShadow->setTag(eShadow_tag);
    
    pBag->runAction(Sequence::create(DelayTime::create(1.0f), EaseBackOut::create(MoveBy::create(1.1f, Vec2(-460, 0))), CallFunc::create([=](){
    
        pBag->setResponsePoint(Point(0.65f, 0.5f));
        pBag->addRectByNode(m_pBowl);
        pBag->setDelegate(this);
        
        pBag->setAnchorPoint(Vec2(0.5f, 0));
        pBag->setPosition(pBag->getPosition() + Vec2(0, -pBag->getContentSize().height * 0.5f));
        pBag->setOriginPoint(pBag->getPosition());
        pBag->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.2, 1.08, 0.8),  Spawn::create(ScaleTo::create(0.3, 0.95, 1.05), JumpBy::create(0.3f, Vec2(0, 0), 40, 1), nullptr), ScaleTo::create(0.1, 1.0), CallFunc::create([=](){
            
            // 添加手指提示
            m_pHandTip = ToolSprite::create("images/ui/fanger.png");
            m_pHandTip->setAnchorPoint(Vec2(0, 1));
            m_pHandTip->setPosition(pBag->getPosition() + Vec2(0, 85));
            this->addChild(m_pHandTip, INT16_MAX);
            this->runHandTipAnimation();
            
        }), nullptr));
    
    }), nullptr));

//    pBag->setAnchorPoint(Vec2(0, 0));
//    pBag->runAction(Sequence::create(Repeat::create(Sequence::create(ScaleTo::create(0.3f, 0.8f, 1.03f), CallFunc::create([=](){
//        
//        pBag->setAnchorPoint(Vec2(1, 0));
//        pBag->setPosition(pBag->getPosition() + Vec2(pBag->getContentSize().width * 0.8f, 0));
//    }), ScaleTo::create(0.3f, 1.1f, 0.97f), CallFunc::create([=](){
//        
//        pBag->setAnchorPoint(Vec2(0, 0));
//        pBag->setPosition(pBag->getPosition() + Vec2(-pBag->getContentSize().width * 1.1f, 0));
//    }), nullptr), 8), ScaleTo::create(0.2f, 1.0f), CallFunc::create([=](){
//    
//        pBag->setAnchorPoint(Vec2(0.5f, 0.5f));
//        pBag->setPosition(pBag->getPosition() + Vec2(pBag->getContentSize().width * 0.5f, pBag->getContentSize().height * 0.5f));
//        
//        pBag->setResponsePoint(Point(0.65f, 0.5f));
//        pBag->addRectByNode(m_pBowl);
//        pBag->setDelegate(this);
//
//        pBag->setAnchorPoint(Vec2(0.5f, 0));
//        pBag->setPosition(pBag->getPosition() + Vec2(0, -pBag->getContentSize().height * 0.5f));
//        pBag->setOriginPoint(pBag->getPosition());
//        pBag->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.2, 1.08, 0.8),  Spawn::create(ScaleTo::create(0.3, 0.95, 1.05), JumpBy::create(0.3f, Vec2(0, 0), 40, 1), nullptr), ScaleTo::create(0.1, 1.0), CallFunc::create([=](){
//            
//            // 添加手指提示
//            m_pHandTip = ToolSprite::create("images/ui/fanger.png");
//            m_pHandTip->setAnchorPoint(Vec2(0, 1));
//            m_pHandTip->setPosition(pBag->getPosition() + Vec2(0, 85));
//            this->addChild(m_pHandTip, INT16_MAX);
//            this->runHandTipAnimation();
//            
//        }), nullptr));
//
//    }), nullptr));
}

void MakeConeStep01::addSuger()
{
    // add suger
    auto pBowl = ToolSprite::create("images/step/page1/bowl_sugar_down.png");
    kAdapterScreen->setExactPosition(pBowl, 750 + 500, 300);
    pBowl->setDrag(true);
    pBowl->setDragBack(true);
    pBowl->setTag(eSugerBowl_tag);
    this->addChild(pBowl, 11);
    
    auto pBowl_on = Sprite::create("images/step/page1/bowl_sugar_on.png");
    pBowl_on->setPosition(pBowl->getContentSize() / 2);
    pBowl->addChild(pBowl_on, 10);
    
    // 添加阴影
    auto pBowlShadow = Sprite::create("images/step/page1/bowl_sugar_shadoe.png");
    pBowlShadow->setPosition(pBowl->getContentSize() / 2);
    pBowl->addChild(pBowlShadow, -1);
    pBowlShadow->setTag(eShadow_tag);
    
    m_pSuger = Sprite::create("images/step/page1/sugar_in_bowl.png");
    m_pSuger->setAnchorPoint(Vec2(0, 0.4f));
    m_pSuger->setPosition(120 - m_pSuger->getContentSize().width / 2, 70 - m_pSuger->getContentSize().height / 2 * 0.1f);
    pBowl->addChild(m_pSuger);
    
    // 向上曲线进场
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(1100 + kAdapterScreen->g_oOffset.x, 450 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(900 + kAdapterScreen->g_oOffset.x, 450 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(750 + kAdapterScreen->g_oOffset.x, 300 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(2.0f, oBezier);
    pBowl->runAction(Sequence::create(EaseBackOut::create(pBezierTo), CallFunc::create([=](){
    
        pBowl->setOriginPoint(pBowl->getPosition());
        pBowl->addRectByNode(m_pBowl);
        pBowl->setDelegate(this);
    }), nullptr));
}

void MakeConeStep01::addButter()
{
    // 添加黄油的盘子
    auto pPlate = ToolSprite::create("images/step/page1/bowl_butter_down.png");
    kAdapterScreen->setExactPosition(pPlate, 750 + 500, 280);
    pPlate->setDrag(true);
    pPlate->setDragBack(true);
    pPlate->setTag(eButterPlate_tag);
    this->addChild(pPlate, 11);
    
    auto pPlate_on = Sprite::create("images/step/page1/bowl_butter_on.png");
    pPlate_on->setPosition(pPlate->getContentSize() / 2);
    pPlate->addChild(pPlate_on, 10);
    
    // 添加阴影
    auto pPlateShadow = Sprite::create("images/step/page1/bowel_butter_shadow.png");
    pPlateShadow->setPosition(pPlate->getContentSize() / 2);
    pPlate->addChild(pPlateShadow, -1);
    pPlateShadow->setTag(eShadow_tag);
    
    // 添加butter
//    m_pButterNode = LayerColor::create(Color4B(100, 100, 100, 100));
    m_pButterNode = Node::create();
    m_pButterNode->setContentSize(pPlate->getContentSize() - Size(70, 40));
    m_pButterNode->ignoreAnchorPointForPosition(false);
    m_pButterNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    m_pButterNode->setPosition(pPlate->getContentSize() / 2 + Size(0, 20));
    pPlate->addChild(m_pButterNode);
    
    //////////////////////////////////////////////////////////
    // 设置黄油盘子的边界
    Vec2 pos[8]={
        Vec2(-128.9, 42.0),
        Vec2(-111.2, 2.9),
        Vec2(-86.3, -20.7),
        Vec2(-45.1, -32.1),
        Vec2(5.5, -33.2),
        Vec2(70.5, -26.1),
        Vec2(126.3, -5.2),
        Vec2(142.5, 22.6)
    };
    auto body = PhysicsBody::createEdgeChain(pos, 8);
    auto edgeShape = Node::create();
    edgeShape->setPhysicsBody(body);
    edgeShape->setPosition(pPlate->getContentSize() / 2);
    pPlate->addChild(edgeShape);
    
    int count = 0;
    while (1) {
        // 黄油颗粒类别
        int type = rand() % 2 + 1;
        // 坐标
        int width = (int)m_pButterNode->getContentSize().width;
        float posX = rand() % width;
        // 角度
        float rotate = rand() % 180;

        if (posX > (width - 25) || posX < 25) {
            continue;
        }
        float zOrder = 0;
        float posY = rand() % 10 + 19;
    
        bool isFlag = false;
        if (m_pButterNode->getChildren().size() > 0) {
            for (auto node : m_pButterNode->getChildren()) {
                if (fabs(node->getPositionX() - posX) <= 2) {
                    isFlag = true;
                }
            }
        }
        if (isFlag) {
            continue;
        }
        
        count++;
        auto pButter = ToolSprite::create(__String::createWithFormat("images/step/page1/butter%d_1.png", type)->getCString());
        pButter->setPosition(posX, posY);
        pButter->setFlag(type);
        auto body = PhysicsBody::createBox(Size(15, 15), PhysicsMaterial(1.5f, 0.5f, 0.5f));
        pButter->setPhysicsBody(body);
        body->setGravityEnable(false);
        m_pButterNode->addChild(pButter, zOrder);
        pButter->setRotation(rotate);
        pButter->setTag(count);
        
        if (count >= 25) {
            break;
        }
    }
 
    // 向下曲线进场
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(1100 + kAdapterScreen->g_oOffset.x, 100 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(900 + kAdapterScreen->g_oOffset.x, 100 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(750 + kAdapterScreen->g_oOffset.x, 280 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(2.0f, oBezier);
    pPlate->runAction(Sequence::create(EaseBackOut::create(pBezierTo), CallFunc::create([=](){
        
        pPlate->setOriginPoint(pPlate->getPosition());
        pPlate->addRectByNode(m_pBowl);
        pPlate->setDelegate(this);
        
    }), nullptr));
}

void MakeConeStep01::addEggs()
{
    // 清空物理世界所有的刚体
    this->getScene()->getPhysicsWorld()->removeAllBodies();
    
    // 第一个鸡蛋
    auto pEgg1 = ToolSprite::create("images/step/page1/egg_1.png");
    kAdapterScreen->setExactPosition(pEgg1, 960, 240);
    pEgg1->setDrag(true);
    pEgg1->setDragBack(true);
    pEgg1->setTag(eEgg1_tag);
    this->addChild(pEgg1, 11);
    pEgg1->setRotation(125);

    auto sha = PhysicsBody::createCircle(30, PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0, 10));
    pEgg1->setPhysicsBody(sha);
    sha->setVelocity(Vec2(-400,-150));
    sha->setLinearDamping(0.95f);
    
    // 在桌子上添加一根线作为鸡蛋滚动的边界
    auto line = PhysicsBody::createEdgeSegment(Vec2(visibleOrigin.x, 180 + kAdapterScreen->g_oOffset.y), Vec2(visibleSize.width, 180 + kAdapterScreen->g_oOffset.y));
    auto edgeNode = Node::create();
    edgeNode->setPhysicsBody(line);
    this->addChild(edgeNode);
    
    pEgg1->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){
        
        // 第二个鸡蛋
        auto pEgg2 = ToolSprite::create("images/step/page1/egg_2.png");
        kAdapterScreen->setExactPosition(pEgg2, 960, 240);
        pEgg2->setDrag(true);
        pEgg2->setDragBack(true);
        pEgg2->setTag(eEgg2_tag);
        this->addChild(pEgg2, 11);
        pEgg2->setRotation(100);
        
        auto sha2 = PhysicsBody::createCircle(30, PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0, 10));
        pEgg2->setPhysicsBody(sha2);
        sha2->setVelocity(Vec2(-400,-150));
        sha2->setLinearDamping(1.0f);
        
        
        pEgg2->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){
            
            this->getScene()->getPhysicsWorld()->removeAllBodies();
            
            pEgg1->setOriginPoint(pEgg1->getPosition());
            pEgg1->addRectByNode(m_pBowl);
            pEgg1->setDelegate(this);
            
            pEgg2->setOriginPoint(pEgg2->getPosition());
            pEgg2->addRectByNode(m_pBowl);
            pEgg2->setDelegate(this);
        }), nullptr));
    }), nullptr));
}

void MakeConeStep01::addBlender()
{
    // 将碗移到屏幕中间，然后添加搅拌机
    auto length = Vec2(visibleSize.width / 2 - m_pBowl->getPositionX(), visibleSize.height / 2 - 40 - m_pBowl->getPositionY());
    log("length.x======%f, length.y========%f", length.x, length.y);
    auto pAction = MoveBy::create(1.0f, length);
    m_pBowl->runAction(pAction);
    m_pButterNode->getParent()->runAction(pAction->clone());
    this->getChildByTag(eEgg1InBowl_tag)->runAction(pAction->clone());
    this->getChildByTag(eEgg2InBowl_tag)->runAction(pAction->clone());
    this->getChildByTag(eNodeGrid_tag)->runAction(pAction->clone());
    this->getChildByTag(eBowlUp_tag)->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
        
        // 添加搅拌机
        auto pBlender = ToolSprite::create("images/step/page1/blender.png");
        kAdapterScreen->setExactPosition(pBlender, 580 + 800, 560);
        pBlender->setDrag(true);
        pBlender->setTag(eBlender_tag);
        this->addChild(pBlender, 9);
        pBlender->setRotation(-10);
        pBlender->setResponsePoint(Point(0.2f, 0.15f));
        pBlender->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(-800, 0)), MoveBy::create(0.8f, Vec2(0, -100)), CallFunc::create([=](){
            
            pBlender->addRectByNode(m_pBowl, 100, 157, -200, -230);
            pBlender->setMoveRectByNode(m_pBowl, 100, 157, -200, -230, true);
            pBlender->setDelegate(this);
        }), nullptr));
        
        // 添加搅拌机的棍子
        m_pStick = Sprite::create("images/step/page1/blender_1.png");
        m_pStick->setPosition(pBlender->getContentSize() / 2);
        pBlender->addChild(m_pStick, -1);
    }), nullptr));
}

Action* MakeConeStep01::getStickMixAnimation()
{
    // 棍子搅拌的动画：帧动画
    auto pAnimation = Animation::create();
    for (int i = 2; i < 5; i++) {
        pAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/step/page1/blender_%d.png", i)->getCString());
    }
    pAnimation->setDelayPerUnit(0.05f);
    pAnimation->setRestoreOriginalFrame(true);
    
    auto pAnimate = Animate::create(pAnimation);
    
    auto pAction = RepeatForever::create(pAnimate);
    return pAction;
}

Action* MakeConeStep01::getMachineShakeAnimation()
{
    auto pMove1 = MoveBy::create(0.01f, Vec2(-5, 2));
    auto pMove2 = MoveBy::create(0.02f, Vec2(10, -4));
    auto pSeq = Sequence::create(pMove1, pMove2, pMove1, CallFunc::create([=](){
    
        m_iMixCount++;
        if (m_iMixCount == 10) {
            // 改变碗里材料的纹理，先移除零碎的材料，再加上整体的纹理
            m_pMixLayer->removeAllChildrenWithCleanup(true);
            m_pButterNode->removeAllChildrenWithCleanup(true);
            this->removeChildByTag(eEgg1InBowl_tag, true);
            this->removeChildByTag(eEgg2InBowl_tag, true);
            
            this->getChildByTag(eNodeGrid_tag)->setVisible(true);
            this->getChildByTag(eNodeGrid_tag)->runAction(RepeatForever::create(getIngredientAnimation()));
        }
        
        if (m_iMixCount % MIXCOUNT_BASE == 0) {
            switch (m_iMixCount / MIXCOUNT_BASE) {
                case 1:
                {
                    Sprite* pTemp = nullptr;
                    m_pIngredient_down->setTexture("images/step/page1/batter2.png");
                    m_pIngredient_down->runAction(FadeIn::create(1.0f));
                    m_pIngredient->runAction(FadeOut::create(1.0f));
                    
                    pTemp = m_pIngredient_down;
                    m_pIngredient_down = m_pIngredient;
                    m_pIngredient = pTemp;
                }
                    break;
                    
                case 2:
                {
                    Sprite* pTemp = nullptr;
                    m_pIngredient_down->setTexture("images/step/page1/batter3.png");
                    m_pIngredient_down->runAction(FadeIn::create(1.0f));
                    m_pIngredient->runAction(FadeOut::create(1.0f));
                    
                    pTemp = m_pIngredient_down;
                    m_pIngredient_down = m_pIngredient;
                    m_pIngredient = pTemp;
                }
                    break;
                    
                case 3:
                {
                    Sprite* pTemp = nullptr;
                    m_pIngredient_down->setTexture("images/step/page1/batter4.png");
                    m_pIngredient_down->runAction(FadeIn::create(1.0f));
                    m_pIngredient->runAction(FadeOut::create(1.0f));
                    
                    pTemp = m_pIngredient_down;
                    m_pIngredient_down = m_pIngredient;
                    m_pIngredient = pTemp;
                }
                    break;
                    
                case 4:
                {
                    Sprite* pTemp = nullptr;
                    m_pIngredient_down->setTexture("images/step/page1/batter5.png");
                    m_pIngredient_down->runAction(FadeIn::create(1.0f));
                    m_pIngredient->runAction(FadeOut::create(1.0f));
                    
                    pTemp = m_pIngredient_down;
                    m_pIngredient_down = m_pIngredient;
                    m_pIngredient = pTemp;
                }
                    break;
            }
        }
        
        // 搅拌结束
        if (m_iMixCount == MIXCOUNT_BASE * 4 + 10) {
            m_bIsMix = false;
            if (m_iMixSoundId != -1) {
                SoundPlayer::getInstance()->stopEffectLoop(m_iMixSoundId);
                m_iMixSoundId = -1;
            }
            
            static_cast<ToolSprite*>(this->getChildByTag(eBlender_tag))->setSpriteEnabled(false);
            m_pStick->stopAllActions();
            static_cast<ToolSprite*>(this->getChildByTag(eBlender_tag))->stopAllActions();
            if (m_pIngredient) {
                this->getChildByTag(eNodeGrid_tag)->stopAllActions();
                this->getChildByTag(eNodeGrid_tag)->runAction(getIngredientAnimation());
            }
            
            // 移除搅拌机，并显示下一步按钮
            m_pStick->setTexture("images/step/page1/blender_1.png");
            static_cast<ToolSprite*>(this->getChildByTag(eBlender_tag))->runAction(Sequence::create(MoveBy::create(0.7f, Vec2(0, 130)), EaseExponentialIn::create(MoveBy::create(0.7f, Vec2(-700, 0))), CallFunc::create([=](){
            
                this->setNextBtnIsVisible(true);
            }), RemoveSelf::create(), nullptr));
        }
    }), nullptr);
    auto pAction = RepeatForever::create(pSeq);
    
    return pAction;
}

ActionInterval* MakeConeStep01::getIngredientAnimation()
{
    //波纹效果
    auto pAction = Ripple3D::create(3, Size(20, 16), Vec2(500 + kAdapterScreen->g_oOffset.x, 280 + kAdapterScreen->g_oOffset.x), 220, 6, 50);
    
    return pAction;
}

void MakeConeStep01::runHandTipAnimation()
{
    if (m_pHandTip) {
        m_pHandTip->stopAllActions();
        m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
        m_pHandTip->setVisible(true);
        m_pHandTip->setOpacity(0);
        m_pHandTip->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
        
            m_pHandTip->runAction(Repeat::create(Sequence::create(FadeIn::create(0.25f), MoveTo::create(1.0f, m_pBowl->getPosition()), FadeOut::create(0.15f), CallFunc::create([=](){
                
                m_pHandTip->setPosition(m_pHandTip->getOriginPoint());
            }), DelayTime::create(0.5f), nullptr), 2));
        }), nullptr));
    }
}

void MakeConeStep01::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag()) {
        case eFlourBag_tag:
        {
            if (m_pHandTip) {
                m_pHandTip->stopAllActions();
                m_pHandTip->setVisible(false);
            }
            
            toolSprite->getChildByTag(eShadow_tag)->setVisible(false);
            toolSprite->stopAllActions();
            toolSprite->setPosition(toolSprite->getOriginPoint());
            toolSprite->setScale(1.0f);
            
            if (toolSprite->getAnchorPoint().y != 0.5f) {
                toolSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
                toolSprite->setPosition(toolSprite->getPosition() + Vec2(0, toolSprite->getContentSize().height * 0.5f));
                toolSprite->setOriginPoint(toolSprite->getPosition());
            }
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(false);
            toolSprite->stopAllActions();
            toolSprite->setPosition(toolSprite->getOriginPoint());
        }
            break;
            
        case eButterPlate_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(false);
            
            for (auto node : m_pButterNode->getChildren()) {
                static_cast<ToolSprite*>(node)->setTexture(__String::createWithFormat("images/step/page1/butter%d.png", static_cast<ToolSprite*>(node)->getFlag())->getCString());
            }
        }
            break;
            
        case eEgg1_tag:
        {
            toolSprite->setTexture("images/step/page1/egg_1_1.png");
            beatEgg1(toolSprite);
        }
            break;
        case eEgg2_tag:
        {
            toolSprite->setTexture("images/step/page1/egg_2_1.png");
            beatEgg2(toolSprite);
        }
            break;
    }
}

void MakeConeStep01::beatEgg1(ToolSprite* toolSprite)
{
    toolSprite->setLocalZOrder(20);
    
    // 打蛋
    if (m_bEgg1AllowClick) {
        toolSprite->setSpriteEnabled(false);
        toolSprite->runAction(Sequence::create(MoveBy::create(0.15f, Vec2(0, 30)), MoveBy::create(0.15f, Vec2(0, -30)), MoveBy::create(0.15f, Vec2(0, 30)), MoveBy::create(0.15f, Vec2(0, -30)), MoveTo::create(0.15f, m_pBowl->getPosition() + Vec2(-30, 210)), Hide::create(), CallFunc::create([=](){
            
            // 添加蛋壳,分左右两边蛋壳，每边蛋壳分两层
            auto pLeft = Sprite::create("images/step/page1/egg_shell_1down.png");
            pLeft->setPosition(m_pBowl->getPosition() + Vec2(-70, 210));
            this->addChild(pLeft, 7);
            pLeft->setRotation(20);
            
            auto pLeft_on = Sprite::create("images/step/page1/egg_shell_1on.png");
            pLeft_on->setPosition(m_pBowl->getPosition() + Vec2(-70, 210));
            this->addChild(pLeft_on, 9);
            pLeft_on->setRotation(20);
            
            // 左边蛋壳打开的动画
            auto pActionLeft = Sequence::create(Spawn::create(RotateBy::create(0.4f, 25), MoveBy::create(0.4f, Vec2(-45, 0)), nullptr), DelayTime::create(0.5f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(-500, 0))), RemoveSelf::create(), nullptr);
            pLeft->runAction(pActionLeft);
            pLeft_on->runAction(pActionLeft->clone());
            
            ////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////
            // 右边蛋壳
            auto pRight = Sprite::create("images/step/page1/egg_shell_2down.png");
            pRight->setPosition(m_pBowl->getPosition() + Vec2(10, 210));
            this->addChild(pRight, 7);
            pRight->setRotation(-20);
            
            auto pRight_on = Sprite::create("images/step/page1/egg_shell_2on.png");
            pRight_on->setPosition(m_pBowl->getPosition() + Vec2(10, 210));
            this->addChild(pRight_on, 9);
            pRight_on->setRotation(-20);
            
            // 右边蛋壳打开的动画
            auto pActionRight = Sequence::create(Spawn::create(RotateBy::create(0.4f, -25), MoveBy::create(0.4f, Vec2(45, 0)), nullptr), DelayTime::create(0.5f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(-500, 0))), RemoveSelf::create(), nullptr);
            pRight->runAction(pActionRight);
            pRight_on->runAction(pActionRight->clone());
            
            this->runAction(Sequence::create(DelayTime::create(0.7f), CallFunc::create([=](){
            
                SoundPlayer::getInstance()->playEffect(beatEggSound);
            }), nullptr));
            // 中间的鸡蛋
            auto pEgg = Sprite::create("images/step/page1/bowl_egg1.png");
            pEgg->setPosition(m_pBowl->getPosition() + Vec2(-30, 210));
            pEgg->setTag(eEgg1InBowl_tag);
            this->addChild(pEgg, 8);
            pEgg->setScale(0);
            pEgg->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.4f, 0.5f, 0.6f), MoveBy::create(0.4f, Vec2(0, -10)), nullptr), Spawn::create(MoveBy::create(0.7f, Vec2(0, -190)), Sequence::create(ScaleTo::create(0.5f, 1.2f, 1.0f), ScaleTo::create(0.2f, 1.0f), nullptr), nullptr), CallFunc::create([=](){
                
                m_iAddEggCount++;
                if (m_iAddEggCount >= 2) {
                    // 添加搅拌机
                    this->addBlender();
                }
            }), nullptr));
            
        }), RemoveSelf::create(), nullptr));
    }
}

void MakeConeStep01::beatEgg2(ToolSprite* toolSprite)
{
    toolSprite->setLocalZOrder(20);
    
    // 打蛋
    if (m_bEgg2AllowClick) {
        toolSprite->setSpriteEnabled(false);
        toolSprite->runAction(Sequence::create(MoveBy::create(0.15f, Vec2(0, 30)), MoveBy::create(0.15f, Vec2(0, -30)), MoveBy::create(0.15f, Vec2(0, 30)), MoveBy::create(0.15f, Vec2(0, -30)),MoveTo::create(0.15f, m_pBowl->getPosition() + Vec2(30, 210)), Hide::create(), CallFunc::create([=](){
            
            // 添加蛋壳,分左右两边蛋壳，每边蛋壳分两层
            auto pLeft = Sprite::create("images/step/page1/egg_shell_1down.png");
            pLeft->setPosition(m_pBowl->getPosition() + Vec2(-10, 210));
            this->addChild(pLeft, 7);
            pLeft->setRotation(20);
            
            auto pLeft_on = Sprite::create("images/step/page1/egg_shell_1on.png");
            pLeft_on->setPosition(m_pBowl->getPosition() + Vec2(-10, 210));
            this->addChild(pLeft_on, 9);
            pLeft_on->setRotation(20);
            
            // 左边蛋壳打开的动画
            auto pActionLeft = Sequence::create(Spawn::create(RotateBy::create(0.4f, 25), MoveBy::create(0.4f, Vec2(-45, 0)), nullptr), DelayTime::create(0.5f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(-500, 0))), RemoveSelf::create(), nullptr);
            pLeft->runAction(pActionLeft);
            pLeft_on->runAction(pActionLeft->clone());
            
            ////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////
            // 右边蛋壳
            auto pRight = Sprite::create("images/step/page1/egg_shell_2down.png");
            pRight->setPosition(m_pBowl->getPosition() + Vec2(70, 210));
            this->addChild(pRight, 7);
            pRight->setRotation(-20);
            
            auto pRight_on = Sprite::create("images/step/page1/egg_shell_2on.png");
            pRight_on->setPosition(m_pBowl->getPosition() + Vec2(70, 210));
            this->addChild(pRight_on, 9);
            pRight_on->setRotation(-20);
            
            // 右边蛋壳打开的动画
            auto pActionRight = Sequence::create(Spawn::create(RotateBy::create(0.4f, -25), MoveBy::create(0.4f, Vec2(45, 0)), nullptr), DelayTime::create(0.5f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(-500, 0))), RemoveSelf::create(), nullptr);
            pRight->runAction(pActionRight);
            pRight_on->runAction(pActionRight->clone());
            
            this->runAction(Sequence::create(DelayTime::create(0.7f), CallFunc::create([=](){
                
                SoundPlayer::getInstance()->playEffect(beatEggSound);
            }), nullptr));
            // 中间的鸡蛋
            auto pEgg = Sprite::create("images/step/page1/bowl_egg2.png");
            pEgg->setPosition(m_pBowl->getPosition() + Vec2(30, 210));
            pEgg->setTag(eEgg2InBowl_tag);
            this->addChild(pEgg, 8);
            pEgg->setScale(0);
            pEgg->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.4f, 0.5f, 0.6f), MoveBy::create(0.4f, Vec2(0, -10)), nullptr), Spawn::create(MoveBy::create(0.75f, Vec2(0, -210)), Sequence::create(ScaleTo::create(0.55f, 1.2f, 1.0f), ScaleTo::create(0.2f, 1.0f), nullptr), nullptr), CallFunc::create([=](){
                
                m_iAddEggCount++;
                if (m_iAddEggCount >= 2) {
                    // 添加搅拌机
                    this->addBlender();
                }
            }), nullptr));
            
        }), RemoveSelf::create(), nullptr));
    }
}

void MakeConeStep01::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eBlender_tag) {
        if (m_bIsMix) {
            m_bIsMix = false;
            if (m_iMixSoundId != -1) {
                SoundPlayer::getInstance()->stopEffectLoop(m_iMixSoundId);
                m_iMixSoundId = -1;
            }
            
            m_pStick->stopAllActions();
            
            toolSprite->stopAllActions();
            
            if (m_pIngredient) {
                this->getChildByTag(eNodeGrid_tag)->stopAllActions();
            }
        }
    }
}

void MakeConeStep01::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    
}

void MakeConeStep01::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    if (toolSprite->getTag() == eBlender_tag) {
        if (!m_bIsMix) {
            m_bIsMix = true;
            m_iMixSoundId = SoundPlayer::getInstance()->playEffectLoop(mixSound);
            
            toolSprite->setMoveRectByNode(m_pBowl, 95, 138, -220, -240, true);
            
            m_pStick->runAction(getStickMixAnimation());
            
            toolSprite->runAction(getMachineShakeAnimation());
            
            if (m_iMixCount >= 10) {
                if (m_pIngredient) {
                    this->getChildByTag(eNodeGrid_tag)->stopAllActions();
                    this->getChildByTag(eNodeGrid_tag)->runAction(RepeatForever::create(getIngredientAnimation()));
                }
            }
        }
    }
}

void MakeConeStep01::onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    switch (toolSprite->getTag()) {
        case eFlourBag_tag:
        {
            toolSprite->setSpriteEnabled(false);
            
            toolSprite->setAnchorPoint(Vec2(0.25f, 0.9f));
            toolSprite->setPosition(toolSprite->getPosition() + Vec2(-toolSprite->getContentSize().width * 0.25f, toolSprite->getContentSize().height * 0.4f));
            toolSprite->runAction(Sequence::create(Spawn::create(RotateBy::create(0.4f, -60), MoveTo::create(0.4f, m_pBowl->getPosition() + Vec2(90, 180)), nullptr), CallFunc::create([=](){
            
                // 倾斜面粉袋，倒完糖之后移除袋子
                toolSprite->runAction(Sequence::create(Spawn::create(Sequence::create(MoveBy::create(1.2f, Vec2(-40, 0)), MoveBy::create(1.3f, Vec2(60, 0)), nullptr), RotateBy::create(2.5f, -15), nullptr), EaseExponentialIn::create(MoveBy::create(0.9f, Vec2(650, 0))), CallFunc::create([=](){
                
                    // add suger
                    this->addSuger();
                }), RemoveSelf::create(), nullptr));
                
                m_iSugerSoundId = SoundPlayer::getInstance()->playEffectLoop(sugerSound);
                // 袋子里的面粉减少
                m_pFlour->runAction(Sequence::create(Spawn::create(ScaleTo::create(2.7f, 0, 0.2f), RotateBy::create(2.7f, 45), MoveBy::create(2.7f, Vec2(-10, 20)), nullptr), CallFunc::create([=](){
                
                    SoundPlayer::getInstance()->stopEffectLoop(m_iSugerSoundId);
                    m_iSugerSoundId = -1;
                }), RemoveSelf::create(), nullptr));

                // 添加倒面粉的粒子动画
                auto pParticle = ParticleSystemQuad::create("plists/pourFlour.plist");
                pParticle->setPosition(toolSprite->getPosition() + Vec2(5, 0));
                this->addChild(pParticle, 20);
                pParticle->setAutoRemoveOnFinish(true);
                pParticle->runAction(Spawn::create(ScaleTo::create(2.5f, 0.6f, 1.0f), Sequence::create(MoveBy::create(1.2f, Vec2(-40, 0)), MoveBy::create(1.3f, Vec2(60, 0)), nullptr), nullptr));
                
                // 添加碗里的面粉
                auto pFlourInBowl = Sprite::create("images/step/page1/bowl_flour.png");
                pFlourInBowl->setPosition(m_pMixLayer->getContentSize() / 2 + Size(0, -80));
                m_pMixLayer->addChild(pFlourInBowl);
                pFlourInBowl->setScale(0);
                pFlourInBowl->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(2.6f, 1.0f), MoveBy::create(2.6f, Vec2(0, 60)), nullptr), nullptr));
            }), nullptr));
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->setSpriteEnabled(false);
            
            toolSprite->setAnchorPoint(Vec2(0.02f, 0.68f));
            toolSprite->setPosition(toolSprite->getPosition() + Vec2(-toolSprite->getContentSize().width * 0.48f, toolSprite->getContentSize().height * 0.18f));
            m_pSuger->runAction(Spawn::create(ScaleTo::create(0.4f, 0.9f, 1.0f), RotateBy::create(0.4f, 15), MoveBy::create(0.4f, Vec2(-12, 15)), nullptr));
            toolSprite->runAction(Sequence::create(Spawn::create(RotateBy::create(0.4f, -50), MoveTo::create(0.4f, m_pBowl->getPosition() + Vec2(60, 150)), nullptr), CallFunc::create([=](){
                
                // 倾斜碗，倒完糖之后移除碗
                toolSprite->runAction(Sequence::create(Spawn::create(Sequence::create(MoveBy::create(0.9f, Vec2(-40, 0)), MoveBy::create(1.1f, Vec2(60, 0)), nullptr), RotateBy::create(2.0f, -25), nullptr), EaseExponentialIn::create(MoveBy::create(0.9f, Vec2(650, 0))), CallFunc::create([=](){
                    
                    // add butter
                    this->addButter();
                }), RemoveSelf::create(), nullptr));
                
                // 添加倒糖的音效
                this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=](){
                
                    m_iSugerSoundId = SoundPlayer::getInstance()->playEffectLoop(sugerSound);
                }), nullptr));
                // 碗里的糖减少
                m_pSuger->runAction(Sequence::create(Spawn::create(ScaleTo::create(2.1f, 0), RotateBy::create(2.1f, 60), MoveBy::create(2.1f, Vec2(15, 50)), nullptr), CallFunc::create([=](){
                
                    SoundPlayer::getInstance()->stopEffectLoop(m_iSugerSoundId);
                    m_iSugerSoundId = -1;
                }), RemoveSelf::create(), nullptr));
                
                // 添加倒糖的粒子动画
                auto pParticle = ParticleSystemQuad::create("plists/pourSuger.plist");
                pParticle->setPosition(toolSprite->getPosition() + Vec2(13, 0));
                this->addChild(pParticle, 20);
                pParticle->setAutoRemoveOnFinish(true);
                pParticle->runAction(Spawn::create(ScaleTo::create(2.0f, 0.5f, 1.0f), Sequence::create(MoveBy::create(0.9f, Vec2(-40, 0)), MoveBy::create(1.1f, Vec2(45, 0)), nullptr), nullptr));
                
                // 添加碗里的糖
                auto pSugerInBowl = Sprite::create("images/step/page1/bowl_sugar.png");
                pSugerInBowl->setPosition(m_pMixLayer->getContentSize() / 2 + Size(-30, -90));
                m_pMixLayer->addChild(pSugerInBowl);
                pSugerInBowl->setScale(0);
                pSugerInBowl->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(2.3f, 1.0f), MoveBy::create(2.3f, Vec2(0, 60)), nullptr), FadeTo::create(1.0f, 220), nullptr));
                
            }), nullptr));
        }
            break;
            
        case eButterPlate_tag:
        {
            toolSprite->setSpriteEnabled(false);

            toolSprite->runAction(Sequence::create(MoveTo::create(0.2f, m_pBowl->getPosition() + Vec2(150, 200)), RotateBy::create(0.4f, -35), CallFunc::create([=](){
                
                toolSprite->setLocalZOrder(7);
                this->addPhysicsEdge();
  
            }), RotateBy::create(0.4f, -48), MoveBy::create(1.0f, Vec2(-40, 10)), MoveBy::create(1.5f, Vec2(60, 0)), CallFunc::create([=](){
                
                auto pNode = Node::create();
                pNode->setContentSize(toolSprite->getContentSize());
                pNode->ignoreAnchorPointForPosition(false);
                pNode->setAnchorPoint(toolSprite->getAnchorPoint());
                pNode->setPosition(toolSprite->getPosition());
                pNode->setRotation(toolSprite->getRotation());
                this->addChild(pNode, toolSprite->getLocalZOrder());
                
                // 将黄油颗粒从盘子中移除
                m_pButterNode->retain();
                m_pButterNode->removeFromParentAndCleanup(false);
                pNode->addChild(m_pButterNode, 2);
                m_pButterNode->release();
                
            }), DelayTime::create(0.8f), MoveBy::create(0.2f, Vec2(0, 36)), EaseExponentialIn::create(MoveBy::create(0.9f, Vec2(900, 0))), CallFunc::create([=](){
                
                // add egg
                this->addEggs();
            }), RemoveSelf::create(), nullptr));
        }
            break;
            
        case eEgg1_tag:
        {
            toolSprite->setSpriteEnabled(false);
            float length = 428;
            int index = 0;
            for (int i = 0; i < 8; i++) {
                if ((toolSprite->getPosition() - pos[i]).length() < length) {
                    length = (toolSprite->getPosition() - pos[i]).length();
                    index = i;
                }
            }
            toolSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.2f, pos[index] + kAdapterScreen->g_oOffset), RotateTo::create(0.2f, 20), nullptr), CallFunc::create([=](){
            
                toolSprite->setSpriteEnabled(true);
                toolSprite->setDrag(false);
                toolSprite->setDragBack(false);
                m_bEgg1AllowClick = true;

            }), nullptr));
        }
            break;
            
        case eEgg2_tag:
        {
            toolSprite->setSpriteEnabled(false);
            float length = 428;
            int index = 0;
            for (int i = 0; i < 8; i++) {
                if ((toolSprite->getPosition() - pos[i]).length() < length) {
                    length = (toolSprite->getPosition() - pos[i]).length();
                    index = i;
                }
            }
            toolSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.2f, pos[index] + kAdapterScreen->g_oOffset), RotateTo::create(0.2f, 0), nullptr), CallFunc::create([=](){
                
                toolSprite->setSpriteEnabled(true);
                toolSprite->setDrag(false);
                toolSprite->setDragBack(false);
                m_bEgg2AllowClick = true;
                
            }), nullptr));
        }
            break;
    }
}

void MakeConeStep01::onMoveBackCallback(ToolSprite* toolSprite)
{
    switch (toolSprite->getTag()) {
        case eFlourBag_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(true);
            toolSprite->runAction(JumpBy::create(0.5f, Vec2::ZERO, 15, 2));
            this->runHandTipAnimation();
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(true);
            toolSprite->runAction(JumpBy::create(0.5f, Vec2::ZERO, 15, 2));
        }
            break;
            
        case eButterPlate_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(true);
            toolSprite->runAction(JumpBy::create(0.5f, Vec2::ZERO, 15, 2));
            
            for (auto node : m_pButterNode->getChildren()) {
                static_cast<ToolSprite*>(node)->setTexture(__String::createWithFormat("images/step/page1/butter%d_1.png", static_cast<ToolSprite*>(node)->getFlag())->getCString());
            }
        }
            break;
            
        case eEgg1_tag:
        {
            toolSprite->setLocalZOrder(11);
            toolSprite->setTexture("images/step/page1/egg_1.png");
        }
            break;
        case eEgg2_tag:
        {
            toolSprite->setLocalZOrder(11);
            toolSprite->setTexture("images/step/page1/egg_2.png");
        }
            break;
    }
}

void MakeConeStep01::addPhysicsEdge()
{
    for (auto node : m_pButterNode->getChildren()) {
        node->getPhysicsBody()->setGravityEnable(true);
    }
    
    // 设置碗的边界
    Vec2 pos[9]={
        Vec2(-186.2, 47.5),
        Vec2(-182.3, 14.7),
        Vec2(-158.0, -14.4),
        Vec2(-105.3, -45.1),
        Vec2(-30.2, -60.7),
        Vec2(45.8, -59.0),
        Vec2(130.7, -31.5),
        Vec2(181.4, 13.5),
        Vec2(188.3, 54.7)
    };
    auto body = PhysicsBody::createEdgeChain(pos, 9);
    auto edgeShape = Node::create();
    edgeShape->setPhysicsBody(body);
    edgeShape->setPosition(m_pBowl->getContentSize() / 2);
    m_pBowl->addChild(edgeShape);

    // 在桌子上添加一根线作为边界，防止个别遗漏的刚体掉出屏幕
    auto line = PhysicsBody::createEdgeSegment(Vec2(visibleOrigin.x, 100 + kAdapterScreen->g_oOffset.y), Vec2(visibleSize.width, 100 + kAdapterScreen->g_oOffset.y));
    auto edgeNode = Node::create();
    edgeNode->setPhysicsBody(line);
    this->addChild(edgeNode);
    
    this->getScene()->getPhysicsWorld()->setGravity(Vec2(0, -240));
}

void MakeConeStep01::onNextCallback()
{
    SceneManager::getInstance()->enterMakeConeStep02Scene();
}

void MakeConeStep01::onEnter()
{
    BaseLayer::onEnter();
}

void MakeConeStep01::onExit()
{
    BaseLayer::onExit();
    
    if (m_iSugerSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iSugerSoundId);
        m_iSugerSoundId = -1;
    }
    if (m_iMixSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iMixSoundId);
        m_iMixSoundId = -1;
    }
}