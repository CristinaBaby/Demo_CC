//
//  XCScrollView.h
//  PizzaHouseStory
//
//  Created by 邓承 on 4/9/15.
//
//

#ifndef __PizzaHouseStory__XCScrollView__
#define __PizzaHouseStory__XCScrollView__

#include "cocos2d.h"

USING_NS_CC;

class XCScrollView;

class XCScrollViewDelegate
{
public:
    /**
     * @js NA
     * @lua NA
     */
    virtual ~XCScrollViewDelegate() {}
    /**
     * @js NA
     * @lua NA
     */
    virtual void scrollViewDidScroll(XCScrollView* view) {};
    /**
     * @js NA
     * @lua NA
     */
    virtual void scrollViewDidZoom(XCScrollView* view) {};
};

class XCScrollView : public Layer, public ActionTweenDelegate
{
public:
    enum class Direction
    {
        NONE = -1,
        HORIZONTAL = 0,
        VERTICAL,
        BOTH
    };
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static XCScrollView* create(Size size, Node* container = NULL);
    
    /**
     * Returns an autoreleased scroll view object.
     *
     * @return autoreleased scroll view object
     */
    static XCScrollView* create();
    /**
     * @js ctor
     */
    XCScrollView();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~XCScrollView();
    
    bool init();
    /**
     * Returns a scroll view object
     *
     * @param size view size
     * @param container parent object
     * @return scroll view object
     */
    bool initWithViewSize(Size size, Node* container = NULL);
    
    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     *
     * @param offset    The new offset.
     * @param animated  If true, the view will scroll to the new offset.
     */
    virtual void setContentOffset(Vec2 offset, bool animated = false);
    Vec2 getContentOffset();
    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     * You can override the animation duration with this method.
     *
     * @param offset    The new offset.
     * @param dt        The animation duration.
     */
    virtual void setContentOffsetInDuration(Vec2 offset, float dt);
    
    void setZoomScale(float s);
    /**
     * Sets a new scale and does that for a predefined duration.
     *
     * @param s         The new scale vale
     * @param animated  If true, scaling is animated
     */
    void setZoomScale(float s, bool animated);
    
    float getZoomScale();
    
    /**
     * Sets a new scale for container in a given duration.
     *
     * @param s     The new scale value
     * @param dt    The animation duration
     */
    void setZoomScaleInDuration(float s, float dt);
    /**
     * Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
     */
    Vec2 minContainerOffset();
    /**
     * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
     */
    Vec2 maxContainerOffset();
    /**
     * Determines if a given node's bounding box is in visible bounds
     *
     * @returns true if it is in visible bounds
     */
    bool isNodeVisible(Node * node);
    /**
     * Provided to make scroll view compatible with SWLayer's pause method
     */
    void pause(Ref* sender);
    /**
     * Provided to make scroll view compatible with SWLayer's resume method
     */
    void resume(Ref* sender);
    
    void setTouchEnabled(bool enabled);
    bool isTouchEnabled() const;
    bool isDragging() const {return _dragging;}
    bool isTouchMoved() const { return _touchMoved; }
    bool isBounceable() const { return _bounceable; }
    void setBounceable(bool bBounceable) { _bounceable = bBounceable; }
    
    /**
     * size to clip. Node boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */
    Size getViewSize() const { return _viewSize; }
    void setViewSize(Size size);
    
    Node * getContainer();
    void setContainer(Node * pContainer);
    
    /**
     * direction allowed to scroll. ScrollViewDirectionBoth by default.
     */
    Direction getDirection() const { return _direction; }
    virtual void setDirection(Direction eDirection) { _direction = eDirection; }
    /**
     * @js NA
     * @lua NA
     */
    XCScrollViewDelegate* getDelegate() { return _delegate; }
    /**
     * @code
     * when this function bound to js or lua,the input param are changed
     * in js: var setDelegate(var jsObject)
     * in lua: local setDelegate()
     * @endcode
     */
    void setDelegate(XCScrollViewDelegate* pDelegate) { _delegate = pDelegate; }
    
    void updateInset();
    
    /**
     * Determines whether it clips its children or not.
     */
    bool isClippingToBounds() { return _clippingToBounds; }
    void setClippingToBounds(bool bClippingToBounds) { _clippingToBounds = bClippingToBounds; }
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
    // Overrides
    virtual void setContentSize(const Size & size) override;
    virtual const Size& getContentSize() const override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    
    using Node::addChild;
    virtual void addChild(Node * child, int zOrder, int tag) override;
    virtual void addChild(Node * child, int zOrder, const std::string &name) override;
    
    /**
     * CCActionTweenDelegate
     */
    void updateTweenAction(float value, const std::string& key);
protected:
    /**
     * Relocates the container at the proper offset, in bounds of max/min offsets.
     *
     * @param animated If true, relocation is animated
     */
    void relocateContainer(bool animated);
    /**
     * implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
     * deacceleration speed. it must be less than 1.0f.
     *
     * @param dt delta
     */
    virtual void deaccelerateScrolling(float dt);
    /**
     * This method makes sure auto scrolling causes delegate to invoke its method
     */
    void performedAnimatedScroll(float dt);
    /**
     * Expire animated scroll delegate calls
     */
    void stoppedAnimatedScroll(Node* node);
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    void onBeforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    void onAfterDraw();
    /**
     * Zoom handling
     */
    void handleZoom();
    
    Rect getViewRect();
    
    /**
     * current zoom scale
     */
    float _zoomScale;
    /**
     * min zoom scale
     */
    float _minZoomScale;
    /**
     * max zoom scale
     */
    float _maxZoomScale;
    /**
     * scroll view delegate
     */
    XCScrollViewDelegate* _delegate;
    
    Direction _direction;
    /**
     * If YES, the view is being dragged.
     */
    bool _dragging;
    
    /**
     * Content offset. Note that left-bottom point is the origin
     */
    Vec2 _contentOffset;
    
    /**
     * Container holds scroll view contents, Sets the scrollable container object of the scroll view
     */
    Node* _container;
    /**
     * Determiens whether user touch is moved after begin phase.
     */
    bool _touchMoved;
    /**
     * max inset point to limit scrolling by touch
     */
    Vec2 _maxInset;
    /**
     * min inset point to limit scrolling by touch
     */
    Vec2 _minInset;
    /**
     * Determines whether the scroll view is allowed to bounce or not.
     */
    bool _bounceable;
    
    bool _clippingToBounds;
    
    /**
     * scroll speed
     */
    Vec2 _scrollDistance;
    /**
     * Touch point
     */
    Vec2 _touchPoint;
    /**
     * length between two fingers
     */
    float _touchLength;
    /**
     * Touch objects to detect multitouch
     */
    std::vector<Touch*> _touches;
    /**
     * size to clip. Node boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */
    Size _viewSize;
    /**
     * max and min scale
     */
    float _minScale, _maxScale;
    /**
     * scissor rect for parent, just for restoring GL_SCISSOR_BOX
     */
    Rect _parentScissorRect;
    bool _scissorRestored;
    
    float m_nMoveStepX;
    
    float m_nMoveStepY;
    
    /** Touch listener */
    EventListenerTouchOneByOne* _touchListener;
    
    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;
};

#endif /* defined(__PizzaHouseStory__XCScrollView__) */
