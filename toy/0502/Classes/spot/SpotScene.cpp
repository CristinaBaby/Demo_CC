//
//  SpotScene.cpp
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-27.
//
//

#include "SpotScene.h"
#include "../AppConfig.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/STMultiScreenHelper.h"
#include "../utilities/STUtility.h"
#include "../utilities/STMultiScreenHelper.h"
#include "../widgets/STCoverLayer.h"
#include "../widgets/STUILayer.h"
#include "../helpers/EncryptDataHelper.h"
#include "../helpers/PurchaseHelper.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "../modules/STSystemFunction.h"
#include "../utilities/UICpp.h"
#include "../gameconfig/MyGameConfig.h"
#include "../gameconfig/BalloonManager.h"


//add by luoxp
#include "cocostudio/CCSGUIReader.h"
#include "ui/UILayout.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;


USING_NS_CC;
using cocos2d::extension::Scale9Sprite;
using std::string;
using namespaceST::STUtility;
int AppGlobal::playGameAttackTimes = 0;

#define TARGET_OBJECTS_NUMBER                           5
// object group name in the ".tmx" map file
#define st_map_group_obj_name                           "Objects"
#define st_map_obj_bg_suffix                            "_bg"

#define NAME_MAP_LAYER_BATCH                            "NAME_MAP_LAYER_BATCH"
#define NAME_COVER_LAYER                                "NAME_COVER_LAYER"
#define NAME_UI_LAYER_TOP                               "NAME_UI_LAYER_TOP"
#define NAME_UI_LAYER_BOTTOM                            "NAME_UI_LAYER_BOTTOM"

#define V_RECT                                          MultiScreenHelper::visible_rect


enum {
    z_bg = 0, z_block = 3, z_studio = 5, z_map = 6, z_indicator = 7, z_ui = 9, // z order
    
    tag_ProgressToAction = 400, // tags
};


enum UI_TAG{
    T_LEFT_BG = 154,
    T_RIGHT_BG = 165,
};

#define TIME_SCHEDULE       schedule_selector(SpotScene::scheduleCallback)
#define COMBO_SCHEDULE      schedule_selector(SpotScene::onComboCallback)

static const int FREEZE_TIME = 5;
static const string GAME_UI_FONT = "bmfonts/gameuifont.fnt";



Scene* SpotScene::createScene(LevelInfo* level)
{
    auto scene = Scene::create();
    
    auto layer = SpotScene::create(level);
    layer->setName("SpotScene");
    
    scene->addChild(layer);
    
    return scene;
}

SpotScene * SpotScene::create(LevelInfo* level)
{
    auto game = new (std::nothrow) SpotScene();
    if (game && game->initData(level)) {
        game->autorelease();
        
        return game;
    }
    
    CC_SAFE_DELETE(game);
    return nullptr;
}

SpotScene:: ~SpotScene()
{
    mTargetObjects.clear();
    
    CC_SAFE_RELEASE_NULL(_mLevelInfo);
}


bool SpotScene::initData(LevelInfo* level)
{
    if (! Layer::init()) {
        return false;
    }
    
    _mLevelInfo = level;
    CC_SAFE_RETAIN(_mLevelInfo);
    
    _mHintNumber = Value( EncryptDataHelper::getPorpsQuantity(key_hint_quantity, 3) );
    _mFreezeNumber = Value( EncryptDataHelper::getPorpsQuantity(key_freeze_quantity, 2) );
    _mAddTimeNumber = Value( EncryptDataHelper::getPorpsQuantity(key_addtiome_quantity, 1) );
    
    this->initGameStatus();
    _mSpentTime = 1;
    
    
    bool bCon = false;
    do {
        //我的代码从这里开始
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui04_spot.csb"));
        addChild(_root, z_studio);
        
        quickButton("ui06_back", _root, CC_CALLBACK_2(SpotScene::onButton, this));
        
        auto text = (Text*)Helper::seekWidgetByName(_root, "ui04_get_label");
        text->setFontName("Times BoldItalic.ttf");
        
        for (int i = 0; i < TARGET_OBJECTS_NUMBER; i ++) {
            string name = "ui04_get_" + std::to_string(i + 1);
            auto check = (CheckBox*)Helper::seekWidgetByName(_root, name);
            check->setSelectedState(true);
            check->setTouchEnabled(false);
            _vStars.push_back(check);
        }
        
        mpIndicatorLayer = SpriteBatchNode::create("game.pvr.ccz");
        this->addChild(mpIndicatorLayer, z_indicator);

        _left = Helper::seekWidgetByTag(_root, T_LEFT_BG);
        _right = Helper::seekWidgetByTag(_root, T_RIGHT_BG);
        
        float margin = MultiScreenHelper::getValue(2);
        Size blockSize = MultiScreenHelper::getSize(950, 1150);
        
        mMapDistance = 2 * margin + blockSize.width;
        
        mLeftBlockCenter = _left->getWorldPosition();
        mRightBlockCenter = _right->getWorldPosition();
        
        
        CC_BREAK_IF(! buildMap() );
        
        CC_BREAK_IF(! buildUI() );
        
        
        this->schedule(TIME_SCHEDULE, 1.f);
        
        
        AudioHelper::getInstance()->preloadSound(sound_game_combo);
        AudioHelper::getInstance()->preloadSound(sound_tap_right);
        AudioHelper::getInstance()->preloadSound(sound_tap_error);
        
        schedule(schedule_selector(SpotScene::updateRecord), 1);
        
        bCon = true;
    } while (false);
    
    return bCon;
}


