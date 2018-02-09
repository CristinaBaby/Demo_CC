//
//  GameScene.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "GameScene.h"
#include "AppConfigs.h"
#include "UICpp.h"
#include "SaveLayer.h"
#include "CBPackManager.h"
#include "CBUtils.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"
#include "ScribbleUtil.h"
#include "PictureLayer.h"
#include "BrushManager.h"
#include "ChooseScene.h"
#include "CloudSprite.h"
#include "LoadingLayer.h"
#include "PensColorManager.h"
#include "CBAppManager.h"
#include "TalkManager.hpp"

#define OFFSET              (Vec2(0, 80))
#define OFFSET_WAVE         (Vec2(0, 15))
#define OFFSET_TRUE         333
#define OFFSET_FALSE        111
#define OFFSET_MOVE         85

GameScene* GameScene::create(int packIndex, int pictureIndex)
{
    GameScene *pRet = new GameScene();
    if (pRet && pRet->init(packIndex, pictureIndex))
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

bool GameScene::init(int packIndex, int pictureIndex){
    
    if(Scene::init())
    {
        setName("Game");
        
        //////////////////////////////////////////////////////////// 数据赋值 ////////////////////////////////////////////////////////////
        _packIndex = packIndex;
        _pictureIndex = pictureIndex;
        //////////////////////////////////////////////////////////// UI ////////////////////////////////////////////////////////////
        Size pictureSize(772, 690);
        auto layerWhite = LayerColor::create(Color4B::WHITE, pictureSize.width, Director::getInstance()->getWinSize().height);
        layerWhite->setPosition(Utils::getInstance()->getScreenCenter().x + (1136 - pictureSize.width)/2 - layerWhite->getContentSize().width/2,
                                Utils::getInstance()->getScreenCenter().y + (768 - pictureSize.height)/2 - layerWhite->getContentSize().height/2);
        addChild(layerWhite);

        
        _root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("mm.json");
        addChild(_root, 2);
        _root->cocos2d::Node::visit();
        
        _loadingLayer = LoadingLayer::create();
        addChild(_loadingLayer,2);
        
        //stencil
        _cliperLayer = Layer::create();

        auto stencil = Sprite::create("mm/stencil.png");
        auto cilper = ClippingNode::create();
        cilper->setStencil(stencil);
        cilper->setPosition(Vec2(Director::getInstance()->getWinSize().width/2 - 386, Director::getInstance()->getWinSize().height/2));
        _root->addChild(cilper,2);
        cilper->addChild(_cliperLayer,100);
        
        ArmatureDataManager::getInstance()->addArmatureFileInfo("MM.ExportJson");
        auto bg = (Layout*)(Helper::seekWidgetByName(_root, "center"));
        auto pos = Vec2(Director::getInstance()->getWinSize()/2);
        auto box2 = quickGetAnimation(1, "MM", pos + Vec2(-415, -330));
        bg->addChild(box2, 5);
        
        //snow
        auto snow_bg = (Layout*)(Helper::seekWidgetByName(_root, "bg"));
        auto snow = ParticleSystemQuad::create("mm/Snow.plist");
        snow->setPosition(Vec2(snow_bg->getContentSize().width/2 + 50, Director::getInstance()->getWinSize().height + 40));
        snow->setPosVar(Vec2(snow_bg->getContentSize().width/3, 0));
        snow_bg->addChild(snow);
        
        auto loadAction = Sequence::create(createLoadingAction(1), createLoadingAction(2), createLoadingAction(3), createLoadingAction(4), NULL);
        runAction(loadAction);
        
        xAds->removeAds(kTypeBannerAds);
        
        return true;
    }
    
    return false;
}

GameScene::~GameScene()
{
    xTalk->clear();
    
    Director::getInstance()->getTextureCache()->removeTextureForKey(_picturePath);
    Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureHistoryPath);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    CC_SAFE_DELETE(_pictureImage);
    CC_SAFE_DELETE(_noneMode);
    CC_SAFE_DELETE(_waitMode);
    CC_SAFE_DELETE(_paintMode);
    CC_SAFE_DELETE(_moveMode);
    CC_SAFE_DELETE(_zoomMode);
    CC_SAFE_RELEASE(_particle);
    
    xBrush->clear();
}

