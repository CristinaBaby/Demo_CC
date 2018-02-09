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
#include "PackManager.h"
#include "CBUtils.h"
#include "BaseMode.h"
#include "NoneMode.h"
#include "PaintMode.h"
#include "MoveMode.h"
#include "ZoomMode.h"
#include "WaitMode.h"
#include "LoadingLayer.h"
#include "CBAppManager.h"
#include "BrushManager.h"
#include "CBPictureScene.h"
#include "PensColorManager.h"
#include "Pens.h"
#include "PensManager.h"
#include "STSystemFunction.h"
#include "TimerManager.hpp"
#include "ScribbleSVGUtil.h"
#include "CCRenderTexture_Multisampling.h"
#include "TextureLinearFilter.h"
#include "UndoManager.h"
#include "SVGCache.h"
#include "AdLoadingLayerBase.h"
#include "AdsLoadingLayer.h"
#include "LockManager.h"
#include "TryLaterLayer.h"

#define PINK                Color3B(150, 97, 59)
#define OFFSET              (Vec2(0, -17))

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
        setName("GameScene");
        _isRewardAd = getNetWorkState();
        
        //////////////////////////////////////////////////////////// 数据赋值 ////////////////////////////////////////////////////////////
        _packIndex = packIndex;
        _pictureIndex = pictureIndex;
        if (!xPack->isSamePicture(packIndex, pictureIndex)) {
            xPack->selectPicture(packIndex, pictureIndex);
            SVGCache::getInstance()->removeAllSources();
        }

        //////////////////////////////////////////////////////////// UI ////////////////////////////////////////////////////////////
        auto layerColor = LayerColor::create(Color4B::WHITE);
        addChild(layerColor);
        
        _root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("game.json");
        addChild(_root, 1);
        
        for (int i = 0; i < 5; i ++) {
            string name = "brush" + std::to_string(i + 1);
            auto brush = quickButton(name, _root, CC_CALLBACK_2(GameScene::onButton, this));
            _chooseBrushes.push_back(brush);
        }
        _chooseBrushes.at(4)->setColor(PINK);
        
        quickButton("back", _root, CC_CALLBACK_2(GameScene::onButton, this));
        quickButton("save", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _undoBtn = quickButton("undo", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _undoBtn->setVisible(false);
        _fillTypeBtn = quickButton("shua", _root, CC_CALLBACK_2(GameScene::onButton, this));
        _fillTypeBtn->loadTextureNormal("game/type_coloring.png");
        _fillTypeBtn->setVisible(false);
        
        auto loadAction = Sequence::create(DelayTime::create(0.5f), createLoadingAction(0), createLoadingAction(1), createLoadingAction(2), createLoadingAction(3), DelayTime::create(0.1f), createLoadingAction(4), NULL);
        runAction(loadAction);
        
        _loadingLayer = LoadingLayer::create();
        addChild(_loadingLayer, 3);
        
        _event = EventListenerCustom::create("director_after_visit", CC_CALLBACK_1(GameScene::onAfterVisit, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_event, this);
        
        auto event_fillnode = EventListenerCustom::create(EVENT_FILLNODE, CC_CALLBACK_1(GameScene::onEventFillnode, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(event_fillnode, this);
        
        return true;
    }
    
    return false;
}

void GameScene::onAfterVisit(EventCustom * event)
{
    _winSize = Director::getInstance()->getWinSize();
    
    Vec2 pos = Vec2(_winSize.width - 60, 0);
    float finalX = _undoBtn->getParent()->convertToNodeSpace(pos).x;
    float diff = finalX - _undoBtn->getPositionX();
    _undoBtn->setPositionX(finalX);
    _fillTypeBtn->setPositionX(_fillTypeBtn->getPositionX() + diff);
    
//    _undoBtn->setVisible(true);
    _fillTypeBtn->setPosition(_undoBtn->getPosition());
    _fillTypeBtn->setVisible(true);
    
    _eventDispatcher->removeEventListener(_event);
}

GameScene::~GameScene()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey(_picturePath);
    Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureHistoryPath);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    unschedule(schedule_selector(GameScene::updateFillModeColor));
    
    CC_SAFE_DELETE(_pictureImage);
    CC_SAFE_DELETE(_noneMode);
    CC_SAFE_DELETE(_waitMode);
    CC_SAFE_DELETE(_paintMode);
    CC_SAFE_DELETE(_moveMode);
    CC_SAFE_DELETE(_zoomMode);
    
    xPcm->clear();
    xBrush->clear();
    xUndo->destroyInstance();
}

ActionInterval* GameScene::createLoadingAction(int num){
    auto sequence = Sequence::create(CallFunc::create([=](){
        onProgress(num);
    }), DelayTime::create(0.f),NULL);
    
    return sequence;
}

void GameScene::onProgress(int percent)
{
    if (_init) {
        switch (percent) {
            case 0:
            {
                SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
                Director::getInstance()->getTextureCache()->removeUnusedTextures();
            }
                break;
            case 1: //////////////////////////////////////////////////////////// Scribble ////////////////////////////////////////////////////////////
            {
                setupScribble();
            }
                break;
            case 2: //////////////////////////////////////////////////////////// UI ////////////////////////////////////////////////////////////
            {
                xUndo->init(_pictureScribble);
                xBrush->init(_pictureScribble);
                xBrush->useBrush(5);
            }
                break;
            case 3: //////////////////////////////////////////////////////////// 画笔 ////////////////////////////////////////////////////////////
            {
                float listHeight = 230;
                auto image = (Widget*)Helper::seekWidgetByName(_root, "Image_15");
                float listStartY = image->getWorldPosition().y + image->getContentSize().height/2 - listHeight - 20;
                auto center = Helper::seekWidgetByName(_root, "center");
                float indent = 24;
                
                _pensList = ListView::create();
                _pensList->setBounceEnabled(false);
                _pensList->setItemsMargin(0);
                _pensList->setContentSize(Size(_winSize.width - indent, listHeight));
                _pensList->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
                _pensList->setGravity(cocos2d::ui::ListView::Gravity::BOTTOM);
                _pensList->setClippingEnabled(true);
                _pensList->setPosition(center->convertToNodeSpace(Vec2(indent, listStartY)));
                
                _pencil = ImageView::create("game/pens/ui04_pen_0.png");
                _pencil->setTouchEnabled(true);
                _pencil->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
                _pencil->setName("colorPen");
                _pensList->pushBackCustomItem(_pencil);
                
                int _step = 1;
                for (int i = 1; i < 31; i++) {
                    if(i > 24)
                    {
                        auto layout = Layout::create();
                        layout->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
                        layout->setContentSize(Size(160,260));
                        for (int j = 25 + (i-25)*5 ; j < (i-24)*5 + 25; j++) {
                            
                            Pens* pen = Pens::create(_step, this, _isRewardAd);
                            pen->setName(std::to_string(j));
                            pen->setTouchEnabled(true);
                            pen->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
                            
                            layout->addChild(pen);
                            pen->setPosition(Vec2(32.5 + 24*(j-(i - 25)*5 - 25), layout->getContentSize().height/3 + 11));
                            
                            _step++;
                        }
                        _pensList->pushBackCustomItem(layout);
                    }
                    else
                    {
                        Pens* pen = Pens::create(_step, this, _isRewardAd);
                        pen->setName(std::to_string(i));
                        pen->setTouchEnabled(true);
                        pen->addTouchEventListener(CC_CALLBACK_2(GameScene::onButton, this));
                        _pensList->pushBackCustomItem(pen);
                        
                        _step++;
                    }
                }
                center->addChild(_pensList, 1);
                _pensList->cocos2d::Node::visit();
                _pensList->jumpToRight();
                onButton(_pencil, Widget::TouchEventType::ENDED);
                _loadingLayer->setListView(_pensList);
            }
                break;
            case 4: //////////////////////////////////////////////////////////// add multi-touch event ////////////////////////////////////////////////////////////
            {
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
                
                _finishCount++;
                if(_finishCount > 1)
                {
                    _loadingLayer->showTapToStart();
                }
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch (percent) {
            case 0:
            {
                save();
            }
                break;
            case 1:
            {
                removeChild(_root);
                removeChild(_colouringLayer);
            }
                break;
            case 2:
            {
                SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
                Director::getInstance()->getTextureCache()->removeUnusedTextures();
            }
                break;
            case 3:
            {
                auto scene = PictureScene::createSceneWithPackIndex(_packIndex - 1);
                ((PictureScene*)scene->getChildren().at(0))->beBack();
                Director::getInstance()->replaceScene(scene);
            }
                break;
            default:
                break;
        }
    
    
    }
}

void GameScene::onEventFillnode(EventCustom* event)
{
    _finishCount++;
    if(_finishCount > 1)
    {
        _loadingLayer->showTapToStart();
    }
}

void GameScene::setupScribble()
{
    //add colouring layer
    _colouringLayer = Layer::create();
    _colouringLayer->setName("colouringLayer");
    _colouringLayer->ignoreAnchorPointForPosition(false);
    _colouringLayer->setAnchorPoint(Vec2::ZERO);
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
    
    //create picture scribble
    _pictureScribble = FillNode::create(Size(1536, 2272));
    _pictureScribble->debugPattern(false);
    _pictureScribble->setDefaultColor(Color4F::WHITE);
    _pictureScribble->setAntiAliasingBrush(true);
    _pictureScribble->setPosition(Director::getInstance()->getWinSize() / 2);
    _pictureScribble->useBrush("game/brushstroke/1/1-1.png");
    _pictureScribble->setLineFrame(Sprite::create(_picturePath));
    
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
    
    _pictureScribble->setPattern(_picturePath, true, 150);
    
    //修正Scribble的Scale使其适应屏幕大小. fix 边缘不能涂色的问题.
    float aspectRatio = _winSize.width / _winSize.height;
    if (aspectRatio > (float)2/3) {
        _magicScale = V::getRelativeScale() * 0.5;
    }
    else
    {
        _magicScale = 0.5;
    }
    _pictureScribble->setScale(_magicScale);
    _colouringLayer->addChild(_pictureScribble);
}

void GameScene::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED || !_init) return;
    
    string name = ((Node*)pSender)->getName();
    
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
    
    if (name.compare("save") == 0) {
        generateCurrentPaintingPicture();
        //PensManager::getInstance()->pensUnLock(1,false);
        auto saveLayer = SaveLayer::create(_pictureImage, this);
        saveLayer->setName("saveLayer");
        addChild(saveLayer, 3);
    }
    else if (name.compare("back") == 0) {
        _init = false;
        unschedule(schedule_selector(GameScene::updateColor));
        unschedule(schedule_selector(GameScene::updateFillModeColor));
        
        xApp->requestFullScreenAd(this, 101);
        AdLoadingLayerBase::loadingDoneCallback = CC_CALLBACK_0(GameScene::closeAds, this);
        xAudio->playDefaultSound();
    }
    else if (name == "undo") {
        xUndo->undo();
        xAudio->playDefaultSound();
    }
    else if (name == "shua") {
        if (_fillType == FillType::eColoring) {
            unschedule(schedule_selector(GameScene::updateFillModeColor));
            _fillType = FillType::eScribble;
            _fillTypeBtn->loadTextureNormal("game/shua.png");
            xPcm->setRainbowPenTag(true);
            xPcm->setDrawType(false);
            if(_useRainbowPen)
            xBrush->useColor(xPcm->getUseColor());
        }
        else
            {
            _fillType = FillType::eColoring;
            xPcm->setRainbowPenTag(false);
            xPcm->setDrawType(true);
            _fillTypeBtn->loadTextureNormal("game/type_coloring.png");
        }
        xAudio->playDefaultSound();
    }
    else if (name.find("brush") != string::npos)
    {
        int tag = std::stoi(name.substr(name.length()- 1, 1));
        
        switch (tag) {
            case 1:
            {
                _pictureScribble->useBrush("game/brushstroke/5/5-0.3.png");
            }
                break;
            case 2:
            {
                _pictureScribble->useBrush("game/brushstroke/5/5-0.4.png");
            }
                break;
            case 3:
            {
                _pictureScribble->useBrush("game/brushstroke/5/5-0.6.png");
            }
                break;
            case 4:
            {
                _pictureScribble->useBrush("game/brushstroke/5/5-0.8.png");
            }
                break;
            case 5:
            {
                _pictureScribble->useBrush("game/brushstroke/5/5-1.png");
            }
                break;
            default:
                break;
        }
        
        for (auto button : _chooseBrushes) {
            button->setColor(Color3B::WHITE);
        }
        
        ((Button*)pSender)->setColor(PINK);
        xAudio->playDefaultSound();
    }
    else
    {
        if (name == "colorPen")
        {
            _useRainbowPen = true;
            
            if(_fillType == FillType::eColoring)
            {
                xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
            }
            else
            {
                xBrush->useColor(xPcm->getUseColor());
            }
            
            if (_skip)
                _skip = false;
            else
                xAudio->playSound("pen/01.mp3");
        }
        else{
            _useRainbowPen = false;
            unschedule(schedule_selector(GameScene::updateFillModeColor));
//            int index = std::stoi(((Node*)pSender)->getName());
//            xBrush->setColor(index - 1);
        }
        // 弹出  收回
        if(_currentBrush) {// 如果上一次点击的笔存在
            auto c_name = _currentBrush->getName();
            if (c_name != "colorPen") {
                int index = stod(c_name);
                if (PensManager::getInstance()->isHaveLock(index)) { //真 为有锁
                    return;
                }
                else
                {
                    MoveToOffset(_currentBrush, false);  // 没有锁 直接收回
                }
            }
            else
            {
                MoveToOffset(_currentBrush, false);    // 如果是彩色 和 橡皮 没有特殊处理直接收回
            }
        }
        else
        {
            log("上一次没有点无锁笔");
        }
        
        auto brush = (Widget*)pSender;  // 当前点击笔
        
        if (name != "colorPen") {
            int index = stod(name);
            if (PensManager::getInstance()->isHaveLock(index)) { //真 为有锁
                _currentBrush = nullptr;
                return;
            }
            else
            {
                MoveToOffset(brush, true);  // 没有锁 直接弹出
                int index = std::stoi(((Node*)pSender)->getName());
                xBrush->setColor(index - 1);
            }
            
            char buffer[3];
            sprintf(buffer, "%02d", index + 1);
            xAudio->playSound("pen/" + string(buffer) + ".mp3");
        }
        else
        {
            MoveToOffset(brush, true);    // 如果是彩色 和 橡皮 没有特殊处理直接弹出
        }
        _currentBrush = brush;
    }
}

void GameScene::closeAds()
{
    AdsLoadingLayerEx * layer = AdsLoadingLayerEx::create();
    addChild(layer, 20);
    
    auto loadAction = Sequence::create(DelayTime::create(0.15f), createLoadingAction(0), createLoadingAction(1), createLoadingAction(2), createLoadingAction(3), NULL);
    runAction(loadAction);
}

void GameScene::updatePensPack(int group){
    int tag = group + 25;
    auto pen = _pensList->getItem(tag)->getChildren().at(4);
    auto lock = pen->getChildByName("lock");
    if(lock){
        xPens->pensUnLock(group, true);
        lock->runAction(CallFuncN::create([](Node* node){
            node->removeFromParent();
        }));
    }
    
    auto pen1 = _pensList->getItem(tag)->getChildren().at(0);
    auto background = pen1->getChildByName("background");
    if(background){
        background->runAction(CallFuncN::create([](Node* node){
            node->removeFromParent();
        }));
    }
}

void GameScene::MoveToOffset(Widget* current, bool offset){
    if(offset){
        current->runAction(MoveBy::create(0.2, -OFFSET));
    }
    else
    {
        current->runAction(MoveBy::create(0.2, OFFSET));
    }
}

void GameScene::updateColor(float dt){
    xBrush->useColor(PensColorManager::getColorManager()->getColor());
}

void GameScene::updateFillModeColor(float dt) {
    xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
}

void GameScene::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    if (_init)
    _currentMode->onTouchesBegan(touches, unused_event);
}

