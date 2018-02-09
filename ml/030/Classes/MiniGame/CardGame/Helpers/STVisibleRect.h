#ifndef _STVISIBLE_H_
#define _STVISIBLE_H_

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

class STVisibleRect
{
public:
    // 初始化分辨率适配，在 AppDelegate 中调用
    static void setupVisibleRect(float width, float height);
    
public:     // 整理过后的部分
    /**
     * @brief   设置 node 的坐标
     * @param   (x, y) == position     设置的坐标
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    此处与 AdapterScreen 这个文件略有不同，node坐标皆以左下角为准
     */
    static void setPositionAdapted(Node* node, float x, float y, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    static void setPositionAdapted(Node* node, Vec2 position, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    
    /**
     * @brief   获取适配后的真实坐标
     * @param   position、vPosition    传入的坐标或一组坐标
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    此处与 AdapterScreen 这个文件略有不同
     */
    static Vec2 getPositionAdapted(Vec2 position, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    static std::vector<Vec2> getPositionAdapted(std::vector<Vec2>& vPosition, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    
    /**
     * @brief   获取适配后的Rect
     * @param   (x, y, width, height) == rect、vRect     传入的Rect或一组Rect
     * @param   borderX     Horizontal Layout
     * @param   borderY     Vertical Layout
     * @note    此处与 AdapterScreen 这个文件略有不同
     */
    static Rect getRectAdapted(float x, float y, float width, float height, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    static Rect getRectAdapted(Rect rect, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    static std::vector<Rect> getRectAdapted(std::vector<Rect>& vRect, BorderType borderX = kBorderNone, BorderType borderY = kBorderNone);
    
public:     // 原来保留的，还未修改
	// initialization offset for STVisibleRect
	static void initRetainOffset(int left, int top, int right, int bottom);
    
    static int getRetainOffsetTop();

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
    static Vec2 getPointOfSceneLeftBottom();
    static Vec2 getPointOfSceneRightUp();
    static Vec2 getPointOfSceneUp();
    
    //限制node底部不超出屏幕底部的区域
    static Rect getBottomMoveRect(Node* apNode);
    //限制node顶部不超出屏幕顶部的区域
    static Rect getTopMoveRect(Node* apNode);
    //限制node左部不超出屏幕左部的区域
    static Rect getLeftMoveRect(Node* apNode);
    //限制node右部不超出屏幕右部的区域
    static Rect getRightMoveRect(Node* apNode);
    
public: // 原来的代码，以后应该废弃的
    static void setPosition(Node* node, float x, float y, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    static void setPosition(Node* node, Vec2 pt, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    static Vec2 getRealPositionForDesignPosition(Vec2 aPosition, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    static std::vector<Vec2> getRealPositionForDesignPosition(std::vector<Vec2>& aPositiones, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
    
    // 获取转换到ST适配上的Rect
    static Rect getRealRect(Rect pRect, bool pTop = false, bool pBottom = false, bool pLeft = false, bool pRight = false);
    static Rect getRealRect(float x, float y, float width, float height, bool pTop = false, bool pBottom = false, bool pLeft = false, bool pRight = false);
    static std::vector<Rect> getRealRects(std::vector<Rect>& aPositiones, bool bTop = false, bool bBottom = false, bool bLeft = false, bool bRight = false);
};

//NS_ST_END
#endif
