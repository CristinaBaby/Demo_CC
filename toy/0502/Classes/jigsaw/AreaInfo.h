//
//  AreaInfo.h
//  cocosTest
//
//  Created by zhangguangzong1 on 2/12/14.
//
//

#ifndef __cocosTest__AreaInfo__
#define __cocosTest__AreaInfo__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

/**
 *  每一块拼图块的信息类
 */
class AreaInfo{
    
public:
    AreaInfo();
public:
    /*拼图块的类型*/
    int mType;
    
    /**右边和底部的偏移
     * 切分图片的时候，按从左往右，从上往下的顺序切分；
     * 拼图块的上下左右4个方向都可能突出或凹陷，按切分顺序，我们只考虑右边和底部的情况；
     */
    float mOffsetRight;
    float mOffsetBottom;
    
    /**/
    float mWidth;
    float mHeight;
    
    /*判定区域,当拼图块中点移动到该区域且拼图块类型等于该区域类型时,将拼图放置在该区域*/
    Rect mJudgeArea;
    
    /*区域中心,相同类型的拼图块放置在此区域时,拼图块的中心应当与此点重合*/
    Vec2 mAreaCenter;
    
    /*当前区域所处整个图片区域的下标位置*/
    int mIndex;
    
    /*left top offset：这个点是用来偏移待切分的图片*/
    Vec2 mLTPoint;
    
    Vec2 mOffset;
};

#endif /* defined(__cocosTest__AreaInfo__) */
