//
//  MakeOtherStep1.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/6.
//
//

#include "MakeOtherStep1.h"
#include "CircleBy.h"
#include "DrawLine.h"
#include "BezierCreator.h"
//REGIST_SECENE(MakeOtherStep1)


enum stepOther1ToolType{
    
    kstepOther1Water = 0,
    kstepOther1Sugar,
    kstepOther1Salt,
    kstepOther1Butter,
    kstepOther1Flour,
    kstepOther1Egg1,
    kstepOther1Egg2,
    kstepOther1Vanilla,
    kstepOther1Chocolate,
    kstepOther1Mint,
    kstepOther1Strawberry,
    
};

//资源名字以此为准
string MakeOtherStep1ToolPath[11]={"water","sugar","salt","butter","flour","gee1","gee2","vanilla","chocolate","mint","strawberry"};
float vanilla1[3]={0.3,0.6,1.0};
MakeOtherStep1::MakeOtherStep1()
{
    m_iStepNum = 0;
    _bowlPos = Vec2::ZERO;
    _LayerPos= Vec2::ZERO;
    m_bIsShowHand = true;
}
MakeOtherStep1::~MakeOtherStep1()
{
    
}
bool MakeOtherStep1::init()
{
    
    if(!BaseLayer::init())
        return false;
    
    m_iStepNum = 0;
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeOtherCsb/MakeOtherStep1.csb");
    addChild(_studioLayer);

    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    
    //显示工具
    _bowlPos = m_pBowl->getPosition();
    _LayerPos = _studioLayer->getNodeByName<Layout*>("layer")->getPosition();
//    _studioLayer->getNodeByName<Layout*>("layer")->setLocalZOrder(20);
    
    _detalPos = _LayerPos- _bowlPos;
    
    for (int i = 0 ;i<11;i++) {
        auto sprite = _studioLayer->getNodeByName<Sprite*>(MakeOtherStep1ToolPath[i].c_str());
        sprite->setLocalZOrder(i);
        sprite->setTag(i);
        
        auto lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, sprite);
        lisenter->setOrginOffset();
        lisenter->addRectByNode(m_pBowl);
        
        lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep1::onTouchDown, this);
        lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep1::onTouchMoveInRect, this);
        lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep1::onTouchUp, this);
        
        m_vTool.pushBack(sprite);
        
        toolPos[i]=sprite->getPosition();
        sprite->setPosition(sprite->getPosition()+Vec2(1000,-500));
        
