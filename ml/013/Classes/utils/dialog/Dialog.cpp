#include "Dialog.h"
#include "SimpleAudioEngine.h"
//#include "ui/CocosGUI.h"
//#include "cocos-ext.h"

USING_NS_CC_EXT;
//using namespace cocos2d::extension;

static const float btn_padding=
#if (resourceType==SF_RESOURCES_2048_1536)
88*2;
#else
88;
#endif

#define fontsize(size) (resourceType==SF_RESOURCES_2048_1536) ? (float)(size) * 2 : size

#define KFadeTime 0.1

Dialog::Dialog() :callback(NULL)
{
    
}

Dialog::~Dialog()
{

}

Dialog* Dialog::create(Size size,void* _args,int type)
{
	Dialog* dialog =new Dialog();
    if (dialog) {
        dialog->autorelease();
        dialog->dialogType=type;
        dialog->dialogSize=size;
        dialog->args=_args;
        dialog->init();
        return dialog;
    }
    CC_SAFE_DELETE(dialog);
	return nullptr;
}
bool Dialog::init()
{
	if (LayerColor::initWithColor(Color4B(0, 0, 0, 150)))
    {
        Size lViSize = Director::getInstance()->getVisibleSize();
        
        contentLayer=LayerColor::create(Color4B(100, 0, 0, 0));
		contentLayer->setContentSize(dialogSize);
//		contentLayer->ignoreAnchorPointForPosition(false);
		contentLayer->setAnchorPoint(Point(0.5,0.5));
        
        contentLayer->setScale(0.1);
		addChild(contentLayer);
        
		Rect rect = Rect(0,0,150,150);
		Rect rect_inside = Rect(50,50,50,50);
        
//        Scale9Sprite* bg = Scale9Sprite::create("dialog/dialog_bg.png", rect,rect_inside);
		Sprite* bg = Sprite::create("dialog/popup.png");
//		bg->setPreferredSize(Size(599, 380));
        bg->setContentSize(Size(440, 327));
		bg->setAnchorPoint(Point(0.5,0.5));
		bg->setPosition(Point((lViSize.width) * 0.5, lViSize.height * 0.5));
		contentLayer->addChild(bg);
        
		title_label = Label::createWithTTF("", font_felt.c_str(), fontsize(40));
		title_label->setPosition(Point(contentLayer->getContentSize().width/2,bg->getContentSize().height*0.9));
		bg->addChild(title_label);

		content_label = Label::createWithTTF("", font_felt.c_str(), fontsize(25));
		content_label->setDimensions(dialogSize.width*0.8, dialogSize.height*0.4);
        
        
		content_label->setAnchorPoint(Point(0.5,1));
		content_label->setColor(Color3B(0,78,124));
		bg->addChild(content_label);
        
        MenuItemSprite* positive = MenuItemSprite::create(Sprite::create("dialog/ui02_btn_yes.png"),Sprite::create("dialog/ui02_btn_yes.png"), CC_CALLBACK_1(Dialog::onClick, this));
        
		positive->setUserData((void*)99);
        //	positive->setPosition(Point2(220,70));
		positive->setAnchorPoint(Point(0.5,0.3));
        
        positive_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30));
        positive_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        positive_label->setDimensions(positive->getContentSize().width, positive->getContentSize().height);
		positive_label->setPosition(Point(positive->getContentSize().width/4,positive->getContentSize().height/2));
		positive_label->setAnchorPoint(Point(0.5,0.5));
		positive_label->setColor(Color3B(0,78,124));
		positive->addChild(positive_label);
        
        
		MenuItemSprite* negative;
        if(dialogType==DIALOG_TYPE_SINGLE)
        {
            negative= MenuItemSprite::create(Sprite::create("dialog/ui02_btn_ok.png"), Sprite::create("dialog/ui02_btn_ok.png"),  CC_CALLBACK_1(Dialog::onClick, this));
        }
        else
        {
            negative = MenuItemSprite::create(Sprite::create("dialog/ui02_btn_no.png"),Sprite::create("dialog/ui02_btn_no.png"), CC_CALLBACK_1(Dialog::onClick, this));
        }

		negative->setUserData((void*)MENU_TYPE_1);
        //	negative->setPosition(Point2(50,70));
		negative->setAnchorPoint(Point(0.5,0.3));
        
        
        negative_label = Label::createWithTTF("",font_felt.c_str(), fontsize(30));
        negative_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        negative_label->setDimensions(negative->getContentSize().width, negative->getContentSize().height);
		negative_label->setPosition(Point(negative->getContentSize().width/4,negative->getContentSize().height/2));
		negative_label->setAnchorPoint(Point(0.5,0.5));
		negative_label->setColor(Color3B(0,0,0));
        
		negative->addChild(negative_label);
        
		if(dialogType==DIALOG_TYPE_SINGLE)
		{
			mMenu = Menu::create(negative,  NULL );
		}else
		{
			mMenu = Menu::create(negative,positive,  NULL );
		}
		
		mMenu->alignItemsHorizontallyWithPadding(40);
		mMenu->setPosition(Point(dialogSize.width/2,dialogSize.height*0.2));
        
		bg->addChild( mMenu,88 );
        
        {
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [=](Touch* touch, Event* event){
                
                return true;
            };
            listener->onTouchMoved =[=](Touch* touch, Event* event){
                
                
            };
            listener->onTouchEnded =[=](Touch* touch, Event* event){
                
                
            };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        }

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
    getParent()->removeChild(this,true);
    
    CCLOG("onBack -> %d", m_FuncType);
	
    if(callback!=NULL)
        callback->onPositiveClick(args);
}

