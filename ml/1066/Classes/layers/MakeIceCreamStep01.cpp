//
//  MakeIceCreamStep01.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#include "MakeIceCreamStep01.hpp"
#define MIXCOUNT_BASE2  50

enum Sprite_2_Tag {
    eMilkCup_tag,
    eSugerBowl_tag,
    eEgg1_tag,
    eEgg2_tag,
    eBlender_tag,
    eShadow_tag,
    eEgg1InBowl_tag,
    eEgg2InBowl_tag,
    eNodeGrid_tag
};

// 鸡蛋拖到碗里的时候放置到碗边的位置
Vec2 pos2[8] = {
    Vec2(150, 360), // 左
    Vec2(520, 360), // 右
    Vec2(330, 468), // 上
    Vec2(330, 260), // 下
    
    Vec2(205, 442), // 左上
    Vec2(452, 442), // 右上
    Vec2(205, 290), // 左下
    Vec2(452, 290), // 右下
};

MakeIceCreamStep01::MakeIceCreamStep01():m_pBg(nullptr),m_pBowl(nullptr),m_pBowl_up(nullptr),m_pMixLayer(nullptr),m_pMilk(nullptr),m_pSuger(nullptr),m_pEgg1(nullptr),m_pEgg2(nullptr),m_bEgg1AllowClick(false),m_bEgg2AllowClick(false),m_iAddEggCount(0),m_bIsMix(false),m_pStick(nullptr),m_iMixCount(0),m_pIngredient(nullptr),m_pIngredient_down(nullptr),m_iMilkSoundId(-1),m_iSugerSoundId(-1),m_iMixSoundId(-1)
{
    
}

MakeIceCreamStep01::~MakeIceCreamStep01()
{
    
}

