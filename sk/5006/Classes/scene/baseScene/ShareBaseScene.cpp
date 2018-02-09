//
//  ShareBaseScene.cpp
//  ccats1061
//
//  Created by liji on 16/8/25.
//
//

#include "ShareBaseScene.h"
#include "SSCInternalLibManager.h"
#include "DialogLayer.h"
#include "SceneHeader.h"
#include "ComponentCycleScroll.h"
#include "ESDataManager.h"
#include "AdsLoadingScene.h"
#include "SSCIAPManager.h"
#include "Analytics.h"
#include "ComponentTouchMove.h"
#define emailSubject "Rainbow Food"
#define emailMessage "Dear friend!\nLook at the delicious Rainbow Food I just made by Rainbow Food!\nIsn't it so cute?\nThere are a lot more tools and decorations in Rainbow Food you'd expect!\nCome on! Download now! Let's have fun together!"
#define albumName "Rainbow Food"

ShareBaseScene::ShareBaseScene():
_nextBtn(nullptr),
_preBtn(nullptr),
_shareBtn(nullptr),
_homeBtn(nullptr),
_isClicked(false),
_eatCount(0),
_coinBg(nullptr),
_coinCount(0),
_allCoinGet(false),
_walkOut(false),
_npc(nullptr),
_isFirstInit(true)

{
//    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("npc1/npc1.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("npc2/npc2.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("npc3/npc3.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("npc0/npc0.ExportJson");

}

ShareBaseScene::~ShareBaseScene()
{
//    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("npc1/npc1.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("npc2/npc2.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("npc3/npc3.ExportJson");
//    cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("npc0/npc0.ExportJson");

}

bool ShareBaseScene::init()
{
    if (!ESMakeBaseScene::init())
    {
        return false;
    }
    
    this->initUI();
//    this->initGirl();
    return true;
}

void ShareBaseScene::initUI()
{
    _nextBtn = Button::create("ui/tap/next_btn.png");
    _nextBtn->loadTexturePressed("ui/tap/next_btn.png");
    _nextBtn->setAnchorPoint(Vec2(1, 1));
    _nextBtn->setPosition(CMVisibleRect::getPosition(10, 10,kBorderRight,kBorderTop));
    this->addToUILayer(_nextBtn);
    _nextBtn->setTag(10);

    _preBtn = Button::create("ui/tap/back_btn.png");
    _preBtn->loadTexturePressed("ui/tap/back_btn.png");
    _preBtn->setAnchorPoint(Vec2(0, 1));
    _preBtn->setPosition(CMVisibleRect::getPosition(10, 10,kBorderLeft,kBorderTop));
    this->addToUILayer(_preBtn);
    _preBtn->setTag(11);

    _shareBtn = Button::create("ui/tap/photo_btn.png");
    _shareBtn->loadTexturePressed("ui/tap/photo_btn.png");
    _shareBtn->setAnchorPoint(Vec2(1, 1));
    _shareBtn->setPosition(CMVisibleRect::getPosition(10, 110,kBorderRight,kBorderTop));
    this->addToUILayer(_shareBtn);
    _shareBtn->setTag(12);
    
    _homeButton->setAnchorPoint(Vec2(0, 1));
    _homeButton->setPosition(CMVisibleRect::getPosition(10, 110,kBorderLeft,kBorderTop));

    auto call = [=](Ref* ref,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(ref);
        if (type == Widget::TouchEventType::BEGAN)
        {
            btn->runAction(Sequence::create(RotateTo::create(0.3f, 1),
                                                     RotateTo::create(0.3f, -1),
                                                     RotateTo::create(0.3f, 0),
                                                     NULL));
        }
        if (type == Widget::TouchEventType::ENDED)
        {
            switch (btn->getTag())
            {
                case 10:// next
                {
                    kAudioUtil->playEffect(kVoiceMakeAnother);
                    _director->getEventDispatcher()->removeAllEventListeners();

                    this->runAction(Sequence::create(DelayTime::create(1.f),
                                                     CallFunc::create([=]
                    {
                        CMAudioUtil::getInstance()->playBackGroundMusic(kBGMDefault);
                        kAudioUtil->setBgMusicVolume(bgmVolume);
                        SceneManager::getInstance()->replaceWithDefaultTransition(ChooseFoodScene::scene());
                    }),
                                                     NULL));
                }
                    break;
                case 11:
                {
                    kAudioUtil->playEffect(kSoundPre);
//                    SceneManager::getInstance()->replaceWithDefaultTransition(ChooseServeScene::scene());
                    if (_preBtnClicked)
                    {
                        _preBtnClicked();
                    }
                }
                    break;
                case 12:
                {
                    kAudioUtil->playEffect(kSoundShare);
                    this->shareBtnClicked();
                }
                    break;
                case 13:
                {
                    kAudioUtil->playEffect(kSoundNext);

                    SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                }
                    break;
                    
                default:
                    break;
            }
        }
    };
    
    _nextBtn->addTouchEventListener(call);
    if (_preBtn)
    {
        _preBtn->addTouchEventListener(call);
    }
    _shareBtn->addTouchEventListener(call);

}