ActionInterval* GameScene::createLoadingAction(int num){
    auto sequence = Sequence::create(CallFunc::create([=](){
        onProgress(num);
    }), DelayTime::create(0.f),NULL);
    
    return sequence;
}

void GameScene::onProgress(int percent)
{
    switch (percent) {
        case 1: //////////////////////////////////////////////////////////// Scribble ////////////////////////////////////////////////////////////
        {
            setupScribble();
        }
            break;
        case 2: //////////////////////////////////////////////////////////// UI ////////////////////////////////////////////////////////////
        {
            _palette = (ImageView*)Helper::seekWidgetByName(_root, "palette");
            _center = (ImageView*)Helper::seekWidgetByName(_root, "center");
            _center->setLocalZOrder(2);
            
            _saveBtn = quickButton("save", _root, CC_CALLBACK_2(GameScene::onButton, this));
            _backBtn = quickButton("refresh", _root, CC_CALLBACK_2(GameScene::onButton, this)); // colorlist back
            quickLayout("back", _root, CC_CALLBACK_2(GameScene::onButton, this));  // scene back
            
            _backBtn->setVisible(false);
            auto pos = _saveBtn->getPosition();
            Button* refresh = Button::create("mm/refresh-1.png");
            refresh->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
            refresh->setName("refresh-1");
            Button* save = Button::create("mm/save-1.png");
            save->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
            save->setName("save-1");
            addChild(refresh, 1);
            addChild(save, 1);
            refresh->setPosition(pos);
            save->setPosition(pos);
            btnVector.push_back(refresh);
            btnVector.push_back(save);
            
            initAnimation();
            
        }
            break;
        case 3: //////////////////////////////////////////////////////////// 画笔 ////////////////////////////////////////////////////////////
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Pens0.plist");
            
            _pensList = (ListView*)Helper::seekWidgetByName(_root, "pens");
            _pensList->setClippingEnabled(false);
            
            _colorsList = (ListView*)_pensList->clone();
            _colorsList->setVisible(false);
            _colorsList->setPosition(_pensList->getPosition() - OFFSET);
            _pensList->getParent()->addChild(_colorsList);
            
            indent(_pensList, true);
            
            for (int i = 1; i <= 10; i++) {
                auto layout = Layout::create();
                layout->setName(std::to_string(i));
                layout->setTouchEnabled(true);
                layout->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
                _pensList->pushBackCustomItem(layout);
                
                auto brush = xBrush->getBrush(i);
                layout->setContentSize(brush.size);
                ArmatureDataManager::getInstance()->addArmatureFileInfo(brush.name + ".ExportJson");
                auto armature = quickGetAnimation(1, brush.name, Vec2(layout->getPosition() + brush.offset));
                layout->addChild(armature);
                _pensAmt.push_back((Armature*)layout->getChildren().at(0));
            }
            indent(_pensList, false);
            xBrush->init(_pictureScribble);
        }
            break;
        case 4: //////////////////////////////////////////////////////////// add multi-touch event ////////////////////////////////////////////////////////////
        {
            for (int i = 1; i <= 10; i++) {
                auto layout = _pensList->getItem(i);
                
                auto brush = xBrush->getBrush(i);
                auto armature = (Armature*)layout->getChildren().at(0);
                
                if(brush.color)
                {
                    auto changes = xBrush->getBrush(i).changes;
                    auto origin = xBrush->getOrigianlColor(i);
                    
                    for (auto change : changes) {
                        auto body = armature->getBone(change);
                        body->updateDisplayedColor(Color3B(origin));
                    }
                }
            }
            
            string name;
            int tag = quickRandom(0, 2);
            switch (tag) {
                case 0:
                    name = "particle/paint_flower.plist";
                    break;
                case 1:
                    name = "particle/paint_heart.plist";
                    break;
                case 2:
                    name = "particle/paint_star.plist";
                    break;
                default:
                    break;
            }
            
            _particle = ParticleSystemQuad::create(name);
            CC_SAFE_RETAIN(_particle);
            _particle->stopSystem();
            addChild(_particle, 3);
            
            _noneMode = new NoneMode(this);
            _waitMode = new WaitMode(this);
            _paintMode = new PaintMode(this);
            _moveMode = new MoveMode(this);
            _zoomMode = new ZoomMode(this);
            switchMode(BaseMode::Mode::NONE);
            
            auto listener = EventListenerTouchAllAtOnce::create();
            listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
            listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
            listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
            listener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
            
            _condition++;
            if (_condition == 2) {
                _loadingLayer->setVisibleFalse();
            }
            
            auto node = Node::create();
            node->setName("2");
            onButton(node, Widget::TouchEventType::ENDED);
            
            schedule(schedule_selector(GameScene::pushPensHandsOn));
        }
            break;
            
        default:
            break;
    }
}

