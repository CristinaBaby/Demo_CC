//
//  MakeConeStep02.cpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#include "MakeConeStep02.hpp"
#include "GameData.h"

enum MakeConeStep02_Tag{
    eSpoon_tag,
    eRoll_tag,
    eBatter_tag,
    eBigLight_tag
};

MakeConeStep02::MakeConeStep02():m_pBg(nullptr),m_pMachine(nullptr),m_pCover(nullptr),m_pBowl(nullptr),m_pBatter(nullptr),m_pBatterInSpoon(nullptr),m_bRollAllowClick(false),m_pBigLight(nullptr),m_iMachineWorkSoundId(-1),m_pHandTip(nullptr),m_pTable1(nullptr),m_pTable2(nullptr),m_pMoveArea(nullptr)
{
    
}

MakeConeStep02::~MakeConeStep02()
{
    
}

Scene* MakeConeStep02::scene()
{
    auto pScene = Scene::create();
    auto pLayer = MakeConeStep02::create();
    pScene->addChild(pLayer);
    
    return pScene;
}

bool MakeConeStep02::init()
{
    bool isInit = false;
    do{
        CC_BREAK_IF(!BaseLayer::init());
        CC_BREAK_IF(!initData());
//        this->setNextBtnIsVisible(true);
        CC_BREAK_IF(!initLayer());
        
        isInit = true;
    }while(0);
    
    return isInit;
}

bool MakeConeStep02::initData()
{
    
    return true;
}

