#include "Dialog.h"
#include "Global.h"
#include "AdapterScreen.h"
#include "SoundPlayer.h"

USING_NS_CC_EXT;


Dialog::Dialog():m_pContentLayer(nullptr),m_pContentLabel(nullptr),m_pPositiveLabel(nullptr),m_pNegativeLabel(nullptr),callback(nullptr),m_pBg(nullptr),m_fScaleX(0.7f),m_fScaleY(0.3f),m_bIsImage(false),m_pImage(nullptr),m_pPositive(nullptr),m_pNegative(nullptr),m_bIsNeedTitle(false),m_pTitle(nullptr)
{
    
}

Dialog::~Dialog()
{
    
}

Dialog* Dialog::create(void* _args, int type, DialogDirection direction, float scaleX, float scaleY, bool isImage, Sprite* image)
{
    Dialog* dialog = new Dialog();
    
    if (dialog) {
        
        dialog->autorelease();
        
        dialog->dialogType = type;
        
        dialog->direction = direction;
        
        dialog->args = _args;
        
        dialog->m_fScaleX = scaleX;
        dialog->m_fScaleY = scaleY;
        dialog->m_bIsImage = isImage;
        dialog->m_pImage = image;
        
        dialog->init();
        
        return dialog;
    }
    
    CC_SAFE_DELETE(dialog);
    return nullptr;
}
bool Dialog::init()
{
    if (LayerColor::initWithColor(Color4B(20, 20, 20, 200)))
    {
        ////////////////////////////////////////////////////////////////////
        // add bg
        m_pContentLayer = LayerColor::create(Color4B(0, 0, 0, 0));
        m_pContentLayer->ignoreAnchorPointForPosition(false);
        m_pContentLayer->setContentSize(Director::getInstance()->getVisibleSize());
        if(direction == HORIZONTAL)
            kAdapterScreen->setExactPosition(m_pContentLayer, 480, 320);
        else
            kAdapterScreen->setExactPosition(m_pContentLayer, 320, 480);
        this->addChild(m_pContentLayer);
        
        
        // add background
        m_pBg = Sprite::create("popup_bg.png");
        if(direction == HORIZONTAL)
            kAdapterScreen->setExactPosition(m_pBg, 480, 320);
        else
            kAdapterScreen->setExactPosition(m_pBg, 320, 540);
        m_pContentLayer->addChild(m_pBg);

        // add content
        if (m_bIsImage) {
            if (m_pImage){
                m_pImage->setPosition(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height / 2 + 50);
                m_pBg->addChild(m_pImage);
            }
        }else{
            m_pContentLabel = Label::createWithTTF("",font_felt.c_str(), 29);
            m_pContentLabel->setDimensions(m_pBg->getContentSize().width * m_fScaleX, m_pBg->getContentSize().height * m_fScaleY);
            m_pContentLabel->setAnchorPoint(Point(0.5f, 0.35f));
            m_pContentLabel->setPosition(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height / 2 + 150);
            m_pContentLabel->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
            m_pBg->addChild(m_pContentLabel);
        }

        ////////////////////////////////////////////////////////////////////
        auto pYesBtn = Sprite::create("popup_btn_yes.png");
        auto pYesBtn_h = Sprite::create("popup_btn_yes.png");
        pYesBtn_h->setColor(Color3B(pYesBtn_h->getColor().r / 2, pYesBtn_h->getColor().g / 2, pYesBtn_h->getColor().b / 2));
        
        // create positive button
        m_pPositive = MenuItemSprite::create(pYesBtn, pYesBtn_h, CC_CALLBACK_1(Dialog::onClick, this));
        m_pPositive->setUserData((void*)99);
        m_pPositive->setAnchorPoint(Point(0.5f, 0));
        
        if (!m_bIsImage) {
            m_pPositiveLabel = Label::createWithTTF("",font_felt.c_str(), 30);
            m_pPositiveLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            m_pPositiveLabel->setDimensions(m_pPositive->getContentSize().width, m_pPositive->getContentSize().height);
            m_pPositiveLabel->setAnchorPoint(Point(0.5f, 0.5f));
            m_pPositive->addChild(m_pPositiveLabel);
        }

        auto pNoBtn = Sprite::create("popup_btn_no.png");
        auto pNoBtn_h = Sprite::create("popup_btn_no.png");
        pNoBtn_h->setColor(Color3B(pNoBtn_h->getColor().r / 2, pNoBtn_h->getColor().g / 2, pNoBtn_h->getColor().b / 2));
        
        auto pOkBtn = Sprite::create("popup_btn_ok.png");
        auto pOkBtn_h = Sprite::create("popup_btn_ok.png");
        pOkBtn_h->setColor(Color3B(pOkBtn_h->getColor().r / 2, pOkBtn_h->getColor().g / 2, pOkBtn_h->getColor().b / 2));
        // create negative button
        if(dialogType == DIALOG_TYPE_SINGLE)
        {
            m_pNegative= MenuItemSprite::create(pOkBtn, pOkBtn_h, CC_CALLBACK_1(Dialog::onClick, this));
        }
        else
        {
            m_pNegative = MenuItemSprite::create(pNoBtn, pNoBtn_h, CC_CALLBACK_1(Dialog::onClick, this));
        }
        m_pNegative->setUserData((void*)1);
        m_pNegative->setAnchorPoint(Point(0.5, 0));
        
        if (!m_bIsImage) {
            m_pNegativeLabel = Label::createWithTTF("", font_felt.c_str(), 30);
            m_pNegativeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            m_pNegativeLabel->setDimensions(m_pNegative->getContentSize().width, m_pNegative->getContentSize().height);
            m_pNegativeLabel->setAnchorPoint(Point(0.5f, 0.5f));
            m_pNegative->addChild(m_pNegativeLabel);
        }

        // create menu
        if(dialogType == DIALOG_TYPE_SINGLE)
        {
            mMenu = Menu::create(m_pNegative,  nullptr);
        }else
        {
            mMenu = Menu::create(m_pNegative, m_pPositive, nullptr);
        }
        mMenu->alignItemsHorizontallyWithPadding(25);
        mMenu->setPosition(Point(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height * 0.1 + 10));
        m_pBg->addChild(mMenu, INT16_MAX);
        
        
        // registe touch event
        {
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [=](Touch* touch, Event* event){
                return true;
            };
            listener->onTouchMoved = [=](Touch* touch, Event* event){
                
            };
            listener->onTouchEnded = [=](Touch* touch, Event* event){
                
            };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        }
        
        m_pBg->setScale(0);
        m_pBg->runAction(Sequence::create(ScaleTo::create(0.2f, 1.15f), ScaleTo::create(0.25f, 1.0f), nullptr));
        
        return true;
    }
    
    
    return false;
}