void SpotScene::updateRecord(float dt)
{
    if (xDialog->isVisible() == false)
    {
        _times++;
        quickSetString("ui04_get_label", _root, Util::formatTime(_times));
    }
}


bool SpotScene:: buildUI()
{
    return true;
    
    float positionX = MultiScreenHelper::horizontalCenter(0).x;
    float positionY = MultiScreenHelper::getScreenTop();
    
    // ---------------------------------------------------------
    
    // bottom background
    //bottomBG->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    //bottomBG->setPosition(positionX, MultiScreenHelper::visible_rect.origin.y);
    
    /* create top bar */
    auto topBG = Sprite::createWithSpriteFrameName("ui08_top.png");
    topBG->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    topBG->setPosition(Point(positionX, positionY));
    this->addChild(topBG, z_ui);
    topBG->setScale(UI_SCALE);
    topBG->setName(NAME_UI_LAYER_TOP);
    
    Size bgSize = topBG->getContentSize();
    
    mpProgressTimer = nullptr;
    
    // zen mode don't have time limit
    if (_mLevelInfo->m_GameMode != AppGlobal::GameMode::ZEN) {
        
        mpProgressTimer = ProgressTimer::create(Sprite::createWithSpriteFrameName("ui08_time_bar.png"));
        mpProgressTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        positionY = bgSize.height - MultiScreenHelper::getValue(37);
        mpProgressTimer->setPosition(MultiScreenHelper::fixedY(363, positionY));
        topBG->addChild(mpProgressTimer);
        
        mpProgressTimer->setType(ProgressTimer::Type::BAR);
        mpProgressTimer->setPercentage(100);
        // 变化终点的相对位置，类似锚点
        mpProgressTimer->setMidpoint(Vec2(0, 0.5));
        // x代表宽度，y代表高度，数字表示变化尺度，0代表保持初始数量的100%，即该方向上不做变化
        mpProgressTimer->setBarChangeRate(Vec2(1, 0));
        // 调整label和进度条之间的同步误差
        auto _mProgressToAction = ProgressTo::create(_mGameMaxTime, 0);
        _mProgressToAction->setTag(tag_ProgressToAction);
        mpProgressTimer->runAction(_mProgressToAction);
        
        auto countdown = Label::createWithBMFont(GAME_UI_FONT, ("Countdown"), TextHAlignment::CENTER);
        topBG->addChild(countdown);
        countdown->setPosition(MultiScreenHelper::getPosition(40, 114));
        countdown->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    }
    
    
    
    
    
    auto pauseItem = MenuItemSprite::create( Sprite::createWithSpriteFrameName("ui08_btn_pause.png"),
                                            Sprite::createWithSpriteFrameName("ui08_btn_pause.png"),
                                            CC_CALLBACK_1(SpotScene::onPauseCallback, this));
    pauseItem->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    positionX = bgSize.width - MultiScreenHelper::getValue(50);
    positionY = bgSize.height - MultiScreenHelper::getValue(14);
    pauseItem->setPosition(Point(positionX, positionY));
    
    auto menu2 = Menu::create(pauseItem, nullptr);
    menu2->setPosition(Vec2::ZERO);
    topBG->addChild(menu2);
    
    
    
    // ---------------------------------------------------------
    
    /* create stars on the bottom of screen */
    
    // just only zen mode can't use this prop
    MenuItemSprite *freezeItem = nullptr;
    if (_mLevelInfo->m_GameMode == AppGlobal::GameMode::ZEN) {
        freezeItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("ui08_tips_freeze_u.png"),
                                            Sprite::createWithSpriteFrameName("ui08_tips_freeze_u.png"));
        
    } else {
        
        freezeItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("ui08_tips_freeze.png"),
                                            Sprite::createWithSpriteFrameName("ui08_tips_freeze.png"),
                                            CC_CALLBACK_1(SpotScene::onFreezeCallback, this));
    }
    freezeItem->setPosition(bgSize / 2);
    
    
    // only time mode have this prop
    MenuItemSprite *addTimeItem = nullptr;
    if (_mLevelInfo->m_GameMode == AppGlobal::GameMode::TIME) {
        addTimeItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("ui08_tips_addtime.png"),
                                             Sprite::createWithSpriteFrameName("ui08_tips_addtime.png"),
                                             CC_CALLBACK_1(SpotScene::onAddTimeCallback, this));
        
    } else {
        addTimeItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("ui08_tips_addtime_u.png"),
                                             Sprite::createWithSpriteFrameName("ui08_tips_addtime_u.png"));
    }
    positionX = bgSize.width - MultiScreenHelper::getValue(35);
    addTimeItem->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    addTimeItem->setPosition(positionX, bgSize.height / 2);
    
    return true;
}


void SpotScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    xAudio->playDefaultSound();
    
    if (name.compare("ui06_back") == 0) {
        xDialog->showQuit("You will lose your current progress if you quit the game,are you sure?", CC_CALLBACK_1(SpotScene::onDialog, this));
    }
}


void SpotScene::onDialog(const string& name)
{
    xBalloon->stop();
    
    xAudio->playDefaultSound();
    
    if(name.compare(dialogYes) == 0)
    {
        xScene->back();
    }
    else if (name.compare(dialogQuit) == 0) {
        xScene->back();
    }
    else if (name.compare(dialogReplay) == 0) {
        resetLevel();
        xDialog->hide();
    }
}


bool SpotScene::buildMap()
{
    auto _info = _mLevelInfo->m_Info;
    
    ValueMap::const_iterator level_iter = _info.find(st_level_identifer);
//    if (level_iter == _info.end())
//    {
//        CCLOG("not found the item named >>> %s <<<", st_level_identifer);
//        return false;
//    }
    const string levelID = xGame->getFileName();
    
    //
    const string packedPicPrefix = string("spot/").append(levelID)
    .append(st_file_seperator).append(levelID);
    
    
    // create tiled map
    const string mapPath = string(packedPicPrefix).append(st_map_suffix);
    
    auto map = experimental::TMXTiledMap::create(mapPath);
    if (nullptr == map) {
        CCLOG("ERROR: load map >>>>> %s <<<<< failed!", mapPath.c_str());
        return false;
    }
    
    
    // get objects from group and create sprites with batch node
    const string packedFile = string(packedPicPrefix).append(st_texture_packer_file_suffix);
    const string packedPic = string(packedPicPrefix).append(st_texture_packer_image_suffix);
    
    // add pictures to cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(packedFile, packedPic);
    
    // NOTE: some devices will not increased automatic(such like HTC G14),so we set max capacity manual.
    //这个东东, 可以不要了.
    
    auto objGroup = map->getObjectGroup(st_map_group_obj_name);
    if (nullptr == objGroup) {
        CCLOG("ERROR: get objects group >>>>> %s <<<<< failed!", st_map_group_obj_name);
        return false;
    }
    
    
    // get all objects information in the map file
    auto objects = objGroup->getObjects();
    
    
    /* set target object random */
    int loop = 0;
    int targetIndex[TARGET_OBJECTS_NUMBER] = {};
    
    while (loop < TARGET_OBJECTS_NUMBER) {
        int temp = STUtility::getRandom(objects.size());
        
        if (loop == 0) {
            targetIndex[loop++] = temp;
            continue;
        }
        
        bool got = false;
        for(int t = 0; t < TARGET_OBJECTS_NUMBER; ++t) {
            if (temp == targetIndex[t]) {
                got = true;
                break;
            }
        }
        
        if (got) continue;
        
        targetIndex[loop++] = temp;
    }
    
    float leftEdge = 0.f, rightEdge = 0.f;
    float topEdge = 0.f, bottomEdge = 0.f;
    
    // set background picture name
    const string bgName = string(levelID).append(st_map_obj_bg_suffix).append(st_suffix_png);
    
    /* we need create two identical maps */
    loop = 2;
    while (loop > 0) {
        
        string spriteName("");
        
        // create background
        auto mapBG = Sprite::createWithSpriteFrameName(bgName);
        //mMapLayer->addChild(mapBG);
        
        
        
        mapBG->setScale(0.87);              //这个是根据studio中Frame的大小试出来的.
        
        if (STUtility::isOddNumber(loop)) {
            V::setPosition(mapBG, mRightBlockCenter);
            spriteName.append("right_%d");
            
            rightEdge = mapBG->getBoundingBox().getMaxX();
            
            mapBG->setPosition(_right->getContentSize() / 2);
            _right->addChild(mapBG);
            
        } else {
            V::setPosition(mapBG, mLeftBlockCenter);
            spriteName.append("left_%d");
            
            leftEdge = mapBG->getBoundingBox().getMinX();
            bottomEdge = mapBG->getBoundingBox().getMinY();
            topEdge = mapBG->getBoundingBox().getMaxY();
            
            mapBG->setPosition(_left->getContentSize() / 2);
            _left->addChild(mapBG);
        }
        
        
        // add touch listener
        auto touchLtr = EventListenerTouchOneByOne::create();
        touchLtr->setSwallowTouches(true);
        touchLtr->onTouchBegan = [this](Touch* touch, Event* event){
            // 获取事件所绑定的 target
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            
            // 获取当前点击点所在相对按钮的位置坐标
            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
            Size s = target->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            
#if DEBUG_GAME
            _mIsGameFailed = false;
            scheduleOnce(schedule_selector(SpotScene::finishLevel), 0.8);
#endif
            
            // 点击范围判断检测
            if (rect.containsPoint(locationInNode)) {
                
                // check target sprites zone.
                for (int idx = 0; idx < mTargetObjects.size(); ++idx)
                {
                    auto spr = mTargetObjects.at(idx);
                    
                    Point _location = spr->convertToNodeSpace(touch->getLocation());
                    Size _size = spr->getContentSize();
                    Rect _rect = Rect(-15, -15, _size.width + 30, _size.height + 30);
                    
                    if (_rect.containsPoint(_location)) {
                        
                        this->spotDifference(spr, Point(_rect.getMidX(), _rect.getMidY()));
                        return true;
                    }
                }
                
                
                this->touchWrong(touch->getLocation());
                return true;
            }
            
            return false;
        };
        // regist touch listener
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLtr, mapBG);
        
        
        for (decltype(objects.size()) ix = 0; ix != objects.size(); ++ix)
        {
            // get object
            auto obj = objects[ix].asValueMap();
            // xxx.png
            string objName = obj["name"].asString().append(st_suffix_png);
            // get object position
            float x= obj["x"].asFloat(), y = obj["y"].asFloat();
            
            if (objName.find("bg") != string::npos) continue;
            
            auto sprite = Sprite::createWithSpriteFrameName(objName);
            if (nullptr == sprite) {
                CCLOG("ERROR: get object >>>>> %s <<<<< failed!", objName.c_str());
                continue; // skip this object avoid crashed.
            }
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprite->setPosition(MultiScreenHelper::getPosition1024(x, y));
            mapBG->addChild(sprite, 1);
            
            char sprName[10] = {};
            sprintf(sprName, spriteName.c_str(), ix);
            
            sprite->setName(string(sprName));
            
            
            // -----------------------------------------------------------
            // only 5 sprites is target, any other will not be touched.
            bool bingo = false;
            for(int t = 0; t < TARGET_OBJECTS_NUMBER; ++t) {
                if (ix == targetIndex[t]) {
                    bingo = true;
                    break;
                }
            }
            
            //生成目标
            if (bingo) {
                
                // add to target array
                mTargetObjects.pushBack(sprite);
                
                // set sprite invisible random for making differences between maps
                if ((STUtility::isOddNumber(loop) && !STUtility::isOddNumber((int)ix))
                    || (!STUtility::isOddNumber(loop) && STUtility::isOddNumber((int)ix))) {
                    
                    sprite->setVisible(false);
                }
            }
            
        }
        
        --loop; // build another map
    }
    
    return true;
}

