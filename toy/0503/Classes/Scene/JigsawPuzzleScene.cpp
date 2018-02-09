//
//  JigsawPuzzleScene.cpp
//  BedtimeStory
//
//  Created by maxiang on 8/31/15.
//
//

#include "JigsawPuzzleScene.h"
#include "MiniGameSelectScene.h"
#include "HomeScene.h"
#include "ChoosePageScene.h"
#include "AppManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace extension;

#define CELL_INTERVEL 30.0
#define TILE_SCALE_IN_CELL 0.8

enum
{
    TILE_SPRITE_UNTOUCHED_ZORDER = 10,
    TILE_SPRITE_TOUCHED_ZORDER,
    PARTICLE_SYSTEM_ZORDER
};

JigsawPuzzleScene::JigsawPuzzleScene():
_gameData(nullptr),
_jigsawToResScale(1.0),
_isTouchInTableTileSprite(false),
_isMovingTableTileSprite(false),
_isTouchInContainerTileSprite(false),
_isMovingContainerTileSprite(false),
_isScrollingTableView(false),
_touchedTableTileSprite(nullptr),
_touchedContainerTileSprite(nullptr),
_isAnimating(false),
_tileSpriteContainer(nullptr),
_jigsawToContainerScale(1.0),
_tableTileSpriteOriginalPosition(Point::ZERO),
_tableCellOriginalPosition(Point::ZERO),
_tableOriginalOffset(Point::ZERO),
_usedTime(0),
_fixedTileSpriteCount(0),
_pause(true),
_timeLabel(nullptr),
_gameContentLayer(nullptr),
_hintButton(nullptr)
{
    
}

JigsawPuzzleScene::~JigsawPuzzleScene()
{
    unschedule(schedule_selector(JigsawPuzzleScene::timerUpdate));

}

#pragma mark- Init methods

