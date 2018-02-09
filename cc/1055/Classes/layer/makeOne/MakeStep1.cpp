//
//  MakeStep1.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/29.
//
//

#include "MakeStep1.h"
#include "MoveLisenter.h"
#include "CircleBy.h"
#include "DrawLine.h"
#include "BezierCreator.h"
//REGIST_SECENE(MakeStep1)

string makeStep1ToolPath[5]={"water","butter","salt","sugar","flour"};

enum step1ToolType{

    kStep1Water = 0,
    kStep1Butter,
    kStep1Salt,
    kStep1Sugar,
    kStep1Flour,

};

MakeStep1::MakeStep1()
{
    m_iStepNum = 0;
    _bowlPos = Vec2::ZERO;
    m_bIsShowHand = true;
}
MakeStep1::~MakeStep1()
{

    

}
bool MakeStep1::init()
{
    if(!BaseLayer::init())
        return false;
    
//    RewardLayer* layer = RewardLayer::create(RewardType::typeMix);
//    this->addChild(layer,100);
//    
//    
    _studioLayer = StudioLayer::create("MakeStep1.csb");
    addChild(_studioLayer);
    
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    _bowlPos = m_pBowl->getPosition();
    _LayerPos = _studioLayer->getNodeByName<Layout*>("layer")->getPosition();
    for (int i = 0 ;i<5;i++) {
        
        auto sprite = _studioLayer->getNodeByName<Sprite*>(makeStep1ToolPath[i].c_str());
        sprite->setTag(i);
        auto lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, sprite);
        lisenter->setOrginOffset();
        lisenter->addRectByNode(m_pBowl);
        
        lisenter->onTouchDown = CC_CALLBACK_2(MakeStep1::onTouchDown, this);
        lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep1::onTouchMoveInRect, this);
        lisenter->onTouchUp = CC_CALLBACK_2(MakeStep1::onTouchUp, this);
        
       // ActionHelper::showBezier(sprite, sprite->getPosition(), ActionHelper::ShowDirection::show_from_right,2.0 + 0.3*i);
        sprite->setVisible(false);
    }
    m_pBowl->setPosition(m_pBowl->getPosition()+Vec2(-1000,0));
    
    auto title = Sprite::create("cocostudio/UI/tips/Addingredients.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    return true;
}
void MakeStep1::onEnter()
{
    BaseLayer::onEnter();
    m_pBowl->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("banner drop.mp3");
    
    }),Spawn::create(MoveBy::create(1.0,Vec2(1000,0)),EaseElasticOut::create(RotateBy::create(1.5,-360*10),1.0), NULL), CallFunc::create([=](){
        for (int i = 0 ;i<5;i++) {
            
            auto sprite = _studioLayer->getNodeByName<Sprite*>(makeStep1ToolPath[i].c_str());
           
            ActionUtils::delayTimeAction(this,0.3*i,CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
            }));
            
            
            
            
            ActionHelper::showBezier(sprite, sprite->getPosition(), ActionHelper::ShowDirection::show_from_right,0.3*i,[=](){
            
                if(i ==4 ){
                
                    if (this->getChildByTag(1001)) {
                        
                        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
                        
                    }
                    
                    showHand();
                    
                    
                    
                    if (rand()%2 == 0) {
                        
                        AudioHelp::getInstance()->playEffect("VoiceOver/Add ingredients.mp3");
                        
                    }else{
                    
                        AudioHelp::getInstance()->playEffect("VoiceOver/Add ingredients.mp3");
                        
                    }
                    
                 
                }
            });
            sprite->setVisible(true);
        }
        
    }),NULL));
    
}
void MakeStep1::showHand()
{
    if (m_bIsShowHand) {
        
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setVisible(true);
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setLocalZOrder(50);
        auto hand = _studioLayer->getNodeByName<Sprite*>("hand");
        hand->setLocalZOrder(100);
        hand->setVisible(true);
//        ActionHelper::runBezierMoveHideAction(hand,Vec2(553,434),Vec2(328,303));
        
        vector<Vec2> points = {
            Vec2(642.730591,418.836975),
            Vec2(434.074219,468.754791),
            Vec2(434.074219,467.756439),
            Vec2(305.286255,318.002991)
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
void MakeStep1::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep1::onExit()
{
    BaseLayer::onExit();
}
void MakeStep1::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
    if(m_bIsShowHand){
        m_bIsShowHand=false;
        _studioLayer->getNodeByName<Sprite*>("jiantou")->setVisible(false);
        auto hand = _studioLayer->getNodeByName<Sprite*>("hand");
        hand->stopAllActions();
        hand->setVisible(false);
        
    }
}
void MakeStep1::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    lisenter->setEnabled(false);
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
 
//    auto layer = _studioLayer->getNodeByName<Layout*>("layer");
//    
////    layer->setAnchorPoint(Vec2(_bowlPos.x/layer->getContentSize().width,_bowlPos.y/layer->getContentSize().height));
//    
//    
////    layer->setPosition(layer->getPosition() + Vec2(-_bowlPos.x,layer->getContentSize().height*(_bowlPos.y/layer->getContentSize().height - 0.5)));
////    Vec2 layerPos = layer->getPosition();
//    layer->runAction(Spawn::create(ScaleTo::create(1.0,1.1),MoveTo::create(1.0,layer->getParent()->getContentSize()/2),nullptr));
    
    int tag = node->getTag();

//    node->retain();
//    node->removeFromParent();
//    layer->addChild(node);
    
    log("-----%d",m_iStepNum);
    switch (tag) {
        case kStep1Water:
        {
            
            node->runAction(Sequence::create(MoveTo::create(0.5,Vec2(412,528)),RotateTo::create(0.5,-90),CallFunc::create([=](){
            
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
               
                
            }),CircleBy::create(1.5,Vec2(-80,-20),60,360*1.5),MoveTo::create(0.5,Vec2(412,480)),CallFunc::create([=](){
            
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
                
            }),DelayTime::create(0.6),RemoveSelf::create(),CallFunc::create([=](){
            
                
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
                m_iStepNum++;
                
                if (m_iStepNum == 5) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep1::goToNextStep), 0.5);
                    
                }
            
            
            }), NULL));
        }
            
            break;
        case kStep1Butter:
        {
            auto butter = dynamic_cast<Sprite*>(node);
            auto butterBowl = Sprite::create("cocostudio/Make1/bowl_butter3.png");
            butterBowl->setPosition(m_pBowl->getContentSize()/2);
            m_pBowl->addChild(butterBowl);
            butterBowl->setOpacity(0);
            
            butter->runAction(Sequence::create(MoveTo::create(1.0,Vec2(303,418 + 100)),RotateTo::create(0.5,-33),CallFunc::create([=](){
                butter->setTexture("cocostudio/Make1/butter1.png");
                butterBowl->setVisible(true);
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                butterBowl->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(1.0),CallFunc::create([=](){}), NULL));
                
                AudioHelp::getInstance()->playEffect("add buter.mp3");
                
            }),DelayTime::create(1.0),CallFunc::create([=](){
            
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
//                particle->setVisible(true);
                particle->stopSystem();
            
                butter->setTexture("cocostudio/Make1/butter4.png");
                
            }),DelayTime::create(1.0),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
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
                
                if (m_iStepNum == 5) {
                    this->scheduleOnce(schedule_selector(MakeStep1::goToNextStep), 0.5);
                }
                
            }),NULL));
        }
            break;
        case kStep1Salt:
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
            
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(304 - 30,470 + 50)),RotateTo::create(1.0,-144.2),nullptr),CallFunc::create([=](){
            
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
                
            }),DelayTime::create(0.55),CallFunc::create([=](){
                
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
            }),DelayTime::create(0.55),CallFunc::create([=](){
            
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
                
                if (m_iStepNum == 5) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep1::goToNextStep), 0.5);
                    
                }
                
                
            }), NULL));
        }
            
            break;
        case kStep1Sugar:
        {
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(293,350)),RotateTo::create(1.0,-110),nullptr),CallFunc::create([=](){
                
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
                
            }),MoveBy::create(2.0,Vec2(0,200)),CallFunc::create([=](){
                
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
                
                if (m_iStepNum == 5) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep1::goToNextStep), 0.5);
                    
                }
            }), NULL));
        }
            
            break;
        case kStep1Flour:
        {
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(354,388)),RotateTo::create(1.0,-90),nullptr),CallFunc::create([=](){
                
                
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
                
            }),MoveBy::create(2.0,Vec2(0,200)),CallFunc::create([=](){
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
                particle->stopSystem();
                
            }),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
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
                
                if (m_iStepNum == 5) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep1::goToNextStep), 0.5);
                    
                }
            }), NULL));
        }
            
            break;
        default:
            break;
    }
    
}
void MakeStep1::goToNextStep(float dt)
{
//    func_bind(nextStep);
    
    GameDataInstance->setstepOnePath("step1.png");
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getstepOnePath();
    Director::getInstance()->getTextureCache()->removeTextureForKey(filePath.c_str());
    //安卓
//    auto filePath = FileUtility::getStoragePath() + "/" + GameDataInstance->getstepOnePath();
//    Director::getInstance()->getTextureCache()->removeTextureForKey(filePath.c_str());
    
    
    log("%s",filePath.c_str());
    
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
    
//    Texture2D* spriteTexture2D = texture->getSprite()->getTexture();
    Image* image1 = texture->newImage();
    image1->saveToFile(filePath.c_str(),false);
    
//    texture->saveToFile(GameDataInstance->getstepOnePath());
    Director::getInstance()->getRenderer()->render();
    
    
    if(nextStep){
        nextStep();
    }
}
void MakeStep1::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    
}
void MakeStep1::touchEnd(ui::Widget* widget)
{
    

}