Scene* MakeIceCreamStep01::scene()
{
    auto pScene = Scene::createWithPhysics();
//    pScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    pScene->getPhysicsWorld()->setGravity(Vec2(0, -240));
    auto pLayer = MakeIceCreamStep01::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool MakeIceCreamStep01::init()
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

bool MakeIceCreamStep01::initData()
{
    
    return true;
}

bool MakeIceCreamStep01::initLayer()
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
    auto pTable1 = Sprite::create("images/step/table_2.png");
    kAdapterScreen->setExactPosition(pTable1, 480, 69, Vec2(0.5f, 0));
    this->addChild(pTable1, 1);
    
    auto pTable2 = Sprite::create("images/step/table_1.png");
    kAdapterScreen->setExactPosition(pTable2, 480, 69, Vec2(0.5f, 1));
    this->addChild(pTable2, 2);
    
    // 播放进场动画
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(520 + kAdapterScreen->g_oOffset.x, 420 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(400 + kAdapterScreen->g_oOffset.x, 220 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(480 - 200 + kAdapterScreen->g_oOffset.x, 320 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(4.5f, oBezier);
    m_pBg->runAction(Sequence::create(DelayTime::create(0.6f), EaseSineOut::create(pBezierTo), nullptr));
    pTable1->runAction(Sequence::create(DelayTime::create(0.6f), ScaleTo::create(2.5f, 1.0f, 0.7f), ScaleTo::create(2, 1.0f), CallFunc::create([=](){

        /////////////////////////////////////////////////////////
        // add bowl
        m_pBowl = Sprite::create("images/step/page3/bown2_down1.png");
        kAdapterScreen->setExactPosition(m_pBowl, 330 - 650, 280);
        this->addChild(m_pBowl, 3);
        
        m_pBowl_up = Sprite::create("images/step/page3/bown2_on.png");
        kAdapterScreen->setExactPosition(m_pBowl_up, 330 - 650, 280);
        this->addChild(m_pBowl_up, 10);
        
        auto pAction = Sequence::create(EaseBackOut::create(MoveBy::create(1.1f, Vec2(650, 0))), nullptr);
        m_pBowl->runAction(pAction);
        m_pBowl_up->runAction(pAction->clone());
        
        
        // 添加搅拌层，所有材料均加到搅拌层上面统一管理
        m_pMixLayer = LayerColor::create(Color4B(0, 0, 0, 0));
        m_pMixLayer->setContentSize(m_pBowl->getContentSize());
        m_pMixLayer->ignoreAnchorPointForPosition(false);
        m_pMixLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        m_pMixLayer->setPosition(m_pBowl->getContentSize() / 2);
        m_pBowl->addChild(m_pMixLayer, 2);
        
        
        /////////////////////////////////////////////////////////
        auto pNodeGrid = NodeGrid::create();
        m_pIngredient = Sprite::create("images/step/page3/batter1_1.png");
        kAdapterScreen->setExactPosition(m_pIngredient, 330, 280);
        //这里注意是将精灵节点加到网格节点上
        pNodeGrid->addChild(m_pIngredient, 2);
        m_pIngredient_down = Sprite::create("images/step/page3/batter1_1.png");
        kAdapterScreen->setExactPosition(m_pIngredient_down, 330, 280);
        pNodeGrid->addChild(m_pIngredient_down);
        m_pIngredient_down->setOpacity(0);
        
        pNodeGrid->setTag(eNodeGrid_tag);
        this->addChild(pNodeGrid, 4);
        pNodeGrid->setVisible(false);
        
        //使用网格之前需要取消OpenGL的深度测试
        Director::getInstance()->setDepthTest(false);
        
        ///////////////
        addMilk();
    }), nullptr));

    return true;
}

void MakeIceCreamStep01::addMilk()
{
    // add milk cup
    auto pMilkCup = ToolSprite::create("images/step/page3/milk_2.png");
    kAdapterScreen->setExactPosition(pMilkCup, 750 + 500, 300);
    pMilkCup->setDrag(true);
    pMilkCup->setDragBack(true);
    pMilkCup->setTag(eMilkCup_tag);
    this->addChild(pMilkCup, 11);
    
    auto pMilkCup_up = Sprite::create("images/step/page3/milk_on.png");
    pMilkCup_up->setPosition(pMilkCup->getContentSize() / 2);
    pMilkCup->addChild(pMilkCup_up, 10);
    
    // add milk
    auto pNodeGrid = NodeGrid::create();
    m_pMilk = Sprite::create("images/step/page3/milk_1.png");
    m_pMilk->setPosition(m_pMilk->getContentSize() / 2);
    //这里注意是将精灵节点加到网格节点上
    pNodeGrid->addChild(m_pMilk);
    pNodeGrid->setContentSize(m_pMilk->getContentSize());
    pNodeGrid->ignoreAnchorPointForPosition(false);
    pNodeGrid->setAnchorPoint(Vec2(0.5f, 0.5f));
    pNodeGrid->setPosition(118, 182);
    pMilkCup->addChild(pNodeGrid, 5);

    //使用网格之前需要取消OpenGL的深度测试
    Director::getInstance()->setDepthTest(false);
    
    //液体效果,参数依次为：持续时间、网格大小、波纹数、振幅
    auto liquid = Liquid::create(4, Size(16, 12), 2, 2);
    pNodeGrid->runAction(liquid);
    
    pMilkCup->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-500, 0))), CallFunc::create([=](){
    
        pMilkCup->setOriginPoint(pMilkCup->getPosition());
        pMilkCup->addRectByNode(m_pBowl);
        pMilkCup->setDelegate(this);
    }), nullptr));
}

void MakeIceCreamStep01::addSuger()
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
    
    pBowl->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1.0f, Vec2(-500, 0))), CallFunc::create([=](){
        
        pBowl->setOriginPoint(pBowl->getPosition());
        pBowl->addRectByNode(m_pBowl);
        pBowl->setDelegate(this);
    }), nullptr));
}