void GameScene::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    if (_init)
    _currentMode->onTouchesMoved(touches, unused_event);
}

void GameScene::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    if (_init)
    _currentMode->onTouchesEnded(touches, unused_event);
}

void GameScene::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    if (_init)
    _currentMode->onTouchesCancelled(touches, unused_event);
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
    {
        for (int i = 0; i < _paints.size(); i ++) {
            Paint paint = _paints.at(i);
            if (i == 0) {
                    if(_fillType == FillType::eColoring)
                    {
                        if (_useRainbowPen){
                            xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
                        }
                        _pictureScribble->getColoringNode()->coloring(paint.pos);
                    }
                    else
                    {
                        _pictureScribble->paint(paint.pos);
                    }
            }
            else
            {
                if(_fillType == FillType::eColoring)
                {
                    if (_useRainbowPen){
                        xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
                    }
                    _pictureScribble->getColoringNode()->coloring(paint.pos);
                    
                }
                else
                {
                    
                    _pictureScribble->paint(paint.pos, paint.previousPos);
                }
            }
        }
        
        if (!_paints.empty()) {
            _haveDraw = true;
            if(_useRainbowPen){
                if(xPcm->getRainbowPenTag())
                {
                    schedule(schedule_selector(GameScene::updateColor), .2);
                }
                else
                {
                    //xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
                    //schedule(schedule_selector(GameScene::updateFillModeColor), .2);
                }
            }
        }
    }
    
    _paints.clear();
}

