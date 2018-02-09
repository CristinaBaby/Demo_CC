//
//  MakeScene.cpp
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/7.

#include "MakeScene.h"
#include "MainScene.h"
#include "BezierCreator.h"
#include "ActionHelper.h"

REGIST_SECENE(MakeScene);

string
toolPath[10]={"sugar","flour","egg","blueberry","chocolate","greentea","strawberry","vanilla","milk","butter"};

Vec2 toolPos[10]={Vec2(536,541 + 20 + 40),Vec2(631 - 30,652 - 150),Vec2(546,364),Vec2(536 - 40,541 + 70),Vec2(536 - 40,541 + 80),Vec2(536 - 40,541 + 80),Vec2(536 - 40,541 + 80),Vec2(536 - 40,541 + 50 + 50),Vec2(631 - 30,652 - 150),Vec2(546,364)};

enum toolType
{
    SUGAR = 0,
    FLOUR,
    EGG,
    BLUEBERRY,
    CHOCOLATE,
    GREENTEA,
    STRAWBERRY,
    VANILLA,
    MILK,
    BUTTER,
    WHISK,
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
    m_pSugar = nullptr;
    m_pSugarMove = true;
    m_bWhiskAction = true;
    m_pHand = nullptr;
    m_bBowlIsMove = false;
}
MakeScene::~MakeScene()
{
    
}
#pragma mark 初始化
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
//    g_path + "
    
    __String* bgPath;
    
    auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
    auto max = std::max(size.height, size.width);
    float scale = 1.0;
    if(max>=2048){
        
        bgPath = __String::create("cocostudio/png/cookies/1/bg_h.jpg");
        scale = 0.5;
    }else{
        
        bgPath = __String::create("cocostudio/png/cookies/1/bg.jpg");
        
    }
    
    auto bg = Sprite::create(bgPath->getCString());
