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
#include "SSCFileUtility.h"
#include "ComponentTouchMove.h"
#define emailSubject "Movie Night Party"
#define emailMessage "It's Movie Time. Let's make yummy snacks like popcorn, soda and Texas toast pizza to serve to the crazy moviegoers. It'll be so much fun. We are the real chefs. Let's Play."
#define albumName "MovieNight"


ShareBaseScene::ShareBaseScene():
_nextBtn(nullptr),
_preBtn(nullptr),
_shareBtn(nullptr),
_homeBtn(nullptr),
_isClicked(false),
_eatCount(0),
_girl(nullptr),
_girlIndex(1),
_coinBg(nullptr),
_coinCount(0)
{

}

ShareBaseScene::~ShareBaseScene()
{

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
    _nextBtn->setAnchorPoint(Vec2(1, 1));
    _nextBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderRight,kBorderTop));
    this->addToUILayer(_nextBtn);
    _nextBtn->setTag(10);
    
    _preBtn = Button::create("ui/tap/back_btn.png");
    _preBtn->setAnchorPoint(Vec2(0, 1));
    _preBtn->setPosition(CMVisibleRect::getPosition(20, 20,kBorderLeft,kBorderTop));
    this->addToUILayer(_preBtn);
    _preBtn->setTag(11);

    _shareBtn = Button::create("ui/tap/photo_btn.png");
    _shareBtn->setAnchorPoint(Vec2(0, 0));
    _shareBtn->setPosition(CMVisibleRect::getPosition(20, 100,kBorderLeft,kBorderBottom));
    this->addToUILayer(_shareBtn);
    _shareBtn->setTag(12);
    
    _homeButton->setAnchorPoint(Vec2(1, 0));
    _homeButton->setPosition(CMVisibleRect::getPosition(20, 100,kBorderRight,kBorderBottom));

    
    auto call = [=](Ref* ref,Widget::TouchEventType type)
    {
        auto btn = dynamic_cast<Button*>(ref);
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
                        SceneManager::getInstance()->replaceWithDefaultTransition(ChooseFoodScene::scene());
                    }),
                                                     NULL));
                }
                    break;
                case 11:
                {
                    kAudioUtil->playEffect(kSoundPre);
                    SceneManager::getInstance()->replaceWithDefaultTransition(ChooseServeScene::scene());
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

                    if (SSCIAPManager::getInstance()->isShowAds())
                    {
                        AdsLoadingScene::showLoading<AdsLoadingScene>(true,nullptr,INT_MAX);
                        AdsLoadingScene::loadingDoneCallback = [=]
                        {
                            SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                        };
                    }
                    else
                    {
                        SceneManager::getInstance()->replaceWithDefaultTransition(HomeScene::scene());
                    }
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
    auto drinkHint = isEat? Sprite::create("ui/tap/tapeat.png"): Sprite::create("ui/tap/tapdrink.png");
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
    this->setBackground("res/serve/serve_bg.png");
    auto desk = Sprite::create("res/serve/desk1.png");
    desk->setAnchorPoint(Vec2(0.5, 0));
    desk->setPosition(CMVisibleRect::getPosition(320, 0,kBorderNone,kBorderBottom));
    this->addToBgLayer(desk,3);

    this->initGirl();
    _girl->setPosition(CMVisibleRect::getPosition(520+800, 630));
    auto girlWalkIn = [=]
    {
        if (_girlWalkedIn)
            _girlWalkedIn();
        this->girlStand();
    };
    this->girlWalk(CMVisibleRect::getPosition(520, 630), 2.f, girlWalkIn);
}


void ShareBaseScene::initGirl()
{
    _girlIndex = random(1, 2);
    
    _girl = Sprite::create(StringUtils::format("res/girl/boby_%d/stand/1.png",_girlIndex));
    this->addToBgLayer(_girl,2);
    _girl->setPosition(CMVisibleRect::getPosition(520,630));
    _girl->setScale(0.7f);
}

