#ifndef CODEBASE_CORE_COMMON_CMVISIBLERECT_H_
#define CODEBASE_CORE_COMMON_CMVISIBLERECT_H_

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
    static void getPosition(std::vector<Vec2>* vPosition,
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
    static void getPositionAdapted(std::vector<Vec2>* vPosition,
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
    static void getRectAdapted(std::vector<Rect>* vRect,
                               BorderType borderX = kBorderNone,
                               BorderType borderY = kBorderNone);
    
public:
	// real design size(bigger than the designed size of game)
	static const Size& getRealDesignSize();
	
	// the designed size of game
	static const Size& getDesignSize();
	
	// get frame size(can consider as screen size)
	static const Size& getFrameSize();
    
	// the offset between game designed size and real design size
	static const Vec2& getDesignOffset();
    
    //the center of design frame.
    static Vec2 getDesignCenter();
    
	// the ratio = RealDesignSize / DesignSize
	static float getRealDesignScale();
    
    //the center coordindate of the screen
    static Vec2 getCenterOfScene();
    
    /*获取一个在屏幕上（世界坐标系下）移动的矩形区域，区域边界是节点移动的最大范围*/
    static Rect getMoveRect(Node* apNode);
};

#endif // defined (CODEBASE_CORE_COMMON_CMVISIBLERECT_H_)