void GameScene::setupScribble()
{
    //add colouring layer
    _colouringLayer = Layer::create();
    _colouringLayer->setName("colouringLayer");
    _colouringLayer->ignoreAnchorPointForPosition(false);
    _colouringLayer->setAnchorPoint(Vec2::ZERO);
    //whitLayer->setPosition(_colouringLayer->getContentSize()/2);
    addChild(_colouringLayer);
    
    _pictureHistoryPath = xPack->getPackHistoryPicturePath(_packIndex, _pictureIndex, true);
    
    //mask path
    PackInfo *pack = xPack->getPackInfo(_packIndex - 1);
    if (pack->isServerPack)
    {
        _picturePath = xPack->getPackPicturePath(_packIndex, _pictureIndex, true);
    }
    else
    {
        _picturePath = xPack->getPackPicturePath(_packIndex, _pictureIndex, false);
    }
    
    auto picture = Sprite::create(_picturePath);
    picture->setPosition(Utils::getInstance()->getScreenCenter());
    _pictureHeight = picture->getContentSize().height;
    
    //create picture scribble
    _offsetPicturePos = (Helper::seekWidgetByName(_root, "left_border"))->getWorldPosition();
    _winSize = Director::getInstance()->getWinSize();
    
    _pictureScribble = ScribbleLineFrameNode::create(Size(picture->getContentSize().width, picture->getContentSize().height));
    _pictureScribble->setName("pictureScribble");
    _pictureScribble->setDelegate(this);
    _pictureScribble->setDefaultColor(Color4F::WHITE);
    _pictureScribble->setAntiAliasingBrush(true);
    _pictureScribble->setPosition(Utils::getInstance()->getScreenCenter().x + (1136 - picture->getContentSize().width)/2,
                                  Utils::getInstance()->getScreenCenter().y + (768 - picture->getContentSize().height)/2);
    
    _pictureScribble->setLineFrame(Sprite::create(_picturePath));
    _colouringLayer->addChild(_pictureScribble, 1);
    
    auto woodUp = Sprite::create("mm/wood.png");
    woodUp->setPosition(_pictureScribble->getPosition() + Vec2(0, (_pictureScribble->getContentSize().height + woodUp->getContentSize().height)/2));
    _colouringLayer->addChild(woodUp, 1);
    
    auto woodDown = Sprite::create("mm/wood.png");
    woodDown->setScaleY(-1);
    woodDown->setPosition(_pictureScribble->getPosition() + Vec2(0, -(_pictureScribble->getContentSize().height + woodUp->getContentSize().height)/2));
    _colouringLayer->addChild(woodDown, 1);
    
    //create white layer
    auto whiteLayer = LayerColor::create(cocos2d::Color4B::WHITE, picture->getContentSize().width, Director::getInstance()->getWinSize().height);
    whiteLayer->setPosition(_pictureScribble->getPosition() - whiteLayer->getContentSize()/2);
    whiteLayer->setName("whiteLayer");
    _colouringLayer->addChild(whiteLayer);
    
    //check sandbox history
    bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
    if (isExsit)
    {
        Sprite *history = Sprite::create(_pictureHistoryPath);
        history->setPosition(_pictureScribble->getContentSize()/2);
        
        _pictureScribble->getCanvas()->begin();
        history->visit();
        _pictureScribble->getCanvas()->end();
    }
    
    _pictureScribble->setPattern(_picturePath, true, 30);
}

