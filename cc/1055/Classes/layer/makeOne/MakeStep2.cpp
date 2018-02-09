//
//  MakeStep22.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#include "MakeStep2.h"

const int MakeStep2::shakeAction = 101010101;

MakeStep2::MakeStep2()
{
    _inertia = nullptr;
    m_bIsSound = false;
    m_bIsVoice = true;
    m_bIsMix = false;
}
MakeStep2::~MakeStep2()
{
    
}
MakeStep2*  MakeStep2::create()
{
    MakeStep2* step = new MakeStep2();
    if (step && step->init()) {
        
        step->autorelease();
        return step;
        
    }
    CC_SAFE_DELETE(step);
    return nullptr;
}
bool MakeStep2::init()
{
    
    if(!BaseLayer::init())
        return false;
    
    isShake = false;
    isFastShake = false;
    isFinish = false;
    
    
    
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStep2.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    m_pSpoon= _studioLayer->getNodeByName<Sprite*>("spoon");

//    m_pBowlCover = Sprite::create(FileUtils::getInstance()->getWritablePath() + GameDataInstance->getstepOnePath());
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getstepOnePath();
    log("---%s",filePath.c_str());
    m_pBowlCover = Sprite::create(filePath.c_str());
    log("---%s",filePath.c_str());
//    m_pBowlCover->setFlippedY(true);
    m_pBowlCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pBowlCover->setPosition(_studioLayer->getNodeByName("mixLayer")->getContentSize()/2);
    _studioLayer->getNodeByName("mixLayer")->addChild(m_pBowlCover);
    
    m_pSpoon->setPosition(m_pSpoon->getPosition()+Vec2(1000,0));
    
    
    auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
    auto loadingBarBg = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar_bg");
    loadingBar->setPositionY(loadingBar->getPositionY()+500);
    loadingBarBg->setPositionY(loadingBarBg->getPositionY()+500);
    
    initData();
    
    auto title = Sprite::create("cocostudio/UI/tips/Mixingredients.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    return true;
}
void MakeStep2::initData(){

    auto mixLayer = _studioLayer->getNodeByName("mixLayer");

    
    auto mixShadow = Sprite::create("cocostudio/Make1/mix_shadow.png");
    mixShadow->setPosition(m_pBowl->getContentSize()/2);
    m_pBowl->addChild(mixShadow,100);
    
    _change = PercentChangeComponent::create();
    std::vector<string> paths =
     {
        "Make1/bowl_mix0.png",
        "Make1/bowl_mix1.png",
        "Make1/bowl_mix2.png",
    };

//    for(int i = 0;i<3;i++){
//        
//        string path = __String::createWithFormat("Make3/bowl_%s_mix%d.png",GameDataInstance->getFlavor().c_str(),i)->getCString();
//        paths.push_back(path);
//        
//    }
   
    _change->setFrameFile(paths);
    _change->addListners(FrameChangeeEnd, CC_CALLBACK_2(MakeStep2::finish,this));
    mixLayer->addComponent(_change);
    
    _inertia = InertiaComponent::create();
    mixLayer->addComponent(_inertia);
    _inertia->setInvokeAction(RotateBy::create(0, 60));
    _inertia->addListners(InertiaInvoking, CC_CALLBACK_2(MakeStep2::inertiaInvoking, this));
    _inertia->addListners(Inertiaing, CC_CALLBACK_2(MakeStep2::inertiaing, this));
    _inertia->addListners(InertiaStop, [this](LQComponent*,OperateListner* lis){
        _change->stop();
    });
}

void MakeStep2::onEnter()
{
    BaseLayer::onEnter();
    showBowl();
}
void MakeStep2::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep2::onExit()
{
    BaseLayer::onExit();
    
}
void MakeStep2::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if(m_pBowl->getChildByTag(101)){
    
        m_pBowl->getChildByTag(101)->removeFromParent();
        
        auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
        auto loadingBarBg = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar_bg");
        
        ActionUtils::moveByElasticOut(loadingBar, Vec2(0,-500));
        ActionUtils::moveByElasticOut(loadingBarBg, Vec2(0,-500),1.0,CallFunc::create([=](){
        
            m_bIsMix = true;
        
        }));
    }
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,500)));
        
    }
    
}
void MakeStep2::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
   m_pSpoon->setTexture("cocostudio/Make3/spoon_mix.png");
    
    if(!m_bIsSound){
    
        m_bIsSound = true;
    
        AudioHelp::getInstance()->playLoofEffect("stir the batter.mp3");
    }
    
    
}
void MakeStep2::onTouchMix(Node* node,MoveLisenter* lisenter,Touch* touch)
{
    if(!m_bIsMix)
        return;
    
    
    if(isFinish)
        return;
    
    auto distance = touch->getDelta();
    auto diagonal = DeviceInfo::getDiagonal();
    if(diagonal>7.9)
        diagonal = 7.9;
    else if(diagonal<5)
        diagonal = 5;
    _inertia->Invoke(distance.getLength()/80*diagonal/6);
    
    
    float percent = distance.getLength()/80*diagonal/6;
    auto mixPointName = "mixPointName";
    MixPoint* mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
    if(!mixPoint) {
        auto bowl = _studioLayer->getNodeByName("bowl");
        mixPoint = MixPoint::create();
        mixPoint->setName(mixPointName);
        mixPoint->setPointPath(__String::createWithFormat("cocostudio/UI/mix/%s_drop.png",GameDataInstance->getFlavor().c_str())->getCString());
        mixPoint->setPosition(bowl->getContentSize()*.5);
        bowl->addChild(mixPoint,10);
      
        
    }
    float time = 0.4/percent;
    if(time<0.7)
        time = 0.7;
    else if(time>1.4)
        time = 1.4;
    mixPoint->run(time);
    
}
void MakeStep2::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    _inertia->stopInvoke();
    unschedule(schedule_selector(MakeStep2::caculateShakeBad));
    _change->stop();
    
    if (m_bIsSound) {
        m_bIsSound = false;
        AudioHelp::getInstance()->stopLoofEffect();
    }
    
    auto mixPointName = "mixPointName";
    auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
    if(mixPoint) {
        mixPoint->stop();
    }

    
}
void MakeStep2::showBowl()
{
    
    
    
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");

    m_pBowl->runAction(Sequence::create(EaseElasticInOut::create(MoveTo::create(1.0,m_pBowl->getParent()->getContentSize()/2),0.5),DelayTime::create(0.0),CallFunc::create([=](){
    
        showSpoon();
        
        startPos = m_pBowl->getPosition();
        
        auto rota = Sprite::create("cocostudio/UI/tips/xuanzhuan.png");
        rota->setPosition(m_pBowl->getContentSize()/2);
        m_pBowl->addChild(rota);
        rota->setTag(101);
        rota->runAction(RepeatForever::create(RotateBy::create(1.0,-60)));
        
    }),NULL));
}
void MakeStep2::showSpoon()
{
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
        
        AudioHelp::getInstance()->playEffect("VoiceOver/stir ingredients.mp3");
        
    }
    
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
     m_pSpoon->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(-1000,0)),0.5),CallFunc::create([=](){
    
        _lisenter = MoveLisenter::create();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lisenter, m_pSpoon);
        _lisenter->setOrginOffset();
        _lisenter->addRectByNode(m_pBowl);
        
        _lisenter->onTouchDown = CC_CALLBACK_2(MakeStep2::onTouchDown, this);
        _lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep2::onTouchMoveInRect, this);
        _lisenter->onTouchUp = CC_CALLBACK_2(MakeStep2::onTouchUp, this);
        _lisenter->onTouchMix = CC_CALLBACK_3(MakeStep2::onTouchMix, this);
        _lisenter->m_bIsMoveBack = false;
        
        _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
         
