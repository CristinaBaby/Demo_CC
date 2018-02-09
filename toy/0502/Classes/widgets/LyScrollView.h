//
//  LyScrollView.h
//  my_demo
//
//  Created by liyang1 on 9/28/14.
//
//

#ifndef my_demo_LyScrollView_h
#define my_demo_LyScrollView_h

#include "cocos2d.h"
#include <GUI/CCScrollView/CCScrollView.h>
//#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LyScrollView  : public ScrollView {

public :
    bool isSurrportZoom ;
    float zoomScale;
    bool isDragContent ;  //是否选中滑动条内的子节点
    float separationSpace ; //start 对 start的间距
    float topDistrict;
    LyScrollView();
    ~LyScrollView();
    
    static  LyScrollView* create();
    static  LyScrollView* create(Size size, Node* container , float scale , float separationSpace);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

};

#endif
