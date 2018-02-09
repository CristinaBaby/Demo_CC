//
//  MakeOtherStep22.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#include "MakeOtherStep2.h"

const int MakeOtherStep2::shakeAction = 101010101;

#define TagSpoon_1  1000
#define TagSpoon_2  2000

MakeOtherStep2::MakeOtherStep2()
{
    _inertia = nullptr;
    m_bIsChooseSpoon  = false;
    m_pSpoon = nullptr;
    m_bIsSound = false;
    m_bIsMix = false;
    m_bIsVoice = true;
    m_bIsFirst = false;
}
MakeOtherStep2::~MakeOtherStep2()
{
    
}
MakeOtherStep2*  MakeOtherStep2::create()
{
    MakeOtherStep2* step = new MakeOtherStep2();
    if (step && step->init()) {
        
        step->autorelease();
        return step;
        
    }
    CC_SAFE_DELETE(step);
    return nullptr;
}
bool MakeOtherStep2::init()
{
    
    if(!BaseLayer::init())
        return false;
    
    isShake = false;
    isFastShake = false;
    isFinish = false;
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeOtherCsb/MakeOtherStep2.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    auto spoon= _studioLayer->getNodeByName<Sprite*>("spoon");
    auto spoon1= _studioLayer->getNodeByName<Sprite*>("spoon1");
    
//    auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
//    loadingBar->setPercent(0);
    
    m_vSpoon.pushBack(spoon);
    m_vSpoon.pushBack(spoon1);
    
    spoon ->setTag(TagSpoon_1);
    spoon1->setTag(TagSpoon_2);
    
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getStepOtherPath();
    m_pBowlCover = Sprite::create(filePath.c_str());
//    m_pBowlCover = Sprite::create(FileUtils::getInstance()->getWritablePath() + GameDataInstance->getStepOtherPath());
    m_pBowlCover->setFlippedY(true);
    m_pBowlCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pBowlCover->setPosition(_studioLayer->getNodeByName("mixLayer")->getContentSize()/2);
    _studioLayer->getNodeByName("mixLayer")->addChild(m_pBowlCover);
    
    spoon->setPosition(spoon->getPosition()+Vec2(1000,0));
    spoon1->setPosition(spoon1->getPosition()+Vec2(1000,0));
    
    auto loadingBar   = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
    auto loadingBarBg = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar_bg");
    loadingBar  ->setPositionY(loadingBar->getPositionY()+500);
    loadingBarBg->setPositionY(loadingBarBg->getPositionY()+500);
    
    initData();
    
    auto title = Sprite::create("cocostudio/UI/tips/Mixingredients.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    return true;
}
void MakeOtherStep2::initData(){
    
    auto mixLayer = _studioLayer->getNodeByName("mixLayer");
    
    auto mixShadow = Sprite::create("cocostudio/Make1/mix_shadow.png");
    mixShadow->setPosition(m_pBowl->getContentSize()/2);
    m_pBowl->addChild(mixShadow,100);
    
    _change = PercentChangeComponent::create();
    std::vector<string> paths;
//    =
//    {
//        "Make1/bowl_mix0.png",
//        "Make1/bowl_mix1.png",
//        "Make1/bowl_mix2.png",
//    };
    
    for(int i = 0;i<3;i++){

        string path = __String::createWithFormat("MakeOther/bowl_%s_mix%d.png",GameDataInstance->getFlavor().c_str(),i)->getCString();
        paths.push_back(path);

    }

    _change->setFrameFile(paths);
    _change->addListners(FrameChangeeEnd, CC_CALLBACK_2(MakeOtherStep2::finish,this));
    mixLayer->addComponent(_change);
    
    _inertia = InertiaComponent::create();
    mixLayer->addComponent(_inertia);
    _inertia->setInvokeAction(RotateBy::create(0, 60));
    _inertia->addListners(InertiaInvoking, CC_CALLBACK_2(MakeOtherStep2::inertiaInvoking, this));
    _inertia->addListners(Inertiaing, CC_CALLBACK_2(MakeOtherStep2::inertiaing, this));
    _inertia->addListners(InertiaStop, [this](LQComponent*,OperateListner* lis){
        _change->stop();
    });
}

void MakeOtherStep2::onEnter()
{
    BaseLayer::onEnter();
//    showBowl();
    showSpoon();
}
void MakeOtherStep2::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherStep2::onExit()
{
    BaseLayer::onExit();
    
}
void MakeOtherStep2::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if(!m_bIsFirst){
    
        m_bIsFirst = true;
        
        auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
        auto loadingBarBg = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar_bg");
        
        ActionUtils::moveByElasticOut(loadingBar, Vec2(0,-500));
        ActionUtils::moveByElasticOut(loadingBarBg, Vec2(0,-500));
    
    }
   
    if(m_pBowl->getChildByTag(101)){
        
        m_pBowl->getChildByTag(101)->removeFromParent();
        
    }
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
    
    if (!m_bIsChooseSpoon) {
        m_bIsChooseSpoon = true;
        
        m_pSpoon = dynamic_cast<Sprite*>(node);
        m_pSpoon->setUserData((void*)100);
        for (auto spoon:m_vSpoon) {
            
            size_t data = (size_t)spoon->getUserData();
            if (data!=100) {
                spoon->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0),CallFunc::create([=](){
                
                    showBowl();
                
                }),DelayTime::create(1.0),CallFunc::create([=](){
                
                    m_bIsMix = true;
                
                    _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
//
//                    Rect rect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
//                    
//                    Rect _rect = Rect(rect.getMinX()  ,rect.getMinY(),190,210);
//                    _lisenter->m_vMoveRect = _rect;
                }), NULL));
            }
           
            
        }
