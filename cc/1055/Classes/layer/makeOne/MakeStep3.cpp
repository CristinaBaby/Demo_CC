//
//  MakeStep33.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#include "MakeStep3.h"

REGIST_SECENE(MakeStep3)

enum step3ToolType{
    
    kStep3Caramel = 0,
    kStep3Vanilla,
    kStep3Chocolatechips,
    kStep3Egg1,
    kStep3Egg2,
    
};

string MakeStep3ToolPath[5]={"peanutbutter","vanilla","chocolatechips","egg1","egg0"};
Vec2 toolPos[5]={Vec2(),Vec2(),Vec2(),Vec2(),Vec2()};
float vanilla[3]={0.3,0.6,1.0};
MakeStep3::MakeStep3()
{
    m_iStepNum = 0;
    _bowlPos = Vec2::ZERO;
}
MakeStep3::~MakeStep3()
{
}
bool MakeStep3::init()
{
    
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStep3.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    
    _bowlPos = Vec2(231.78,344.70);
    _LayerPos = _studioLayer->getNodeByName<Layout*>("layer")->getPosition();
    
    auto bowlMix = Sprite::create("cocostudio/Make1/bowl_mix2.png");
    bowlMix->setPosition(m_pBowl->getContentSize()/2);
    m_pBowl->addChild(bowlMix);
    
    for (int i = 0 ;i<5;i++) {
        auto sprite = _studioLayer->getNodeByName<Sprite*>(MakeStep3ToolPath[i].c_str());
        m_vTool.pushBack(sprite);
        sprite->setTag(i);
        sprite->setLocalZOrder(i+10);
        toolPos[i] = sprite->getPosition();
        sprite->setPosition(sprite->getParent()->getContentSize() + Size(200,0));
        
        if (i == 0 && GameDataInstance->getFlavor() == "peanutbutter") {
            
            sprite->setVisible(true);
        
        }
        
        if (i == 2 && GameDataInstance->getFlavor() == "chocolatechips") {
            
            sprite->setVisible(true);
            
        }
        
    }
    auto title = Sprite::create("cocostudio/UI/tips/Addingredients.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    
    //显示工具
    return true;
}
void MakeStep3::onEnter()
{
    BaseLayer::onEnter();
    showBowl();
}
void MakeStep3::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep3::onExit()
{
    BaseLayer::onExit();
    
}
void MakeStep3::showBowl()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    m_pBowl->runAction(Sequence::create(EaseElasticInOut::create(MoveTo::create(0.5,Vec2(266.78,346.70)),1.0),CallFunc::create([=](){
    
        showShowTool();
        
    }), NULL));
}
void MakeStep3::showShowTool()
{
    
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
        AudioHelp::getInstance()->playEffect("VoiceOver/Add ingredients.mp3");
    }
    
    
    for (auto tool:m_vTool) {
        
        
        ActionUtils::delayTimeAction(this,tool->getTag()*0.3,CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
            
        }));
        
        ActionHelper::showBezier(tool, toolPos[tool->getTag()],ActionHelper::ShowDirection::show_from_right,tool->getTag()*0.3,[=](){
        
            auto lisenter = MoveLisenter::create();
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, tool);
            lisenter->setOrginOffset();
            lisenter->addRectByNode(m_pBowl);
            
            lisenter->onTouchDown = CC_CALLBACK_2(MakeStep3::onTouchDown, this);
            lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep3::onTouchMoveInRect, this);
            lisenter->onTouchUp = CC_CALLBACK_2(MakeStep3::onTouchUp, this);
        });
    }

}
void MakeStep3::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
    if(node->getTag()==kStep3Vanilla){
    
        node->setCascadeOpacityEnabled(false);
        node->runAction(FadeOut::create(0.2));
        
        auto pour = node->getChildByName("pour");
        if (pour) {
            pour->setVisible(true);
            //                    pour->runAction(FadeIn::create(0.2));
        }
    
    }
}
void MakeStep3::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
    lisenter->setEnabled(false);
    //所有点击都停止
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
//    node->setVisible(false);
    
//    m_iStepNum++;
    