void SpotScene:: onEnter()
{
    Layer::onEnter();
    
    // solve the problem when pop up shop or setting scene game resume automatically
    if (_mIsGamePause)
    {
        _mIsGamePause = false;
        this->pauseGame();
    }
    
    
    if (willRefreshRectAd) {
        willRefreshRectAd = false;
    }
}

void SpotScene:: onExit()
{
    Layer::onExit();
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

//找到目标了
void SpotScene:: spotDifference(Sprite* target, const cocos2d::Point& point)
{
    AudioHelper::getInstance()->playSound(sound_tap_right);
    
    // get world coordinate
    Point center = target->convertToWorldSpace(point);
    // get mirror image coordinate
    Point mirrorCenter = this->getMirrorSymmetryPoint(center);
    
    // draw a indicator
    auto circle = Sprite::createWithSpriteFrameName("ui08_right.png");
    circle->setPosition(center);
    mpIndicatorLayer->addChild(circle);
    circle->setScale(UI_SCALE);
    
    // draw a indicator above the mirror image also
    auto symmetry = Sprite::createWithSpriteFrameName("ui08_right.png");
    symmetry->setPosition(mirrorCenter);
    mpIndicatorLayer->addChild(symmetry);
    symmetry->setScale(UI_SCALE);
    
    
    decltype(mTargetObjects.size()) remain = mTargetObjects.size();
    remain = remain >> 1; // remain / 2
    
    /* light the indication star */
    // 把星星(TARGET_OBJECTS_NUMBER - remain)点亮 (变成未选中状态, UI配反了)
    _vStars.at(TARGET_OBJECTS_NUMBER - remain)->setSelectedState(false);
    
//    auto starFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ui08_icon_get.png");
//    if (nullptr != starFrame) {
//        auto star = mUIStars.at(TARGET_OBJECTS_NUMBER - remain);
//        star->setSpriteFrame(starFrame);
//    } else {
//        CCLOG("get >>>>> ui08_icon_get.png <<<<< failed!");
//    }
    
    /* because the objects stored symmetrical in the array, we can delete them in pair. */
    decltype(remain) targetIndex = mTargetObjects.getIndex(target);
    decltype(remain) mirrorIndex = 0;
    
    
    if (targetIndex >= remain)
        mirrorIndex = targetIndex - remain;
    else
        mirrorIndex = targetIndex + remain;
    
    // get mirror image at symmetrical position
    auto mirror = mTargetObjects.at(mirrorIndex);
    
    // remove touch listener to avoid retouch
    _eventDispatcher->removeEventListenersForTarget(target);
    mTargetObjects.eraseObject(target);
    
    // also remove mirror image
    _eventDispatcher->removeEventListenersForTarget(mirror);
    mTargetObjects.eraseObject(mirror);
    
    
    // basic score
    _mGameScore += 1000;
    
    
//    // only attack mode not have combo bonus
//    if (_mLevelInfo->m_GameMode == AppGlobal::GameMode::TIME_ATTACK) {
//        
//        int _remaining_ = _mRemainingTime.asInt();
//        
//        // only took no more than 5 seconds will get skill bonus
//        if ( _remaining_ >= (_mGameMaxTime - 5) ) {
//            _mGameScore += 500;
//        }
//        
//        // time bonus   100 * (t-7) (t>=8)
//        if ( _remaining_ > 7 ) {
//            _mGameScore += ( 100 * _remaining_ );
//        }
//        
//    } else
    {
        // combo bonus
        
        if (_mIsComboStatus) {
            
            AudioHelper::getInstance()->playSound(sound_game_combo);
            
            // combo image
            Sprite *combo = nullptr;
            
            switch (++_mComboNum)
            {
                case 1:
                {
                    combo = Sprite::createWithSpriteFrameName("ui08_combox2.png");
                    break;
                }
                    
                case 2:
                {
                    combo = Sprite::createWithSpriteFrameName("ui08_combox3.png");
                    break;
                }
                    
                case 3:
                {
                    combo = Sprite::createWithSpriteFrameName("ui08_combox4.png");
                    break;
                }
                    
                case 4:
                {
                    combo = Sprite::createWithSpriteFrameName("ui08_combox5.png");
                    break;
                }
            }
            
            combo->setPosition(center);
            mpIndicatorLayer->addChild(combo);
            combo->setScale(UI_SCALE);
            
            Action *action = Sequence::create(DelayTime::create(AppGlobal::ANIMATION_DURING),
                                              FadeOut::create(AppGlobal::ANIMATION_DURING),
                                              CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, combo)),
                                              nullptr);
            combo->runAction(action);
            
            // update the score label
            settleCombo();
            
        } else {
            // start combo status
            _mIsComboStatus = true;
            this->scheduleOnce(COMBO_SCHEDULE, 5.f);
        }
    }
    
    // check if accomplished the game.
    