void ShareBaseScene::initEatHint(bool isEat)
{
    kAudioUtil->playEffect(kVoiceTapToDrink);
    auto drinkHint = isEat? Sprite::create("ui/tap/tapeat.png"): Sprite::create("ui/tap/tapeat.png");
    drinkHint->setPosition(CMVisibleRect::getPosition(320, 480+200));
    this->addToUILayer(drinkHint,20);
    
    drinkHint->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.8f, CMVisibleRect::getPosition(320, 480))),
                                          DelayTime::create(1.5f),
                                          Spawn::create(EaseBackIn::create(MoveTo::create(0.8f, CMVisibleRect::getPosition(320, 480+200))),
                                                        FadeOut::create(0.8f),
                                                        NULL),
                                          NULL));

}

void ShareBaseScene::chooseServe()
{
//    int index = random(1, 3);
    
//    this->setBackground(StringUtils::format("res/select/bg%d.png",index));
//    this->setBackground("res/select/street.png");

    Sprite* btm = Sprite::create("res/select/street.png");
    this->addToContentLayer(btm,-1);
    btm->setPosition(CMVisibleRect::getPosition(337, 765));
    btm->setScale(0.4f);
    
    Sprite* bg = Sprite::create();
    this->addToContentLayer(bg,5);
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    
    switch (kESDataManager->getCurrentFood())
    {
//        case ESDataManager::FoodType::kFoodPizzacone:
//        {
//            bg->setTexture("res/serve/pizzacone.png");
//        }
//            break;
//        case ESDataManager::FoodType::kFoodCottonCandy:
//        {
//            bg->setTexture("res/serve/cottoncandy.png");
//        }
//            break;
//        case ESDataManager::FoodType::kFoodCorndog:
//        {
//            bg->setTexture("res/serve/corndogs.png");
//        }
//            break;
            
        default:
            break;
    }


}

void ShareBaseScene::resetEat()
{
    _foodCount--;
    if (_foodCount == 0)
    {
//        this->girlAnother();
        
        _foodCount = _eatComVec.size();
        
        kSoundInstance->playRandomEatVoice();
        auto eatAgainButton = Button::create("ui/tap/eatagain.png");
        eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
        addToUILayer(eatAgainButton, 20);
        
        eatAgainButton->runAction(ActionHelper::createFloatAction());
        
        eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
            
            if (type == Widget::TouchEventType::ENDED)
            {
                for (auto e : _eatComVec)
                {
                    e->reset();
                }
//                for (auto e : _bubbleVec)
//                {
//                    e->removeFromParent();
//                }
//                _bubbleVec.clear();
                
                eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
                eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
            }
        });

    }

}


