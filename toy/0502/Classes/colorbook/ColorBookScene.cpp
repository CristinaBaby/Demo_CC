//
//  ColorBookScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#include "ColorBookScene.h"
#include "../AppGlobal.h"
#include "../utilities/STVisibleRect.h"
#include "../utilities/UICpp.h"
#include "../utilities/scribble/ScribblePatternNode.h"
#include "../utilities/scribble/ScribbleUtil.h"
#include "../modules/STSystemFunction.h"
#include "../modules/AnalyticX.h"
#include "../gameconfig/MyGameConfig.h"
#include "../helpers/AudioHelper.h"
#include "../helpers/AdHelper.h"
   
#define EXIT_DIALOG_TAG 700

#define Tmp_PictureName "tmp_picture.png"

//tag
enum UI_TAG{
    ui01_play = 8,
    ui01_policy = 10,
};

enum class ZORDERS
{
    SETTING_ZORDER = 100,
    SAVE_ZORDER = 100,
    SETTING_LAYER_ZORDER = 101,
    SAVING_LAYER_ZORDER = 101,
    PENS_LAYER_ZORDER = 99,
    
    MASK_ZORDER = 1000,
    PROGRESS_LAYER_ZORDER = 1001
};


Scene* ColorBookScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = ColorBookScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool ColorBookScene::init()
{
    if (Layer::init()) {
        //////////////////////////////////////////////////////////// cocostudio ////////////////////////////////////////////////////////////
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui05_colourbook.csb"));
        addChild(_root);
        
        _popup = (Layout*)Helper::seekWidgetByName(_root, "ui06-abs");
        _popup->setVisible(false);
        _popup->setContentSize(V::getRealDesignSize());
        
        getImagePath();
        
        _pictureHistoryPath = FileUtils::getInstance()->getWritablePath() + _appendHistoryPath;
        
        bool isExsit = FileUtils::getInstance()->isFileExist(_pictureHistoryPath);
        
        if (isExsit)
        {
            xDialog->showContinue("Do you wish to continue your previous painting?", CC_CALLBACK_1(ColorBookScene::onDialog, this), this);
        }
        else
        {
            //进度条读取
            xDialog->showProgress(10, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);
        }
        
        return true;
    }
    
    return false;
}

ColorBookScene::~ColorBookScene()
{
    _eventDispatcher->removeEventListenersForTarget(this);
}

void ColorBookScene::onButton(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    xAudio->stopSound(_nSoundId);
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    xAudio->playDefaultSound();
    
    if (name.compare("ui04_eraser") == 0) {
        useEraser();
        
        if (_chooseBtn != nullptr)
        {
            _chooseBtn->stopAllActions();
            _chooseBtn->setPosition(_oldChoosePos);
        }
    }
    else if (name.compare("ui06_back_0") == 0) {
        
        if (_hasPaint) {
            generateCurrentPaintingPicture(Tmp_PictureName);
            
            _popup->setVisible(true);
            
            Sprite * sprite = Sprite::create(Tmp_PictureName);
            sprite->setScale(_image->getContentSize().width * 0.96/ sprite->getContentSize().width);
            sprite->setPosition(_image->getContentSize() / 2 );
            _image->addChild(sprite);
        }
        else
        {
            finishSaveImage();
        }
    }
    else if (name.compare("ui07_btn_quit") == 0) {
        disableAllButtons();
        _pictureScribble->getCanvas()->saveToFile(_appendHistoryPath);
    }
    else if (name.compare("ui07_btn_replay") == 0) {            //名字叫replay, 其实是save and quit
        disableAllButtons();
        STSystemFunction function;
        function.saveLocalImage(_pictureImage);
        
        _pictureScribble->getCanvas()->saveToFile(_appendHistoryPath);
        log("%s", _appendHistoryPath.c_str());
    }
    else if (name.compare("ui07_close") == 0) {
        _popup->setVisible(false);
    }
    
    //换笔
    if (name.find("ui04_pen_") != string::npos) {
        
        if (_chooseBtn != widget) {
            //先把之前的笔放回去
            if (_chooseBtn != nullptr)
            {
                _chooseBtn->stopAllActions();
                _chooseBtn->setPosition(_oldChoosePos);
            }
            
            _chooseBtn = widget;
            _oldChoosePos = _chooseBtn->getPosition();
            _chooseBtn->runAction(MoveBy::create(0.3, Vec2(-60, 0)));
        }
        
        for (int i = 0; i < (*_pen).size(); i++) {
            auto value = (*_pen).at(i);
            auto vm = value.asValueMap();
            string logo = vm["Logo"].asString();
            
            string logoName = name + ".png";
            
            if (logo.compare(name + ".png") == 0) {
                changePen(vm);
            }
        }
    }
    
    //画板
    if (name.find("ui04_pattern_") != string::npos) {
        
        changePattern(name);
        
        for (int i = 24; i < _btnPen.size(); i++) {
            auto pen = _btnPen.at(i);
            
            if (name.compare(pen->getName()) == 0) {
                if (_chooseBtn != widget) {
                    //先把之前的笔放回去
                    if (_chooseBtn != nullptr)
                    {
                        _chooseBtn->stopAllActions();
                        _chooseBtn->setPosition(_oldChoosePos);
                    }
                    
                    _chooseBtn = widget;
                    _oldChoosePos = _chooseBtn->getPosition();
                    _chooseBtn->runAction(MoveBy::create(0.3, Vec2(-60, 0)));
                }
            }
        }
    }
}