#if DEBUG_GAME
    if (true) {
#else
    if (remain < 2) {
#endif
        _mIsGameFailed = false;
        scheduleOnce(schedule_selector(SpotScene::finishLevel), 0.8);
        
    } else if (_mLevelInfo->m_GameMode == AppGlobal::GameMode::TIME_ATTACK) {
        
        AudioHelper::getInstance()->stopSound(sound_game_time_cd_id);
        
        // when we found a difference we will reset time in attack mode
        initGameStatus();
        
        // reset progress bar
        mpProgressTimer->setPercentage(100.f);
        mpProgressTimer->stopActionByTag(tag_ProgressToAction);
        auto action = ProgressTo::create(_mRemainingTime.asFloat(), 0);
        action->setTag(tag_ProgressToAction);
        mpProgressTimer->runAction(action);
        // we pause action when during the freeze time
        if (_mIsGameTimeFroze) {
            _pauseGameTime();
        }
    }
    
    
}

Point SpotScene::getMirrorSymmetryPoint(const Point& original)
{
    float x;
    
    Vec2 half = V::getRealPositionForDesignPosition(Vec2(960/2, 0));
    
    if (original.x < half.x) {
        x = original.x + (mRightBlockCenter.x - mLeftBlockCenter.x);
    }
    else
    {
        x = original.x - (mRightBlockCenter.x - mLeftBlockCenter.x);
    }
    
    return Point(x, original.y);
}