//        if(i < 4)
//            ActionHelper::showBezier(sprite, toolPos[i], ActionHelper::ShowDirection::show_from_right,1.0 + 0.2*i);
//        
    }
    m_pBowl->setPosition(m_pBowl->getPosition()+Vec2(-1000,0));
    m_iStepNum = 0;
    
    auto title = Sprite::create("cocostudio/UI/tips/Addingredients.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    return true;
}
void MakeOtherStep1::onEnter()
{
    BaseLayer::onEnter();
    
    AudioHelp::getInstance()->playEffect("banner drop.mp3");
    
    
    
    PopDialogType type = newFlavor;
    
    string gameState = UserDefault::getInstance()->getStringForKey(GameState);
    
    if(gameState == GameStateTwo){
    
        if (GameDataInstance->getPopDialogType() == PopDialogTypeNewFlavor) {
            
            type = newFlavor;
            
        }
    }else if(gameState == GameStateThree){
    
        m_pBowl->runAction(Sequence::create(Spawn::create(MoveBy::create(1.0,Vec2(1000,0)),EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL), NULL));
        
        
        showTool(0);
        return;
        
        
        
    }
    
    PopDialogLayer* layer = PopDialogLayer::create(type);
    layer->setAnchorPoint(Vec2::ZERO);
    this->addChild(layer);
    layer->setListener([=](KSPopLayer*, KSPopLayer::TouchEventType touch){
        if(touch == KSPopLayer::TouchEventType::ENDED){
            
            layer->removeFromParent();
            showTool(0);
            
        }
    });

    layer->btnClose=[=](){
        
        layer->removeFromParent();
        showTool(0);
        
    };
    
    m_pBowl->runAction(Sequence::create(DelayTime::create(1.0),Spawn::create(MoveBy::create(1.0,Vec2(1000,0)),EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL), NULL));
    
    
}
void MakeOtherStep1::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherStep1::onExit()
{
    BaseLayer::onExit();
}
void MakeOtherStep1::showTool(float dt)
{
    for(auto tool : m_vTool){
    
        log("%d",tool->getTag());
        if(tool->getTag()<4){
        
            ActionUtils::delayTimeAction(this,1.0 + 0.2*tool->getTag(),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                
            }));
            
            ActionHelper::showBezier(tool, toolPos[tool->getTag()], ActionHelper::ShowDirection::show_from_right,1.0 + 0.2*tool->getTag(),[=](){
                
                if(tool->getTag() == 3){
                    
                    if (this->getChildByTag(1001)) {
                        
                        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
                        
                    }
                    
                    showHand();
                    AudioHelp::getInstance()->playEffect("VoiceOver/Add ingredients.mp3");
                }
            });
        
        }
    }
}
void MakeOtherStep1::showHand()
{
    if (m_bIsShowHand) {
        
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setVisible(true);
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setLocalZOrder(200);
        auto hand = _studioLayer->getNodeByName<Sprite*>("hand");
        hand->setVisible(true);
        hand->setLocalZOrder(210);
        //        ActionHelper::runBezierMoveHideActi->setLocalZOrder(i)on(hand,Vec2(553,434),Vec2(328,303));
        
        vector<Vec2> points = {
            Vec2(560.865417,404.860016),
            Vec2(334.238617,492.715332),
            Vec2(371.177765,342.961914),
            Vec2(270.343811,274.075378)
        };
        
        hand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            
            hand->setVisible(true);
        }),BezierCreator::createAction(points, 2.0),CallFunc::create([=](){
            
            hand->setVisible(false);
            
        }),DelayTime::create(2.0), NULL)));
        
//        auto drawLine = DrawLine::create();
//        drawLine->setContentSize(hand->getParent()->getContentSize());
//        hand->getParent()->addChild(drawLine,30);
//        drawLine->setAcctionNode(hand);
        
    }

}
void MakeOtherStep1::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if(m_bIsShowHand){
        m_bIsShowHand=false;
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setVisible(false);
        auto hand = _studioLayer->getNodeByName<Sprite*>("hand");
        hand->stopAllActions();
        hand->setVisible(false);
        
    }
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
    
    if(node->getTag()==kstepOther1Vanilla){
        
        node->setCascadeOpacityEnabled(false);
        node->runAction(FadeOut::create(0.2));
        
        auto pour = node->getChildByName("pour");
        if (pour) {
            pour->setVisible(true);
            //                    pour->runAction(FadeIn::create(0.2));
        }
        
    }
    
}
void MakeOtherStep1::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    lisenter->setEnabled(false);
   
//  m_iStepNum
    log("m_iStepNum--->%d",m_iStepNum);
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
//    auto layer = _studioLayer->getNodeByName<Layout*>("layer");

//    layer->runAction(Spawn::create(ScaleTo::create(1.0,1.1),MoveTo::create(1.0,layer->getParent()->getContentSize()/2),nullptr));
//    
    int tag = node->getTag();
