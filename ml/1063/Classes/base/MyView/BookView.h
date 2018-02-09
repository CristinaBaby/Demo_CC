//
//  BookView.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/18.
//
//

#ifndef BookView_hpp
#define BookView_hpp

#include <iostream>
#include "cocos2d.h"
#include "ViewAdapter.h"
using namespace std;
USING_NS_CC;
class BookPage: public Node {
protected:
    CC_SYNTHESIZE(int, _pageIndex, PageIndex)
public:
    BookPage():
    leftPage(nullptr)
    ,rightPage(nullptr)
    ,_pageIndex(-1)
    ,currentPage(nullptr)
    ,scaleEnd(nullptr)
    ,isScaleEnd(true){
        
    }
    
    static BookPage* create(const Size& size){
        BookPage * page =  new BookPage();
        if(page&&page->init()){
            page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            page->setContentSize(size);
            page->autorelease();
            return page;
        }
        CC_SAFE_RELEASE_NULL(page);
        return nullptr;
    }
    
    void addLeftPage(Node* pNode){
        pNode->retain();
        pNode->removeFromParent();
        if(leftPage != nullptr)
            leftPage->removeFromParent();
        leftPage = pNode;
        addChild(leftPage);
        pNode->release();
        leftPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        leftPage->setPosition(getContentSize()*.5);
    }
    
    void addRightPage(Node* pNode) {
        pNode->retain();
        pNode->removeFromParent();
        if(rightPage != nullptr)
            rightPage->removeFromParent();
        rightPage = pNode;
        addChild(rightPage);
        pNode->release();
        rightPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        rightPage->setPosition(getContentSize()*.5);
    }
    
    Node* getLeftPage() {
        return leftPage;
    }
    
    Node* getRightPage() {
        return rightPage;
    }
    
    void CaculateCurrentPage(Vec2 world){
        currentPage = nullptr;
        if(leftPage != nullptr&&leftPage->isVisible()){
            auto _pos = leftPage->convertToNodeSpace(world);
            Rect limit;
            limit.size = leftPage->getContentSize();
            if(limit.containsPoint(_pos))
                currentPage = leftPage;
            
        }
        if(currentPage == nullptr&& rightPage != nullptr&&rightPage->isVisible()){
            auto _pos = rightPage->convertToNodeSpace(world);
            Rect limit;
            limit.size = rightPage->getContentSize();
            if(limit.containsPoint(_pos))
                currentPage = rightPage;
        }
    }
    void scaleCurrentPage(const Vec2& modeDeta){
        scheduleUpdate();
        float scaleDeta = modeDeta.x/(getContentSize().width*.3);
        float newScale;
        if(currentPage == leftPage)
            newScale = currentPage->getScaleX()-scaleDeta;
        else
            newScale = currentPage->getScaleX()+scaleDeta;
        if(newScale>=1){
            newScale = 1;
            isScaleEnd = true;
        }
        if(newScale<=0){
            newScale = 0;
            currentPage->setVisible(false);
            if(currentPage == leftPage)
                currentPage = rightPage;
            else
                currentPage = leftPage;
        }
        if(currentPage != nullptr){
            currentPage->setScaleX(newScale);
            currentPage->setVisible(true);
        }
        if(isScaleEnd&&scaleEnd&&newScale>=1){
            unscheduleUpdate();
            currentPage->setSkewY(0);
            scaleEnd();
        }
    
    }
    Node* getCurrentPage(){
        return currentPage;
    }
    void clearCurrentPage(){
        currentPage = nullptr;
    }
    bool IsScaleEnd(){return isScaleEnd;}
    void setScaleEnd(bool flag ){isScaleEnd = flag;}
    void update(float){
        if(currentPage != nullptr) {
            float scaleX = currentPage->getScaleX();
            float distance = 10;
            if(currentPage == leftPage)
                distance = -10;
            if(currentPage->getScaleX()>0.5)
                currentPage->setSkewY((1-scaleX)/0.5*distance);
            else
                currentPage->setSkewY(distance);
        }
    }
protected:
    Node* leftPage,*rightPage;
    Node* currentPage;
    bool isScaleEnd;
public:
    std::function<void()> scaleEnd;
};

class BookPageStruct {
public:
    BookPageStruct():
    prePage(nullptr)
    ,nextPage(nullptr)
    ,data(nullptr){
        
    }
    BookPageStruct* prePage,*nextPage;
    BookPage* data;
    
};

class BookView : public Node {
protected:
    
public:
    BookView();
    ~BookView();
    static BookView* create(const Size&);
    void setAdapter(ViewAdapter*);
    BookPageStruct* getPage(){return pageControl;}
protected:
    ViewAdapter* _adapter;
    BookPageStruct *pageControl;
    
    bool caculateTouchPage(BookPageStruct *control, Vec2 wordpos);
    BookPage* getPage(size_t _index);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancle(Touch *pTouch, Event *pEvent);
    void onEnterTransitionDidFinish();
    EventListenerTouchOneByOne* _lis;
    void clearPage();
    void turnUpdate(float);
    Vec2 accel;
    bool pageChanged;
    bool isMove;
public:
    std::function<void()> pageMove;
};

#endif /* BookView_hpp */