void GameScene::initAnimation()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Model01.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Model02.plist");
    
    auto zero = _center->convertToNodeSpace(Vec2::ZERO);
    Vec2 pos((zero.x + (_backBtn->getPosition().x))/2 - 10, _backBtn->getPosition().y + 135);
    _model = make_shared<ArmatureManager>();
    _model->setScale(0.65);
    _model->setPosition(pos);
    _model->setLocalZOrder(6);
    
    _model->add("ModelWait1.ExportJson", _center);
    _model->add("ModelWait3.ExportJson", _center);
    _model->add("Paint.ExportJson", _center, Vec2(14, -12));// 14  -12
    
    auto armature = _model->add("ModelWait2.ExportJson", _center, Vec2(-3, 45));// -3 45
    armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(GameScene::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    armature = _model->add("Talk.ExportJson", _center);
    armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(GameScene::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    armature = _model->add("Suggest.ExportJson", _center, Vec2(11, 0));
    armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(GameScene::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    
    _model->setVoiceStart(3);
    _model->switchArmature();
    
    auto armatures = _model->getArmatures();
    for (auto armature : armatures) {
        armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(GameScene::onMovementEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    
    auto layout =  Layout::create();
    layout->setName("suggest");
    layout->setContentSize(Size(220, 450));
    layout->setAnchorPoint(Vec2(0.5, 0.5));
    layout->setPosition(pos);
    _center->addChild(layout);
    
    layout->setTouchEnabled(true);
    layout->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
    
    _loadingLayer->setArmature(_model);
    
    auto box1 = quickGetAnimation(0, "MM", pos + Vec2(365, 30));
    _center->addChild(box1, 5);
}

void GameScene::onMovementEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
    {
        //如果是建议颜色, 就从115帧继续播放
        if (armature->getName() == "Suggest") {
            
            if (_suggestTime > 3) {
                _suggestTime = 0;
            }
            else
            {
                _suggestTime++;
                armature->getAnimation()->gotoAndPlay(115);
                return;
            }
        }
        
        _model->switchArmature();
    }
    else if (movementType == MovementEventType::START)
    {
        _onSuggest = false;        
    }
}

void GameScene::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
    if (evt == "sing_start") {
        xAudio->playSound("interactive/sound_humming.mp3");
        xAudio->playSound("sound_humming.mp3");
    }
    else if(evt == "talk")
    {
        TalkManager::getTalkManger()->playTalk();
    }
    else if (evt == "suggest_color")
    {
        auto colors = xBrush->getColors();
        auto pen_5 = bone;
        
        int suggestIndex = quickRandom(0, (int)colors.size() - 1);
        while (suggestIndex == _lastSuggestIndex) {
            suggestIndex = quickRandom(0, (int)colors.size() - 1);
        }
        
        _lastSuggestIndex = suggestIndex;
        _suggestColor = Color3B(colors.at(suggestIndex));
        pen_5->updateDisplayedColor(_suggestColor);
        _onSuggest = true;
        
        xAudio->playSound("interactive/sound_trycolor.mp3");
    }
}

void GameScene::switchAnimation(float dt)
{
    animation->stop();
    animation->playWithIndex(quickRandom(0, 5));
}

void GameScene::popupMenu(){
    auto action_1 = MoveBy::create(0.3f, Vec2(0, -OFFSET_MOVE));
    auto action_2 = MoveBy::create(0.3f, Vec2(0, -1.86*OFFSET_MOVE));
    if (!_touchSaveBtn)
    {
        _touchSaveBtn = true;
        btnVector.at(0)->runAction(action_1);
        btnVector.at(1)->runAction(action_2);        
    }
    else
    {
        _touchSaveBtn = false;
        action_1 = action_1->reverse();
        action_2 = action_2->reverse();
        btnVector.at(0)->runAction(action_1);
        btnVector.at(1)->runAction(action_2);        
    }    
}

void GameScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED || _blockAll) return;
    
    string name = ((Node*)pSender)->getName();
    
    xAudio->stopSound(_nSoundId);
    _nSoundId = 0;
    
    if (name == "refresh-1") {
        xAudio->playSound("sfx/sfx_reset.mp3");
    }
    else if (name == "save")
    {
        xAudio->playSound("sfx/sfx_setting_out.mp3");
    }
    else if (name == "save-1")
    {
        xAudio->playSound("sfx/sfx_pop.mp3");
    }
    else
    {
        xAudio->playDefaultSound();
    }
    
    if (name.compare("save") == 0) {
        popupMenu();
    }
    else if (name.compare("save-1") == 0) {
        generateCurrentPaintingPicture();

        _saveLayer = SaveLayer::create(_pictureImage, this);
        addChild(_saveLayer, 10);
    }
    else if (name.compare("back") == 0) {
        _blockAll = true;
        save();
        int count = UserDefault::getInstance()->getDoubleForKey("count");
        auto scene = ChooseScene::create(false, _packIndex, count);
        scene->beBack();
        Director::getInstance()->replaceScene(scene);
    }
    else if (name.compare("refresh") == 0) {
        resumePensList();
    }
    else if (name == "refresh-1") {
        _pictureScribble->resetCanvas();
    }
    else if (name == "suggest") {
        if (_onSuggest) {
            if(xBrush->useSuggestColor(_suggestColor))
            {
                auto node = Node::create();
                node->setName(std::to_string(_lastSuggestIndex));
                onColorPen(node, Widget::TouchEventType::ENDED);
                
                xBrush->setChooseIndex(_lastSuggestIndex);
            }
        }
    }
    else if (_pensEnabled == true)
    {
        int index = std::stoi(((Node*)pSender)->getName());
        
        xBrush->useBrush(index);
        
        //缩进去再弹出来.
        if (xBrush->getBrush(index).color || xBrush->getBrush(index).select) {
            createColorsList(index);
            backToOffset();
        }
        else
        {
            if (_pensIndex == index) return;
            
            if (_pensIndex != 0) {
                //退回所选的笔
                auto layout = _pensList->getItem(_pensIndex);
                layout->setPositionY(0);
                auto armature = (Armature*)layout->getChildren().at(0);
                armature->getAnimation()->play("static");
            }
            
            auto layout = _pensList->getItem(index);
            layout->setTag(OFFSET_TRUE);
            auto sequence = Sequence::create(MoveBy::create(0.3, OFFSET_WAVE),
                                             CallFuncN::create([&](Node *node)
                                                               {
                                                                   auto layout = (Layout*)node;
                                                                   auto armature = (Armature*)layout->getChildren().at(0);
                                                                   armature->getAnimation()->play("wave");
                                                                   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(GameScene::onMovementToStatic, this, std::placeholders::_1, std::placeholders::_2));
                                                               }),
                                             NULL);
            layout->runAction(sequence);
        }
        
        _pensIndex = index;
    }
}
void GameScene::backToOffset(){
    for (int i = 1; i <= 10 ; i++) {
        auto layout = _pensList->getItem(_pensIndex);
        if (OFFSET_TRUE == layout->getTag()) {
            layout->setTag(OFFSET_FALSE);
            layout->runAction(MoveBy::create(0.3, -OFFSET_WAVE));
        }
    }
}