bool JigsawPuzzleScene::init()
{
    if (!BaseScene::init()) {
        return false;
    }
    
    //init ui
    _rootLayout = GUIReader::getInstance()->widgetFromJsonFile("jigsaw(11).json");
    addChild(_rootLayout, 1);

    //game content layer
    _gameContentLayer = Layer::create();
    addChild(_gameContentLayer, 2);
    
    //container
    _tileSpriteContainer = (ImageView *)Helper::seekWidgetByName(_rootLayout, "tile_container");
    _tileSpriteContainer->loadTexture("frame_2.png");
    _tileSpriteContainer->retain();
    _tileSpriteContainer->removeFromParent();
    namespaceST::STVisibleRect::setPosition(_tileSpriteContainer, 120.0, 575.0);
    _gameContentLayer->addChild(_tileSpriteContainer, 1);
    _tileSpriteContainer->release();
    
    auto frame = Sprite::create("frame_1.png");
    frame->setPosition(_tileSpriteContainer->getContentSize()/2);
    _tileSpriteContainer->addChild(frame);
    
    //time label
    _timeLabel = dynamic_cast<Text*>(Helper::seekWidgetByName(_rootLayout, "time_label"));
    _timeLabel->setString(AppManager::getInstance()->AppManager::getInstance()->getFormatTimeString(_usedTime));
    
    //back
    Utils::addEventToButton(_rootLayout, "back_button", Widget::TouchEventType::ENDED, [&](){
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        auto scene = BaseScene::createScene<ChoosePageScene>();
        ChoosePageScene::needShowFullScreen = true;
        auto fadeTransition = TransitionFade::create(0.3, scene);
        auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
        layer->setupGameType(GameType::Jigsaw);
        
        Director::getInstance()->replaceScene(fadeTransition);
    });
    
    //hint
    _hintButton = Utils::addEventToButton(_rootLayout, "hint_button", Widget::TouchEventType::ENDED, [&](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);

        _hintButton->loadTextureNormal("time2_disable.png");
        _hintButton->setTouchEnabled(false);
        
        auto sprite = Sprite::create("time2.png");
        auto progressTimer = ProgressTimer::create(sprite);
        progressTimer->runAction(Sequence::create(ProgressFromTo::create(SPOT_GAME_HINT_COOL_DOWN, 0, 100), CallFunc::create([=](){
            
            progressTimer->removeFromParent();
            _hintButton->loadTextureNormal("time2.png");
            _hintButton->setTouchEnabled(true);
        }), NULL));
        progressTimer->setPosition(_hintButton->getPosition());
        progressTimer->setType(cocos2d::ProgressTimer::Type::BAR);
        progressTimer->setMidpoint(Vec2(0.5, 0.0));
        progressTimer->setBarChangeRate(Vec2(0.0, 1.0));
        _gameContentLayer->addChild(progressTimer);
        
        showHint();
    });
    
    //init table view
    _tableView = extension::TableView::create(this, Size(134.0, 470.0));
    _tableView->ignoreAnchorPointForPosition(false);
    namespaceST::STVisibleRect::setPosition(_tableView, 860.0, 280.0, false, false, false, true);
    _tableView->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setDelegate(this);
    _gameContentLayer->addChild(_tableView, 10);
    
    //schedule timer
    schedule(schedule_selector(JigsawPuzzleScene::timerUpdate), 1);
    
    //register touch listener
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(JigsawPuzzleScene::onTouchBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(JigsawPuzzleScene::onTouchMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(JigsawPuzzleScene::onTouchEnded, this);
    touchEventListener->onTouchCancelled = CC_CALLBACK_2(JigsawPuzzleScene::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, _gameContentLayer);
    
    //iap delegate
    _iap.setIABDelegate(this);
    
    return true;
}

#pragma mark- Game

void JigsawPuzzleScene::startGameWithGameLevel(const int level)
{
    CCASSERT(MiniGameManager::getInstance()->getJigsawDataVector().size() >= level, "Invalid game level");

    _usedTime = 0;

    _timeLabel->setString(AppManager::getInstance()->getFormatTimeString(0));
    
    //store current game level
    _gameLevel = level;
    
    //setup jigsaw tiles
    setupJigsawTiles(_gameLevel);
    
    //start refresh timer label
    _pause = false;
    
    //reload tile sprite table view
    _tableView->reloadData();
}

void JigsawPuzzleScene::setupJigsawTiles(const int level)
{
    CCASSERT(MiniGameManager::getInstance()->getJigsawDataVector().size() > level, "Invalid game level");
    
    auto gameData = MiniGameManager::getInstance()->getJigsawDataVector().at(level);
    _gameData = gameData;

    int rows = _gameData->config->rows;
    int columns = rows;
    _matrixRow = rows;
    
    auto jigsawSprite = Sprite::create(_gameData->textureFileName);
    jigsawSprite->setAnchorPoint(Vec2(0.0, 1.0));
    BlendFunc blend = {GL_DST_ALPHA, GL_ONE_MINUS_DST_COLOR};
    jigsawSprite->setBlendFunc(blend);
    _jigsawToResScale = _gameData->config->totalWidth / 1024.0;
    _jigsawToContainerScale = _gameData->config->totalWidth / _tileSpriteContainer->getContentSize().width;
    
    _tileSpritesVector.clear();
    int index = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            auto tileInfo = gameData->config->jigsawTilesVector.at(index);
            auto jigsawTileSprite = Sprite::create(tileInfo.tilePath);
            jigsawTileSprite->setAnchorPoint(Vec2::ZERO);
            jigsawTileSprite->setPosition(Vec2::ZERO);
            jigsawTileSprite->setScale(1.0/_jigsawToResScale);

            jigsawSprite->setPosition(Vec2(-(tileInfo.offsetx - tileInfo.width/2)*(1.0/_jigsawToResScale), (tileInfo.offsety + tileInfo.height/2)*(1.0/_jigsawToResScale)));
            RenderTexture *render = RenderTexture::create(tileInfo.width*(1.0/_jigsawToResScale), tileInfo.height*(1.0/_jigsawToResScale), Texture2D::PixelFormat::RGBA8888);
            
            render->begin();
            jigsawTileSprite->visit();
            jigsawSprite->visit();
            render->end();
            Director::getInstance()->getRenderer()->render();

            auto image = render->newImage();
            auto texture = new Texture2D();
            texture->initWithImage(image);
            auto sprite = JigsawTileSprite::createWithTexture(texture);
            sprite->setTag(index);
            sprite->setScale(_jigsawToResScale);
            _tileSpritesVector.pushBack(sprite);

            image->autorelease();
            texture->autorelease();
            
            index++;
        }
    }
    
    //TODO: --
    std::random_shuffle(_tileSpritesVector.begin(), _tileSpritesVector.end());
}