void ShareBaseScene::shareBtnClicked()
{
    auto call = [=](RenderTexture* rtx, const std::string&)
    {
        auto layer = LayerColor::create(Color4B(0, 0, 0, 185));
        this->addToUILayer(layer,INT_MAX);
//        layer->setOpacity(155);
        {
            //屏蔽事件
            auto eventListener = EventListenerTouchOneByOne::create();
            eventListener->setSwallowTouches(true);
            eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
                
                return true;
            };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
        }

        Vec2 deltaPos = Vec2(0, 0);
        
        auto image = Sprite::createWithTexture(rtx->getSprite()->getTexture());
        layer->addChild(image);
        auto pos = Vec2(41,253);
        Rect area = Rect(0, 0, 542,694);
        area.origin = pos;

//        image->setScale(0.8f);
        image->setTextureRect(area);
        image->setFlippedY(true);
        image->setPosition(CMVisibleRect::getCenterOfScene()+deltaPos);
        
        auto frame = Sprite::create("ui/photo/bg.png");
        frame->setPosition(CMVisibleRect::getPosition(320, 480)+deltaPos);
        layer->addChild(frame,3);
       
//        Button* emailBtn = Button::create("ui/photo/emil_btn.png");
//        emailBtn->setPosition(CMVisibleRect::getPosition(193,132)+deltaPos);
//        layer->addChild(emailBtn,10,1);
//        
        Button* downloadBtn = Button::create("ui/photo/downloads.png");
        downloadBtn->setPosition(CMVisibleRect::getPosition(320,157)+deltaPos);
        layer->addChild(downloadBtn,10,2);
        
        Button* closeBtn = Button::create("ui/photo/x_btn.png");
        closeBtn->setPosition(CMVisibleRect::getPosition(559,801)+deltaPos);
        layer->addChild(closeBtn,10,3);
        
        auto call = [=](Ref* ref,Widget::TouchEventType type)
        {
            auto btn = dynamic_cast<Button*>(ref);
            
            if (type == Widget::TouchEventType::ENDED)
            {
                switch (btn->getTag())
                {
                    case 1:
                    {
                        SSCInternalLibManager::getInstance()->sendEmailAndFilePic(emailSubject, emailMessage, (FileUtils::getInstance()->getWritablePath()+"share.png").c_str());
                    }
                        break;
                    case 2:
                    {
                        if(_isClicked)
                            return;
#if __cplusplus > 201100L
                        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int           requestcode,bool bgranted){
                            onPermissionGrantedResult(requestcode, bgranted);
                        };
#else
                        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
                        
                        //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
                        int requestCode = 1;
                        //调用权限申请的方法,根据需要申请敏感权限
                        RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
                    }
                        break;
                    case 3:
                    {
                        layer->removeFromParent();
                    }
                        break;
                    default:
                        break;
                }
                
            }
        };
        downloadBtn->addTouchEventListener(call);
//        emailBtn->addTouchEventListener(call);
        closeBtn->addTouchEventListener(call);

        
    };
    this->saveForShare("share.png", call);

}

void ShareBaseScene::saveForShare(const std::string& name, const std::function<void(RenderTexture*, const std::string&)>& callback)
{
    auto renderTexture = RenderTexture::create(getContentSize().width, getContentSize().height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

    renderTexture->begin();
    _bgLayer->visit();
    _contentLayer->visit();
    renderTexture->end();
    _director->getRenderer()->render();
    
    renderTexture->saveToFile(name, true, callback);
}


void ShareBaseScene::onEnter()
{
    ESMakeBaseScene::onEnter();
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSIgnoreAllTouchNotification);

}
void ShareBaseScene::onExit()
{
    ESMakeBaseScene::onExit();
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CSCancelIgnoreAllTouchNotification);
    
}

void ShareBaseScene::onEnterTransitionDidFinish()
{
    ESMakeBaseScene::onEnterTransitionDidFinish();
//    this->initFood();

}