void ColorBookScene::disableAllButtons()
{
    quickSetEnabled("ui07_close", _root, false);
    quickSetEnabled("ui07_btn_quit", _root, false);
    quickSetEnabled("ui07_btn_replay", _root, false);
}

void ColorBookScene::finishSaveImage()
{
    xScene->back();
}

void ColorBookScene::onCheckBox(Ref* sender,CheckBox::EventType type)
{
    if (type != CheckBox::EventType::SELECTED)
    {
        CheckBox * check = (CheckBox*)sender;
        check->setSelectedState(true);
        return;
    }
    
    auto widget = (Widget*)sender;
    const string& name = widget->getName();
    
    _chkPen->setSelectedState(false);
    _chkMorePen->setSelectedState(false);
    _chkPattern->setSelectedState(false);
    
    _layoutPen->setVisible(false);
    _layoutMorePen->setVisible(false);
    _layoutPattern->setVisible(false);
    
    int index = 0;
    //不选择其它checkbox, 显示当前layout, 隐藏其它layout, 不处理取消选中
    if (name.compare("ui04_btn_pen") == 0) {
        _chkPen->setSelectedState(true);
        _layoutPen->setVisible(true);
        _pen = &_pen1;
        
        auto value = (*_pen).at(0);
        auto vm = value.asValueMap();
        changePen(vm);
        
        index = 0;
    }
    else if (name.compare("ui04_btn_morepen") == 0) {
        _chkMorePen->setSelectedState(true);
        _layoutMorePen->setVisible(true);
        _pen = &_pen2;
        
        auto value = (*_pen).at(0);
        auto vm = value.asValueMap();
        changePen(vm);
        
        index = 12;
    }
    else if (name.compare("ui04_btn_pattern_h") == 0) {
        _chkPattern->setSelectedState(true);
        _layoutPattern->setVisible(true);
        
        changePattern("pattern_1");
        
        index = 24;
    }
    
    widget = _btnPen.at(index);
    if (_chooseBtn != widget) {
        //先把之前的笔放回去
        if (_chooseBtn != nullptr)
        {
            _chooseBtn->stopAllActions();
            _chooseBtn->setPosition(_oldChoosePos);
        }
        
        _chooseBtn = widget;
        _oldChoosePos = _chooseBtn->getPosition();
        _chooseBtn->setPositionX(_oldChoosePos.x -60) ;
    }

}

void ColorBookScene::onExit()
{
    Layer::onExit();
}

void ColorBookScene::initTouches(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ColorBookScene::touchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ColorBookScene::touchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ColorBookScene::touchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ColorBookScene::touchCancelled, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ColorBookScene::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = this->convertToWorldSpace(pTouch->getLocation());
    
    Vec2 lPointInPattern = _picture->convertToNodeSpace(lTouchPoint);
    
    Color4B lColor = ScribbleUtil::getColor(_patternImage, lPointInPattern);
    Color4F lTouchedColor(lColor);
    
    _pictureScribble->setPatternColor(lTouchedColor);
    
    _pictureScribble->paint(lTouchPoint);
    
    _nSoundId = xAudio->playSound("sfx_color_brushing.mp3", true);
    
    _hasPaint = true;
    
    return true;
}

void ColorBookScene::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    Point lTouchPoint = this->convertToWorldSpace(pTouch->getLocation());
    Point lPreTouchPoint = this->convertToWorldSpace(pTouch->getPreviousLocation());
    _pictureScribble->paint(lTouchPoint, lPreTouchPoint);
}

void ColorBookScene::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    xAudio->stopSound(_nSoundId);
}


void ColorBookScene::touchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    xAudio->stopSound(_nSoundId);
}