//    kAdapterScreen->setExactPosition(bg, 320, 480);
//    this->addToBGLayer(bg);
    bg->setScale(scale);
    
    m_pBG = Sprite::create("cocostudio/png/cookies/1/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 320, 480);
    this->addToBGLayer(m_pBG);
//    m_pBG->setVisible(false);
    
    bg->setAnchorPoint(Vec2::ZERO);
    m_pBG->addChild(bg,0);
    
    m_pBowl = Sprite::create(g_path + "make/make_bowl.png");
    m_pBowl->setPosition(Vec2(210 + 50,457 + 50));
//    m_pBowl->setScale(0);
    m_pBowl->setAnchorPoint(Vec2(0.5,0.5));
    m_pBG->addChild(m_pBowl,11);
    
    auto bowl = Sprite::create(g_path + "make/make_bowl1.png");
    bowl->setAnchorPoint(Vec2::ZERO);
    m_pBowl->addChild(bowl,100);
    
    Vec2 pos = m_pBG->convertToWorldSpace(m_pBowl->getPosition());
    Rect rect = Rect(pos.x-120,pos.y+10 - 80,230,80 + 60);
    m_pBowl->setPositionY(m_pBowl->getPositionY()+800);
    //DEBUG
//    g_choose_waste = toolPath[3];
    
    //将需要现实的工具push入容器
    m_vToolPath.push_back(toolPath[0]);
    m_vToolPath.push_back(toolPath[1]);
    m_vToolPath.push_back(toolPath[2]);
    m_vToolPath.push_back(toolPath[8]);
    m_vToolPath.push_back(toolPath[9]);
    m_vToolPath.push_back(g_choose_waste);
    
    //添加物品
    for (int i = 0;i<10 ;i++) {
        auto tool = ToolSprite::create(__String::createWithFormat("%smake/make_%s0.png",g_path.c_str(),toolPath[i].c_str())->getCString());
        tool->setDelegate(this);
        tool->cancelEvent(true);
        tool->setPosition(toolPos[i]);
        tool->setTag(i);
        tool->setVisible(false);
        tool->addRect(rect);
        m_pBG->addChild(tool,i+1);
        
        //添加黄油
        if(i == 9){
        
            auto butter = Sprite::create(g_path + "make/butter.png");
            butter->setAnchorPoint(Vec2::ZERO);
            tool->addChild(butter);
            butter->setTag(100);
            
            auto shadow = Sprite::create(g_path + "make/butter_shadow.png");
            shadow->setAnchorPoint(Vec2::ZERO);
            butter->addChild(shadow,-1);
            
        }
        if(i == 1 || i == 8 || i == 0){
        
            auto butter = Sprite::create(__String::createWithFormat("%smake/%s_shadow.png",g_path.c_str(),toolPath[i].c_str())->getCString());
            butter->setAnchorPoint(Vec2::ZERO);
            tool->addChild(butter,-1);
            butter->setTag(100);
            
        
        
        }
        
        //根据需求，xian需要的物品
        vector<string>::iterator v_tool;
        for (v_tool=m_vToolPath.begin();v_tool!=m_vToolPath.end() ; v_tool++) {
            
            if (*v_tool == toolPath[i]) {
                tool->setVisible(true);
                
                if(i<3){
                    
                    tool->setUserData((void*)1);
                    tool->setPositionX(tool->getPositionX()+500);
                    
                }else{
                    
                    tool->setUserData((void*)2);
                    tool->setPositionX(tool->getPositionX()+500);
                }
                
            }
            
        }
        
        
        
        m_vTool.pushBack(tool);
    }
    
    m_pWhisk = ToolSprite::create((g_path + "make/make_whisk0.png").c_str());
    m_pWhisk->setDelegate(this);
    m_pWhisk->cancelEvent(true);
    m_pWhisk->addRect(rect);
    m_pWhisk->setTag(WHISK);
    kAdapterScreen->setExactPosition(m_pWhisk, 20 - 500,200,Vec2(1,0),kBorderRight,kBorderBottom);
    this->addToContentLayer(m_pWhisk,10);
    m_pWhisk->setResponsePoint(Vec2(0.14,0.18));
    
}
#pragma mark 材料进入
void MakeScene::MaterialComing1(float dt)
{
    AudioHelp::getInstance()->playEffect("swish.mp3");

    ActionHelper::showBezier(m_pBowl, Vec2(210 + 50,457 + 50), ActionHelper::ShowDirection::show_from_bottom,[=](){
    
        MaterialComing2(0);
        
//     MaterialComing3(0);
    });
}
void MakeScene::MaterialComing2(float dt)
{
    
    for (auto tool:m_vTool) {
        
        size_t i = (size_t)tool->getUserData();
        if (i == 1) {
            
//            MoveBy* move = MoveBy::create(1.0, Vec2(-500,0));
//            EaseSineIn* esi = EaseSineIn::create(move);
//            tool->runAction(esi);
//
            this->runAction(Sequence::create(DelayTime::create(tool->getTag()*0.3),CallFunc::create([=](){
               
                AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                ActionHelper::showBezier(tool, toolPos[tool->getTag()], ActionHelper::ShowDirection::show_from_right);
                
                
            }), NULL));
            
            
        }
        
    }
//    scheduleOnce(schedule_selector(MakeScene::MaterialComing3), 1.2);
    scheduleOnce(schedule_selector(MakeScene::MaterialComing4), 1.5);
}
void MakeScene::MaterialComing3(float dt)
{
//    AudioHelp::getInstance()->playEffect("slide.mp3");
    for (auto tool:m_vTool) {
        
        size_t i = (size_t)tool->getUserData();
        if (i == 2) {
            
//            MoveBy* move = MoveBy::create(1.0, Vec2(-500,0));
//            EaseSineOut* esi = EaseSineOut::create(move);
//            tool->runAction(esi);
            
//            ActionHelper::showBezier(tool, toolPos[tool->getTag()], ActionHelper::ShowDirection::show_from_right);
//            
            this->runAction(Sequence::create(DelayTime::create(tool->getTag()*0.3),CallFunc::create([=](){
                
                log("-------m_vTool");
                
                  AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                ActionHelper::showBezier(tool, toolPos[tool->getTag()], ActionHelper::ShowDirection::show_from_right);
                
                
            }), NULL));
        }
    }
//    scheduleOnce(schedule_selector(MakeScene::MaterialComing4), 2.0);
}
void MakeScene::MaterialComing4(float dt)
{
    showHand();
    
    ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
    
        for (auto tool:m_vTool ) {
            tool->cancelEvent(false);
        }
        
    
    }));
    
    
    this->schedule(schedule_selector(MakeScene::checkStep), 0.5);
}
void MakeScene::showHand()
{
    if(!m_pHand){

        m_pHand = Sprite::create("cocostudio/ui/tips/finger_1.png");
        m_pHand->setAnchorPoint(Vec2(0.11,0.8));
        m_pHand->setPosition(toolPos[0]);
        m_pBG->addChild(m_pHand,100);

        m_pArrow = Sprite::create("cocostudio/ui/tips/down_3.png");
        m_pArrow->setPosition(Vec2(303,222));
        m_pArrow->setAnchorPoint(Vec2(0.08,0.15));
        m_pBowl->addChild(m_pArrow);
        
        
        Vec2 handPos = m_pHand->getPosition();

        m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        
            m_pHand->setVisible(true);
        
        
        }),BezierCreator::createParabola(1.5, handPos, m_pBowl->getPosition()),CallFunc::create([=](){
        
            m_pHand->setVisible(false);
            m_pHand->setPosition(handPos);
            
        
        }),DelayTime::create(2.0), NULL)));
        
        
    
    }

}
#pragma mark 响应拖拽事件
void MakeScene::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
//    
//    
//    m_pBowl->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.25,1.1)),EaseBackOut::create(ScaleTo::create(0.25,1.0)),nullptr));
//    
    if(m_pHand){
    
        m_pHand->stopAllActions();
        
        m_pArrow->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(),CallFunc::create([=](){
           
            
        }), NULL));
        m_pHand->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(),CallFunc::create([=](){
        
            m_pHand = nullptr;
        
        }), NULL));
    
    
    }
    
    oldPos = toolSprite->getPosition();
    int tag = toolSprite->getTag();
    m_iToolZorder = toolSprite->getLocalZOrder();
