#include "Dialog.h"
#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"
#include "Global.h"
#include "AudioHelp.h"

using namespace cocos2d::extension;

static const float btn_padding=
#if (resourceType==SF_RESOURCES_2048_1536)
50*2;
#else
30;
#endif


Dialog::Dialog()
{
    m_bIsCocos = false;
}

Dialog::~Dialog()
{
    
}

Dialog* Dialog::create(Size size,void* _args,int type)
{
    Dialog* dialog =new Dialog();
    dialog->autorelease();
    dialog->dialogType=type;
    dialog->dialogSize=size;
    
    dialog->args=_args;
    dialog->init();
    return dialog;
}

bool Dialog::init()
{
    if (LayerColor::init())
    {
        contentLayer=LayerColor::create(Color4B(0,0,0,160));
        
        addChild(contentLayer);
        
        onNegativeClick = nullptr;
        onPositiveClick = nullptr;
        if (m_bIsCocos) {
            if(dialogType==DIALOG_TYPE_SINGLE){
                m_pUL = GUIReader::getInstance()->widgetFromJsonFile("dialog/popup.json");
                this->addChild(m_pUL);
                Button* pOKButton = dynamic_cast<Button*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "popup_btn_ok"));
                pOKButton->addTouchEventListener(CC_CALLBACK_2(Dialog::_onUIButtonCallback, this));
                pOKButton->setVisible(true);
                
                Button* pYESButton = dynamic_cast<Button*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "popup_btn_yes"));
                pYESButton->addTouchEventListener(CC_CALLBACK_2(Dialog::_onUIButtonCallback, this));
                pYESButton->setVisible(false);
                pOKButton->setTag(0);
            }else{
                m_pUL = GUIReader::getInstance()->widgetFromJsonFile("dialog/popup_double.json");
                this->addChild(m_pUL);
                
                Button* pNOButton = dynamic_cast<Button*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "popup_btn_no"));
                pNOButton->addTouchEventListener(CC_CALLBACK_2(Dialog::_onUIButtonCallback, this));
                pNOButton->setTag(0);
                
                Button* pYESButton = dynamic_cast<Button*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "popup_btn_yes"));
                pYESButton->addTouchEventListener(CC_CALLBACK_2(Dialog::_onUIButtonCallback, this));
                pYESButton->setTag(1);
                
            }
        }else{
            bg = Scale9Sprite::create("dialog/popup_bg.png");
            bg->setPreferredSize(Size(dialogSize.width, dialogSize.height));
            bg->setContentSize(dialogSize);
            //            bg->setCapInsets(Rect(2,2,dialogSize.width-10,dialogSize.height-10));
            bg->setInsetLeft(10);
            bg->setInsetRight(10);
            bg->setInsetTop(10);
            bg->setInsetBottom(10);
            bg->setContentSize(dialogSize);
            //            bg->setCapInsets(Rect(10,10,dialogSize.width*0.9,dialogSize.height*0.9));
            //            bg->setAnchorPoint(Vec2(0.5,0.5));
            
            bg->setPosition(Vec2(contentLayer->getContentSize().width/2, contentLayer->getContentSize().height/ 2.0f));
            contentLayer->addChild(bg);
            
            title_label = Label::createWithTTF("",font_felt.c_str(), fontsize(50),Size(bg->getContentSize().width*0.8, bg->getContentSize().height*0.4),TextHAlignment::CENTER, TextVAlignment::CENTER);
            title_label->setPosition(Vec2(contentLayer->getContentSize().width/2,bg->getContentSize().height*0.75));
            bg->addChild(title_label);
            
            content_label = Label::createWithTTF("",font_felt.c_str(), fontsize(40),Size(dialogSize.width*0.9,dialogSize.height*0.8),TextHAlignment::CENTER, TextVAlignment::CENTER);
            content_label->setDimensions(dialogSize.width*0.7,dialogSize.height*0.5);
            
            
            content_label->setAnchorPoint(Vec2(0.5,0.5));
            content_label->setColor(Color3B::WHITE);
            bg->addChild(content_label);
            
            
            MenuItemSprite* positive;
            if(dialogType==DIALOG_TYPE_SINGLE)
            {
                positive= MenuItemSprite::create(Sprite::create("dialog/btn_ok.png"), Sprite::create("dialog/btn_ok.png"),
                                                 CC_CALLBACK_1(Dialog::onClick, this));
            }else
            {
                positive= MenuItemSprite::create(Sprite::create("dialog/btn_yes.png"), Sprite::create("dialog/btn_yes.png"),
                                                 CC_CALLBACK_1(Dialog::onClick, this));
            }
            positive->setUserData((void*)99);
            //	positive->setPosition(ccp2(220,70));
            positive->setAnchorPoint(Vec2(0.5,0.5));
            
            positive_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30), positive->getContentSize(), TextHAlignment::CENTER, TextVAlignment::CENTER);
            positive_label->setPosition(Vec2(positive->getContentSize().width/2,positive->getContentSize().height/2));
            positive_label->setAnchorPoint(Vec2(0.5,0.5));
            positive_label->setColor(Color3B(250,110,2));
            positive->addChild(positive_label);
            
            
            MenuItemSprite* negative= MenuItemSprite::create(Sprite::create("dialog/btn_no.png"), Sprite::create("dialog/btn_no.png"),
                                                             CC_CALLBACK_1(Dialog::onClick,this));
            
            negative->setUserData((void*)MENU_TYPE_1);
            //	negative->setPosition(ccp2(50,70));
            negative->setAnchorPoint(Vec2(0.5,0.5));
            
            negative_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30),negative->getContentSize(),TextHAlignment::CENTER, TextVAlignment::CENTER);
            negative_label->setPosition(Vec2(negative->getContentSize().width/2,negative->getContentSize().height/2));
            negative_label->setAnchorPoint(Vec2(0.5,0.5));
            negative_label->setColor(Color3B(250,110,2));
            
            negative->addChild(negative_label);
            
            if(dialogType==DIALOG_TYPE_SINGLE)
            {
                mMenu = Menu::create(positive,  NULL );
            }else
            {
                mMenu = Menu::create(positive,negative,  NULL );
            }
            
            mMenu->alignItemsHorizontallyWithPadding(btn_padding);
            mMenu->setPosition(Vec2(dialogSize.width/2,dialogSize.height*0.2));
            
            bg->addChild( mMenu );
            //            contentLayer->runAction(ScaleTo::create(0.3,1));
            refreshPosition();
        }
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan =
        listener->onTouchBegan = [=](cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        return true;
    }
    AudioHelp::getInstance()->playEffect("popup_show.mp3");
    return false;
}