void GameScene::paint(Vec2 & current, Vec2 & previous)
{
    {
        if(_fillType == FillType::eColoring)
        {
            if (_useRainbowPen){
                xBrush->useColor(PensColorManager::getColorManager()->getColorForFillMode());
            }
            _pictureScribble->getColoringNode()->coloring(current);
        }
        else
        {
        _pictureScribble->paint(current, previous);
        }

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
    
    auto image = (ImageView*)Helper::seekWidgetByName(_root, "Image_15");
    float offsetY = (image->getWorldPosition().y + image->getContentSize().height/2);
    _pictureScribble->setPositionY(_pictureScribble->getPositionY() - offsetY);
    
    auto renderTexture = RenderTexture::create(_colouringLayer->getContentSize().width,
                                               Utils::SCREEN_HEIGHT - (314 - (1136 - Utils::SCREEN_HEIGHT)/2.0),
                                               Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _pictureScribble->visit();
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    CC_SAFE_RELEASE_NULL(_pictureImage);
    _pictureImage = renderTexture->newImage();
    
    _pictureScribble->setPositionY(_pictureScribble->getPositionY() + offsetY);
    
    _colouringLayer->setScale(scale);
    _colouringLayer->setPosition(pos);
    _colouringLayer->setAnchorPoint(anchor);
}

void GameScene::stopPaint()
{
    if (_fillType == FillType::eColoring) {
        _pictureScribble->getColoringNode()->resetPatternColor();
        //unschedule(schedule_selector(GameScene::updateFillModeColor));
        
    }
    unschedule(schedule_selector(GameScene::updateColor));
    _paints.clear();
}

void GameScene::save()
{
    generateCurrentPaintingPicture();
    
    /* save the picture logo, include wireframe */
    std::string path = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex, _pictureIndex, true);
    _pictureImage->saveToFile(path.c_str());
    
    if (_haveDraw) {
        /* save the picture history, just color mask, not include wireframe */
        std::string picturePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex, _pictureIndex, false);
        _pictureScribble->getCanvas()->saveToFile(picturePath.c_str());
    }
    
    Director::getInstance()->getRenderer()->render();
}

