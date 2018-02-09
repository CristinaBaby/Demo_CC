#include "Dialog.h"
#include "Global.h"
#include "AdapterScreen.h"
//#include "ui/CocosGUI.h"
//#include "cocos-ext.h"
#include "Global.h"
USING_NS_CC_EXT;





string dialogPath = "";
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
        
        contentLayer=LayerColor::create(Color4B(100, 0, 0, 0));
		contentLayer->setContentSize(dialogSize);
//		contentLayer->ignoreAnchorPointForPosition(false);
		contentLayer->setAnchorPoint(Point(0.5,0.5));
        
        contentLayer->setScale(0.1);
		addChild(contentLayer);
        
		Rect rect = Rect(0,0,150,150);
		Rect rect_inside = Rect(50,50,50,50);
       
//        Scale9Sprite* bg = Scale9Sprite::create("dialog/dialog_bg.png", rect,rect_inside);
		m_pBg = Sprite::create(g_path + "dialog/popup_bg.png");
//		bg->setPreferredSize(Size(599, 380));
		m_pBg->setAnchorPoint(Point(0.5,0.5));
        m_pBg->setScale(0);
        log("------>%f %f",DESIGN_WIDTH/2, DESIGN_HEIGHT/2);
        
        kAdapterScreen->setExactPosition(m_pBg, DESIGN_WIDTH/2, DESIGN_HEIGHT/2);
		contentLayer->addChild(m_pBg);
        
		title_label = Label::createWithTTF("", font_felt.c_str(), 40);
		title_label->setPosition(Point(contentLayer->getContentSize().width/2,m_pBg->getContentSize().height*0.9));
		m_pBg->addChild(title_label);

		content_label = Label::createWithTTF("", font_felt.c_str(), 30);
		content_label->setDimensions(dialogSize.width*0.85, dialogSize.height*0.3);
        
        
		content_label->setAnchorPoint(Point(0.5,1));
        content_label->setPosition(Vec2(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));
//		content_label->setColor(Color3B(0,0,128));
        content_label->setColor(Color3B(255,255,255));
		m_pBg->addChild(content_label);
        
        MenuItemSprite* positive = MenuItemSprite::create(Sprite::create(g_path + "dialog/popup_btn_yes.png"),Sprite::create(g_path + "dialog/ui02_btn_yes.png"), CC_CALLBACK_1(Dialog::onClick, this));
        
		positive->setUserData((void*)99);
        positive->setAnchorPoint(Point(0.5,0));
        positive->setPosition(Vec2(2*m_pBg->getContentSize().width/3,5));
        
        positive_label = Label::createWithTTF("",font_felt.c_str(), 30);
        positive_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        positive_label->setDimensions(positive->getContentSize().width, positive->getContentSize().height);
		positive_label->setAnchorPoint(Point(0.5,0.5));
		positive_label->setColor(Color3B(0,0,128));
		positive->addChild(positive_label);

        
		MenuItemSprite* negative;
        if(dialogType==DIALOG_TYPE_SINGLE)
        {
            negative= MenuItemSprite::create(Sprite::create(g_path + "dialog/popup_btn_ok.png"), Sprite::create(g_path + "dialog/popup_btn_ok.png"),  CC_CALLBACK_1(Dialog::onClick, this));
        }
        else
        {
            negative = MenuItemSprite::create(Sprite::create(g_path + "dialog/popup_btn_no.png"),Sprite::create(g_path + "dialog/popup_btn_no.png"), CC_CALLBACK_1(Dialog::onClick, this));
        }

		negative->setUserData((void*)1);
        negative->setAnchorPoint(Point(0.5,0));
        negative_label = Label::createWithTTF("",font_felt.c_str(), 30);
        negative_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        negative_label->setDimensions(negative->getContentSize().width, negative->getContentSize().height);
		negative_label->setPosition(Point(negative->getContentSize().width/2,negative->getContentSize().height/2));
		negative_label->setAnchorPoint(Point(0.5,0.5));
		negative_label->setColor(Color3B(0,0,128));
        
		negative->addChild(negative_label);
        
		if(dialogType==DIALOG_TYPE_SINGLE)
		{
			mMenu = Menu::create(negative,  NULL );
		}else
		{
			mMenu = Menu::create(negative,positive,  NULL );
		}
		
		mMenu->alignItemsHorizontallyWithPadding(50);
		mMenu->setPosition(Point(dialogSize.width/2,dialogSize.height*0.05 + 20));
        
		m_pBg->addChild( mMenu,88 );
        
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
	if(type==1)
	{
		
        FadeOut* lFade = FadeOut::create(0.1);
        ScaleTo* lScale = ScaleTo::create(0.1,0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, NULL);
        DelayTime* lDelay = DelayTime::create(0.1);
		FiniteTimeAction* action = Sequence::create(EaseElasticIn::create(ScaleTo::create(0.5,0),1.0), lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::dismiss, this)), NULL);
//        contentLayer->runAction(EaseElasticIn::create(ScaleTo::create(0.5,1.0),1.0));
		m_pBg->runAction(action);
        
	}else
	{
        FadeOut* lFade = FadeOut::create(0.1);
        ScaleTo* lScale = ScaleTo::create(0.1,0);
        Spawn* lSpawn = Spawn::create(lFade, lScale, NULL);
        DelayTime* lDelay = DelayTime::create(0.1);
		FiniteTimeAction* action = Sequence::create(EaseElasticIn::create(ScaleTo::create(0.5,0),1.0), lDelay, CallFunc::create(CC_CALLBACK_0(Dialog::onBack, this)) , NULL);
//		contentLayer->runAction(EaseElasticIn::create(ScaleTo::create(0.5,1.0),1.0));
		m_pBg->runAction(action);
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
    
//    m_pBg->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0), NULL));
    MoveTo* move = MoveTo::create(0, kAdapterScreen->getExactPostion(Vec2(DESIGN_WIDTH/2, DESIGN_HEIGHT/2)));
    m_pBg->runAction(Sequence::create(move,EaseElasticOut::create(ScaleTo::create(0.5,1.0),1.0), NULL));
    
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
	content_label->setPosition(Point(dialogSize.width/2+10,dialogSize.height*0.6-title_label->getContentSize().height));
}

