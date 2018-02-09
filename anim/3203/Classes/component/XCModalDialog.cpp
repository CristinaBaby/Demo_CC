


#include "XCModalDialog.h"
#include "XCVisibleRect.h"

//窗口资源配置
#define kDialogBgFile "ui/dialog/popup_box.png"  //背景图
#define kDialogButtonOne_d "ui/dialog/popup_btn_yes.png" //第一个按钮的默认图
#define kDialogButtonTwo_d "ui/dialog/popup_btn_no.png" //第二个按钮的默认图

const static int s_gModalZorder = 10000;

XCModalDialog *XCModalDialog::g_sDialog = nullptr;

XCModalDialog::XCModalDialog()
:m_fBackgroundColorDepth(0.75)
{
    
}

bool XCModalDialog::initWithXCModalDialog(const std::string &aContent, std::function<void(ButtonTag)> aFunc)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, m_fBackgroundColorDepth * 255)) ) {
        return false;
    }
    
    m_Func = aFunc;
    
    m_pBg = Sprite::create(kDialogBgFile);
    m_pBg->setPosition(XCVisibleRect::getPosition(320, 480));
    addChild(m_pBg);
    
    Button *pButtonOne = Button::create(kDialogButtonOne_d);
    pButtonOne->setPosition(Vec2(m_pBg->getContentSize().width * 0.75, m_pBg->getContentSize().height * 0.2));
    pButtonOne->addTouchEventListener(CC_CALLBACK_2(XCModalDialog::onButtonCallback, this));
    pButtonOne->setTag(BUTTON_1);
    m_pBg->addChild(pButtonOne);
    
    Button *pButtonTwo = Button::create(kDialogButtonTwo_d);
    pButtonTwo->setPosition(Vec2(m_pBg->getContentSize().width * 0.25, m_pBg->getContentSize().height * 0.2));
    pButtonTwo->addTouchEventListener(CC_CALLBACK_2(XCModalDialog::onButtonCallback, this));
    pButtonTwo->setTag(BUTTON_2);
    m_pBg->addChild(pButtonTwo);
    
    Size BoxSize = m_pBg->getContentSize();
    m_pContent = Label::createWithTTF("fonts/Marker Felt.ttf", aContent);
    m_pContent->setWidth(BoxSize.width * 0.9);
    m_pContent->setSystemFontSize(30);
    m_pContent->setPosition(BoxSize.width * 0.5, BoxSize.height * 0.7);
    m_pContent->setAlignment(TextHAlignment::CENTER);
    m_pBg->addChild(m_pContent);
    
    auto pEvent = EventListenerTouchOneByOne::create();
    pEvent->setSwallowTouches(true);
    pEvent->onTouchBegan = [](Touch*, Event*)->bool{
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pEvent, this);
    
    return true;
}

void XCModalDialog::show(const std::string &aContent, std::function<void(ButtonTag)> aFunc)
{
    
    if (g_sDialog == nullptr) {
        g_sDialog = new XCModalDialog();
        g_sDialog->initWithXCModalDialog(aContent, aFunc);
    } else {
        
        if (g_sDialog->getParent()) {
            return;
        }
        
        g_sDialog->m_Func = aFunc;
        g_sDialog->m_pContent->setString(aContent);
    }
    
    g_sDialog->m_pBg->setScale(0.1);
    g_sDialog->m_pBg->runAction(EaseBackOut::create(ScaleTo::create(0.3, 1)));
    Director::getInstance()->getRunningScene()->addChild(g_sDialog, s_gModalZorder);
}

void XCModalDialog::onButtonCallback(Ref *aBtn, Widget::TouchEventType aType)
{
    if (aType != Widget::TouchEventType::ENDED) {
        return;
    }
    ButtonTag tag = static_cast<ButtonTag>(dynamic_cast<Button *>(aBtn)->getTag());
    m_Func(tag);
    removeFromParentAndCleanup(true);
}

XCModalDialog::~XCModalDialog()
{
    
}