void Dialog::setCallback(DialogCallback* _callback)
{
    callback=_callback;
}

void Dialog::onBack()
{
    getParent()->removeChild(this, true);
    
    CCLOG("onBack -> %d", m_FuncType);
    
    if(callback != nullptr)
        callback->onPositiveClick(args);
}

void Dialog::dismiss()
{
//    this->removeFromParent();
    getParent()->removeChild(this);
    
    CCLOG("dismiss -> %d", m_FuncType);
    
    if(callback != nullptr)
        callback->onNegativeClick(args);
}

void Dialog::onClick(Ref* obj)
{
    if(!mMenu->isEnabled())
        return;
    
    SoundPlayer::getInstance()->playEffect("sounds/all_button_sound.mp3");
    mMenu->setEnabled(false);
    MenuItemSprite* btn = (MenuItemSprite*)obj;
    
    int type = (uintptr_t)btn->getUserData();
    if(type == 1)
    {
        FadeOut* lFade = FadeOut::create(0.4f);
        ScaleTo* lScale = ScaleTo::create(0.4f, 0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, nullptr);
        DelayTime* lDelay = DelayTime::create(0.1f);
        
        FiniteTimeAction* action = Sequence::create(lSpawn, lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::dismiss, this)), nullptr);
        
        m_pContentLayer->runAction(action);
        
    }
    else
    {
        FadeOut* lFade = FadeOut::create(0.4f);
        ScaleTo* lScale = ScaleTo::create(0.4f, 0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, nullptr);
        DelayTime* lDelay = DelayTime::create(0.1f);
        FiniteTimeAction* action = Sequence::create(lSpawn, lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::onBack, this)) , nullptr);
        
        m_pContentLayer->runAction(action);
    }
}

void Dialog::onEnter()
{
    LayerColor::onEnter();
}

void Dialog::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    
    m_pContentLayer->setScale(1.0);

    refreshPosition();
}

void Dialog::onExit()
{
    LayerColor::onExit();
}

void Dialog::setPositiveBtnText(const char* text)
{
    m_pPositiveLabel->setString(text);
}

void Dialog::setNegativeBtnText(const char* text)
{
    m_pNegativeLabel->setString(text);
}

void Dialog::setPositiveBtnImage(const std::string &filename)
{
    m_pPositive->setNormalImage(Sprite::create(filename));
    m_pPositive->setSelectedImage(Sprite::create(filename));
}

void Dialog::setNegativeBtnImage(const std::string &filename)
{
    m_pNegative->setNormalImage(Sprite::create(filename));
    m_pNegative->setSelectedImage(Sprite::create(filename));
}

void Dialog::setTitle(const std::string &title)
{
    if (!m_pTitle) {
        m_pTitle = Sprite::create(title);
        m_pTitle->setPosition(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height);
        m_pBg->addChild(m_pTitle);
    }else{
        m_pTitle->setTexture(title);
    }
}

void Dialog::setContentText(const char* text)
{
    if (m_bIsImage)
        return;
    
    m_pContentLabel->setString(text);
    
    refreshPosition();
}
void Dialog::setContentTextColor(const Color3B &color)
{
    if (m_bIsImage)
        return;
    
    m_pContentLabel->setColor(color);
}

void Dialog::refreshPosition()
{
    if (m_bIsImage)
        return;
    
    m_pContentLabel->setPosition(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height / 2 + 8);
}