void MakeIceCreamStep01::addEggs()
{
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
            
            // 移除物理世界中所有的刚体
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

void MakeIceCreamStep01::addBlender()
{
    // 将碗移到屏幕中间，然后添加搅拌机
    auto length = Vec2(visibleSize.width / 2 - m_pBowl->getPositionX(), visibleSize.height / 2 - 40 - m_pBowl->getPositionY());
    log("length.x======%f, length.y========%f", length.x, length.y);
    auto pAction = MoveBy::create(1.0f, length);
    m_pBowl->runAction(pAction);
    this->getChildByTag(eEgg1InBowl_tag)->runAction(pAction->clone());
    this->getChildByTag(eEgg2InBowl_tag)->runAction(pAction->clone());
    this->getChildByTag(eNodeGrid_tag)->runAction(pAction->clone());
    m_pBowl_up->runAction(Sequence::create(pAction->clone(), CallFunc::create([=](){
        
        // 添加搅拌机
        auto pBlender = ToolSprite::create("images/step/page1/blender.png");
        kAdapterScreen->setExactPosition(pBlender, 580 + 800, 560);
        pBlender->setDrag(true);
        pBlender->setTag(eBlender_tag);
        this->addChild(pBlender, 9);
        pBlender->setResponsePoint(Point(0.2f, 0.15f));
        pBlender->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(-800, 0)), MoveBy::create(0.8f, Vec2(0, -100)), CallFunc::create([=](){
            
            pBlender->addRectByNode(m_pBowl, 100, 157, -200, -230);
            pBlender->setMoveRectByNode(m_pBowl, 95, 152, -190, -230, true);
            pBlender->setDelegate(this);
        }), nullptr));
        
        // 添加搅拌机的棍子
        m_pStick = Sprite::create("images/step/page1/blender_1.png");
        m_pStick->setPosition(pBlender->getContentSize() / 2);
        pBlender->addChild(m_pStick, -1);
    }), nullptr));
}

