


#include "CursorTextField.h"

const static float DELTA = 0.5f;

CursorTextField::CursorTextField(void)
{
    TextFieldTTF();
    
    m_pCursorSprite = NULL;
    m_pCursorAction = NULL;
    
    m_pInputText = NULL;
}

CursorTextField::~CursorTextField(void)
{
    delete m_pInputText;
}

void CursorTextField::onEnter()
{
    TextFieldTTF::onEnter();
}

CursorTextField * CursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
    CursorTextField *pRet = new CursorTextField();
    
    if(pRet && pRet->initWithPlaceHolder("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->initCursorSprite(fontSize);
        
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void CursorTextField::initCursorSprite(int nHeight)
{
    // 初始化光标
    int column = 4;
    //int pixels[nHeight][column];
    int pixels[64][4];
    for (int i=0; i<nHeight; ++i) {
        for (int j=0; j<column; ++j) {
            pixels[i][j] = 0xffffffff;
        }
    }
    
    Texture2D *texture = new Texture2D();
    Size contentSize = Size(column, nHeight);
    texture->initWithData(pixels, sizeof(pixels), Texture2D::PixelFormat::RGBA8888, 1, 1, contentSize);
    
    m_pCursorSprite = Sprite::createWithTexture(texture);
    Size winSize = getContentSize();
    m_cursorPos = Vec2(0, winSize.height / 2);
    m_pCursorSprite->setPosition(m_cursorPos);
    this->addChild(m_pCursorSprite);
    
    m_pCursorAction = RepeatForever::create((ActionInterval *) Sequence::create(FadeOut::create(0.25f), FadeIn::create(0.25f), nullptr));
    
    m_pCursorSprite->runAction(m_pCursorAction);
    
    m_pInputText = new std::string();
    
    this->setDelegate(this);
    
    EventListenerTouchOneByOne *pEvent = EventListenerTouchOneByOne::create();
    pEvent->onTouchBegan = CC_CALLBACK_2(CursorTextField::ccTouchBegan, this);
    pEvent->onTouchEnded = CC_CALLBACK_2(CursorTextField::ccTouchEnded, this);
    pEvent->onTouchCancelled = CC_CALLBACK_2(CursorTextField::ccTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pEvent, this);
}

bool CursorTextField::ccTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    m_beginPos = pTouch->getLocation();
    
    return true;
}

Rect CursorTextField::getRect()
{
    Size size = getContentSize();
    
    return  Rect(-size.width / 2, -size.height / 2, size.width, size.height);
}

bool CursorTextField::isInTextField(cocos2d::Touch *pTouch)
{
    return getRect().containsPoint(convertTouchToNodeSpaceAR(pTouch));
}

void CursorTextField::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
    Vec2 endPos = pTouch->getLocation();
    
    // 判断是否为点击事件....
    if (::abs(endPos.x - m_beginPos.x) > DELTA ||
        ::abs(endPos.y - m_beginPos.y))
    {
        // 不是点击事件
        m_beginPos.x = m_beginPos.y = -1;
        
        return;
    }
    
    CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);
    
    // 判断是打开输入法还是关闭输入法
    isInTextField(pTouch) ? openIME() : closeIME();
}

bool CursorTextField::onTextFieldAttachWithIME(cocos2d::TextFieldTTF *pSender)
{
    if (m_pInputText->empty()) {
        return false;
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return false;
}

bool CursorTextField::onTextFieldInsertText(cocos2d::TextFieldTTF *pSender, const char *text, size_t nLen)
{
    CCLOG("Width: %f", pSender->getContentSize().width);
    CCLOG("Text: %s", text);
    CCLOG("Length: %zu", nLen);
    
    m_pInputText->append(text);
    setString(m_pInputText->c_str());
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return true;
}

bool CursorTextField::onTextFieldDeleteBackward(cocos2d::TextFieldTTF *pSender, const char *delText, size_t nLen)
{
    m_pInputText->resize(m_pInputText->size() - nLen);
    setString(m_pInputText->c_str());
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    if (m_pInputText->empty()) {
        m_pCursorSprite->setPositionX(0);
    }
    
    return false;
}

bool CursorTextField::onTextFieldDetachWithIME(cocos2d::TextFieldTTF *pSender)
{
    return false;
}

void CursorTextField::openIME()
{
    m_pCursorSprite->setVisible(true);
    this->attachWithIME();
}

void CursorTextField::closeIME()
{
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

void CursorTextField::onExit()
{
    TextFieldTTF::onExit();
}