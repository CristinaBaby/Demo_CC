//
//  RecordScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#include "RecordScene.h"
#include "HomeScene.h"
#include "../AppGlobal.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/UICpp.h"
#include "../modules/STSystemFunction.h"
#include "../modules/AnalyticX.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
#include "TransitionFadeEx.h"
#include "../Jigsaw/JPDifficultyScene.h"
#include "../Jigsaw/JPDifficultyScene.h"
#include "../Spot/SpotScene.h"
#include "../utilities/STUtility.h"
#include "../modules/AudioRecorder.h"
#include "../widgets/Dialog.h"
#include "../gameconfig/MyGameConfig.h"
#include "../gameconfig/BalloonManager.h"


#define EXIT_DIALOG_TAG 700
#define IMAGE_COUNT     (30)
#define FISH_COUNT      (1)

#define OFFSET          (350)


Scene* RecordScene::scene()
{
    auto scene = Scene::create();
    auto layer = RecordScene::create(false);
    scene->addChild(layer);
    return scene;
}

TransitionFadeEx* RecordScene::createScene()
{
    auto scene = Scene::create();
    auto fadeScene = TransitionFadeEx::create(3, scene, Color3B::WHITE);
    
    auto layer = RecordScene::create(true);
    
    scene->addChild(layer);
    
    return fadeScene;
}

RecordScene* RecordScene::create(bool isForward)
{ 
    RecordScene *pRet = new RecordScene(); 
    if (pRet && pRet->init(isForward))
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}