void MakeIceCreamStep01::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag()) {
        case eMilkCup_tag:
        {
            toolSprite->setTexture("images/step/page3/milk_down.png");
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(false);
            toolSprite->stopAllActions();
            toolSprite->setPosition(toolSprite->getOriginPoint());
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

void MakeIceCreamStep01::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
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

void MakeIceCreamStep01::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    if (toolSprite->getTag() == eBlender_tag) {
        if (!m_bIsMix) {
            m_bIsMix = true;
            m_iMixSoundId = SoundPlayer::getInstance()->playEffectLoop(mixSound);
            
            toolSprite->setMoveRectByNode(m_pBowl, 100, 125, -200, -240, true);
            
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

void MakeIceCreamStep01::onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    switch (toolSprite->getTag()) {
        case eMilkCup_tag:
        {
            toolSprite->setSpriteEnabled(false);
            
            m_pMilk->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(0.6f, 1.1f, 1.0f), MoveBy::create(0.6f, Vec2(-20, 5)), RotateBy::create(0.6f, 0), nullptr) , nullptr));
            toolSprite->runAction(Sequence::create(MoveTo::create(0.2f, m_pBowl->getPosition() + Vec2(170, 205)), RotateBy::create(0.6f, -60), CallFunc::create([=](){
                
                toolSprite->setLocalZOrder(8);
                toolSprite->setAnchorPoint(Vec2(0.02f, 0.8f));
                toolSprite->setPosition(toolSprite->getPosition() + Vec2(-toolSprite->getContentSize().width * 0.48f, -toolSprite->getContentSize().height * 0.3f));
                
                // 倾斜牛奶杯子
                toolSprite->runAction(Sequence::create(Spawn::create(Sequence::create(MoveBy::create(1.8f, Vec2(-40, 15)), MoveBy::create(2.2f, Vec2(60, 15)), nullptr), RotateBy::create(4.0f, -15), nullptr), DelayTime::create(1.0f), EaseExponentialIn::create(MoveBy::create(0.6f, Vec2(650, 0))), CallFunc::create([=](){
                    
                    // add suger
                    this->addSuger();
                }), RemoveSelf::create(), nullptr));
                
                m_iMilkSoundId = SoundPlayer::getInstance()->playEffectLoop(waterSound);
                // 杯子里的牛奶减少
                m_pMilk->runAction(Sequence::create(Spawn::create(ScaleTo::create(4.0f, 1.0f, 0.4f), RotateBy::create(4.0f, 45), MoveBy::create(4.0f, Vec2(-30, -50)), nullptr), CallFunc::create([=](){
                
                    if (m_iMilkSoundId != -1) {
                        SoundPlayer::getInstance()->stopEffectLoop(m_iMilkSoundId);
                        m_iMilkSoundId = -1;
                    }
                }), RemoveSelf::create(), nullptr));

                // 添加倒牛奶的粒子动画
                auto pParticle = ParticleSystemQuad::create("plists/pourMilk.plist");
                kAdapterScreen->setExactPosition(pParticle, 394, 410);
                this->addChild(pParticle, 9);
                pParticle->setAutoRemoveOnFinish(true);
                pParticle->runAction(Sequence::create(MoveBy::create(1.8f, Vec2(-40, 15)), MoveBy::create(2.2f, Vec2(60, 15)), nullptr));
                
                // 添加碗里的牛奶
                auto pNodeGrid = NodeGrid::create();
                auto pMilkInBowl = Sprite::create("images/step/page3/milk.png");
                pMilkInBowl->setPosition(pMilkInBowl->getContentSize() / 2);
                //这里注意是将精灵节点加到网格节点上
                pNodeGrid->addChild(pMilkInBowl);
                pNodeGrid->setContentSize(pMilkInBowl->getContentSize());
                pNodeGrid->ignoreAnchorPointForPosition(false);
                pNodeGrid->setAnchorPoint(Vec2(0.5f, 0.5f));
                pNodeGrid->setPosition(m_pMixLayer->getContentSize() / 2 + Size(5, -80));
                m_pMixLayer->addChild(pNodeGrid);
                pNodeGrid->setScale(0);
                pNodeGrid->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(4.0f, 1.0f), MoveBy::create(4.0f, Vec2(0, 70)), nullptr), nullptr));
                
                //使用网格之前需要取消OpenGL的深度测试
                Director::getInstance()->setDepthTest(false);
                
                //液体效果,参数依次为：持续时间、网格大小、波纹数、振幅
//                auto liquid = Liquid::create(8, Size(16, 12), 4, 6);
                auto liquid1 = Liquid::create(4, Size(16, 12), 3, 4);
                auto liquid2 = Liquid::create(4, Size(16, 12), 2, 3);
                pNodeGrid->runAction(Sequence::create(liquid1, liquid2, nullptr));

            }), nullptr));
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->setSpriteEnabled(false);
            
            toolSprite->setAnchorPoint(Vec2(0.02f, 0.68f));
            toolSprite->setPosition(toolSprite->getPosition() + Vec2(-toolSprite->getContentSize().width * 0.48f, toolSprite->getContentSize().height * 0.18f));
            m_pSuger->runAction(Spawn::create(ScaleTo::create(0.4f, 0.9f, 1.0f), RotateBy::create(0.4f, 15), MoveBy::create(0.4f, Vec2(-12, 15)), nullptr));
            toolSprite->runAction(Sequence::create(Spawn::create(RotateBy::create(0.4f, -50), MoveTo::create(0.4f, m_pBowl->getPosition() + Vec2(68, 186)), nullptr), CallFunc::create([=](){
                
                // 倾斜碗，倒完糖之后移除碗
                toolSprite->runAction(Sequence::create(Spawn::create(Sequence::create(MoveBy::create(0.9f, Vec2(-40, 0)), MoveBy::create(1.1f, Vec2(60, 0)), nullptr), RotateBy::create(2.0f, -25), nullptr), EaseExponentialIn::create(MoveBy::create(0.9f, Vec2(650, 0))), CallFunc::create([=](){
                    
                    // add butter
                    this->addEggs();
                }), RemoveSelf::create(), nullptr));
                
                // 添加倒糖的音效
                this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=](){
                    
                    m_iSugerSoundId = SoundPlayer::getInstance()->playEffectLoop(sugerSound);
                }), nullptr));
                // 碗里的糖减少
                m_pSuger->runAction(Sequence::create(Spawn::create(ScaleTo::create(2.1f, 0), RotateBy::create(2.1f, 60), MoveBy::create(2.1f, Vec2(15, 50)), nullptr), CallFunc::create([=](){
                    
                    if (m_iSugerSoundId != -1) {
                        SoundPlayer::getInstance()->stopEffectLoop(m_iSugerSoundId);
                        m_iSugerSoundId = -1;
                    }
                }), RemoveSelf::create(), nullptr));
                
                // 添加倒糖的粒子动画
                auto pParticle = ParticleSystemQuad::create("plists/pourSuger.plist");
                pParticle->setPosition(toolSprite->getPosition() + Vec2(13, 0));
                this->addChild(pParticle, 20);
                pParticle->setAutoRemoveOnFinish(true);
                pParticle->runAction(Spawn::create(ScaleTo::create(2.0f, 0.5f, 1.0f), Sequence::create(MoveBy::create(0.9f, Vec2(-40, 0)), MoveBy::create(1.1f, Vec2(45, 0)), nullptr), nullptr));
                
                // 添加碗里的糖
                auto pSugerInBowl = Sprite::create("images/step/page1/bowl_sugar.png");
                pSugerInBowl->setPosition(m_pMixLayer->getContentSize() / 2 + Size(-30, -80));
                m_pMixLayer->addChild(pSugerInBowl);
                pSugerInBowl->setScale(0);
                pSugerInBowl->runAction(Sequence::create(DelayTime::create(0.2f), Spawn::create(ScaleTo::create(2.3f, 1.0f), MoveBy::create(2.3f, Vec2(0, 70)), nullptr), FadeTo::create(1.0f, 180), nullptr));
                
            }), nullptr));
        }
            break;
            
        case eEgg1_tag:
        {
            toolSprite->setSpriteEnabled(false);
            float length = 428;
            int index = 0;
            for (int i = 0; i < 8; i++) {
                if ((toolSprite->getPosition() - pos2[i]).length() < length) {
                    length = (toolSprite->getPosition() - pos2[i]).length();
                    index = i;
                }
            }
            toolSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.2f, pos2[index] + kAdapterScreen->g_oOffset), RotateTo::create(0.2f, 20), nullptr), CallFunc::create([=](){
                
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
                if ((toolSprite->getPosition() - pos2[i]).length() < length) {
                    length = (toolSprite->getPosition() - pos2[i]).length();
                    index = i;
                }
            }
            toolSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.2f, pos2[index] + kAdapterScreen->g_oOffset), RotateTo::create(0.2f, 0), nullptr), CallFunc::create([=](){
                
                toolSprite->setSpriteEnabled(true);
                toolSprite->setDrag(false);
                toolSprite->setDragBack(false);
                m_bEgg2AllowClick = true;
                
            }), nullptr));
        }
            break;
    }
}