void JigsawPuzzleScene::finishGame()
{
    //pause the timer
    _pause = true;
    
    //set fixed tile count to 0
    _fixedTileSpriteCount = 0;

    //hide ui layer
    //_rootLayout->setVisible(false);
    
    //hide game content layer
    //_gameContentLayer->setVisible(false);
    
    //remove all tile sprites in container vector
    for (auto sprite : _containerTileSpritesVector)
    {
        sprite->removeFromParent();
    }
    _containerTileSpritesVector.clear();
    
    //remove all tile sprites in tile sprite vector
    _tileSpritesVector.clear();

    //init finish game layer
    auto layer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 0.0));
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch *touch, Event *unusedEvent){
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, layer);
    addChild(layer, 110);
    
    layer->runAction(FadeTo::create(0.3, 200.0));
    
    AppManager::getInstance()->playEffect(SOUND_POPUB);

    auto ui = GUIReader::getInstance()->widgetFromJsonFile("finish_dialog(1).json");
    ui->setAnchorPoint(Point::ANCHOR_MIDDLE);
    ui->setPosition(getContentSize()/2);
    layer->addChild(ui, 1);

    AppManager::getInstance()->setBannerAdVisible(true);
    ui->setScale(0.1);
    ui->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
        
        /* Ads Logic */
        AppManager::getInstance()->requestFullScreenAd();
    }), NULL));
    
    auto label = static_cast<Text*>(Helper::seekWidgetByName(ui, "time_label"));
    label->setString(AppManager::getInstance()->getFormatTimeString(_usedTime));

    Utils::addEventToButton(ui, "next_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        _pause = false;
   
        int level = _gameLevel + 1;
        
        if (MiniGameManager::getInstance()->getJigsawDataVector().size() <= level)
        {
            level = 0;//如果已经到达最后一张，循环回到第一张
        }
        
        if (level >= 15 && !AppManager::getInstance()->getHasUnlockAll())
        {
            showPurchaseDialog();
            return;
        }
        
        startGameWithGameLevel(level);
        
        layer->removeFromParent();
    });
    
    Utils::addEventToButton(ui, "replay_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        _pause = false;

        startGameWithGameLevel(_gameLevel);
        
        layer->removeFromParent();
    });
    
    Utils::addEventToButton(ui, "level_button", Widget::TouchEventType::ENDED, [=](){
        
        AppManager::getInstance()->playEffect(SOUND_BTN_DEFAULT);
        AppManager::getInstance()->setBannerAdVisible(false);

        auto scene = BaseScene::createScene<ChoosePageScene>();
        auto fadeTransition = TransitionFade::create(0.3, scene);
        auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
        layer->setupGameType(GameType::Jigsaw);
        
        Director::getInstance()->replaceScene(fadeTransition);
    });
}

#pragma mark- Hint

void JigsawPuzzleScene::showHint()
{
    if (!_gameData)
        return;
    
    auto jigsawSprite = Sprite::create(_gameData->textureFileName);
    jigsawSprite->setScale(_tileSpriteContainer->getContentSize().width/1024.0);
    jigsawSprite->setOpacity(0.0);
    jigsawSprite->setPosition(_tileSpriteContainer->getContentSize()/2);
    jigsawSprite->runAction(Sequence::create(FadeTo::create(0.3, 170.0), DelayTime::create(5.0), FadeOut::create(0.3), CallFunc::create([=](){
        
        jigsawSprite->removeFromParent();
    }), NULL));
    
    _tileSpriteContainer->addChild(jigsawSprite, 2);
}

#pragma mark- Adapt Tile Sprite

bool JigsawPuzzleScene::adaptTileSpriteOriginalPosition(JigsawTileSprite *tileSprite, const Point& point)
{
    if (_gameData->config->jigsawTilesVector.size() <= tileSprite->getTag()) {
        return false;
    }
    
    auto touchTileInfo = _gameData->config->jigsawTilesVector.at(tileSprite->getTag());
    
    auto pp = _tileSpriteContainer->convertToNodeSpace(point);
    CCLOG("%f %f",pp.x, pp.y);
    
    float dstx = touchTileInfo.offsetx * (1.0/_jigsawToContainerScale);
    float dsty = touchTileInfo.offsety * (1.0/_jigsawToContainerScale);
    
    if (fabsf(pp.x - dstx) < JIGSAW_ORIGINAL_INTERVAL &&
        fabsf(_tileSpriteContainer->getContentSize().height - pp.y - dsty) < JIGSAW_ORIGINAL_INTERVAL)
    {
        AppManager::getInstance()->playEffect(SOUND_JIGSAW_CORRECT);

        float fixedx = dstx + _tileSpriteContainer->getPosition().x;
        float fixedy = _tileSpriteContainer->getPosition().y - dsty;
        
        float changedX = tileSprite->getPosition().x - fixedx;
        float changedY = tileSprite->getPosition().y - fixedy;
        
        tileSprite->setPosition(Vec2(fixedx, fixedy));
        tileSprite->setIsFixed(true);
        tileSprite->setZOrder(0);//已经固定的图块放到最下层，防止挡住还没有拼好的图块
        _fixedTileSpriteCount++;
        showParticle(tileSprite->getPosition());

        for (auto child : tileSprite->getLinkedTileChildren())
        {
            child->setPosition(Vec2(child->getPosition().x - changedX, child->getPosition().y - changedY));
            child->setIsFixed(true);
            child->setZOrder(0);//已经固定的图块放到最下层，防止挡住还没有拼好的图块

            _fixedTileSpriteCount++;
            showParticle(child->getPosition());
        }
        
        if (_fixedTileSpriteCount >= _matrixRow * _matrixRow)
        {
            //finish game
            _isAnimating = true;
            runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([&](){
                
                _isAnimating = false;
                finishGame();
            }), NULL));
        }
        return true;
    }
    
    return false;
}

