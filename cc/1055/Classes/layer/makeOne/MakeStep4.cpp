//
//  MakeStep42.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#include "MakeStep4.h"

const int MakeStep4::shakeAction = 101010101;

#define TagSpoon1  1000
#define TagSpoon2  2000
MakeStep4::MakeStep4()
{
    _inertia = nullptr;
    m_bIsChooseSpoon  = false;
    m_pSpoon = nullptr;
    m_bIsAction = false;
    m_bIsSound = false;
    m_bIsVoice = true;
    m_bIsMix = false;
    m_bIsFirst = false;
}
MakeStep4::~MakeStep4()
{
    
}
MakeStep4*  MakeStep4::create()
{
    MakeStep4* step = new MakeStep4();
    if (step && step->init()) {
        
        step->autorelease();
        return step;
        
    }
    CC_SAFE_DELETE(step);
    return nullptr;
}
bool MakeStep4::init()
{
    
    if(!BaseLayer::init())
        return false;
    
    isShake = false;
    isFastShake = false;
    isFinish = false;
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStep4.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
//    m_pSpoon= _studioLayer->getNodeByName<Sprite*>("mixer");
    
    auto spoon= _studioLayer->getNodeByName<Sprite*>("spoon");
    auto spoon1= _studioLayer->getNodeByName<Sprite*>("mixer");
    
    spoon ->setTag(TagSpoon1);
    spoon1->setTag(TagSpoon2);
    
    m_vSpoon.pushBack(spoon);
    m_vSpoon.pushBack(spoon1);
    
    auto filePath = FileUtils::getInstance()->getWritablePath() + GameDataInstance->getstepTwoPath();
    m_pBowlCover = Sprite::create(filePath.c_str());
//    m_pBowlCover = Sprite::create(FileUtility::getStoragePath() + "/" + GameDataInstance->getstepTwoPath());
    
    m_pBowlCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_pBowlCover->setPosition(_studioLayer->getNodeByName("mixLayer")->getContentSize()/2);
    _studioLayer->getNodeByName("mixLayer")->addChild(m_pBowlCover);
    
//    m_pSpoon->setPosition(m_pSpoon->getPosition()+Vec2(1000,0));
    spoon->setPosition(spoon->getPosition()+Vec2(1000,0));
    spoon1->setPosition(spoon1->getPosition()+Vec2(1000,0));
    
    
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
void MakeStep4::initData(){
    
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

        string path = __String::createWithFormat("Make3/bowl_%s_mix%d.png",GameDataInstance->getFlavor().c_str(),i)->getCString();
        paths.push_back(path);

    }
    
    _change->setFrameFile(paths);
    _change->addListners(FrameChangeeEnd, CC_CALLBACK_2(MakeStep4::finish,this));
    mixLayer->addComponent(_change);
    
    _inertia = InertiaComponent::create();
    mixLayer->addComponent(_inertia);
    _inertia->setInvokeAction(RotateBy::create(0, 60));
    _inertia->addListners(InertiaInvoking, CC_CALLBACK_2(MakeStep4::inertiaInvoking, this));
    _inertia->addListners(Inertiaing, CC_CALLBACK_2(MakeStep4::inertiaing, this));
    _inertia->addListners(InertiaStop, [this](LQComponent*,OperateListner* lis){
        _change->stop();
    });
}

void MakeStep4::onEnter()
{
    BaseLayer::onEnter();
//    showBowl();
    showSpoon();

}
void MakeStep4::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep4::onExit()
{
    BaseLayer::onExit();
    
}
void MakeStep4::onTouchDown(Node* node,MoveLisenter* lisenter)
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
        
        showBowl();
        
        m_pSpoon = dynamic_cast<Sprite*>(node);
        m_pSpoon->setUserData((void*)100);
        for (auto spoon:m_vSpoon) {
            
            size_t data = (size_t)spoon->getUserData();
            if (data!=100) {
                spoon->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)),1.0), CallFunc::create([=](){
                
                    m_bIsMix = true;
                
                }),NULL));
            }
        }
