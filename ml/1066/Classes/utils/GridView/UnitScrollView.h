//
//  UnitScrollView.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-11-5.
//
//

#ifndef __OreoMaker__UnitScrollView__
#define __OreoMaker__UnitScrollView__

#include <iostream>
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

using namespace cocos2d::extension;
USING_NS_CC;
using namespace std;

class UnitScrollView:public KDGridView{
private:
    UnitScrollView();
    
    UnitScrollView(const std::string& path, const std::string& lock, const std::string& unitBg);
    
    ~UnitScrollView();
    
public:
    virtual bool init(Size size);
    
    static UnitScrollView* create(Size size, const std::string& path, const std::string& lock, const std::string& unitBg = "");
    
private:
    bool initUI();
    
    void _onTypePageCallback(Ref* sender, Control::EventType controlEvent);
    
public:
    void loadingUnitScrollViewWithType(int index = 0, bool isPng = true);
    
    std::string getDecType();
    
    void setOffsetX(float x);
    void setOffsetY(float y);
    
    void setClickEffect(int index, bool isPng = true);
    void setIsNeedClickEffect(bool isNeed);
    
    void setScaleValue(float value = 1.0f);
    
private:
    Vector<Sprite*> m_vDecorates;
    Vector<Sprite*> m_vDecorates_h;
    
    KSSimpleAdapter* m_pAdapter;
    
    string m_pType;
    
    std::string m_sDecorationImagesPath;
    
    std::string m_sUnitBgPath;
    
    std::string m_sLockPath;
    
    float m_fOffsetX;
    float m_fOffsetY;
    
    bool m_bIsClickEffect;
    
    float m_fScale;
};

#endif /* defined(__OreoMaker__UnitScrollView__) */