bool JigsawPuzzleScene::adaptTileSpriteRelativePosition(JigsawTileSprite *tileSprite, const Point& point)
{
    int index = tileSprite->getTag();
    if (_gameData->config->jigsawTilesVector.size() <= index) {
        return false;
    }
    
    //four direction
    int topIndex = index - _matrixRow;
    int leftIndex = index - 1;
    int rightIndex = index + 1;
    int bottomIndex = index + _matrixRow;
    
    int adaptToTileInfoIndex = -1;
    JigsawTileSprite *adaptToSprite = nullptr;
    float adaptFromTilePositionx = tileSprite->getPosition().x;
    float adaptFromTilePositiony = tileSprite->getPosition().y;
    
    for (auto sprite : _containerTileSpritesVector)
    {
        auto iterator = std::find(tileSprite->getLinkedTileChildren().begin(), tileSprite->getLinkedTileChildren().end(), sprite);
        if (iterator != tileSprite->getLinkedTileChildren().end())
            continue;//no need to adapt linked tile
        
        float adaptToTilePositionx = sprite->getPosition().x;
        float adaptToTilePositiony = sprite->getPosition().y;
        
        adaptToTileInfoIndex = -1;

        if (sprite->getTag() == topIndex)
        {
            if (adaptFromTilePositiony > adaptToTilePositiony)
                return false;
            
            adaptToTileInfoIndex = topIndex;
        }
        else if(sprite->getTag() == leftIndex)
        {
            if (adaptFromTilePositionx < adaptToTilePositionx)
                return false;
            
            adaptToTileInfoIndex = leftIndex;
        }
        else if(sprite->getTag() == rightIndex)
        {
            if (adaptFromTilePositionx > adaptToTilePositionx)
                return false;
            
            adaptToTileInfoIndex = rightIndex;
        }
        else if(sprite->getTag() == bottomIndex)
        {
            if (adaptFromTilePositiony < adaptToTilePositiony)
                return false;
            
            adaptToTileInfoIndex = bottomIndex;
        }
            
        if (adaptToTileInfoIndex != -1)
        {
            adaptToSprite = sprite;
            
            auto adaptFromTileInfo = _gameData->config->jigsawTilesVector.at(index);
            auto adaptToTileInfo = _gameData->config->jigsawTilesVector.at(adaptToTileInfoIndex);
            
            float adaptx = (adaptFromTileInfo.offsetx - adaptToTileInfo.offsetx) * (1.0/_jigsawToContainerScale);
            float adapty = (adaptFromTileInfo.offsety - adaptToTileInfo.offsety) * (1.0/_jigsawToContainerScale);
            
            if (fabsf(fabsf(point.x - adaptToSprite->getPosition().x) - fabsf(adaptx)) < JIGSAW_RELATIVE_INTERVAL &&
                fabsf(fabsf(point.y - adaptToSprite->getPosition().y) - fabsf(adapty)) < JIGSAW_RELATIVE_INTERVAL)
            {
                AppManager::getInstance()->playEffect(SOUND_JIGSAW_CORRECT);

                float fixedx = adaptToSprite->getPosition().x + adaptx;
                float fixedy = adaptToSprite->getPosition().y - adapty;
                
                float changedX = tileSprite->getPosition().x - fixedx;
                float changedY = tileSprite->getPosition().y - fixedy;
             
                //tileSprite->runAction(MoveTo::create(0.1, Vec2(fixedx, fixedy)));
                tileSprite->setPosition(Vec2(fixedx, fixedy));
                tileSprite->addTileChild(adaptToSprite);
                adaptToSprite->addTileChild(tileSprite);
                
                //move linked children
                for (auto child : tileSprite->getLinkedTileChildren())
                {
                    child->setPosition(Vec2(child->getPosition().x - changedX, child->getPosition().y - changedY));
                }
  
                return true;
            }
        }
    }

    return false;
}

#pragma mark- Moving Fuctions

void JigsawPuzzleScene::moveContainerTileSprite(Touch* touch)
{
    Point currentPoint = touch->getLocation();
    Point previousPoint = touch->getPreviousLocation();

    Point offset = currentPoint - previousPoint;
    _touchedContainerTileSprite->setPosition(_touchedContainerTileSprite->getPosition() + offset);
    for (auto child : _touchedContainerTileSprite->getLinkedTileChildren())
    {
        child->setPosition(child->getPosition() + offset);
    }
}