bool MakeConeStep02::initLayer()
{
    m_pMoveArea = Node::create();
    m_pMoveArea->setContentSize(Size(1136, 768));
    m_pMoveArea->ignoreAnchorPointForPosition(false);
    m_pMoveArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    kAdapterScreen->setExactPosition(m_pMoveArea, 480, 320);
    this->addChild(m_pMoveArea);
    
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
    m_pTable1 = Sprite::create("images/step/table_2.png");
    kAdapterScreen->setExactPosition(m_pTable1, 480, 69, Vec2(0.5f, 0));
    this->addChild(m_pTable1, 1);
    
    m_pTable2 = Sprite::create("images/step/table_1.png");
    kAdapterScreen->setExactPosition(m_pTable2, 480, 69, Vec2(0.5f, 1));
    this->addChild(m_pTable2, 2);
    
    // 播放进场动画
    ccBezierConfig oBezier;
    oBezier.controlPoint_1 = Vec2(920 + kAdapterScreen->g_oOffset.x, 420 + kAdapterScreen->g_oOffset.y);
    oBezier.controlPoint_2 = Vec2(800 + kAdapterScreen->g_oOffset.x, 220 + kAdapterScreen->g_oOffset.y);
    oBezier.endPosition = Vec2(480 + 200 + kAdapterScreen->g_oOffset.x, 320 + kAdapterScreen->g_oOffset.y);
    auto pBezierTo = BezierTo::create(4.5f, oBezier);
    m_pBg->runAction(Sequence::create(DelayTime::create(0.6f), EaseSineOut::create(pBezierTo), nullptr));
    m_pTable1->runAction(Sequence::create(DelayTime::create(0.6f), ScaleTo::create(2.5f, 1.0f, 0.7f), ScaleTo::create(2, 1.0f), CallFunc::create([=](){
        
        //////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////
        // add machine
        m_pMachine = Sprite::create("images/step/page2/machine_1.png");
        kAdapterScreen->setExactPosition(m_pMachine, 300 - 600, 390);
        this->addChild(m_pMachine, 3);
        
        // 在烤炉上设置一个响应区域
        auto pMachineResponseArea = Node::create();
        pMachineResponseArea->setContentSize(Size(160, 100));
        pMachineResponseArea->ignoreAnchorPointForPosition(false);
        kAdapterScreen->setExactPosition(pMachineResponseArea, 325, 290);
        this->addChild(pMachineResponseArea, 4);
        
        // add cover
        m_pCover = Sprite::create("images/step/page2/machine_off.png");
        m_pCover->setPosition(m_pMachine->getContentSize() / 2);
        m_pMachine->addChild(m_pCover, 4);
        
        // 添加盖子上的指示灯
        m_pBigLight = Sprite::create("images/step/page2/light1_off.png");
        m_pBigLight->setPosition(170, 242);
        m_pCover->addChild(m_pBigLight);
        
        for (int i = 0; i < 3; i++) {
            auto pSmallLight = Sprite::create("images/step/page2/light2_off.png");
            pSmallLight->setPosition(200 + i * 25, 242);
            pSmallLight->setTag(100 + i);
            m_pCover->addChild(pSmallLight);
        }
        
        m_pMachine->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1.0f, Vec2(600, 0))), nullptr));
        
        /////////////////////////////////////////////////////////
        // add bowl
        m_pBowl = Sprite::create("images/step/page1/bown_down.png");
        kAdapterScreen->setExactPosition(m_pBowl, 750 + 600, 260);
        this->addChild(m_pBowl, 3);
        m_pBowl->setScale(0.83f);
        
        auto pBowl_up = Sprite::create("images/step/page1/bowl_on.png");
        pBowl_up->setPosition(m_pBowl->getContentSize() / 2);
        m_pBowl->addChild(pBowl_up, 3);
        
        // add batter(面糊)
        m_pBatter = Sprite::create("images/step/page1/batter5.png");
        m_pBatter->setPosition(m_pBowl->getContentSize() / 2);
        m_pBowl->addChild(m_pBatter, 2);
        
        // 在碗上设置一个响应区域（碗的资源周围有透明区域，如果用做响应区域，不够精确）
        auto pBowlResponseArea = Node::create();
        pBowlResponseArea->setContentSize(Size(200, 100));
        pBowlResponseArea->ignoreAnchorPointForPosition(false);
        kAdapterScreen->setExactPosition(pBowlResponseArea, 750, 270);
        this->addChild(pBowlResponseArea, 4);
        
        m_pBowl->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-600, 0))), CallFunc::create([=](){
            
            // 打开烤炉盖子
            m_pCover->setTexture("images/step/page2/machine_on.png");
            m_pCover->setLocalZOrder(-1);
            m_pCover->setScaleY(0.7f);
            m_pCover->runAction(ScaleTo::create(0.4f, 1.0f));
            
            // add spoon
            auto pSpoon = ToolSprite::create("images/step/page2/spoon.png");
            kAdapterScreen->setExactPosition(pSpoon, 600 + 800, 460);
            pSpoon->setDrag(true);
            pSpoon->setTag(eSpoon_tag);
            this->addChild(pSpoon, 10);
            pSpoon->setResponsePoint(Point(0.12f, 0.1f));
            
            pSpoon->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-800, 0))), CallFunc::create([=](){
                
                pSpoon->setMoveRectByNode(m_pMoveArea, 80, 160, -290, -300);
                pSpoon->addRectByNode(pBowlResponseArea);
                pSpoon->addRectByNode(pMachineResponseArea);
                pSpoon->setDelegate(this);
                
                // 添加勺子上的面糊，初始化时隐藏
                m_pBatterInSpoon = Sprite::create("images/step/page2/batter.png");
                m_pBatterInSpoon->setPosition(50, 48);
                pSpoon->addChild(m_pBatterInSpoon, 2);
                m_pBatterInSpoon->setVisible(false);
            }), nullptr));
            
        }), nullptr));
    }), nullptr));

    return true;
}