//    node->retain();
//    node->removeFromParent();
//    layer->addChild(node);
  
    switch (tag) {
        case kstepOther1Water:
        {
            node->runAction(Sequence::create(MoveTo::create(0.5,Vec2(354 + 100,443)),RotateTo::create(0.5,-90),CallFunc::create([=](){
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                    //                    pour->runAction(FadeIn::create(0.2));
                }
                
                //添加水
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                auto water = Sprite::create("cocostudio/Make1/bowl_water.png");
                water->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(water);
                water->setScale(0.6);
                water->setOpacity(0);
                water->runAction(Sequence::create(Spawn::create(FadeIn::create(2.0),ScaleTo::create(2.0,1.0),nullptr), NULL));
                
                 AudioHelp::getInstance()->playEffect("add water.mp3");
                
            }),CircleBy::create(2.0,Vec2(-60,-20),60,360*1.5),MoveTo::create(0.5,Vec2(412,480)),CallFunc::create([=](){
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
                
            }),Spawn::create(RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),CallFunc::create([=](){
                
            }),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
              m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::showNextTool), 0.2);
                    
                }
                
                
            }), NULL));
            
        }
            break;
        case kstepOther1Sugar:
        {
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(235,306)),RotateTo::create(1.0,-110),nullptr),CallFunc::create([=](){
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                    //                    pour->runAction(FadeIn::create(0.2));
                }
                
                //粒子效果
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                
                auto sugar_bowl = Sprite::create("cocostudio/Make1/bowl_sugar.png");
                sugar_bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(sugar_bowl);
                sugar_bowl->setOpacity(0);
                sugar_bowl->runAction(FadeIn::create(2.2));
                
                AudioHelp::getInstance()->playEffect("add_powder.mp3");
                
            }),MoveBy::create(2.0,Vec2(0,280)),CallFunc::create([=](){
                
                //完成
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
                
            }),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),RotateTo::create(1.0,0),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                 Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::showNextTool), 0.2);
                    
                }
            }), NULL));
            
        }
            break;
        case kstepOther1Salt:
        {
            auto salt_bowl1 = Sprite::create("cocostudio/Make1/bowl_salt.png");
            salt_bowl1->setPosition(m_pBowl->getContentSize()/2);
            m_pBowl->addChild(salt_bowl1);
            salt_bowl1->setScale(0.5);
            salt_bowl1->setOpacity(0);
            
            auto salt_bowl = Sprite::create("cocostudio/Make1/bowl_salt.png");
            salt_bowl->setPosition(m_pBowl->getContentSize()/2);
            m_pBowl->addChild(salt_bowl);
            salt_bowl->setOpacity(0);
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(282,446)),RotateTo::create(1.0,-144.2),nullptr),CallFunc::create([=](){
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                    //                    pour->runAction(FadeIn::create(0.2));
                }
                
                
            }),JumpBy::create(0.5, Vec2::ZERO, 50,1),CallFunc::create([=](){
                
                //粒子效果
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                //                particle->setDuration(0.5);
                
                salt_bowl1->runAction(FadeIn::create(0.6));
                
                AudioHelp::getInstance()->playEffect("add salt.mp3");
                
            }),DelayTime::create(0.5),CallFunc::create([=](){
                
                salt_bowl->runAction(FadeIn::create(0.6));
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->stopSystem();
                
            }),DelayTime::create(0.5),JumpBy::create(0.5, Vec2::ZERO, 50,1),CallFunc::create([=](){
                
                //粒子效果
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle1"));
                particle->setVisible(true);
                particle->resetSystem();
                
                salt_bowl->runAction(FadeIn::create(0.6));
                
                AudioHelp::getInstance()->playEffect("add salt.mp3");
                
            }),DelayTime::create(0.5),CallFunc::create([=](){
                
                //完成
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle1"));
                particle->stopSystem();
                
            }),DelayTime::create(0.5),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::showNextTool), 0.2);
                    
                }
            }), NULL));
        }
            break;
        case kstepOther1Butter:
        {
            auto butter = dynamic_cast<Sprite*>(node);
            auto butterBowl = Sprite::create("cocostudio/Make1/bowl_butter3.png");
            butterBowl->setPosition(m_pBowl->getContentSize()/2);
            m_pBowl->addChild(butterBowl);
//            butterBowl->setVisible(false);
            butterBowl->setOpacity(0);
            butter->runAction(Sequence::create(MoveTo::create(1.0,Vec2(267,402 + 100)),RotateTo::create(0.5,-30),CallFunc::create([=](){
                butter->setTexture("cocostudio/Make1/butter1.png");
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                butterBowl->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(1.0),CallFunc::create([=](){}), NULL));
                
                AudioHelp::getInstance()->playEffect("add buter.mp3");
                
                butterBowl->runAction(FadeIn::create(2.00));
                
            }),DelayTime::create(1.0),CallFunc::create([=](){
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                //                particle->setVisible(true);
                particle->stopSystem();
                
                butter->setTexture("cocostudio/Make1/butter4.png");
                
            }),DelayTime::create(1.0),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::showNextTool), 0.2);
                    
                }
                
            }),NULL));
        }
            break;
        case kstepOther1Flour:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(299,317)),RotateTo::create(1.0,-90),nullptr),CallFunc::create([=](){
                
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                    //                    pour->runAction(FadeIn::create(0.2));
                }
                
                //粒子效果
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                auto sugar_bowl = Sprite::create("cocostudio/Make1/bowl_flour.png");
                sugar_bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(sugar_bowl);
                sugar_bowl->setOpacity(0);
                sugar_bowl->runAction(FadeIn::create(2.2));
                
                 AudioHelp::getInstance()->playEffect("add_powder.mp3");
                
            }),MoveBy::create(2.0,Vec2(0,300)),CallFunc::create([=](){
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
                particle->stopSystem();
                
            }),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),RotateTo::create(1.0,0),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                
            }), NULL));
        }
            break;
        case kstepOther1Vanilla:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            node->runAction(Sequence::create(MoveTo::create(0.5,Vec2(302 - 50,437 + 50)),RotateTo::create(0.5,-131.43),CallFunc::create([=](){
                
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                    //                    pour->runAction(FadeIn::create(0.2));
                }
                
                for(int i =0;i<3;i++){
                    
                    auto gum = node->getChildByName(__String::createWithFormat("gum%d",i)->getCString());
                    
                    gum->runAction(Sequence::create(DelayTime::create(0.15*i),CallFunc::create([=](){
                        
                        gum->setVisible(true);
                        
                    }),EaseSineIn::create(MoveTo::create(0.25, Vec2(-31.56,246))),CallFunc::create([=](){
                        
                        auto bowl_vanilla = Sprite::create("cocostudio/Make2/bowl_vanilla.png");
                        bowl_vanilla->setPosition(m_pBowl->getContentSize()/2);
                        m_pBowl->addChild(bowl_vanilla);
                        bowl_vanilla->setOpacity(0);
                        bowl_vanilla->setScale(0);
                        bowl_vanilla->runAction(Sequence::create(Spawn::create(FadeIn::create(0.4),ScaleTo::create(0.4, vanilla1[i]), NULL), NULL));
                        
                    }),FadeOut::create(0.2), NULL));
                    
                }
                 AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
                
            }),DelayTime::create(2.5),Spawn::create(RotateTo::create(1.0,0),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
////                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                 Director::getInstance()->getEventDispatcher()->setEnabled(true);
                m_iStepNum++;
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
            }), NULL));

        }
            break;
        case kstepOther1Egg1:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