void JigsawPuzzleScene::endTouchContainerTileSprite(Touch* touch)
{
    Point currentPoint = touch->getLocation();
    Point previousPoint = touch->getPreviousLocation();
    
    Point offset = currentPoint - previousPoint;
    
    //adapt original tile position
    bool adaptOriginalPosition = adaptTileSpriteOriginalPosition(_touchedContainerTileSprite, _touchedContainerTileSprite->getPosition() + offset);
    
    if (!adaptOriginalPosition)
    {
        for (auto child : _touchedContainerTileSprite->getLinkedTileChildren())
        {
            if (adaptTileSpriteOriginalPosition(child, child->getPosition() + offset))
            {
                adaptOriginalPosition = true;
                break;
            }
        }
    }

    //adapt relative tile position
    if (!adaptOriginalPosition)
    {
        if (!adaptTileSpriteRelativePosition(_touchedContainerTileSprite, _touchedContainerTileSprite->getPosition() + offset))
        {
            for (auto child : _touchedContainerTileSprite->getLinkedTileChildren())
            {
                findLinkedChildren(child);
  
                if (adaptTileSpriteRelativePosition(child, child->getPosition() + offset))
                    break;
            }
        }
    }

    std::sort(_containerTileSpritesVector.begin(), _containerTileSpritesVector.end(), [](JigsawTileSprite *sprite1, JigsawTileSprite *sprite2){
        
        return sprite1->getZOrder() > sprite2->getZOrder();
    });
    
    restoreTouchState();
}

void JigsawPuzzleScene::moveTableTileSprite(Touch* touch)
{
    if (_isScrollingTableView)
        return;
    
    if (!_isMovingTableTileSprite)
    {
        Vec2 currentPoint = touch->getLocation();
        Vec2 startPoint = touch->getStartLocation();
        
        if (fabsf(startPoint.y - currentPoint.y) > 5.0)
        {
            _isScrollingTableView = true;
            return;
        }
        
        if (fabsf(startPoint.x - currentPoint.x) > 5.0)
        {
            _isMovingTableTileSprite = true;
        }
    }
    
    if (!_isMovingTableTileSprite)
        return;
    
    _tableView->setTouchEnabled(false);
    
    Vec2 previousPoint = touch->getPreviousLocation();
    Vec2 currentPoint = touch->getLocation();
    
    if (_touchedTableTileSprite->getParent() != _gameContentLayer)
    {
        Vec2 point = _gameContentLayer->convertToNodeSpace(_touchedTableTileSprite->getParent()->convertToWorldSpace(_touchedTableTileSprite->getPosition()));
        
        //store the tile sprite original position
        _tableTileSpriteOriginalPosition = point;
        
        //store the tile sprite cell original postion
        _tableCellOriginalPosition = _touchedTableTileSprite->getParent()->getPosition();
        
        //store the table view current offset
        _tableOriginalOffset = _tableView->getContentOffset();
        
        //remove tile sprite from table view cell
        _touchedTableTileSprite->removeFromParent();
        
        //add the tile sprite to layer
        _touchedTableTileSprite->setPosition(point + (currentPoint-previousPoint));
        _gameContentLayer->addChild(_touchedTableTileSprite, TILE_SPRITE_TOUCHED_ZORDER);
    }
    else
    {
        Vec2 point = _touchedTableTileSprite->getPosition() + (currentPoint-previousPoint);

        Rect containerBox;
        //containerBox.origin = Vec2(namespaceST::STVisibleRect::getDesignOffset().x + 162.0, namespaceST::STVisibleRect::getDesignOffset().y + 104.0);
        containerBox.size = _tileSpriteContainer->getContentSize();
        if (containerBox.containsPoint(_tileSpriteContainer->convertToNodeSpace(point)))
        {
            float scale = _jigsawToResScale * (1.0/_jigsawToContainerScale);
            _touchedTableTileSprite->setScale(scale);
        }
        else
        {
            _touchedTableTileSprite->setScale(_jigsawToResScale * TILE_SCALE_IN_CELL);
        }
        _touchedTableTileSprite->setPosition(point);
    }
}