void Dialog::_onUIButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED)
    {
        Button* btn = (Button*)ref;
        int tag = btn->getTag();
        if (tag==0) {
            dismiss();
        }else{
            
            onBack();
        }
    }
    
}
void Dialog::onBack()
{
    if(onPositiveClick){
        onPositiveClick(args);
    }
    getParent()->removeChild(this,true);
}
void Dialog::dismiss()
{
    AudioHelp::getInstance()->playEffect("popup_dismiss.mp3");
    if(onNegativeClick){
        onNegativeClick(args);
    }
    getParent()->removeChild(this,true);
    
    
}

void Dialog::onClick(Ref* obj)
{
    
    if(!mMenu->isEnabled())
        return;
    mMenu->setEnabled(false);
    MenuItemSprite* btn=(MenuItemSprite*)obj;
    size_t type = (size_t)btn->getUserData();
    if(type==MENU_TYPE_1)
    {
        
        FiniteTimeAction* action = Sequence::create(ScaleTo::create(0.3,0.001),CallFunc::create(CC_CALLBACK_0(Dialog::dismiss,this)) , NULL);
        
        bg->runAction(action);
        
    }else
    {
        FiniteTimeAction* action = Sequence::create(ScaleTo::create(0.3,0.001),CallFunc::create(CC_CALLBACK_0(Dialog::onBack,this)) , NULL);
        
        bg->runAction(action);
    }
}


void Dialog::setPositiveBtnText(const char* text)
{
    if (!m_bIsCocos) {
        positive_label->setString(text);
    }
}
void Dialog::setNegativeBtnText(const char* text)
{
    if (!m_bIsCocos) {
        negative_label->setString(text);
    }
}



void Dialog::setSelectBtn(const char* po,const char* ne)
{
    if (!m_bIsCocos) {
        mMenu->removeAllChildren();
        
        MenuItemSprite* positive;
        std::stringstream ostr;
        ostr<<"dialog/"<<po;
        positive= MenuItemSprite::create(Sprite::create(ostr.str()), Sprite::create(ostr.str()),
                                         CC_CALLBACK_1(Dialog::onClick, this));
        
        positive->setUserData((void*)99);
        //	positive->setPosition(ccp2(220,70));
        positive->setAnchorPoint(Vec2(0.5,0.5));
        
        positive_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30), positive->getContentSize(), TextHAlignment::CENTER, TextVAlignment::CENTER);
        positive_label->setPosition(Vec2(positive->getContentSize().width/2,positive->getContentSize().height/2));
        positive_label->setAnchorPoint(Vec2(0.5,0.5));
        positive_label->setColor(Color3B(250,110,2));
        positive->addChild(positive_label);
        
        
        std::stringstream ostrNegative;
        ostrNegative<<"dialog/"<<ne;
        MenuItemSprite* negative= MenuItemSprite::create(Sprite::create(ostrNegative.str()), Sprite::create(ostrNegative.str()),
                                                         CC_CALLBACK_1(Dialog::onClick,this));
        
        negative->setUserData((void*)MENU_TYPE_1);
        //	negative->setPosition(ccp2(50,70));
        negative->setAnchorPoint(Vec2(0.5,0.5));
        
        negative_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30),negative->getContentSize(),TextHAlignment::CENTER, TextVAlignment::CENTER);
        negative_label->setPosition(Vec2(negative->getContentSize().width/2,negative->getContentSize().height/2));
        negative_label->setAnchorPoint(Vec2(0.5,0.5));
        negative_label->setColor(Color3B(250,110,2));
        
        negative->addChild(negative_label);
        mMenu->addChild(positive);
        mMenu->addChild(negative);
        mMenu->alignItemsHorizontallyWithPadding(btn_padding);
    }
}
void Dialog::setContentText(const char* text)
{
    if (!m_bIsCocos) {
        content_label->setString(text);
        refreshPosition();
    }else{
        Text* pText = dynamic_cast<Text*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "text"));
        Text *pShadow = dynamic_cast<Text*>(cocos2d::ui::Helper::seekNodeByName(m_pUL, "text_shadow"));
        pText->setString(string(text));
        pShadow->setString(string(text));
    }
    
    
}


void Dialog::setTitleText( const char* text)
{
    if (!m_bIsCocos) {
        content_label->setString(text);
        refreshPosition();
    }
    title_label->setString(text);
}

void Dialog::refreshPosition()
{
    content_label->setPosition(Vec2(dialogSize.width/2,dialogSize.height*0.6));
}