void ColorBookScene::initWireframe(){
#if 0
    _mask = Sprite::create(_pictureMaskPath);
    _mask->setPosition(_colouringLayer->getContentSize() / 2);
    _colouringLayer->addChild(_mask, 2);
#endif
    _picture = Sprite::create(_picturePath);     //_picturePath
    _colouringLayer->addChild(_picture, 3);
    _picture->setVisible(true);
    _picture->setPosition(_colouringLayer->getContentSize() / 2);
}


void ColorBookScene::initPattern(){
//    _picture = Sprite::create(_picturePath);
//    _picture->setPosition(V::getCenterOfScene());
    
    auto size = Director::getInstance()->getWinSize();
    _pictureScribble = ScribblePatternNode::create(size);       //_picture->getContentSize()
    _colouringLayer->addChild(_pictureScribble, 2);
    _pictureScribble->setPosition(_picture->getPosition());
}


void  ColorBookScene::savePicture(const std::string& pictureName)
{
    _pictureScribble->getCanvas()->saveToFile(_appendHistoryPath);
}

void ColorBookScene::changePen(ValueMap &vm)
{
    float r = vm["R"].asFloat();
    float g = vm["G"].asFloat();
    float b = vm["B"].asFloat();
    
    _pictureScribble->useTarget(Color4F(r/255, g/255, b/255, 1));
}

void ColorBookScene::changePattern(const string& name)
{
    auto pattern = Sprite::create("pattern/" + name.substr(name.find("pattern")) + ".jpg");
    _pictureScribble->useTarget(pattern);
}

void ColorBookScene::useEraser()
{
    _pictureScribble->useTarget(Color4F(0, 0, 0, 0));
}