void Dialog::dismiss()
{
    getParent()->removeChild(this,true);

    CCLOG("dismiss -> %d", m_FuncType);
    
    if(callback!=NULL)
        callback->onNegativeClick(args);
}

void Dialog::onClick(Ref* obj)
{
	if(!mMenu->isEnabled())
		return;
	mMenu->setEnabled(false);
    MenuItemSprite* btn=(MenuItemSprite*)obj;
	int type=(uintptr_t)btn->getUserData();
	if(type==MENU_TYPE_1)
	{
		
        FadeOut* lFade = FadeOut::create(KFadeTime);
        ScaleTo* lScale = ScaleTo::create(KFadeTime,0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, NULL);
        DelayTime* lDelay = DelayTime::create(0.1);
		FiniteTimeAction* action = Sequence::create(lSpawn, lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::dismiss, this)), NULL);
		
		contentLayer->runAction(action);
        
	}else
	{
        FadeOut* lFade = FadeOut::create(KFadeTime);
        ScaleTo* lScale = ScaleTo::create(KFadeTime,0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, NULL);
        DelayTime* lDelay = DelayTime::create(0.1);
		FiniteTimeAction* action = Sequence::create(lSpawn, lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::onBack, this)) , NULL);
		
		contentLayer->runAction(action);
	}
}

void Dialog::onEnter()
{
    LayerColor::onEnter();
}

void Dialog::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    contentLayer->setScale(1.0);
//    contentLayer->runAction(ScaleTo::create(0.3, 1.0));
    refreshPosition();
}

void Dialog::onExit()
{
    LayerColor::onExit();
}

void Dialog::setPositiveBtnText(const char* text)
{
    positive_label->setString(text);
}
void Dialog::setNegativeBtnText(const char* text)
{
    negative_label->setString(text);
}
void Dialog::setContentText(const char* text)
{
    content_label->setString(text);
	refreshPosition();
}

void Dialog::setTitleText( const char* text)
{
	title_label->setString(text);
	refreshPosition();
}

void Dialog::refreshPosition()
{
	content_label->setPosition(Point(dialogSize.width/2,dialogSize.height/1.5-title_label->getContentSize().height));
}