bool RecordScene::init(bool isForward)
{
    if (Layer::init()) {
        
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui02_record.csb"));
        addChild(_root);
        
        quickButton("ui02_jigsaw", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        quickButton("ui02_spot", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        quickButton("ui02_paint", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        
        _btnRepeat = quickButton("ui02_repeat", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        _btnStop = quickButton("ui02_stop", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        
        quickButton("ui02_previous", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        quickButton("ui02_next", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        quickButton("ui02_play", _root, CC_CALLBACK_2(RecordScene::onButton, this));                           //如果没有录音, 弹出tips, 如果有录音, 播放
        _btnHome = quickButton("ui02_home", _root, CC_CALLBACK_2(RecordScene::onButton, this));                //回home界面
        _chkSound = quickCheckBox("ui02_sound", _root, CC_CALLBACK_2(RecordScene::onCheckBoxClicked, this));          //静音
        _btnRecord = quickButton("ui02_record", _root, CC_CALLBACK_2(RecordScene::onButton, this));            //录音
        _btnClose = quickButton("ui07_close", _root, CC_CALLBACK_2(RecordScene::onButton, this));
        
        _popupRecord = (Layout*)Helper::seekWidgetByName(_root, "ui02_prompt_abs");
        _popupRecord->setVisible(false);
        
        _textTime = (Text*)Helper::seekWidgetByName(_root, "ui02_popup_label_01");
        _textTime->setFontName("Times BoldItalic.ttf");
        
        quickSetFontName("ui02_prompt_label_01", _root, "Tartine Script Black.ttf");
        
        //先隐藏3个按钮
        _btnHome->setVisible(false);
        _chkSound->setVisible(false);
        _btnRecord->setVisible(false);
        
        _popup = (Layout*)Helper::seekWidgetByName(_root, "popup_1");
        _popup->setVisible(false);
        _popup->setContentSize(V::getRealDesignSize());
        
        _layoutUI02 = (Layout*)Helper::seekWidgetByName(_root, "ui02_abs");
        quickSetVisible("ui02_picture", _root, false);
        
        _pictureLabel = (Text*)Helper::seekWidgetByName(_root, "ui02_picture_label");
        _pictureLabel->setFontName("Tartine Script Black.ttf");
        
        _btnMenu = quickButton("ui02_btn_menu", _root, CC_CALLBACK_2(RecordScene::onButton, this));                       //菜单
        
        _isForward = isForward;
        if(!isForward)
        {
            scheduleOnce(schedule_selector(RecordScene::initMenu), 0);
        }
        
        _pictureIndex = xGame->getPictureIndex();
        getCurrentImageName();
        
        //////////////////////////////////////////////////////////// 加入图片 ////////////////////////////////////////////////////////////
        
        //图片内容
        Vec2 posClipper = _layoutUI02->getContentSize() / 2;
        posClipper.add(Vec2(16, 9));
        
        _underSprite = Sprite::create(getCurrentPageName());
        _underSprite->setPosition(posClipper);
        
        float imageScale = (_layoutUI02->getContentSize().height  / _underSprite->getContentSize().height) * 1.036;
        _underSprite->setScale(imageScale);
        
        _layoutUI02->addChild(_underSprite, -1);
        
        setStoryText();
        
        //音量设置
        if (!(xAudio->getMusicVolume() > 0 || xAudio->getSoundVolume() > 0))
        {
            _chkSound->setSelectedState(true);
        }
        
        _direction = (Direction)namespaceST::STUtility::getRandom(8);
        
        AudioHelper::getInstance()->playMusic("bg_story.mp3");
        
        return true;
    }
    
    return false;
}


void RecordScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    if (_isForward) {
        initMenu(0);
    }
    else
    {
        ArmatureDataManager::getInstance()->removeArmatureFileInfo("fish1.ExportJson");
        ArmatureDataManager::getInstance()->addArmatureFileInfo("fish1.ExportJson");
        
        ArmatureDataManager::getInstance()->removeArmatureFileInfo("062514.csb");
        ArmatureDataManager::getInstance()->addArmatureFileInfo("062514.csb");
    }
    
    //加载广告
    SceneTag tag = xScene->getLastSceneTag();
    
    if (tag == S_PREFACE) {
        //MX: request full screen ad, if is not first launch app
        if (!UserDefault::getInstance()->getBoolForKey(key_first_play_game, true))
        {
            AdHelper::requestInterstitialAds();
        }
        else
        {
            UserDefault::getInstance()->setBoolForKey(key_first_play_game, false);
        }
    }
    else if (tag == S_PUZZLE || tag == S_SPOT || tag == S_COLOR_BOOK)
    {
        AdHelper::requestInterstitialAds();
    }
}


void RecordScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    AudioHelper::getInstance()->playSound(sound_button);
    
    auto widget = (Widget*)sender;
    
    const string& name = widget->getName();
    
    if (name.compare("ui02_previous") == 0 ||
        name.compare("ui02_next") == 0) {
        
        xAudio->playSound("sfx_btn_play.mp3");
        
        _moveCount++;
        _rateCount++;
        
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
        if (_moveCount >= 5) {
            _moveCount = 0;
            AdHelper::requestInterstitialAds();
        }
        if (_rateCount == 3) {
            STSystemFunction sf;
            sf.rating();
        }
#endif
    }
    else if (name.compare("ui02_record") == 0 ||
             name.compare("ui02_stop") == 0 ||
             name.compare("ui02_repeat") == 0 ) {
        
        xAudio->playSound("sfx_story_recordstart.mp3");
    }
    else {
        xAudio->playDefaultSound();
    }
    
    if (name.compare("ui02_previous") == 0) {
        stopMenuActions();
        switchPicture(false);
    }
    else if (name.compare("ui02_next") == 0)
    {
        stopMenuActions();
        switchPicture(true);
    }
    else if (name.compare("ui02_jigsaw") == 0)
    {
        xScene->forward(S_PUZZLE_DIFF);
    }
    else if (name.compare("ui02_spot") == 0)
    {
        xScene->forward(S_SPOT);
    }
    else if (name.compare("ui02_paint") == 0)
    {
        xScene->forward(S_COLOR_BOOK);
    }
    else if (name.compare("ui02_btn_menu") == 0)
    {
        if(_vecMenus.empty() || _block)
        {
            return;
        }
        
        if(_menuState != M_MOVE)
        {
            if (_menuState == M_SHOW) {
                if (_guideMode == Guide::RECORD_WAIT) {
                    _btnRecord->stopAllActions();
                    _btnRecord->setScale(1);
                    _guideMode = Guide::NONE;
                }
                
                hideMenu();
            }
            else
            {
                showMenu();
                
                if (_guideMode == Guide::MENU_WAIT) {
                    _btnMenu->stopAllActions();
                    _btnMenu->setScale(1);
                    
                    auto sequence = Sequence::create(ScaleTo::create(0.5, 1.5), ScaleTo::create(0.5, 1), NULL);
                    _btnRecord->runAction(RepeatForever::create(sequence));
                    
                    _guideMode = Guide::RECORD_WAIT;
                }
            }
        }
    }
    else if (name.compare("ui02_home") == 0)
    {
        AudioHelper::getInstance()->playMusic("bg_start.mp3");
        xScene->back(S_HOME);
    }
    else if (name.compare("ui02_record") == 0)
    {
        disableAllButtons();
        
        _btnRecord->stopAllActions();
        _btnRecord->setScale(1);
        
        _popup->setVisible(true);
        xAudio->stopMusic();
        
        _time = 2 * 60;
        quickSetString("ui02_popup_label_01", _root, Util::formatTime(_time));
        
        startRecord(0);
    }
    else if (name.compare("ui02_stop") == 0)
    {
        unschedule(schedule_selector(RecordScene::updateRecord));
        xRecorder->stop();
        _isFinishStop = false;
        scheduleOnce(schedule_selector(RecordScene::onFinishStop), 1.5);
        disableAllButtons();
        schedule(schedule_selector(RecordScene::checkStopFinish), 0.5);
    }
    else if (name.compare("ui02_repeat") == 0)
    {
        //android 机器stop后必须等待一段时间才能start (它用线程写的)
        xRecorder->stop();
        _isFinishStop = false;
        scheduleOnce(schedule_selector(RecordScene::onFinishStop), 1.5);
        disableAllButtons();
        startRecord(0);
    }
    else if (name.compare("ui07_close") == 0)
    {
        unschedule(schedule_selector(RecordScene::updateRecord));
        xRecorder->stop();
        _isFinishStop = false;
        scheduleOnce(schedule_selector(RecordScene::onFinishStop), 1.5);
        xAudio->playMusic("bg_story.mp3");
        _popup->setVisible(false);
    }
    else if (name.compare("ui02_play") == 0)
    {
//        xAudio->stopMusic();
        //先检查是否存在某个文件.
        //xRecorder->play(getCurrentImageName());
        
        if(xRecorder->isFileExist(getCurrentRecordName()))
        {
            xAudio->stopSound(_effectid);
            _effectid = xRecorder->play(getCurrentRecordName());
        }
        else
        {
            _popupRecord->setVisible(true);
            scheduleOnce(schedule_selector(RecordScene::disappear), 3);

            //激活, 引导陌生
            auto sequence = Sequence::create(ScaleTo::create(0.5, 1.5), ScaleTo::create(0.5, 1), NULL);
            if (abs(_btnHome->getPositionX() - _vecPos.at(0).x) < FLT_EPSILON) {
                _guideMode = Guide::RECORD_WAIT;
                _btnRecord->runAction(RepeatForever::create(sequence));
            }
            else
            {
                _guideMode = Guide::MENU_WAIT;
                _btnMenu->runAction(RepeatForever::create(sequence));
            }
        }
    }
}

void RecordScene::startRecord(float dt)
{
    if (_isFinishStop) {
        
        if (_bBuffer == true) {
            _bBuffer = false;
            return;
        }
        
        unschedule(schedule_selector(RecordScene::startRecord));

        if (xRecorder->start(getCurrentRecordName()) == false) {
            xDialog->show("Please enable the microphone of the device before recording!");
            _popup->setVisible(false);
            xAudio->playMusic("bg_story.mp3");
        }
        else
        {
            xAudio->stopMusic();
            _bStartRecord = true;
            
            _time = 2 * 60;
            quickSetString("ui02_popup_label_01", _root, Util::formatTime(_time));
            
            STSystemFunction sf;
            sf.onFlurryEvent("record");
            schedule(schedule_selector(RecordScene::updateRecord), 1);
        }
        
        enableAllButtons(0);
    }
    else if (fabs(dt - 0) < FLT_EPSILON) {
        _bBuffer = true;
        schedule(schedule_selector(RecordScene::startRecord), 0.3);
    }
}

void RecordScene::checkStopFinish(float dt)
{
    if (xRecorder->isFinishStop()) {
        unschedule(schedule_selector(RecordScene::checkStopFinish));
        
        enableAllButtons(0);
        xAudio->playMusic("bg_story.mp3");
    }
}

void RecordScene::disableAllButtons()
{
    if (_isFinishStop == false)
    {
        _btnRepeat->setEnabled(false);
        _btnRepeat->setBright(false);
        _btnStop->setEnabled(false);
        _btnStop->setBright(false);
        _btnClose->setEnabled(false);
        _btnClose->setBright(false);
    }
}

void RecordScene::enableAllButtons(float dt)
{
    _btnRepeat->setEnabled(true);
    _btnRepeat->setBright(true);
    _btnStop->setEnabled(true);
    _btnStop->setBright(true);
    _btnClose->setEnabled(true);
    _btnClose->setBright(true);
}

void RecordScene::disappear(float dt)
{
    _popupRecord->setVisible(false);
}

void RecordScene:: onExit()
{
    Layer::onExit();
    
    unscheduleAllSelectors();
}

void RecordScene::initMenu(float dt)
{
    //_btnMenu->setPositionX(_btnMenu->getPositionX() + 100);
    _vecPos.push_back(_btnHome->getPosition());
    _vecPos.push_back(_chkSound->getPosition());
    _vecPos.push_back(_btnRecord->getPosition());
    
    _hidePos = _btnMenu->getWorldPosition();
    _hidePos = _btnHome->convertToNodeSpace(_hidePos);
    
    _btnHome->setPosition(_hidePos);
    _chkSound->setPosition(_hidePos);
    _btnRecord->setPosition(_hidePos);
    
    _btnHome->setVisible(true);
    _chkSound->setVisible(true);
    _btnRecord->setVisible(true);
    
    _vecMenus.pushBack(_btnHome);
    _vecMenus.pushBack(_chkSound);
    _vecMenus.pushBack(_btnRecord);
    
    _menuState = M_HIDE;
}

void RecordScene::hideMenu()
{
    for (int i = 0; i < 3; i++) {
        MoveTo * moveto = MoveTo::create(0.3, _hidePos);
        _vecMenus.at(i)->runAction(moveto);
    }
    
    //0.4 秒不能点击
    _block = true;
    scheduleOnce(schedule_selector(RecordScene::unblockMenu), 0.4);
    _menuState = M_HIDE;
    
}

void RecordScene::showMenu()
{
    for (int i = 0; i < 3; i++) {
        MoveTo * moveto = MoveTo::create(0.3, _vecPos.at(i));
        _vecMenus.at(i)->runAction(moveto);
    }
    
    _block = true;
    scheduleOnce(schedule_selector(RecordScene::unblockMenu), 0.4);
    _menuState = M_SHOW;
}

void RecordScene::stopMenuActions()
{
    if (_guideMode != Guide::NONE) {
        _guideMode = Guide::NONE;
        
        _btnMenu->stopAllActions();
        _btnMenu->setScale(1);
        
        _btnRecord->stopAllActions();
        _btnRecord->setScale(1);
    }
}

void RecordScene::unblockMenu(float dt)
{
    _block = false;
}

void RecordScene::onCheckBoxClicked(Ref* ,CheckBox::EventType type)
{
    auto audio = AudioHelper::getInstance();
    if (type == CheckBox::EventType::SELECTED) {
        audio->switchSound(false);
    }
    else
    {
        audio->switchSound(true);
        xAudio->playMusic("bg_story.mp3");
    }
}


void RecordScene::switchPicture(bool direction)
{
    if (_moveBlock) {
        return;
    }
    else
    {
        _moveBlock = true;
        quickSetVisible("ui02_previous", _root, false);
        quickSetVisible("ui02_next", _root, false);
    }
    
    xRecorder->stopPlay();
    
    _period = Period::MOVE_STENCIL;
    
    //再做一个这么大的ClippingNode, 覆盖到原ClippingNode上面, 然后再加个stencil
    
    //图片内容
    namespaceST::STUtility::cycleNum(IMAGE_COUNT, direction, &_pictureIndex);
    xGame->setPictureIndex(_pictureIndex);
    
    auto content = Sprite::create(getCurrentPageName());
    
    _childStencil = Sprite::create("fish_mask.png");
    _newClipper = ClippingNode::create(_childStencil);
    
    _newClipper->setAlphaThreshold(0.8f);
    _newClipper->setInverted(false);
    _newClipper->setAnchorPoint(Vec2(0.5, 0.5));
    _newClipper->addChild(content);
    //clipper->setPosition( Vec2(_parentClipper->getContentSize().width / 2, _parentClipper->getContentSize().height / 2) );
    
    // _parentClipper 这个的坐标要保持与 underContent 一致
    float imageScale = (_layoutUI02->getContentSize().height  / content->getContentSize().height) * 1.036;
    _newClipper->setScale(imageScale);
    
    _newClipper->setPosition(_underSprite->getPosition());

    _layoutUI02->addChild(_newClipper, 2);
    
    //判断方向
    float offset = 1024;    //为了和以前坐标兼容
    
    int directionCopy = (int)_direction;
    namespaceST::STUtility::cycleNum(8, true, &directionCopy);
    _direction = (Direction)directionCopy;
    
    
    Vec2 moveDistance = Vec2(0, 1*offset);
    
    Sequence * sequence;
    Vec2 childStencilOffset = Vec2(0,  -offset - 200);
    Vec2 distance = Vec2(0, offset);
    
    switch (_direction) {
        case Direction::UP:
            //其它方向都是在向上方向的基础上旋转的
            break;
        case Direction::DOWN:
            rotateWithAngle(moveDistance, 180);
            rotateWithAngle(childStencilOffset, 180);
            rotateWithAngle(distance, 180);
            
            _childStencil->setRotation(180);
            break;
        case Direction::LEFT:
            rotateWithAngle(moveDistance, 270);
            rotateWithAngle(childStencilOffset, 270);
            rotateWithAngle(distance, 270);
            
            _childStencil->setRotation(90);
            break;
        case Direction::RIGHT:
            rotateWithAngle(moveDistance, 90);
            rotateWithAngle(childStencilOffset, 90);
            rotateWithAngle(distance, 90);
            
            _childStencil->setRotation(270);
            break;
        case Direction::DOWN_LEFT:
            rotateWithAngle(moveDistance, 315);
            rotateWithAngle(childStencilOffset, 315);
            rotateWithAngle(distance, 315);
            
            _childStencil->setRotation(45); //这个是逆时针的.
            break;
        case Direction::UP_LEFT:
            rotateWithAngle(moveDistance, 225);
            rotateWithAngle(childStencilOffset, 225);
            rotateWithAngle(distance, 225);
            
            _childStencil->setRotation(135); //这个是逆时针的.
            break;
        case Direction::UP_RIGHT:
            rotateWithAngle(moveDistance, 135);
            rotateWithAngle(childStencilOffset, 135);
            rotateWithAngle(distance, 135);
            
            _childStencil->setRotation(225); //这个是逆时针的.
            break;
        case Direction::DOWN_RIGHT:
            rotateWithAngle(moveDistance, 45);
            rotateWithAngle(childStencilOffset, 45);
            rotateWithAngle(distance, 45);
            
            _childStencil->setRotation(315); //这个是逆时针的.
            break;
        default:
            break;
    }
    
    _childStencil->setPosition(_childStencil->getPosition() + childStencilOffset);
    _endPos = _childStencil->getPosition() - distance;
    sequence = Sequence::create(MoveBy::create(2, distance), CallFunc::create(CC_CALLBACK_0(RecordScene::moveFinish, this)), NULL);
    
    //直接开始move fish
    _childStencil->runAction(sequence);
    
    Vec2 pos = _childStencil->convertToWorldSpaceAR(Vec2::ZERO);
    pos = convertToNodeSpace(pos);
    
    log("%f %f : height %f", pos.x, pos.y + content->getContentSize().height - _childStencil->getContentSize().height/2, content->getContentSize().height);
    
    //scheduleUpdate();
    createFishes(pos, moveDistance);
}


void RecordScene::updateRecord(float delta)
{
    if (_btnRepeat->isEnabled() == true) {
        _time--;
        
        if (_bStartRecord == true) {
            _bStartRecord = false;
            _time = 2 * 60 - 1;
        }
        
        quickSetString("ui02_popup_label_01", _root, Util::formatTime(_time));
    }

    if (_time == 0) {
        unschedule(schedule_selector(RecordScene::updateRecord));
        xRecorder->stop();
        _isFinishStop = false;
        scheduleOnce(schedule_selector(RecordScene::onFinishStop), 1.5);
        xAudio->playMusic("bg_story.mp3");
        _popup->setVisible(false);
    }

}

void RecordScene::onFinishStop(float dt)
{
    _isFinishStop = true;
}

void RecordScene::moveFinish()
{
    setStoryText();
    
    unscheduleUpdate();
    
    //删除parentClipper, 直接用新图替换就图
    _layoutUI02->removeChild(_newClipper);
    
    _underSprite->setTexture(getCurrentPageName());
}

void RecordScene::moveContent()
{
    
}

//0 左, 90, 上, 180, 右, 270, 下
void RecordScene::createFishes(Vec2& pos, Vec2& moveDistance)
{
    if (_fishes == nullptr) {
        Armature *armature = Armature::create("fish1");
        auto ani = armature->getAnimation();
        
        
        ani->playWithIndex(0);
        armature->setScale(1);
        
        
        _fishes = armature;
    }
    else
    {
        _fishes->stopAllActions();
    }
    
    _fishes->setVisible(true);
    
    if (_fishes->getParent() == nullptr) {
        addChild(_fishes);
    }
    
    Vec2 center = V::getCenterOfScene();
    Size frame = V::getDesignSize();
    //_fishes->setPosition(Vec2(center.x - frame.width * 0.18, pos.y + 480));
//    log("%f %f", pos.x, center.x - frame.width * 0.18);
    
    Size size = V::getDesignSize();
    
    switch (_direction) {
        case Direction::UP:
            _fishes->setRotation(90);
            _fishes->setPosition(Vec2(pos.x + 350 * size.width / 1136 , pos.y + 440  * size.height / 640));
            break;
        case Direction::DOWN:
            _fishes->setRotation(270);
            _fishes->setPosition(Vec2(pos.x + 750 * size.width / 1136, pos.y + 220 * size.height / 640));
            break;
        case Direction::LEFT:
            _fishes->setRotation(180);
            _fishes->setPosition(Vec2(pos.x + 700 * size.width / 1136, pos.y + 500 * size.height / 640));
            break;
        case Direction::RIGHT:
            _fishes->setRotation(0);
            _fishes->setPosition(Vec2(pos.x + 470 * size.width / 1136, pos.y + 170 * size.height / 640));
            break;
        case Direction::DOWN_LEFT:
            _fishes->setRotation(135);    //这个也是逆时针
            _fishes->setPosition(Vec2(pos.x + 550 * size.width / 1136, pos.y + 500 * size.height / 640));
            break;
        case Direction::UP_LEFT:
            _fishes->setRotation(225);    //这个也是逆时针
            _fishes->setPosition(Vec2(pos.x + 790 * size.width / 1136, pos.y + 410 * size.height / 640));
            break;
        case Direction::UP_RIGHT:
            _fishes->setRotation(315);    //这个也是逆时针
            _fishes->setPosition(Vec2(pos.x + 600 * size.width / 1136, pos.y + 180 * size.height / 640));
            break;
        case Direction::DOWN_RIGHT:
            _fishes->setRotation(45);    //这个也是逆时针
            _fishes->setPosition(Vec2(pos.x + 400 * size.width / 1136, pos.y + 290 * size.height / 640));
            break;
        default:
            break;
    }

    auto sequence = Sequence::create(MoveBy::create(4, 2 * moveDistance), CallFunc::create(CC_CALLBACK_0(RecordScene::unblockMove, this)), NULL);
    _fishes->runAction(sequence);
}

void RecordScene::unblockMove()
{
    _moveBlock = false;
    quickSetVisible("ui02_previous", _root, true);
    quickSetVisible("ui02_next", _root, true);
    
    setStoryText();
    
    _fishes->setVisible(false);
}

string RecordScene::getCurrentImageName()
{
    CCString * name = CCString::createWithFormat("L%.2d", _pictureIndex + 1);
    string str = name->getCString();
    
    xGame->setFileName(str);
    
    return std::move(str);
}


string RecordScene::getCurrentPageName()
{
    CCString * name = CCString::createWithFormat("P%.2d.png", _pictureIndex + 1);
    string str = name->getCString();
    
    CCString * name1 = CCString::createWithFormat("L%.2d", _pictureIndex + 1);
    string str1 = name1->getCString();
    
    xGame->setFileName(str1);
    
    return std::move(str);
}

string RecordScene::getCurrentRecordName()
{
    CCString * name = CCString::createWithFormat("stmrecord%.2d", _pictureIndex + 1);
    string str = name->getCString();
    
    return std::move(str);
}

void RecordScene::rotateWithAngle(Vec2& pos, float fDegree)
{
    //角度转弧度
    float angle = M_PI * fDegree /180;
    pos.rotate(Vec2::ZERO, angle);
}


void RecordScene::setStoryText()
{
    switch (_pictureIndex + 1) {
       case 1:
            _pictureLabel->setString("Once a little boy was given a birthday present of a set of twenty-five toy soldiers. All the soldiers had shiny red and blue uniforms and gleaming tin muskets.");
            break;
        case 2:
            _pictureLabel->setString("However, one of the soldiers stood out from the rest because he had only one leg.");
            break;
        case 3:
            _pictureLabel->setString("In the house, there were many other toys including wax puppets and stuffed toys.");
            break;
        case 4:
            _pictureLabel->setString("It also lived a beautiful, dancing ballerina doll who stood at the entrance to a cardboard castle which had a small mirror for a lake in front of it.");
            break;
        case 5:
            _pictureLabel->setString("She balanced herself on one leg so well that the tin soldier thought that she had only one leg.");
            break;
        case 6:
            _pictureLabel->setString("She wore a white dress with a tinsel heart pinned on it in the centre. The tin soldier admired the ballerina from a distance and soon fell in love with her.");
            break;
        case 7:
            _pictureLabel->setString("\"But she is so pretty and she lives in a beautiful castle! She will not like me at all!\" thought the toy soldier and he felt very sad.");
            break;
        case 8:
            _pictureLabel->setString("When the moon rose and the city became dark and silent.");
            break;
        case 9:
            _pictureLabel->setString("The little boy would fall asleep and magic happened. All the toys went alive, they started to dance and sing together happily.");
            break;
        case 10:
            _pictureLabel->setString("The evil jack popped out and stood in the dark corner watching all of the toys. He dislike the noise produced by the merry making.");
            break;
        case 11:
            _pictureLabel->setString("The evil jack spotted the toy soldier staring at the ballerina. He warned the toy soldier, \"You better stay away from the ballerina! She is mine!\"");
            break;
        case 12:
            _pictureLabel->setString("However, the toy soldier loved the ballerina so much that he stood on the windowsill and watched her dance all night.");
            break;
        case 13:
            _pictureLabel->setString("One night when the window was open, the evil jack crept behind the toy soldier and pushed him out of  the window.");
            break;
        case 14:
            _pictureLabel->setString("After a while, the little boy who were passing by found him in the bushes.");
            break;
        case 15:
            _pictureLabel->setString("He floated him down a gutter in a paper boat. The toy soldier flowed away.");
            break;
        case 16:
            _pictureLabel->setString("He flowed into the drain nearby, he was heartbroken because he was afraid that he would never see the ballerina ever again.");
            break;
        case 17:
            _pictureLabel->setString("A huge rat ran out, saw the toy soldier and rushed at him. The toy soldier was brave, he held onto his musket and fought it off.");
            break;
        case 18:
            _pictureLabel->setString("The toy soldier floated away just in time and escaped.");
            break;
        case 19:
            _pictureLabel->setString("The drain led into a canal where the water swirled fiercely. Here the little paper boat was torn apart.");
            break;
        case 20:
            _pictureLabel->setString("A fish saw the toy soldier and tried to swallow him, the toy soldier did his best to fight against it but to no avail.");
            break;
        case 21:
            _pictureLabel->setString("He lay inside the fish for a long time. He thought that he would trapped inside the fish forever.");
            break;
        case 22:
            _pictureLabel->setString("However, the toy soldier missed the ballerina badly, and was worried that evil jack would do harm to her.");
            break;
        case 23:
            _pictureLabel->setString("One day, a fisherman caught the fish and sold it in the market. A lady who happened to be the little boy’s mother bought the fish. She cut the fish and found the toy soldier inside.");
            break;
        case 24:
            _pictureLabel->setString("Exclaiming in wonder, she called the little boy and gave him the toy soldier back.");
            break;
        case 25:
            _pictureLabel->setString("The toy soldier was very happy to be back in the house and the best of all, he could see the lovely ballerina dance again.");
            break;
        case 26:
            _pictureLabel->setString("But alas! The little boy did not like the toy soldier anymore and he threw the poor toy soldier into the fireplace.");
            break;
        case 27:
            _pictureLabel->setString("The poor toy soldier started to melt, he was not afraid of death as a soldier, but he was sad that he really could never see the ballerina dance ever again.");
            break;
        case 28:
            _pictureLabel->setString("Suddenly, a strong gush of wind blew the ballerina into the fire.");
            break;
        case 29:
            _pictureLabel->setString("The toy soldier and the ballerina melted in the fire.");
            break;
        case 30:
            _pictureLabel->setString("The next morning, when the maid cleaned the fireplace, she found a tiny red tin heart. It belonged to the toy soldier and the ballerina. They were finally together.");
            break;
        default:
            break;
    }

}