//保存历史图片
void ColorBookScene::generateCurrentPaintingPicture(const std::string& pictureName)
{                             
    Vec2 real = V::getRealPositionForDesignPosition(Vec2(960 - 223, 0));
    
    auto renderTexture = RenderTexture::create(real.x, _colouringLayer->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->beginWithClear(1.0, 1.0, 1.0, 1.0);
    _colouringLayer->visit();
    renderTexture->end();
    
    Director::getInstance()->getRenderer()->render();
    
    _pictureImage = renderTexture->newImage();
    
    /* used in saving layer */
    Director::getInstance()->getTextureCache()->removeTextureForKey(pictureName);
    Director::getInstance()->getTextureCache()->addImage(_pictureImage, pictureName);
//    _pictureImage->retain();
}

void ColorBookScene::getImagePath()
{
    int pictureIndex = xGame->getPictureIndex() + 1;

    int add = 0;
    int pack = 1;
    
    int old = add;
    while (add < pictureIndex) {
        old = add;
        add += getPackImageCount(pack++);
    }
    
    if (pack > 1) {
        pack--;
    }
    
    char path[100];
    sprintf(path, "Pack%d/L%02d_picture.png", pack, pictureIndex - old);
    _picturePath = path;
    sprintf(path, "Pack%d/L%02d_mask.png", pack, pictureIndex - old);
    _pictureMaskPath = path;
    sprintf(path, "L%02d_history.png", pictureIndex);
    _appendHistoryPath = path;
}


void ColorBookScene::onDialog(const string& name)
{
    if (name.compare("ui07_btn_next") == 0) {
        //这是new
        _continue = false;
        
        if(remove(_pictureHistoryPath.c_str()) != 0 )
        {
            log( "Error deleting file" );
        }
    }
    else if (name.compare("ui07_btn_continue") == 0) {
        _continue = true;
    }
    
    //进度条读取
    xDialog->showProgress(10, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);
}

void ColorBookScene::onProgress()
{
    int percent = xDialog->getPercent();
    
    switch (percent) {
        case 10:
        {
            quickButton("ui06_back_0", _root, CC_CALLBACK_2(ColorBookScene::onButton, this));
            quickButton("ui07_btn_quit", _root, CC_CALLBACK_2(ColorBookScene::onButton, this));
            quickButton("ui07_btn_replay", _root, CC_CALLBACK_2(ColorBookScene::onButton, this));
            quickButton("ui04_eraser", _root, CC_CALLBACK_2(ColorBookScene::onButton, this));
            quickButton("ui07_close", _root, CC_CALLBACK_2(ColorBookScene::onButton, this));
            
            //checkbox
            _chkPen = quickCheckBox("ui04_btn_pen", _root, CC_CALLBACK_2(ColorBookScene::onCheckBox, this));
            _chkPen->setSelectedState(true);
            _chkMorePen = quickCheckBox("ui04_btn_morepen", _root, CC_CALLBACK_2(ColorBookScene::onCheckBox, this));
            _chkPattern = quickCheckBox("ui04_btn_pattern_h", _root, CC_CALLBACK_2(ColorBookScene::onCheckBox, this));
            
            _layoutPen = (Layout*)Helper::seekWidgetByName(_root, "pen");
            _layoutMorePen = (Layout*)Helper::seekWidgetByName(_root, "morepen");
            _layoutPattern = (Layout*)Helper::seekWidgetByName(_root, "pattern");
            
            _image = (ImageView*)Helper::seekWidgetByName(_root, "Image_100");
            
            //////////////////////////////////////////////////////////// plist ////////////////////////////////////////////////////////////
            
            auto valueMap = FileUtils::getInstance()->getValueMapFromFile("colorbook/pens1.plist");
            _pen1 = valueMap["Pens"].asValueVector();
            valueMap = FileUtils::getInstance()->getValueMapFromFile("colorbook/pens2.plist");
            _pen2 = valueMap["Pens"].asValueVector();
            _pen = &_pen1;
            
            for (int i = 1; i <= 24; i++) {
                string name = "ui04_pen_" + std::to_string(i);
                
                Button *btn = (Button*)Helper::seekWidgetByName(_root, name);
                btn->setTouchEnabled(true);
                btn->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::onButton, this));
                
                _btnPen.push_back(btn);
                _btnPos.push_back(btn->getPosition());
            }
            
            auto widget = _btnPen.at(0);
            if (_chooseBtn != widget) {
                _chooseBtn = widget;
                _oldChoosePos = _chooseBtn->getPosition();
                _chooseBtn->setPositionX(_oldChoosePos.x -60);
            }
            
            //画板
            for (int i = 1; i <= 12; i++)
            {
                string name = "ui04_pattern_" + std::to_string(i);
                Button *btn = (Button*)Helper::seekWidgetByName(_root, name);
                btn->setTouchEnabled(true);
                btn->addTouchEventListener(CC_CALLBACK_2(ColorBookScene::onButton, this));
                
                _btnPen.push_back(btn);
                _btnPos.push_back(btn->getPosition());
            }
            
            xDialog->showProgress(30, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);
        }
            break;
        case 30:
        {
            auto bg = (Layout*)Helper::seekWidgetByName(_root, "bg");
            
            //add colouring layer
            _colouringLayer = Layer::create();
            bg->addChild(_colouringLayer, 0);
            
            initWireframe();
            initPattern();
            
            xDialog->showProgress(60, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);

        }
            break;
        case 60:
        {
            //check sandbox history
            Director::getInstance()->getTextureCache()->removeTextureForKey(_appendHistoryPath);
            Director::getInstance()->getTextureCache()->removeTextureForKey(_pictureHistoryPath);
            if (_continue)
            {
                Director::getInstance()->getTextureCache()->addImage(_pictureHistoryPath);
                
                _pictureScribble->setPattern(_pictureHistoryPath);
                
                Sprite *history = Sprite::create(_pictureHistoryPath);
                auto size = Director::getInstance()->getWinSize();
                history->setPosition(size / 2);
                
                _pictureScribble->getCanvas()->begin();
                history->visit();
                _pictureScribble->getCanvas()->end();
                
                _finishLoadHistory = true;
            }
            
            xDialog->showProgress(80, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);

        }
            break;
        case 80:
        {
            _pictureScribble->setPattern(_pictureMaskPath);
            
            _pictureScribble->useBrush("pattern/solid_brush.png");//指定用于绘制的画刷。
            
            _pictureScribble->useTarget(Sprite::create(_pictureMaskPath));                      //至少有2次useTarget, 一次是设置颜色, 一次是设置mask
            
            auto value = (_pen1).at(0);
            auto vm = value.asValueMap();
            changePen(vm);
            
            
            CC_SAFE_DELETE(_patternImage);
            _patternImage = new Image();
            _patternImage->initWithImageFile(_pictureMaskPath);        //_pictureMaskPath
            
            initTouches();
            
            quickAddEvent(EVENT_SAVE_IMAGE, CC_CALLBACK_0(ColorBookScene::finishSaveImage, this), this);
            
            xDialog->showProgress(100, CC_CALLBACK_0(ColorBookScene::onProgress, this), this);
        }
            break;
        case 100:
        {
            xDialog->hide();
        }
            break;
        default:
            break;
    }
}


int ColorBookScene::getPackImageCount(int pack)
{
    int imageCount = 0;
    switch (pack) {
        case 1:
            imageCount = 6;
            break;
        case 2:
            imageCount = 6;
            break;
        case 3:
            imageCount = 6;
            break;
        case 4:
            imageCount = 6;
            break;
        case 5:
            imageCount = 6;
            break;
        default:
            break;
    }
    
    return imageCount;
}



