//
//  CreamScene.cpp
//  CCATS016_CakeMaker
//
//  Created by wusonglin1 on 15/9/21.
//
//

#include "CreamScene.h"
#include "MainScene.h"
#include "BezierCreator.h"
REGIST_SECENE(CreamScene);
CreamScene::CreamScene()
{
    oldPos = Vec2::ZERO;
    m_bPaint=false;
    m_bShowHand=true;
    m_bMakeOver = false;
    m_pWhiskCover = nullptr;
    m_pHand = nullptr;
}
CreamScene::~CreamScene()
{
    
}
bool CreamScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
    
}
void CreamScene::onEnter()
{
    GameBaseScene::onEnter();
}
void CreamScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
    MaterialComing();
}
void CreamScene::onExit()
{
    GameBaseScene::onExit();
}
void CreamScene::initUI()
{
    m_pBG = Sprite::create(g_path + "cream/cream_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
    
    m_pStands = Sprite::create(g_path + "cream/stands.png");
    m_pStands->setPosition(Vec2(m_pBG->getContentSize().width/2 - 100,564 - 40));
    m_pBG->addChild(m_pStands);
    
//    g_choose_waste = "blueberry";

    auto cake = Sprite::create(__String::createWithFormat("%scream/cake_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
    cake->setPosition(Vec2(244,350 + 50));
    cake->setAnchorPoint(Vec2(0.5,0.5));
    m_pStands->addChild(cake);
    
    auto bowl = Sprite::create(g_path + "cream/cream_bowl0.png");
    bowl->setPosition(Vec2(549 + 1000,363));
    bowl->setTag(101);
    m_pBG->addChild(bowl);

    auto bowl_cream = Sprite::create(__String::createWithFormat("%scream/bowl_cream_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
    bowl_cream->setTag(101);
    bowl_cream->setAnchorPoint(Vec2::ZERO);
    bowl->addChild(bowl_cream);
    
    auto _bowl = Sprite::create(g_path + "cream/cream_bowl1.png");
    _bowl->setAnchorPoint(Vec2::ZERO);
    bowl->addChild(_bowl);

    m_pCakePaint = MyPaintSprite::create(361,222);
//    m_pCakePaint->setAnchorPoint(Vec2(0.5,0.5));
    
    m_pCakePaint->useTarget(__String::createWithFormat("%scream/cream_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
    m_pCakePaint->useBrush(g_path + "cream/scribble.png");
//    this->addToContentLayer(m_pCakePaint);
//    m_pCakePaint->setPosition(m_pStands->convertToWorldSpace(cake->getPosition()));
    m_pCakePaint->setPosition(cake->getContentSize()/2);
    cake->addChild(m_pCakePaint);
    
    m_pStands->setPositionX(m_pStands->getPositionX() - 1000);
    
    
    Rect rect = Rect(bowl->getPositionX(),bowl->getPositionY(),240,34);
    
    m_pWhisk = ToolSprite::create((g_path + "cream/cream_whisk0.png").c_str());
    m_pWhisk->setDelegate(this);
    kAdapterScreen->setExactPosition(m_pWhisk, 0 + 50 - 500,680,Vec2(1,1),kBorderRight,kBorderNone);
    this->addToContentLayer(m_pWhisk);
    m_pWhisk->setResponsePoint(Vec2(0.31,0.24));
    
    m_pWhisk->cancelEvent(true);
}
void CreamScene::MaterialComing()
{

    ActionUtils::moveByElasticOut(m_pStands, Vec2(1000,0));
    
    AudioHelp::getInstance()->playEffect("slide.mp3");
    auto func = CallFunc::create([=](){
    
        MaterialComing1();
    
    });
    
    ActionUtils::moveByElasticOut(m_pBG->getChildByTag(101), Vec2(-1000,0),1.0,func);
}
void CreamScene::MaterialComing1()
{
    AudioHelp::getInstance()->playEffect("swish.mp3");
    auto func = CallFunc::create([=](){
        
        m_pWhisk->cancelEvent(false);
        m_pWhisk->addRectByNode(m_pBG->getChildByTag(101));
        
        showHandOne();
    });
    ActionUtils::moveByElasticOut(m_pWhisk, Vec2(-500,0),1.0,func);
}
void CreamScene::showHand()
{
    auto hand = Sprite::create("cocostudio/ui/tips/finger_1.png");
    hand->setPosition(Vec2(178,420));
    hand->setAnchorPoint(Vec2(0.14,0.86));
    m_pStands->addChild(hand);

    MoveBy* move1 = MoveBy::create(0.25, Vec2(50,-50));
    MoveBy* move2 = MoveBy::create(0.25, Vec2(50,50));
    MoveBy* move3 = MoveBy::create(0.25, Vec2(50,-50));
    MoveBy* move4 = MoveBy::create(0.25, Vec2(50,50));
    
    hand->runAction(Sequence::create(move1,move2,move3,move4,RemoveSelf::create(), NULL));

}
void CreamScene::showHandOne()
{
    if(!m_pHand){
    
        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(m_pWhisk->getPosition()+Vec2(-30 - 100,-50 - 100));
        this->addChild(m_pHand,100);
        
        Vec2 pos = m_pWhisk->getPosition();
        log("%f",pos.x);
        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            
            m_pHand->setVisible(true);
            
        }),MoveTo::create(1.5, m_pBG->convertToWorldSpace(Vec2(549,363))),CallFunc::create([=](){
        
            m_pHand->setVisible(false);
            log("%f",pos.x);
            m_pHand->setPosition(pos);
        
        
        
        }),DelayTime::create(2.0), nullptr)));
    }
}
void CreamScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    oldPos = toolSprite->getPosition();
    
    if(m_pHand){
        
        m_pHand->stopAllActions();
        m_pHand->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(),CallFunc::create([=](){
            
            m_pHand = nullptr;
            
        }), NULL));
        
        
    }
}

void CreamScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    if (m_bPaint) {
        
        m_pCakePaint->paint(Vec2(toolSprite->getPosition().x-124,toolSprite->getPosition().y-200));
    }
    
}
void CreamScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
//    m_pWhisk->setTexture(g_path + "cream/cream_whisk2.png");
    
    if (!m_pWhiskCover) {

        m_pWhiskCover = Sprite::create(__String::createWithFormat("%scream/cream_whisk_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
        m_pWhiskCover->setAnchorPoint(Vec2::ZERO);
        m_pWhisk->addChild(m_pWhiskCover);
        
        
        Sprite* bowl = dynamic_cast<Sprite*>(m_pBG->getChildByTag(101));
        dynamic_cast<Sprite*>(bowl->getChildByTag(101))->setTexture(__String::createWithFormat("%scream/bowl_cream_%s1.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
        
    }
    
    m_bPaint = true;
    
    if (m_bShowHand) {
        m_bShowHand = false;
         showHand();
    }
   
}
void CreamScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    toolSprite->cancelEvent(true);
    
    MoveTo* move = MoveTo::create(0.5, oldPos);
    EaseSineOut* esi = EaseSineOut::create(move);
    
    toolSprite->runAction(Sequence::create(esi,CallFunc::create([=](){
    
        toolSprite->cancelEvent(false);
        
        if (m_pCakePaint->getPecent()>0.99 && !m_bMakeOver) {
            
            
            auto light = Sprite::create("cocostudio/ui/public/light.png");
            light->setPosition(m_pNextButton->getContentSize()/2);
            m_pNextButton->addChild(light,-1);
            
            light->runAction(RepeatForever::create(RotateBy::create(1,90)));
            
            m_pNextButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),Sequence::create(ScaleTo::create(0.3, 1.1),ScaleTo::create(0.3, 1), nullptr), nullptr)));
            
            m_bMakeOver=true;
//            m_pWhisk->cancelEvent(true);
            
            
            ActionUtils::moveByElasticIn(m_pBG->getChildByTag(101), Vec2(-1000,0),1.0);
            ActionUtils::moveByElasticIn(m_pWhisk, Vec2(1000,0),1.0,CallFunc::create([=](){
                
                m_pStands->runAction(Sequence::create(Spawn::create(MoveBy::create(1.0, Vec2(100,-40)),ScaleTo::create(1.0, 1.2), NULL), CallFunc::create([=](){
                
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");
                    emitter1->setPosition(m_pStands->getContentSize()/2);
                    emitter1->setAutoRemoveOnFinish(true);
                    emitter1->setPosition(emitter1->getPosition() + Vec2(0,50));
                    m_pStands->addChild(emitter1,100);
                    
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                
                    showNextButton(true);
                
                }),NULL));
                
                
                
                
            
            }));
        }
    }), NULL));
}
void CreamScene::nextStep()
{
    log("next");
    hideNextButton();
    SceneChangeManager->enterDecScene();
}