//        _lisenter = MoveLisenter::create();
//        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lisenter, m_pSpoon);
//        _lisenter->setOrginOffset();
//        _lisenter->addRectByNode(m_pBowl);
//        
//        _lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep2::onTouchDown, this);
//        _lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep2::onTouchMoveInRect, this);
//        _lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep2::onTouchUp, this);
//        _lisenter->onTouchMix = CC_CALLBACK_3(MakeOtherStep2::onTouchMix, this);
//        _lisenter->m_bIsMoveBack = false;
//        
//        _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());

        
    }
    
}
void MakeOtherStep2::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    
    if(m_pBowl->getChildByTag(101)){
        
        m_pBowl->getChildByTag(101)->removeFromParent();
        
    }
    
    //
//    m_pSpoon->setTexture("cocostudio/Make3/spoon_mix.png");
    if(node->getTag() == TagSpoon_1){
        
        m_pSpoon->setTexture("cocostudio/Make3/spoon_mix.png");
        
    }else if(node->getTag() == TagSpoon_2 && !m_bIsAction){
        
        m_bIsAction = true;
        //动画
        auto pOvenAnimation = Animation::create();
        pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make3/mix1.png");
        pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make3/mixer0.png");
        
        pOvenAnimation->setDelayPerUnit(0.05);
        pOvenAnimation->setLoops(-1);
        
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        pOvenAnimat->setTag(100);
        m_pSpoon->runAction(pOvenAnimat);
        
    }
    
}
void MakeOtherStep2::onTouchMix(Node* node,MoveLisenter* lisenter,Touch* touch)
{
    if (!m_bIsMix) {
        return;
    }
    if(!m_bIsSound){
        
        m_bIsSound = true;
        
        AudioHelp::getInstance()->playLoofEffect("stir the batter.mp3");
    }
    
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
void MakeOtherStep2::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    _inertia->stopInvoke();
    unschedule(schedule_selector(MakeOtherStep2::caculateShakeBad));
    _change->stop();
    m_pSpoon->stopActionByTag(100);
    m_bIsAction = false;
    
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
void MakeOtherStep2::showBowl()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    m_pBowl->runAction(Sequence::create(EaseElasticInOut::create(MoveTo::create(1.0,m_pBowl->getParent()->getContentSize()/2),1.0),DelayTime::create(0.0),CallFunc::create([=](){
        
//        showSpoon();
        
        startPos = m_pBowl->getPosition();
        
        auto rota = Sprite::create("cocostudio/UI/tips/xuanzhuan.png");
        rota->setPosition(m_pBowl->getContentSize()/2);
        m_pBowl->addChild(rota);
        rota->setTag(101);
        rota->runAction(RepeatForever::create(RotateBy::create(1.0,-60)));
        
    }),NULL));
}
void MakeOtherStep2::showSpoon()
{
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
        
         AudioHelp::getInstance()->playEffect("VoiceOver/mix the ingredients,be careful not to spill.mp3");
    }
    for (auto spoon:m_vSpoon) {
        AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
        spoon->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(-1000,0)),1.0),CallFunc::create([=](){
            _lisenter = MoveLisenter::create();
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lisenter, spoon);
            _lisenter->setOrginOffset();
//            _lisenter->addRectByNode(m_pBowl);
            
            auto bowl = Sprite::createWithTexture(m_pBowl->getTexture());
            bowl->setPosition(m_pBowl->getParent()->getContentSize()/2);
            m_pBowl->getParent()->addChild(bowl);
            _lisenter->addRectByNode(bowl);
            bowl->setVisible(false);
            
//
            
            _lisenter->onTouchDown = CC_CALLBACK_2(MakeOtherStep2::onTouchDown, this);
            _lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep2::onTouchMoveInRect, this);
            _lisenter->onTouchUp = CC_CALLBACK_2(MakeOtherStep2::onTouchUp, this);
            _lisenter->onTouchMix = CC_CALLBACK_3(MakeOtherStep2::onTouchMix, this);
            _lisenter->m_bIsMoveBack = false;
            
