
#include "SetPriceLayer.h"
#include "SceneManager.h"
#include "CMVisibleRect.h"
#include <regex>


static Vec2 gItemPos[]={
    Vec2(480,310),
    Vec2(180,370),
    Vec2(810,350),
};
SetPriceLayer::SetPriceLayer()
{
    onPriceSetCallbak = nullptr;
}

SetPriceLayer::~SetPriceLayer()
{
}
bool SetPriceLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(color);
    
    currentTex = "";
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void SetPriceLayer::showPriceTag()
{
    //    bg
    m_pBg = Sprite::create("content/showcase/price1.png");
//    m_pBg->setPosition(Vec2(visibleSize.width*0.5-50, visibleSize.height*0.5));
    m_pBg->setPosition(m_pos);
    this->addChild(m_pBg);
    ccBezierConfig cfg;
    cfg.controlPoint_1 = cfg.controlPoint_2 = Vec2(600, 400);
    cfg.endPosition = Vec2(visibleSize.width*0.5-50, visibleSize.height*0.5);
    m_pBg->setScale(0.1);
    m_pBg->runAction(Sequence::create(Spawn::create(RotateBy::create(0.3, 360*2),
                                                    BezierTo::create(1, cfg),
                                                    ScaleTo::create(0.8, 1), NULL),
                                      CallFunc::create([=](){
        Rect rect = m_pBg->getBoundingBox();
        
        m_pBtnOK = Button::create("content/showcase/btn_confirm.png");
        this->addChild(m_pBtnOK,1);
        m_pBtnOK->setPosition(Vec2(rect.getMaxX(), rect.getMaxY()));
        m_pBtnOK->addTouchEventListener(CC_CALLBACK_2(SetPriceLayer::onButtonCallback, this));
        m_pBtnOK->setTag(1);
        
        m_pBtnCancel = Button::create("content/showcase/btn_delete.png");
        this->addChild(m_pBtnCancel,1);
        m_pBtnCancel->setPosition(Vec2(rect.getMaxX(), rect.getMinY()));
        m_pBtnCancel->addTouchEventListener(CC_CALLBACK_2(SetPriceLayer::onButtonCallback, this));
        m_pBtnCancel->setTag(0);
        
        m_pExText = Label::createWithTTF("$", font_felt, 135);
        m_pExText->setTextColor(Color4B::BLACK);
        m_pBg->addChild(m_pExText);
        m_pExText->setPosition(Vec2(-20, 0)+m_pBg->getContentSize()*0.5);
        
        m_pText = EditBox::create(Size(450,202), Scale9Sprite::create());
        m_pText->setFontName(font_felt.c_str());
        m_pText->setFontSize(135);
        m_pText->setFontColor(Color3B::BLACK);
        m_pText->setPlaceholderFontSize(135);
        m_pText->setPlaceholderFontColor(Color3B::BLACK);
        m_pText->setMaxLength(5); //限制字符长度
        
        //模式类型设置
        m_pText->setInputMode(cocos2d::ui::EditBox::InputMode::DECIMAL);
        //    m_pText->setInputFlag(cocos2d::ui::EditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS);
        m_pText->setReturnType(EditBox::KeyboardReturnType::DONE);
        m_pText->setDelegate(this);
        m_pBg->addChild(m_pText);
        m_pText->setPosition(Vec2(230, 0)+m_pBg->getContentSize()*0.5);
        

    }), NULL));
    
}
void SetPriceLayer::onEnter()
{
    Layer::onEnter();
    
}

void SetPriceLayer::onExit()
{
    Layer::onExit();
}


void SetPriceLayer::onButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED){
        Button* pBtn = (Button*)ref;
        int tag = pBtn->getTag();
        float price = 0;
        if (tag==0) {
            
        }else{
            price = atof(m_pText->getText());
        }
        m_pBtnOK->runAction(ScaleTo::create(0.3, 0));
        m_pBtnCancel->runAction(ScaleTo::create(0.3, 0));
        ccBezierConfig cfg;
        cfg.controlPoint_1 = cfg.controlPoint_2 = Vec2(600, 400);
        cfg.endPosition = m_pos;
        m_pText->setTouchEnabled(false);
        m_pBg->runAction(Sequence::create(Spawn::create(BezierTo::create(1, cfg),
                                                        ScaleTo::create(0.8, 0.1), NULL),
                                          RotateBy::create(0.5, 360*2),
                                          CallFunc::create([=](){
            if(onPriceSetCallbak){
                onPriceSetCallbak(price);
            }
            this->removeFromParent();
        }),
                                          NULL));
    }
    
}
void SetPriceLayer::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidBegin.", editBox);
}

void SetPriceLayer::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

void SetPriceLayer::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text)
{
    log("editBox %p TextChanged, text: %s", editBox, text.c_str());
    bool isMatch = true;
    if(!pattern.empty()){
        std::regex _pattern(pattern);
        isMatch = regex_match(text,_pattern);
    }
    if(isMatch){
        if (text!=".") {
            float price = atof(text.c_str());
            if (price>=0) {
                currentTex  = text;
            }
        }
    }
    editBox->setText(currentTex.c_str());
}

void SetPriceLayer::editBoxReturn(cocos2d::extension::EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}