void GameScene::onColorPen(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    xAudio->playDefaultSound();
    
    string name = ((Node*)pSender)->getName();
    auto index = std::stoi(name) + 1;
    
    if (_chooseIndex == index || _colorsEnabled == false) return;
    
    //退回 首次默认选中的笔
    if(_defualtPen){
        auto layout = _colorsList->getItem(1);
        layout->stopAllActions();
        layout->setPositionY(0);
        
        _defualtPen = false;
    }
    
    if (_chooseIndex != 0) {
        //退回所选的笔
        auto layout = _colorsList->getItem(_chooseIndex);
        layout->stopAllActions();
        layout->setPositionY(0);
        auto armature = (Armature*)layout->getChildren().at(0);
        armature->getAnimation()->play("static");
    }
    _chooseIndex = index;
    xBrush->setChooseIndex(index - 1);
    
    auto layout = _colorsList->getItem(index);
    auto sequence = Sequence::create(MoveBy::create(0.3, OFFSET_WAVE),
                                     CallFuncN::create([&](Node *node)
                                                       {
                                                           auto layout = (Layout*)node;
                                                           auto armature = (Armature*)layout->getChildren().at(0);
                                                           armature->getAnimation()->play("wave");
                                                           armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(GameScene::onMovementToStatic, this, std::placeholders::_1, std::placeholders::_2));
                                                       }),
                                     NULL);
    layout->runAction(sequence);
    
    //换颜色
    xBrush->setColor(_chooseIndex - 1);
}