//完成了.
void SpotScene::finishLevel(float dt)
{
    //AudioHelper::getInstance()->pauseSound(sound_game_time_cd_id);
    
    // combo score accounting.
    if (_mIsComboStatus) {
        // cancel the schedule
        this->unschedule(COMBO_SCHEDULE);
        this->onComboCallback(0);
    }

    if (_mIsGameFailed)
    {
        
    } else {
        // time bonus accounting
        // it's not nessary for losing game.
        this->settleTime();
        
        
        // delay to pop up finish layer for displaying the last object
        string time = Util::formatTime(_times);
        
        int starCount;
        if (_times < 100) {
            starCount = 3;
        }
        else if (_times < 180) {
            starCount = 2;
        }
        else {
            starCount = 1;
        }
        
        xDialog->showOver(time, starCount, CC_CALLBACK_1(SpotScene::onDialog, this));
        
        xBalloon->start();
    }
}


bool SpotScene:: pauseGame()
{
    if (_mIsGamePause)
        return false;
    
    _mIsGamePause = true;
    
    // pause alarm if necessary
    AudioHelper::getInstance()->pauseSound(sound_game_time_cd_id);
    
//    if(! _scheduler->isTargetPaused(this))
//        _scheduler->pauseTarget(this);
    
    _pauseGameTime();
    
    return true;
}

bool SpotScene:: resumeGame()
{
    if (!_mIsGamePause)
        return false;
    
    // resume alarm if necessary
    AudioHelper::getInstance()->resumeSound(sound_game_time_cd_id);
    
//    if(_scheduler->isTargetPaused(this))
//        _scheduler->resumeTarget(this);
    
    _resumeGameTime();
    
    _mIsGamePause = false;
    
    return true;
}

void SpotScene:: _pauseGameTime()
{
    if (mpProgressTimer)
        mpProgressTimer->pause();
}

void SpotScene:: _resumeGameTime()
{
    // we will not resume the action until freeze time end
    if (mpProgressTimer && !_mIsGameTimeFroze)
        mpProgressTimer->resume();
}

void SpotScene:: resetLevel()
{
    _left->removeAllChildren();
    _right->removeAllChildren();
    mpIndicatorLayer->removeAllChildren();
    mTargetObjects.clear();
    
    // reset time label
    _times = 0;
    
    // reset scores
    _mGameScore = 0;
    
    // reset map
    buildMap();
    // reset ui
    buildUI();
    
    _mIsGameFailed = false;
    
    AudioHelper::getInstance()->stopSound(sound_game_time_cd_id);
    
    //
    resumeGame();
    
    for (auto star : _vStars) {
        star->setSelectedState(true);
    }
}

void SpotScene:: onPauseCallback(Ref* pSender)
{
    if( this->pauseGame() )
    {
        if (nullptr != pSender) {
            AudioHelper::getInstance()->playSound(sound_button);
        }
    }
}

void SpotScene:: onResumeCallback(Ref*)
{
    if ( this->resumeGame() ) {
        
        AudioHelper::getInstance()->playSound(sound_button);
        
        auto cover = dynamic_cast<CoverLayer *>( this->getChildByName(NAME_COVER_LAYER) );
        if (cover) {
            cover->closeCoverLayer(this);
        }
    }
}

