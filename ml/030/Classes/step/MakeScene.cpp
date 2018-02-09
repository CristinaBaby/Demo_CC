//
//  MakeScene.cpp
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/7.
//
//

#include "MakeScene.h"

string
toolPath[8]={"milk","chocolate","greentea","egg","cookie","sugar","vanilla","strawberry"};

Vec2 toolPos[8]={Vec2(193,537),Vec2(281,477),Vec2(181,353),Vec2(245,276),Vec2(898,573),Vec2(833,450),Vec2(1009,330),Vec2(825,274)};



enum toolType
{
    
    MILK = 0,
    CHOCOLATE,
    GREENTEA,
    EGG,
    COOKIE,
    SUGAR,
    VANILLA,
    STRAWBERRY,
    WHISK
    
};
MakeScene::MakeScene()
{
    oldPos = Vec2::ZERO;
    m_iToolZorder = 0;
    m_iEggNum = 0;
    m_iWhiskNum = 0;
    m_iStepNum = 0;
    m_pBowlMix = nullptr;
    m_bIsSounds = true;
    isTouch = true;
    m_bCloseDevice = false;
}
MakeScene::~MakeScene()
{
    
}
bool MakeScene::init()
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
void MakeScene::onEnter()
{
    GameBaseScene::onEnter();
}
void MakeScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
    MaterialComing1(0);
}
void MakeScene::onExit()
{
    GameBaseScene::onExit();
    AudioHelp::getInstance()->stopLoofEffect();
}
void MakeScene::initUI()
{
    
    m_pBG = Sprite::create("images/make/make_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    m_pBowl = Sprite::create("images/make/make_bowl.png");
    m_pBowl->setPosition(m_pBG->getContentSize()/2);
//    m_pBowl->setScale(0);
    m_pBG->addChild(m_pBowl);
    
    
    Vec2 pos = m_pBG->convertToWorldSpace(m_pBowl->getPosition());
    
    Rect rect = Rect(pos.x-100,pos.y-100,190,190);
    
    m_pBowl->setPositionY(m_pBowl->getPositionY()+800);
    //DEBUG
//    g_choose_waste = toolPath[1];
    
    //将需要现实的工具push入容器
    m_vToolPath.push_back(toolPath[0]);
    m_vToolPath.push_back(toolPath[3]);
    m_vToolPath.push_back(toolPath[5]);
    m_vToolPath.push_back(g_choose_waste);
    
    //添加物品
    for (int i = 0;i<8 ;i++) {
        auto tool = ToolSprite::create(__String::createWithFormat("images/make/make_%s0.png",toolPath[i].c_str())->getCString());
        tool->setDelegate(this);
        tool->cancelEvent(true);
        tool->setPosition(toolPos[i]);
        tool->setTag(i);
        tool->setVisible(false);
        tool->addRect(rect);
        m_pBG->addChild(tool,i);
        
        //根据需求，现实需要的物品
        vector<string>::iterator v_tool;
        for (v_tool=m_vToolPath.begin();v_tool!=m_vToolPath.end() ; v_tool++) {
            
            if (*v_tool == toolPath[i]) {
                tool->setVisible(true);
            }
            
        }
        
        if(i<4){
        
            tool->setUserData((void*)1);
            tool->setPositionX(tool->getPositionX()-500);
            
        }else{
        
            tool->setUserData((void*)2);
            tool->setPositionX(tool->getPositionX()+500);
        }
        
        
        m_vTool.pushBack(tool);
    }
    
    m_pWhisk = ToolSprite::create("images/make/make_whisk.png");
    m_pWhisk->setDelegate(this);
    m_pWhisk->cancelEvent(true);
    m_pWhisk->addRect(rect);
    m_pWhisk->setTag(WHISK);
    kAdapterScreen->setExactPosition(m_pWhisk, 20 - 500,20,Vec2(1,1),kBorderRight,kBorderTop);
    this->addToContentLayer(m_pWhisk,10);
    m_pWhisk->setResponsePoint(Vec2(0.23,0.23));
    
    //开启重力感应
//    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(MakeScene::didAccelerate, this));
    //在事件监听器中注册
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}
void MakeScene::MaterialComing1(float dt)
{
    AudioHelp::getInstance()->playEffect("swish.mp3");
    auto func = CallFunc::create([=](){
    
        
        MaterialComing2(0);
        
    });
    
//    RotateBy* rota = RotateBy::create(1.0, 360*2);
//    ScaleTo* scale = ScaleTo::create(1.0, 1.0);
//    ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
//    ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
//    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
//    m_pBowl->runAction(Sequence::create(rota,scale,s1,s2,s3,DelayTime::create(0.3),func,NULL));
    ActionUtils::dropOut(m_pBowl, Vec2(0,-800),1.0,func);
}
void MakeScene::MaterialComing2(float dt)
{
     AudioHelp::getInstance()->playEffect("slide.mp3");
    for (auto tool:m_vTool) {
        
        size_t i = (size_t)tool->getUserData();
        if (i == 1) {
            
            MoveBy* move = MoveBy::create(1.0, Vec2(500,0));
            EaseSineIn* esi = EaseSineIn::create(move);
            tool->runAction(esi);
        }
        
        
    }
    scheduleOnce(schedule_selector(MakeScene::MaterialComing3), 1.2);
}
void MakeScene::MaterialComing3(float dt)
{
    AudioHelp::getInstance()->playEffect("slide.mp3");
    for (auto tool:m_vTool) {
        
        size_t i = (size_t)tool->getUserData();
        if (i == 2) {
            
            MoveBy* move = MoveBy::create(1.0, Vec2(-500,0));
            EaseSineOut* esi = EaseSineOut::create(move);
            tool->runAction(esi);
        }
        
        
    }
    scheduleOnce(schedule_selector(MakeScene::MaterialComing4), 1.2);
}
void MakeScene::MaterialComing4(float dt)
{
    for (auto tool:m_vTool ) {
        tool->cancelEvent(false);
    }
    this->schedule(schedule_selector(MakeScene::checkStep), 2.5);
}
void MakeScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    oldPos = toolSprite->getPosition();
    int tag = toolSprite->getTag();
    m_iToolZorder = toolSprite->getLocalZOrder();
    log("---->%d",m_iToolZorder);
    toolSprite->getParent()->reorderChild(toolSprite, 100);
    
    if (tag == EGG) {

        toolSprite->setTexture("images/make/egg1.png");
        
        auto egg = Sprite::create(__String::createWithFormat("images/make/make_egg%d.png",m_iEggNum +1)->getCString());
        egg->setPosition(toolSprite->getPosition());
        egg->setTag(101);
        m_pBG->addChild(egg,toolSprite->getLocalZOrder()-1);
        
        
        
    }else if(tag == STRAWBERRY){
    
        toolSprite->setTexture("images/make/make_strawberry1.png");
        
        auto strawberry = Sprite::create(__String::create("images/make/make_strawberry0_1.png")->getCString());
        strawberry->setPosition(toolSprite->getPosition());
        strawberry->setTag(102);
        m_pBG->addChild(strawberry,toolSprite->getLocalZOrder()-1);
        
    }
    
}
void MakeScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    for (auto tool:m_vTool ) {
        tool->cancelEvent(true);
    }
    int tag = toolSprite->getTag();
    
    if (tag == WHISK) {
        m_pWhisk->setTexture("images/make/make_whisk.png");
    }
    
    
    auto func = CallFunc::create([=](){
        for (auto tool:m_vTool ) {
            tool->cancelEvent(false);
        }
        toolSprite->getParent()->reorderChild(toolSprite, m_iToolZorder);
        
        
        if (tag == EGG) {
            
            if (m_pBG->getChildByTag(101)) {
                m_pBG->getChildByTag(101)->removeFromParent();
            }
            
            toolSprite->setTexture(__String::createWithFormat("images/make/make_egg%d.png",m_iEggNum)->getCString());
            
            
            
        }else if(tag == STRAWBERRY){
        
            if (m_pBG->getChildByTag(102)) {
                m_pBG->getChildByTag(102)->removeFromParent();
            }
            
            
            toolSprite->setTexture(__String::create("images/make/make_strawberry0.png")->getCString());
            
        }
        
        
        
        
    });
    MoveTo* move = MoveTo::create(0.5, oldPos);
    toolSprite->runAction(Sequence::create(move,func, NULL));
    
}
void MakeScene::didAccelerate(Acceleration* acc, Event* event)
{
    
    log("%f",acc->x);
    if (m_bCloseDevice) {
        m_dAccX = acc->x * 200;
    }
    
    
 
}
void MakeScene::milkAction(float dt)
{
    if (m_dAccX <= -40 ) {
       
    }else if(m_dAccX >= 100){
    
       
        this->m_pContentLayer->getChildByTag(102)->setRotation(100);
        this->unschedule(schedule_selector(MakeScene::milkAction));
        Device::setAccelerometerEnabled(false);
        m_bCloseDevice = false;
       
        
        auto particle = ParticleSystemQuad::create("particle/milkParticle2.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setDuration(3.0);
        particle->setPosition(Vec2::ZERO);
        particle->setRotation(-100 + 90);
        particle->setScale(0.7);
        particle->setScaleX(0.3);
        this->m_pContentLayer->getChildByTag(102)->addChild(particle,2);
        particle->setPosition(this->m_pContentLayer->getChildByTag(102)->getContentSize() + Size(-130+100                                                                                                                                                                                                                                                  ,-35));

        auto milk = Sprite::create("images/make/bowl_milk.png");
        milk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        milk->setPosition(m_pBowl->getContentSize()/2);
        m_pBowl->addChild(milk);
        milk->setScale(0);
        
        AudioHelp::getInstance()->playLoofEffect("add water.mp3");
        
        
        auto func = CallFunc::create([=](){
            
            this->m_pContentLayer->getChildByTag(102)->setVisible(false);
            for (auto tool:m_vTool ) {
                tool->cancelEvent(false);
            }
            m_iStepNum++;
            AudioHelp::getInstance()->stopLoofEffect();
            
             m_dAccX = 0.0;
        });
        
        
        milk->runAction(Sequence::create(DelayTime::create(0.5),ScaleTo::create(3.0, 1.0),func,nullptr));
        
        

    }
    else{
        
        this->m_pContentLayer->getChildByTag(102)->setRotation(m_dAccX);
        
    }
}
void MakeScene::sugarAction(float dt)
{
    if (m_dAccX >40 ) {
        
    }else if(m_dAccX <= -80){
        
        m_pBowl->getChildByTag(101)->setRotation(-80);
//        this->m_pContentLayer->getChildByTag(102)->runAction(RotateTo::create(0.5, -80));
        
        this->unschedule(schedule_selector(MakeScene::sugarAction));
        Device::setAccelerometerEnabled(false);
        
        m_bCloseDevice = false;
        
        auto func = CallFunc::create([=](){
        
            m_pBowl->getChildByTag(101)->setVisible(false);
            for (auto tool:m_vTool ) {
                tool->cancelEvent(false);
            }
            auto milk = Sprite::create("images/make/bowl_sugar.png");
            milk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            milk->setPosition(m_pBowl->getContentSize()/2);
            m_pBowl->addChild(milk);
            m_iStepNum++;
            m_dAccX = 0.0;
        });
        
        auto particle = ParticleSystemQuad::create("particle/sugarParticle.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);    
        particle->setVisible(true);
        particle->setRotation(80);
        particle->setScale(0.7);
        particle->setDuration(1.0);
        particle->setPosition(m_pBowl->getChildByTag(101)->getContentSize() + Size(-130                                                                                                                                                                                                                                                  ,-5));
        m_pBowl->getChildByTag(101)->addChild(particle,2);
        
        AudioHelp::getInstance()->playEffect("add powder.mp3");
        
        ActionUtils::delayTimeAction(this,1.0,func);
        
    }
    else{
        
        m_pBowl->getChildByTag(101)->setRotation(m_dAccX);
        
    }
}
void MakeScene::eggAction(float dt)
{
    

}
void MakeScene::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    log("move in rect");
    int tag = toolSprite->getTag();
    log("tag ---> %d",tag);
    for (auto tool:m_vTool ) {
        tool->cancelEvent(true);
    }
    
    switch (tag) {
        case MILK:
            {
                m_dAccX = 0.0;
                toolSprite->setVisible(false);
                
                
                auto tool = Sprite::create(__String::createWithFormat("images/make/make_%s1.png",toolPath[tag].c_str())->getCString());
//                tool->setPosition(m_pBG->convertToWorldSpace(toolSprite->getPosition()));
                
                tool->setTag(102);
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(55 + 40,291 + 50)));
                this->addToContentLayer(tool,100);
                
                
              
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
                
                
                auto sp = Sprite::create("images/make/incline1.png");
                kAdapterScreen->setExactPosition(sp, 480, 320);
                this->addToUILayer(sp);
                
                auto func = CallFunc::create([=](){
                   
                    if(abs(m_dAccX)>80){
                        
                        m_dAccX = 0.0;
                    }
                   this->schedule(schedule_selector(MakeScene::milkAction), 0.1);
                    Device::setAccelerometerEnabled(true);
                    m_bCloseDevice = true;
                   
                });
                
                
                
                RotateBy* rota = RotateBy::create(0.5, 50);
                RotateBy* rota1 = RotateBy::create(0.5, 50);
                sp->runAction(Sequence::create(rota,rota->reverse(),rota1,rota1->reverse(),RemoveSelf::create(),func,NULL));
                
            }
            break;
        case CHOCOLATE:
            {
                toolSprite->setVisible(false);
                
                
                auto tool = Sprite::create(__String::create("images/make/make_chocolate1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(111,90))+Vec2(-49,72));
                this->addToContentLayer(tool);
               
                RotateBy* rota = RotateBy::create(0.35, 90);
                MoveBy* move1 = MoveBy::create(0.25,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.25,Vec2(0,10));
                
                
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("add.mp3");
                    
                    auto chocolate = Sprite::create("images/make/bowl_chocolate.png");
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    m_iStepNum++;
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                
                });
                
                tool->runAction(Sequence::create(rota,move1,move1->reverse(),move2,move2->reverse(),RemoveSelf::create(),func,NULL));
                
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
                
            }
            
            break;
        case GREENTEA:
            {
                toolSprite->setVisible(false);
                
                auto tool = Sprite::create(__String::create("images/make/make_greentea1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(106,131))+Vec2(-58,100));
                this->addToContentLayer(tool);
                
                RotateBy* rota = RotateBy::create(0.35, 90);
                MoveBy* move1 = MoveBy::create(0.25,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.25,Vec2(0,10));
                
                
                AudioHelp::getInstance()->playEffect("add powder.mp3");
                
                auto func = CallFunc::create([=](){
                    
                    
                    
                    auto chocolate = Sprite::create("images/make/bowl_greentea.png");
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    m_iStepNum++;
                });
                
                tool->runAction(Sequence::create(rota,move1,move1->reverse(),move2,move2->reverse(),RemoveSelf::create(),func,NULL));
                
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
            }
            
            break;
        case EGG:
            {
                
                Vec2 pos = Vec2(m_pBowl->getPositionX() - 132 - 40,m_pBowl->getPositionY()+138+10);
                
                auto func = CallFunc::create([=](){
                
                    auto egg = Sprite::create("images/make/egg.png");
                    egg->setPosition(Vec2(137+ (m_iEggNum-1)*80,200));
                    m_pBowl->addChild(egg);
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    m_iStepNum++;
                });
                
                
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile("images/make/egg2.png");
                pOvenAnimation->addSpriteFrameWithFile("images/make/egg3.png");
                
                pOvenAnimation->setDelayPerUnit(0.4);
                pOvenAnimation->setLoops(1);
                
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                
                auto func1 = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("crack egg.mp3");
                    
                });
                
                
                MoveTo* move = MoveTo::create(0.5, pos);
                MoveBy* move1 = MoveBy::create(0.35,Vec2(24,-20));
                auto move2 = MoveBy::create(0.35,Vec2(24,-20));
                MoveBy* move3 = MoveBy::create(0.5, Vec2(100 + m_iEggNum*80,0));
                toolSprite->runAction(Sequence::create(move,move1,move1->reverse(),move2,move2->reverse(),move3,func1,pOvenAnimat,CallFunc::create([=](){
                
                    m_iEggNum++;
                    log("m_iEggNum---->%d",m_iEggNum);
                    if (m_pBG->getChildByTag(101)) {
                        m_pBG->getChildByTag(101)->removeFromParent();
                    }
                    toolSprite->setPosition(oldPos);
                    toolSprite->setTexture(__String::createWithFormat("images/make/make_egg%d.png",m_iEggNum)->getCString());
                
                    if (m_iEggNum==2) {
                        toolSprite->runAction(FadeOut::create(1.0));
                        
                        m_vTool.eraseObject(toolSprite);
                        toolSprite->removeFromParent();
                    };
                    
                    
                }),func,NULL));

            }
            
            break;
        case COOKIE:
            {
                toolSprite->setVisible(false);
                
                auto tool = Sprite::create(__String::create("images/make/make_cookie1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(188,135))+Vec2(112,96));
                this->addToContentLayer(tool);
                
                RotateBy* rota = RotateBy::create(0.5, -90);
                MoveBy* move1 = MoveBy::create(0.5,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.5,Vec2(0,10));
                
                
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("add.mp3");
                    
                    auto chocolate = Sprite::create("images/make/bowl_cookie.png");
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    m_iStepNum++;
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    
                });
                
                tool->runAction(Sequence::create(rota,move1,move1->reverse(),move2,move2->reverse(),RemoveSelf::create(),func,NULL));
                
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
            }
            
            break;
        case SUGAR:
            {
                m_dAccX = 0.0;
                toolSprite->setVisible(false);
                
                auto tool = Sprite::create(__String::createWithFormat("images/make/make_%s1.png",toolPath[tag].c_str())->getCString());
//                tool->setPosition(m_pBG->convertToWorldSpace(toolSprite ->getPosition()));
                tool->setTag(101);
                tool->setPosition(Vec2(318,310));
                m_pBowl->addChild(tool,100);
//                this->addToContentLayer(tool);
                
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
                
                auto sp = Sprite::create("images/make/incline.png");
                kAdapterScreen->setExactPosition(sp, 480, 320);
                this->addToUILayer(sp);
                auto func = CallFunc::create([=](){
                    
                    
                    
                    if(abs(m_dAccX)>80){
                        
                        m_dAccX = 0.0;
                    }
                   this->schedule(schedule_selector(MakeScene::sugarAction), 0.1);
                    
                    Device::setAccelerometerEnabled(true);
                    m_bCloseDevice = true;
                    
                });
                
                RotateBy* rota = RotateBy::create(0.5, 50);
                RotateBy* rota1 = RotateBy::create(0.5, 50);
                sp->runAction(Sequence::create(rota,rota->reverse(),rota1,rota1->reverse(),RemoveSelf::create(),func,NULL));
            }
            
            break;
        case VANILLA:
            {
                toolSprite->setVisible(false);
                
                
                auto tool = Sprite::create(__String::create("images/make/make_vanilla1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(204,153))+Vec2(23,96));
                this->addToContentLayer(tool);
                
                RotateBy* rota = RotateBy::create(0.5, -90);
                MoveBy* move1 = MoveBy::create(0.5,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.5,Vec2(0,10));
                
                
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("crack egg.mp3");
                    
                    auto chocolate = Sprite::create("images/make/bowl_vanilla.png");
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    m_iStepNum++;
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    
                });
                
                tool->runAction(Sequence::create(rota,move1,move1->reverse(),move2,move2->reverse(),RemoveSelf::create(),func,NULL));
                
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
            }
            
            break;
        case STRAWBERRY:
            {
                m_vTool.eraseObject(toolSprite);
                toolSprite->removeFromParent();
                
                AudioHelp::getInstance()->playEffect("crack egg.mp3");
                
                auto strawberry = Sprite::create("images/make/bowl_strawberry.png");
                strawberry->setAnchorPoint(Vec2::ZERO);
                m_pBowl->addChild(strawberry);
                m_iStepNum++;
                
                if (m_pBG->getChildByTag(102)) {
                    m_pBG->getChildByTag(102)->runAction(FadeOut::create(0.5));
                    
                }

                for (auto tool:m_vTool ) {
                    tool->cancelEvent(false);
                }
            }
            
            break;
        case WHISK:
            {
                m_pWhisk->setTexture("images/make/make_whisk1.png");
                
                if (pTouch->getDelta().x>5) {
                    
                    m_iWhiskNum++;

                    if (m_bIsSounds) {
                        AudioHelp::getInstance()->playEffect("cut.mp3");
                        m_bIsSounds = false;
                    }
                    
                    if (isTouch) {
                        isTouch=false;
                        auto funcSounds=CallFunc::create([&](){
                            
                            m_bIsSounds = true;
                            isTouch=true;
                        });
                        
                        this->runAction(Sequence::create(DelayTime::create(3.0),funcSounds, NULL));
                        
                    }
                    
                    if (m_iWhiskNum == 20) {
                        
                        if(!m_pBowlMix){
                            
                            m_pBowl->removeAllChildren();
                            m_pBowlMix = Sprite::create(__String::createWithFormat("images/make/mix_%s0.png",g_choose_waste.c_str())->getCString());
                            m_pBowlMix->setPosition(m_pBowl->getContentSize()/2);
                            m_pBowl->addChild(m_pBowlMix,101);
                            
                        
                        }
                        
                        
                    }else if(m_iWhiskNum == 40){
                    
                        m_pBowlMix->setTexture(__String::createWithFormat("images/make/mix_%s1.png",g_choose_waste.c_str())->getCString());
                        
                    }else if(m_iWhiskNum == 60){
                        
                        m_pBowlMix->setTexture(__String::createWithFormat("images/make/mix_%s2.png",g_choose_waste.c_str())->getCString());
                        
                    }else if(m_iWhiskNum == 80){
                        
                        m_pBowlMix->setTexture(__String::createWithFormat("images/make/mix_%s3.png",g_choose_waste.c_str())->getCString());
                        
                        m_pWhisk->cancelEvent(true);
                        
                        auto func = CallFunc::create([=](){
                        
                            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                            
                            emitter1->setPosition(m_pBG->getContentSize()/2);
                            
                            emitter1->setAutoRemoveOnFinish(true);
                            
                            m_pBG->addChild(emitter1,100);
                            
                            AudioHelp::getInstance()->playEffect("all star .mp3");
                            
                            showNextButton(true);
                        });
                        
                        m_pWhisk->runAction(Sequence::create(MoveBy::create(0.5,Vec2(700,0)),func,nullptr));
                    }
                    
                    
                    
                }
            }
            break;
            
        default:
            break;
    }
    
}
void MakeScene::makeOver()
{
    

}
void MakeScene::checkStep(float dt)
{
    if (m_iStepNum == 5) {
        
        this->unschedule(schedule_selector(MakeScene::checkStep));
        
        
        auto func = CallFunc::create([=](){
        
            m_pWhisk->cancelEvent(false);
        });
        
        ActionUtils::moveByElasticOut(m_pWhisk, Vec2(-500,0),0.5,func);
        
    }


}
void MakeScene::nextStep()
{
    log("next");
    hideNextButton();
    SceneChangeManager->enterIceCreamScene();
}