void GameScene::networkChanged()
{
    //只处理从"有网"变成"没有网络"
    runAction(CallFuncN::create([&](Node * node){
        if(xLock->getNetworkState() && getNetWorkState() == false)
        {
            _isRewardAd = false;
            for (auto tag : xPens->getPackInfoID()) {
                if (xPens->getLockState(tag)) {
                    auto pen = _pensList->getItem(tag)->getChildren().at(4);
                    
                    auto lock = pen->getChildByName("lock");
                    if (lock) {
                        auto logo = (ImageView*)lock->getChildByName("logo");
                        if (logo) {
                            logo->loadTexture("game/lock_pack/play_2.png");
                        }
                    }
                }
            }
            
        }
    }));
}

void GameScene::craeteUnlockAds(Ref* pSender, Widget::TouchEventType type){
    if(Widget::TouchEventType::ENDED != type) return;
    
    _lockIndex = ((Widget*)pSender)->getTag();
    
    if (!_isRewardAd) {
        xLock->purchase(xPens->getParchaseInfoFromIndex(_lockIndex));
    }
    else
    {
        if (getChildByName("viewadslayer") == nullptr) {
            xDialog->show("text1", "buy", "play_now", CC_CALLBACK_1(GameScene::onDialog, this));
        }
    }
}

void GameScene::onDialog(const string& name)
{
    if (name == "buy") {
        xLock->purchase(xPens->getParchaseInfoFromIndex(_lockIndex));
    }
    else if (name == "play_now")
    {
        if (getNetWorkState()) {
            xLock->rewardAd(_lockIndex);
        }
        else
        {
            auto tryLater = TryLaterLayer::create();
            addChild(tryLater, 10);
        }
    }
}

bool GameScene::getNetWorkState(){
    STSystemFunction sf;
    return sf.checkNetworkAvailable();
}

float GameScene::getScaleAntialias()
{
    GLint max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
    
    auto size = _pictureScribble->getContentSize();
    
    float scaleWidth = max/size.width;
    float scaleHeight = max/size.height;
    float scale = scaleWidth < scaleHeight ? scaleWidth : scaleHeight;
    
    if (scale > 4) {
        scale = 4;
    }
    else
    {
        scale *= 0.9;
    }
    
    return scale;
}
