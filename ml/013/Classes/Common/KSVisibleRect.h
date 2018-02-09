#ifndef _KIDS_VISIBLE_H_
#define _KIDS_VISIBLE_H_

#include "cocos2d.h"


USING_NS_CC;

typedef enum
{
    kBorderNone     = 0 << 0,
    kBorderTop      = 1 << 1,
    kBorderBottom   = 1 << 2,
    kBorderLeft     = 1 << 3,
    kBorderRight    = 1 << 4,
    kNoOffsetX      = 1 << 5,
    kNoOffsetY      = 1 << 6,
}BorderType;

class KSVisibleRect {
public:
	// initialization for KSVisibleRect
	static void setupVisibleRect(const Size& frameSize, const Size& designSize);
    
	static void setupDesignRect(const Size& frameSize, const Size& designSize);

    static void calculateOffset();
    
	// initialization offset for KSVisibleRect
	static void initRetainOffset(int left, int top, int right, int bottom);

	// real design size(bigger than the designed size of game)
	static Size& getRealDesignSize();
	
	// the designed size of game
	static Size& getDesignSize();
	
	// get frame size(can consider as screen size)
	static Size& getFrameSize();

	// the margin of top and left according to frame size
//	static Point& getTopLeftOffset();

	// the offset between game designed size and real design size
	static Point& getDesignOffset();

	// the ratio = RealDesignSize / DesignSize
	static float getRealDesignScale();

	// adjust CCNode's position(attention:top & bottom feature)
//	static void setPosition(CCNode* node, float x, float y, bool bTop = false, bool bBottom = false);

	static void setPosition(Node* node, float x, float y,
                            BorderType xBorder = kBorderNone,
                            BorderType yBorder = kBorderNone);
    
    static Point getPosition(float x, float y,
                               BorderType xBorder = kBorderNone,
                               BorderType yBorder = kBorderNone);
    
	// adjust CCNode's position
	static void setPositionWithRetainOffset(Node* node, float x, float y, bool bTop = false, bool bBottom = false);
    
    static bool isPortrait(const Size& frameSize);
    
    static void setPortraitSize(const Size& frameSize , Size designSize);
    
    static void setLandSpaceSize(const Size& frameSize , Size designSize);
    
};


#endif