//
//  ComponentDecoration.h
//  
//
//  Created by maxiang on 4/25/16.
//
//

#ifndef ComponentDecoration_h
#define ComponentDecoration_h

#include <stdio.h>
#include "component/makeComponent/basicComponent/ComponentBase.h"

/** @brief 此组件用于小装饰品的放大 缩小 旋转
 */
class ComponentDecoration : public ComponentBase
{
public:
    CREATE_FUNC(ComponentDecoration);
    
    /** @brief 装饰品上的边框
     */
    void setFrame(const std::string& frameFile);
    
    /** @brief 边框上的缩放按钮
     */
    void setZoomButtonTexture(const std::string& zoomFile);
    
    /** @brief 边框上的删除按钮
     */
    void setDeleteButtonTexture(const std::string& deleteFile);
    
    /* @brief 设置可移动区域, 默认为整个屏幕
     */
    void setMoveArea(const Rect& area);
    
    /* @brief 所有设置完成后调用
     */
    virtual void start() override;
    
    /* @brief 设置装饰框的可见性
     */
    void setFrameVisible(bool visible);
    
    /* @brief 选中回调
     */
    std::function<void()> onSelectCallback = nullptr;
    
    /* @brief 删除回调
     */
    std::function<void()> onDeleteCallback = nullptr;

public:
    ComponentDecoration():
    _frame(nullptr),
    _zoom(nullptr),
    _delete(nullptr),
    _originalRotation(1.0),
    _isZooming(false),
    _isMoving(false),
    _noTouchTime(0),
    _pauseTimer(false),
    _moveBoundingBox(Rect::ZERO),
    _eventListenerTouchOneByOne(nullptr)
    {};
    virtual ~ComponentDecoration();
    
    virtual bool init() override;
    virtual void onAdd() override;
    
protected:
    void hideFrameTimer(float time);

    void addTouchEventListener();
    void removeTouchEventListener();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
protected:
    cocos2d::Sprite  *_frame;
    cocos2d::Sprite  *_zoom;
    cocos2d::Sprite  *_delete;
    
    float _originalRotation;
    float _originalLenght;
    
    int _noTouchTime;
    
    bool _isZooming;
    bool _isMoving;
    bool _pauseTimer;
    
    cocos2d::Rect _moveBoundingBox;
    
    cocos2d::EventListenerTouchOneByOne *_eventListenerTouchOneByOne;
};

#endif /* ComponentDecoration_h */
