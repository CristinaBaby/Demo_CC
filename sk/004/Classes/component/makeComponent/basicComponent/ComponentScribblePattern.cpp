//
//  ComponentScribblePattern.cpp
//
//
//  Created by maxiang on 4/20/16.
//
//

#include "ComponentScribblePattern.h"

ComponentScribblePattern *ComponentScribblePattern::create(const std::string& maskFile)
{
    auto ret = new (std::nothrow) ComponentScribblePattern();
    if (ret)
    {
        ret->init(maskFile);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ComponentScribblePattern::ComponentScribblePattern():
_eventListenerTouchOneByOne(nullptr),
_patternMaskImage(nullptr),
_pattern(nullptr),
_scribblePattern(nullptr),
_brushFile("\0"),
_isTakeColor(false),
_isColouring(false),
_scribbleToolSprite(nullptr)
{
    
}

ComponentScribblePattern::~ComponentScribblePattern()
{
    removeTouchEventListener();
    CC_SAFE_RELEASE_NULL(_patternMaskImage);
    CC_SAFE_RELEASE_NULL(_patternMaskSprite);
    CC_SAFE_RELEASE_NULL(_scribblePattern);
    
    if (_scribbleToolSprite)
        _scribbleToolSprite->removeFromParent();
}

bool ComponentScribblePattern::init(const std::string& maskFile)
{
    if (!ComponentBase::init())
        return false;
    
    _patternMaskImage = new Image();
    bool success = _patternMaskImage->initWithImageFile(maskFile);
    CCASSERT(success, "* ComponentScribblePattern: mask创建失败! *");
    
    //用于检测触摸区域
    _patternMaskSprite = Sprite::create(maskFile);
    _patternMaskSprite->setVisible(false);
    _patternMaskSprite->retain();

    _scribblePattern = ScribblePatternNode::create(Size(_patternMaskImage->getWidth(), _patternMaskImage->getHeight()));
    _scribblePattern->setPattern(maskFile);
    _scribblePattern->retain();
    
    return true;
}

void ComponentScribblePattern::setMask(const std::string& file)
{
    if (_patternMaskImage)
    {
        delete _patternMaskImage;
        _patternMaskImage = nullptr;
    }
    _patternMaskImage = new Image();
    _patternMaskImage->initWithImageFile(file);
    
    _patternMaskSprite->removeFromParent();
    CC_SAFE_RELEASE_NULL(_patternMaskSprite);
    _patternMaskSprite = Sprite::create(file);
    _patternMaskSprite->setVisible(false);
    _patternMaskSprite->retain();
    _owner->addChild(_patternMaskSprite, 2);

    _scribblePattern->setPattern(file);
}

void ComponentScribblePattern::setBrush(const std::string& brushFile)
{
    _scribblePattern->useBrush(brushFile);
}

void ComponentScribblePattern::setScribbleTool(const std::string& toolFile,
                                               const Vec2& position,
                                               Node *parent)
{
    if (!_scribbleToolSprite)
    {
        _scribbleToolSprite = Sprite::create(toolFile);
        _scribbleToolSprite->setPosition(position);
        parent->addChild(_scribbleToolSprite, 20);
    }
    else
    {
        _scribbleToolSprite->setTexture(toolFile);
    }
}

void ComponentScribblePattern::showScribbleTool(const Vec2& toPosition)
{
    if (!_scribbleToolSprite)
        return;
    
    _scribbleToolSprite->runAction(MoveTo::create(1.0, toPosition));
}

void ComponentScribblePattern::setPattern(Sprite *pattern)
{
    CCASSERT(pattern, "* ComponentScribblePattern: pattern不能为空 *");
    
    _scribblePattern->useTarget(pattern);
}

void ComponentScribblePattern::start()
{
    ComponentBase::start();
    
    addTouchEventListener();
}

void ComponentScribblePattern::stop()
{
    removeTouchEventListener();
    
    ComponentBase::stop();
}

void ComponentScribblePattern::onAdd()
{
    ComponentBase::onAdd();

    _owner->addChild(_scribblePattern, 2);
    _owner->addChild(_patternMaskSprite, 2);
}

void ComponentScribblePattern::addTouchEventListener()
{
    if (_eventListenerTouchOneByOne || !getOwner())
        return;
    
    _eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
    _eventListenerTouchOneByOne->setSwallowTouches(false);
    _eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(ComponentScribblePattern::onTouchBegan, this);
    _eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(ComponentScribblePattern::onTouchMoved, this);
    _eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(ComponentScribblePattern::onTouchEnded, this);
    _eventListenerTouchOneByOne->onTouchCancelled = CC_CALLBACK_2(ComponentScribblePattern::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_eventListenerTouchOneByOne, _owner);
}

void ComponentScribblePattern::removeTouchEventListener()
{
    if (_eventListenerTouchOneByOne)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListenerTouchOneByOne);
        _eventListenerTouchOneByOne = nullptr;
    }
}