//         Rect rect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
//         
//         Rect _rect = Rect(rect.getMinX() + 100,rect.getMinY() + 100,190,210);
//         _lisenter->m_vMoveRect = _rect;
//
//         AudioHelp::getInstance()->playEffect("VoiceOver/stir ingredients.mp3");

    }), NULL));
}
void MakeStep2::touchEnd(ui::Widget* widget)
{
    
    
}
void MakeStep2::inertiaInvoking(LQComponent* lqc,OperateListner* lis){
    
    if(isFinish)
        return;
    
    auto data = dynAny_cast<float>(lis->getData());
    if(data>1.7){
    
        data = 1.7;
        
        if (m_bIsVoice) {
            m_bIsVoice = false;
            AudioHelp::getInstance()->playEffect("VoiceOver/watch out.mp3");
            
            ActionUtils::delayTimeAction(this,2.0,CallFunc::create([=](){
            
                m_bIsVoice = true;
            
            }));
        }
    }

    CCLOG("data====%f",data);
    auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
    auto loadingPercent = loadingBar->getPercent()+ data*0.3;
    loadingBar->setPercent(loadingPercent);
    dynamic_cast<PercentChangeComponent*>(lqc->getOwner()->getComponent("PercentChangeComponent"))->invoking(loadingPercent*0.01);
    
//    if(data<0.4)
//        unschedule(schedule_selector(MakeStep2::caculateShakeBad));
//    if(data<0.3){
//        isShake = false;
//        isFastShake = false;
//        unschedule(schedule_selector(MakeStep2::caculateShake));
//        unschedule(schedule_selector(MakeStep2::caculateFastShake));
//        
//    }else if (isFastShake) {
//        shake(15,0.04);
//        if(data>0.75&& !isScheduled(schedule_selector(MakeStep2::caculateShakeBad)))
//            this->scheduleOnce(schedule_selector(MakeStep2::caculateShakeBad), 2);
//    } else if(isShake){
//        shake(11,0.07);
//        if(data>0.6&& !isFastShake&& !isScheduled(schedule_selector(MakeStep2::caculateFastShake)))
//            this->scheduleOnce(schedule_selector(MakeStep2::caculateFastShake), 1);
//    }
//    
//    if(data>0.6){
//        if(!isShake&& !isScheduled(schedule_selector(MakeStep2::caculateShake)))
//            this->scheduleOnce(schedule_selector(MakeStep2::caculateShake), 1);
//    }
    if(data<0.2){
        unschedule(schedule_selector(MakeStep2::caculateShakeBad));
        isShake = false;
        isFastShake = false;
        unschedule(schedule_selector(MakeStep2::caculateShake));
        unschedule(schedule_selector(MakeStep2::caculateFastShake));
        
    }else if (isFastShake) {
        shake(15,0.04);
        if(data>0.4&& !isScheduled(schedule_selector(MakeStep2::caculateShakeBad)))
            this->scheduleOnce(schedule_selector(MakeStep2::caculateShakeBad), 2);
    } else if(isShake){
        shake(11,0.07);
        if(data>0.5&& !isFastShake&& !isScheduled(schedule_selector(MakeStep2::caculateFastShake)))
            this->scheduleOnce(schedule_selector(MakeStep2::caculateFastShake), 1);
    }
    
    if(data>0.6){
        if(!isShake&& !isScheduled(schedule_selector(MakeStep2::caculateShake)))
            this->scheduleOnce(schedule_selector(MakeStep2::caculateShake), 1);
    }
}