//        _lisenter = MoveLisenter::create();
//        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_lisenter, m_pSpoon);
//        _lisenter->setOrginOffset();
//        _lisenter->addRectByNode(m_pBowl);
//        
//        _lisenter->onTouchDown = CC_CALLBACK_2(MakeStep4::onTouchDown, this);
//        _lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep4::onTouchMoveInRect, this);
//        _lisenter->onTouchUp = CC_CALLBACK_2(MakeStep4::onTouchUp, this);
//        _lisenter->onTouchMix = CC_CALLBACK_3(MakeStep4::onTouchMix, this);
//        _lisenter->m_bIsMoveBack = false;
//        
//        _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
    }
}
void MakeStep4::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    if(m_pBowl->getChildByTag(101)){
        
        m_pBowl->getChildByTag(101)->removeFromParent();
        
    }
    
//    m_pSpoon->setTexture("cocostudio/Make3/spoon_mix.png");
    if(node->getTag() == TagSpoon1){
    
        m_pSpoon->setTexture("cocostudio/Make3/spoon_mix.png");
    
    }else if(node->getTag() == TagSpoon2 && !m_bIsAction){
    
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
    
    if(!m_bIsSound){
    
        m_bIsSound = true;
        
        AudioHelp::getInstance()->playLoofEffect("stir the batter.mp3");
    
    }
    
}
void MakeStep4::onTouchMix(Node* node,MoveLisenter* lisenter,Touch* touch)
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
void MakeStep4::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    _inertia->stopInvoke();
    unschedule(schedule_selector(MakeStep4::caculateShakeBad));
    _change->stop();
//    m_pSpoon->stopAllActions();
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
void MakeStep4::showBowl()
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    m_pBowl->runAction(Sequence::create(EaseElasticInOut::create(MoveTo::create(1.0,m_pBowl->getParent()->getContentSize()/2),0.56),DelayTime::create(0.5),CallFunc::create([=](){
        
//        _lisenter->addRectByNode(m_pBowl);
        
        startPos = m_pBowl->getPosition();
        
        auto rota = Sprite::create("cocostudio/UI/tips/xuanzhuan.png");
        rota->setPosition(m_pBowl->getContentSize()/2);
        m_pBowl->addChild(rota);
        rota->setTag(101);
        rota->runAction(RepeatForever::create(RotateBy::create(1.0,-60)));
        
    }),NULL));
}
void MakeStep4::showSpoon()
{
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
        
         AudioHelp::getInstance()->playEffect("VoiceOver/stir ingredients.mp3");
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
            
            _lisenter->m_vMoveRect =  RectApplyAffineTransform(Rect(0, 0, bowl->getContentSize().width, bowl->getContentSize().height),bowl->nodeToWorldTransform());
            
//            Rect rect =  RectApplyAffineTransform(Rect(0, 0, m_pBowl->getContentSize().width, m_pBowl->getContentSize().height),m_pBowl->nodeToWorldTransform());
//            
//            Rect _rect = Rect(rect.getMinX()  ,rect.getMinY(),190,210);
//            _lisenter->m_vMoveRect = _rect;
            
            _lisenter->onTouchDown = CC_CALLBACK_2(MakeStep4::onTouchDown, this);
            _lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep4::onTouchMoveInRect, this);
            _lisenter->onTouchUp = CC_CALLBACK_2(MakeStep4::onTouchUp, this);
            _lisenter->onTouchMix = CC_CALLBACK_3(MakeStep4::onTouchMix, this);
            _lisenter->m_bIsMoveBack = false;
            
            

            
        }), NULL));
    }
