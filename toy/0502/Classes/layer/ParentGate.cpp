//
//  ParentGate.cpp
//  KidsBook
//
//  Created by lidong on 14-10-27.
//
//

#include "ParentGate.h"
//#include "../Common/KHelper.h"
//#include "../Common/STVisibleRect.h"
#include "../modules/STSystemFunction.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "OCHelp.h"
#endif
#include "../modules/STSystemFunction.h"
#include "../gameconfig/GameConfig.h"
#include "../AppGlobal.h"

#define FontName   "Helvetica Rounded LT Black.ttf"

enum
{
   eShip_MenuIterm_Tag=1,
   eAirplay_MenuItem_Tag,
   eFireBallon_MenuItem_Tag,
   eClose_MenuItem_Tag
};

bool ParentGate::init()
{
    LayerColor::initWithColor(Color4B(0, 0, 0, 180));
    
    _delegate=NULL;
    
    Sprite* lSprite= Sprite::create("box.png");
    lSprite->setPosition(ccp_center_screen);
    addChild(lSprite);
    
    int lRand=arc4random()%3+1;
    _value=(AnswerValue)lRand;
    
    int lNumber=arc4random()%2+2;
    _number=lNumber;
    
    int lStartValue=100;
    switch (_value) {
        case Answer_ShipValue:
        {
           
        }
            break;
        case Answer_AirplayValue:
        {
            lStartValue=50;
        }
            break;
        case Answer_BallonValue:
        {
            lStartValue=10;
        }
            break;
        default:
            break;
    }
    float my_scale = 1;
    log("%d" , _value);
    std::stringstream ss;
    ss<<"Tap the ";
    Label* label=getMyLabel(FontName, ss.str().c_str(), 30);
    lSprite->addChild(label);
    label->setAnchorPoint(Vec2(0.f, 0.5f));
    label->setPosition(Vec2(lSprite->getContentSize().width*0.05f, lSprite->getContentSize().height/3*2));
    label->setScale(my_scale);
    
    std::stringstream ss2;
    ss2<<this->getStr(_value);
    ss2<<" ";
    ss2<<_number;
    
    log("%d" , _value);
    log("%s" , ss2.str().c_str());
    Label* label2=getMyLabel(FontName, ss2.str().c_str(), 30);
    lSprite->addChild(label2);
    label2->setAnchorPoint(Vec2(0, 0.5f));
    label2->setPosition(Vec2(label->getPosition().x+label->getContentSize().width*my_scale, label->getPosition().y));
    label2->setColor(Color3B(248, 215, 58));
        label2->setScale(my_scale);
    
    std::stringstream ss3;
    ss3<<" ";
    ss3<<"times";
    Label* label3=getMyLabel(FontName, ss3.str().c_str(), 30);
    lSprite->addChild(label3);
    label3->setAnchorPoint(Vec2(0, 0.5f));
    label3->setPosition(Vec2(label2->getPosition().x+label2->getContentSize().width*my_scale, label2->getPosition().y));
    label3->setScale(my_scale);
    
    MenuItemSprite* lShipMenuItem=MenuItemSprite::create(Sprite::create("ship.png"), Sprite::create("ship.png"));
    lShipMenuItem->setTag(eShip_MenuIterm_Tag);
    lShipMenuItem->setPosition((Vec2(120,130)*my_scale));
    
    MenuItemSprite* lAirplayMenuItem= MenuItemSprite::create(Sprite::create("airplane.png"), Sprite::create("airplane.png"));
    lAirplayMenuItem->setTag(eAirplay_MenuItem_Tag);
    lAirplayMenuItem->setPosition((Vec2(278,130)*my_scale));
    
    MenuItemSprite* lFireBallon=  MenuItemSprite::create(Sprite::create("fireballoon.png"), Sprite::create("fireballoon.png"));
    lFireBallon->setTag(eFireBallon_MenuItem_Tag);
    lFireBallon->setPosition((Vec2(436,130)*my_scale));
    
    MenuItemSprite* lCloseMenuItem=  MenuItemSprite::create(Sprite::create("close.png"), Sprite::create("close.png"));
    lCloseMenuItem->setTag(eClose_MenuItem_Tag);
    lCloseMenuItem->setPosition((Vec2(516, 330)*my_scale));
    
    _menu=Menu::create(lShipMenuItem,lAirplayMenuItem,lFireBallon,lCloseMenuItem,NULL);
    _menu->setPosition(Vec2(0, 0));
    _menu->setAnchorPoint(Vec2(0, 0));
    lSprite->addChild(_menu);
    
    _counter=0;
    
    registerWithMyTouchDispatcher();
    this->setScale(0.0f);
    ScaleTo* lScaleTo=ScaleTo::create(0.5f, 1.0f);
    EaseBackInOut* lEaseInOut=EaseBackInOut::create(lScaleTo);
    this->runAction(lEaseInOut);
    
    auto eventPlay = EventListenerTouchOneByOne::create();
    eventPlay->setSwallowTouches(true);
    eventPlay->onTouchBegan = [](Touch* touch, Event* event){
        // event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        //Get the position of the current point relative to the button
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        //Check the click area
        if (rect.containsPoint(locationInNode))
        {
            //               log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
//            target->setOpacity(180);
            return true;
        }
        return false;
    };
    
    //Process the touch end event
    eventPlay->onTouchEnded = [=](Touch* touch, Event* event){
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setOpacity(255);
        //Reset zOrder and the display sequence will change
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
            ParentGate::doAction(target);
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventPlay, lShipMenuItem);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventPlay->clone(), lAirplayMenuItem);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventPlay->clone(), lFireBallon);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventPlay->clone(), lCloseMenuItem);
    
    return true;
}