//            m_pBowl->setLocalZOrder(100);
//            m_pBowl->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, m_pBowl->getParent()->getContentSize()/2),ScaleTo::create(0.5,1.5), NULL), NULL));
//
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0, Vec2(8,531)),ScaleTo::create(0.5,1.0), NULL),MoveTo::create(0.35, Vec2(68,479)),EaseElasticInOut::create(MoveBy::create(0.65, Vec2(-20,20)),1.0),DelayTime::create(0.2),MoveTo::create(0.5,Vec2(158,450)),CallFunc::create([=](){
                
                node->setVisible(false);
                auto egg = Sprite::create("cocostudio/Make2/bowl_egg0.png");
                m_pBowl->addChild(egg);
                egg->setPosition(m_pBowl->getContentSize()/2);
                egg->setPosition(Vec2(118,310));
                
                AudioHelp::getInstance()->playEffect("crack egg.mp3");
                
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg0.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg1.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg2.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg3.png");
                
                pOvenAnimation->setDelayPerUnit(0.4);
                pOvenAnimation->setLoops(1);
                
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                egg->runAction(pOvenAnimat);
            
            }),DelayTime::create(2.0),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
                m_iStepNum++;
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }

                
            }), NULL));

        }
            break;
        case kstepOther1Egg2:
        {
             Director::getInstance()->getEventDispatcher()->setEnabled(false);
//            m_pBowl->setLocalZOrder(100);
//            m_pBowl->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, m_pBowl->getParent()->getContentSize()/2),ScaleTo::create(0.5,1.5), NULL), NULL));
            
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(455,527)),ScaleTo::create(0.5,1.0), NULL),MoveTo::create(0.35,Vec2(373,474)),EaseElasticInOut::create(MoveBy::create(0.35, Vec2(20,20)),1.0),MoveTo::create(0.5,Vec2(287,446)),CallFunc::create([=](){
                
                node->setVisible(false);
                auto egg = Sprite::create("cocostudio/Make2/bowl_egg0.png");
                m_pBowl->addChild(egg);
                egg->setPosition(m_pBowl->getContentSize()/2);
                egg->setPosition(Vec2(270,309));
                
                AudioHelp::getInstance()->playEffect("crack egg.mp3");
                
                auto pOvenAnimation = Animation::create();
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg0.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg1.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg2.png");
                pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make2/bowl_egg3.png");
                
                pOvenAnimation->setDelayPerUnit(0.4);
                pOvenAnimation->setLoops(1);
                
                auto pOvenAnimat = Animate::create(pOvenAnimation);
                egg->runAction(pOvenAnimat);
                
            }),DelayTime::create(2.0),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