void ShareBaseScene::registEat(Sprite *sp)
{
    auto eatShape = Sprite::create("ui/eat1.png");
    eatShape->setScale(0.8);
    auto eatComp = ComponentEat::create();
    eatComp->setEatShape(eatShape);
    eatComp->setAlphaThreshold(10);
    eatComp->setEatRate(0.15);
    eatComp->setName("eatComp1");
    sp->addComponent(eatComp);
    
    eatComp->eatingCallback = [=](Touch* touch)
    {
        static int x = 0;
        x++;
        if (x == 5)
        {
            x = 0;
            kSoundInstance->playRandomEatVoice();
        }
        kAudioUtil->playEffect(kSoundEat);
    };
    
    eatComp->finishEatCallback = [=]()
    {
        this->resetEat();
        kAudioUtil->playEffect("sound/voice over1/Soda/nice work.mp3");
    };
    
    eatComp->start();
    _eatComVec.push_back(eatComp);
    
    _foodCount = _eatComVec.size();

}

void ShareBaseScene::registServe(Node* foodNode,Rect r,function<void()> callfunc)
{
    auto cTouch = ComponentTouchMove::create();
    cTouch->setBackToOriginal(true,foodNode->getPosition());
    cTouch->touchEndedCall=[=](Touch* t)
    {
        if (r.containsPoint(t->getLocation()))
        {
            cTouch->setEnabled(false);
            if (callfunc)
            {
                callfunc();
            }
        }
        else
        {
            cTouch->back();
        }
    };

    foodNode->addComponent(cTouch);
}

void ShareBaseScene::showCoin()
{
    vector<Vec2> posVec =
    {
        CMVisibleRect::getPosition(183-60, 262+30),
        CMVisibleRect::getPosition(320, 262+30),
        CMVisibleRect::getPosition(467+60, 262+30),
    };
    kAudioUtil->playEffect(kSoundPickUpCoin);

    if (!_coinBg)
    {
        _coinBg = Sprite::create("ui/serve/box.png");
        _coinBg->setAnchorPoint(Vec2(0.5, 1));
        _coinBg->setPosition(CMVisibleRect::getPosition(320, 40,kBorderNone,kBorderTop));
        this->addToUILayer(_coinBg,5);
    }
    _tempScore = kESDataManager->getCoinNumber();
    kESDataManager->setCoinNumber(_tempScore+3);
    for (int i = 0; i<3; i++)
    {
//        auto score = Sprite::create("ui/serve/0.png");
//        _coinBg->addChild(score);
//        score->setPosition(Vec2(60+i*20, 29));
//        _scoreNumberVec.push_back(score);
        
        auto coin = Sprite::create("ui/serve/heart.png");
        this->addToUILayer(coin,10);
        coin->setPosition(CMVisibleRect::getCenterOfScene());
        coin->setScale(0);
        coin->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.3f, 0.95f)),
                                         ActionHelper::moveToPostionByBezier(CMVisibleRect::getCenterOfScene(), posVec.at(i), 0.3f),
                                         CallFunc::create([=]
        {
            auto cTouch = ComponentTouchMove::create();
            cTouch->setIsCanMove(false);
            cTouch->touchClickedCall = [=](Touch* t)
            {
                cTouch->setEnabled(false);
                coin->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.2f, 1.05)),
                                                 Spawn::create(ActionHelper::moveToPostionByBezier(coin->getPosition(),_coinBg->getPosition(), 0.6f),
                                                               ScaleTo::create(0.6f, 0.f),
                                                               NULL),
                                                 CallFunc::create([=]
                {
                    //                    addCoinScore();        ;
                    kAudioUtil->playEffect(kSoundCollectpCoin);
                    _coinBg->runAction(Sequence::create(ScaleTo::create(0.2f, 1.05),
                                                        ScaleTo::create(0.2f, 0.95),
                                                        ScaleTo::create(0.2f, 1.f),
                                                        NULL));

                    _coinCount++;
                    auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    particle->setPosition(_coinBg->getPosition()-Vec2(0, 20));
                    this->addToUILayer(particle,6);
                    if (_coinCount >= 3)
                    {
                        _coinCount = 0;
//                        _coinBg->removeFromParent();_coinBg = nullptr;
                        kSoundInstance->playRandomStepFinishVoice();
                        if (_allCoinCollceted)
                        {
                            _allCoinCollceted();
                        }
                    }
                    
                }),
                                                 NULL));
                
            };
            coin->addComponent(cTouch);
        }),
                                         NULL));
    }