void GameScene::onMovementToStatic(Armature *armature, MovementEventType movementType)
{
    if (movementType == LOOP_COMPLETE || movementType == COMPLETE)
    {
        armature->getAnimation()->play("static");
    }
}

void GameScene::pushPensHandsOn(float dt){
    
    _accUpdateTime += dt;
    
    if(_accUpdateTime > _maxUpdateTime)
    {
        _maxUpdateTime = quickRandomFloat(2, 4);
        _accUpdateTime = 0;
        
        ListView * list = _pensList;
        vector<Armature *> * amt = &_pensAmt;
        if (_colorsEnabled) {
            list = _colorsList;
            amt = &_colorsAmt;
        }
        
        //截取当前笔的显示范围
        int start = 0;
        while (list->getItem(start)->getWorldPosition().x < _offsetPicturePos.x) {
            start++;
        }
        
        int end = list->getItems().size() - 3;
        float x = _offsetPicturePos.x + _colorsList->getContentSize().width;
        while (list->getItem(end)->getWorldPosition().x > x) {
            end--;
        }
        
        while (true) {
            auto animation =  amt->at(quickRandom(start, end))->getAnimation();
            
            if (animation->getCurrentMovementID() == "static") {
                if(quickRandomFloat(0, 1) > 0.7){
                    animation->play("wave");
                }
                else{
                    animation->play("blink");
                }
                
                animation->setMovementEventCallFunc(CC_CALLBACK_0(GameScene::onMovementToStatic, this, std::placeholders::_1, std::placeholders::_2));
                break;
            }
        }
    }
}

void GameScene::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    _currentMode->onTouchesBegan(touches, unused_event);
}

void GameScene::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    _currentMode->onTouchesMoved(touches, unused_event);
}

void GameScene::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    _currentMode->onTouchesEnded(touches, unused_event);
    xAudio->stopSound(_nSoundId);
    _nSoundId = 0;
    _startPaint = false;
}

void GameScene::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    _currentMode->onTouchesCancelled(touches, unused_event);
    xAudio->stopSound(_nSoundId);
    _nSoundId = 0;
    _startPaint = false;
}

void GameScene::switchMode(BaseMode::Mode mode)
{
    if (_currentMode)
    {
        _currentMode->stop();
    }
    
    switch (mode) {
        case BaseMode::Mode::NONE:
        {
            _currentMode = _noneMode;
        }
            break;
        case BaseMode::Mode::PAINT:
        {
            _currentMode = _paintMode;
        }
            break;
        case BaseMode::Mode::WAIT:
        {
            _currentMode = _waitMode;
        }
            break;
        case BaseMode::Mode::ZOOM:
        {
            _currentMode = _zoomMode;
        }
            break;
        case BaseMode::Mode::MOVE:
        {
            _currentMode = _moveMode;
        }
            break;
        default:
            break;
    }
    
    _currentMode->start();
}