void MakeConeStep02::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eRoll_tag) {
        if (m_bRollAllowClick) {
            toolSprite->setSpriteEnabled(false);
            if (m_pHandTip) {
                m_pHandTip->stopAllActions();
                m_pHandTip->setVisible(false);
            }
            
            toolSprite->setVisible(false);
            static_cast<Sprite*>(m_pMachine->getChildByTag(eBatter_tag))->setTexture("images/step/page2/cakw2_1.png");
            
            // 卷蛋筒的动画
            auto pAnimation = Animation::create();
            for (int i = 3; i < 6; i++) {
                pAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/step/page2/cake%d.png", i)->getCString());
            }
            pAnimation->setRestoreOriginalFrame(false);
            pAnimation->setDelayPerUnit(0.7f);
            auto pAnimate = Animate::create(pAnimation);
            m_pMachine->getChildByTag(eBatter_tag)->runAction(Sequence::create(pAnimate, CallFunc::create([=](){
                
                // 移除桌子
                auto pAction = Sequence::create(MoveBy::create(0.6f, Vec2(0, -500)), RemoveSelf::create(), nullptr);
                m_pTable1->runAction(pAction);
                m_pTable2->runAction(pAction->clone());
                
                // 展示做好的蛋筒
                auto pCake = Sprite::create("images/dec/decCone/wafer_cone.png");
                pCake->setPosition(m_pMachine->getPosition() - m_pMachine->getContentSize() / 2 + m_pMachine->getChildByTag(eBatter_tag)->getPosition());
                this->addChild(pCake, 4);
                pCake->setScale(0.7f);
                pCake->setRotation(108);
                pCake->runAction(Sequence::create(Spawn::create(MoveTo::create(0.8f, visibleSize / 2), RotateBy::create(0.8f, -108), ScaleTo::create(0.5f, 1.0f), nullptr), CallFunc::create([=](){
                
                    SoundPlayer::getInstance()->playEffect(decSound);
                    auto par = ParticleSystemQuad::create("plists/decorate.plist");
                    par->setPosition(pCake->getContentSize() / 2 + Size(0, 35));
                    pCake->addChild(par, 100);
                    par->setAutoRemoveOnFinish(true);
                    
                }), nullptr));
            }), Hide::create(), CallFunc::create([=](){
            
                // 移除烤炉
                m_pMachine->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.5f, Vec2(-600, 0))), CallFunc::create([=](){
                
                    // 显示下一步按钮
                    this->setNextBtnIsVisible(true);
                }), RemoveSelf::create(), nullptr));
            }), nullptr));
        }
    }
}

void MakeConeStep02::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
}

void MakeConeStep02::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    log("index==========%d", index);
    if (toolSprite->getTag() == eSpoon_tag) {
        if (index == 0) {
            // 舀出碗里的面糊
            if (!m_pBatterInSpoon->isVisible()) {
                m_pBatterInSpoon->setScale(0);
                m_pBatterInSpoon->setVisible(true);
                SoundPlayer::getInstance()->playEffect(addSound);
                
                m_pBatter->runAction(Spawn::create(ScaleTo::create(0.2f, 0.92f), MoveBy::create(0.2f, Vec2(0, -10)), nullptr));
                m_pBatterInSpoon->runAction(ScaleTo::create(0.2f, 1.0f));
            }
        }else {
            // 将面糊倒在烤炉里
            if (m_pBatterInSpoon->isVisible()) {
                m_pBatterInSpoon->runAction(Sequence::create(ScaleTo::create(0.2f, 0), RemoveSelf::create(), nullptr));
                toolSprite->setSpriteEnabled(false);

                // 将面糊添加到烤炉里
                auto pBatterInMachine = Sprite::create("images/step/page2/machine_batter.png");
                pBatterInMachine->setPosition(214, 162);
                pBatterInMachine->setTag(eBatter_tag);
                m_pMachine->addChild(pBatterInMachine);
                pBatterInMachine->setScale(0);
                pBatterInMachine->runAction(ScaleTo::create(0.2f, 1.0f));
                SoundPlayer::getInstance()->playEffect(addSound);
                
                // 移除勺子
                toolSprite->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.8f, Vec2(800, 0))), CallFunc::create([=](){
                
                    // 盖上烤炉盖子
                    m_pCover->runAction(Sequence::create(ScaleTo::create(0.4f, 1.0f, 0.7f), CallFunc::create([=](){
                    
                        m_pCover->setLocalZOrder(4);
                        m_pCover->setTexture("images/step/page2/machine_off.png");
                        m_pCover->setScale(1.0f);
                    }), nullptr));
                    
                    
                    // 移除空碗，并把烤炉移到屏幕中间
                    m_pBowl->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.5f, Vec2(600, 0))), RemoveSelf::create(), nullptr));
                    
                    m_pMachine->runAction(Sequence::create(EaseExponentialIn::create(MoveTo::create(0.6f, visibleSize / 2 + Size(-80, 65))), DelayTime::create(0.2f), CallFunc::create([=](){
         
                        this->addProgress();
                        
                        m_iMachineWorkSoundId = SoundPlayer::getInstance()->playEffectLoop(workSound);
                        this->bakeCake(pBatterInMachine);
                    
                    }), nullptr));
                }), RemoveSelf::create(), nullptr));
            }
        }
    }else if (toolSprite->getTag() == eRoll_tag){
        toolSprite->setSpriteEnabled(false);

        toolSprite->runAction(Sequence::create(MoveTo::create(0.2f, m_pMachine->getPosition() + Vec2(40, -100)), CallFunc::create([=](){
            
            toolSprite->setTexture("images/step/page2/cake_rolle.png");
            toolSprite->setContentSize(Size(297, 205));
            m_bRollAllowClick = true;
            toolSprite->setSpriteEnabled(true);
            toolSprite->setDrag(false);
            
            // 添加手指提示
            m_pHandTip = ToolSprite::create("images/ui/fanger.png");
            m_pHandTip->setAnchorPoint(Vec2(0, 1));
            m_pHandTip->setPosition(toolSprite->getPosition() + Vec2(0, 0));
            this->addChild(m_pHandTip, INT16_MAX);
            this->runHandTipAnimation();

        }), nullptr));
    };
}