void ShareBaseScene::girlStand()
{
    _girl->stopAllActions();

    auto stand = ActionHelper::createAnimate("res/girl/boby_" + to_string(_girlIndex)+"/stand/%d.png", 1, 3,0.3f);
    _girl->runAction(RepeatForever::create(Sequence::create(stand,
                                                            DelayTime::create(0.5f),
                                                            NULL)));
}

void ShareBaseScene::girlSmile()
{
    _girl->stopAllActions();
    _girl->setTexture(StringUtils::format("res/girl/boby_%d/stand/1.png",_girlIndex));
    auto smile = ActionHelper::createAnimate("res/girl/boby_" + to_string(_girlIndex)+"/finishEat/%d.png", 7, 13,0.2f);
    _girl->runAction(Sequence::create(smile,
                                      CallFunc::create(CC_CALLBACK_0(ShareBaseScene::girlStand, this)),
                                      CallFunc::create([=]
    {
        kAudioUtil->playEffect("sound/voice over1/Soda/nice work.mp3");
        this->showCoin();
    }),
                                      NULL));
}

void ShareBaseScene::girlWalk(Vec2 endPos,float duration,function<void()> call)
{
    _girl->stopAllActions();
    _girl->setScale(0.7f);
    auto walk = RepeatForever::create(ActionHelper::createAnimate("res/girl/boby_" + to_string(_girlIndex)+"/walking/1/boby_r%d.png", 1, 3,0.2f));
    _girl->runAction(walk);
    _girl->runAction(Sequence::create(MoveTo::create(duration, endPos),
                                      CallFunc::create(call),
                                      NULL));
}


void ShareBaseScene::girlEat()
{
    _girl->stopAllActions();
    _girl->setTexture(StringUtils::format("res/girl/boby_%d/stand/1.png",_girlIndex));
    auto eat = ActionHelper::createAnimate("res/girl/boby_" + to_string(_girlIndex)+"/eat/%d.png", 4, 9,0.2f);
    _girl->runAction(Sequence::create(eat,
                                      CallFunc::create(CC_CALLBACK_0(ShareBaseScene::girlStand, this)),
                                      NULL));
}


