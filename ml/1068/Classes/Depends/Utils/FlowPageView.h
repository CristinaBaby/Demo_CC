
#ifndef __FlowPageView__
#define __FlowPageView__


#include "cocos2d.h"
#include "ui/UIPageView.h"

using namespace cocos2d::ui;
USING_NS_CC;

class FlowPageView : public PageView
{
public:
    FlowPageView();
    ~FlowPageView();
    
    /**
     * Allocates and initializes.
     */
    static FlowPageView* create();
    
    virtual bool init();
    
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
    /**
     * scroll pageview to index.
     *
     * @param idx    index of page.
     */
    void scrollToPage(ssize_t idx);
    
    void setPageSize(Size size){
        m_PageSize = size;
    }
    void setMinScale(float scale){
        m_fMinScale = scale;
    }
    void setMaxScale(float scale){
        m_fMaxScale = scale;
    }
    void setScrollSpeed(float speed){
        m_fScrollSpeed = speed;
    }
    void setScollByPage(bool page){
        m_bIsScrollByPage = page;
    }
    
    void setStartPointOffset(Vec2 point){startPointOffset = point;}
    
    void scrollToPageAnimation(int index,float duration);
    virtual void update(float dt) override;

    void refresh();
    bool isScrolled(){return m_bIsScrolling;}
    std::function<void(int pageIndex)> scrollEnd;
protected:
    virtual void doLayout();
    virtual bool scrollPages(Vec2 touchOffset);
    virtual void updateAllPagesSize();
    virtual void updateAllPagesPosition();
    virtual void handleReleaseLogic(Touch *touch) ;
    void autoScroll(float dt);
    
    Vec2 startPointOffset;
    Size m_PageSize;
    float m_fMinScale;
    float m_fMaxScale;
    float m_fScrollSpeed;
    bool m_bIsScrolling;
    bool m_bIsScrollByPage;
};

#endif