std::string ParentGate::getStr(AnswerValue pValue)
{
    std::string str;
    
    switch (pValue) {
        case Answer_ShipValue:
        {
           str="Ship";
        }
            break;
        case Answer_AirplayValue:
        {
            str="Airplane";
        }
            break;
        case Answer_BallonValue:
        {
            str="Hot Air Balloon";
        }
            break;
        default:
            break;
    }
    
    return str;
}



const char* ParentGate::getBlankStr(AnswerValue value)
{
    std::string str;
    
    switch (value) {
        case Answer_ShipValue:
        {
            str="    ";
        }
            break;
        case Answer_AirplayValue:
        {
            str="        ";
        }
            break;
        case Answer_BallonValue:
        {
            str="               ";
        }
            break;
        default:
            break;
    }

    return str.c_str();
}



void ParentGate::doAction(cocos2d::Ref *pSender)
{
    MenuItemSprite* lMenuItem=(MenuItemSprite*)pSender;
    
    if(lMenuItem->getTag()==eClose_MenuItem_Tag)
    {
        _menu->setEnabled(false);
        
        this->setScale(1.0f);
        ScaleTo* lScaleTo=ScaleTo::create(0.5f, 0.0f);
        EaseBackIn* lEaseIn=EaseBackIn::create(lScaleTo);
        CallFunc* lFunc=CallFunc::create(this, callfunc_selector(ParentGate::gotoBack));
        Sequence* lAction=CCSequence::create(lEaseIn,lFunc,NULL);
        this->runAction(lAction);
        
        
        return;
    }
    
    if(lMenuItem->getTag()==(int)_value)
    {
        _counter++;
    }
    else
    {
        _counter=0;
    }
    
    if(_counter==_number)
    {
        _menu->setEnabled(false);
        this->gotoNext(0);
    }
    log("%d" , _counter);
}

Label* ParentGate::getMyLabel(std::string label_font , const char *label_name , int size)
{
    Label *label = Label::create(label_name, label_font, size);
    return label;
}

void ParentGate::gotoNext(float dt)
{

    if(strcmp(_linkStr.c_str(), "support")==0)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        OCHelp::loadSupport();
#endif
    }
    else if(strcmp(_linkStr.c_str(), "rateus")==0)
    {
        STSystemFunction *sf = new STSystemFunction;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //  sf->rating(kAppleID,RATE_INFORMATION);
//        OCHelp::loadRateUs("955758648",RATE_INFORMATION);
#else
//        sf->rating();
#endif
        
        CC_SAFE_DELETE(sf);
    }
    else if(strcmp(_linkStr.c_str(), "terms")==0)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
          OCHelp::loadTermsOfUse();
#endif
       
    }
    else if(strcmp(_linkStr.c_str(), "more")==0)
    {
        STSystemFunction *sf = new STSystemFunction;
        sf->showMoreGame();
        CC_SAFE_DELETE(sf);
    }else if (strcmp(_linkStr.c_str(), "privacy") == 0){

        STSystemFunction sf;
        sf.go2PrivacyPage();
    }
    
    if(_delegate!=NULL)
    {
        _delegate->parentGateBack(_linkStr);
        
    }
    
    
      this->removeFromParentAndCleanup(true);
    
}

void ParentGate::gotoBack()
{
    if(this->getDelegate()!=NULL)
    {
//        this->getDelegate()->parentGateBack("");
    }
    
    this->removeFromParentAndCleanup(true);
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}

void ParentGate::setLinkString(const char *info)
{
    _linkStr=info;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    scheduleOnce(schedule_selector(ParentGate::gotoNext), 0);
#endif
}

#pragma mark - Touch Event

void ParentGate::registerWithMyTouchDispatcher(){
    //注册监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ParentGate::touchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ParentGate::touchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ParentGate::touchEnded,this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}
bool ParentGate::touchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
void ParentGate::touchMoved(Touch *pTouch, Event *pEvent)
{
}
void ParentGate::touchEnded(Touch *pTouch, Event *pEvent)
{
}