void ShareBaseScene::initFood()
{
    auto bg = Sprite::create("res/share/bg.png");
    bg->setPosition(CMVisibleRect::getCenterOfScene());
    this->addToContentLayer(bg);
    
    auto bg2 = Sprite::create("res/share/table.png");
    bg2->setPosition(CMVisibleRect::getCenterOfScene());
    this->addToContentLayer(bg2,3);
    
    auto cottonNode = Node::create();
    this->addToContentLayer(cottonNode,10);
    cottonNode->setScale(0.8f);
    cottonNode->setPosition(CMVisibleRect::getPosition(273+100, 280));

    auto cottonBtm = Sprite::create("res/share/box_1.png");
    cottonBtm->setPosition(Vec2(0, -140));
    cottonNode->addChild(cottonBtm,1);
    cottonBtm->setScale(1.2f);

    auto cottonTop = Sprite::create("res/share/box_2.png");
    cottonTop->setPosition(Vec2(0, -140));
    cottonNode->addChild(cottonTop,3);
    cottonTop->setScale(1.2f);

    string stickPath = StringUtils::format("res/decoration/stick/stick_%d.png",kESDataManager->getCottonCandyFood().stickIndex);
    auto stick = Sprite::create(stickPath);
    cottonNode->addChild(stick,2);
    stick->setScale(0.5f);

    {
        auto path = FileUtils::getInstance()->getWritablePath()+ ScreenNameCotton;
        if (_director->getTextureCache()->getTextureForKey("paint_1"))
        {
            _director->getTextureCache()->removeTextureForKey("paint_1");
        }
        Image *img = new Image();
        img->initWithImageFile(path);
        auto sp = Sprite::createWithTexture(_director->getTextureCache()->addImage(img, "paint_1"));
        cottonNode->addChild(sp,3);
        sp->setPosition(Vec2(0, 137)-Vec2(0,40)/2);
        log("%f , %f,",sp->getContentSize().width,sp->getContentSize().height);
        kAudioUtil->playEffect(kSoundShow);
        
        auto eatShape = Sprite::create("ui/eat1.png");
        auto eatComp = ComponentEat::create();
        eatComp->setEatShape(eatShape);
        eatComp->setAlphaThreshold(10);
        eatComp->setEatRate(0.05);
        eatComp->setName("eatComp");
        sp->addComponent(eatComp);
        eatComp->eatingCallback = [=](Touch* touch)
        {
            static int x = 0;
            x++;
            if (x == 5)
            {
                x = 0;
                kSoundInstance->playRandomEatVoice();
                this->girlEat();
            }
            kAudioUtil->playEffect(kSoundEat);
            
        };
        
        eatComp->finishEatCallback = [=]()
        {
            this->resetEat();

        };
        
        eatComp->start();
        _eatComVec.push_back(eatComp);

        
        delete img;
    }
    
    if (kESDataManager->getCottonCandyFood().hasOtherFood)
    {
        auto plate = Sprite::create("res/decoration/other food/plate.png");
        this->addToContentLayer(plate,11);
        plate->setPosition(CMVisibleRect::getPosition(570+100, 167));
        plate->setScale(0.45);
        
        auto path = FileUtils::getInstance()->getWritablePath()+ ScreenNameFood;
        if (_director->getTextureCache()->getTextureForKey("paint_2"))
        {
            _director->getTextureCache()->removeTextureForKey("paint_2");
        }
        Image *img = new Image();
        img->initWithImageFile(path);
        auto sp = Sprite::createWithTexture(_director->getTextureCache()->addImage(img, "paint_2"));
        this->addToContentLayer(sp,12);
        sp->setPosition(CMVisibleRect::getPosition(571+100, 226));
        log("%f , %f,",sp->getContentSize().width,sp->getContentSize().height);
        kAudioUtil->playEffect(kSoundShow);
        
        auto eatShape = Sprite::create("ui/eat1.png");
        auto eatComp = ComponentEat::create();
        eatComp->setEatShape(eatShape);
        eatComp->setAlphaThreshold(10);
        eatComp->setEatRate(0.05);
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
        };
        
        eatComp->start();
        _eatComVec.push_back(eatComp);

        delete img;
    }

    _foodCount = _eatComVec.size();
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
        auto layer = LayerColor::create(Color4B(0, 0, 0, 155));
        this->addToUILayer(layer,INT_MAX);
        
        {
            //屏蔽事件
            auto eventListener = EventListenerTouchOneByOne::create();
            eventListener->setSwallowTouches(true);
            eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
                
                return true;
            };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
        }

        auto image = Sprite::createWithTexture(rtx->getSprite()->getTexture());
        layer->addChild(image);
        auto pos = CMVisibleRect::getPosition(-40,201);
        Rect area = Rect(0, 0, 686,671);
        area.origin = pos;

        image->setScale(0.8f);
        image->setTextureRect(area);
        image->setFlippedY(true);
        image->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,10));
        
        auto frame = Sprite::create("ui/photo/bg.png");
        frame->setPosition(CMVisibleRect::getCenterOfScene());
        layer->addChild(frame,3);
       
        Button* emailBtn = Button::create("ui/photo/emil_btn.png");
        emailBtn->setPosition(CMVisibleRect::getPosition(151,210));
        layer->addChild(emailBtn,10,1);
        
        Button* downloadBtn = Button::create("ui/photo/download_btn.png");
        downloadBtn->setPosition(CMVisibleRect::getPosition(495, 200));
        layer->addChild(downloadBtn,10,2);
        
        Button* closeBtn = Button::create("ui/photo/x_btn.png");
        closeBtn->setPosition(CMVisibleRect::getPosition(585,757));
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
#if __cplusplus > 201100L
                        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int           requestcode,bool bgranted){
                            onPermissionGrantedResult(requestcode, bgranted);
                        };
