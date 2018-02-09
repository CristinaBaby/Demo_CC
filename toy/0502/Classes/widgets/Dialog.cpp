#include "Dialog.h"
#include "../utilities/UICpp.h"
#include "../helpers/AudioHelper.h"


#define kMyDialogZOrder             (100)

Dialog* Dialog::getInstance()
{
    static Dialog instance;
    //Dialog 自己不是autorelease的, 不用retain
    return &instance;
}

Dialog::Dialog()
{
    auto root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromBinaryFile("ui07_popup.csb"));

    _rootQuit = (Layout*)Helper::seekWidgetByName(root, "ui07_abs");
    _rootQuit->removeFromParent();
    //_labelQuit  = layout->clone(); //必须要clone, 不然加到Scene里面会崩溃
    quickButton("ui07_btn_yes", _rootQuit, CC_CALLBACK_2(Dialog::onButtonQuit, this));
    quickButton("ui07_close", _rootQuit, CC_CALLBACK_2(Dialog::onButtonQuit, this));
    _labelQuit = (Text*)Helper::seekWidgetByName(_rootQuit, "ui07_label_1");
    _labelQuit->setFontName("Tartine Script Black.ttf");
    _rootQuit->setContentSize(V::getRealDesignSize());
    _rootQuit->setVisible(true);
    _rootQuit->retain();
    
    _quitIcon = (ImageView*)Helper::seekWidgetByName(_rootQuit, "Image_12");
    

    _rootOver = (Layout*)Helper::seekWidgetByName(root, "ui07_abs1");
    _rootOver->removeFromParent();
//    layout  = layout->clone();
//    layout->setVisible(true);
    quickButton("ui07_btn_quit", _rootOver, CC_CALLBACK_2(Dialog::onButtonOver, this));
    quickButton("ui07_btn_replay", _rootOver, CC_CALLBACK_2(Dialog::onButtonOver, this));
    _labelOver = (Text*)Helper::seekWidgetByName(_rootOver, "ui07_label_2");
    _rootOver->setContentSize(V::getRealDesignSize());
    _rootOver->setVisible(true);
    _rootOver->retain();
    
    _rootContinue = (Layout*)Helper::seekWidgetByName(root, "ui07_abs3");
    _rootContinue->removeFromParent();
    quickButton("ui07_btn_next", _rootContinue, CC_CALLBACK_2(Dialog::onButtonContinue, this));
    quickButton("ui07_btn_continue", _rootContinue, CC_CALLBACK_2(Dialog::onButtonContinue, this));
    _labelContinue = (Text*)Helper::seekWidgetByName(_rootContinue, "ui07_label_2");
    _labelContinue->setFontName("Tartine Script Black.ttf");
    _rootContinue->setContentSize(V::getRealDesignSize());
    _rootContinue->setVisible(true);
    _rootContinue->retain();
    
    _rootExit = (Layout*)Helper::seekWidgetByName(root, "ui07_abs4");
    _rootExit->removeFromParent();
    quickButton("ui07_btn_yes", _rootExit, CC_CALLBACK_2(Dialog::onButtonQuit, this));
    quickButton("ui07_btn_no", _rootExit, CC_CALLBACK_2(Dialog::onButtonQuit, this));
    auto label = (Text*)Helper::seekWidgetByName(_rootExit, "ui07_label_2");
    label->setFontSize(40);
    label->setFontName("Tartine Script Black.ttf");
    _rootExit->setContentSize(V::getRealDesignSize());
    _rootExit->setVisible(true);
    _rootExit->retain();
    
    auto star = (CheckBox*)Helper::seekWidgetByName(_rootOver, "ui07_star_1");
    _stars.pushBack(star);
    star = (CheckBox*)Helper::seekWidgetByName(_rootOver, "ui07_star_2");
    _stars.pushBack(star);
    star = (CheckBox*)Helper::seekWidgetByName(_rootOver, "ui07_star_3");
    _stars.pushBack(star);
    
    _rootProgress = (Layout*)Helper::seekWidgetByName(root, "ui07_abs2");
    _rootProgress->removeFromParent();
    _rootProgress->setContentSize(V::getRealDesignSize());
    _rootProgress->setVisible(true);
    _rootProgress->retain();
    
    auto progressImage = (ImageView*)Helper::seekWidgetByName(_rootProgress, "ui07_saveing_a");
    
    _progressTimer = ProgressTimer::create(Sprite::create("ui07_popup/ui07_saveing_b.png"));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setBarChangeRate(Vec2(1, 0));
    _progressTimer->setMidpoint(Vec2(0, 1));
    _progressTimer->setPosition(progressImage->getContentSize() / 2);
    _progressTimer->setPercentage(0);
    progressImage->addChild(_progressTimer);
    
    V::setPosition(_rootQuit, Vec2((960 - _rootQuit->getContentSize().width)/2, (640 - _rootQuit->getContentSize().height) / 2));
    V::setPosition(_rootOver, Vec2((960 - _rootOver->getContentSize().width)/2, (640 - _rootOver->getContentSize().height) / 2));
    V::setPosition(_rootContinue, Vec2((960 - _rootContinue->getContentSize().width)/2, (640 - _rootContinue->getContentSize().height) / 2));
    V::setPosition(_rootProgress, Vec2((960 - _rootProgress->getContentSize().width)/2, (640 - _rootProgress->getContentSize().height) / 2));
}

