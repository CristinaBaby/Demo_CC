//
//  GameOverView.cpp
//  Make
//
//  Created by liji on 15/6/3.
//
//

#include "GameOverView.h"
#include "GameMaster.h"
#include "AudioController.h"
GameOverView* GameOverView::create(bool isWin)
{
    GameOverView* pRet = new GameOverView();
    if (pRet && pRet -> init(isWin))
    {
        pRet -> autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;

}

bool GameOverView::init(bool isWin)
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> stopBackgroundMusic();
    
    auto layerColor = LayerColor::create();
    layerColor -> setColor(Color3B(0, 0, 0));
    layerColor -> setOpacity(150);
    layerColor -> setContentSize(Size(9999,9999));
    layerColor -> setPosition(-480, -320);
    addChild(layerColor,0);
    
    listener_ -> retain();
    listener_ -> onTouchBegan = [&](Touch* touch, Event* event){return true;};
    listener_ -> setSwallowTouches(true);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener_ -> clone(), layerColor);

    if (isWin)
    {
        this -> initWinGame();
    }
    else
    {
        this -> initLoseGame();
    }
    
    
    resetGameBtn_ -> setVisible(true);
    nextBtn_ -> setVisible(true);
    resetGameBtn_ -> setPosition(Vec2(200, 300));
    nextBtn_ -> setPosition(Vec2(480, 300));
    
    return true;
}


void GameOverView::initWinGame()
{
    auto star1 = Sprite::create("minigame/popup/star1.png");
    star1 -> setPosition(Vec2(160, 579));
    this -> addChild(star1,1);
    
    auto star2 = Sprite::create("minigame/popup/star2.png");
    star2 -> setPosition(Vec2(329, 661));
    this -> addChild(star2,1);
    
    auto star3 = Sprite::create("minigame/popup/star3.png");
    star3 -> setPosition(Vec2(500, 579));
    this -> addChild(star3,1);

    
    star1 -> setScale(0);
    star2 -> setScale(0);
    star3 -> setScale(0);
    
    CallFunc* lcall = CallFunc::create([&]
                                       {
                                           auto par = ParticleSystemQuad::create("starsRain.plist");
                                           par -> setPosition(Vec2(320, 480));
                                           this -> addChild(par, 4);
                                           
                                           CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/childern happy.mp3");

                                           
                                       });

    
    star1 -> runAction(Sequence::create(DelayTime::create(0.1),
                                        CallFunc::create([&]
                                                         {
                                                             CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/star_success.mp3");
                                                         }),
                                        ScaleTo::create(0.2, 1.2),
                                        ScaleTo::create(0.2, 1),
                                        NULL));
    star3 -> runAction(Sequence::create(DelayTime::create(0.5),
                                        CallFunc::create([&]
                                                         {
                                                             CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/star_success.mp3");
                                                         }),
                                        ScaleTo::create(0.2, 1.2),
                                        ScaleTo::create(0.2, 1),

                                        NULL));
    star2 -> runAction(Sequence::create(DelayTime::create(0.9),
                                        CallFunc::create([&]
                                                         {
                                                             CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/star_success.mp3");
                                                         }),
                                        ScaleTo::create(0.2, 1.2),
                                        ScaleTo::create(0.2, 1),

                                        TargetedAction::create(this, lcall),
                                        NULL));


    
    
    auto word = Sprite::create("minigame/popup/welldone.png");
    word -> setPosition(Vec2(320, 420));
    this -> addChild(word,2);
    
    
    
    

}

void GameOverView::initLoseGame()
{
    

    auto logo = Sprite::create("minigame/popup/ice-failed.png");
    logo -> setPosition(Vec2(320, 600));
    this -> addChild(logo,1);
    
    auto word = Sprite::create("minigame/popup/you failed.png");
    word -> setPosition(Vec2(320, 420));
    this -> addChild(word,2);
    
    auto par = ParticleSystemQuad::create("snow.plist");
    par -> setPosition(Vec2(320, 960));
    this -> addChild(par, 4);

    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/fail2.mp3");

}

void GameOverView::nextBtnClicked()
{
    BaseLayer::nextBtnClicked();

    AudioController::getInstance() -> playBackGroundMusic();

    GameMaster::getInstance() -> goToMakeIceLayer();
}

void GameOverView::resetGameBtnClicked()
{
    BaseLayer::resetGameBtnClicked();
    GameMaster::getInstance() -> goToMiniGameLayer();
}