//    log("---->%d",m_iToolZorder);
    log("tag---->%d",tag);
    toolSprite->getParent()->reorderChild(toolSprite, 100);
    
    if (tag == EGG) {

        toolSprite->setTexture(g_path + "make/egg0.png");
        auto egg = Sprite::create(__String::createWithFormat("%smake/make_egg%d.png",g_path.c_str(),m_iEggNum +1)->getCString());
        egg->setPosition(toolSprite->getPosition());
        egg->setTag(101);
        m_pBG->addChild(egg,toolSprite->getLocalZOrder()-1);
        
        
    }else if(tag == STRAWBERRY){
    
        toolSprite->setTexture(g_path + "make/make_strawberry1.png");
        
        auto strawberry = Sprite::create(__String::create(g_path + "make/make_strawberry0.png")->getCString());
        strawberry->setPosition(toolSprite->getPosition());
        strawberry->setTag(102);
        m_pBG->addChild(strawberry);
        
    }else if (tag == SUGAR){
    
//        toolSprite->setTexture(g_path + "make/make_sugar0_1.png");
        
    }else if(tag == BLUEBERRY){
        
        toolSprite->setTexture(g_path + "make/make_blueberry1.png");
        
        auto blueberry = Sprite::create(g_path + "make/make_blueberry0.png");
        blueberry->setPosition(toolSprite->getPosition());
        m_pBG->addChild(blueberry);
        blueberry->setTag(200+BLUEBERRY);
        
    }else if(tag == WHISK){
    
        
        
        
    }
    
}
void MakeScene::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    
    if (tag == SUGAR) {
        
//        if (m_pSugarMove) {
//            m_pSugarMove = false;
//            
//             toolSprite->setTexture(g_path + "make/make_sugar1.png");
//            
//            auto sugar = Sprite::create(g_path + "make/make_sugar0.png");
//            sugar->setPosition(toolSprite->getPosition());
//            m_pBG->addChild(sugar,20);
//            sugar->setTag(300);
//        }
       
      
        
    }
    if(tag == WHISK){
    
        if (!m_bBowlIsMove) {
            m_bBowlIsMove = true;
            ActionUtils::moveToElasticInOut(m_pBowl,m_pBG->convertToNodeSpace(Director::getInstance()->getVisibleSize()/2),1.5,CallFunc::create([=](){
                
                
                Vec2 pos = m_pBG->convertToWorldSpace(m_pBowl->getPosition());
                Rect rect = Rect(pos.x-120,pos.y+10 - 80,230,80 + 60);
                m_pWhisk->clearRectVector();
                m_pWhisk->addRect(rect);
                
                
            }));
        }
    }
}
void MakeScene::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    for (auto tool:m_vTool ) {
        tool->cancelEvent(true);
    }
    int tag = toolSprite->getTag();
    
    
    if(tag == WHISK){
    
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        m_pWhisk->stopAllActions();
        m_pWhisk->setTexture(g_path + "make/make_whisk0.png");
    
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
            
            toolSprite->setTexture(__String::createWithFormat("%smake/make_egg%d.png",g_path.c_str(),m_iEggNum)->getCString());
        }else if(tag == STRAWBERRY){
        
            if (m_pBG->getChildByTag(102)) {
                m_pBG->getChildByTag(102)->removeFromParent();
            }
            
            
            toolSprite->setTexture(__String::create(g_path + "make/make_strawberry0.png")->getCString());
            
        }else if (tag == SUGAR){
//            m_pSugarMove = true;
//            toolSprite->setTexture(g_path + "make/make_sugar0.png");
//            if (m_pBG->getChildByTag(300)) {
//                m_pBG->getChildByTag(300)->removeFromParent();
//            }
            
            
            
        }else if(tag == BLUEBERRY){
        
            toolSprite->setTexture(g_path + "make/make_blueberry0.png");
        
            if (m_pBG->getChildByTag(200+BLUEBERRY)) {
                m_pBG->getChildByTag(200+BLUEBERRY)->removeFromParent();
            }
            
        }
        
        
        
        
    });
    MoveTo* move = MoveTo::create(0.5, oldPos);
    toolSprite->runAction(Sequence::create(move,func, NULL));
    
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
                
                auto tool = Sprite::create(__String::createWithFormat("%smake/make_%s1.png",g_path.c_str(),toolPath[tag].c_str())->getCString());
//                tool->setPosition(m_pBG->convertToWorldSpace(toolSprite->getPosition()));
                
                tool->setTag(102);
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(55 + 40 + 200,191 + 80 + 30 + 50)));
                this->addToContentLayer(tool,100);
                
                m_vTool.eraseObject(toolSprite);