void GameScene::paintPrevious()
{
    if (!xBrush->getStamp(true)) {
        for (int i = 0; i < _paints.size(); i ++) {
            Paint paint = _paints.at(i);
            if (i == 0) {
                _startPaint = true;
                _pictureScribble->paint(paint.pos);
            }
            else
            {
                _pictureScribble->paint(paint.pos, paint.previousPos);
            }
        }
        
        if (!_paints.empty()) {
            _haveDraw = true;
            _particle->resetSystem();
            _particle->setPosition(_paints.at(_paints.size() - 1).pos);
            if(xBrush->getBrush().name == "rainbowpen")
            {
                schedule(schedule_selector(GameScene::updateColor), .2);
            }
        }
    }
    else
    {
        if (!_paints.empty()) {
            _haveDraw = true;
            paintStamp(_paints.at(0).pos);
        }
    }
    
    _paints.clear();
}

void GameScene::paint(Vec2 & current, Vec2 & previous)
{
    if (!xBrush->getStamp(true)) {
        _pictureScribble->paint(current, previous);
        _particle->setPosition(current);
        
        if (_startPaint && _nSoundId == 0) {
            _nSoundId = xAudio->playSound("sfx/sfx_color_brushing.mp3", true);
        }
    }
}

void GameScene::paintStamp(Vec2 & current)
{
    if (xBrush->getStamp(true)) {
        _pictureScribble->paint(xBrush->getStamp(), current);
    }
}

Layer * GameScene::getColouringLayer()
{
    return _colouringLayer;
}

void GameScene::generateCurrentPaintingPicture()
{
    auto scale = _colouringLayer->getScale();
    auto pos = _colouringLayer->getPosition();
    auto anchor = _colouringLayer->getAnchorPoint();
    
    _colouringLayer->setScale(1);
    _colouringLayer->setPosition(Vec2::ZERO);
    _colouringLayer->setAnchorPoint(Vec2::ZERO);
    
    float offset = 768 - _pictureHeight;
    auto renderTexture = RenderTexture::create(_winSize.width - _offsetPicturePos.x, _winSize.height - offset, Texture2D::PixelFormat::RGBA8888);
    
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _colouringLayer->setPosition(-_offsetPicturePos.x, -offset);
    _colouringLayer->visit();
    _colouringLayer->setPosition(Vec2::ZERO);
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    CC_SAFE_RELEASE_NULL(_pictureImage);
    _pictureImage = renderTexture->newImage();
    
    _colouringLayer->setScale(scale);
    _colouringLayer->setPosition(pos);
    _colouringLayer->setAnchorPoint(anchor);
}

void GameScene::stopPaint()
{
    _particle->stopSystem();
    unschedule(schedule_selector(GameScene::updateColor));
}

void GameScene::updateColor(float dt)
{
    _isRandColorPen = true;
    _color = PensColorManager::getColorManager()->getColor();
    _pictureScribble->useTarget(_color);
}

void GameScene::loadPercent(Node *pNode, float pPercent)
{
}

void GameScene::loadEnded(Node *pNode)
{
    _condition++;
    if (_condition == 2) {
        //_loadingLayer->removeFromParent();
        //_loadingLayer->setTouchEnabled(true);
        _loadingLayer->setVisibleFalse();
    }
}

void GameScene::indent(ListView * list, bool isBefore)
{
    float width = 40;
    if(!isBefore)
    {
        width = 100;
    }
    
    auto indent = Layout::create();
    indent->setContentSize(Size(width, list->getContentSize().height));
    list->pushBackCustomItem(indent);
}