void SpotScene:: onFreezeCallback(Ref* ref)
{
    // not allow use freeze props during freeze time.
    if (_mIsGamePause || _mIsGameTimeFroze)
        return;
    
    if (_mFreezeNumber.asInt() > 0) {
        
        AudioHelper::getInstance()->playSound(sound_button);
        
        _mIsGameTimeFroze = true;
        
        _mFreezeTime = FREEZE_TIME;
        
        _pauseGameTime();
        
#if !(DEBUG_PROPS)
       
#endif
        
    } else {
        
        this->goToShop(ref);
    }
    
    
}

void SpotScene:: onAddTimeCallback(Ref* ref)
{
    if (_mIsGamePause || (_mGameMaxTime == _mRemainingTime.asInt()) )
        return;
    
    if (_mAddTimeNumber.asInt() > 0) {
        
        // stop alarm
        AudioHelper::getInstance()->stopSound(sound_game_time_cd_id);
        
        AudioHelper::getInstance()->playSound(sound_button);
        
        int time = MIN( _mGameMaxTime, (_mRemainingTime.asInt() + 10) );
        
        _mRemainingTime = Value(time);
        
        // reset progress bar
        float percentage = float(time) / float(_mGameMaxTime) * 100.f;
        mpProgressTimer->setPercentage(percentage);
        mpProgressTimer->stopActionByTag(tag_ProgressToAction);
        auto action = ProgressTo::create(_mRemainingTime.asFloat(), 0);
        action->setTag(tag_ProgressToAction);
        mpProgressTimer->runAction(action);
        
        // we pause action when during the freeze time
        if (_mIsGameTimeFroze) {
            _pauseGameTime();
        }
        
        
#if !(DEBUG_PROPS)
        
#endif
        
    } else {
        
        this->goToShop(ref);
        
    }
}


void SpotScene:: onRefreshCallback(Ref*)
{
//    auto dialog = DialogBox::createDialog("Are you sure you want to restart?",
//                                          CC_CALLBACK_1(SpotScene::onRefreshCloseCallback, this));
//    this->addChild(dialog, INT_MAX);
    
    AudioHelper::getInstance()->playSound(sound_button);
    
}

void SpotScene:: onRefreshCloseCallback(Node *dialog)
{
//    auto dlg = dynamic_cast<DialogBox *>(dialog);
//    switch (dlg->getButtonType())
//    {
//        case DialogBox::Button::BUTTON_YES:
//            AdHelper::removeRectAds();
//            this->resetLevel();
//            
//            break;
//            
//        case DialogBox::Button::BUTTON_NO:
//            break;
//            
//        default:
//            break;
//    }
    
}

void SpotScene:: onLevelsCallback(Ref*)
{
}

void SpotScene:: onSettingCallback(Ref*)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    willRefreshRectAd = true;
    
//    auto setting = SettingScene::createScene();
//    Director::getInstance()->pushScene(setting);
}

void SpotScene:: onNextCallback(Ref *)
{
    AudioHelper::getInstance()->playSound(sound_button);
}

void SpotScene:: onModeCallback(cocos2d::Ref* ref)
{
    AudioHelper::getInstance()->playSound(sound_button);
    
    // refresh current level, not enter the next level
    auto btn = dynamic_cast<Node *>(ref);
    std::string btnName = btn->getName();
    if (btnName == "time")
    {
        _mLevelInfo->m_GameMode = AppGlobal::GameMode::TIME;
    }
    else if (btnName == "attack")
    {
        if (btn->getTag() == 1)
        {
//            auto dialog = DialogBox::createAlert("Finish time mode to unlock Time Attack.");
//            this->addChild(dialog, INT_MAX);
            return;
        }
        else
        {
            _mLevelInfo->m_GameMode = AppGlobal::GameMode::TIME_ATTACK;
        }
    }
    else if (btnName == "zen")
    {
        if (btn->getTag() == 1)
        {
//            auto dialog = DialogBox::createAlert("Finish 2 modes to unlock Zen Mode.");
//            this->addChild(dialog, INT_MAX);
            return;
        }
        else
        {
            _mLevelInfo->m_GameMode = AppGlobal::GameMode::ZEN;
        }
    }
    
    resetLevel();
}