void MakeIceCreamStep01::onMoveBackCallback(ToolSprite* toolSprite)
{
    switch (toolSprite->getTag()) {
        case eMilkCup_tag:
        {
            toolSprite->setTexture("images/step/page3/milk_2.png");
            
            //液体效果,参数依次为：持续时间、网格大小、波纹数、振幅
            auto liquid1 = Liquid::create(2.5f, Size(16, 12), 2, 3);
            auto liquid2 = Liquid::create(2, Size(16, 12), 1, 1);
            m_pMilk->getParent()->runAction(Sequence::create(liquid1, liquid2, nullptr));
        }
            break;
            
        case eSugerBowl_tag:
        {
            toolSprite->getChildByTag(eShadow_tag)->setVisible(true);
            toolSprite->runAction(JumpBy::create(0.5f, Vec2::ZERO, 15, 2));
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

void MakeIceCreamStep01::beatEgg1(ToolSprite* toolSprite)
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
            auto pEgg = Sprite::create("images/step/page3/egg1.png");
            pEgg->setPosition(m_pBowl->getPosition() + Vec2(-30, 210));
            pEgg->setTag(eEgg1InBowl_tag);
            this->addChild(pEgg, 8);
            pEgg->setScale(0);
            pEgg->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.4f, 0.5f, 0.6f), MoveBy::create(0.4f, Vec2(0, -10)), nullptr), Spawn::create(MoveBy::create(0.7f, Vec2(0, -190)), Sequence::create(ScaleTo::create(0.4f, 1.2f, 1.0f), ScaleTo::create(0.3f, 1.0f), nullptr), nullptr), FadeTo::create(0.5f, 100), CallFunc::create([=](){
                
                pEgg->setTexture("images/step/page3/egg1_1.png");
                pEgg->setOpacity(255);
                
                m_iAddEggCount++;
                if (m_iAddEggCount >= 2) {
                    // 添加搅拌机
                    this->addBlender();
                }
            }), nullptr));
            
        }), RemoveSelf::create(), nullptr));
    }
}

