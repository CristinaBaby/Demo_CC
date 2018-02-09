//
//  StepLayer1.cpp
//  CookiesMaker
//
//  Created by wangbosheng1 on 14-11-24.
//
//

#include "StepLayer1.h"
#include "../../Depends/utils/AudioHelp.h"
#include "SimpleAudioEngine.h"
#include "../../Depends/utils/Utils.h"
#include "../../Depends/utils/AdapterScreen.h"
#include "ActionUtils.h"
#include "../../Depends/utils/Global.h"
#include "SimpleAudioEngine.h"
#include "AreaLayer.h"


StepLayer1::StepLayer1(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
StepLayer1::~StepLayer1()
{
    
}
StepLayer1* StepLayer1::create(GameLayer* gameLayer)
{
    StepLayer1* step1 = new StepLayer1(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool StepLayer1::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
        CC_BREAK_IF(!initUI());
        oldPosition = Vec2::ZERO;
        count = 0;
        m_pIsTouch = true;
       
        
        m_iRote = 0;
        m_bIsSounds = true;
        isInit = true;
       
    } while (0);
    
    return isInit;
    
}
bool StepLayer1::initUI()
{
    m_pPlate= Sprite::create("images/pizzaStep/1/bowl.png");
    m_pPlate->setAnchorPoint(Vec2(0.5,0.5));
    m_pPlate->setPosition(Point(331 - 600,389.5));
    m_pGameLayer->addToBGSprite(m_pPlate);
    
    Rect rect = Rect(m_pPlate->getPositionX() - 160 + 600, m_pPlate->getPositionY() - 130, 316, 308);

    m_pYeast = ToolSprite::create("images/pizzaStep/1/yeast.png");
    kAdapterScreen->setExactPosition(m_pYeast,-200 - 50, 80,kAnchorRT,kBorderRight,kBorderTop);
    m_pYeast->setTag(KYEAST);
    m_pYeast->setDelegate(this);
    m_pYeast->addRect(rect);
    this->addChild(m_pYeast);
    m_vToolSprite.pushBack(m_pYeast);
   
    m_pSalt = ToolSprite::create("images/pizzaStep/1/salt0.png");
    kAdapterScreen->setExactPosition(m_pSalt,-300 + 80, 0,kAnchorRT,kBorderRight,kBorderTop);
    m_pSalt->setTag(KSALT);
    m_pSalt->setDelegate(this);
    m_pSalt->addRect(rect);
    this->addChild(m_pSalt);
    m_vToolSprite.pushBack(m_pSalt);
    
    m_pSugar = ToolSprite::create("images/pizzaStep/1/sugar.png");
    kAdapterScreen->setExactPosition(m_pSugar, -200, 50,kAnchorRB,kBorderRight,kBorderBottom);
    m_pSugar->setTag(KSUGAR);
    m_pSugar->setDelegate(this);
    m_pSugar->addRect(rect);
    this->addChild(m_pSugar);
    m_vToolSprite.pushBack(m_pSugar);
    
    m_pFlour = ToolSprite::create("images/pizzaStep/1/flour0.png");
    kAdapterScreen->setExactPosition(m_pFlour, 300 - 1000, 350,Vec2(0.5,0.5),kBorderRight,kBorderBottom);
    m_pFlour->setTag(KFLOUR);
    m_pFlour->setDelegate(this);
    m_pFlour->addRect(rect);
    this->addChild(m_pFlour,1);;
    
    
    m_pWater = ToolSprite::create("images/pizzaStep/1/water.png");
    kAdapterScreen->setExactPosition(m_pWater, -300, 50,kAnchorRB,kBorderRight,kBorderBottom);
    m_pWater->setTag(KWATER);
    m_pWater->setDelegate(this);
    m_pWater->addRect(rect);
    this->addChild(m_pWater);
    m_vToolSprite.pushBack(m_pWater);

    m_pSpoon = ToolSprite::create("images/pizzaStep/2/spoon.png");
    kAdapterScreen->setExactPosition(m_pSpoon, 550, 1000,Vec2(0.1,0.85),kBorderNone,kBorderBottom);
    m_pSpoon->setResponsePoint(Vec2(0.1,0.85));
    m_pSpoon->setTag(KSPOON);
    m_pSpoon->setDelegate(this);
    //m_pSpoon->addRect(rect);
    this->addChild(m_pSpoon,1);

    
   
    
    for (auto tool:m_vToolSprite) {
        
        tool->cancelEvent(true);
        
    }
    
//    AreaLayer* areaLayer = AreaLayer::create();
//    areaLayer->setRect(rect);
//    areaLayer->AddRectLayerToParent(this);
    

//
//    AreaLayer* areaLayer2 = AreaLayer::create();
//    areaLayer2->setNode(m_pBake);
//    areaLayer2->AddNodeLayerToParent(this);
    
    
    return true;
}
void StepLayer1::onEnter()
{
    StepLayer::onEnter();
     this->MaterialComing1();
    schedule(schedule_selector(StepLayer1::stepControl), 1.0);
}
void StepLayer1::onExit()
{
    StepLayer::onExit();

}
//各个物品进入动画
void StepLayer1::MaterialComing1()
{
    
    MoveBy* move1 = MoveBy::create(1.0, Vec2(600,0));
    EaseBounceOut* ease = EaseBounceOut::create(move1);
    
    auto func1 = CallFunc::create([&](){
        
        this->MaterialComing2();
    });
    auto funcSounds=CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("add.mp3");
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
    
    m_pPlate->runAction(Sequence::create(DelayTime::create(1.5),ease,DelayTime::create(0.5),func1,NULL));
}
void StepLayer1::MaterialComing2()
{
    AudioHelp::getInstance()->playEffect("slide.mp3");
    auto func2 = CallFunc::create([&](){
        
        this->MaterialComing3();
        
    });
    
    MoveBy* move2 = MoveBy::create(1.0, Vec2(-500,0));
    EaseBackOut* ease2 = EaseBackOut::create(move2);
    m_pSugar->runAction(Sequence::create(ease2,func2,NULL));
    m_pYeast->runAction(Sequence::create(ease2->clone(),NULL));

}
void StepLayer1::MaterialComing3()
{
   AudioHelp::getInstance()->playEffect("slide.mp3");
        auto func = CallFunc::create([&](){
   
            MaterialComing4();
    
     });
    
    MoveBy* move3 = MoveBy::create(1.0, Vec2(-320,0));
    EaseBounceOut* ease3 = EaseBounceOut::create(move3);
    m_pSalt->runAction(Sequence::create(ease3,NULL));
    m_pWater->runAction(Sequence::create(ease3->clone(),func,NULL));
}
void StepLayer1::MaterialComing4()
{
    AudioHelp::getInstance()->playEffect("slide.mp3");
    auto func = CallFunc::create([&](){
        
        for (auto tool:m_vToolSprite) {
            tool->cancelEvent(false);
        }
        
    });
    
    MoveBy* move3 = MoveBy::create(1.0, Vec2(-1000,0));
    EaseBounceOut* ease3 = EaseBounceOut::create(move3);
    m_pFlour->runAction(Sequence::create(ease3,func,NULL));
}
void StepLayer1::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    log("count------>%d",count);
    toolSprite->cancelEvent(!m_pIsTouch);
    this->reorderChild(toolSprite, 100);
    oldPosition = toolSprite->getPosition();
    
}
void StepLayer1::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    
}
void StepLayer1::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    CocosDenshion::SimpleAudioEngine::SimpleAudioEngine::getInstance()->stopAllEffects();
    toolSprite->setPosition(oldPosition);
    oldPosition = Vec2::ZERO;
   
    m_bIsSounds = true;
    
    if (tag == KSPOON) {
        
        toolSprite->setTexture("images/pizzaStep/2/spoon.png");
        
    }
    
}
void StepLayer1::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
    int tag = toolSprite->getTag();
    switch (tag) {
        case KSALT:
        {
            AudioHelp::getInstance()->playEffect("add powder.mp3");
            toolSprite->removeFromParent();
            m_vToolSprite.eraseObject(toolSprite);
            
            
             auto funcSounds=CallFunc::create([&](){
                 
                 AudioHelp::getInstance()->playEffect("add powder.mp3");
                 
             });
            
            this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
            
            
            
            
            for (auto tool:m_vToolSprite) {
                tool->cancelEvent(true);
            }
            
          
            Sprite* flour = Sprite::create("images/pizzaStep/1/salt1.png");
            flour->setPosition(Point(335,444));
            m_pPlate->addChild(flour);
            
            auto funcFlour=CallFunc::create([&](){
    
       
                Sprite* flourBowl = Sprite::create("images/pizzaStep/1/salt_bowl.png");
                flourBowl->setPosition(Vec2::ZERO);
                flourBowl->setAnchorPoint(Vec2(0,0));
                m_pPlate->addChild(flourBowl);
                
                for (auto tool:m_vToolSprite) {
                    tool->cancelEvent(false);
                }
               
            });

            auto pOvenAnimation = Animation::create();
            
            for (int i = 1; i < 4; i++) {
             
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/pizzaStep/1/salt%d.png", i)->getCString());
            }
            
            pOvenAnimation->setDelayPerUnit(0.3f);
            pOvenAnimation->setLoops(2);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            
            flour->runAction(Sequence::create(pOvenAnimat, CCDelayTime::create(0.5),funcFlour,RemoveSelf::create(),NULL));
            count++;
        }
            break;
        case KYEAST:
        {
            AudioHelp::getInstance()->playEffect("add powder.mp3");
            Sprite* bowl = Sprite::create("images/pizzaStep/1/bowl_yeast.png");
            bowl->setPosition(toolSprite->getPosition());
            bowl->setAnchorPoint(Vec2(1,1));
            this->addChild(bowl);
            
            
            toolSprite->removeFromParent();
            m_vToolSprite.eraseObject(toolSprite);
   
            Sprite* yeast = Sprite::create("images/pizzaStep/1/yeast_bowl.png");
            yeast->setPosition(Vec2::ZERO);
            yeast->setAnchorPoint(Vec2(0,0));
            m_pPlate->addChild(yeast);
            
            MoveTo* move = MoveTo::create(0.5,oldPosition);
            FadeOut* fade = FadeOut::create(0.5);
            bowl->runAction(Sequence::create(DelayTime::create(0.5),move,fade,RemoveSelf::create(), NULL));
            
            count++;
        }
            break;
        case KSUGAR:
        {
            AudioHelp::getInstance()->playEffect("add powder.mp3");
            toolSprite->removeFromParent();
            m_vToolSprite.eraseObject(toolSprite);
           
            auto funcSounds=CallFunc::create([&](){
                
                AudioHelp::getInstance()->playEffect("add powder.mp3");
                
            });
            
            this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
            
            
            Sprite* sugar = Sprite::create("images/pizzaStep/1/sugar1.png");
            sugar->setPosition(Point(446,325));
            m_pPlate->addChild(sugar);
            
            for (auto tool:m_vToolSprite) {
                tool->cancelEvent(true);
            }
            
            auto funcSugar=CallFunc::create([&](){
                

                Sprite* flourBowl = Sprite::create("images/pizzaStep/1/sugar_bowl.png");
                flourBowl->setPosition(Vec2::ZERO);
                flourBowl->setAnchorPoint(Vec2(0,0));
                m_pPlate->addChild(flourBowl);

                
                for (auto tool:m_vToolSprite) {
                    tool->cancelEvent(false);
                }
            });
            
            auto pOvenAnimation = Animation::create();
            
            for (int i = 1; i < 4; i++) {
               
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/pizzaStep/1/sugar%d.png", i)->getCString());
            }
            
            pOvenAnimation->setDelayPerUnit(0.3f);
            pOvenAnimation->setLoops(2);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            
            sugar->runAction(Sequence::create(pOvenAnimat, CCDelayTime::create(0.5),funcSugar,RemoveSelf::create(),NULL));
        
            count++;
        }
            
            break;
        case KWATER:
        {
            AudioHelp::getInstance()->playEffect("add water.mp3");
            toolSprite->removeFromParent();
            m_vToolSprite.eraseObject(toolSprite);
            
            auto funcSounds=CallFunc::create([&](){
                
                AudioHelp::getInstance()->playEffect("add water.mp3");
                
            });
            
            this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
            
            
            Sprite* sugar = Sprite::create("images/pizzaStep/1/water1.png");
            sugar->setPosition(Point(410,474));
            m_pPlate->addChild(sugar);
            
            for (auto tool:m_vToolSprite) {
                tool->cancelEvent(true);
            }
            
            auto funcSugar=CallFunc::create([&](){
                
                
                Sprite* flourBowl = Sprite::create("images/pizzaStep/1/water_bowl.png");
                flourBowl->setPosition(Vec2::ZERO);
                flourBowl->setAnchorPoint(Vec2(0,0));
                m_pPlate->addChild(flourBowl);
                
                
                for (auto tool:m_vToolSprite) {
                    tool->cancelEvent(false);
                }
            });
            
            auto pOvenAnimation = Animation::create();
            
            for (int i = 1; i < 4; i++) {
                
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/pizzaStep/1/water%d.png", i)->getCString());
            }
            
            pOvenAnimation->setDelayPerUnit(0.3f);
            pOvenAnimation->setLoops(2);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            
            sugar->runAction(Sequence::create(pOvenAnimat, CCDelayTime::create(0.5),funcSugar,RemoveSelf::create(),NULL));
            
            count++;
        }
            
            break;
        case KFLOUR:
        {
            AudioHelp::getInstance()->playEffect("add powder.mp3");
            toolSprite->removeFromParent();
            m_vToolSprite.eraseObject(toolSprite);
            
            auto funcSounds=CallFunc::create([&](){
                
                AudioHelp::getInstance()->playEffect("add powder.mp3");
                
            });
            
            this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
            
        
            Sprite* flour = Sprite::create("images/pizzaStep/1/flour1.png");
            flour->setPosition(Point(310,474));
            m_pPlate->addChild(flour);
            
            for (auto tool:m_vToolSprite) {
                tool->cancelEvent(true);
            }
            
            auto funcSugar=CallFunc::create([&](){
                
               
                Sprite* flourBowl = Sprite::create("images/pizzaStep/1/flour4_bowl.png");
                flourBowl->setPosition(Vec2::ZERO);
                flourBowl->setAnchorPoint(Vec2(0,0));
                m_pPlate->addChild(flourBowl);
                
                
                for (auto tool:m_vToolSprite) {
                    tool->cancelEvent(false);
                }
            });
            
            auto pOvenAnimation = Animation::create();
            
            for (int i = 1; i < 4; i++) {
                
                pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/pizzaStep/1/flour%d.png", i)->getCString());
            }
            
            pOvenAnimation->setDelayPerUnit(0.3f);
            pOvenAnimation->setLoops(2);
            
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            
            flour->runAction(Sequence::create(pOvenAnimat, CCDelayTime::create(0.5),funcSugar,RemoveSelf::create(),NULL));
            
            count++;
        
        
        }
            break;
        case KSPOON:
        {
            
            if (count == 8)
            {
                static int type = 1;
              
                if (abs(pTouch->getDelta().x) > 3)
                {
                
                    toolSprite->setTexture("images/pizzaStep/2/spoon1.png");
                    
                    if (m_bIsSounds) {
                        
                        log("------------------->bool sound  %d",m_bIsSounds);
                        m_bIsSounds = false;
                        log("------------------->play sound");
                        AudioHelp::getInstance()->playEffect("stir the batter.mp3");
                       
                    }
                    
                   
                    
                    m_iRote++;
                    if (m_iRote%3 == 0)
                    {
                        ActionInterval* rote = RotateBy::create(0.5,45);
                        m_pPlateContent->runAction(rote);
                    }
                    if (m_iRote == 40) {
                        
                        m_pPlateContent->setVisible(true);
                        
                    }
                    
                    
                    if (m_iRote%70 == 0)
                    {
                        
                        m_pPlateContent->setTexture(__String::createWithFormat("images/pizzaStep/2/batter_%d_bowl.png", type)->getCString());
                        type++;
                    }
                    
                    if (type > 4) {
                        
                        type = 4;
                    }
                }
                if (m_iRote == 300) {
                    count++;
                    CocosDenshion::SimpleAudioEngine::SimpleAudioEngine::getInstance()->stopAllEffects();
                    m_iRote = 0;
                    break;
                }
            }
            
            
            
         }
            
            break;
        default:
            break;
          
    }
    
}
void StepLayer1::onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if (tag == KSPOON) {
       
        toolSprite->setTexture("images/pizzaStep/2/spoon.png");
        
    }

}
//步骤控制
void StepLayer1::stepControl(float dt)
{
    static bool isTouch = true;
    log("---------%d",count);
    if(count == 5)
    {
        auto func = CallFunc::create([&](){
            
            count++;
        });
        MoveBy* move = MoveBy::create(1.0,Vec2(kAdapterScreen->screenSize.width/2 - m_pPlate->getPositionX(), 0));
        m_pPlate->runAction(Sequence::create(DelayTime::create(4.0),move,func, NULL));
        count++;
    }
    
    if(count == 7)
    {
        Rect rect = Rect(m_pPlate->getPositionX()-124 , m_pPlate->getPositionY()-197 , 270, 245);
//        AreaLayer* areaLayer = AreaLayer::create();
//        areaLayer->setRect(rect);
//        areaLayer->AddRectLayerToParent(this);
        
        m_pSpoon->addRect(rect);
        m_pPlate->removeAllChildren();
        m_pPlateContent = Sprite::create("images/pizzaStep/2/batter_0_bowl.png");
        m_pPlateContent->setPosition(Vec2(565 * 0.5, 605 * 0.5));
        m_pPlateContent->setAnchorPoint(Vec2(0.5,0.5));
        m_pPlate->addChild(m_pPlateContent,10);
        
        MoveBy* move = MoveBy::create(3.0,Vec2(0, -600));
        EaseElasticOut* eeo = EaseElasticOut::create(move,0.5);
        m_pSpoon->runAction(Sequence::create(eeo, NULL));
        
        count++;
    }
    
    if (count == 8)
    {
        if (isTouch) {
            isTouch = false;
            auto funcSounds=CallFunc::create([&](){
                
                m_bIsSounds = true;
                isTouch = true;
            });
            
            this->runAction(Sequence::create(DelayTime::create(2.5),funcSounds, NULL));
        }
        
    }
    
    
    if (count == 9) {
        
        auto func = CallFunc::create([=](){
            
//            m_pGameLayer->doNextStep(PizzaStepLayer2::create(m_pGameLayer));
            
        });
        

        ActionUtils::fadeOut(m_pSpoon,1.0,func);
    }
    
    
}