void GameScene::createColorsList(int index) //从1 开始
{
    _pensList->setVisible(false);
    _pensList->runAction(MoveBy::create(0.3, -OFFSET));
    _pensEnabled = false;
    
    _backBtn->setVisible(true);
    //添加所有的笔
    _colorsAmt.clear();
    _colorsList->removeAllItems();

    indent(_colorsList, true);
    
    auto brush = xBrush->getBrush(index);
    
    size_t size;
    if(brush.name == "rollerbrush")
    {
        size = xBrush->getPatternSelects().size();
    }
    else if(brush.name == "gold_pen_body")
    {
        size = xBrush->getGoldPenColors().size();
    }
    else
    {
        size = xBrush->getColors().size();
    }
    
    for(int i = 0; i < size; i++)
    {
        auto layout = Layout::create();
        layout->setName(std::to_string(i));
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(CC_CALLBACK_2(GameScene::onColorPen, this));
        
        layout->setContentSize(brush.size);
        auto armature = quickGetAnimation(1, brush.name, Vec2(layout->getPosition() + brush.offset));
        layout->addChild(armature);
        _colorsAmt.push_back(armature);
        
        _colorsList->pushBackCustomItem(layout);
    }
    
    indent(_colorsList, false);
    
    _chooseIndex = xBrush->getChooseIndex() + 1;
    
    _colorsList->setVisible(true);
    _colorsList->setTouchEnabled(false);
    _colorsList->runAction(Sequence::create(MoveBy::create(0.3, OFFSET),
                                          CallFuncN::create([&](Node * node)
                                                            {
                                                                _colorsEnabled = true;
                                                                _colorsList->setTouchEnabled(true);
                                                                
                                                                auto firstPen = _colorsList->getItems().at(_chooseIndex);
                                                                firstPen->setPosition(Vec2(OFFSET_WAVE + firstPen->getPosition()));
                                                            }),
                                          NULL));
    
    scheduleOnce(schedule_selector(GameScene::makeColor), 0);
}

void GameScene::makeColor(float dt)
{
    size_t count = _colorsList->getItems().size();
    
    for(int i = 1; i < count - 1; i++)
    {
        auto layout = (Layout*)_colorsList->getItem(i);
        auto armature = (Armature*)layout->getChildren().at(0);
        
        auto changes = xBrush->getBrush(_pensIndex).changes;
        for (auto change : changes) {
            auto body = armature->getBone(change);
            
            if(xBrush->getBrush().name == "gold_pen_body")
            {
                body->updateDisplayedColor(Color3B(xBrush->getGoldPenColors().at(i - 1)));
            }
            else if (xBrush->getBrush().name == "rollerbrush")
            {
                Skin * skin = Skin::create("game/roller/roller" + std::to_string(i) + ".png");
                body->addDisplay(skin, 0);
            }
            else
            {
                body->updateDisplayedColor(Color3B(xBrush->getColors().at(i - 1)));
            }
            
        }
    }
    
    //简单处理
    auto x = _colorsList->getItem(_chooseIndex)->getPosition().x - 46;
    auto width = _colorsList->getInnerContainerSize().width - 46;
    
    if (width - x < 550) {
        _colorsList->jumpToRight();
    }
    else
    {
        float percent = x * 100 / width;
        _colorsList->jumpToPercentHorizontal(percent);
    }
}

void GameScene::resumePensList()
{
    _backBtn->setVisible(false);
    _pensList->setVisible(true);
    _pensList->setTouchEnabled(false);
    _pensList->runAction(Sequence::create(MoveBy::create(0.3, OFFSET),
                                          CallFuncN::create([&](Node * node)
                                                            {
                                                                _pensEnabled = true;
                                                                _pensList->setTouchEnabled(true);
                                                            }),
                                          NULL));
    
    _colorsList->setVisible(false);
    _colorsList->runAction(MoveBy::create(0.3, -OFFSET));
    _colorsEnabled = false;
}

void GameScene::save()
{
    if (_haveDraw)
    {
        generateCurrentPaintingPicture();
        
        /* save the picture logo, include wireframe */
        std::string path = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex, _pictureIndex, true);
        _pictureImage->saveToFile(path.c_str());
        
        /* save the picture history, just color mask, not include wireframe */
        std::string picturePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex, _pictureIndex, false);
        _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
        
        Director::getInstance()->getRenderer()->render();
    }
}