//            _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
            
        }), NULL));
    }
//    AudioHelp::getInstance()->playEffect("VoiceOver/stir ingredients.mp3");
    
}
void MakeOtherStep2::touchEnd(ui::Widget* widget)
{
    
    
}
void MakeOtherStep2::inertiaInvoking(LQComponent* lqc,OperateListner* lis){
    
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
//        AudioHelp::getInstance()->playEffect("VoiceOver/watch out.mp3");
    }
    CCLOG("data====%f",data);
    auto loadingBar = _studioLayer->getNodeByName<ui::LoadingBar*>("LoadingBar");
    auto loadingPercent = loadingBar->getPercent()+ data*0.3;
    loadingBar->setPercent(loadingPercent);
    dynamic_cast<PercentChangeComponent*>(lqc->getOwner()->getComponent("PercentChangeComponent"))->invoking(loadingPercent*0.01);
    
//    if(data<0.4)
//        unschedule(schedule_selector(MakeOtherStep2::caculateShakeBad));
//    if(data<0.3){
//        isShake = false;
//        isFastShake = false;
//        unschedule(schedule_selector(MakeOtherStep2::caculateShake));
//        unschedule(schedule_selector(MakeOtherStep2::caculateFastShake));
//        
//    }else if (isFastShake) {
//        shake(15,0.04);
//        if(data>0.75&& !isScheduled(schedule_selector(MakeOtherStep2::caculateShakeBad)))
//            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateShakeBad), 2);
//    } else if(isShake){
//        shake(11,0.07);
//        if(data>0.6&& !isFastShake&& !isScheduled(schedule_selector(MakeOtherStep2::caculateFastShake)))
//            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateFastShake), 1);
//    }
//    
//    if(data>0.6){
//        if(!isShake&& !isScheduled(schedule_selector(MakeOtherStep2::caculateShake)))
//            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateShake), 1);
//    }
    if(data<0.2){
        unschedule(schedule_selector(MakeOtherStep2::caculateShakeBad));
        isShake = false;
        isFastShake = false;
        unschedule(schedule_selector(MakeOtherStep2::caculateShake));
        unschedule(schedule_selector(MakeOtherStep2::caculateFastShake));
        
    }else if (isFastShake) {
        shake(15,0.04);
        if(data>0.4&& !isScheduled(schedule_selector(MakeOtherStep2::caculateShakeBad)))
            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateShakeBad), 2);
    } else if(isShake){
        shake(11,0.07);
        if(data>0.5&& !isFastShake&& !isScheduled(schedule_selector(MakeOtherStep2::caculateFastShake)))
            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateFastShake), 1);
    }
    
    if(data>0.6){
        if(!isShake&& !isScheduled(schedule_selector(MakeOtherStep2::caculateShake)))
            this->scheduleOnce(schedule_selector(MakeOtherStep2::caculateShake), 1);
    }
}