void MakeStep2::inertiaing(LQComponent*,OperateListner* lis){
    auto data = dynAny_cast<float>(lis->getData());
    if(data >1.6)
        data = 1.6;
     auto spoon = _studioLayer->getNodeByName("spoon");
    auto spoonPos =  spoon->getParent()->convertToWorldSpace(spoon->getPosition());
    auto bowPos = _studioLayer->getNodeByName("bowl")->getParent()->convertToWorldSpace(startPos);
   
    spoonPos.rotate(bowPos, -data*0.1);
    spoon->setPosition(spoon->getParent()->convertToNodeSpace(spoonPos));
}

void MakeStep2::shake(float distance ,float time){
    auto bowl = _studioLayer->getNodeByName("bowl");
    if(!bowl->getActionByTag(shakeAction)) {
        auto action = Repeat::create(ActionHelper::createShakeAction(distance,time),6);
        action->setTag(shakeAction);
        _studioLayer->getNodeByName("bowl")->setPosition(startPos);
        bowl->runAction(action);
    }
}

void MakeStep2::caculateShake(float){
    isShake = true;
}
void MakeStep2::caculateFastShake(float){
    isFastShake = true;
}
//搅拌失败
void MakeStep2::caculateShakeBad(float){
    if(!isFinish) {
        _lisenter->setEnabled(false);
        auto mixPointName = "mixPointName";
        auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
        if(mixPoint) {
            mixPoint->removeFromParent();
        }
        
//        move->setEnabled(false);
//        ActionHelper::hide(move->getOwner(), ActionHelper::ShowDirection::show_from_right);
        _studioLayer->getNodeByName("mixLayer")->getComponent("InertiaComponent")->setEnabled(false);
        _studioLayer->getNodeByName("bowl")->runAction(EaseExponentialIn::create(MoveBy::create(0.5, Vec2(700,700))));
        ActionHelper::delayFunc(0.4, this, [this](){
            this->runAction(EaseBounceOut::create(ScaleTo::create(0.5, 1.3)));
            this->runAction(EaseBounceOut::create(MoveBy::create(0.5, Vec2(0, -90))));
            
            //DEBUG
//            func_bind(nextStep);
            
//            func_bind(resetLayer);
            
            AudioHelp::getInstance()->stopLoofEffect();
            
            ActionUtils::delayTimeAction(this,1.5,CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("VoiceOver/let's try again.mp3");
                
            }));
            
            //debug
            Size visible = Director::getInstance()->getVisibleSize();
            
            auto sp = Sprite::create(__String::createWithFormat("cocostudio/UI/mix/%s_splatter.png",GameDataInstance->getFlavor().c_str())->getCString());
            sp->setPosition(Vec2(visible.width/4,visible.height*3/4));
            this->addChild(sp);
            sp->setScale(0);
            
            auto sp1 = Sprite::create(__String::createWithFormat("cocostudio/UI/mix/%s_splatter.png",GameDataInstance->getFlavor().c_str())->getCString());
            sp1->setPosition(Vec2(visible.width*3/4,visible.height/4));
            this->addChild(sp1);
            sp1->setScale(0);
            
            
            AudioHelp::getInstance()->playEffect("splash.mp3");
            sp->runAction(Sequence::create(ScaleTo::create(0.25,1.1),ScaleTo::create(0.05,1.5),CallFunc::create([=](){
                
                AudioHelp::getInstance()->playEffect("splash.mp3");
                sp1->runAction(Sequence::create(ScaleTo::create(0.25,1.1),ScaleTo::create(0.05,1.5),ScaleTo::create(0.25,1.0),DelayTime::create(1.0),FadeOut::create(2.0),CallFunc::create([=](){
                    
                    func_bind(resetLayer);
                    
                }),nullptr));
            
            }),ScaleTo::create(0.25,1.0),DelayTime::create(1.0),CallFunc::create([=](){
            }),FadeOut::create(0.5), CallFunc::create([=](){
            }),NULL));
        });
    }
}
//搅拌完成
void MakeStep2::finish(LQComponent*,OperateListner* lis){
    isFinish = true;
    _lisenter->setEnabled(false);
    
    auto mixPointName = "mixPointName";
    auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
    if(mixPoint) {
        mixPoint->removeFromParent();
    }
    
    AudioHelp::getInstance()->stopLoofEffect();
   
    m_pSpoon->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)), 1.0),CallFunc::create([=](){
    
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        if (gameState == GameStateOne) {
        
            RewardLayer* layer = RewardLayer::create(RewardType::typeMix);
            this->addChild(layer,100);
            layer->btnOkClick=[=](){
                
                func_bind(nextStep);
                
            };
        }else{
        
            FlowerLayer::Loading(this,[=](){
                
                func_bind(nextStep);
                
            });
        }
        
    }), NULL));
//    move->setEnabled(false);
//    ActionHelper::hide(move->getOwner(), ActionHelper::ShowDirection::show_from_right);
}