//    showCoinScore(_tempScore);
}

void ShareBaseScene::showCoinScore(int number,bool animated)
{

    int h = number/100%10;
    int d = number/10%10;
    int u = number/1%10;
    
    vector<int> temp;
    
    temp.push_back(h);
    temp.push_back(d);
    temp.push_back(u);
    
    if (animated)
    {
        kAudioUtil->playEffect("sound/sound1/popcorn/Pick up coins.mp3");
        _coinBg->runAction(Sequence::create(ScaleTo::create(0.2f, 1.05),
                                            ScaleTo::create(0.2f, 0.95),
                                            ScaleTo::create(0.2f, 1.f),
                                            NULL));
    }
    
    for (int i=0; i<3; i++)
    {
        _scoreNumberVec.at(i)->setTexture(StringUtils::format("ui/serve/%d.png",temp.at(i)));
    }
}

void ShareBaseScene::addCoinScore()
{
    _tempScore++;
    _coinCount++;
    if (_coinCount==3 )
    {
        auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
        particle->setPosition(_coinBg->getPosition());
        this->addToUILayer(particle,6);

        if (_foodServedOver)
        {
            _foodServedOver();
        }
    }
    showCoinScore(_tempScore,true);
}

void ShareBaseScene::showServeAgain(function<void()> call)
{
    auto eatAgainButton = Button::create("ui/serve/serveagain.png");
    eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,100));
    addToUILayer(eatAgainButton, 20);
    
    eatAgainButton->runAction(ActionHelper::createFloatAction());
    
    eatAgainButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            if (call)
            {
                call();
            }
            
            eatAgainButton->getActionManager()->removeAllActionsFromTarget(eatAgainButton);
            eatAgainButton->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(RotateBy::create(0.2, 180.0), ScaleTo::create(0.2, 0.0), NULL)), RemoveSelf::create(), NULL));
        }
    });
}