void MakeConeStep02::runHandTipAnimation()
{
    if (m_pHandTip) {
        m_pHandTip->setVisible(true);
        m_pHandTip->setOpacity(0);
        m_pHandTip->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
            
            m_pHandTip->runAction(RepeatForever::create(Sequence::create(Sequence::create(FadeIn::create(0.25f), Repeat::create(Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), nullptr), 2), FadeOut::create(0.15f), nullptr), DelayTime::create(6.0f), nullptr)));
        }), nullptr));
    }
}

void MakeConeStep02::onTouchUpInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
}

void MakeConeStep02::bakeCake(Sprite* pBatterInMachine)
{
    m_pCover->runAction(Sequence::create(Repeat::create(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
    
        static_cast<Sprite*>(m_pCover->getChildByTag(100))->setTexture("images/step/page2/light2_on.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(101))->setTexture("images/step/page2/light2_off.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(102))->setTexture("images/step/page2/light2_off.png");
        
    }), DelayTime::create(0.5f), CallFunc::create([=](){
    
        static_cast<Sprite*>(m_pCover->getChildByTag(100))->setTexture("images/step/page2/light2_off.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(101))->setTexture("images/step/page2/light2_on.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(102))->setTexture("images/step/page2/light2_off.png");
        
    }), DelayTime::create(0.5f), CallFunc::create([=](){
        
        static_cast<Sprite*>(m_pCover->getChildByTag(100))->setTexture("images/step/page2/light2_off.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(101))->setTexture("images/step/page2/light2_off.png");
        static_cast<Sprite*>(m_pCover->getChildByTag(102))->setTexture("images/step/page2/light2_on.png");
        
    }), nullptr), 7), DelayTime::create(0.5f), CallFunc::create([=](){
    
        static_cast<Sprite*>(m_pCover->getChildByTag(102))->setTexture("images/step/page2/light3_on.png");
    }), DelayTime::create(0.5f), CallFunc::create([=](){
    
        static_cast<Sprite*>(m_pCover->getChildByTag(101))->setTexture("images/step/page2/light3_on.png");
    }), DelayTime::create(0.5f), CallFunc::create([=](){
        
        static_cast<Sprite*>(m_pCover->getChildByTag(100))->setTexture("images/step/page2/light3_on.png");
    }), DelayTime::create(0.5f), CallFunc::create([=](){
        
        m_pBigLight->setTexture("images/step/page2/light1_on.png");
    }), DelayTime::create(1.0f), CallFunc::create([=](){

        // 烘烤完毕
        if (m_iMachineWorkSoundId != -1) {
            SoundPlayer::getInstance()->playEffect(doneSound);
            SoundPlayer::getInstance()->stopEffectLoop(m_iMachineWorkSoundId);
            m_iMachineWorkSoundId = -1;
        }
        pBatterInMachine->setTexture("images/step/page2/cake1.png");
        // 打开烤炉盖子
        m_pCover->setTexture("images/step/page2/machine_on.png");
        m_pCover->setLocalZOrder(-1);
        m_pCover->setScaleY(0.7f);
        m_pCover->runAction(ScaleTo::create(0.4f, 1.0f));
        
    }), DelayTime::create(1.0f), CallFunc::create([=](){
        
        pBatterInMachine->setTexture("images/step/page2/cakw2.png");
        pBatterInMachine->setScale(0.98f);
        pBatterInMachine->runAction(Spawn::create(MoveBy::create(0.06f, Vec2(0, 5)), ScaleTo::create(0.06f, 1.0f), nullptr));
        
        // add roll
        auto pRoll = ToolSprite::create("images/step/page2/rolle_1.png");
        kAdapterScreen->setExactPosition(pRoll, 750 + 600, 280);
        pRoll->setTag(eRoll_tag);
        pRoll->setDrag(true);
        this->addChild(pRoll, 20);
        pRoll->runAction(Sequence::create(DelayTime::create(0.8f), EaseBackOut::create(MoveBy::create(1.0f, Vec2(-600, 0))), CallFunc::create([=](){
            
            pRoll->setMoveRectByNode(m_pMoveArea, 110, 200, -220, -300, true);
            pRoll->addRectByNode(pBatterInMachine);
            pRoll->setDelegate(this);
        }), nullptr));
        
    }), nullptr));
}

