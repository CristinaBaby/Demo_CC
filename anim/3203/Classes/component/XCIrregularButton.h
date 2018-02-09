/*----------------------------------------------------------------
 // Copyright (C) 公司名称 2014年 __MyCompanyName__. All rights reserved.
 //
 // 文件名：XCIrregularButton
 // 文件功能描述：
 //
 //
 // 创建标识：dengcheng on 11/11/14.
 //
 // 修改标识：
 // 修改描述：
 //
 // 修改标识：
 // 修改描述：
 //---------------------------------------------------------------
 */

#ifndef __MONS4035__XCIrregularButton__
#define __MONS4035__XCIrregularButton__

#include "cocos2d.h"
USING_NS_CC;

class XCIrregularButton : public Sprite
{
public:
    
    XCIrregularButton();
    ~XCIrregularButton();
    
    static XCIrregularButton *create(std::string pName);
    virtual bool initWithXCIrregularButton(std::string pName);
    
    void setEventCallBack(std::function<void(Node *)> callBack);
    
    /**
     *  关闭触摸
     *
     *  @param true
     */
    void closeTouch(bool close = true);
    
    /**
     *  设置触摸按钮透明阀值
     *
     *  @param aAlpha 最大不能超过255
     */
    void setTouchAlphaValue(unsigned char aAlpha);
    
protected://Variables
    
protected://method
    
private://Variables
    
    unsigned char m_cMinAlpha;
    
    bool m_bIstouch;
    
    Image *m_pImage;
    
    std::function<void(Node *)> m_fCallback;
    
    bool m_bClose;
    
private://method
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    unsigned char getAlpha(const Vec2 &aPos);
    
};

#endif