//计时器, 单位1秒
void SpotScene:: scheduleCallback(float delta)
{
    // used the freeze props
    if (_mIsGameTimeFroze) {
        
        if ( (--_mFreezeTime) == 0) {
            
            _mIsGameTimeFroze = false;
            _resumeGameTime();
        }
        
        return;
    }
    
    // get current game remaining time
    int remain = _mRemainingTime.asInt();
    
    
    if (_mLevelInfo->m_GameMode == AppGlobal::GameMode::ZEN) {
        
        // not concerned about the remaining time when over 60 seconds in zen mode
        if (remain > 0) {
            _mRemainingTime = Value(--remain);
        }
        
    } else {
        
        // check the remaining time
        if (remain == 0) {
            // game over
            _mIsGameFailed = true;
            scheduleOnce(schedule_selector(SpotScene::finishLevel), 0.8);
            
            return;
            
        } else if(remain == 6) {
            // alert user
        }
        
        _mRemainingTime = Value(--remain);
    }
    
    //
    _mSpentTime++;
}


void SpotScene:: touchWrong(const Point& pot)
{
    AudioHelper::getInstance()->playSound(sound_tap_error);
    
    auto wrong = Sprite::createWithSpriteFrameName("ui08_wrong.png");
    wrong->setPosition(pot);
    wrong->setScale(UI_SCALE);
    mpIndicatorLayer->addChild(wrong);
    
    Action *action = Sequence::create(DelayTime::create(AppGlobal::ANIMATION_DURING),
                                      FadeOut::create(AppGlobal::ANIMATION_DURING),
                                      CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, wrong)),
                                      nullptr);
    wrong->runAction(action);
    
    
    // combo score accounting.
    if (_mIsComboStatus) {
        // cancel the schedule
        this->unschedule(COMBO_SCHEDULE);
        this->onComboCallback(0);
    }
}

void SpotScene:: onComboCallback(float delta)
{
    // score accounting
    this->settleCombo();
    
    // reset value
    _mIsComboStatus = false;
    _mComboNum = 0;
}


void SpotScene:: initGameStatus()
{
    switch (_mLevelInfo->m_GameMode)
    {
        case AppGlobal::GameMode::TIME:
        {
            _mGameMaxTime = 90;
            _mRemainingTime = Value(90);
            break;
        }
            
        case AppGlobal::GameMode::TIME_ATTACK:
        {
            _mGameMaxTime = 15;
            _mRemainingTime = Value(_mGameMaxTime);
            break;
        }
            
        case AppGlobal::GameMode::ZEN:
        {
            _mGameMaxTime = 60;
            _mRemainingTime = Value(60);
            break;
        }
    }
}

void SpotScene:: settleCombo()
{
    // only time mode have combo scores
    if (_mLevelInfo->m_GameMode != AppGlobal::GameMode::TIME)
        return;
    
    _mGameScore += ( 1000 * (_mComboNum + 1) );
}

void SpotScene:: settleTime()
{
    switch (_mLevelInfo->m_GameMode)
    {
        case AppGlobal::GameMode::TIME:
        {
            // 100 x (t-20) (t>20)
            const int _remaining = _mRemainingTime.asInt() - 20;
            if (_remaining > 0) {
                _mGameScore += ( 100 * _remaining );
            }
            
            break;
        }
            
        case AppGlobal::GameMode::TIME_ATTACK:
        {
            // when we got a difference, we sttle time bonus already
            break;
        }
            
        case AppGlobal::GameMode::ZEN:
        {
            if (_mRemainingTime.asInt() > 0) {
                _mGameScore += 7500;
            }
            
            break;
        }
    }
}

void SpotScene:: goToShop(cocos2d::Ref* ref)
{
//    // pause game
//    this->onPauseCallback(nullptr);
//    
//    // go to shop scene
//    auto shopLayer = ShopScene::create(this);
//    if (!shopLayer)
//        return;
//    
//    // called from next level
//    if (nullptr == ref) {
//        shopLayer->setCloseCallback(CC_CALLBACK_0(SpotScene::onShopClosedCallback, this));
//    }
//    
//    auto shop = Scene::create();
//    shop->addChild(shopLayer);
//    
//    Director::getInstance()->pushScene(shop);
    
}

void SpotScene:: onShopClosedCallback()
{
    // already purchased all levels, just go to next level
    if (PurchaseHelper::isPurchased)
    {

    }
    else
    {
        // the next level is locked, and user not buy it, return to levels scene
        this->onLevelsCallback(nullptr);
    }
}


void SpotScene:: purchaseSuccessful(const char* pid)
{
    updatePropsNum(pid);
}

void SpotScene:: purchaseFailed(const char* pid, int errorCode)
{
    
}

void SpotScene:: restoreSuccessful(const char* pid)
{
    updatePropsNum(pid);
}

void SpotScene:: restoreFailed(const char* pid, int errorCode)
{
    
}

void SpotScene:: updatePropsNum(const std::string &sku)
{
}





