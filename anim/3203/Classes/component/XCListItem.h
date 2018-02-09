//
// XCListItem.h
// SHAKE4040
//
// Created by dengcheng on 15/8/24.
//
//

#ifndef __SHAKE4040__XCListItem__
#define __SHAKE4040__XCListItem__

#include "cocos2d.h"

USING_NS_CC;

class XCListItem : public Node
{
public:
    XCListItem();
    ~XCListItem();

    static XCListItem* create(const std::string &aFileName);
    virtual bool init(const std::string &aFileName);
    
    inline int getID() {
        return m_nID;
    }
    
    inline int getThesholdsAlphaValue() {
        return m_nThresholdsAlpha;
    }
    
    inline void setResourcePath(const std::string &aName) {
        m_sResourcePath = aName;
    }
    
    inline const std::string &getResourcePath() {
        return m_sResourcePath;
    }
    
    inline const Vec2 &getTouchPosition() {
        return m_vTouchPos;
    }
    
    void setShowImage(const std::string &aFilePath);
    
    void showTouchStatus();
    
    void showDefualtStatus();
    
    /**
     *  设置点击透明像素阀值,范围0-255(默认为40)
     */
    void setThesholdsAlphaValue(const int &aAlpha);
    
    /**
     *  查询是否点击到item
     *
     *  @param aPos 世界坐标
     *
     *  @return 是否点击到
     */
    bool getTouchItem(const Vec2 &aPos);
    
protected://method

	void onEnter() override;

	void onExit() override;

private://Variables
    
    Image *m_pImage;
    
    Sprite *m_pItem;
    
    Sprite *m_pTouchShow;
    
    Sprite *m_pTouchShowImage;
    
    Vec2 m_vTouchPos;
    
    int m_nID;
    
    int m_nThresholdsAlpha;
    
    Size m_sContentSize;
    
    std::string m_sResourcePath;
    
private://method
    bool touchAtImageIsAlpha(const Vec2 &aPos);
};

#endif /* defined(__SHAKE4040__XCListItem__) */