void MakeOtherStep2::inertiaing(LQComponent*,OperateListner* lis){
    auto data = dynAny_cast<float>(lis->getData());
    if(data >1.6)
        data = 1.6;
    auto spoon = _studioLayer->getNodeByName("spoon");
    auto spoonPos =  spoon->getParent()->convertToWorldSpace(spoon->getPosition());
    auto bowPos = _studioLayer->getNodeByName("bowl")->getParent()->convertToWorldSpace(startPos);
    
    spoonPos.rotate(bowPos, -data*0.1);
    spoon->setPosition(spoon->getParent()->convertToNodeSpace(spoonPos));
}

void MakeOtherStep2::shake(float distance ,float time){
    auto bowl = _studioLayer->getNodeByName("bowl");
    if(!bowl->getActionByTag(shakeAction)) {
        auto action = Repeat::create(ActionHelper::createShakeAction(distance,time),6);
        action->setTag(shakeAction);
        _studioLayer->getNodeByName("bowl")->setPosition(startPos);
        bowl->runAction(action);
    }
}

void MakeOtherStep2::caculateShake(float){
    isShake = true;
}
void MakeOtherStep2::caculateFastShake(float){
    isFastShake = true;
}
//搅拌失败
void MakeOtherStep2::caculateShakeBad(float){
    if(!isFinish) {
        _lisenter->setEnabled(false);
        //        move->setEnabled(false);
        //        ActionHelper::hide(move->getOwner(), ActionHelper::ShowDirection::show_from_right);
        _studioLayer->getNodeByName("mixLayer")->getComponent("InertiaComponent")->setEnabled(false);
        _studioLayer->getNodeByName("bowl")->runAction(EaseExponentialIn::create(MoveBy::create(0.5, Vec2(700,700))));
        
        auto mixPointName = "mixPointName";
        auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
        if(mixPoint) {
            mixPoint->removeFromParent();
        }
        AudioHelp::getInstance()->stopLoofEffect();
        ActionHelper::delayFunc(0.4, this, [this](){
            this->runAction(EaseBounceOut::create(ScaleTo::create(0.5, 1.3)));
            this->runAction(EaseBounceOut::create(MoveBy::create(0.5, Vec2(0, -90))));
            
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
            
            sp->runAction(Sequence::create(ScaleTo::create(0.25,1.1),ScaleTo::create(0.05,1.5),CallFunc::create([=](){
                
                sp1->runAction(Sequence::create(ScaleTo::create(0.25,1.1),ScaleTo::create(0.05,1.5),ScaleTo::create(0.25,1.0),DelayTime::create(1.0),FadeOut::create(0.5),CallFunc::create([=](){
                    
                    func_bind(resetLayer);
                    
                }),nullptr));
                
                
            }),ScaleTo::create(0.25,1.0),DelayTime::create(1.0),CallFunc::create([=](){
                
                
                
                
                
            }),FadeOut::create(0.5), CallFunc::create([=](){
                
                
                
            }),NULL));
        });
    }
}
//搅拌完成
void MakeOtherStep2::finish(LQComponent*,OperateListner* lis){
    isFinish = true;
    _lisenter->setEnabled(false);
    
    AudioHelp::getInstance()->stopLoofEffect();
    
    
    auto mixPointName = "mixPointName";
    auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
    if(mixPoint) {
        mixPoint->removeFromParent();
    }
    
    m_pSpoon->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)), 1.0),CallFunc::create([=](){
        
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        if (gameState == GameStateTwo) {
            
            RewardLayer* layer = RewardLayer::create(RewardType::typeRolling);
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