Sprite *ComponentScribblePattern::getSprite()
{
    return _scribblePattern->getCanvas()->getSprite();
}

bool ComponentScribblePattern::onTouchBegan(Touch* touch, Event* event)
{
    if (!_enabled)
        return false;
    
    //如果有涂抹工具，则以拖动涂抹工具为准
    if (_scribbleToolSprite)
    {
        Rect boundingbox = Rect(0.0, 0.0, _scribbleToolSprite->getContentSize().width, _scribbleToolSprite->getContentSize().height);
        
        _touchInToolLocation = _scribbleToolSprite->convertTouchToNodeSpace(touch);
        if (boundingbox.containsPoint(_touchInToolLocation))
        {
            Vec2 touchInPicture = _patternMaskSprite->convertToNodeSpace(_scribbleToolSprite->convertToWorldSpace(Vec2::ZERO));
            Color4B color = ScribbleUtil::getInstance()->getColor(_patternMaskImage, touchInPicture);
            
            if (color.a != 0)
            {
                _isTakeColor = true;
                _isColouring = true;
                
                _scribblePattern->setPatternColor(Color4F(color));
                _scribblePattern->paint(touch->getLocation());
                
                if (touchBeganCall)
                    touchBeganCall(touch);
            }
            
            return true;
        }
    }
    else
    {
        Vec2 touchInPicture = _patternMaskSprite->convertTouchToNodeSpace(touch);
        Color4B color = ScribbleUtil::getInstance()->getColor(_patternMaskImage, touchInPicture);
        
        if (color.a != 0)
        {
            _isTakeColor = true;
            _isColouring = true;
         
            _scribblePattern->setPatternColor(Color4F(color));
            _scribblePattern->paint(touch->getLocation() - _touchInToolLocation);
            
            if (touchBeganCall)
                touchBeganCall(touch);
            
            return true;
        }
    }
    
    return false;
}

void ComponentScribblePattern::onTouchMoved(Touch* touch, Event* event)
{
    if (!_isTakeColor)
    {
        Vec2 touchInPicture = _patternMaskSprite->convertToNodeSpace(_scribbleToolSprite->convertToWorldSpace(Vec2::ZERO));
        Color4B color = ScribbleUtil::getInstance()->getColor(_patternMaskImage, touchInPicture);
        
        if (color.a != 0)
        {
            _isTakeColor = true;
            _isColouring = true;
            
            _scribblePattern->setPatternColor(Color4F(color));
        }
    }
    
    if (_scribbleToolSprite){
        
        auto position = _scribbleToolSprite->getPosition() + touch->getDelta();
        auto boundingbox = Rect(60.0, 60.0, Director::getInstance()->getRunningScene()->getContentSize().width - 120.0, Director::getInstance()->getRunningScene()->getContentSize().height - 120.0);
        if (boundingbox.containsPoint(touch->getLocation())){
            _scribbleToolSprite->setPosition(position);
        }
    }
    
    if (_isColouring)
    {
        if (_scribbleToolSprite)
        {
            _scribblePattern->paint(touch->getLocation() - _touchInToolLocation, touch->getPreviousLocation() - _touchInToolLocation);
        }
        else
        {
            _scribblePattern->paint(touch->getLocation(), touch->getPreviousLocation());
        }
        
        if (touchMovedCall)
            touchMovedCall(touch);
    }
}

void ComponentScribblePattern::onTouchEnded(Touch* touch, Event* event)
{
    _isTakeColor = false;
    _isColouring = false;
    
    if (touchEndedCall)
        touchEndedCall(touch);
}

void ComponentScribblePattern::onTouchCancelled(Touch* touch, Event* event)
{
    _isTakeColor = false;
    _isColouring = false;
    
    if (touchCancelledCall)
        touchCancelledCall(touch);
}