void JigsawPuzzleScene::endTouchTableTileSprite(Touch* touch)
{
    Rect containerBox;
    //containerBox.origin = Vec2(namespaceST::STVisibleRect::getDesignOffset().x + 162.0, namespaceST::STVisibleRect::getDesignOffset().y + 104.0);
    containerBox.size = _tileSpriteContainer->getContentSize();
    
    Vec2 currentPoint = touch->getLocation();
    if (containerBox.containsPoint(_tileSpriteContainer->convertToNodeSpace(currentPoint)))
    {
        _isAnimating = true;

        for (auto cell : _visibleCells)
        {
            if (cell->getPosition().y < _tableCellOriginalPosition.y)
            {
                cell->runAction(MoveBy::create(0.3, Vec2(0.0, _touchedTableTileSprite->getContentSize().height*_jigsawToResScale + CELL_INTERVEL)));
            }
        }
        
        Vec2 previousPoint = touch->getPreviousLocation();
        Vec2 point = _touchedTableTileSprite->getPosition() + (currentPoint-previousPoint);
        adaptTileSpriteOriginalPosition(_touchedTableTileSprite, point);
        adaptTileSpriteRelativePosition(_touchedTableTileSprite, point);

        runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([&](){
            
            auto height = _touchedTableTileSprite->getContentSize().height*_jigsawToResScale + CELL_INTERVEL;
            auto iterator = std::find(_tileSpritesVector.begin(), _tileSpritesVector.end(), _touchedTableTileSprite);
            
            _touchedTableTileSprite->setScale(_jigsawToResScale * (1.0/_jigsawToContainerScale));
            
            if (iterator != _tileSpritesVector.end())
            {
                _containerTileSpritesVector.pushBack(*iterator);
                _tileSpritesVector.erase(iterator);
                
                std::sort(_containerTileSpritesVector.begin(), _containerTileSpritesVector.end(), [](JigsawTileSprite *sprite1, JigsawTileSprite *sprite2){
                    
                    return sprite1->getZOrder() > sprite2->getZOrder();
                });
            }
            
            _tableView->reloadData();
            
            /* 如果table的偏移量小于一个图块的位置，说明用户已经滑动到了最下方，此时滚动图块后，要设置
            table的偏移量为0，保持在最下方（并且总的图块数>3, 防止用户从底部一直拖出图块返回到顶部，
            导致图块还是保持在最下方）*/
            if (fabsf(_tableOriginalOffset.y) < height&&_tileSpritesVector.size()>=3)
            {
                _tableView->setContentOffset(Vec2(0.0, height));
                _tableView->setContentOffset(Vec2(0.0, 0.0), true);
            }
            else
                _tableView->setContentOffset(Vec2(0.0, _tableOriginalOffset.y + height));

            restoreTouchState();
        }), NULL));
    }
    else
    {
        _isAnimating = true;
        _touchedTableTileSprite->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(0.5, _tableTileSpriteOriginalPosition), 0.3), CallFunc::create([&](){
            
            //remove from this layer
            _touchedTableTileSprite->removeFromParent();
            
            //refresh table view
            _tableView->reloadData();
            _tableView->setContentOffset(_tableOriginalOffset);

            restoreTouchState();
        }), NULL));
    }
}

void JigsawPuzzleScene::restoreTouchState()
{
    _isTouchInTableTileSprite = false;
    if (_touchedTableTileSprite) {
        _touchedTableTileSprite->setZOrder(TILE_SPRITE_UNTOUCHED_ZORDER);
    }
    _touchedTableTileSprite = nullptr;
    _isMovingTableTileSprite = false;
    _isScrollingTableView = false;
    _tableView->setTouchEnabled(true);
    
    _isTouchInContainerTileSprite = false;
    if (_touchedContainerTileSprite)
    {
        _touchedContainerTileSprite->setIsCheckedLink(false);
        if (_touchedContainerTileSprite->getZOrder() != 0)//如果zorder是0，说明已经固定
            _touchedContainerTileSprite->setZOrder(TILE_SPRITE_UNTOUCHED_ZORDER);
        
        for (auto linkSprite : _touchedContainerTileSprite->getLinkedTileChildren())
        {
            if (linkSprite->getZOrder() != 0)//如果zorder是0，说明已经固定
                linkSprite->setZOrder(TILE_SPRITE_UNTOUCHED_ZORDER);
            linkSprite->setIsCheckedLink(false);
        }
        _touchedContainerTileSprite->getLinkedTileChildren().clear();
    }
    _touchedContainerTileSprite = nullptr;
    _isMovingContainerTileSprite = false;
    
    _tableTileSpriteOriginalPosition = Point::ZERO;
    _tableCellOriginalPosition = Point::ZERO;
    _tableOriginalOffset = Point::ZERO;
    
    _isAnimating = false;
}

#pragma mark- Timer

void JigsawPuzzleScene::timerUpdate(float time)
{
    if (_pause) {
        return;
    }
    
    _usedTime++;
    _timeLabel->setString(AppManager::getInstance()->getFormatTimeString(_usedTime));
}

#pragma mark- Inner Functions