void MakeConeStep02::addProgress()
{
    //////////////////////////////////////////////////////////////
    // 添加进度条
    auto pProgressBg = Sprite::create("images/ui/bar_box.png");
    kAdapterScreen->setExactPosition(pProgressBg, 15, 320, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    pProgressBg->setTag(160);
    this->addChild(pProgressBg, 99);
    pProgressBg->setOpacity(0);
    pProgressBg->runAction(FadeIn::create(0.5f));
    
    // add progressbar
    auto pProgressTimer = ProgressTimer::create(Sprite::create("images/ui/bar1.png"));
    kAdapterScreen->setExactPosition(pProgressTimer, 15, 320, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    this->addChild(pProgressTimer, 100);
    pProgressTimer->setPercentage(0);
    pProgressTimer->setType(ProgressTimer::Type::BAR);
    pProgressTimer->setMidpoint(Point(0.5f, 0));
    pProgressTimer->setBarChangeRate(Point(0, 1));
    pProgressTimer->runAction(Sequence::create(DelayTime::create(0.5f), ProgressFromTo::create(12, 0, 100), nullptr));
    
    // add flag
    auto pFlag = Sprite::create("images/ui/bar1_1.png");
    kAdapterScreen->setExactPosition(pFlag, 10, 320 - 225, Vec2(0, 0.5f), kBorderLeft, kBorderNone);
    this->addChild(pFlag, 101);
    pFlag->setScale(0.8f);
    pFlag->setOpacity(0);
    pFlag->runAction(Sequence::create(FadeIn::create(0.5f), MoveBy::create(11.7f, Vec2(0, 455)), DelayTime::create(1.0f), CallFunc::create([=](){
        
        auto pAction = Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr);
        pProgressBg->runAction(pAction);
        pProgressTimer->runAction(pAction->clone());
        pFlag->runAction(pAction->clone());
    }), nullptr));
}

void MakeConeStep02::onNextCallback()
{
    SceneManager::getInstance()->enterDecorationConeScene();
}

void MakeConeStep02::onEnter()
{
    BaseLayer::onEnter();
}

void MakeConeStep02::onExit()
{
    BaseLayer::onExit();
}