//     AudioHelp::getInstance()->playEffect("VoiceOver/stir ingredients.mp3");
//    m_pSpoon->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(-1000,0)),1.0),CallFunc::create([=](){
//        
//
//    }), NULL));
}
void MakeStep4::touchEnd(ui::Widget* widget)
{
    
    
}
void MakeStep4::inertiaInvoking(LQComponent* lqc,OperateListner* lis){
    
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
//        unschedule(schedule_selector(MakeStep4::caculateShakeBad));
//    if(data<0.3){
//        isShake = false;
//        isFastShake = false;
//        unschedule(schedule_selector(MakeStep4::caculateShake));
//        unschedule(schedule_selector(MakeStep4::caculateFastShake));
//        
//    }else if (isFastShake) {
//        shake(15,0.04);
//        if(data>0.75&& !isScheduled(schedule_selector(MakeStep4::caculateShakeBad)))
//            this->scheduleOnce(schedule_selector(MakeStep4::caculateShakeBad), 2);
//    } else if(isShake){
//        shake(11,0.07);
//        if(data>0.6&& !isFastShake&& !isScheduled(schedule_selector(MakeStep4::caculateFastShake)))
//            this->scheduleOnce(schedule_selector(MakeStep4::caculateFastShake), 1);
//    }
//    
//    if(data>0.6){
//        if(!isShake&& !isScheduled(schedule_selector(MakeStep4::caculateShake)))
//            this->scheduleOnce(schedule_selector(MakeStep4::caculateShake), 1);
//    }
    if(data<0.2){
        unschedule(schedule_selector(MakeStep4::caculateShakeBad));
        isShake = false;
        isFastShake = false;
        unschedule(schedule_selector(MakeStep4::caculateShake));
        unschedule(schedule_selector(MakeStep4::caculateFastShake));
        
    }else if (isFastShake) {
        shake(15,0.04);
        if(data>0.4&& !isScheduled(schedule_selector(MakeStep4::caculateShakeBad)))
            this->scheduleOnce(schedule_selector(MakeStep4::caculateShakeBad), 2);
    } else if(isShake){
        shake(11,0.07);
        if(data>0.5&& !isFastShake&& !isScheduled(schedule_selector(MakeStep4::caculateFastShake)))
            this->scheduleOnce(schedule_selector(MakeStep4::caculateFastShake), 1);
    }
    
    if(data>0.6){
        if(!isShake&& !isScheduled(schedule_selector(MakeStep4::caculateShake)))
            this->scheduleOnce(schedule_selector(MakeStep4::caculateShake), 1);
    }
}

void MakeStep4::inertiaing(LQComponent*,OperateListner* lis){
    auto data = dynAny_cast<float>(lis->getData());
    if(data >1.6)
        data = 1.6;

    auto spoonPos =  m_pSpoon->getParent()->convertToWorldSpace(m_pSpoon->getPosition());
    auto bowPos = _studioLayer->getNodeByName("bowl")->getParent()->convertToWorldSpace(startPos);
    
    spoonPos.rotate(bowPos, -data*0.1);
    m_pSpoon->setPosition(m_pSpoon->getParent()->convertToNodeSpace(spoonPos));
}

void MakeStep4::shake(float distance ,float time){
    auto bowl = _studioLayer->getNodeByName("bowl");
    if(!bowl->getActionByTag(shakeAction)) {
        auto action = Repeat::create(ActionHelper::createShakeAction(distance,time),6);
        action->setTag(shakeAction);
        _studioLayer->getNodeByName("bowl")->setPosition(startPos);
        bowl->runAction(action);
    }
}

void MakeStep4::caculateShake(float){
    isShake = true;
}
void MakeStep4::caculateFastShake(float){
    isFastShake = true;
}
//搅拌失败
void MakeStep4::caculateShakeBad(float){
    if(!isFinish) {
        _lisenter->setEnabled(false);
        
        auto mixPointName = "mixPointName";
        auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
        if(mixPoint) {
            mixPoint->removeFromParent();
        }
        
        _studioLayer->getNodeByName("mixLayer")->getComponent("InertiaComponent")->setEnabled(false);
        _studioLayer->getNodeByName("bowl")->runAction(EaseExponentialIn::create(MoveBy::create(0.5, Vec2(700,700))));
        ActionHelper::delayFunc(0.4, this, [this](){
            
            this->runAction(EaseBounceOut::create(ScaleTo::create(0.5, 1.3)));
            this->runAction(EaseBounceOut::create(MoveBy::create(0.5, Vec2(0, -90))));

            
            AudioHelp::getInstance()->stopLoofEffect();
            
            
            ActionUtils::delayTimeAction(this,1.5,CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("VoiceOver/let's try again.mp3");
            
            }));

            
            
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
void MakeStep4::finish(LQComponent*,OperateListner* lis){
    isFinish = true;
    _lisenter->setEnabled(false);
    
    
    auto mixPointName = "mixPointName";
    auto mixPoint = dynamic_cast<MixPoint*>(_studioLayer->getNodeByName(mixPointName));
    if(mixPoint) {
        mixPoint->removeFromParent();
    }
    
    AudioHelp::getInstance()->stopLoofEffect();
    
    m_pSpoon->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(1000,0)), 1.0),CallFunc::create([=](){
        
//        RewardLayer* layer = RewardLayer::create(RewardType::typeScoop);
//        this->addChild(layer);
//        layer->btnOkClick=[=](){
//        
//            func_bind(nextStep);
//            
//        };
        
        string gameState = UserDefault::getInstance()->getStringForKey(GameState);
        if (gameState == GameStateOne) {
            
            RewardLayer* layer = RewardLayer::create(RewardType::typeScoop);
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