//                toolSprite->removeFromParent();
//                tool->setRotation(-100);
                tool->runAction(Sequence::create(RotateTo::create(0.5,-100),CallFunc::create([=](){
                    
                    auto particle = ParticleSystemQuad::create("cocostudio/particles/milkParticle.plist");
                    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                    particle->setVisible(true);
                    particle->setDuration(3.0);
                    particle->setPosition(Vec2::ZERO);
                    particle->setRotation(100);
                    particle->setScale(0.7);
//                    particle->setScaleX(0.3);
                    tool->addChild(particle,2);
                    particle->setPosition(tool->getContentSize() + Size(-130+100-92 - 50                                                                                                                                                                                                                                                  ,-35+5 + 20));
                    
                    auto milk = Sprite::create(g_path + "make/bowl_milk.png");
                    milk->setAnchorPoint(Vec2(0.5,0));
                    milk->setPosition(Vec2(m_pBowl->getContentSize().width/2,0));
                    m_pBowl->addChild(milk,-1);
                    milk->setOpacity(0);
                    
                    auto milk1 = Sprite::create(g_path + "make/bowl_milk1.png");
                    milk1->setAnchorPoint(Vec2(0.5,0));
                    milk1->setPosition(Vec2(m_pBowl->getContentSize().width/2,0));
                    m_pBowl->addChild(milk1,20);
                    milk1->setOpacity(0);
                    
                    AudioHelp::getInstance()->playLoofEffect("add milk.mp3");
                    
                    auto func = CallFunc::create([=](){

                        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                        m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                        tool->setVisible(false);
                        for (auto tool:m_vTool ) {
                            tool->cancelEvent(false);
                        }
                     
                        AudioHelp::getInstance()->stopLoofEffect();
                        m_iStepNum++;
                    });
                    milk->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(3.0),func,nullptr));
                    milk1->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(3.0),nullptr));
                }), NULL));
              
                
                
            }
            break;
        case FLOUR:
            {
                m_dAccX = 0.0;
                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
                
                auto tool = Sprite::create(__String::createWithFormat("%smake/make_%s1.png",g_path.c_str(),toolPath[tag].c_str())->getCString());
                //                tool->setPosition(m_pBG->convertToWorldSpace(toolSprite ->getPosition()));
                tool->setTag(101);
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(318,310 - 10)));
//                m_pBowl->addChild(tool,100);
                this->addToContentLayer(tool,100);
                
                tool->runAction(Sequence::create(RotateTo::create(0.5,-80),CallFunc::create([=](){
                
                 
                    auto func = CallFunc::create([=](){
                        
//                        tool->setVisible(false);
                        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                        m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                        tool->removeAllChildren();
                        tool->runAction(Sequence::create(Spawn::create(RotateTo::create(1.0,0),MoveBy::create(1.0, Vec2(1000,0)), NULL), NULL));
                        
                        for (auto tool:m_vTool ) {
                            tool->cancelEvent(false);
                        }
                        auto milk = Sprite::create(g_path + "make/bowl_flour.png");
                        milk->setAnchorPoint(Vec2::ZERO);
                        milk->setPosition(Vec2(0,10));
                        //            milk->setPosition(Vec2(0 + 50,m_pBowl->getContentSize().height/2));
                        m_pBowl->addChild(milk);
                        m_iStepNum++;
                        m_dAccX = 0.0;
                    });
                
                    auto particle = ParticleSystemQuad::create("cocostudio/particles/sugarParticle.plist");
                    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                    particle->setVisible(true);
                    particle->setRotation(80);
                    particle->setScale(0.7);
                    particle->setDuration(1.0);
                    particle->setPosition(tool->getContentSize() + Size(-130-40                                                                                                                                                                                                                                                  ,-5 - 20));
                    tool->addChild(particle,2);
                    
                    AudioHelp::getInstance()->playEffect("add sugar.mp3");
                    
                    ActionUtils::delayTimeAction(this,1.5,func);
                }),nullptr));
             }
                break;
        case CHOCOLATE:
            {
                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
                
                auto tool = Sprite::create(__String::create(g_path + "make/make_chocolate1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(111,90))+Vec2(-49,72 + 100));
                this->addToContentLayer(tool);

                MoveBy* move1 = MoveBy::create(0.25,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.25,Vec2(0,10));
                
                auto func = CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
                    auto chocolate = Sprite::create(g_path + "make/bowl_chocolate.png");
                    chocolate->setPosition(Vec2(40,m_pBowl->getContentSize().height/2 - 35 - 100 + 40));
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    m_iStepNum++;
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                
                });
                auto func1 = CallFunc::create([=](){
                 
                    MoveBy* move1 = MoveBy::create(0.5, Vec2(0,-70));
                    EaseSineIn* esi1 = EaseSineIn::create(move1);
                    
                    MoveBy* move2 = MoveBy::create(0.5, Vec2(0,-70));
                    EaseSineIn* esi2 = EaseSineIn::create(move2);
                    
                    MoveBy* move3 = MoveBy::create(0.5, Vec2(0,-70));
                    EaseSineIn* esi3 = EaseSineIn::create(move3);
                    
                    auto chotelate = Sprite::create(g_path + "make/chocolate.png");
                    chotelate->setAnchorPoint(Vec2(0.5,1));
                    chotelate->setPosition(Vec2(241,-5));
                    chotelate->setRotation(90);
                    tool->addChild(chotelate,2);
                    
                    chotelate->runAction(Sequence::create(esi1, RemoveSelf::create(),NULL));
                    
                    
                    auto chotelate1 = Sprite::create(g_path + "make/chocolate.png");
                    chotelate1->setAnchorPoint(Vec2(0.5,1));
                    chotelate1->setPosition(Vec2(241,-5));
                    chotelate1->setRotation(90);
                    chotelate1->setVisible(false);
                    tool->addChild(chotelate1,2);
                    
                    chotelate1->runAction(Sequence::create(DelayTime::create(0.15),CallFunc::create([=](){
                    
                         chotelate1->setVisible(true);
                    
                    }),esi2, RemoveSelf::create(),NULL));
                    
                    auto chotelate2 = Sprite::create(g_path + "make/chocolate.png");
                    chotelate2->setAnchorPoint(Vec2(0.5,1));
                    chotelate2->setPosition(Vec2(241,-5));
                    chotelate2->setRotation(90);
                    chotelate2->setVisible(false);
                    tool->addChild(chotelate2,2);
                    
                    chotelate2->runAction(Sequence::create(DelayTime::create(0.35),CallFunc::create([=](){
                        
                        chotelate2->setVisible(true);
                        
                    }),esi3, RemoveSelf::create(),NULL));
                    
                    
                    AudioHelp::getInstance()->playEffect("pour ice.mp3");
                    
                    
                 
                });
                auto func2 = CallFunc::create([=](){
                    
//                    m_pBowl->runAction(Sequence::create(DelayTime::create(1.0),ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    auto chocolate = Sprite::create(g_path + "make/bowl_chocolate.png");
                    chocolate->setPosition(Vec2(100,m_pBowl->getContentSize().height/2 - 35 - 100));
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    chocolate->setScaleY(0.5);
                    
                    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
                });
                tool->runAction(Sequence::create(move1,move1->reverse(),func1,DelayTime::create(1.0),func2,move2,move2->reverse(),func1,DelayTime::create(1.0),RemoveSelf::create(),func,NULL));
                
//                toolSprite->setDelegate(nullptr);
                m_vTool.eraseObject(toolSprite);
//                toolSprite->removeFromParent();
                
            }
            
            break;
        case GREENTEA:
            {
                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
                
                auto tool = Sprite::create(__String::create(g_path + "make/make_greentea1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(106,131))+Vec2(-58 - 50,130));
                this->addToContentLayer(tool);
                
                RotateBy* rota = RotateBy::create(0.35, 90);
                MoveBy* move1 = MoveBy::create(0.25,Vec2(0,10));
                MoveBy* move2 = MoveBy::create(0.25,Vec2(0,10));

                
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    auto chocolate = Sprite::create(g_path + "make/bowl_greentea.png");
                    chocolate->setPosition(Vec2(0 + 50,m_pBowl->getContentSize().height/2 - 35 - 80 + 30));
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    m_iStepNum++;
                });
                auto func2 = CallFunc::create([=](){
                    
                    auto chocolate = Sprite::create(g_path + "make/bowl_greentea.png");
                    chocolate->setPosition(Vec2(0 + 50,m_pBowl->getContentSize().height/2 - 35 - 80));
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    chocolate->setScaleY(0.5);
                    
                });
                auto func1 = CallFunc::create([=](){

                    auto particle = ParticleSystemQuad::create("cocostudio/particles/sugarParticle1.plist");
                    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                    particle->setVisible(true);
                    particle->setScale(0.7);
                    particle->setAnchorPoint(Vec2(0,1));
                    particle->setDuration(0.25);
                    particle->setRotation(-90);
                    particle->setPosition(Vec2(140 + 20,183));
                    tool->addChild(particle,2);
                    
                    AudioHelp::getInstance()->playEffect("add sugar.mp3");
                   
                });
                
                
                tool->runAction(Sequence::create(rota,move1,move1->reverse(),func1,DelayTime::create(1.0),func2,move2,move2->reverse(),func1,DelayTime::create(1.0),RemoveSelf::create(),func,NULL));
//                toolSprite->setDelegate(nullptr);
                m_vTool.eraseObject(toolSprite);
//                toolSprite->removeFromParent();
            }
            
            break;
        case EGG:
            {
                
                Vec2 pos = Vec2(m_pBowl->getPositionX() - 132,m_pBowl->getPositionY()+118);
                
                auto func = CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    auto egg = Sprite::create(g_path + "make/egg.png");
                    egg->setPosition(Vec2(87+ (m_iEggNum-1)*80,150 + 40));
                    m_pBowl->addChild(egg);
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    m_iStepNum++;
                });
                
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile(g_path + "make/egg1.png");
                pOvenAnimation->addSpriteFrameWithFile(g_path + "make/egg2.png");
                pOvenAnimation->addSpriteFrameWithFile(g_path + "make/egg3.png");
                
                pOvenAnimation->setDelayPerUnit(0.4);
                pOvenAnimation->setLoops(1);
                
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                
                auto func1 = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("crack egg.mp3");
                    
                });
                
                
                MoveTo* move = MoveTo::create(0.5, pos);
                MoveBy* move1 = MoveBy::create(0.35,Vec2(24 + 30,-20+10));
                auto move2 = MoveBy::create(0.35,Vec2(24+30,-20+10));
                MoveBy* move3 = MoveBy::create(0.5, Vec2(100 + m_iEggNum*80,0 - 50));
                toolSprite->runAction(Sequence::create(move,/*move1,move1->reverse(),*/move2,move2->reverse(),move3,func1,pOvenAnimat,CallFunc::create([=](){
                
                    m_iEggNum++;
                    log("m_iEggNum---->%d",m_iEggNum);
                    if (m_pBG->getChildByTag(101)) {
                        m_pBG->getChildByTag(101)->removeFromParent();
                    }
                    toolSprite->setPosition(oldPos);
                    toolSprite->setTexture(__String::createWithFormat("%smake/make_egg%d.png",g_path.c_str(),m_iEggNum)->getCString());
                
                    if (m_iEggNum==2) {
                        toolSprite->runAction(FadeOut::create(1.0));
                        
                        m_vTool.eraseObject(toolSprite);
                        toolSprite->setVisible(false);
                    };
                    
                    
                }),func,NULL));

            }
            
            break;
       
        case SUGAR:
            {
              
//              toolSprite->setDelegate(nullptr);
                m_vTool.eraseObject(toolSprite);
//              toolSprite->removeFromParent();
//                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
                AudioHelp::getInstance()->playEffect("add.mp3");
                
                toolSprite->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->convertToWorldSpace(Vec2(338,312 + 50))),RotateTo::create(0.5,-30),CallFunc::create([=](){

                    auto particle = ParticleSystemQuad::create("cocostudio/particles/sugarParticle.plist");
                    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
                    particle->setVisible(true);
                    particle->setScale(0.7);
                    particle->setAnchorPoint(Vec2(1,1));
                    particle->setDuration(0.25);
                    particle->setPosition(Vec2(12 - 20,9 + 40));
                    toolSprite->addChild(particle,2);
                    
                    AudioHelp::getInstance()->playEffect("add sugar.mp3");
                
                
                }),DelayTime::create(1.0),CallFunc::create([=](){
                
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    auto sugar = Sprite::create(g_path + "make/bowl_sugar.png");
                    sugar->setPosition(m_pBowl->getContentSize()/2);
                    sugar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_pBowl->addChild(sugar);
                    sugar->setPosition(sugar->getPosition() + Vec2(-150,-90 + 40));
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    
                    m_iStepNum++;

                    
                
                }),RotateTo::create(0.25,0),EaseElasticIn::create(MoveBy::create(1.0, Vec2(500,0)), 1.0), NULL));
            }
            break;
        case VANILLA:
            {
                toolSprite->setVisible(false);
                
                auto tool = Sprite::create(__String::create(g_path + "make/make_vanilla1.png")->getCString());
                tool->setPosition(m_pBowl->convertToWorldSpace(Vec2(4,153))+Vec2(23,96));
                this->addToContentLayer(tool);
                
                RotateBy* rota = RotateBy::create(0.5, 90);
          
                auto func = CallFunc::create([=](){
                    
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    auto chocolate = Sprite::create(g_path + "make/bowl_vanilla.png");
                    
                    chocolate->setPosition(Vec2(103,90 + 40));
                    chocolate->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(chocolate);
                    
                    
                    m_iStepNum++;
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    
                });
                auto func1 = CallFunc::create([=](){
                    
                    auto oil = Sprite::create(g_path + "make/oil drop.png");
                    oil->setAnchorPoint(Vec2(0.5,1));
                    oil->setPosition(Vec2(100,286));
                    oil->setRotation(90 - 180);
                    tool->addChild(oil,2);
                    
                    oil->runAction(Sequence::create(MoveBy::create(0.5, Vec2(100,0)), RemoveSelf::create(),NULL));
                    
                    AudioHelp::getInstance()->playEffect("drop_oil.mp3");
                    
                    
                });
                tool->runAction(Sequence::create(rota,func1,DelayTime::create(0.75),func1,DelayTime::create(0.75),func1,DelayTime::create(0.75),func1,DelayTime::create(1.0),RemoveSelf::create(),func,NULL));
                
//                toolSprite->setDelegate(nullptr);
                m_vTool.eraseObject(toolSprite);
                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
            }
            
            break;
        case STRAWBERRY:
            {
//                toolSprite->setDelegate(nullptr);
                m_vTool.eraseObject(toolSprite);
                toolSprite->setVisible(false);
                toolSprite->cancelEvent(true);
                AudioHelp::getInstance()->playEffect("add.mp3");
                
                
                auto strawberry = Sprite::create(g_path + "make/bowl_strawberry.png");
//                strawberry->setAnchorPoint(Vec2::ZERO);
                strawberry->setPosition(Vec2(0 + 100,m_pBowl->getContentSize().height/2 - 40 + 40));
                strawberry->setAnchorPoint(Vec2::ZERO);
                m_pBowl->addChild(strawberry);
                
                strawberry->setPositionY(strawberry->getPositionY()+100);
                
                
                MoveBy* move2 = MoveBy::create(0.5, Vec2(0,-160));
                EaseSineIn* esi = EaseSineIn::create(move2);
                auto func = CallFunc::create([=](){
                    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                    m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                    m_iStepNum++;
                    
                    if (m_pBG->getChildByTag(102)) {
                        m_pBG->getChildByTag(102)->runAction(FadeOut::create(0.5));
                        
                    }
                    
                    for (auto tool:m_vTool ) {
                        tool->cancelEvent(false);
                    }
                    
                });
              
                strawberry->runAction(Sequence::create(esi,CallFunc::create([=](){
                
//                    m_pBowl->runAction(Sequence::create(DelayTime::create(1.0),ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                
                }), DelayTime::create(0.5),func,NULL));
                
                
                
                
                
                
                
            }
            
            break;
        case WHISK:
            {
                m_pWhisk->setTexture(g_path + "make/make_whisk1.png");
                
                if (m_bWhiskAction) {
                    m_bWhiskAction = false;
                    
                    auto pOvenAnimation = Animation::create();
                    
                    pOvenAnimation->addSpriteFrameWithFile(g_path + "make/make_whisk1.png");
                    pOvenAnimation->addSpriteFrameWithFile(g_path + "make/make_whisk1_1.png");
                    pOvenAnimation->addSpriteFrameWithFile(g_path + "make/make_whisk1_2.png");
                    pOvenAnimation->setDelayPerUnit(0.2);
                    pOvenAnimation->setLoops(-1);
                    
                    auto pOvenAnimat = Animate::create(pOvenAnimation);
                    
                    m_pWhisk->runAction(pOvenAnimat);
                    
                }
                
//                    m_iWhiskNum =  m_iWhiskNum + 0.5;
                    m_iWhiskNum++;
                
                
                    if (m_bIsSounds) {
                        AudioHelp::getInstance()->playEffect("stir the batter.mp3");
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
                    
                    if (m_iWhiskNum == 80) {
                        
                        if(!m_pBowlMix){
                            
                            m_pBowl->removeAllChildren();
                            
                            
                            auto bowl = Sprite::create(g_path + "make/make_bowl1.png");
                            bowl->setAnchorPoint(Vec2::ZERO);
                            m_pBowl->addChild(bowl,100);
                            
                            
                            m_pBowlMix = Sprite::create(__String::createWithFormat("%smake/mix_%s0.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
                            m_pBowlMix->setPosition(m_pBowl->getContentSize()/2);
                            m_pBowl->addChild(m_pBowlMix,99);
                            
                        
                        }
                        
                        
                    }else if(m_iWhiskNum == 160){
                    
//                        m_pBowl->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.25,1.1)),EaseBackOut::create(ScaleTo::create(0.25,1.0)),nullptr));
                        m_pBowlMix->setTexture(__String::createWithFormat("%smake/mix_%s1.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
                        
                    }else if(m_iWhiskNum == 190){
                        
//                        m_pBowl->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.25,1.1)),EaseBackOut::create(ScaleTo::create(0.25,1.0)),nullptr));
                        m_pBowlMix->setTexture(__String::createWithFormat("%smake/mix_%s2.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
                        
                    }else if(m_iWhiskNum == 260){
                        
                        m_pBowlMix->setTexture(__String::createWithFormat("%smake/mix_%s3.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
                        
                        m_pWhisk->cancelEvent(true);
                        
                        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                       m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                        
                        
                        auto func = CallFunc::create([=](){

                            ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");
                            
                            emitter1->setPosition(m_pBowl->getContentSize()/2);
                            
                            emitter1->setAutoRemoveOnFinish(true);
                            
                            m_pBowl->addChild(emitter1,200);
                            
                            AudioHelp::getInstance()->playEffect("all star .mp3");
                            
                            showNextButton(true);
                            
                            auto light = Sprite::create("cocostudio/ui/public/light.png");
                            light->setPosition(m_pNextButton->getContentSize()/2);
                            m_pNextButton->addChild(light,-1);
                            
                            light->runAction(RepeatForever::create(RotateBy::create(1,90)));
                            
                            m_pNextButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),Sequence::create(ScaleTo::create(0.3, 1.1),ScaleTo::create(0.3, 1), nullptr), nullptr)));
                            
                        });
                        
                        m_pWhisk->runAction(Sequence::create(MoveBy::create(0.5,Vec2(700,0)),func,nullptr));
                    }
                    
                    
                    
            }
       
            break;
        case BUTTER:
        {
//            AudioHelp::getInstance()->playEffect("add.mp3");
            
            toolSprite->cancelEvent(true);
//            toolSprite->setIsMove(false);
//            toolSprite->setDelegate(nullptr);
            
            m_vTool.eraseObject(toolSprite);
//            toolSprite->removeFromParent();
//            toolSprite->setVisible(false);
//            auto sugar = Sprite::create(g_path + "make/make_butter1.png");
//            sugar->setPosition(Vec2(0 - 50,m_pBowl->getContentSize().height/2-50 - 37));
//            sugar->setAnchorPoint(Vec2::ZERO);
//            m_pBowl->addChild(sugar);
            
            toolSprite->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->convertToWorldSpace(Vec2(137,345))),CallFunc::create([=](){
            
            }),RotateTo::create(0.5,30),CallFunc::create([=](){
            
                Sprite* butter = dynamic_cast<Sprite*>(toolSprite->getChildByTag(100));
//                butter->setVisible(false);
//                 AudioHelp::getInstance()->playEffect("add buter.mp3");
                butter->runAction(Sequence::create(MoveBy::create(0.5, Vec2(135,-68)),CallFunc::create([=](){
                
                    butter->setVisible(false);
                    auto bowl_butter = Sprite::create(g_path + "make/butter.png");
                    bowl_butter->setPosition(m_pBowl->convertToNodeSpace(toolSprite->convertToWorldSpace(butter->getPosition()))+Vec2(0,40));
                    bowl_butter->setRotation(30);
                    bowl_butter->setAnchorPoint(Vec2::ZERO);
                    m_pBowl->addChild(bowl_butter);
                    
                    
                    bowl_butter->runAction(Sequence::create(MoveTo::create(0.5,Vec2(107,70)),CallFunc::create([=](){
                    
                        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                        m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
                        toolSprite->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5,Vec2(-500,0)), 1.0), NULL));
                    
                    
                    }),DelayTime::create(0.5),CallFunc::create([=](){
                    
                        for (auto tool:m_vTool ) {
                            tool->cancelEvent(false);
                        }
                        
                        m_iStepNum++;

                    }),NULL));
                    
                    
                 }), NULL));
            
            }),NULL));
        
