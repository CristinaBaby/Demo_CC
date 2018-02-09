//
//  XCVisibleRect.h
//  WOLEGEQU
//
//  Created by dengcheng on 27/5/14.
//
//

#ifndef __WOLEGEQU__XCVisibleRect__
#define __WOLEGEQU__XCVisibleRect__

#include "cocos2d.h"

typedef enum {
    kVerticalNone,
    kTop,
    kBottom
}VerticalBorder;

typedef enum {
    kHorizontalNone,
    kLeft,
    kRight
}HorizontalBorder;

class XCVisibleRect {
    
public:
    
    /**
     *	@brief	设置分辨率大小a
     *
     *	@param 	frameSize 	设备分辨率
     *	@param 	designSize 	设计分辨率
     */
    static void setVisibleRect(const cocos2d::Size &frameSize, const cocos2d::Size &designSize);
    
    /**
     *	@brief	获取设计分辨率下的实际坐标
     *
     *	@param 	positionX 	X轴
     *	@param 	positionY 	Y轴
     *	@param 	kHorizontalNone 	X轴参照点
     *	@param 	kVerticalNone 	Y轴参照点
     *
     *	@return	实际坐标
     */
    static cocos2d::Point getPosition(float positionX, float positionY,
                                        HorizontalBorder xType = kHorizontalNone,
                                        VerticalBorder yType = kVerticalNone);
    /**
     *	@brief	获取设计分辨率下的实际坐标
     *
     *	@param 	position 	设计坐标
     *	@param 	kHorizontalNone 	X轴参照点
     *	@param 	kVerticalNone 	Y轴参照点
     *
     *	@return	实际坐标
     */
    static cocos2d::Point getPosition(cocos2d::Point position,
                                        HorizontalBorder xType = kHorizontalNone,
                                        VerticalBorder yType = kVerticalNone);
    
    /**
     *	@brief	获取实际设计分辨率
     *
     *	@return	实际设计分辨率
     */
    static const cocos2d::Size &getRealDesignSize();
    
    /**
     *	@brief	获取设计X轴的实际分辨率
     *
     *	@param 	positionX 	X轴
     *	@param 	kHorizontalNone 	参照点
     *
     *	@return	实际位置
     */
    static float getPositionX(float positionX, HorizontalBorder xType = kHorizontalNone);
    
    /**
     *	@brief	获取设计Y轴的实际分辨率
     *
     *	@param 	positionY 	Y轴
     *	@param 	kVerticalNone 	参照点
     *
     *	@return	实际位置
     */
    static float getPositionY(float positionY, VerticalBorder yType = kVerticalNone);
    
    /**
     *	@brief	获取中点实际坐标
     *
     *	@return	中点
     */
    static cocos2d::Point getMidPoint();
    
private:
    /**
     *	@brief	计算偏移量
     */
    static void calculateOffset();
    
};

#endif