#else
                        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
                        
                        //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
                        int requestCode = 2;
                        //调用权限申请的方法,根据需要申请敏感权限
                        RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
                        

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
        emailBtn->addTouchEventListener(call);
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
    
    Image* pImage = renderTexture->newImage();
    std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = SSCFileUtility::getStoragePath()+name;
#else
    path = SSCFileUtility::getStoragePath()+"/"+name;
#endif
    pImage->saveToFile(path, false);
    pImage->autorelease();
    callback(renderTexture,path);
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

void ShareBaseScene::registServe(Node* foodNode ,function<void()> callfunc)
{
    kAudioUtil->playEffect(kVoiceDragServe);
    
    auto cTouch = ComponentTouchMove::create();
    cTouch->setBackToOriginal(true,foodNode->getPosition());
    cTouch->touchEndedCall=[=](Touch* t)
    {
        if (_girl->getBoundingBox().containsPoint(t->getLocation()))
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
        CMVisibleRect::getPosition(183, 562),
        CMVisibleRect::getPosition(320, 586),
        CMVisibleRect::getPosition(467, 562),
    };
    
    _coinBg = Sprite::create("ui/serve/gold_2.png");
    _coinBg->setAnchorPoint(Vec2(0.5, 1));
    _coinBg->setPosition(CMVisibleRect::getPosition(320, 20,kBorderNone,kBorderTop));
    this->addToUILayer(_coinBg,5);
    _tempScore = kESDataManager->getCoinNumber();
    kESDataManager->setCoinNumber(_tempScore+3);
    for (int i = 0; i<3; i++)
    {
        auto score = Sprite::create("ui/serve/0.png");
        _coinBg->addChild(score);
        score->setPosition(Vec2(60+i*20, 29));
        _scoreNumberVec.push_back(score);
        
        auto coin = Sprite::create("ui/serve/gold.png");
        this->addToUILayer(coin,10);
        coin->setPosition(CMVisibleRect::getCenterOfScene());
        coin->setScale(0);
        coin->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.3f, 1)),
                                         ActionHelper::moveToPostionByBezier(CMVisibleRect::getCenterOfScene(), posVec.at(i), 0.3f),
                                         CallFunc::create([=]
        {
            auto cTouch = ComponentTouchMove::create();
            cTouch->setIsCanMove(false);
            cTouch->touchClickedCall = [=](Touch* t)
            {
                cTouch->setEnabled(false);
                coin->runAction(Sequence::create(Spawn::create(ActionHelper::moveToPostionByBezier(coin->getPosition(),_coinBg->getPosition(), 0.6f),
                                                               ScaleTo::create(0.6f, 0.f),
                                                               NULL),
                                                 CallFunc::create([=]
                {
                    addCoinScore();
                }),
                                                 NULL));
                
            };
            coin->addComponent(cTouch);
        }),
                                         NULL));
    }

    showCoinScore(_tempScore);
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
    auto eatAgainButton = Button::create("ui/chooseserve/serveagain.png");
    eatAgainButton->setPosition(CMVisibleRect::getCenterOfScene()+Vec2(0,-100));
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

void ShareBaseScene::shareImage(){
    
    
    auto image = new Image();
    image->initWithImageFile(FileUtils::getInstance()->getWritablePath() + "share.png");
    
    std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = SSCFileUtility::getStoragePath()+"share.png";
#else
    path = SSCFileUtility::getStoragePath()+"/share.png";
#endif
    SSCInternalLibManager::getInstance()->sendEmailAndFilePic(emailSubject, emailMessage, (FileUtils::getInstance()->getWritablePath() + "share.png").c_str());
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
        image->release();
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
