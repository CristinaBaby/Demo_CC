
#ifndef DecorationUtil_h
#define DecorationUtil_h

#include "cocos2d.h"
#include "FillMaterialModel.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC_EXT;
USING_NS_CC;
class DecorationUtil :public Node
{
public:
    typedef enum {
        NONE,
        MOVE,
        SCALE,
        ROTATE,
    } EventType;

    DecorationUtil();
    ~DecorationUtil();
    
    static DecorationUtil* createDefault(Size size);
    static DecorationUtil* create(Size size,std::string pathFrame = "",std::string pathZoom = "",std::string pathDelete = "");
    virtual bool init(Size size,std::string pathFrame,std::string pathZoom,std::string pathDelete);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    virtual void onEnter();
    virtual void onExit();
    virtual void onExitTransitionDidStart();
    
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);
    virtual void updateFrameStatus(float);
    virtual void updateFrameStatusToVisible();
    
    void setIgnoreTip(bool ignore = true)
    {
        m_bIgnoreTip = ignore;
        updateFrameStatus(0);
        m_eEventType = EventType::MOVE;
    }
    void setMoveLimitRect(Rect rect)
    {
        m_LimitRect = rect;
    }
    std::function<void()> utilDeletFuncCallback;
    
    Scale9Sprite* m_pFrame; //框
    Button* m_pBtnDelet;  //删除按钮
    Sprite* m_pZoom;  //缩放旋转
    
    EventType m_eEventType;
    
    string m_sFramePath; //框的图片路径
    string m_sZoomPath; //缩放旋转的图片路径
    
    Rect m_LimitRect;  //可移动范围
    
    bool m_bFrameShowing; //框／缩放旋转／关闭按钮处于显示状态
    
    bool m_bIsShotScreen;  //截屏  （框 删除按钮等不能截图进去）
    
    bool m_bIgnoreTip;  //忽略提示框
    
    float m_fOrignalRotate; //初始状态的旋转角度
    int m_nFrameSpace;
    
};
#endif /* DecorationUtil_h */