void Dialog::showQuit(const string& text, ccDialogButtonCallback callback)
{
    if (_rootQuit->getParent() != nullptr) {
        return;
    }

    Scene *pRunScene = Director::getInstance()->getRunningScene();
    
    if (pRunScene) {
        _rootQuit->setVisible(true);
        _quitIcon->setVisible(true);
        _labelQuit->setString(text);
        _ccButtonCallback = callback;
        xAudio->playSound("sfx_popup.mp3");
        pRunScene->addChild(_rootQuit, kMyDialogZOrder);
    }
}

void Dialog::show(const string& text)
{
    if (_rootQuit->getParent() != nullptr) {
        return;
    }
    
    Scene *pRunScene = Director::getInstance()->getRunningScene();
    
    if (pRunScene) {
        _rootQuit->setVisible(true);
        _quitIcon->setVisible(false);
        _labelQuit->setString(text);
        xAudio->playSound("sfx_popup.mp3");
        pRunScene->addChild(_rootQuit, kMyDialogZOrder);
    }
}

void Dialog::showOver(const string& text, int starCount, ccDialogButtonCallback callback)
{
    if (_rootOver->getParent() != nullptr) {
        return;
    }
    
    Scene *pRunScene = Director::getInstance()->getRunningScene();
    
    if (pRunScene) {
        _rootOver->setVisible(true);
        _labelOver->setString(text);
        _ccButtonCallback = callback;
        
        for (int i = 0; i < 3; i ++) {
            if (starCount <= i) {
                _stars.at(i)->setSelectedState(true);
            }
            else
            {
                _stars.at(i)->setSelectedState(false);
            }
        }
        xAudio->playSound("sfx_gameover_win.mp3");
        pRunScene->addChild(_rootOver, kMyDialogZOrder);
    }
}


void Dialog::showProgress(float percent, ccDialogProgressCallback callback, Layer *layer)
{
    if (_rootProgress->getParent() != nullptr) {
        auto sequense = Sequence::create(ProgressTo::create(0.015 * percent / 10, percent), CallFunc::create(_ccProgressCallback), nullptr);
        _progressTimer->runAction(sequense);
        return;
    }
    
    if (layer) {
        _progressTimer->setPercentage(0);
        _ccProgressCallback = callback;
        auto sequense = Sequence::create(ProgressTo::create(0.015 * percent / 10, percent), CallFunc::create(_ccProgressCallback), nullptr);
        _progressTimer->runAction(sequense);
        layer->addChild(_rootProgress, kMyDialogZOrder);
    }
}

void Dialog::showContinue(const string& text, ccDialogButtonCallback callback, Layer *layer)
{
    if (_rootContinue->getParent() != nullptr) {
        return;
    }
    
    if (layer) {
        _ccButtonCallback = callback;
        _labelContinue->setString(text);
        xAudio->playSound("sfx_popup.mp3");
        layer->addChild(_rootContinue, kMyDialogZOrder);
    }
}

void Dialog::showExit(ccDialogButtonCallback callback, Layer *layer)
{
    if (_rootExit->getParent() != nullptr) {
        return;
    }
    
    if (layer) {
        _ccButtonCallback = callback;
        xAudio->playSound("sfx_popup.mp3");
        layer->addChild(_rootExit, kMyDialogZOrder);
    }
}

void Dialog::onButtonQuit(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    Widget *widget = (Widget*)pSender;
    const string& name = widget->getName();
    
    _rootQuit->removeFromParent();
    _rootExit->removeFromParent();
    
    if (_ccButtonCallback)
    {
        _ccButtonCallback(name);
    }
}

void Dialog::onButtonOver(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    Widget *widget = (Widget*)pSender;
    const string& name = widget->getName();
    
    if (name.compare("ui07_close") == 0)
    {
        _rootOver->removeFromParent();
    }
    
    if (_ccButtonCallback)
    {
        _ccButtonCallback(name);
    }
}

void Dialog::onButtonContinue(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    Widget *widget = (Widget*)pSender;
    const string& name = widget->getName();
    
    _rootContinue->removeFromParent();
    
    if (_ccButtonCallback)
    {
        _ccButtonCallback(name);
    }
}

void Dialog::hide()
{
    _rootQuit->removeFromParent();
    _rootOver->removeFromParent();
    _rootProgress->removeFromParent();
}

bool Dialog::isVisible()
{
    if (_rootOver->getParent() || _rootQuit->getParent()) {
        return true;
    }
    else
    {
        return false;
    }
}

float Dialog::getPercent()
{
    return _progressTimer->getPercentage();
}