////                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                m_iStepNum++;
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
            }), NULL));

        }
            break;
        case kstepOther1Chocolate:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            PointArray *array = PointArray::create(3);
            array->addControlPoint(Vec2(226,329));
            array->addControlPoint(Vec2(228,363));
            array->addControlPoint(Vec2(278,308));
            array->addControlPoint(Vec2(242,395));
            
            array->addControlPoint(Vec2(299,337));
            array->addControlPoint(Vec2(273,405));
            array->addControlPoint(Vec2(314,366));
            array->addControlPoint(Vec2(327,397));
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5,Vec2(226,329)), RotateTo::create(0.5,-134.5),NULL),CallFunc::create([=](){
            
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                }
            
                auto bowl = Sprite::create(__String::createWithFormat("cocostudio/MakeOther/bowl_%s.png",GameDataInstance->getFlavor().c_str())->getCString());
                bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(bowl);
                bowl->setOpacity(0);
                bowl->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(1.5),NULL));
            
            }),CatmullRomTo::create(2.0,array),Spawn::create(RotateTo::create(1.0,0),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
////                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
            
                m_iStepNum++;
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                
            }),NULL));
        }
            break;
        case kstepOther1Mint:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            PointArray *array = PointArray::create(3);
            array->addControlPoint(Vec2(226,329));
            array->addControlPoint(Vec2(228,363));
            array->addControlPoint(Vec2(278,308));
            array->addControlPoint(Vec2(242,395));
            
            array->addControlPoint(Vec2(299,337));
            array->addControlPoint(Vec2(273,405));
            array->addControlPoint(Vec2(314,366));
            array->addControlPoint(Vec2(327,397));
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5,Vec2(226,329)), RotateTo::create(0.5,-134.5),NULL),CallFunc::create([=](){
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                }
                

                auto bowl = Sprite::create(__String::createWithFormat("cocostudio/MakeOther/bowl_%s.png",GameDataInstance->getFlavor().c_str())->getCString());
                bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(bowl);
                bowl->setOpacity(0);
                bowl->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(1.5),NULL));
                
            }),CatmullRomTo::create(2.0,array),Spawn::create(RotateTo::create(1.0,0),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
////                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                m_iStepNum++;
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                
            }),NULL));
            
        }
            break;
        case kstepOther1Strawberry:
        {
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            PointArray *array = PointArray::create(3);
            array->addControlPoint(Vec2(226,329));
            array->addControlPoint(Vec2(228,363));
            array->addControlPoint(Vec2(278,308));
            array->addControlPoint(Vec2(242,395));
            
            array->addControlPoint(Vec2(299,337));
            array->addControlPoint(Vec2(273,405));
            array->addControlPoint(Vec2(314,366));
            array->addControlPoint(Vec2(327,397));
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5,Vec2(226,329)), RotateTo::create(0.5,-134.5),NULL),CallFunc::create([=](){
                
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                }

               auto bowl = Sprite::create(__String::createWithFormat("cocostudio/MakeOther/bowl_%s.png",GameDataInstance->getFlavor().c_str())->getCString());
                bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(bowl);
                bowl->setOpacity(0);
                bowl->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(1.5),NULL));
                
            }),CatmullRomTo::create(2.0,array),Spawn::create(RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),nullptr),CallFunc::create([=](){
                
//                layer->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5,1.0),MoveTo::create(0.25,_LayerPos),nullptr),CallFunc::create([=](){
//                    
////                    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////                    layer->setPosition(_LayerPos);
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                m_iStepNum++;
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                if (m_iStepNum == 8) {
                    this->scheduleOnce(schedule_selector(MakeOtherStep1::goToNextStep), 0.5);
                }
                
            }),NULL));
        }
            break;
        default:
            break;
    }
}
void MakeOtherStep1::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    if(node->getTag()==kstepOther1Vanilla){
        node->setCascadeOpacityEnabled(false);
        node->runAction(FadeIn::create(0.0));
        
        auto pour = node->getChildByName("pour");
        if (pour) {
            pour->setVisible(false);
        }
    }
}
void MakeOtherStep1::touchEnd(ui::Widget* widget)
{
    
    
}
void MakeOtherStep1::showNextTool(float dt)
{
    auto tool = _studioLayer->getNodeByName<Sprite*>(GameDataInstance->getFlavor().c_str());
    if (tool)
        tool->setVisible(true);
    
    for(auto sprite:m_vTool){
    
        int tag = sprite->getTag();
        
        
        
        
        if(tag > 3){
        
            ActionUtils::delayTimeAction(this,0.2*tag,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
                
            }));
            
         ActionHelper::showBezier(sprite, toolPos[tag], ActionHelper::ShowDirection::show_from_right,0.2*tag);
        }
        
    }
}
void MakeOtherStep1::goToNextStep(float dt)
{
    
    GameDataInstance->setStepOtherPath("otherstep1.png");
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getStepOtherPath();
    Director::getInstance()->getTextureCache()->removeTextureForKey(filePath.c_str());

    
    RenderTexture* texture = RenderTexture::create(m_pBowl->getContentSize().width, m_pBowl->getContentSize().height);
    texture->retain();
    texture->begin();
    
    m_pBowl->setCascadeOpacityEnabled(false);
    m_pBowl->setOpacity(0);
    m_pBowl->setPosition(Vec2::ZERO);
    m_pBowl->setAnchorPoint(Vec2::ZERO);
    m_pBowl->visit();
    
    texture->end();
    Director::getInstance()->getRenderer()->render();
    texture->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    Image* image1 = texture->newImage();
    image1->saveToFile(filePath.c_str(),false);
//    Texture2D* spriteTexture2D = texture->getSprite()->getTexture();
//    
//    texture->saveToFile(GameDataInstance->getStepOtherPath());
    Director::getInstance()->getRenderer()->render();

    
    if(nextStep){
        
        nextStep();
        
    }
}