void MakeIceCreamStep01::beatEgg2(ToolSprite* toolSprite)
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
            auto pEgg = Sprite::create("images/step/page3/egg2.png");
            pEgg->setPosition(m_pBowl->getPosition() + Vec2(30, 210));
            pEgg->setTag(eEgg2InBowl_tag);
            this->addChild(pEgg, 8);
            pEgg->setScale(0);
            pEgg->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.4f, 0.5f, 0.6f), MoveBy::create(0.4f, Vec2(0, -10)), nullptr), Spawn::create(MoveBy::create(0.75f, Vec2(0, -210)), Sequence::create(ScaleTo::create(0.4f, 1.2f, 1.0f), ScaleTo::create(0.35f, 1.0f), nullptr), nullptr), FadeTo::create(0.5f, 100), CallFunc::create([=](){
                
                pEgg->setTexture("images/step/page3/egg2_1.png");
                pEgg->setOpacity(255);
                
                m_iAddEggCount++;
                if (m_iAddEggCount >= 2) {
                    // 添加搅拌机
                    this->addBlender();
                }
            }), nullptr));
            
        }), RemoveSelf::create(), nullptr));
    }
}

Action* MakeIceCreamStep01::getStickMixAnimation()
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

Action* MakeIceCreamStep01::getMachineShakeAnimation()
{
    auto pMove1 = MoveBy::create(0.01f, Vec2(-5, 2));
    auto pMove2 = MoveBy::create(0.02f, Vec2(10, -4));
    auto pSeq = Sequence::create(pMove1, pMove2, pMove1, CallFunc::create([=](){
        
        m_iMixCount++;
        if (m_iMixCount == 10) {
            // 改变碗里材料的纹理，先移除零碎的材料，再加上整体的纹理
            m_pMixLayer->removeAllChildrenWithCleanup(true);
            this->removeChildByTag(eEgg1InBowl_tag, true);
            this->removeChildByTag(eEgg2InBowl_tag, true);
            
            this->getChildByTag(eNodeGrid_tag)->setVisible(true);
            this->getChildByTag(eNodeGrid_tag)->runAction(RepeatForever::create(getIngredientAnimation()));
        }
        
        if (m_iMixCount % MIXCOUNT_BASE2 == 0) {
            switch (m_iMixCount / MIXCOUNT_BASE2) {
                case 1:
                {
                    Sprite* pTemp = nullptr;
                    m_pIngredient_down->setTexture("images/step/page3/batter1_2.png");
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
                    m_pIngredient_down->setTexture("images/step/page3/batter1_3.png");
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
                    m_pIngredient_down->setTexture("images/step/page3/batter1_4.png");
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
        if (m_iMixCount == MIXCOUNT_BASE2 * 3 + 10) {
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
            static_cast<ToolSprite*>(this->getChildByTag(eBlender_tag))->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0, 100)), EaseExponentialIn::create(MoveBy::create(0.7f, Vec2(-680, 0))), CallFunc::create([=](){
                
                this->setNextBtnIsVisible(true);
            }), RemoveSelf::create(), nullptr));
        }
    }), nullptr);
    auto pAction = RepeatForever::create(pSeq);
    
    return pAction;
}

ActionInterval* MakeIceCreamStep01::getIngredientAnimation()
{
    //波纹效果
    auto pAction = Ripple3D::create(3, Size(20, 16), Vec2(500 + kAdapterScreen->g_oOffset.x, 280 + kAdapterScreen->g_oOffset.x), 220, 6, 50);
    
    return pAction;
}

void MakeIceCreamStep01::onNextCallback()
{
    SceneManager::getInstance()->enterMakeIceCreamStep02Scene();
}

void MakeIceCreamStep01::onEnter()
{
    BaseLayer::onEnter();
}

void MakeIceCreamStep01::onExit()
{
    BaseLayer::onExit();
    
    if (m_iMilkSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iMilkSoundId);
        m_iMilkSoundId = -1;
    }
    
    if (m_iSugerSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iSugerSoundId);
        m_iSugerSoundId = -1;
    }
    
    if (m_iMixSoundId != -1) {
        SoundPlayer::getInstance()->stopEffectLoop(m_iMixSoundId);
        m_iMixSoundId = -1;
    }
}