//    auto layer = _studioLayer->getNodeByName<Layout*>("layer");
//    
////    layer->setAnchorPoint(Vec2(_bowlPos.x/layer->getContentSize().width,_bowlPos.y/layer->getContentSize().height));
////    layer->setPosition(layer->getPosition() + Vec2(-_bowlPos.x,layer->getContentSize().height*(_bowlPos.y/layer->getContentSize().height - 0.5)));
////    Vec2 layerPos = layer->getPosition();
//    layer->runAction(Spawn::create(ScaleTo::create(1.0,1.1),MoveTo::create(1.0,layer->getParent()->getContentSize()/2),nullptr));
//
//    node->retain();
//    node->removeFromParent();
//    layer->addChild(node);
    
    

    switch (node->getTag()) {
        case kStep3Caramel:
        {
            node->runAction(Sequence::create(MoveTo::create(0.5,Vec2(346 + 55,512)),RotateTo::create(0.5,-116),CallFunc::create([=](){
            
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
            
                auto bowl = Sprite::create("cocostudio/Make2/bowl_peanutbutter.png");
                bowl->setPosition(m_pBowl->getContentSize()/2 + Size(0,-100));
                m_pBowl->addChild(bowl);
                bowl->setOpacity(0);
                bowl->setScale(0);
                bowl->runAction(Sequence::create(DelayTime::create(2.0),Spawn::create(FadeIn::create(1.5),ScaleTo::create(1.5, 1.0), NULL), NULL));
  
            }),DelayTime::create(0.5),CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("add buter.mp3");

                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
//                particle->setVisible(false);
                particle->stopSystem();
            
            }),DelayTime::create(2.5),CallFunc::create([=](){
                
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
//                particle->stopSystem();
                
                
            }),Spawn::create(RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
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
            }),DelayTime::create(0.6),RemoveSelf::create(),CallFunc::create([=](){
                
                m_iStepNum++;
                log("m_iStepNum-----%d",m_iStepNum);
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep3::goToNextStep), 0.5);
                    
                }
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
            }), NULL));
        }
            break;
        case kStep3Chocolatechips:
        {
            node->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,Vec2(292,299 + 200)),RotateTo::create(1.0,-102),nullptr),CallFunc::create([=](){

                //粒子效果
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(true);
                particle->resetSystem();
                
                auto bowl = Sprite::create("cocostudio/Make2/bowl_chocolatechips.png");
                bowl->setPosition(m_pBowl->getContentSize()/2);
                m_pBowl->addChild(bowl);
                bowl->setOpacity(0);
                bowl->setScale(0);
                bowl->runAction(Sequence::create(DelayTime::create(0.5),Spawn::create(FadeIn::create(1.5),ScaleTo::create(1.5, 1.0), NULL), NULL));
                
                AudioHelp::getInstance()->playEffect("add_powder.mp3");

                
            }),DelayTime::create(1.0),CallFunc::create([=](){
                
                //完成
                auto particle = dynamic_cast<ParticleSystem*>(node->getChildByName("particle"));
                particle->setVisible(false);
                
                node->setCascadeOpacityEnabled(false);
                node->runAction(FadeOut::create(0.2));
                
                auto pour = node->getChildByName("pour");
                if (pour) {
                    pour->setVisible(true);
                }
                
            }),Spawn::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
                
            }),nullptr),RotateTo::create(1.0,0),nullptr),CallFunc::create([=](){
                
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
                log("m_iStepNum-----%d",m_iStepNum);
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep3::goToNextStep), 0.5);
                    
                }
                
                
            }), NULL));
        }
            break;
        case kStep3Vanilla:
        {
            
            node->runAction(Sequence::create(MoveTo::create(0.5,Vec2(324,513)),RotateTo::create(0.5,-112.43),CallFunc::create([=](){
                
                
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
                        
                    }),EaseSineIn::create(MoveTo::create(0.25, Vec2(-42.56,225.45))),CallFunc::create([=](){
                        
                        auto bowl_vanilla = Sprite::create("cocostudio/Make2/bowl_vanilla.png");
                        bowl_vanilla->setPosition(m_pBowl->getContentSize()/2);
                        m_pBowl->addChild(bowl_vanilla);
                        bowl_vanilla->setOpacity(0);
                        bowl_vanilla->setScale(0);
                        bowl_vanilla->runAction(Sequence::create(Spawn::create(FadeIn::create(0.4),ScaleTo::create(0.4, vanilla[i]), NULL), NULL));
                        
                    }),FadeOut::create(0.2), NULL));
                    
                }
                
                AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");

                
            }),DelayTime::create(1.0),Spawn::create(RotateTo::create(1.0,0),Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                
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
                
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
                m_iStepNum++;
                log("m_iStepNum-----%d",m_iStepNum);
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep3::goToNextStep), 0.5);
                    
                }
                
                
            }), NULL));
        }
            break;
        case kStep3Egg1:
        {

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
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
                
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                
                m_iStepNum++;
                log("m_iStepNum-----%d",m_iStepNum);
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep3::goToNextStep), 0.5);
                    
                }
                
                
            }), NULL));

        }
            break;
        case kStep3Egg2:
        {
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
//                    
//                    
//                }),nullptr));
                m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),ScaleTo::create(0.17,1.0),nullptr));
            }),DelayTime::create(0.6),CallFunc::create([=](){
            
                m_iStepNum++;
                log("m_iStepNum-----%d",m_iStepNum);
                if (m_iStepNum == 4) {
                    
                    this->scheduleOnce(schedule_selector(MakeStep3::goToNextStep), 0.5);
                    
                }
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
            }), NULL));
        }
            break;
            
        default:
            break;
    }
}
void MakeStep3::goToNextStep(float dt)
{

//    func_bind(nextStep);
    
    
//    RewardLayer::showRewardLayer(RewardType::typeMix,[=](){
//    
//
//    
//    });
    GameDataInstance->setstepTwoPath("step2.png");
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getstepTwoPath();
    Director::getInstance()->getTextureCache()->removeTextureForKey(filePath.c_str());
    
//    auto filePath = FileUtility::getStoragePath() + "/" + GameDataInstance->getstepTwoPath();
//    Director::getInstance()->getTextureCache()->removeTextureForKey(filePath.c_str());
    
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
        //texture->saveToFile(GameDataInstance->getstepTwoPath());
    
        Director::getInstance()->getRenderer()->render();
            if(nextStep)
                nextStep();
    
    
}
void MakeStep3::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    node->setCascadeOpacityEnabled(false);
    node->runAction(FadeIn::create(0.0));
    
    auto pour = node->getChildByName("pour");
    if (pour) {
        pour->setVisible(false);
    }
}
void MakeStep3::touchEnd(ui::Widget* widget)
{
    
    
}