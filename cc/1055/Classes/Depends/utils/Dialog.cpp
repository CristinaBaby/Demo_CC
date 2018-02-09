#include "Dialog.h"
#include "Global.h"
#include "AdapterScreen.h"

USING_NS_CC_EXT;


Dialog::Dialog():m_pContentLayer(nullptr),m_pContentLabel(nullptr),m_pPositiveLabel(nullptr),m_pNegativeLabel(nullptr),callback(nullptr),m_pBg(nullptr),m_fOffsetX(0),m_fOffsetY(0)
{
    
}

Dialog::~Dialog()
{
    
}

Dialog* Dialog::create(void* _args, int type, DialogDirection direction, float offsetX, float offsetY )
{
    Dialog* dialog = new Dialog();
    
    if (dialog) {
        
        dialog->autorelease();
        
        dialog->dialogType = type;
        
        dialog->direction = direction;
        
        dialog->args = _args;
        
        dialog->m_fOffsetX = offsetX;
        dialog->m_fOffsetY = offsetY;
        
        dialog->init();
        
        return dialog;
    }
    
    CC_SAFE_DELETE(dialog);
    return nullptr;
}
bool Dialog::init()
{
    if (LayerColor::initWithColor(Color4B(100, 100, 100, 150)))
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
        m_pBg = Sprite::create("cocostudio/UI/popup/popup-bg.png");
        if(direction == HORIZONTAL)
            kAdapterScreen->setExactPosition(m_pBg, 480, 320);
        else
            kAdapterScreen->setExactPosition(m_pBg, 320, 480);
        m_pContentLayer->addChild(m_pBg);
        
        
        // add content
        m_pContentLabel = Label::createWithTTF("",font_felt.c_str(), 29);
        m_pContentLabel->setDimensions(m_pBg->getContentSize().width * 0.65f, m_pBg->getContentSize().height * 0.3f);
        m_pContentLabel->setAnchorPoint(Point(0.5f, 0.5f));
        m_pContentLabel->setPosition(m_pBg->getContentSize().width / 2 + m_fOffsetX, m_pBg->getContentSize().height / 2 + 30 + m_fOffsetY);
        m_pBg->addChild(m_pContentLabel);
        m_pContentLabel->setTextColor(Color4B(196,54,4,255));

        ////////////////////////////////////////////////////////////////////
        // create positive button
        MenuItemSprite* positive = MenuItemSprite::create(Sprite::create("cocostudio/UI/popup/yes-btn.png"),Sprite::create("cocostudio/UI/popup/yes-btn.png"), CC_CALLBACK_1(Dialog::onClick, this));
        positive->setUserData((void*)99);
        positive->setAnchorPoint(Point(0.5f, 0));
        
        m_pPositiveLabel = Label::createWithTTF("",font_felt.c_str(), 30);
        m_pPositiveLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        m_pPositiveLabel->setDimensions(positive->getContentSize().width, positive->getContentSize().height);
        m_pPositiveLabel->setAnchorPoint(Point(0.5f, 0.5f));
        positive->addChild(m_pPositiveLabel);

        // create negative button
        MenuItemSprite* negative;
        if(dialogType == DIALOG_TYPE_SINGLE)
        {
            negative= MenuItemSprite::create(Sprite::create("cocostudio/UI/popup/ok-btn.png"), Sprite::create("cocostudio/UI/popup/ok-btn.png"),  CC_CALLBACK_1(Dialog::onClick, this));
        }
        else
        {
            negative = MenuItemSprite::create(Sprite::create("cocostudio/UI/popup/no-btn.png"),Sprite::create("cocostudio/UI/popup/no-btn.png"), CC_CALLBACK_1(Dialog::onClick, this));
        }
        negative->setUserData((void*)1);
        negative->setAnchorPoint(Point(0.5, 0));
        
        m_pNegativeLabel = Label::createWithTTF("", font_felt.c_str(), 30);
        m_pNegativeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        m_pNegativeLabel->setDimensions(negative->getContentSize().width, negative->getContentSize().height);
        m_pNegativeLabel->setAnchorPoint(Point(0.5f, 0.5f));
        negative->addChild(m_pNegativeLabel);
        
        
        // create menu
        if(dialogType == DIALOG_TYPE_SINGLE)
        {
            mMenu = Menu::create(negative,  nullptr);
        }else
        {
            mMenu = Menu::create(negative, positive, nullptr);
        }
        mMenu->alignItemsHorizontallyWithPadding(30);
        mMenu->setPosition(Point(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height * 0.1 + 15));
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
        m_pBg->setPosition(Vec2::ZERO);
        
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
    
    AudioHelp::getInstance()->playEffect("button_general.mp3");
    
    mMenu->setEnabled(false);
    MenuItemSprite* btn = (MenuItemSprite*)obj;
    
    size_t type = (size_t)btn->getUserData();
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
    
    
    AudioHelp::getInstance()->playEffect("present in.mp3");
    
    MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(DESIGN_WIDTH/2, DESIGN_HEIGHT/2)));
    EaseSineOut* esi = EaseSineOut::create(move);
    ScaleTo* scale = ScaleTo::create(0.5, 1.0);
    Spawn* sp = Spawn::create(esi,scale, NULL);
    
    ScaleTo* s1 = ScaleTo::create(0.1f,1.08f,0.90f);
    ScaleTo* s2 = ScaleTo::create(0.1f,0.90,1.08f);
    ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);

    
    m_pBg->runAction(Sequence::create(sp,s2,s1,s3, NULL));
    
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

void Dialog::setContentText(const char* text)
{
    m_pContentLabel->setString(text);
    
    refreshPosition();
}
void Dialog::setContentTextColor(const Color3B &color)
{
    m_pContentLabel->setColor(color);
}

void Dialog::refreshPosition()
{
    m_pContentLabel->setPosition(m_pBg->getContentSize().width / 2, m_pBg->getContentSize().height / 2 + 30);
}
