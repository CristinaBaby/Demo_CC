//
//  CBPageControl.h
//  ColorBook
//
//  Created by maxiang on 4/23/15.
//
//

#ifndef __ColorBook__CBPageControl__
#define __ColorBook__CBPageControl__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"


class PageControl : public cocos2d::Layer
{
public:
    static PageControl* create(const std::string& normalImage, const std::string& selectImage);
    bool init(const std::string& normalImage, const std::string& selectImage);
    
    void setPagesCount(const int pagesCount);
    void setSelectPage(const int index);
    
protected:
    std::string _normalImageName;
    std::string _selectImageName;
    int _pagesCount;
    cocos2d::Sprite *_selectedControl;
};



#endif /* defined(__ColorBook__CBPageControl__) */