void JigsawPuzzleScene::showPurchaseDialog()
{
    auto maskLayer = Utils::createMaskLayer(200.0);
    addChild(maskLayer, 200);
    
    AppManager::getInstance()->setBannerAdVisible(true);
    
    auto dialogBg = Sprite::create("popup.png");
    dialogBg->setPosition(maskLayer->getContentSize()/2);
    dialogBg->setScale(0.0);
    maskLayer->addChild(dialogBg);
    dialogBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5, 1.0), 0.7), CallFunc::create([=](){
        
        
    }), NULL));
    
    auto text = Sprite::create("text(3).png");
    text->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    text->setPosition(Vec2(70.0, 187.0));
    dialogBg->addChild(text);
    
    auto yesButton = Button::create("buy.png");
    yesButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    yesButton->setPosition(Vec2(110.0, 52.0));
    yesButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
            
            _iap.purchase(IAP_UNLOCK_All);
        }
    });
    dialogBg->addChild(yesButton);
    
    auto noButton = Button::create("later.png");
    noButton->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    noButton->setPosition(Vec2(320.0, 52.0));
    noButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
        
        if (type == Widget::TouchEventType::ENDED)
        {
            AppManager::getInstance()->setBannerAdVisible(false);
            
            dialogBg->runAction(Sequence::create(ScaleTo::create(0.3, 0.0), CallFunc::create([=](){
                
                maskLayer->removeFromParent();
            }), NULL));
        }
    });
    dialogBg->addChild(noButton);
}

void JigsawPuzzleScene::showQuitGameLayer()
{
    _pause = true;
    auto quitLayer = LayerColor::create(Color4B(0.0, 0.0, 0.0, 220.0));
    auto ui = GUIReader::getInstance()->widgetFromJsonFile("ui11_quitspot.json");
    auto closeButton = Helper::seekWidgetByName(ui, "Button_24");
    closeButton->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type){
        
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            _pause = false;
            quitLayer->removeFromParent();
        }
    });
    
    auto yesButton = Helper::seekWidgetByName(ui, "Button_26");
    yesButton->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type){
        
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            auto scene = BaseScene::createScene<ChoosePageScene>();
            auto layer = dynamic_cast<ChoosePageScene*>(scene->getChildren().at(0));
            
            layer->setupGameType(GameType::Jigsaw);
            Director::getInstance()->replaceScene(scene);
        }
    });
    
    //text label
    auto label = dynamic_cast<Text*>(Helper::seekWidgetByName(ui, "Label_25"));
    label->setFontName("TitanOne-Regular.ttf");
    label->setFontSize(27);
    label->setString("You will lose your current\nprogress if you quit the game,\nare you sure?");
    label->setColor(Color3B::BLACK);
    label->setTextHorizontalAlignment(TextHAlignment::CENTER);
    label->setTextVerticalAlignment(TextVAlignment::CENTER);
    
    quitLayer->addChild(ui);
    this->addChild(quitLayer, 10);
}

void JigsawPuzzleScene::showParticle(const Point& position)
{
    auto emitter = ParticleFlower::create();
    _gameContentLayer->addChild(emitter, 100);
    
    emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("particle/stars2.png"));
    // duration
    emitter->setDuration(0);

    // angle
    emitter->setAngle(90);
    emitter->setAngleVar(360);
    
    // speed of particles
    emitter->setSpeed(300* 6.f/ 4);
    emitter->setSpeedVar(250);
    
    emitter->setStartSize(15*6.f - 4*15);
    // life of particles
    emitter->setLife(0.4);
    emitter->setLifeVar(0.2);
    
    // color of particles
    Color4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
    emitter->setStartColor(startColor);

    Color4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
    emitter->setEndColor(endColor);

    // emits per second
    emitter->setEmissionRate(emitter->getTotalParticles()*3);
    
    // additive
    emitter->setBlendAdditive(false);
   
    emitter->setPosition(position);
}

void JigsawPuzzleScene::findLinkedChildren(JigsawTileSprite* tileSprite)
{
    //clear old
    tileSprite->getLinkedTileChildren().clear();
    
    //recursive find
    recursiveFindLinkedTileSprite(tileSprite->getLinkedTileChildren(), tileSprite);
    
    //reset flag
    tileSprite->setIsCheckedLink(false);
    for (auto linkedChild : tileSprite->getLinkedTileChildren())
    {
        linkedChild->setIsCheckedLink(false);
    }
}

void JigsawPuzzleScene::recursiveFindLinkedTileSprite(std::vector<JigsawTileSprite*>& linkedTileVector, JigsawTileSprite *tileSprite)
{
    tileSprite->setIsCheckedLink(true);
    for (auto child : tileSprite->getTileChildren())
    {
        if (child->getIsCheckedLink())
            continue;
        
        auto iterator = std::find(linkedTileVector.begin(), linkedTileVector.end(), child);
        if (iterator == linkedTileVector.end())
        {
            linkedTileVector.push_back(child);
        }

        child->setIsCheckedLink(true);
        recursiveFindLinkedTileSprite(linkedTileVector, child);
    }
}