//            sugar->setPositionY(sugar->getPositionY()+100);
//            
//            
//            MoveBy* move2 = MoveBy::create(0.5, Vec2(0,-100));
//            EaseSineIn* esi = EaseSineIn::create(move2);
//            auto func = CallFunc::create([=](){
//            });
//            
//            
//            sugar->runAction(Sequence::create(esi, DelayTime::create(0.5),func,NULL));
            
            
            

        }
            break;
        case BLUEBERRY:
        {
            AudioHelp::getInstance()->playEffect("add.mp3");
            
//            toolSprite->setDelegate(nullptr);
            m_vTool.eraseObject(toolSprite);
            toolSprite->setVisible(false);
            toolSprite->cancelEvent(true);
            auto blueberry = Sprite::create(g_path + "make/bowl_blueberry.png");
            blueberry->setPosition(Vec2(0 + 50,m_pBowl->getContentSize().height/2 - 45 + 40));
            blueberry->setAnchorPoint(Vec2::ZERO);
            m_pBowl->addChild(blueberry);
            
            blueberry->setPositionY(blueberry->getPositionY()+100);
            
            MoveBy* move2 = MoveBy::create(0.5, Vec2(0,-200));
            EaseSineIn* esi = EaseSineIn::create(move2);
            auto func = CallFunc::create([=](){
                for (auto tool:m_vTool ) {
                    tool->cancelEvent(false);
                }
                if (m_pBG->getChildByTag(200+BLUEBERRY)) {
                    m_pBG->getChildByTag(200+BLUEBERRY)->removeFromParent();
                }
                m_iStepNum++;
            });

            
            blueberry->runAction(Sequence::create(esi, CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.1),ScaleTo::create(0.17,1.0),nullptr));
            
            
            }),DelayTime::create(0.5),func,NULL));
        }
            break;
            
        default:
            break;
    }
    
}
void MakeScene::onTouchMoveOutRect(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == WHISK) {
        
        m_pWhisk->stopAllActions();
        m_pWhisk->setTexture(g_path + "make/make_whisk0.png");
        m_bWhiskAction = true;
        
    }



}
#pragma mark 控制步骤
void MakeScene::checkStep(float dt)
{
    log("--->m_istep----->%d",m_iStepNum);
    
    if(m_iStepNum == 4){
    
        m_iStepNum++;
        scheduleOnce(schedule_selector(MakeScene::MaterialComing3), 0.3);
    
    }
    
    
    
    if (m_iStepNum == 8) {
      
        this->unschedule(schedule_selector(MakeScene::checkStep));
        
        
        auto func = CallFunc::create([=](){
        
            m_pWhisk->cancelEvent(false);
        });
        AudioHelp::getInstance()->playEffect("swish.mp3");
        MoveBy* move = MoveBy::create(1.0, Vec2(-500,0));
        EaseElasticOut* ease = EaseElasticOut::create(move,1.0);
//        ActionUtils::moveByElasticOut(m_pWhisk, Vec2(-500,0),1.0,func);
        m_pWhisk->runAction(Sequence::create(DelayTime::create(1.0),ease,func, NULL));
    }
}
#pragma mark 下一步
void MakeScene::nextStep()
{
    log("next");
    hideNextButton();
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    SceneChangeManager->enterOvenScene();
}