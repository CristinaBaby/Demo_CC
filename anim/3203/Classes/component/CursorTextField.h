/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：CursorTextField
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 20/4/15.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __PizzaHouseStory__CursorTextField__
#define __PizzaHouseStory__CursorTextField__

#include "cocos2d.h"
USING_NS_CC;

class CursorTextField : public TextFieldTTF, public TextFieldDelegate
{
private:
    // 点击开始位置
    Point m_beginPos;
    
    // 光标精灵
    Sprite *m_pCursorSprite;
    
    // 光标动画
    Action *m_pCursorAction;
    
    // 光标坐标
    Point m_cursorPos;
    
    // 输入框内容
    std::string *m_pInputText;
    
    
public:
    CursorTextField(void);
    virtual ~CursorTextField(void);
    
    // static
    static CursorTextField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
    // CCLayer
    void onEnter();
    void onExit();
    
    // 初始化光标精灵
    void initCursorSprite(int nHeight);
    
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(TextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(TextFieldTTF * pSender, const char * text, size_t nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * pSender, const char * delText, size_t nLen);
    
    // CCLayer Touch
    bool ccTouchBegan(Touch *pTouch, Event *pEvent);
    void ccTouchEnded(Touch *pTouch, Event *pEvent);
    
    // 判断是否点击在TextField处
    bool isInTextField(Touch *pTouch);
    // 得到TextField矩形
    Rect getRect();
    
    // 打开输入法
    void openIME();
    // 关闭输入法
    void closeIME();
    
};

#endif