#pragma mark- Touch Actions

bool JigsawPuzzleScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (_isAnimating) {
        return false;
    }
    
    _isScrollingTableView = false;
    
    Point point = touch->getLocation();
    for (auto cell : _visibleCells)
    {
        JigsawTileSprite *tileSprite = nullptr;
        if (cell->getChildren().size() > 0)
            tileSprite = static_cast<JigsawTileSprite *>(cell->getChildren().at(0));
        else
            continue;
        
        Rect boudingbox;
        boudingbox.size = tileSprite->getContentSize();
        if (boudingbox.containsPoint(tileSprite->convertToNodeSpace(point)))
        {
            _isTouchInTableTileSprite = true;
            _touchedTableTileSprite = tileSprite;
            return true;
        }
    }
    
    if (!_isTouchInTableTileSprite)
    {
        for (auto tileSprite : _containerTileSpritesVector)
        {
            if (tileSprite->getIsFixed()) {
                continue;
            }
            
            Rect boudingbox;
            boudingbox.size = tileSprite->getContentSize();
            if (boudingbox.containsPoint(tileSprite->convertToNodeSpace(point)))
            {
                _isTouchInContainerTileSprite = true;
                _isMovingContainerTileSprite = true;
                _touchedContainerTileSprite = tileSprite;
                
                findLinkedChildren(_touchedContainerTileSprite);
                for (auto linkedChild : _touchedContainerTileSprite->getLinkedTileChildren())
                {
                    linkedChild->setZOrder(TILE_SPRITE_TOUCHED_ZORDER);
                }
  
                _touchedContainerTileSprite->setZOrder(TILE_SPRITE_TOUCHED_ZORDER);

                return true;
            }
        }
    }

    return false;
}

void JigsawPuzzleScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (_isAnimating)
        return;
    
    if (_isTouchInTableTileSprite)
    {
        moveTableTileSprite(touch);
    }
    
    if (_isTouchInContainerTileSprite)
    {
        moveContainerTileSprite(touch);
    }
}

void JigsawPuzzleScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (_isMovingTableTileSprite && _touchedTableTileSprite)
    {
        endTouchTableTileSprite(touch);
    }
    else if (_isMovingContainerTileSprite && _touchedContainerTileSprite)
    {
        endTouchContainerTileSprite(touch);
    }
    else
    {
        restoreTouchState();
    }
}

void JigsawPuzzleScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    restoreTouchState();
}

#pragma mark- TableView delegate methods

ssize_t JigsawPuzzleScene::numberOfCellsInTableView(TableView *table)
{
    return _tileSpritesVector.size();
}

void JigsawPuzzleScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size JigsawPuzzleScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    auto tileSprite = _tileSpritesVector.at(idx);
    return Size(134.0, tileSprite->getContentSize().height * _jigsawToResScale + CELL_INTERVEL);
}

TableViewCell* JigsawPuzzleScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = TableViewCell::create();
    }
    
    cell->removeAllChildren();

    auto tileSprite = _tileSpritesVector.at(idx);
    if (tileSprite->getParent()) {
        tileSprite->removeFromParent();
    }
    cell->setContentSize(Size(134.0, tileSprite->getContentSize().height * _jigsawToResScale + CELL_INTERVEL));
    tileSprite->setPosition(cell->getContentSize()/2);
    tileSprite->setScale(_jigsawToResScale * TILE_SCALE_IN_CELL);
    cell->addChild(tileSprite);
    
    _visibleCells.pushBack(cell);
    
    return cell;
}

void JigsawPuzzleScene::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    auto iterator = std::find(_visibleCells.begin(), _visibleCells.end(), cell);
    
    if (iterator != _visibleCells.end())
        _visibleCells.erase(iterator);
}

#pragma mark- STIABDelegate

void JigsawPuzzleScene::purchaseSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void JigsawPuzzleScene::purchaseFailed(const char *pid, int errorCode)
{
    
}

void JigsawPuzzleScene::restoreSuccessful(const char* pid)
{
    if (std::string(pid) == IAP_UNLOCK_All)
    {
        std::string userDefault = UserDefaultKey_HasPurchasedItem_ + std::string(pid);
        UserDefault::getInstance()->setBoolForKey(userDefault.c_str(), true);
        UserDefault::getInstance()->flush();
        
        AppManager::getInstance()->setHasUnlockAll(true);
    }
}

void JigsawPuzzleScene::restoreFailed(const char* pid, int errorCode)
{
    
}







