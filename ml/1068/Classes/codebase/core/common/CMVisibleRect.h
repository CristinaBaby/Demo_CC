#ifndef _CMVISIBLE_H_
#define _CMVISIBLE_H_

#include "cocos2d.h"

USING_NS_CC;

enum BorderType
{
    kBorderNone         =       1 << 0,
    kBorderLeft         =       1 << 1,
    kBorderRight        =       1 << 2,
    kBorderTop          =       1 << 3,
    kBorderBottom       =       1 << 4,
    kNoOffset           =       1 << 5,
    kNoOffsetX          =       1 << 6,/*以前有的版本有这个参数，以免报错加上，实际没用到*/
    kNoOffsetY          =       1 << 7,/*以前有的版本有这个参数，以免报错加上，实际没用到*/
};

class CMVisibleRect
{
public:
    // 初始化分辨率适配，在 AppDelegate 中调用
    static void setupVisibleRect(float width, float height);
public:
    /*以下方法传入node坐标在参数为边距时，传入的坐标做边距计算*/
    /**
     * @brief   设置 node 的坐标
     * @param   (x, y) == position     设置的坐标
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    传入node坐标在参数为边距时，传入的坐标做边距计算
     */
    static void setPosition(Node* node, float x, float y,
                            BorderType borderX = kBorderNone,
                            BorderType borderY = kBorderNone);
    static void setPosition(Node* node, Vec2 position,
                            BorderType borderX = kBorderNone,
                            BorderType borderY = kBorderNone);
    
    static Vec2 getPosition(float x, float y,
                            BorderType borderX = kBorderNone,
                            BorderType borderY = kBorderNone);
    static void getPosition(std::vector<Vec2>& vPosition,
                            BorderType borderX = kBorderNone,
                            BorderType borderY = kBorderNone);
public:
    /*以下方法传入node坐标皆以左下角为准*/
    /**
     * @brief   设置 node 的坐标
     * @param   (x, y) == position     设置的坐标
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    此方法传入node坐标皆以左下角为准
     */
    static void setPositionAdapted(Node* node, float x, float y,
                                   BorderType borderX = kBorderNone,
                                   BorderType borderY = kBorderNone);
    static void setPositionAdapted(Node* node, Vec2 position,
                                   BorderType borderX = kBorderNone,
                                   BorderType borderY = kBorderNone);
    
    /**
     * @brief   获取适配后的真实坐标
     * @param   position、vPosition    传入的坐标或一组坐标
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    此方法传入node坐标皆以左下角为准
     */
    static Vec2 getPositionAdapted(Vec2 position,
                                   BorderType borderX = kBorderNone,
                                   BorderType borderY = kBorderNone);
    static void getPositionAdapted(std::vector<Vec2>& vPosition,
                                                BorderType borderX = kBorderNone,
                                                BorderType borderY = kBorderNone);
    
    /**
     * @brief   传入在设计尺寸下的rect,获取适配后的Rect
     * @param   (x, y, width, height) == rect、vRect     传入的Rect或一组Rect
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     */
    static Rect getRectAdapted(float x, float y, float width, float height,
                               BorderType borderX = kBorderNone,
                               BorderType borderY = kBorderNone);
    static Rect getRectAdapted(Rect rect,
                               BorderType borderX = kBorderNone,
                               BorderType borderY = kBorderNone);
    static void getRectAdapted(std::vector<Rect>& vRect,
                               BorderType borderX = kBorderNone,
                               BorderType borderY = kBorderNone);
    
public:
	// real design size(bigger than the designed size of game)
	static Size& getRealDesignSize();
	
	// the designed size of game
	static Size& getDesignSize();
    
    //the center of design frame.
    static Vec2 getDesignCenter();
	
	// get frame size(can consider as screen size)
	static Size& getFrameSize();
    
	// the offset between game designed size and real design size
	static Vec2 getDesignOffset();

	// the ratio = RealDesignSize / DesignSize
	static float getRealDesignScale();
    
    //the center coordindate of the screen
    static Vec2 getCenterOfScene();
    
    /*获取一个在屏幕上（世界坐标系下）移动的矩形区域，区域边界是节点移动的最大范围*/
    static Rect getMoveRect(Node* apNode);
public: // 原来的代码，以后应该废弃的
    /*use setupVisibleRect(float width, float height) instead*/
    CC_DEPRECATED_ATTRIBUTE static void setupVisibleRect(const Size& frameSize, const Size& designSize)
    {
        CMVisibleRect::setupVisibleRect(designSize.width, designSize.height);
    }
    CC_DEPRECATED_ATTRIBUTE static void setPositionST(Node* node, float x, float y, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    CC_DEPRECATED_ATTRIBUTE static void setPositionST(Node* node, Vec2 pt, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    CC_DEPRECATED_ATTRIBUTE static Vec2 getRealPositionForDesignPosition(Vec2 aPosition, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    CC_DEPRECATED_ATTRIBUTE static std::vector<Vec2> getRealPositionForDesignPosition(std::vector<Vec2>& aPositiones, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    CC_DEPRECATED_ATTRIBUTE static Vec2 getRealPoint(float x, float y,  bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    // 获取转换到ST适配上的Rect
    CC_DEPRECATED_ATTRIBUTE static Rect getRealRect(Rect pRect, bool pTop = false, bool pBottom = false, bool pLeft = false, bool pRight = false);
    CC_DEPRECATED_ATTRIBUTE static Rect getRealRect(float x, float y, float width, float height, bool pTop = false, bool pBottom = false, bool pLeft = false, bool pRight = false);
    CC_DEPRECATED_ATTRIBUTE static std::vector<Rect> getRealRects(std::vector<Rect>& aPositiones, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
};

CC_DEPRECATED_ATTRIBUTE inline void _G_SetPositon(Node* node,float x, float y,BorderType borderX = kBorderNone,BorderType borderY = kBorderNone)
{
    CMVisibleRect::setPosition(node, x, y, borderX, borderY);
}

CC_DEPRECATED_ATTRIBUTE typedef CMVisibleRect KSVisibleRect;
CC_DEPRECATED_ATTRIBUTE typedef CMVisibleRect STVisibleRect;
#endif