void ShareBaseScene::resetServe(Node* food)
{
    int npcIndex = random(0, 3);
    
    if (_npc)
    {
        _npc->removeFromParent();_npc = nullptr;
    }
    _npc = Charactor::createModel(StringUtils::format("npc%d",npcIndex));
    this->addToContentLayer(_npc,2,npcIndex);
    _npc->setPosition(CMVisibleRect::getPosition(559+800-30, 361));
    _npc->setScale(0.9f);
    if (_isFirstInit)
    {
        _isFirstInit = false;
        this->chooseServe();
    }
    food->setScale(0.7f);
    auto foodPos = food->getPosition();
    
    
    ActionHelper::moveInToSceneBounce(food, foodPos, kActionDirectionLeft);
    
    auto npcWalkIn = [=]
    {
        TipsUIController::getInstance()->showMoveTip(this, foodPos, CMVisibleRect::getPosition(413+30, 554+50));
        
        kAudioUtil->playEffect(StringUtils::format("sound/npc/hello%d.mp3",_npc->getTag()).c_str());
        
        
        _npc->getAnimation()->stop();
        _npc->getAnimation()->play("breath",-1,1);
        
        this->registServe(food,
                          _npc->getBoundingBox(),
                          [=]
                          {
                              TipsUIController::getInstance()->removeAllTips();
                              
                              Bone* bone = _npc->getBone("MS");
                              if (!bone)
                                  bone = _npc->getBone("ms");

                              
                              food->runAction(Spawn::create(ScaleTo::create(0.7f, 0.32),
                                                            MoveTo::create(0.7f, CMVisibleRect::getPosition(313+80, 554+50)),
                                                            NULL));
                              _npc->getAnimation()->play("na",-1,1);
                              
                              auto spawn = Spawn::create(MoveBy::create(4.5f, CMVisibleRect::getPosition(-900, 0)),
                                                         Sequence::create(DelayTime::create(1.f),
                                                                          CallFunc::create([=]
                                                                                           {
                                                                                               this->showCoin();
                                                                                               this->_allCoinCollceted = [=]
                                                                                               {
                                                                                                   _allCoinGet = true;
                                                                                                   checkReserve();
                                                                                               };
                                                                                           }),
                                                                          NULL),
                                                         CallFunc::create([=]
                                                                          {
                                                                              _walkOut = true;
                                                                              checkReserve();
                                                                          }),
                                                         
                                                         NULL);
                              
                              _npc->runAction(Sequence::create(DelayTime::create(0.6f),
                                                               CallFunc::create([=]
                                                                                {
                                                                                    kAudioUtil->playEffect(StringUtils::format("sound/npc/cool%d.mp3",_npc->getTag()).c_str());
                                                                                    
                                                                                    _npc->getAnimation()->stop();
                                                                                    auto sp =Sprite::create();
//                                                                                    juice->showDrink();
                                                                                    food->retain();
                                                                                    food->setScale(1.f);
                                                                                    food->setPosition(Vec2::ZERO);
                                                                                    food->removeFromParent();
                                                                                    sp->addChild(food);
                                                                                    sp->setPosition(Vec2(0, 10));
                                                                                    bone->addDisplay(sp,0);
                                                                                    bone->setIgnoreMovementBoneData(true);
                                                                                    bone->setScale(0.3);
                                                                                    bone->changeDisplayWithIndex(0, true);
                                                                                    _npc->getAnimation()->play("take",-1,1);
                                                                                }),
                                                               spawn,
                                                               NULL));
                          });
        
    };
    _npc->walkTo(CMVisibleRect::getPosition(556-30, 361), 3.5f, npcWalkIn);
}

void ShareBaseScene::checkReserve()
{
    if (_allCoinGet && _walkOut)
    {
        _allCoinGet = false;
        _walkOut = false;
//        this->resetServe();
        if (_foodServedOver)
            _foodServedOver();
        
    }
}

void ShareBaseScene::shareImage(){
    
    
    auto image = new Image();
    image->initWithImageFile(FileUtils::getInstance()->getWritablePath() + "share.png");
    
    image->saveToFile( SSCInternalLibManager::getInstance()->getInstance()->getSDCardPath()+"/"+albumName+"/share.png");
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic(emailSubject, emailMessage, (SSCInternalLibManager::getInstance()->getInstance()->getSDCardPath()+"/"+albumName+"/share.png").c_str());
}
void ShareBaseScene::saveImage(){
    _eventDispatcher->setEnabled(false);
    _isClicked = true;
    auto image = new Image();
    image->initWithImageFile(FileUtils::getInstance()->getWritablePath() + "share.png");
    SSCInternalLibManager::getInstance()->saveToAlbum(image, [=](bool flag){
        if(flag) {
            DialogLayer::showWithMessage("Image was saved to \nyour Camera Roll!", [=](){
                
                
            }, nullptr, Director::getInstance()->getRunningScene(), 40.f,true,true);
            
            
        } else {
            DialogLayer::showWithMessage("Image was unable to be saved. Please check your device storage and try again.", [=](){
                
                
            }, nullptr, Director::getInstance()->getRunningScene(), 40.0,true,true);
            
        }
        Director::getInstance()->getRenderer()->render();
        _isClicked = false;
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
    },albumName);
    
}


void ShareBaseScene::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            this->runAction(Sequence::create(DelayTime::create(0.0f),
                                             CallFunc::create([=] {
                saveImage();
            }),NULL))    ;
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
    if (requestCode == 2) {
        if (bGranted) {
            this->runAction(Sequence::create(DelayTime::create(0.0f),
                                             CallFunc::create([=] {
                shareImage();
            }),NULL))    ;
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
    
}


